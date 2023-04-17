//============================================================================
//
//モデルの処理[model.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _MODEL_H_						//このマクロ定義がされてなかったら
#define _MODEL_H_						//2連インクルード防止のマクロ定義

#include "main.h"		//作成したmain.hをインクルードする

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_MODEL			(3)		//モデルの最大数
#define IMAGE_MODEL			(3)		//Xモデル数
#define MODEL_WIDTH			(50)	//モデルの横幅
#define MODEL_HEIGHT		(70)	//モデルの縦幅
#define BULLET_INTERVAL		(10)	//弾発射間隔

//****************************************************************************
//モデルの種類の定義
//****************************************************************************
//モデルの種類
typedef enum
{
	MODEL_TYPE_ROCK = 0,	//モデル(岩)
	MODEL_TYPE_STONE,		//モデル(石)
	MODEL_TYPE_TREE,		//モデル(木)
	MODEL_TYPE_MAX
}MODEL_TYPE;

//****************************************************************************
//構造体定義
//****************************************************************************
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
		float fAngle;			//角度(アークタンジェント)
		float fLength;			//長さ(スクウェアルート)
		bool bUse;				//判定
		MODEL_TYPE type;		//種類
}MODEL;

//****************************************************************************
//モデル構造体定義
//****************************************************************************
typedef struct
{
	LPD3DXMESH pMesh;		//メッシュデータへのポインタ
	LPD3DXBUFFER pBuffMat;	//頂点バッファへのポインタ
	DWORD nNumMat;			//メッシュマテリアル情報の個数
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_MODEL];
}X_MODEL;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
bool ModelCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL_TYPE type);
MODEL *GetModel(void);

#endif
