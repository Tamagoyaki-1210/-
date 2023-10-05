//============================================
//
//チーム制作<フライングパワーディスク>[select.cpp]
//Author:Kishimoto Eiji
//
//============================================
#include "select.h"
#include "option.h"
#include "menu.h"
#include "input.h"
#include "sound.h"

#include <assert.h>

//***********************************
//マクロ定義
//***********************************
#define MAX_SELECT		(3)			//選択肢の最大数
#define SELECT_SIZE		(35.0f)		//選択部分のサイズ
#define SELECT_POS_X	(260.0f)	//選択肢の位置から見た選択部分のX座標

#define MAX_TIMELIMIT	(5)			//制限時間の選択肢の最大数
#define MAX_POINT		(4)			//ポイント数の選択肢の最大数
#define MAX_SETCOUNT	(4)			//セット数の選択肢の最大数

#define MAX_ARROW		(6)			//矢印の最大数
#define ARROW_SIZE		(25.0f)		//矢印のサイズ
#define ARROW_POS_X		(100.0f)	//選択部分から見た矢印のX座標

#define TEXTURE_TIMELIMIT	("data/TEXTURE/WORD/FPD_SelectUI_TimeLimit_Re.png")	//制限時間のテクスチャ
#define TEXTURE_POINT		("data/TEXTURE/WORD/FPD_SelectUI_Point_Re.png")		//ポイント数のテクスチャ
#define TEXTURE_SETCOUNT	("data/TEXTURE/WORD/FPD_SelectUI_SetCount_Re.png")	//セット数のテクスチャ
#define TEXTURE_ARROW		("data/TEXTURE/WORD/FPD_SelectUI_Arrow_Re.png")		//矢印のテクスチャ

//***********************************
//スタティック変数
//***********************************
static LPDIRECT3DTEXTURE9		s_apTextureSelect[MAX_SELECT];	//テクスチャへのポインタ(選択部分)
static LPDIRECT3DTEXTURE9		s_pTextureArrow;				//テクスチャへのポインタ(矢印)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffSelect = NULL;		//頂点バッファへのポインタ(選択部分)
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffArrow = NULL;			//頂点バッファへのポインタ(矢印)

static int s_nSelectTimeLimit;	//選ばれている制限時間
static int s_nSelectPoint;		//選ばれているポイント数
static int s_nSelectSetCount;	//選ばれているセット数

static int s_nTimeLimit;	//制限時間を保存する
static int s_nPoint;		//ポイント数を保存する
static int s_nSetCount;		//セット数を保存する

//***********************************
//プロトタイプ宣言
//***********************************

static void ChangeTexture(int nSelectMenu,int nSelectOption);
static void ChangeColorSelectNow(void);
static void ChangeColorSelectBefore(void);

