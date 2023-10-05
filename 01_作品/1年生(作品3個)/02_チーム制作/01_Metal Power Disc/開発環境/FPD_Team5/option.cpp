//============================================
//
//チーム制作<フライングパワーディスク>[option.cpp]
//Author: goto yuuki
//Author:Kishimoto Eiji
//
//============================================
#include "option.h"
#include "select.h"
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"
#include "game.h"
#include "gear.h"
#include "sound.h"

#include <assert.h>

//***********************************
//マクロ定義
//***********************************
#define MAX_TEXTURE		(4)			//使用するテクスチャの最大数
#define MENU_WIDTH		(850.0f)	//メニューの幅
#define MENU_HEIGHT		(85.0f)		//メニューの高さ

//***********************************
//スタティック変数
//***********************************
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;				//テクスチャへのポインタ(背景)
static LPDIRECT3DTEXTURE9		s_apTextureMenu[MAX_TEXTURE];	//テクスチャへのポインタ(メニュー)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;				//頂点バッファへのポインタ

static int s_nSelectMenuNow;	//現在選ばれているメニュー
static int s_nSelectMenuBefore;	//前回選ばれていたメニュー
static int s_nSelectTimeLimit;	//選択した制限時間
static int s_nSelectPoint;		//選択したポイント数
static int s_nSelectSetCount;	//選択したセット数

//***********************************
//プロトタイプ宣言
//***********************************

static void SelectMenu(void);

//============================================
//オプションの初期化
//============================================
void InitOption(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/OldPaper.png",
								&s_pTexture);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/Option/FPD_OptionUI_TimenLimit_HGPmintyou.png",
								&s_apTextureMenu[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/Option/FPD_OptionUI_Point_HGPmintyou.png",
								&s_apTextureMenu[1]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/Option/FPD_OptionUI_SetCount_HGPmintyou.png",
								&s_apTextureMenu[2]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/Option/FPD_OptionUI_BattleStart_HGPmintyou.png",
								&s_apTextureMenu[3]);

	//変数の初期化
	s_nSelectMenuNow = 0;
	s_nSelectMenuBefore = 0;
	
	VERTEX_2D * pVtx = NULL;	//頂点情報へのポインタ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuff,
								NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.8f,0.8f, 0.8f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.8f,0.8f, 0.8f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.8f,0.8f, 0.8f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.8f,0.8f, 0.8f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	/*********** メニューのセット **********/

	//メニューの引数の情報
	MenuArgument menu;
	menu.nNumUse = OPTION_MAX;
	menu.fLeft = 0.0f;
	menu.fRight = SCREEN_WIDTH;
	menu.fTop = 0.0f;
	menu.fBottom = SCREEN_HEIGHT;
	menu.fWidth = MENU_WIDTH;
	menu.fHeight = MENU_HEIGHT;

	for (int i = 0; i < OPTION_MAX; i++)
	{
		menu.pTexture[i] = s_apTextureMenu[i];
	}

	//枠の引数の情報
	FrameArgument frame;
	frame.pTexture = NULL;
	frame.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	frame.bUse = false;

	//メニューのセット
	InitMenu();
	SetMenu(menu, frame);

	InitSelect();

	//歯車の初期化処理
	InitGear();

	//===============================================================================
	//オプション画面の歯車の設定
	SetGear(D3DXVECTOR3(353.0f, 273.0f, 0.0f), 150.0f, -D3DX_PI * 0.015f, 0);
	SetGear(D3DXVECTOR3(690.0f, 410.0f, 0.0f), 200.0f, -D3DX_PI * 0.005f, 1);
	SetGear(D3DXVECTOR3(1020.0f, 40.0f, 0.0f), 250.0f, D3DX_PI * 0.011f, 2);
	SetGear(D3DXVECTOR3(350.0f, 500.0f, 0.0f), 230.0f, D3DX_PI * 0.009f, 3);
	SetGear(D3DXVECTOR3(950.0f, 460.0f, 0.0f), 280.0f, -D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(45.0f, 0.0f, 0.0f), 280.0f, -D3DX_PI * 0.018f, 2);
	SetGear(D3DXVECTOR3(750.0f, 730.0f, 0.0f), 300.0f, D3DX_PI * 0.008f, 1);
	SetGear(D3DXVECTOR3(1250.0f, 700.0f, 0.0f), 300.0f, D3DX_PI * 0.022f, 0);
	SetGear(D3DXVECTOR3(1145.0f, 415.0f, 0.0f), 220.0f, D3DX_PI * 0.008f, 4);
	SetGear(D3DXVECTOR3(165.0f, 625.0f, 0.0f), 300.0f, -D3DX_PI * 0.009f, 3);
	SetGear(D3DXVECTOR3(60.0f, 233.0f, 0.0f), 280.0f, D3DX_PI * 0.018f, 2);
	SetGear(D3DXVECTOR3(460.0f, 340.0f, 0.0f), 150.0f, D3DX_PI * 0.015f, 0);
	SetGear(D3DXVECTOR3(500.0f, 0.0f, 0.0f), 100.0f, -D3DX_PI * 0.03f, 4);
	SetGear(D3DXVECTOR3(565.0f, 45.0f, 0.0f), 100.0f, D3DX_PI * 0.03f, 4);
	SetGear(D3DXVECTOR3(260.0f, 45.0f, 0.0f), 240.0f, D3DX_PI * 0.018f, 2);
	//===============================================================================

	// 初期値の入力
	s_nSelectTimeLimit = SelectTimeLimit(s_nSelectMenuNow);
	s_nSelectPoint = SelectPoint(s_nSelectMenuNow);
	s_nSelectSetCount = SelectSetCount(s_nSelectMenuNow);

	//ゲーム開始が選択されている状態にする
	s_nSelectMenuNow = OPTION_GOTOGAME;

	ChangeOption(s_nSelectMenuNow);	//選択肢を変更
}

