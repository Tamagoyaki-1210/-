//============================================================================
//
//ハイスコアの処理[high_score.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _HIGH_SCORE_H_
#define _HIGH_SCORE_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_HIGH_SCORE			(99999999)	//点数の最大数
#define HIGH_SCORE_WIDTH			(150)		//点数の横サイズ
#define HIGH_SCORE_HEIGHT			(100)		//点数の縦サイズ
#define FILENAME_DATA	"score,bin"	//読み込むファイル名

//****************************************************************************
//弾構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;	//回転
	D3DXCOLOR	col;	//色
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
}HIGHSCORE;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitHighScore(void);
void UninitHighScore(void);
void UpdateHighScore(void);
void DrawHighScore(void);
void LoadHighScore(void);
void SaveHighScore(int nValue);

#endif