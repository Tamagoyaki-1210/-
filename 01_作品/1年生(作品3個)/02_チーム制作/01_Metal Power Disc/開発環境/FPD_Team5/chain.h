//================================//
//								  //
//       鎖のヘッダー		 	  //
//       Author: Ricci Alex		  //
//								  //
//================================//
#ifndef _CHAIN_H_		//このマクロが定義されてなかったら
#define _CHAIN_H_		//2重インクルード防止のマクロ定義

#include "main.h" 

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 size;				//サイズ
	bool bUse;						//使用されているかどうか
}Chain;

//プロトタイプ宣言
void InitChain(void);
void UninitChain(void);
void UpdateChain(void);
void DrawChain(void);
void SetChain(D3DXVECTOR3 pos, D3DXVECTOR3 size);

#endif