//============================================================================
//
//背景の処理[background.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "background.h"	//作成したbackground.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBg[MAX_BG] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;
Background g_aBg[MAX_BG];	//背景の情報

//============================================================================
//背景の初期化処理
//============================================================================
void InitBg(void)
{
	int nCntBg;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

		//テクスチャの読み込み1
		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\sky000.jpg",	//テクスチャのファイル名
			&g_pTextureBg[0]
		);

		//テクスチャの読み込み2
		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\wood.png",	//テクスチャのファイル名
			&g_pTextureBg[1]
		);

		//テクスチャの読み込み1
		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\frame_score.png",	//テクスチャのファイル名
			&g_pTextureBg[2]
		);

		//テクスチャの読み込み2
		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\frame_timer001.png",	//テクスチャのファイル名
			&g_pTextureBg[3]
		);

		//構造体の初期化処理
		g_aBg[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 , 2.0f);
		g_aBg[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[0].nPatternAnim = 0;
		g_aBg[0].fAngle = atan2f(SCREEN_WIDTH, SCREEN_HEIGHT);
		g_aBg[0].fLength = sqrtf(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT) / 2.0f;

		g_aBg[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT /2, 1.0f);
		g_aBg[1].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[1].nPatternAnim = 0;
		g_aBg[1].fAngle = atan2f(SCREEN_WIDTH, SCREEN_HEIGHT);
		g_aBg[1].fLength = sqrtf(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT) / 2.0f;

		g_aBg[2].pos = D3DXVECTOR3(BG_SCORE_WIDTH / 2, BG_SCORE_HEIGHT / 2, 2.0f);
		g_aBg[2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[2].nPatternAnim = 0;
		g_aBg[2].fAngle = atan2f(BG_SCORE_WIDTH, BG_SCORE_HEIGHT);
		g_aBg[2].fLength = sqrtf(BG_SCORE_WIDTH * BG_SCORE_WIDTH + BG_SCORE_HEIGHT * BG_SCORE_HEIGHT) / 2.0f;

		g_aBg[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, BG_TIMER_HEIGHT / 2, 1.0f);
		g_aBg[3].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[3].nPatternAnim = 0;
		g_aBg[3].fAngle = atan2f(BG_TIMER_WIDTH, BG_TIMER_HEIGHT);
		g_aBg[3].fLength = sqrtf(BG_TIMER_WIDTH * BG_TIMER_WIDTH + BG_TIMER_HEIGHT * BG_TIMER_HEIGHT) / 2.0f;
	
	//頂点情報の設定
	VERTEX_2D *pVtx;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aBg[nCntBg].pos.x - sinf(g_aBg[nCntBg].fAngle + g_aBg[nCntBg].rot.x) * g_aBg[nCntBg].fLength;
		pVtx[0].pos.y = g_aBg[nCntBg].pos.y - cosf(g_aBg[nCntBg].fAngle + g_aBg[nCntBg].rot.y) * g_aBg[nCntBg].fLength;
		pVtx[0].pos.z = g_aBg[nCntBg].pos.z + 0.0f;

		pVtx[1].pos.x = g_aBg[nCntBg].pos.x + sinf(g_aBg[nCntBg].fAngle - g_aBg[nCntBg].rot.x) * g_aBg[nCntBg].fLength;
		pVtx[1].pos.y = g_aBg[nCntBg].pos.y - cosf(g_aBg[nCntBg].fAngle - g_aBg[nCntBg].rot.y) * g_aBg[nCntBg].fLength;
		pVtx[1].pos.z = g_aBg[nCntBg].pos.z + 0.0f;

		pVtx[2].pos.x = g_aBg[nCntBg].pos.x - sinf(g_aBg[nCntBg].fAngle - g_aBg[nCntBg].rot.x) * g_aBg[nCntBg].fLength;
		pVtx[2].pos.y = g_aBg[nCntBg].pos.y + cosf(g_aBg[nCntBg].fAngle - g_aBg[nCntBg].rot.y) * g_aBg[nCntBg].fLength;
		pVtx[2].pos.z = g_aBg[nCntBg].pos.z + 0.0f;

		pVtx[3].pos.x = g_aBg[nCntBg].pos.x + sinf(g_aBg[nCntBg].fAngle + g_aBg[nCntBg].rot.x) * g_aBg[nCntBg].fLength;
		pVtx[3].pos.y = g_aBg[nCntBg].pos.y + cosf(g_aBg[nCntBg].fAngle + g_aBg[nCntBg].rot.y) * g_aBg[nCntBg].fLength;
		pVtx[3].pos.z = g_aBg[nCntBg].pos.z + 0.0f;

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
	g_pVtxBuffBg->Unlock();
}

//============================================================================
//背景の終了処理
//============================================================================
void UninitBg(void)
{
	int nCntBg;

	for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		//テクスチャの破棄
		if (g_pTextureBg[nCntBg] != NULL)
		{
			g_pTextureBg[nCntBg]->Release();
			g_pTextureBg[nCntBg] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//============================================================================
//背景の更新処理
//============================================================================
void UpdateBg(void)
{
	//頂点情報の設定
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//アニメーションの処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	g_aBg[0].nPatternAnim++;			//パターンNo.を更新する

	if (g_aBg[0].nPatternAnim >= BG_ANIM_CNT)
	{//総パターン数を超えた時の処理
		g_aBg[0].nPatternAnim = 0;	//パターンNo.を初期値に戻す
	}

	//テクスチャ座標の設定:X座標 = (1.0f / xパターン数) * (パターンNo. % xパターン数), Y座標 = (1.0f / yパターン数) * (パターンNo. / xパターン数) : int型なので少数点は反映されない
	pVtx[0].tex = D3DXVECTOR2((1.0f / BG_ANIM_CNT) * (g_aBg[0].nPatternAnim % BG_ANIM_CNT + 0.0f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / BG_ANIM_CNT) * (g_aBg[0].nPatternAnim % BG_ANIM_CNT + 1.0f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / BG_ANIM_CNT) * (g_aBg[0].nPatternAnim % BG_ANIM_CNT + 0.0f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / BG_ANIM_CNT) * (g_aBg[0].nPatternAnim % BG_ANIM_CNT + 1.0f), 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

//============================================================================
//背景の描画処理
//============================================================================
void DrawBg(void)
{
	int nCntBg;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBg,			//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBg[nCntBg]);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntBg * 4,				//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}