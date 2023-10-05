//============================================================================
//
//白の処理[white.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "main.h"		//作成したmain.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "white.h"		//作成したwhite.hをインクルードする
#include "game.h"		//作成したgame.hをインクルードする
#include "fade.h"		//作成したfade.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWhite = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWhite = NULL;		//頂点バッファへのポインタ
White g_aWhite[MAX_WHITE_DIGIT];					//点数桁毎の情報
int g_nWhite;										//白の値

//============================================================================
//白の初期化処理
//============================================================================
void InitWhite(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWhite;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\number000.png",			//テクスチャのファイル名
		&g_pTextureWhite
	);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//構造体の初期化処理
		g_aWhite[nCntWhite].pos = D3DXVECTOR3(SCREEN_WIDTH * 2 / 3 + (nCntWhite + 1.0f) * WHITE_WIDTH, WHITE_HEIGHT, 0.0f);		//位置の初期化処理
		g_aWhite[nCntWhite].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//移動量の初期化処理
		g_aWhite[nCntWhite].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//向きを初期化する
		g_aWhite[nCntWhite].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					//スコアの初期配色
		g_aWhite[nCntWhite].fAngle = atan2f(WHITE_WIDTH, WHITE_HEIGHT);
		g_aWhite[nCntWhite].fLength = sqrtf(WHITE_WIDTH * WHITE_WIDTH + WHITE_HEIGHT * WHITE_HEIGHT) / 2.0f;
	}
	//初期スコア
	g_nWhite = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_WHITE_DIGIT,	//sizeof(VERTEX_2D) * 必要な頂点数 * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffWhite,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWhite->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aWhite[nCntWhite].pos.x - sinf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[0].pos.y = g_aWhite[nCntWhite].pos.y - cosf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[0].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[1].pos.x = g_aWhite[nCntWhite].pos.x + sinf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[1].pos.y = g_aWhite[nCntWhite].pos.y - cosf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[1].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[2].pos.x = g_aWhite[nCntWhite].pos.x - sinf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[2].pos.y = g_aWhite[nCntWhite].pos.y + cosf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[2].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[3].pos.x = g_aWhite[nCntWhite].pos.x + sinf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[3].pos.y = g_aWhite[nCntWhite].pos.y + cosf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[3].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / BASE_WHITE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / BASE_WHITE_NUMBER, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffWhite->Unlock();
}

//============================================================================
//白の終了処理
//============================================================================
void UninitWhite(void)
{

	//テクスチャの破棄
	if (g_pTextureWhite != NULL)
	{
		g_pTextureWhite->Release();
		g_pTextureWhite = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffWhite != NULL)
	{
		g_pVtxBuffWhite->Release();
		g_pVtxBuffWhite = NULL;
	}
}

//============================================================================
//白の更新処理
//============================================================================
void UpdateWhite(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntWhite;

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D *pVtx;

	FADE fade;
	fade = GetFade();

	if (g_nWhite == 0)
	{//ゲームクリア
		if (fade == FADE_NONE)
		{//フェードが行われてない場合の処理
			SetGameState(GAMESTATE_GAMECLEAR);
		}
	}

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWhite->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aWhite[nCntWhite].pos.x - sinf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[0].pos.y = g_aWhite[nCntWhite].pos.y - cosf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[0].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[1].pos.x = g_aWhite[nCntWhite].pos.x + sinf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[1].pos.y = g_aWhite[nCntWhite].pos.y - cosf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[1].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[2].pos.x = g_aWhite[nCntWhite].pos.x - sinf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[2].pos.y = g_aWhite[nCntWhite].pos.y + cosf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[2].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[3].pos.x = g_aWhite[nCntWhite].pos.x + sinf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[3].pos.y = g_aWhite[nCntWhite].pos.y + cosf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[3].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffWhite->Unlock();
}

//============================================================================
//白の描画処理
//============================================================================
void DrawWhite(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntWhite;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWhite,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWhite);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntWhite * 4,			//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}

//============================================================================
////白の設定処理
//============================================================================
void SetWhite(int nWhite)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntWhite;
	int aPosTexU[MAX_WHITE_DIGIT];	//各桁の数字を格納

								//デバイスの取得
	pDevice = GetDevice();

	g_nWhite += nWhite;

	if (g_nWhite >= 99999999)
	{
		g_nWhite = 99999999;
	}

	aPosTexU[0] = g_nWhite % 10000 / 1000;
	aPosTexU[1] = g_nWhite % 1000 / 100;
	aPosTexU[2] = g_nWhite % 100 / 10;
	aPosTexU[3] = g_nWhite % 10 / 1;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWhite->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 1.0f / BASE_WHITE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 1.0f / BASE_WHITE_NUMBER, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWhite->Unlock();
}

//============================================================================
////白の加算処理
//============================================================================
void AddWhite(int nValue)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntWhite;
	int aPosTexU[MAX_WHITE_DIGIT];	//各桁の数字を格納

								//デバイスの取得
	pDevice = GetDevice();

	g_nWhite -= nValue;

	if (g_nWhite >= 99999999)
	{
		g_nWhite = 99999999;
	}

	aPosTexU[0] = g_nWhite % 10000 / 1000;
	aPosTexU[1] = g_nWhite % 1000 / 100;
	aPosTexU[2] = g_nWhite % 100 / 10;
	aPosTexU[3] = g_nWhite % 10 / 1;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWhite->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 1.0f / BASE_WHITE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 1.0f / BASE_WHITE_NUMBER, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWhite->Unlock();
}