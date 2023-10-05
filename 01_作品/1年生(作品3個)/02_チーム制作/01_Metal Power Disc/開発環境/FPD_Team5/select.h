//============================================
//
//チーム制作<フライングパワーディスク>[select.h]
//Author:Kishimoto Eiji
//
//============================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "main.h"

//***********************************
//制限時間の列挙型
//***********************************
typedef enum
{
	TIMELIMIT_15 = 0,		//15秒
	TIMELIMIT_30,			//30秒
	TIMELIMIT_45,			//45秒
	TIMELIMIT_90,			//90秒
	TIMELIMIT_INFINITY,		//無限(制限時間なし)
	TIMELIMIT_MAX
}TIMELIMIT;

//***********************************
//ポイント数の列挙型
//***********************************
typedef enum
{
	POINTCOUNT_12 = 0,	//12点
	POINTCOUNT_15,		//15点
	POINTCOUNT_21,		//21点
	POINTCOUNT_25,		//25点
	POINTCOUNT_MAX
}POINTCOUNT;

//***********************************
//セット数の列挙型
//***********************************
typedef enum
{
	SETCOUNT_ONE = 0,	//1セット
	SETCOUNT_TWO,		//2セット
	SETCOUNT_THREE,		//3セット
	SETCOUNT_FOUR,		//4セット
	SETCOUNT_MAX
}SETCOUNT;

//***********************************
//プロトタイプ宣言
//***********************************

void InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);

int SelectTimeLimit(int nSelectMenu);
int SelectPoint(int nSelectMenu);
int SelectSetCount(int nSelectMenu);

#endif