//============================================
//オプションの終了
//============================================
void UninitOption(void)
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (s_apTextureMenu[i] != NULL)
		{//テクスチャの破棄
			s_apTextureMenu[i]->Release();
			s_apTextureMenu[i] = NULL;
		}
	}
	if (s_pTexture != NULL)
	{//テクスチャの破棄
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	if (s_pVtxBuff != NULL)
	{//頂点バッファの破棄
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	UninitGear();		//歯車の終了処理
	UninitSelect();		//選択部分の終了処理
}

//============================================
//オプションの更新
//============================================
void UpdateOption(void)
{
	s_nSelectMenuBefore = s_nSelectMenuNow;		//前回選択していた番号を保存

	UpdateGear();	//歯車の更新処理
	SelectMenu();	//メニュー選択
	UpdateMenu();	//メニュー更新
	UpdateSelect();
}

//============================================
//オプションの描画
//============================================
void DrawOption(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの形
							0,					//頂点の開始場所
							2);					//プリミティブの数

	DrawGear();		//歯車の描画処理
	DrawMenu();		//メニュー描画
	DrawSelect();
}

//--------------------------------------------
// メニューの選択
//--------------------------------------------
static void SelectMenu(void)
{
	if (GetFade() != FADE_NONE)
	{// フェードしている
		return;
	}

	if (GetKeyboardTrigger(DIK_W) || GetJoypadTrigger(JOYKEY_UP, 0))
	{// 上を入力( Wキー or 十字キー上 )
		InitColorOption();		//選択肢の色を初期化

		//選択肢を上にずらす
		s_nSelectMenuNow = ((s_nSelectMenuNow - 1) + OPTION_MAX) % OPTION_MAX;

		ChangeOption(s_nSelectMenuNow);	//選択肢を変更

		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	else if (GetKeyboardTrigger(DIK_S) || GetJoypadTrigger(JOYKEY_DOWN, 0))
	{// 下を入力( Sキー or 十字キー下 )
		InitColorOption();		//選択肢の色を初期化

		//選択肢を下にずらす
		s_nSelectMenuNow = ((s_nSelectMenuNow + 1) + OPTION_MAX) % OPTION_MAX;

		ChangeOption(s_nSelectMenuNow);	//選択肢を変更

		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	switch (s_nSelectMenuNow)
	{
	case OPTION_TIMELIMIT:	//制限時間

		//制限時間の選択
		s_nSelectTimeLimit = SelectTimeLimit(s_nSelectMenuNow);
		break;

	case OPTION_POINT:	//ポイント数
		
		//ポイント数の選択
		s_nSelectPoint = SelectPoint(s_nSelectMenuNow);
		break;

	case OPTION_SETCOUNT:	//セット数

		//セット数の選択
		s_nSelectSetCount = SelectSetCount(s_nSelectMenuNow);
		break;

	case OPTION_GOTOGAME:	//ゲームへ進む

		if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START, 0))
		{//決定キー(ENTERキー)が押されたかどうか

			SetNeedTime(s_nSelectTimeLimit);	//制限時間
			SetNeedPoint(s_nSelectPoint);	//ポイント数
			SetNeedSet(s_nSelectSetCount);	//セット数

			ChangeMode(MODE_GAME);

			PlaySound(SOUND_LABEL_SE_GEARSTOP);
		}
		break;

	default:
		assert(false);
		break;
	}

	if (GetKeyboardTrigger(DIK_BACKSPACE) || GetJoypadTrigger(JOYKEY_B, 0))
	{
		ChangeMode(MODE_TITLE);
		PlaySound(SOUND_LABEL_SE_NO);
	}
}

//============================================
//現在選択されているメニューを取得
//============================================
int GetSelectMenuNow(void)
{
	return s_nSelectMenuNow;
}

//============================================
//前回選択されていたメニューを取得
//============================================
int GetSelectMenuBefore(void)
{
	return s_nSelectMenuBefore;
}