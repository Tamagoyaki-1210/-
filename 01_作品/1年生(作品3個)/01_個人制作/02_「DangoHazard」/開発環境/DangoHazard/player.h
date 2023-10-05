//============================================================================
//
//プレイヤーの処理[player.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _PLAYER_H_		//このマクロ定義がされてなかったら
#define _PLAYER_H_		//2連インクルード防止のマクロ定義

#include "main.h"		//作成したmain.hをインクルードする

//****************************************************************************
//マクロ定義
//****************************************************************************
#define IMAGE_PLAYER		(1)		//プレイヤーの画像数
#define PLAYER_WIDTH		(50)	//プレイヤーの横幅
#define PLAYER_HEIGHT		(70)	//プレイヤーの縦幅
#define MAX_PLAYER_LIFE		(30)	//プレイヤーの最大体力
#define PLAYER_LIFE			(5)		//プレイヤーの体力
#define PLAYER_ATTACK		(2)		//プレイヤーの攻撃判定持続時間
#define PLAYER_SPEAD		(3)		//プレイヤーの移動速度倍率
#define MAX_PLAYER_PARTS	(4)		//プレイヤーのパーツ数

//****************************************************************************
//プレイヤー種類の定義
//****************************************************************************
//プレイヤーのテクスチャ種類
typedef enum
{
	PLAYER_TEXTURE_MOFU = 0,	//羽毛テクスチャ
	PLAYER_TYPE_MAX
}PLAYER_TEXTURE;

//プレイヤーのモーション種類
typedef enum
{
	PLAYER_MOTION_NUTRAL = 0,	//ニュートラルモーション
	PLAYER_MOTION_WARK,			//歩きモーション
	PLAYER_MOTION_JUMP,			//ジャンプモーション
	PLAYER_MOTION_LANDING,		//着地モーション
	PLAYER_MOTION_ATTACK,		//攻撃モーション
	PLAYER_MOTION_DAMEGE,		//被弾モーション
	PLAYER_MOTION_MAX
}PLAYER_MOTION;

//****************************************************************************
//構造体定義
//****************************************************************************
typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_PLAYER];
	D3DXMATRIX Worldmtx;	//ワールドマトリックス
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 rot;		//向き
	float fFrame;			//フレーム数
	int nIdxModelParent;	//親モデルのインデックス
}PLAYER_PARTS;

typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 posOld;		//頂点座標前回位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 Destrot;	//向き目的値
	D3DXMATRIX Worldmtx;	//ワールドマトリックス
	D3DCOLOR col;			//頂点カラー
	int nLife;				//体力
	int nPower;				//力
	float fAngle;			//角度(アークタンジェント)
	float fLength;			//長さ(スクウェアルート)
	float fFlame;			//フレーム数
	float fAttack;			//攻撃待機時間
	bool bUse;				//使用判定
	bool bDamege;			//被弾判定
	bool bLoop;				//ループ判定
	bool bGround;			//接地判定の場合
	PLAYER_MOTION motion;	//モーション種類
	PLAYER_TEXTURE tex;		//テクスチャ種類
	PLAYER_PARTS aParts[MAX_PLAYER_PARTS];
}PLAYER;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

#endif

