//============================================================================
//
//敵の処理[enemy.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "block.h"		//作成したblock.hをインクルードする

//****************************************************************************
//マクロ定義
//****************************************************************************
#define NUM_ENEMY				(5)		//敵の種類数
#define MAX_ENEMY				(16)	//敵の最大数
#define ENEMY_WIDTH				(40)	//全敵の横サイズ
#define ENEMY_HEIGHT			(40)	//全敵の縦サイズ
#define MAX_ENEMY_LIFE			(1)	//敵の最大体力
#define ENEMY1_SPEAD			(2)		//敵1の速さ
#define ENEMY2_SPEAD			(4)		//敵2の速さ
#define ENEMY3_SPEAD			(6)		//敵3の速さ
#define ENEMY4_SPEAD			(8)		//敵4の速さ

//****************************************************************************
//敵の種類の定義
//****************************************************************************
//敵の種類
typedef enum
{
	ENEMY_TYPE_1 = 0,	//敵1
	ENEMY_TYPE_2,		//敵2
	ENEMY_TYPE_3,		//敵3
	ENEMY_TYPE_4,		//敵4
	ENEMY_TYPE_WHITE,	//敵消去状態
	ENEMY_TYPE_MAX
}ENEMYTYPE;			

//****************************************************************************
//敵の状態の定義
//****************************************************************************
typedef enum
{
	ENEMY_STATE_NORMAL = 0,			//通常状態
	ENEMY_STATE_DAMEGE,				//ダメージ処理
	ENEMY_STATE_WHITE,				//消去状態
	ENEMY_STATE_TRUN_R,				//右移動
	ENEMY_STATE_TRUN_L,				//左移動
	ENEMY_STATE_MAX
}ENEMYSTATE;

//****************************************************************************
//構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXCOLOR	col;		//色
	ENEMYSTATE state;		//状態
	ENEMYTYPE type;			//種類
	int nCounterState;		//状態管理カウンター
	int nLife;				//体力
	bool bUse;				//使用しているかどうか
	bool bMove;				//動けるかどうか
	bool bHide;				//隠れ判定
	BLOCK *pBlock;			//ブロックの情報
}ENEMY;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE nType);
ENEMY *GetEnemy(void);
void HitEnemy(int nCntEnemy,int nLife);

#endif
