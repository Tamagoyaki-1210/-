//================================//
//								  //
//       影のヘッダー			  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _SHADOW_H_		//このマクロが定義されてなかったら
#define _SHADOW_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//マクロ定義
#define MAX_SHADOW			(8)				//影の最大数

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;						//ディスクの現在の位置
	D3DXVECTOR3 move;						//ディスクの移動量
	D3DXVECTOR3 acc;						//ディスクの加速
	float fSize;							//ディスクの大きさ
	bool bUse;								//使用されているかどうか
}Shadow;

//プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, float size);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
Shadow *GetShadow(void);

#endif#pragma once
