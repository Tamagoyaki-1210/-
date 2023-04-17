//============================================================================
//
//タイマーの処理[timer.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_TIMER				(99999999)	//タイマーの最大数
#define MAX_TIMER_DIGIT			(4)			//タイマーの桁数
#define COUNT_TIMER				(80)		//タイマーがカウントする数字
#define MINIUT_TIMER_DIGIT		(2)			//タイマーの桁数(分)
#define SECOND_TIMER_DIGIT		(2)			//タイマーの桁数(秒)
#define TIMER_BASE_NUMBER		(10)		//進数
#define TIMER_WIDTH				(40)		//タイマーの横サイズ
#define TIMER_HEIGHT			(60)		//タイマーの縦サイズ

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
}TIMER;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
void SetTimer(int nTimer);
void CounterTimer(int nValue);

#endif