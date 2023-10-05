//============================================================================
//
//アイテムの処理[item.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"

//****************************************************************************
//マクロ定義
//****************************************************************************
#define NUM_ITEM			(3)		//アイテムの種類
#define MAX_ITEM			(4)		//アイテムの最大数
#define ITEM_WIDTH			(30)	//通常アイテムの横サイズ
#define ITEM_HEIGHT			(60)	//通常アイテムの縦サイズ
#define COIN_MAX_ANIMA		(4)		//コインのアニメパターン数
#define COIN_ANIMX			(4)		//コインのX軸アニメ数
#define COIN_ANIMY			(1)		//コインのY軸アニメ数
#define EFFECT_MAX_ANIMA	(8)		//コインのアニメパターン数
#define EFFECT_ANIMX		(8)		//コインのX軸アニメ数
#define EFFECT_ANIMY		(1)		//コインのY軸アニメ数

//****************************************************************************
//アイテムの種類
//****************************************************************************
typedef enum
{
	ITEM_TYPE_COIN = 0,	//コイン
	ITEM_TYPE_EFFECT,
	ITEM_TYPE_WHITE,
	ITEM_TYPE_MAX
}ITEM_TYPE;

typedef enum
{
	ITEM_STATE_NORMAL = 0,
	ITEM_STATE_TRUN_R,
	ITEM_STATE_TRUN_L,
	ITEM_STATE_DELETE,
	ITEM_STATE_MAX
}ITEM_STATE;
//****************************************************************************
//アイテム構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//回転
	D3DXCOLOR	col;		//色
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 setpos;		//設置位置
	ITEM_STATE state;		//出現
	ITEM_TYPE type;			//種別
	int nCounterAnim;		//アニメーションカウンター
	int nCounterAnimBreak;	//アニメーションカウンターブレーキ
	int nPatternAnim;		//アニメーションパターン番号
	int nDeleteItemCounter;	//消える秒数
	float fWidth;			//幅
	float fHeight;			//高さ
	float fAngle;			//角度(アークタンジェント)
	float fLength;			//長さ(スクウェアルート)
	bool bUse;				//使用判定
	BLOCK *pBlock;			//ブロックの情報
}ITEM;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEM_TYPE type, float Width, float Height);
void ColisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void DeleteItem(int nCntItem);
ITEM *GetItem(void);
void HitItem(int nCntItem);

#endif

