//============================================================================
//
//リザルト画面の処理[result.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_RESULT_IMAGE	(3)					//リザルト画面の画像最大枚数
#define RESULT_BG_WIDTH		(SCREEN_WIDTH)		//背景の横サイズ
#define RESULT_BG_HEIGHT	(SCREEN_HEIGHT)		//背景の縦サイズ
#define RESULT_ENTER_WIDTH	(SCREEN_WIDTH / 4)	//ENTERの横サイズ
#define RESULT_ENTER_HEIGHT	(SCREEN_HEIGHT / 8)	//ENTERの縦サイズ
#define RESULT_LOGO_WIDTH	(SCREEN_WIDTH)		//ロゴの横サイズ
#define RESULT_LOGO_HEIGHT	(SCREEN_HEIGHT)		//ロゴの縦サイズ

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//回転
	D3DXCOLOR	col;	//色
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
}Result;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif