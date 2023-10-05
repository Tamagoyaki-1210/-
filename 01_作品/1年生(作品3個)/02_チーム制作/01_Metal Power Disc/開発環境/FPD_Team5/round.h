//=========================================
// 
// ラウンド処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _ROUND_H_
#define _ROUND_H_

#include "main.h"

//------------------------------------
//ポイントの構造体
//------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 pos;					//現在の位置
	int nRound;							//スコアの値
	bool bUse;
}Round;

//------------------------------------
//プロトタイプ宣言
//------------------------------------
void InitRound(void);
void UninitRound(void);
void UpdateRound(void);
void DrawRound(void);
void SetRound(void);

#endif