//============================================
//セレクトの初期化
//============================================
void InitSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//テクスチャの読み込み(選択部分)
	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_TIMELIMIT,
								&s_apTextureSelect[0]);	//制限時間

	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_POINT,
								&s_apTextureSelect[1]);	//ポイント数

	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_SETCOUNT,
								&s_apTextureSelect[2]);	//セット数

	//テクスチャの読み込み(矢印)
	D3DXCreateTextureFromFile(pDevice,
								TEXTURE_ARROW,
								&s_pTextureArrow);

	//変数の初期化
	s_nSelectTimeLimit = 0;
	s_nSelectPoint = 0;
	s_nSelectSetCount = 0;
	s_nTimeLimit = 0;
	s_nPoint = 0;
	s_nSetCount = 0;

	VERTEX_2D * pVtx = NULL;	//頂点情報へのポインタ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SELECT,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffSelect,
								NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ARROW,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffArrow,
								NULL);

	/**************** 選択部分 ****************/

	//頂点バッファをロックし、頂点データへのポインタを取得
	s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posSelect;					//選択部分の位置設定用
	D3DXVECTOR3 aPosArrow[MAX_SELECT];		//矢印の位置設定用

	for (int i = 0; i < MAX_SELECT; i++)
	{
		//メニューの位置を始点に、選択部分の位置を設定する
		posSelect.x = (GetMenu()->Option[i].pos.x) + SELECT_POS_X;
		posSelect.y = GetMenu()->Option[i].pos.y;
		posSelect.z = 0.0f;

		aPosArrow[i] = posSelect;	//矢印の設定時に使用するため一時保存

		//変数にまとめて短くする
		float fLeft		= (posSelect.x - SELECT_SIZE);
		float fRight	= (posSelect.x + SELECT_SIZE);
		float fTop		= (posSelect.y - SELECT_SIZE);
		float fBottom	= (posSelect.y + SELECT_SIZE);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(fLeft,  fTop,		0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fTop,		0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fBottom,	0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fBottom,	0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = NO_SELECT_COLOR;
		pVtx[1].col = NO_SELECT_COLOR;
		pVtx[2].col = NO_SELECT_COLOR;
		pVtx[3].col = NO_SELECT_COLOR;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.25f);

		pVtx += 4;		//4つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffSelect->Unlock();

	/**************** 矢印 ****************/

	//頂点バッファをロックし、頂点データへのポインタを取得
	s_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_ARROW; i++)
	{
		float fPosX, fPosY;		//位置設定用
		
		if ((i >= 0) && (i < (MAX_ARROW / 2)))
		{//左の縦列
			//保存しておいた座標から、左側の矢印の位置を決める
			fPosX = aPosArrow[i].x - ARROW_POS_X;
			fPosY = aPosArrow[i].y;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else if ((i >= (MAX_ARROW / 2)) && (i < MAX_ARROW))
		{//右の縦列
			//保存しておいた座標から、右側の矢印の位置を決める
			fPosX = aPosArrow[i % MAX_SELECT].x + ARROW_POS_X;
			fPosY = aPosArrow[i % MAX_SELECT].y;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		//変数にまとめて短くする
		float fLeft		= (fPosX - ARROW_SIZE);
		float fRight	= (fPosX + ARROW_SIZE);
		float fTop		= (fPosY - ARROW_SIZE);
		float fBottom	= (fPosY + ARROW_SIZE);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(fLeft,  fTop,		0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fTop,		0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fBottom,	0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fBottom,	0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = NO_SELECT_COLOR;
		pVtx[1].col = NO_SELECT_COLOR;
		pVtx[2].col = NO_SELECT_COLOR;
		pVtx[3].col = NO_SELECT_COLOR;

		pVtx += 4;		//4つ分進める
	}

	//頂点バッファをアンロックする
	s_pVtxBuffArrow->Unlock();
}

//============================================
//セレクトの終了
//============================================
void UninitSelect(void)
{
	/****** テクスチャの破棄 ******/

	for (int i = 0; i < MAX_SELECT; i++)
	{
		if (s_apTextureSelect[i] != NULL)
		{//選択部分
			s_apTextureSelect[i]->Release();
			s_apTextureSelect[i] = NULL;
		}
	}

	if (s_pTextureArrow != NULL)
	{//矢印
		s_pTextureArrow->Release();
		s_pTextureArrow = NULL;
	}

	/****** 頂点バッファの破棄 ******/

	if (s_pVtxBuffSelect != NULL)
	{//選択部分
		s_pVtxBuffSelect->Release();
		s_pVtxBuffSelect = NULL;
	}

	if (s_pVtxBuffArrow != NULL)
	{//矢印
		s_pVtxBuffArrow->Release();
		s_pVtxBuffArrow = NULL;
	}
}

//============================================
//セレクトの更新
//============================================
void UpdateSelect(void)
{
	//現在選択されている部分の色を変える
	ChangeColorSelectNow();

	//前回選択されていた部分の色を戻す
	ChangeColorSelectBefore();
}

//============================================
//セレクトの描画
//============================================
void DrawSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	/**************** 選択部分 ****************/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_SELECT; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_apTextureSelect[i]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの形
								i * 4,				//頂点の開始場所
								2);					//プリミティブの数
	}

	/**************** 矢印 ****************/

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffArrow, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_ARROW; i++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTextureArrow);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//ポリゴンの形
								i * 4,				//頂点の開始場所
								2);					//プリミティブの数
	}
}

//============================================
// 制限時間の選択
// int nSelectMenu ---> 選択しているメニュー
//============================================
int SelectTimeLimit(int nSelectMenu)
{
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT, 0))
	{// 左を入力( Aキー or 十字キー左 )

		//選択肢を一つ前(左)にする
		s_nSelectTimeLimit = ((s_nSelectTimeLimit - 1) + TIMELIMIT_MAX) % TIMELIMIT_MAX;
		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT, 0))
	{// 右を入力( Dキー or 十字キー右 )
		
		//選択肢を一つ後(右)にする
		s_nSelectTimeLimit = ((s_nSelectTimeLimit + 1) + TIMELIMIT_MAX) % TIMELIMIT_MAX;
		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	int nTimeLimit = 0;		//値を返す用

	switch (s_nSelectTimeLimit)
	{
	case TIMELIMIT_15:		//15秒
		nTimeLimit = 15;	//一時保存
		break;

	case TIMELIMIT_30:		//30秒
		nTimeLimit = 30;	//一時保存
		break;

	case TIMELIMIT_45:		//45秒
		nTimeLimit = 45;	//一時保存
		break;

	case TIMELIMIT_90:		//90秒
		nTimeLimit = 90;	//一時保存
		break;

	case TIMELIMIT_INFINITY:	//無限(制限時間なし)
		nTimeLimit = 99;		//一時保存(仮で送る値)
		break;

	default:
		assert(false);
		break;
	}

	//テクスチャ位置の切り替え
	ChangeTexture(nSelectMenu, s_nSelectTimeLimit);

	return nTimeLimit;	//設定した値を返す
}

