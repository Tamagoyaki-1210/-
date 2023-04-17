//============================================================================
//
//攻撃判定の処理[item.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _ATTACK_H_						//このマクロ定義がされてなかったら
#define _ATTACK_H_						//2連インクルード防止のマクロ定義

#include "main.h"		//作成したmain.hをインクルードする

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_ATTACK			(255)	//攻撃判定の最大数
#define IMAGE_ATTACK		(2)		//攻撃判定数
#define ATTACK_WIDTH		(50)	//攻撃判定の横幅
#define ATTACK_HEIGHT		(70)	//攻撃判定の縦幅

//****************************************************************************
//攻撃判定の種類の定義
//****************************************************************************
//攻撃判定の種類
typedef enum
{
	ATTACK_TYPE_PLAYER = 0,	//当たり判定(プレイヤー)
	ATTACK_TYPE_ENEMY,		//当たり判定(敵キャラ)
	ATTACK_TYPE_MAX
}ATTACK_TYPE;

//****************************************************************************
//構造体定義
//****************************************************************************
typedef struct
{
	LPD3DXMESH pMesh;		//データへのポインタ
	LPD3DXBUFFER pBuffMat;	//頂点バッファへのポインタ
	DWORD nNumMat;			//マテリアル情報の個数
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_ATTACK];
}X_ATTACK;

typedef struct
{
	D3DXMATRIX Worldmtx;	//ワールドマトリックス
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 rot;		//向き
}ATTACK_PARTS;

typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 posOld;		//頂点座標
	D3DXVECTOR3 move;		//動き
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 Destrot;	//向き目的値
	D3DXMATRIX Worldmtx;	//ワールドマトリックス
	D3DCOLOR col;			//頂点カラー
	int nIndxShadowModel;	//影のインデックス番号
	int nLife;				//寿命
	float fAngle;			//角度(アークタンジェント)
	float fLength;			//長さ(スクウェアルート)
	bool bUse;				//判定
	ATTACK_TYPE type;		//種類
	ATTACK_PARTS aParts[IMAGE_ATTACK]; //パーツ
}ATTACK;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitAttack(void);
void UninitAttack(void);
void UpdateAttack(void);
void DrawAttack(void);
bool AttackCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size);
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int life, ATTACK_TYPE type);
ATTACK *GetAttack(void);

#endif
