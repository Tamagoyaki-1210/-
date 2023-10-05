//================================//
//								  //
//       配管のヘッダー		 	  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _PIPE_H_		//このマクロが定義されてなかったら
#define _PIPE_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//マクロ定義
#define MAX_PIPE			(16)			//配管の最大数

typedef enum
{
	PIPE_TYPE_STEAM = 0,
	PIPE_TYPE_VERTICAL,
	PIPE_TYPE_MAX
}PIPE_TYPE;

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 size;				//サイズ
	int nType;						//種類
	int nDelay;						//アニメーション用の変数
	int nCntAnim;					//アニメーションカウンター
	int nCntMove;					//移動用の変数
	float fRand;					//アニメーション移動用の変数
	int nScored;					
	bool bUse;						//使用されているかどうか
}Pipe;

//プロトタイプ宣言
void InitPipe(void);
void UninitPipe(void);
void UpdatePipe(void);
void DrawPipe(void);
void PipeAnimation(int nCntPipe);

//位置・サイズ・一フレームの回転角度・種類
void SetPipe(D3DXVECTOR3 pos, D3DXVECTOR3 size, PIPE_TYPE type);

void PointSmokeAnimation(int nPlayer);

#endif