//============================================
// ポイント数の選択
// int nSelectMenu ---> 選択しているメニュー
//============================================
int SelectPoint(int nSelectMenu)
{
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT, 0))
	{// 左を入力( Aキー or 十字キー左 )
		
		//選択肢を一つ前(左)にする
		s_nSelectPoint = ((s_nSelectPoint - 1) + POINTCOUNT_MAX) % POINTCOUNT_MAX;
		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT, 0))
	{// 右を入力( Dキー or 十字キー右 )
		
		//選択肢を一つ後(右)にする
		s_nSelectPoint = ((s_nSelectPoint + 1) + POINTCOUNT_MAX) % POINTCOUNT_MAX;
		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	int nPoint = 0; 	//値を返す用

	switch (s_nSelectPoint)
	{
	case POINTCOUNT_12:	//12点
		nPoint = 12;	//一時保存
		break;

	case POINTCOUNT_15:	//15点
		nPoint = 15;	//一時保存
		break;

	case POINTCOUNT_21:	//21点
		nPoint = 21;	//一時保存
		break;

	case POINTCOUNT_25:	//25点
		nPoint = 25;	//一時保存
		break;

	default:
		assert(false);
		break;
	}

	//テクスチャ位置の切り替え
	ChangeTexture(nSelectMenu, s_nSelectPoint);

	return nPoint;	//設定した値を返す
}

//============================================
// セット数の選択
// int nSelectMenu ---> 選択しているメニュー
//============================================
int SelectSetCount(int nSelectMenu)
{
	if (GetKeyboardTrigger(DIK_A) || GetJoypadTrigger(JOYKEY_LEFT, 0))
	{// 左を入力( Aキー or 十字キー左 )
		
		//選択肢を一つ前(左)にする
		s_nSelectSetCount = ((s_nSelectSetCount - 1) + SETCOUNT_MAX) % SETCOUNT_MAX;
		PlaySound(SOUND_LABEL_SE_SELECT);
	}
	else if (GetKeyboardTrigger(DIK_D) || GetJoypadTrigger(JOYKEY_RIGHT, 0))
	{// 右を入力( Dキー or 十字キー右 )
		
		//選択肢を一つ後(右)にする
		s_nSelectSetCount = ((s_nSelectSetCount + 1) + SETCOUNT_MAX) % SETCOUNT_MAX;
		PlaySound(SOUND_LABEL_SE_SELECT);
	}

	int nSetCount = 0;	//値を返す用

	switch (s_nSelectSetCount)
	{
	case SETCOUNT_ONE:		//1セット
		nSetCount = 1;		//一時保存
		break;

	case SETCOUNT_TWO:		//2セット
		nSetCount = 2;		//一時保存
		break;

	case SETCOUNT_THREE:	//3セット
		nSetCount = 3;		//一時保存
		break;

	case SETCOUNT_FOUR:		//4セット
		nSetCount = 4;		//一時保存
		break;

	default:
		assert(false);
		break;
	}

	//テクスチャ位置の切り替え
	ChangeTexture(nSelectMenu, s_nSelectSetCount);

	return nSetCount;	//設定した値を返す
}

