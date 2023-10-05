//============================================
//
//チーム制作<フライングパワーディスク>[option.h]
//Author: goto yuuki
//Author:Kishimoto Eiji
//
//============================================
#ifndef _OPTION_H_		//このマクロが定義されてなかったら
#define _OPTION_H_		//2重インクルード防止のマクロ定義

#include "main.h"

//***********************************
//オプションの列挙型
//***********************************
typedef enum
{
	OPTION_TIMELIMIT = 0,	//制限時間
	OPTION_POINT,			//ポイント数
	OPTION_SETCOUNT,		//セット数
	OPTION_GOTOGAME,		//ゲームへ進む
	OPTION_MAX
}OPTION;

//***********************************
//プロトタイプ宣言
//***********************************

void InitOption(void);
void UninitOption(void);
void UpdateOption(void);
void DrawOption(void);
void SetOption(void);
int GetSelectMenuNow(void);
int GetSelectMenuBefore(void);

#endif
