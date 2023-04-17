//============================================================================
//
//アイテムの処理[item.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _ITEM_H_						//このマクロ定義がされてなかったら
#define _ITEM_H_						//2連インクルード防止のマクロ定義

#include "main.h"		//作成したmain.hをインクルードする

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_ITEM			(400)		//アイテムの最大数
#define IMAGE_ITEM			(1)		//アイテム数
#define ITEM_WIDTH			(50)	//アイテムの横幅
#define ITEM_HEIGHT			(70)	//アイテムの縦幅

//****************************************************************************
//アイテムの種類の定義
//****************************************************************************
//アイテムの種類
typedef enum
{
	ITEM_TYPE_CORN = 0,	//モデル(玉蜀黍)
	ITEM_TYPE_MAX
}ITEM_TYPE;

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
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_ITEM];
}X_ITEM;

typedef struct
{
	D3DXMATRIX Worldmtx;	//ワールドマトリックス
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 rot;		//向き
}ITEM_PARTS;

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
	ITEM_TYPE type;		//種類
	ITEM_PARTS aParts[IMAGE_ITEM]; //パーツ
}ITEM;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
bool ItemCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ITEM_TYPE type);
ITEM *GetItem(void);

#endif

