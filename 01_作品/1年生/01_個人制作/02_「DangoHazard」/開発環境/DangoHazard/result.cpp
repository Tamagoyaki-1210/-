//============================================================================
//
//リザルト画面の処理[result.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "input.h"		//作成したinput.hをインクルードする
#include "result.h"		//作成したresult.hをインクルードする
#include "fade.h"		//作成したfade.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_IMAGE];	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		//頂点バッファへのポインタ
Result g_aResult[MAX_RESULT_IMAGE];						//リザルト画面の情報

int g_nCounterResultState = 0;						//状態管理カウンター

//============================================================================
//リザルト画面の初期化処理
//============================================================================
void InitResult(void)
{
	int nCntResult;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//テクスチャのファイル名
		&g_pTextureResult[0]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\press_enter.png",	//テクスチャのファイル名
		&g_pTextureResult[1]
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\GAMECLEAR.png",	//テクスチャのファイル名
		&g_pTextureResult[2]
	);

	//構造体の初期化処理
	g_aResult[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);	//位置の初期化処理
	g_aResult[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//向きを初期化する
	g_aResult[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aResult[0].fAngle = atan2f(RESULT_BG_WIDTH, RESULT_BG_HEIGHT);
	g_aResult[0].fLength = sqrtf(RESULT_BG_WIDTH * RESULT_BG_WIDTH + RESULT_BG_HEIGHT * RESULT_BG_HEIGHT) / 2.0f;

	g_aResult[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.25f, 0.0f);	//位置の初期化処理
	g_aResult[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//向きを初期化する
	g_aResult[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aResult[1].fAngle = atan2f(RESULT_ENTER_WIDTH, RESULT_ENTER_HEIGHT);
	g_aResult[1].fLength = sqrtf(RESULT_ENTER_WIDTH *RESULT_ENTER_WIDTH + RESULT_ENTER_HEIGHT * RESULT_ENTER_HEIGHT) / 2.0f;

	g_aResult[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);	//位置の初期化処理
	g_aResult[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//向きを初期化する
	g_aResult[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aResult[2].fAngle = atan2f(RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT);
	g_aResult[2].fLength = sqrtf(RESULT_LOGO_WIDTH * RESULT_LOGO_WIDTH + RESULT_LOGO_HEIGHT * RESULT_LOGO_HEIGHT) / 2.0f;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_RESULT_IMAGE,	//sizeof(VERTEX_2D) * 必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResult = 0; nCntResult < MAX_RESULT_IMAGE; nCntResult++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aResult[nCntResult].pos.x - sinf(g_aResult[nCntResult].fAngle + g_aResult[nCntResult].rot.x) * g_aResult[nCntResult].fLength;
		pVtx[0].pos.y = g_aResult[nCntResult].pos.y - cosf(g_aResult[nCntResult].fAngle + g_aResult[nCntResult].rot.y) * g_aResult[nCntResult].fLength;
		pVtx[0].pos.z = g_aResult[nCntResult].pos.z + 0.0f;

		pVtx[1].pos.x = g_aResult[nCntResult].pos.x + sinf(g_aResult[nCntResult].fAngle - g_aResult[nCntResult].rot.x) * g_aResult[nCntResult].fLength;
		pVtx[1].pos.y = g_aResult[nCntResult].pos.y - cosf(g_aResult[nCntResult].fAngle - g_aResult[nCntResult].rot.y) * g_aResult[nCntResult].fLength;
		pVtx[1].pos.z = g_aResult[nCntResult].pos.z + 0.0f;

		pVtx[2].pos.x = g_aResult[nCntResult].pos.x - sinf(g_aResult[nCntResult].fAngle - g_aResult[nCntResult].rot.x) * g_aResult[nCntResult].fLength;
		pVtx[2].pos.y = g_aResult[nCntResult].pos.y + cosf(g_aResult[nCntResult].fAngle - g_aResult[nCntResult].rot.y) * g_aResult[nCntResult].fLength;
		pVtx[2].pos.z = g_aResult[nCntResult].pos.z + 0.0f;

		pVtx[3].pos.x = g_aResult[nCntResult].pos.x + sinf(g_aResult[nCntResult].fAngle + g_aResult[nCntResult].rot.x) * g_aResult[nCntResult].fLength;
		pVtx[3].pos.y = g_aResult[nCntResult].pos.y + cosf(g_aResult[nCntResult].fAngle + g_aResult[nCntResult].rot.y) * g_aResult[nCntResult].fLength;
		pVtx[3].pos.z = g_aResult[nCntResult].pos.z + 0.0f;

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}

//============================================================================
//リザルト画面の終了処理
//============================================================================
void UninitResult(void)
{
	int nCntResult;

	for (nCntResult = 0; nCntResult < MAX_RESULT_IMAGE; nCntResult++)
	{
		//テクスチャの破棄
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//============================================================================
//リザルト画面の更新処理
//============================================================================
void UpdateResult(void)
{
	FADE fade;			
	fade = GetFade();

	if (fade == FADE_NONE)
	{//フェードが行われてない場合の処理
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//Enterキー又はスペースキーが押された場合
		 //モード設定
			SetFade(MODE_TITLE);
		}
	}
}

//============================================================================
//リザルト画面の描画処理
//============================================================================
void DrawResult(void)
{
	int nCntResult;
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffResult,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntResult = 0; nCntResult < MAX_RESULT_IMAGE; nCntResult++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[nCntResult]);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			4 * nCntResult,			//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}