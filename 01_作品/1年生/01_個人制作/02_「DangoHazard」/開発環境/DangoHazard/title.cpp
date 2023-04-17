//============================================================================
//
//タイトルの処理[title.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "main.h"		//作成したmain.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "title.h"		//作成したtitle.hをインクルードする
#include "fade.h"		//作成したfade.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE_IMAGE];	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			//頂点バッファへのポインタ
Title g_aTitle[MAX_TITLE_IMAGE];						//タイトルの情報

int g_nCounterTitleState = 0;							//状態管理カウンター
bool TitleCounterUse = false;

//============================================================================
//タイトルの初期化処理
//============================================================================
void InitTitle(void)
{
	int nCntTitle;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\dangohazard.png",			//テクスチャのファイル名
		&g_pTextureTitle[0]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\press_enter.png",	//テクスチャのファイル名
		&g_pTextureTitle[1]
	);

	//タイトル_背景
	g_aTitle[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1.0f);		//位置の初期化処理
	g_aTitle[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//回転を初期化する
	g_aTitle[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aTitle[0].fAngle = atan2f(TITLE_BG_WIDTH, TITLE_BG_HEIGHT);
	g_aTitle[0].fLength = sqrtf(TITLE_BG_WIDTH * TITLE_BG_WIDTH + TITLE_BG_HEIGHT * TITLE_BG_HEIGHT) / 2.0f;

	//タイトル_ENTER
	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.25f, 0.0f);	//位置の初期化処理
	g_aTitle[1].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//回転を初期化する
	g_aTitle[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aTitle[1].fAngle = atan2f(TITLE_ENTER_WIDTH, TITLE_ENTER_HEIGHT);
	g_aTitle[1].fLength = sqrtf(TITLE_ENTER_WIDTH * TITLE_ENTER_WIDTH + TITLE_ENTER_HEIGHT * TITLE_ENTER_HEIGHT) / 2.0f;

	//タイトル_ロゴ
	g_aTitle[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f, 0.0f);	//位置の初期化処理
	g_aTitle[2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//回転を初期化する
	g_aTitle[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aTitle[2].fAngle = atan2f(TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT);
	g_aTitle[2].fLength = sqrtf(TITLE_LOGO_WIDTH * TITLE_LOGO_WIDTH + TITLE_LOGO_HEIGHT * TITLE_LOGO_HEIGHT) * 2 / 3;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_TITLE_IMAGE,	//sizeof(VERTEX_2D) * 必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_IMAGE; nCntTitle++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aTitle[nCntTitle].pos.x - sinf(g_aTitle[nCntTitle].fAngle + g_aTitle[nCntTitle].rot.x) * g_aTitle[nCntTitle].fLength;
		pVtx[0].pos.y = g_aTitle[nCntTitle].pos.y - cosf(g_aTitle[nCntTitle].fAngle + g_aTitle[nCntTitle].rot.y) * g_aTitle[nCntTitle].fLength;
		pVtx[0].pos.z = g_aTitle[nCntTitle].pos.z + 0.0f;

		pVtx[1].pos.x = g_aTitle[nCntTitle].pos.x + sinf(g_aTitle[nCntTitle].fAngle - g_aTitle[nCntTitle].rot.x) * g_aTitle[nCntTitle].fLength;
		pVtx[1].pos.y = g_aTitle[nCntTitle].pos.y - cosf(g_aTitle[nCntTitle].fAngle - g_aTitle[nCntTitle].rot.y) * g_aTitle[nCntTitle].fLength;
		pVtx[1].pos.z = g_aTitle[nCntTitle].pos.z + 0.0f;

		pVtx[2].pos.x = g_aTitle[nCntTitle].pos.x - sinf(g_aTitle[nCntTitle].fAngle - g_aTitle[nCntTitle].rot.x) * g_aTitle[nCntTitle].fLength;
		pVtx[2].pos.y = g_aTitle[nCntTitle].pos.y + cosf(g_aTitle[nCntTitle].fAngle - g_aTitle[nCntTitle].rot.y) * g_aTitle[nCntTitle].fLength;
		pVtx[2].pos.z = g_aTitle[nCntTitle].pos.z + 0.0f;

		pVtx[3].pos.x = g_aTitle[nCntTitle].pos.x + sinf(g_aTitle[nCntTitle].fAngle + g_aTitle[nCntTitle].rot.x) * g_aTitle[nCntTitle].fLength;
		pVtx[3].pos.y = g_aTitle[nCntTitle].pos.y + cosf(g_aTitle[nCntTitle].fAngle + g_aTitle[nCntTitle].rot.y) * g_aTitle[nCntTitle].fLength;
		pVtx[3].pos.z = g_aTitle[nCntTitle].pos.z + 0.0f;

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
	g_pVtxBuffTitle->Unlock();
}

//============================================================================
//タイトルの終了処理
//============================================================================
void UninitTitle(void)
{
	int nCntTitle;

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_IMAGE; nCntTitle++)
	{
		//テクスチャの破棄
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//============================================================================
//タイトルの更新処理
//============================================================================
void UpdateTitle(void)
{
	FADE fade;
	fade = GetFade();

	if (fade == FADE_NONE)
	{//フェードが行われてない場合の処理
		 //Enterキー又はスペースキーが押された場合
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//モード設定
			SetFade(MODE_GAME);

			TitleCounterUse = true;
		}
	}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Enterが押された後の秒数を数える処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (g_nCounterTitleState >= 60)
	{//60f(フレーム)後の処理
		TitleCounterUse = false;
		g_nCounterTitleState = 0;
	}
	else if (TitleCounterUse == true)
	{//Enterが押されてからの時間を数える処理
		g_nCounterTitleState++;
	}
}

//============================================================================
//タイトルの描画処理
//============================================================================
void DrawTitle(void)
{
	int nCntTitle;
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffTitle,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_IMAGE; nCntTitle++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			4 * nCntTitle,			//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}