//--------------------------------------------
// テクスチャを切り替える
// int nSelectMenu -----> 選択しているメニュー
// int nSelectOption ---> 選択しているオプション
//--------------------------------------------
static void ChangeTexture(int nSelectMenu, int nSelectOption)
{
	VERTEX_2D * pVtx = NULL;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nSelectMenu * 4);		//指定の位置まで進める

	float fDivid = 0,fTexV = 0;		//テクスチャ座標設定用

	switch (nSelectMenu)
	{
	case OPTION_TIMELIMIT:		//制限時間

		fDivid = (1.0f / MAX_TIMELIMIT);	//テクスチャに合わせて分割
		fTexV = fDivid * nSelectOption;		//選択肢に応じたテクスチャ位置にする
		break;

	case OPTION_POINT:		//ポイント数

		fDivid = (1.0f / MAX_POINT);		//テクスチャに合わせて分割
		fTexV = fDivid * nSelectOption;		//選択肢に応じたテクスチャ位置にする
		break;

	case OPTION_SETCOUNT:	//セット数

		fDivid = (1.0f / MAX_SETCOUNT);		//テクスチャに合わせて分割
		fTexV = fDivid * nSelectOption;		//選択肢に応じたテクスチャ位置にする
		break;

	default:
		assert(false);
		break;
	}

	//テクスチャ座標の設定------>後で変える
	pVtx[0].tex = D3DXVECTOR2(0.0f, fTexV);
	pVtx[1].tex = D3DXVECTOR2(1.0f, fTexV);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fTexV + fDivid);
	pVtx[3].tex = D3DXVECTOR2(1.0f, fTexV + fDivid);

	//頂点バッファをアンロックする
	s_pVtxBuffSelect->Unlock();
}

//--------------------------------------------
// 現在選択されている部分の色を変える
//--------------------------------------------
static void ChangeColorSelectNow(void)
{
	if ((GetSelectMenuNow() >= OPTION_TIMELIMIT) && (GetSelectMenuNow() <= OPTION_SETCOUNT))
	{//「ゲーム開始」以外を選択している場合

		VERTEX_2D *pVtx = NULL;								//頂点情報へのポインタ
		D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//設定用

		/**************** 選択部分 ****************/

		//頂点バッファをロックし、頂点データへのポインタを取得
		s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (GetSelectMenuNow() * 4);		//指定の位置まで進める

		//頂点カラーの設定
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		//頂点バッファをアンロックする
		s_pVtxBuffSelect->Unlock();

		/**************** 矢印 ****************/

		//頂点バッファをロックし、頂点データへのポインタを取得
		s_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (GetSelectMenuNow() * 4);	//左側の矢印

		//頂点カラーの設定
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		//頂点バッファをアンロックする
		s_pVtxBuffArrow->Unlock();

		//頂点バッファをロックし、頂点データへのポインタを取得
		s_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += ((GetSelectMenuNow() + 3) * 4);	//右側の矢印

		//頂点カラーの設定
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		//頂点バッファをアンロックする
		s_pVtxBuffArrow->Unlock();
	}
}

//--------------------------------------------
// 前回選択されていた部分の色を戻す
//--------------------------------------------
static void ChangeColorSelectBefore(void)
{
	if ((GetSelectMenuBefore() != OPTION_GOTOGAME) && (GetSelectMenuBefore() != GetSelectMenuNow()))
	{//「試合開始」以外を選択していた & 前回とは別のものを選択したとき

		VERTEX_2D *pVtx = NULL;		//頂点情報へのポインタ

		/**************** 選択部分 ****************/

		//頂点バッファをロックし、頂点データへのポインタを取得
		s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (GetSelectMenuBefore() * 4);		//指定の位置まで進める

		//頂点カラーの設定
		pVtx[0].col = NO_SELECT_COLOR;
		pVtx[1].col = NO_SELECT_COLOR;
		pVtx[2].col = NO_SELECT_COLOR;
		pVtx[3].col = NO_SELECT_COLOR;

		//頂点バッファをアンロックする
		s_pVtxBuffSelect->Unlock();

		if ((GetSelectMenuNow() != OPTION_GOTOGAME) || (GetSelectMenuNow() == OPTION_GOTOGAME))
		{//「試合開始」以外を選択中 or 「試合開始」を選択中

			/**************** 矢印 ****************/

			//頂点バッファをロックし、頂点データへのポインタを取得
			s_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (GetSelectMenuBefore() * 4);	//左側の矢印

			//頂点カラーの設定
			pVtx[0].col = NO_SELECT_COLOR;
			pVtx[1].col = NO_SELECT_COLOR;
			pVtx[2].col = NO_SELECT_COLOR;
			pVtx[3].col = NO_SELECT_COLOR;

			//頂点バッファをアンロックする
			s_pVtxBuffArrow->Unlock();

			//頂点バッファをロックし、頂点データへのポインタを取得
			s_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += ((GetSelectMenuBefore() + 3) * 4);	//右側の矢印

			//頂点カラーの設定
			pVtx[0].col = NO_SELECT_COLOR;
			pVtx[1].col = NO_SELECT_COLOR;
			pVtx[2].col = NO_SELECT_COLOR;
			pVtx[3].col = NO_SELECT_COLOR;

			//頂点バッファをアンロックする
			s_pVtxBuffArrow->Unlock();
		}
	}
}