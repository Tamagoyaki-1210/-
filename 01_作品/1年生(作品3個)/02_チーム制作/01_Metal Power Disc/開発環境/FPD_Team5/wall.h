//================================//
//								  //
//       壁のヘッダー	    	  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _WALL_H_		//このマクロが定義されてなかったら
#define _WALL_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//マクロ定義
#define MAX_WALL			(2)			//壁の最大数

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;						//壁の現在の位置
	float fWidth;							//壁の幅
	float fHeight;							//壁の高さ
	float fAlpha;							//壁の回転角度
	D3DXVECTOR3 nor;						//壁の法線(当たり判定用)
	bool bUse;								//使用されているかどうか
}Wall;

//プロトタイプ宣言
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, float width, float height, float angle);
void WallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pAcc, float fRadius);
bool SpecialWallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, float fRadius);

#endif