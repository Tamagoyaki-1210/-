//============================================================================
//
//ビルボード描画の処理[wall.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _BILLBOARD_H_						//このマクロ定義がされてなかったら
#define _BILLBOARD_H_						//2連インクルード防止のマクロ定義

#include "main.h"		//作成したmain.hをインクルードする

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_BILLBOARD				(4)		//ビルボードの最大設置数
#define IMAGE_BILLBOARD				(2)		//ビルボードの画像数
#define BILLBOARD_BULLET_WIDTH		(10)	//ビルボードの横幅(弾)
#define BILLBOARD_BULLET_HEIGHT		(10)	//ビルボードの縦幅(弾)
#define BILLBOARD_TREE_WIDTH		(25)	//ビルボードの横幅(木)
#define BILLBOARD_TREE_HEIGHT		(30)	//ビルボードの縦幅(木)

//****************************************************************************
//ビルボードの種類の定義
//****************************************************************************
//ビルボードの種類
typedef enum
{
	BILLBOARD_TYPE_BULLET = 0,	//ビルボード(弾)
	BILLBOARD_TYPE_TREE,		//ビルボード(木)
	BILLBOARD_TYPE_MAX
}BILLBOARD_TYPE;

//****************************************************************************
//構造体定義
//****************************************************************************
//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX Worldmtx;	//ワールドマトリックス
	D3DCOLOR col;			//頂点カラー
	int IndxShadow;			//影のインデックス
	float fAngle;			//角度(アークタンジェント)
	float fLength;			//長さ(スクウェアルート)
	bool bUse;				//使用判定
	BILLBOARD_TYPE type;	//種類
}BILLBOARD;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitBillBoard(void);
void UninitBillBoard(void);
void UpdateBillBoard(void);
void DrawBillBoard(void);
void SetBillBoard(D3DXVECTOR3 pos, D3DXVECTOR3 rot , BILLBOARD_TYPE type, float fAngle,float fLength);

#endif 