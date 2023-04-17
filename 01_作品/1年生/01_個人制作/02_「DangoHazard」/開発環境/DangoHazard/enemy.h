//============================================================================
//
//敵キャラの処理[enemy.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _ENEMY_H_						//このマクロ定義がされてなかったら
#define _ENEMY_H_						//2連インクルード防止のマクロ定義

#include "main.h"		//作成したmain.hをインクルードする

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_ENEMY			(400)	//敵キャラの最大数
#define MAX_ENEMY_PARTS		(1)		//敵キャラのパーツ数
#define IMAGE_ENEMY			(2)		//敵キャラ数
#define ENEMY_ATTACK		(6)		//敵キャラの攻撃判定持続時間
#define ENEMY_SPEAD			(1.5)		//敵キャラの移動速度倍率
#define ENEMY_BATTLE_RANGE	(100)	//敵キャラの戦闘射程
#define ENEMY_ATTACK_RANGE	(13)	//敵キャラの攻撃開始射程
#define ENEMY_WIDTH			(50)	//敵キャラの横幅
#define ENEMY_HEIGHT		(70)	//敵キャラの縦幅

//****************************************************************************
//敵キャラの種類の定義
//****************************************************************************
//敵キャラの種類
typedef enum
{
	ENEMY_TYPE_POTATE = 0,	//モデル(芋団子)
	ENEMY_TYPE_PUNPKIN,		//モデル(南瓜団子)
	ENEMY_TYPE_MAX
}ENEMY_TYPE;

//敵キャラのモーション種類
typedef enum
{
	ENEMY_MOTION_NUTRAL = 0,	//ニュートラルモーション
	ENEMY_MOTION_WORK,			//歩きモーション
	ENEMY_MOTION_ATTACK,		//攻撃モーション
	ENEMY_MOTION_DAMEGE,		//被弾モーション
	ENEMY_MOTION_MAX
}ENEMY_MOTION;

//****************************************************************************
//構造体定義
//****************************************************************************
typedef struct
{
	LPD3DXMESH pMesh;		//敵キャラデータへのポインタ
	LPD3DXBUFFER pBuffMat;	//頂点バッファへのポインタ
	DWORD nNumMat;			//敵キャラマテリアル情報の個数
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_ENEMY];
}X_ENEMY;

typedef struct
{
	D3DXMATRIX Worldmtx;	//ワールドマトリックス
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 rot;		//向き
}ENEMY_PARTS;

typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 posOld;		//頂点座標
	D3DXVECTOR3 move;		//動き
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 Destrot;	//向き目的値
	D3DXVECTOR3 vec;		//ベクトル変数
	D3DXVECTOR3 vec1;		//ベクトル変数(1固定)
	D3DXMATRIX Worldmtx;	//ワールドマトリックス
	D3DCOLOR col;			//頂点カラー
	int nIndxShadowModel;	//影のインデックス番号
	int nLife;
	float fAngle;			//角度(アークタンジェント)
	float fLength;			//長さ(スクウェアルート)
	float fFlame;			//フレーム数
	float fVecCnt;		//敵キャラ追従フレーム数
	bool bLoop;				//ループ判定
	bool bUse;				//判定
	bool bDamege;			//被弾判定
	bool bBattle;			//戦闘判定
	ENEMY_MOTION motion;	//モーション種類
	ENEMY_TYPE type;		//種類
	ENEMY_PARTS aParts[MAX_ENEMY_PARTS];	//パーツ
}ENEMY;


//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
bool EnemyCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type);
ENEMY *GetEnemy(void);

#endif

