//=========================================
// 
// ゲーム内処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "game.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "stage.h"
#include <stdio.h>
#include "wall.h"
#include "disk.h"
#include "shadow.h"
#include "effect.h"
#include "referee.h"
#include "landingPoint.h"
#include "score.h"
#include "result.h"
#include "time.h"
#include "ui.h"
#include "stgfg.h"
#include "pipe.h"
#include "gear.h"
#include "sound.h"
#include "point.h"
#include "menu.h"
#include "pause.h"
#include "chain.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define RESET_DELAY	(120)	// 次開始までの遅延

//------------------------------------
// 静的変数
//------------------------------------
static int s_nNeedTime;			// 必要タイム数
static int s_nNeedPoint;		// 必要ポイント数
static int s_nNeedSet;			// 必要セット数
static int s_nPlayerSet[2];		// プレイヤーがどれほどセットを取っているか
static bool bIsResult;			// リザルトの表示中か
static bool bIsResetGame;		// スコアリセット中か
static int s_nResetGamePosCnt;	// 所定の位置に戻ってどれほど経過したか
static bool s_bPause;			// ポーズ中かどうか

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void RoundReset(void);

//=========================================
// 初期化
//=========================================
void InitGame(void)
{
	InitDisk();			// ディスク
	InitShadow();		// 影
	InitPlayer();		// プレイヤー
	InitUI();			// UI
	InitStage();		// ステージ
	InitStgFg();		// ステージ前景
	InitLandingMark();	// ディスクの落下地点
	InitEffect();		// エフェクト
	InitResult();		// リザルト
	InitPipe();			// 配管
	InitGear();			// 歯車
	InitPause();		// ポーズ
	InitChain();		// 鎖

	// 初期化
	s_nPlayerSet[0] = 0;
	s_nPlayerSet[1] = 0;
	s_nResetGamePosCnt = 0;
	bIsResult = false;
	s_bPause = false;
	RoundReset();

	SetBackground();
}

//=========================================
// 終了
//=========================================
void UninitGame(void)
{
	UninitPlayer();			// プレイヤー
	UninitShadow();			// 影
	UninitDisk();			// ディスク
	UninitStage();			// ステージ
	UninitStgFg();			// ステージ前景
	UninitLandingMark();	// ディスクの落下地点
	UninitEffect();			// エフェクト
	UninitUI();				// UI
	UninitResult();			// リザルト
	UninitPipe();			// 配管
	UninitGear();			// 歯車
	UninitPause();			// ポーズ
	UninitChain();			// 鎖
}

//=========================================
// 更新
//=========================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START, 0))
	{
		s_bPause = !s_bPause;

		if (s_bPause)
		{// ポーズしてる
			SetPause();		// メニューの設定
		}
	}

	// ポーズ中ならポーズ以外を更新しない
	if (s_bPause)
	{
		UpdateMenu();		// メニュー
		UpdatePause();		// ポーズ
		return;
	}

	if (bIsResult)
	{ // リザルト中
		UpdateResult();			// リザルト
	}
	else
	{
		if (bIsResetGame && ResetPosPlayer())
		{
			s_nResetGamePosCnt++;
			if (s_nResetGamePosCnt == RESET_DELAY)
			{
				DestroyDisk();
				GetReferee()->bThrow = true;
			}

			if (GetPlayer()[0].bHaveDisk || GetPlayer()[1].bHaveDisk)
			{
				s_nResetGamePosCnt = 0;
				bIsResetGame = false;	// リセットの終了
			}
		}

		UpdateStage();			// ステージ
		UpdateStgFg();			// ステージ前景
		UpdatePlayer();			// プレイヤー
		UpdateDisk();			// ディスク
		UpdateLandingMark();	// ディスクの落下地点
		UpdateShadow();			// 影
		UpdateUI();				// UI
		UpdateChain();			// 鎖
	}
		UpdatePipe();			// 配管
		UpdateGear();			// 歯車
		UpdateEffect();			// エフェクト

	// スコア関係の処理
	{
		SCORE* pScore = GetScore();

		// スコアが一定以上か否か
		for (int i = 0; i < 2; i++)
		{
			if (pScore[i].nScore >= s_nNeedPoint)
			{
				// １セットのリセット
				s_nPlayerSet[i]++;	// セット数の取得
				RoundReset();
				PlaySound(SOUND_LABEL_SE_WHISTLE_FINISH);
			}
		}

		if (GetTime()->nTime <= 0)
		{
			DestroyDisk();	// ディスクの削除
			PlaySound(SOUND_LABEL_SE_WHISTLE_STOP);

			if (GetScore()[0].nScore > GetScore()[1].nScore)
			{ // P1の勝ち
				s_nPlayerSet[0]++;	// セット数の取得
				SetThoThrowRefreeIdx(1);	// 投げる方向の選択
				RoundReset();
			}
			else if (GetScore()[0].nScore < GetScore()[1].nScore)
			{ // P2の勝ち
				s_nPlayerSet[1]++;	// セット数の取得
				SetThoThrowRefreeIdx(0);	// 投げる方向の選択
				RoundReset();
			}
			else
			{ // 引き分け
				RoundReset();
			}
			SetPointRound(s_nPlayerSet);
		}

		// セット数が一定以上か否か
		if (s_nPlayerSet[0] >= s_nNeedSet)
		{
			SetWInPlayerIdx(0);
			bIsResult = true;
		}
		else if (s_nPlayerSet[1] >= s_nNeedSet)
		{
			SetWInPlayerIdx(1);
			bIsResult = true;
		}
	}
}

