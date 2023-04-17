//============================================================================
//
//影描画の処理[shadow.h] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#ifndef _SHADOW_H_						//このマクロ定義がされてなかったら
#define _SHADOW_H_						//2連インクルード防止のマクロ定義

#include "main.h"		//作成したmain.hをインクルードする

//****************************************************************************
//マクロ定義
//****************************************************************************
#define MAX_SHADOW				(1)		//影の最大数
#define SHADOW_WIDTH			(10)	//影画像の横幅
#define SHADOW_HEIGHT			(10)	//影画像の縦幅
//****************************************************************************
//構造体定義
//****************************************************************************
//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 rot;	//向き
	D3DXMATRIX Worldmtx;//ワールドマトリックス
	D3DCOLOR col;		//頂点カラー
	bool bUse;
	float fAngle;		//角度(アークタンジェント)
	float fLength;		//長さ(スクウェアルート)
}SHADOW;

//****************************************************************************
//プロトタイプ宣言
//****************************************************************************
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void SetpositionShadow(int nIdxShadow, D3DXVECTOR3 pos ,float fLenght, D3DXCOLOR col);

#endif 