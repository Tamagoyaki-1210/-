//=============================================================================
//
// 計算用マトリクス
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _MATRIX_H_
#define _MATRIX_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

float Normalize(float val);
D3DXVECTOR3 NormalizeLength(float valX, float valY);
bool CollisionCircle(D3DXVECTOR3 pos, float fLength, D3DXVECTOR3 posSub, float fLengthSub);		//円型当たり判定計算処理
bool CollisionSquare(D3DXVECTOR3 pos, D3DXVECTOR3 posSub, float fWidth, float fHeight, float fWidthSub, float fHeightSub);	//四角形当たり判定計算処理

#endif // !_MATRIX_H_
