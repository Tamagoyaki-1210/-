//============================================================================
//
//ゲームオーバーの処理[gameover.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_GAMEOVER_IMAGE	(3)						//タイトルの画像最大枚数
#define GAMEOVER_BG_WIDTH		(SCREEN_WIDTH)		//背景の横サイズ
#define GAMEOVER_BG_HEIGHT		(SCREEN_HEIGHT)		//背景の縦サイズ
#define GAMEOVER_ENTER_WIDTH	(SCREEN_WIDTH / 4)	//ENTERの横サイズ
#define GAMEOVER_ENTER_HEIGHT	(SCREEN_HEIGHT / 8)	//ENTERの縦サイズ
#define GAMEOVER_LOGO_WIDTH		(SCREEN_WIDTH / 2)	//ロゴの横サイズ
#define GAMEOVER_LOGO_HEIGHT	(SCREEN_HEIGHT / 4)	//ロゴの縦サイズ

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;	//向き
	D3DXCOLOR	col;	//色
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
}GAMEOVER;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitGameOver(void);
void UninitGameOver(void);
void UpdateGameOver(void);
void DrawGameOver(void);

#endif#pragma once
