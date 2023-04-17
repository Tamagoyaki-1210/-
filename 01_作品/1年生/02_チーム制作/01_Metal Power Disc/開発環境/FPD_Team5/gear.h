//================================//
//								  //
//       歯車のヘッダー		 	  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _GEAR_H_		//このマクロが定義されてなかったら
#define _GEAR_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//マクロ定義
#define MAX_GEAR			(128)			//はぐるまの最大数

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	float rot;						//回転角度
	float fAngle;					//回転用の角度
	float fLenght;					//回転用の長さ
	float fSize;					//サイズ
	float fFrameRot;				//一フレームの回転角度
	int nType;						//種類
	bool bUse;						//使用されているかどうか
}Gear;

//プロトタイプ宣言
void InitGear(void);
void UninitGear(void);
void UpdateGear(void);
void DrawGear(void);

//位置・サイズ・一フレームの回転角度・種類
void SetGear(D3DXVECTOR3 pos, float size, float frameRot, int type);
void SetDarkerGear(D3DXVECTOR3 pos, D3DXCOLOR col, float size, float frameRot, int type);

#endif