//============================================================================
//
//ライフの処理[life.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "life.h"		//作成したlife.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする

LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	//頂点バッファへのポインタ
Life g_aLife[MAX_PLAYER_LIFE];						//ライフの情報

//============================================================================
//ライフの初期化処理
//============================================================================
void InitLife(void)
{
	int nCntLife;
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ

	pDevice = GetDevice();				//デバイスの取得

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\life000.png",	//テクスチャのファイル名
		&g_pTextureLife
	);
	//ライフの情報の初期化
	for (nCntLife = 0; nCntLife < MAX_PLAYER_LIFE; nCntLife++)
	{
		//位置の初期化処理
		g_aLife[nCntLife].pos = D3DXVECTOR3(SCREEN_WIDTH + (LIFE_WIDTH * nCntLife) - (LIFE_WIDTH * PLAYER_LIFE),
			LIFE_HEIGHT, 0.0f);		//位置
		g_aLife[nCntLife].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//回転
		g_aLife[nCntLife].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色
		g_aLife[nCntLife].fAngle = atan2f(LIFE_WIDTH, LIFE_HEIGHT);
		g_aLife[nCntLife].fLength = sqrtf(LIFE_WIDTH * LIFE_WIDTH + LIFE_HEIGHT * LIFE_HEIGHT) / 2.0f;
		g_aLife[nCntLife].bUse = true;								//ライフ使用判定
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PLAYER_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLife = 0; nCntLife < MAX_PLAYER_LIFE; nCntLife++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aLife[nCntLife].pos.x - sinf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[0].pos.y = g_aLife[nCntLife].pos.y - cosf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[0].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[1].pos.x = g_aLife[nCntLife].pos.x + sinf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[1].pos.y = g_aLife[nCntLife].pos.y - cosf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[1].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[2].pos.x = g_aLife[nCntLife].pos.x - sinf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[2].pos.y = g_aLife[nCntLife].pos.y + cosf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[2].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[3].pos.x = g_aLife[nCntLife].pos.x + sinf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[3].pos.y = g_aLife[nCntLife].pos.y + cosf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[3].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}

//============================================================================
//ライフの終了処理
//============================================================================
void UninitLife(void)
{
	//テクスチャの破棄
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//============================================================================
//ライフの更新処理
//============================================================================
void UpdateLife(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//追加されたライフを表示する処理
	for (int nCntAddLife = 0; nCntAddLife < (pPlayer->nLife / PLAYER_LIFE) + 1 ; nCntAddLife++)
	{
		for (int nCntTreeLife = 0; nCntTreeLife < PLAYER_LIFE; nCntTreeLife++)
		{
			g_aLife[(nCntAddLife * PLAYER_LIFE) + nCntTreeLife].pos = D3DXVECTOR3(SCREEN_WIDTH - (LIFE_WIDTH * nCntTreeLife) - (LIFE_WIDTH * PLAYER_LIFE),
				LIFE_HEIGHT * (nCntAddLife + 1), 0.0f);
			g_aLife[(nCntAddLife * PLAYER_LIFE) + nCntTreeLife].bUse = true;
		}
	}
	for (int nCntLife = 0; nCntLife < MAX_PLAYER_LIFE; nCntLife++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aLife[nCntLife].pos.x - sinf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[0].pos.y = g_aLife[nCntLife].pos.y - cosf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[0].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[1].pos.x = g_aLife[nCntLife].pos.x + sinf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[1].pos.y = g_aLife[nCntLife].pos.y - cosf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[1].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[2].pos.x = g_aLife[nCntLife].pos.x - sinf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[2].pos.y = g_aLife[nCntLife].pos.y + cosf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[2].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[3].pos.x = g_aLife[nCntLife].pos.x + sinf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[3].pos.y = g_aLife[nCntLife].pos.y + cosf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[3].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx += 4;
	}
	//ライフが減った分だけfalseにする
	for (int nCntPLife = pPlayer->nLife; nCntPLife < MAX_PLAYER_LIFE; nCntPLife++)
	{
		g_aLife[nCntPLife].bUse = false;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
	
}
//============================================================================
//ライフの描画処理
//============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntLife;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffLife,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	for (nCntLife = 0; nCntLife < MAX_PLAYER_LIFE; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == true)
		{//弾が使用されている 
		 //ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntLife * 4,			//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}
}