//=========================================
// 描画
//=========================================
void DrawGame()
{
	DrawGear();			// 歯車
	DrawStage();		// ステージ
	DrawShadow();		// 影
	DrawLandingMark();	// ディスクの落下地点
	DrawPipe();			// 配管
	DrawChain();		// 鎖
	DrawStgFg();		// ステージ前景

	if (GetDisk()->type == DISK_TYPE_LOB)
	{
		DrawPlayer();		// プレイヤー
		DrawDisk();			// ディスク
	}
	else
	{
		DrawDisk();			// ディスク
		DrawPlayer();		// プレイヤー
	}
	DrawUI();			// UI
	DrawEffect();		// エフェクト

	if (bIsResult)
	{
		DrawResult();		// リザルト
	}

	if (s_bPause)
	{
		DrawPause();		// ポーズ
		DrawMenu();			// メニュー
	}
}

//=========================================
// 必要タイムの設定
//=========================================
void SetNeedTime(int nPoint)
{
	s_nNeedTime = nPoint;
}

//=========================================
// 必要ポイントの設定
//=========================================
void SetNeedPoint(int nPoint)
{
	s_nNeedPoint = nPoint;
}

//=========================================
// 必要セット数の設定
//=========================================
void SetNeedSet(int nPoint)
{
	s_nNeedSet = nPoint;
}

//=========================================
// リセット中か否か
//=========================================
bool* GetResetScore(void)
{
	return &bIsResetGame;
}

//=========================================
// ラウンドリセット
//=========================================
void RoundReset(void)
{
	Player * pPlayer = GetPlayer();

	for (int nIdxPlayer = 0; nIdxPlayer < 2; nIdxPlayer++, pPlayer++)
	{
		ResetScore();
		pPlayer->bHaveDisk = false;
	}

	SetTime(s_nNeedTime);				// タイムリセット
	bIsResetGame = true;

	PlaySound(SOUND_LABEL_SE_CHEERS2);
}

