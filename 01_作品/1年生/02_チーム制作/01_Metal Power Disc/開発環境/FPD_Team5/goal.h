//=========================================
// 
// ゴール(ヘッダーファイル)
// Author Tanimoto_Kosuke
//
// Update 22/03/22
// 
//=========================================
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define GOAL_WIDTH				(80)	//ゴールの横サイズ
#define GOAL_HEIGHT				(150)	//ゴールの縦サイズ
#define STRIKE_GOAL_HEIGHT		(170)	//真ん中ゴールの縦サイズ
#define MAX_GOAL				(6)		//ゴールの最大数
#define MAX_IMAGE_DRUM			(2)		//ゴール画像の最大数
#define MAX_POST				(2)		//ポスト画像の最大数
#define GOAL_FLASH				(15)	//ゴールの点滅速度

//****************************************************************************
//ゴールの種類
//****************************************************************************
typedef enum
{
	GOAL_TYPE_NORMAL = 0,	//3点ゴール
	GOAL_TYPE_STRIKE,		//5点ゴール
	GOAL_TYPE_MAX
}GOAL_TYPE;

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;	//回転
	D3DXVECTOR3 nor;	//法線
	D3DXCOLOR	col;	//色
	GOAL_TYPE type;		//種類
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
	float fGoalCounter;	//ゴールカウンター
	float fFlashCounter;//点滅カウンター
	bool bUse;			//使用判定
	bool bGoal;			//ゴール判定
	bool bAddFlash;		//点滅判定
	bool bSide;			//どっち側判定(false:左側 true:右側)
}Goal;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
void SetGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, GOAL_TYPE type, int nIdxGoal);

void ColisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos);
bool ColSegmentsGoal(D3DXVECTOR3 &seg1Start, D3DXVECTOR3 &seg1Vec, D3DXVECTOR3 &seg2Start, D3DXVECTOR3 &seg2Vec);
float Vec3CrossGoal(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2);

Goal *GetGoal(void);

#endif