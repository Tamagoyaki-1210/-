//=========================================
// 
// ステージ(ヘッダーファイル)
// Author Tanimoto_Kosuke
//
// Update 22/03/23
// 
//=========================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define STAGE_HEIGHT_DOWN		(50)								//ステージ縦のズレ(下側)
#define STAGE_WIDTH				(SCREEN_WIDTH + 50)					//ステージの横サイズ
#define STAGE_HEIGHT			(SCREEN_HEIGHT - 250)				//ステージの縦サイズ
#define STAGE_NET_WIDTH			(10)								//網の横サイズ
#define STAGE_NET_HEIGHT		(SCREEN_HEIGHT - 250)				//網の縦サイズ
#define STAGE_WALL_WIDTH		(SCREEN_WIDTH)					//壁の横サイズ
#define STAGE_WALL_HEIGHT		(50)								//壁の縦サイズ
#define MAX_STAGE_OBJECT		(4)									//ステージオブジェクトの最大数
#define MAX_IMAGE_STAGE			(4)									//ステージ画像の最大数
#define DISK_DELETE				(30)								//落ちたディスクが消える時間

#define MIN_WIDTH				(80)								//横最小値
#define MIN_HEIGHT				(150)								//縦最小値
#define MAX_WIDTH				(SCREEN_WIDTH - MIN_WIDTH)			//横最大値
#define MAX_HEIGHT				(SCREEN_HEIGHT - MIN_HEIGHT + 50)	//縦最大値

//****************************************************************************
//ステージの種類
//****************************************************************************
typedef enum
{
	STAGE_TYPE_FRONT = 0,	//ステージ前側
	STAGE_TYPE_BACK,		//ステージ後ろ側
	STAGE_TYPE_NET,			//網
	STAGE_TYPE_WALL,		//壁(デザイン)
	STAGE_TYPE_MAX
}STAGE_TYPE;

//****************************************************************************
//ステージの長さ
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 min;	//最小値
	D3DXVECTOR3 max;	//最大値
}STAGE_LENGTH;

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//回転
	STAGE_TYPE type;	//種類
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
}STAGE;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);

STAGE_LENGTH *GetP1StgLng(void);
STAGE_LENGTH *GetP2StgLng(void);

#endif