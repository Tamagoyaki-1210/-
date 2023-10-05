//==================================================
//
// FPD制作 ( mode.cpp )
// Author  : katsuki mizuki
// Author:Kishimoto Eiji
//
//==================================================

//--------------------------------------------------
// インクルード
//--------------------------------------------------
#include "mode.h"
#include "fade.h"
#include "title.h"
//#include "tutorial.h"
#include "option.h"
#include "game.h"
#include "sound.h"
#include "bgm.h"

#include <assert.h>

//--------------------------------------------------
// スタティック変数
//--------------------------------------------------
static MODE		s_mode = MODE_NONE;			// 現在のモード
static MODE		s_modeNext = MODE_NONE;		// 次のモード

//--------------------------------------------------
// 終了
//--------------------------------------------------
void UninitMode(void)
{
	// タイトルの終了
	UninitTitle();

	// ゲームの終了
	UninitGame();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void UpdateMode(void)
{
	switch (s_mode)
	{// どのモード？
	case MODE_TITLE:		// タイトル
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		// チュートリアル
		//UpdateTutorial();
		break;

	case MODE_OPTION:		// オプション
		UpdateOption();
		break;

	case MODE_GAME:			// ゲーム
		UpdateGame();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void DrawMode(void)
{
	switch (s_mode)
	{// どのモード？
	case MODE_TITLE:		// タイトル
		DrawTitle();
		break;

	case MODE_TUTORIAL:		// チュートリアル
		//DrawTutorial();
		break;

	case MODE_OPTION:		// オプション
		DrawOption();
		break;

	case MODE_GAME:			// ゲーム
		DrawGame();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}
}

//--------------------------------------------------
// モードの設定
//--------------------------------------------------
void SetMode(void)
{
	if (s_modeNext == MODE_NONE)
	{// 次のモードが決まってない
		return;
	}

	if (GetFade() == FADE_NONE)
	{// 何もしていない状態なら
		StartFadeOut();
	}

	if (GetFade() != FADE_IN)
	{// フェードイン状態じゃない
		return;
	}

	switch (s_mode)
	{// 現在のモードの終了
	case MODE_TITLE:		// タイトル
		//サウンドの停止
		StopSound(SOUND_LABEL_BGM_TITLE);
		UninitTitle();
		break;

	case MODE_TUTORIAL:		// チュートリアル
		//UninitTutorial();
		break;

	case MODE_OPTION:		// オプション
		StopSound(SOUND_LABEL_BGM_OPSION);
		UninitOption();
		break;

	case MODE_GAME:			// ゲーム
		UninitBgm();
		UninitGame();
		break;

	case MODE_NONE:
		/* 処理なし */
		break;

	default:
		assert(false);
		break;
	}

	switch (s_modeNext)
	{// 次のモードの初期化
	case MODE_TITLE:		// タイトル
		PlaySound(SOUND_LABEL_BGM_TITLE);
		InitTitle();
		break;

	case MODE_TUTORIAL:		// チュートリアル
		//InitTutorial();
		break;

	case MODE_OPTION:		// オプション
		PlaySound(SOUND_LABEL_BGM_OPSION);
		InitOption();
		break;

	case MODE_GAME:			// ゲーム
		SetBgm();
		InitGame();
		break;

	case MODE_NONE:
	default:
		assert(false);
		break;
	}

	s_mode = s_modeNext;		// 現在の画面(モード)を切り替える
	s_modeNext = MODE_NONE;
}

//--------------------------------------------------
// モードの取得
//--------------------------------------------------
MODE GetMode(void)
{
	return s_mode;
}

//--------------------------------------------------
// モードの変更
//--------------------------------------------------
void ChangeMode(MODE mode)
{
	s_modeNext = mode;
}
