//============================================================================
//
//プレイヤー描画(プレイヤー000)の処理[player.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _POLYGON_H_		//このマクロ定義がされてなかったら
#define _POLYGON_H_		//2重インクルード防止のマクロ定義

#include "main.h"		//作成したmain.hをインクルードする
#include "block.h"		//作成したblock.hをインクルードする

//****************************************************************************
//マクロ定義
//****************************************************************************
#define PLAYER_WIDTH			(50)	//プレイヤー画像の横幅
#define PLAYER_HEIGHT			(70)	//プレイヤー画像の縦幅
#define PLAYER_MAXJUMP			(2)		//プレイヤーのジャンプ回数
#define MAX_PLAYER_LIFE			(3)		//プレイヤーの最大ライフ
#define PLAYER_ANIMATION		(4)		//プレイヤーのアニメパターン数
#define PLAYER_ANIMX			(4)		//プレイヤーのX軸アニメ数
#define PLAYER_ANIMY			(2)		//プレイヤーのY軸アニメ数

//****************************************************************************
//プレイヤー構造体の定義
//****************************************************************************
typedef enum
{	
	PLAYERSTATE_APPEAR = 0,		//出現状態(点滅)
	PLAYERSTATE_NORMAL,	//通常状態
	PLAYERSTATE_DAMAGE,		//ダメージ状態
	PLAYERSTATE_DEATH,		//死亡状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{
	APPEARSTATE_IN = 0,		//見える状態
	APPEARSTATE_OUT,		//見えない状態
	APPEARSTATE_MAX
}APPEARSTATE;

typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 moveBullet;	//弾の移動量
	D3DXVECTOR3 rot;		//回転
	D3DXVECTOR3 posOld;		//前回の位置 
	D3DXCOLOR	col;		//色
	PLAYERSTATE state;		//状態
	APPEARSTATE Astate;		//出現状態
	int nCounterState;		//状態管理カウンター
	int nCounterAppear;		//出現状態カウンター
	int nPatternAnim;		//アニメーションパターン
	int nLife;				//寿命
	int nDirectionMove;
	float fWidth;			//幅
	float fHeight;			//高さ
	float fAngle;			//角度(アークタンジェント)
	float fLength;			//長さ(スクウェアルート)
	bool bUse;				//使用判定
	bool bAppearUse;		//出現使用判定
	bool bDisp;
	bool bIsJumping;		//ジャンプ使用判定
	BLOCK *pBlock;			//ブロックの情報
}Player;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);

#endif