//=========================================
//背景のパイプと歯車の設定処理
//=========================================
void SetBackground(void)
{
	SetChain(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - 2.0f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(10.0f, 270.0f, 0.0f));

	SetPipe(D3DXVECTOR3(125.0f, 70.0f, 0.0f), D3DXVECTOR3(18.21f, 50.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(250.0f, 70.0f, 0.0f), D3DXVECTOR3(18.21f, 50.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(375.0f, 70.0f, 0.0f), D3DXVECTOR3(18.21f, 50.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(500.0f, 70.0f, 0.0f), D3DXVECTOR3(18.21f, 50.0f, 0.0f), PIPE_TYPE_VERTICAL);

	SetPipe(D3DXVECTOR3(775.0f, 70.0f, 0.0f), D3DXVECTOR3(18.21f, 50.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(900.0f, 70.0f, 0.0f), D3DXVECTOR3(18.21f, 50.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(1025.0f, 70.0f, 0.0f), D3DXVECTOR3(18.21f, 50.0f, 0.0f), PIPE_TYPE_VERTICAL);
	SetPipe(D3DXVECTOR3(1150.0f, 70.0f, 0.0f), D3DXVECTOR3(18.21f, 50.0f, 0.0f), PIPE_TYPE_VERTICAL);

	SetPipe(D3DXVECTOR3(420.0f, 35.0f, 0.0f), D3DXVECTOR3(70.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);
	SetPipe(D3DXVECTOR3(420.0f, 90.0f, 0.0f), D3DXVECTOR3(70.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);
	SetPipe(D3DXVECTOR3(820.0f, 35.0f, 0.0f), D3DXVECTOR3(70.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);
	SetPipe(D3DXVECTOR3(820.0f, 90.0f, 0.0f), D3DXVECTOR3(70.0f, 21.85f, 0.0f), PIPE_TYPE_STEAM);

	//========================================================================================================================
	//左上の穴
	SetDarkerGear(D3DXVECTOR3(50.0f, 20.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 100.0f, -D3DX_PI * 0.01f, 1);
	SetDarkerGear(D3DXVECTOR3(20.0f, 110.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 80.0f, -D3DX_PI * 0.01f, 1);

	SetDarkerGear(D3DXVECTOR3(110.0f, 100.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 155.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(-3.0f, 25.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 175.0f, D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(90.0f, 30.0f, 0.0f), 90.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(46.0f, 81.0f, 0.0f), 65.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(10.0f, 45.0f, 0.0f), 50.0f, -D3DX_PI * 0.008f, 2);

	//========================================================================================================================

	//========================================================================================================================
	//真ん中上の穴
	SetDarkerGear(D3DXVECTOR3(290.0f, 108.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 120.0f, -D3DX_PI * 0.01f, 1);
	SetDarkerGear(D3DXVECTOR3(345.0f, 35.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 90.0f, D3DX_PI * 0.01f, 1);
	SetDarkerGear(D3DXVECTOR3(405.0f, 75.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 110.0f, -D3DX_PI * 0.01f, 1);

	SetDarkerGear(D3DXVECTOR3(450.0f, 40.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 115.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(400.0f, 107.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 95.0f, D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(331.0f, 61.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 110.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(234.0f, 38.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 130.0f, D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(430.0f, 60.0f, 0.0f), 130.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(330.0f, 130.0f, 0.0f), 150.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(248.0f, 48.0f, 0.0f), 120.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(328.0f, 0.0f, 0.0f), 90.0f, D3DX_PI * 0.008f, 2);
	//========================================================================================================================

	//========================================================================================================================
	//右上の穴
	SetDarkerGear(D3DXVECTOR3(988.0f, 30.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 120.0f, -D3DX_PI * 0.01f, 1);
	SetDarkerGear(D3DXVECTOR3(1108.0f, 30.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 120.0f, D3DX_PI * 0.01f, 1);
	SetDarkerGear(D3DXVECTOR3(1148.0f, 120.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 80.0f, D3DX_PI * 0.01f, 1);
	SetDarkerGear(D3DXVECTOR3(1060.0f, 85.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 70.0f, D3DX_PI * 0.01f, 1);

	SetDarkerGear(D3DXVECTOR3(1188.0f, 112.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 70.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(1175.0f, 47.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 90.0f, D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(1114.0f, 86.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 85.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(1040.0f, 70.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 100.0f, D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(970.0f, 81.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 70.0f, -D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(1000.0f, 93.0f, 0.0f), 90.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1060.0f, 40.0f, 0.0f), 100.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1116.0f, 90.0f, 0.0f), 70.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1174.0f, 52.0f, 0.0f), 90.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1265.0f, 81.0f, 0.0f), 130.0f, D3DX_PI * 0.008f, 2);
	//========================================================================================================================

	//========================================================================================================================

	SetDarkerGear(D3DXVECTOR3(150.0f, 230.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 110.0f, D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(230.0f, 250.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 90.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(292.0f, 292.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 90.0f, D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(349.0f, 255.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 70.0f, -D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(210.0f, 220.0f, 0.0f), 160.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(280.0f, 320.0f, 0.0f), 130.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(325.0f, 230.0f, 0.0f), 110.0f, D3DX_PI * 0.008f, 2);
	//========================================================================================================================

	//========================================================================================================================

	SetDarkerGear(D3DXVECTOR3(850.0f, 490.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 140.0f, D3DX_PI * 0.01f, 1);

	SetDarkerGear(D3DXVECTOR3(835.0f, 460.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 100.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(855.0f, 570.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 120.0f, D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(810.0f, 550.0f, 0.0f), 60.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(857.0f, 519.0f, 0.0f), 70.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(815.0f, 482.0f, 0.0f), 60.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(887.0f, 473.0f, 0.0f), 60.0f, D3DX_PI * 0.008f, 2);

	SetDarkerGear(D3DXVECTOR3(665.0f, 165.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 120.0f, D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(682.0f, 250.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 90.0f, -D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(630.0f, 200.0f, 0.0f), 95.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(690.0f, 252.0f, 0.0f), 95.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(730.0f, 332.0f, 0.0f), 110.0f, -D3DX_PI * 0.008f, 2);

	SetDarkerGear(D3DXVECTOR3(430.0f, 390.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 130.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(410.0f, 495.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 130.0f, D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(420.0f, 405.0f, 0.0f), 95.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(387.0f, 473.0f, 0.0f), 80.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(470.0f, 450.0f, 0.0f), 70.0f, -D3DX_PI * 0.008f, 2);
	//========================================================================================================================

	//========================================================================================================================
	//左下側にある穴

	SetDarkerGear(D3DXVECTOR3(20.0f, 685.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 50.0f,  D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(60.0f, 712.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 70.0f, -D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(70.0f, 660.0f, 0.0f), 70.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(35.0f, 700.0f, 0.0f), 55.0f,  D3DX_PI * 0.008f, 2);

	SetDarkerGear(D3DXVECTOR3(570.0f, 741.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 120.0f, -D3DX_PI * 0.01f, 1);
	SetDarkerGear(D3DXVECTOR3(490.0f, 720.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f),  70.0f,  D3DX_PI * 0.01f, 1);
	SetDarkerGear(D3DXVECTOR3(460.0f, 660.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 100.0f, -D3DX_PI * 0.01f, 1);
	SetDarkerGear(D3DXVECTOR3(370.0f, 700.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 125.0f,  D3DX_PI * 0.01f, 1);

	SetDarkerGear(D3DXVECTOR3(360.0f, 640.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 110.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(431.0f, 688.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 95.0f, D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(495.0f, 650.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 85.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(538.0f, 713.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 100.0f, D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(604.0f, 671.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 90.0f, -D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(350.0f, 630.0f, 0.0f), 110.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(388.0f, 707.0f, 0.0f), 90.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(460.0f, 700.0f, 0.0f), 80.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(520.0f, 670.0f, 0.0f), 80.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(582.0f, 660.0f, 0.0f), 65.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(578.0f, 721.0f, 0.0f), 80.0f,  D3DX_PI * 0.008f, 2);
	//========================================================================================================================

	//========================================================================================================================
	//右下側にある穴

	SetDarkerGear(D3DXVECTOR3(1075.0f, 705.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), 105.0f, D3DX_PI * 0.01f, 1);

	SetDarkerGear(D3DXVECTOR3(978.0f, 685.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 95.0f, -D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(1050.0f, 640.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 110.0f, D3DX_PI * 0.003f, 3);
	SetDarkerGear(D3DXVECTOR3(1095.0f, 710.0f, 0.0f), D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f), 100.0f, -D3DX_PI * 0.003f, 3);

	SetGear(D3DXVECTOR3(960.0f, 680.0f, 0.0f), 75.0f, D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1018.0f, 708.0f, 0.0f), 65.0f, -D3DX_PI * 0.008f, 2);
	SetGear(D3DXVECTOR3(1050.0f, 660.0f, 0.0f), 70.0f, D3DX_PI * 0.008f, 2);
	//========================================================================================================================
}

//=========================================
// ポーズの設定処理
//=========================================
void SetEnablePause(bool bUse)
{
	s_bPause = bUse;
}