//=========================================
// 
// bgm(ヘッダーファイル)
// Author Tanimoto
// 
// Update 22/03/24
//
//=========================================
//------------------------------------
// include
//------------------------------------
#include "bgm.h"
#include "sound.h"

#include <time.h>			//タイム関数を使用するために必要
#include <stdlib.h>			//ランダム関数やsystem("cls")を使用するために必要

//------------------------------------
// 静的変数
//------------------------------------
static BGM s_aBgm;
static int s_nRndBgm;

//=========================================
// 初期化
//=========================================
void InitBgm(void)
{
	srand((unsigned int)time(0));

	s_nRndBgm = rand() % MAX_BGM;

	if (s_nRndBgm == BGM_TYPE_1)
	{
		PlaySound(SOUND_LABEL_BGM_GAME1);
	}
	else if (s_nRndBgm == BGM_TYPE_2)
	{
		PlaySound(SOUND_LABEL_BGM_GAME2);
	}
	else if (s_nRndBgm == BGM_TYPE_3)
	{
		PlaySound(SOUND_LABEL_BGM_GAME3);
	}
}

//=========================================
// 終了
//=========================================
void UninitBgm(void)
{
	if (s_nRndBgm == BGM_TYPE_1)
	{
		StopSound(SOUND_LABEL_BGM_GAME1);
	}
	else if (s_nRndBgm == BGM_TYPE_2)
	{
		StopSound(SOUND_LABEL_BGM_GAME2);
	}
	else if (s_nRndBgm == BGM_TYPE_3)
	{
		StopSound(SOUND_LABEL_BGM_GAME3);
	}
}

//=========================================
// 更新
//=========================================
void UpdateBgm(void)
{}

//=========================================
// 描画
//=========================================
void DrawBgm(void)
{}

//=========================================
// 設定
//=========================================
void SetBgm(void)
{
	InitBgm();
}
