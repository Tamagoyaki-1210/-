//============================================================================
//
//矢印の処理[arrow.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "arrow.h"		//作成したlife.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "item.h"		//作成したitem.hをインクルードする
#include "enemy.h"		//作成したenemy.hをインクルードする

LPDIRECT3DTEXTURE9 g_pTextureArrow = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffArrow = NULL;	//頂点バッファへのポインタ
ARROW g_aArrow[MAX_ARROW];						//矢印の情報

//============================================================================
//矢印の初期化処理
//============================================================================
void InitArrow(void)
{
	int nCntArrow;
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ

	pDevice = GetDevice();				//デバイスの取得

										//テクスチャーの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Arrow.png",	//テクスチャのファイル名
		&g_pTextureArrow
	);
		//位置の初期化処理
		g_aArrow[0].pos = D3DXVECTOR3(ARROW_WIDTH / 2, ARROW_HEIGHT / 2 * 3, 0.5f);		//位置
		g_aArrow[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//回転
		g_aArrow[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色
		g_aArrow[0].fAngle = atan2f(ARROW_WIDTH, ARROW_HEIGHT);
		g_aArrow[0].fLength = sqrtf(ARROW_WIDTH * ARROW_WIDTH + ARROW_HEIGHT * ARROW_HEIGHT) / 2.0f;
		g_aArrow[0].bUse = true;								//ライフ使用判定

		g_aArrow[1].pos = D3DXVECTOR3(SCREEN_WIDTH - ARROW_WIDTH / 2, ARROW_HEIGHT / 2 * 3, 0.5f);		//位置
		g_aArrow[1].rot = D3DXVECTOR3(3.14f, 3.14f, 0.0f);		//回転
		g_aArrow[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色
		g_aArrow[1].fAngle = atan2f(ARROW_WIDTH, ARROW_HEIGHT);
		g_aArrow[1].fLength = sqrtf(ARROW_WIDTH * ARROW_WIDTH + ARROW_HEIGHT * ARROW_HEIGHT) / 2.0f;
		g_aArrow[1].bUse = true;								//ライフ使用判定

		g_aArrow[2].pos = D3DXVECTOR3(ARROW_WIDTH / 2, SCREEN_HEIGHT - ARROW_HEIGHT / 2, 0.5f);		//位置
		g_aArrow[2].rot = D3DXVECTOR3(3.14f, 3.14f, 0.0f);		//回転
		g_aArrow[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色
		g_aArrow[2].fAngle = atan2f(ARROW_WIDTH, ARROW_HEIGHT);
		g_aArrow[2].fLength = sqrtf(ARROW_WIDTH * ARROW_WIDTH + ARROW_HEIGHT * ARROW_HEIGHT) / 2.0f;
		g_aArrow[2].bUse = true;								//ライフ使用判定

		g_aArrow[3].pos = D3DXVECTOR3(SCREEN_WIDTH - ARROW_WIDTH / 2, SCREEN_HEIGHT - ARROW_HEIGHT / 2, 0.5f);		//位置
		g_aArrow[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//回転
		g_aArrow[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//色
		g_aArrow[3].fAngle = atan2f(ARROW_WIDTH, ARROW_HEIGHT);
		g_aArrow[3].fLength = sqrtf(ARROW_WIDTH * ARROW_WIDTH + ARROW_HEIGHT * ARROW_HEIGHT) / 2.0f;
		g_aArrow[3].bUse = true;								//ライフ使用判定

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ARROW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffArrow,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aArrow[nCntArrow].pos.x - sinf(0.0f + g_aArrow[nCntArrow].fAngle + g_aArrow[nCntArrow].rot.x) * g_aArrow[nCntArrow].fLength;
		pVtx[0].pos.y = g_aArrow[nCntArrow].pos.y - cosf(0.0f + g_aArrow[nCntArrow].fAngle + g_aArrow[nCntArrow].rot.y) * g_aArrow[nCntArrow].fLength;
		pVtx[0].pos.z = g_aArrow[nCntArrow].pos.z + 0.0f;

		pVtx[1].pos.x = g_aArrow[nCntArrow].pos.x + sinf(0.0f + g_aArrow[nCntArrow].fAngle - g_aArrow[nCntArrow].rot.x) * g_aArrow[nCntArrow].fLength;
		pVtx[1].pos.y = g_aArrow[nCntArrow].pos.y - cosf(0.0f + g_aArrow[nCntArrow].fAngle - g_aArrow[nCntArrow].rot.y) * g_aArrow[nCntArrow].fLength;
		pVtx[1].pos.z = g_aArrow[nCntArrow].pos.z + 0.0f;

		pVtx[2].pos.x = g_aArrow[nCntArrow].pos.x - sinf(0.0f + g_aArrow[nCntArrow].fAngle - g_aArrow[nCntArrow].rot.x) * g_aArrow[nCntArrow].fLength;
		pVtx[2].pos.y = g_aArrow[nCntArrow].pos.y + cosf(0.0f + g_aArrow[nCntArrow].fAngle - g_aArrow[nCntArrow].rot.y) * g_aArrow[nCntArrow].fLength;
		pVtx[2].pos.z = g_aArrow[nCntArrow].pos.z + 0.0f;

		pVtx[3].pos.x = g_aArrow[nCntArrow].pos.x + sinf(0.0f + g_aArrow[nCntArrow].fAngle + g_aArrow[nCntArrow].rot.x) * g_aArrow[nCntArrow].fLength;
		pVtx[3].pos.y = g_aArrow[nCntArrow].pos.y + cosf(0.0f + g_aArrow[nCntArrow].fAngle + g_aArrow[nCntArrow].rot.y) * g_aArrow[nCntArrow].fLength;
		pVtx[3].pos.z = g_aArrow[nCntArrow].pos.z + 0.0f;

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
	g_pVtxBuffArrow->Unlock();
}

//============================================================================
//矢印の終了処理
//============================================================================
void UninitArrow(void)
{
	//テクスチャの破棄
	if (g_pTextureArrow != NULL)
	{
		g_pTextureArrow->Release();
		g_pTextureArrow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffArrow != NULL)
	{
		g_pVtxBuffArrow->Release();
		g_pVtxBuffArrow = NULL;
	}
}

//============================================================================
//矢印の更新処理
//============================================================================
void UpdateArrow(void)
{}
//============================================================================
//矢印の描画処理
//============================================================================
void DrawArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntArrow;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffArrow,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureArrow);

	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_aArrow[nCntArrow].bUse == true)
		{//弾が使用されている 
		 //ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntArrow * 4,			//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}
}