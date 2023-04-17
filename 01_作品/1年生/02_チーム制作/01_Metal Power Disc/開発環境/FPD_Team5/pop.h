//=========================================
// 
// ポップアップ(ヘッダーファイル)
// Author Tanimoto_Kosuke
//
// Update 22/03/23
// 
//=========================================
#ifndef _POP_H_
#define _POP_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define GOAL_POP_WIDTH		(160)				//ゴールポップの横サイズ
#define GOAL_POP_HEIGHT		(48)				//ゴールポップの縦サイズ
#define FELL_POP_WIDTH		(160)				//フォールポップの横サイズ
#define FELL_POP_HEIGHT		(48)				//フォールポップの縦サイズ
#define MAX_POP				(11)				//ポップの最大数
#define MAX_IMAGE_POP		(3)					//ポップ画像の最大数
#define POP_SPEAD			(4)					//ポップの移動速度

//****************************************************************************
//ポップの種類
//****************************************************************************
typedef enum
{
	POP_TYPE_NORMAL = 0,	//3点ゴールポップ
	POP_TYPE_STRIKE,		//5点ゴールポップ
	POP_TYPE_FELL,			//2点ゴールポップ
	POP_TYPE_MAX
}POP_TYPE;

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;	//回転
	D3DXCOLOR	col;	//色
	POP_TYPE type;		//種類
	bool bSide;			//どっち側判定(false:左側 true:右側)
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
	float fPopCounter;	//ポップカウンター
	bool bUse;			//使用判定
}POP;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitPop(void);
void UninitPop(void);
void UpdatePop(void);
void DrawPop(void);
void SetPop(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, POP_TYPE type, int nIdxGoal);

void PopCounter(int nIdxPop);

POP *GetPop(void);

#endif