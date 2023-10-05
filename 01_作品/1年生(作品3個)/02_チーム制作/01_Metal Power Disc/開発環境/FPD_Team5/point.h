//=========================================
// 
// ポップアップ(ヘッダーファイル)
// Author YudaKaito
// Author Tanimoto
// 
//=========================================
#ifndef _POINT_H_
#define _POINT_H_

#include "main.h"

//------------------------------------
//マクロ定義
//------------------------------------
#define POINT_WIDTH		(100)	//スコアポップの横サイズ
#define POINT_HEIGHT	(100)	//スコアポップの縦サイズ

//------------------------------------
//ポイントの構造体
//------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 pos;					//現在の位置
	D3DXCOLOR col;
	int nScore;							//スコアの値
	bool bUse;
}Point;

//------------------------------------
//プロトタイプ宣言
//------------------------------------
void InitPoint(void);
void UninitPoint(void);
void UpdatePoint(void);
void DrawPoint(void);
void SetPoint(void);
void SetPointRound(int* nIdx);

#endif