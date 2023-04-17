//============================================================================
//
//ブロックの処理[block.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define NUM_BLOCK			(5)		//ブロックの種類
#define MAX_BLOCK			(128)	//ブロックの最大数
#define BLOCK_WIDTH			(40)	//通常ブロックの横サイズ
#define BLOCK_HEIGHT		(40)	//通常ブロックの縦サイズ
#define BLOCK_NONE_SET		(2)		//画面外ブロックの個数

//****************************************************************************
//ブロックの種類
//****************************************************************************
typedef enum
{
	BLOCK_TYPE_NORMAL = 0,	//通常ブロック
	BLOCK_TYPE_EVADE,		//すり抜けブロック
	BLOCK_TYPE_MOVE,		//移動ブロック
	BLOCK_TYPE_NONE,		//画面外ブロック
	BLOCK_TYPE_WHITE,		//白化ブロック
	BLOCK_TYPE_MAX
}BLOCK_TYPE;

typedef enum
{
	BLOCK_STATE_NORMAL = 0,
	BLOCK_STATE_WHITE,
	BLOCK_STATE_MAX
}BLOCK_STATE;

typedef enum
{
	BLOCK_SIDE_C = 0,	//真ん中ブロック
	BLOCK_SIDE_L,		//左側ブロック
	BLOCK_SIDE_R,		//右側ブロック
	BLOCK_SIDE_MAX
}BLOCK_SIDE;

typedef enum
{
	BLOCK_COLLISION_NONE = 0,
	BLOCK_COLLISION_TOP,
	BLOCK_COLLISION_BOTTOM,
	BLOCK_COLLISION_RIGHT,
	BLOCK_COLLISION_LEFT,
	BLOCK_COLLISION_MAX
}BLOCK_COLLISION;

typedef struct
{
	D3DXVECTOR3 pos;//配置位置
	float fWidth;	//幅
	float fHeight;	//高さ
}BLOCKINFO;
//****************************************************************************
//ブロック構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 rot;	//回転
	D3DXCOLOR	col;	//色
	D3DXVECTOR3 posOld;	//前回の位置
	D3DXVECTOR3 setpos;	//設置位置
	BLOCK_TYPE type;	//種類
	BLOCK_STATE state;	//状態管理
	BLOCK_SIDE side;	//側
	float fWidth;		//幅
	float fHeight;		//高さ
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
	bool bUse;			//使用判定
}BLOCK;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos,BLOCK_TYPE type, BLOCK_SIDE side,int set,int cur);
BLOCK_COLLISION ColisionBlock(D3DXVECTOR3 *pPos,D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pMove,float fWidth,float fHeight,BLOCK **pBlock);
BLOCK *GetBlock(void);
void HitBlock(int nCntBlock);

#endif
