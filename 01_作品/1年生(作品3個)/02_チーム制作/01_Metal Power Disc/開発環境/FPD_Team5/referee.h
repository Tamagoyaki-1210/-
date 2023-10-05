//=========================================
// 
// レフェリー(ヘッダーファイル)
// Author Tanimoto_Kosuke
//
// Update 22/03/14
// 
//=========================================
#ifndef _REFEREE_H_
#define _REFEREE_H_

#include "main.h"

//****************************************************************************
// 構造体の定義
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;	// 回転
	D3DXVECTOR3 nor;	// 法線
	D3DXCOLOR	col;	// 色
	float fAngle;		// 角度(アークタンジェント)
	float fLength;		// 長さ(スクウェアルート)
	bool bThrow;		// 投げるか否か
	bool bUse;			// 使用判定
}REFEREE;

//****************************************************************************
// プロトタイプ宣言
//****************************************************************************
void InitReferee(void);
void UninitReferee(void);
void UpdateReferee(void);
void DrawReferee(void);
REFEREE *GetReferee(void);
void SetThoThrowRefreeIdx(int nIdx);

#endif