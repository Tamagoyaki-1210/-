//============================================================================
//
//タイトルの処理[title.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_TITLE_IMAGE	(3)						//タイトルの画像最大枚数
#define TITLE_BG_WIDTH		(SCREEN_WIDTH)		//背景の横サイズ
#define TITLE_BG_HEIGHT		(SCREEN_HEIGHT)		//背景の縦サイズ
#define TITLE_ENTER_WIDTH	(SCREEN_WIDTH / 4)	//ENTERの横サイズ
#define TITLE_ENTER_HEIGHT	(SCREEN_HEIGHT / 8)	//ENTERの縦サイズ
#define TITLE_LOGO_WIDTH	(SCREEN_WIDTH)		//ロゴの横サイズ
#define TITLE_LOGO_HEIGHT	(SCREEN_HEIGHT)		//ロゴの縦サイズ

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
}Title;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif