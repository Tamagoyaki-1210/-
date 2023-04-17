//=========================================
// 
// ドラム缶(ヘッダーファイル)
// Author Tanimoto_Kosuke
//
// Update 22/03/22
// 
//=========================================
#ifndef _DRUM_H_
#define _DRUM_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define DRUM_WIDTH				(96.0f * 0.55f)	//ドラム缶の横サイズ
#define DRUM_HEIGHT				(160.0f * 0.75f)	//ドラム缶の縦サイズ
#define MAX_DRUM				(6)		//ドラム缶の最大数
#define MAX_IMAGE_DRUM			(2)		//ドラム缶画像の最大数

//****************************************************************************
//ドラム缶の種類
//****************************************************************************
typedef enum
{
	DRUM_TYPE_NORMAL = 0,	//3点ドラム缶
	DRUM_TYPE_STRIKE,		//5点ドラム缶
	DRUM_TYPE_MAX
}DRUM_TYPE;

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
	DRUM_TYPE type;		//種類
	bool bSide;			//どっち側判定(false:左側 true:右側)
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
	bool bUse;			//使用判定
}DRUM;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitDrum(void);
void UninitDrum(void);
void UpdateDrum(void);
void DrawDrum(void);
void SetDrum(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, DRUM_TYPE type, int nIdxGoal);

DRUM *GetDrum(void);

#endif