//------------------------------
//時間設定のヘッダー
//ACG000
//Author: goto yuuki
//
//------------------------------
#ifndef _TIME_H_		//このマクロ定義がされてなかったら
#define _TIME_H_		//２重インクルード防止のマクロ定義

#include "main.h"
#include "fade.h"

//マクロ定義
#define MAX_TIME		(30)							//最大タイム設定数
#define MAX_CNT_TIME	(2)								//最大カウンター数

//タイムの構造体
typedef struct
{
	D3DXVECTOR3 pos[MAX_CNT_TIME];									//現在の位置
	int nTime;											//タイムの設定数
	int nMinusTime;										//タイムを引く数
	bool bTimeFlag;										//タイトルの移動のフラグ
}TIME;

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
TIME *GetTime(void);
void SetTime(int nTime);
#endif