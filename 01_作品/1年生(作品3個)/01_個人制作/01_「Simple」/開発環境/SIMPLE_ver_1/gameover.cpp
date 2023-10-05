//============================================================================
//
//ゲームオーバーの処理[gameover.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "main.h"		//作成したmain.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "gameover.h"	//作成したgameover.hをインクルードする
#include "fade.h"		//作成したfade.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGameOver[MAX_GAMEOVER_IMAGE];	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameOver = NULL;			//頂点バッファへのポインタ
GAMEOVER g_aGameOver[MAX_GAMEOVER_IMAGE];					//タイトルの情報

int g_nCounterGameOverState = 0;							//状態管理カウンター
bool GameOverCounterUse = false;

//============================================================================
//ゲームオーバーの初期化処理
//============================================================================
void InitGameOver(void)
{
	int nCntGameOver;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\wood.png",			//テクスチャのファイル名
		&g_pTextureGameOver[0]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\press_enter.png",	//テクスチャのファイル名
		&g_pTextureGameOver[1]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\gameover_logo.png",	//テクスチャのファイル名
		&g_pTextureGameOver[2]
	);

	//構造体の初期化処理
	g_aGameOver[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);	//位置の初期化処理
	g_aGameOver[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGameOver[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//回転を初期化する
	g_aGameOver[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aGameOver[0].fAngle = atan2f(GAMEOVER_BG_WIDTH, GAMEOVER_BG_HEIGHT);
	g_aGameOver[0].fLength = sqrtf(GAMEOVER_BG_WIDTH * GAMEOVER_BG_WIDTH + GAMEOVER_BG_HEIGHT * GAMEOVER_BG_HEIGHT) / 2.0f;

	g_aGameOver[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.25f, 0.0f);	//位置の初期化処理
	g_aGameOver[1].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGameOver[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//回転を初期化する
	g_aGameOver[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aGameOver[1].fAngle = atan2f(GAMEOVER_ENTER_WIDTH, GAMEOVER_ENTER_HEIGHT);
	g_aGameOver[1].fLength = sqrtf(GAMEOVER_ENTER_WIDTH * GAMEOVER_ENTER_WIDTH + GAMEOVER_ENTER_HEIGHT * GAMEOVER_ENTER_HEIGHT) / 2.0f;

	g_aGameOver[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f, 0.0f);	//位置の初期化処理
	g_aGameOver[2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGameOver[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//回転を初期化する
	g_aGameOver[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aGameOver[2].fAngle = atan2f(GAMEOVER_LOGO_WIDTH, GAMEOVER_LOGO_HEIGHT);
	g_aGameOver[2].fLength = sqrtf(GAMEOVER_LOGO_WIDTH * GAMEOVER_LOGO_WIDTH + GAMEOVER_LOGO_HEIGHT * GAMEOVER_LOGO_HEIGHT) / 2.0f;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_GAMEOVER_IMAGE,	//sizeof(VERTEX_2D) * 必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameOver,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGameOver->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGameOver = 0; nCntGameOver < MAX_GAMEOVER_IMAGE; nCntGameOver++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aGameOver[nCntGameOver].pos.x - sinf(g_aGameOver[nCntGameOver].fAngle + g_aGameOver[nCntGameOver].rot.x) * g_aGameOver[nCntGameOver].fLength;
		pVtx[0].pos.y = g_aGameOver[nCntGameOver].pos.y - cosf(g_aGameOver[nCntGameOver].fAngle + g_aGameOver[nCntGameOver].rot.y) * g_aGameOver[nCntGameOver].fLength;
		pVtx[0].pos.z = g_aGameOver[nCntGameOver].pos.z + 0.0f;

		pVtx[1].pos.x = g_aGameOver[nCntGameOver].pos.x + sinf(g_aGameOver[nCntGameOver].fAngle - g_aGameOver[nCntGameOver].rot.x) * g_aGameOver[nCntGameOver].fLength;
		pVtx[1].pos.y = g_aGameOver[nCntGameOver].pos.y - cosf(g_aGameOver[nCntGameOver].fAngle - g_aGameOver[nCntGameOver].rot.y) * g_aGameOver[nCntGameOver].fLength;
		pVtx[1].pos.z = g_aGameOver[nCntGameOver].pos.z + 0.0f;

		pVtx[2].pos.x = g_aGameOver[nCntGameOver].pos.x - sinf(g_aGameOver[nCntGameOver].fAngle - g_aGameOver[nCntGameOver].rot.x) * g_aGameOver[nCntGameOver].fLength;
		pVtx[2].pos.y = g_aGameOver[nCntGameOver].pos.y + cosf(g_aGameOver[nCntGameOver].fAngle - g_aGameOver[nCntGameOver].rot.y) * g_aGameOver[nCntGameOver].fLength;
		pVtx[2].pos.z = g_aGameOver[nCntGameOver].pos.z + 0.0f;

		pVtx[3].pos.x = g_aGameOver[nCntGameOver].pos.x + sinf(g_aGameOver[nCntGameOver].fAngle + g_aGameOver[nCntGameOver].rot.x) * g_aGameOver[nCntGameOver].fLength;
		pVtx[3].pos.y = g_aGameOver[nCntGameOver].pos.y + cosf(g_aGameOver[nCntGameOver].fAngle + g_aGameOver[nCntGameOver].rot.y) * g_aGameOver[nCntGameOver].fLength;
		pVtx[3].pos.z = g_aGameOver[nCntGameOver].pos.z + 0.0f;

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
	g_pVtxBuffGameOver->Unlock();

}

//============================================================================
//ゲームオーバーの終了処理
//============================================================================
void UninitGameOver(void)
{
	int nCntGameOver;

	for (nCntGameOver = 0; nCntGameOver < MAX_GAMEOVER_IMAGE; nCntGameOver++)
	{
		//テクスチャの破棄
		if (g_pTextureGameOver[nCntGameOver] != NULL)
		{
			g_pTextureGameOver[nCntGameOver]->Release();
			g_pTextureGameOver[nCntGameOver] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGameOver != NULL)
	{
		g_pVtxBuffGameOver->Release();
		g_pVtxBuffGameOver = NULL;
	}
}

//============================================================================
//ゲームオーバーの更新処理
//============================================================================
void UpdateGameOver(void)
{
	FADE fade;
	fade = GetFade();

	if (fade == FADE_NONE)
	{//フェードが行われてない場合の処理
	 //Enterキー又はスペースキーが押された場合
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//モード設定
			SetFade(MODE_TITLE);

			GameOverCounterUse = true;
		}
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Enterが押された後の秒数を数える処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (g_nCounterGameOverState >= 60)
	{//60f(フレーム)後の処理
		GameOverCounterUse = false;
		g_nCounterGameOverState = 0;
	}
	else if (GameOverCounterUse == true)
	{//Enterが押されてからの時間を数える処理
		g_nCounterGameOverState++;
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//タイトルロゴを動かす処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//VERTEX_2D *pVtx;

	////頂点バッファをロックし、頂点データへのポインタを取得
	//g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//pVtx += 8;	//先にポインタを8つ(画像2つ分)動かす処理

	//if (g_aTitle[2].pos.y < TITLE_HEIGHT / 4)
	//{
	//	g_aTitle[2].move.y += (1.0f / 60.0f);

	//	if (fade == FADE_NONE)
	//	{//フェードが行われてない場合の処理
	//		if (GetKeyboardTrigger(DIK_RETURN) == true)
	//		{//タイトルロゴを下に落とす処理
	//			g_aTitle[2].pos.y = TITLE_HEIGHT / 4;
	//		}
	//	}
	//}
	//else
	//{
	//	g_aTitle[2].move.y = 0.0f;

	//	////Enterキー又はスペースキーが押された場合
	//	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//	//{
	//	//	//モード設定
	//	//	SetMode(MODE_GAME);
	//	//}
	//}

	//g_aTitle[2].pos.y += g_aTitle[2].move.y;

	////頂点座標の設定
	//pVtx[0].pos.x = g_aTitle[2].pos.x - sinf(0.0f + g_fAngleTitle + g_aTitle[2].rot.x) * g_fLengthTitle / 2;
	//pVtx[0].pos.y = g_aTitle[2].pos.y - cosf(0.0f + g_fAngleTitle + g_aTitle[2].rot.y) * g_fLengthTitle / 6;
	//pVtx[0].pos.z = g_aTitle[2].pos.z + 0.0f;

	//pVtx[1].pos.x = g_aTitle[2].pos.x + sinf(0.0f + g_fAngleTitle - g_aTitle[2].rot.x) * g_fLengthTitle / 2;
	//pVtx[1].pos.y = g_aTitle[2].pos.y - cosf(0.0f + g_fAngleTitle - g_aTitle[2].rot.y) * g_fLengthTitle / 6;
	//pVtx[1].pos.z = g_aTitle[2].pos.z + 0.0f;

	//pVtx[2].pos.x = g_aTitle[2].pos.x - sinf(0.0f + g_fAngleTitle - g_aTitle[2].rot.x) * g_fLengthTitle / 2;
	//pVtx[2].pos.y = g_aTitle[2].pos.y + cosf(0.0f + g_fAngleTitle - g_aTitle[2].rot.y) * g_fLengthTitle / 6;
	//pVtx[2].pos.z = g_aTitle[2].pos.z + 0.0f;

	//pVtx[3].pos.x = g_aTitle[2].pos.x + sinf(0.0f + g_fAngleTitle + g_aTitle[2].rot.x) * g_fLengthTitle / 2;
	//pVtx[3].pos.y = g_aTitle[2].pos.y + cosf(0.0f + g_fAngleTitle + g_aTitle[2].rot.y) * g_fLengthTitle / 6;
	//pVtx[3].pos.z = g_aTitle[2].pos.z + 0.0f;

	////rhwの設定
	//pVtx[0].rhw = 1.0f;
	//pVtx[1].rhw = 1.0f;
	//pVtx[2].rhw = 1.0f;
	//pVtx[3].rhw = 1.0f;

	////頂点カラーの設定
	//pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	////テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	////頂点バッファをアンロックする
	//g_pVtxBuffTitle->Unlock();
}

//============================================================================
//ゲームオーバーの描画処理
//============================================================================
void DrawGameOver(void)
{
	int nCntGameOver;
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

								//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffGameOver,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntGameOver = 0; nCntGameOver < MAX_GAMEOVER_IMAGE; nCntGameOver++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGameOver[nCntGameOver]);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			4 * nCntGameOver,			//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}