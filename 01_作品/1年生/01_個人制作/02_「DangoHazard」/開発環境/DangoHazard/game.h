//============================================================================
//
//ゲーム画面の処理[game.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define GAME_WIDTH		(SCREEN_WIDTH)	//タイトルの横サイズ
#define GAME_HEIGHT		(SCREEN_HEIGHT)	//タイトルの縦サイズ

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//回転
	D3DXCOLOR	col;	//色
}Game;

typedef enum
{
	GAMESTATE_NONE = 0,		//何もしてない状態
	GAMESTATE_NORMAL,		//通常状態
	GAMESTATE_GAMECLEAR,	//ゲームクリア状態
	GAMESTATE_GAMEOVER,		//ゲームオーバー状態
	GAMESTATE_MAX
}GAMESTATE;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);

#endif