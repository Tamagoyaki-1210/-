//=============================================================================
//
// debugProc.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _DEBUG_PROC_H_
#define _DEBUG_PROC_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"
#include <stdarg.h>
#include <string>

class CDebugProc
{
public:
	CDebugProc();										//コンストラクタ
	~CDebugProc();										//デストラクタ
														
	void Init(void);									//初期化処理
	void Uninit(void);									//終了処理
														
	static void Print(const char *pFormat, ...);		//文字列の設定処理
	static void Draw(void);								//描画処理

	static CDebugProc* Create(void);					//生成処理

private:

	static LPD3DXFONT m_pFont;							//フォント
	static std::string m_str;							//文字列
};


#endif