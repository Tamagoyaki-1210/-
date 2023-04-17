//==========================================//
//										    //
//       上投げの落ちる位置のヘッダー	    //
//       Author: Ricci Alex				    //
//										    //
//==========================================//

#ifndef _LANDING_POINT_H_		//このマクロが定義されてなかったら
#define _LANDING_POINT_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//マクロ定義
#define MAX_LANDING_POINT			(1)			//上投げの落ちる位置のマークの最大数

//構造体
typedef struct
{
	D3DXVECTOR3 pos;
	int nCntAnim;
	int nAnimPattern;
	float fSize;
	bool bUse;
}LandingMark;

//プロトタイプ宣言
void InitLandingMark(void);
void UninitLandingMark(void);
void UpdateLandingMark(void);
void DrawLandingMark(void);
void SetLandingMark(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeight, float fSpeed, float fAcc, float fSize);
void DestroyLandingMark(void);
LandingMark* GetLandingMark(void);

#endif