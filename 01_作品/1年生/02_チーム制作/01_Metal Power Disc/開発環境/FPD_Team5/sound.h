//=============================================================================
// 
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
// 
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// *****************************************************************************
// サウンドファイル
// *****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,		// BGMタイトル
	SOUND_LABEL_BGM_OPSION,
	SOUND_LABEL_BGM_GAME1,
	SOUND_LABEL_BGM_GAME2,
	SOUND_LABEL_BGM_GAME3,
	SOUND_LABEL_SE_SELECT,
	SOUND_LABEL_SE_YES,
	SOUND_LABEL_SE_NO,
	SOUND_LABEL_SE_GEARSTOP,
	SOUND_LABEL_SE_WHISTLE_START,
	SOUND_LABEL_SE_WHISTLE_STOP,
	SOUND_LABEL_SE_WHISTLE_FINISH,
	SOUND_LABEL_SE_THROW_NORMAL,
	SOUND_LABEL_SE_THROW_LOB,
	SOUND_LABEL_SE_THROW_SPECIAL,
	SOUND_LABEL_SE_SLIDING,
	SOUND_LABEL_SE_CATCH,
	SOUND_LABEL_SE_BOUNCE_NORMAL,
	SOUND_LABEL_SE_BOUNCE_SPECIAL,
	SOUND_LABEL_SE_DISKFELL,
	SOUND_LABEL_SE_GOAL1,
	SOUND_LABEL_SE_GOAL2,
	SOUND_LABEL_SE_CHEERS1,
	SOUND_LABEL_SE_CHEERS2,
	SOUND_LABEL_SE_ROUND,
	SOUND_LABEL_SE_POINT,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

// *****************************************************************************
// プロトタイプ宣言
// *****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
