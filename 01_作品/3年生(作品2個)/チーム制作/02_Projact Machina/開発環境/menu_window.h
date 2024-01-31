//=============================================================================
//
// menu_window.h (メニューウィンドウ.h)
// Author : Saito Shian
//
//=============================================================================
#ifndef  _MENU_WINDOW_H_				//このマクロ定義がされてなかったら
#define  _MENU_WINDOW_H_				//二重インクルード防止のマクロ定義

//=============================================================================
// インクルード
//=============================================================================
#include "main.h"
#include "object.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CObject2D;

//=============================================================================
// メニューウィンドウクラス
//=============================================================================
class CMenuWindow
{
	const float SizeXScaleSpeed = 31.8f;	// Xサイズ拡大縮小のスピード
	const float SizeYScaleSpeed = 27.5f;	// Yサイズ拡大縮小のスピード
	const float SizeXScaleMax = 500.0f;		// Xサイズの最大サイズ
	const float SizeYScaleMax = 220.0f;		// Yサイズの最大サイズ

public:
	CMenuWindow();							// メニューウィンドウのコンストラクタ
	~CMenuWindow();							// メニューウィンドウのデストラクタ

	HRESULT Init();							// 初期化処理
	void Uninit();							// 終了処理
	void Update();							// 更新処理

	void MenuWindowScale();					// メニューのサイズ処理
	bool MenuWindowScaleReduce();			// メニューのサイズ縮小処理

	static CMenuWindow* Create();			// 生成

private:
	CObject2D* m_pMenuWindow;				// メニューウィンドウ用object2Dのインスタンス
	D3DXVECTOR2 m_MenuWindowSize;			// メニューウィンドウのサイズ

	bool m_bMenuWindowMaxSize;				// 最大サイズまで行ったのか
	bool m_bMenuWindowScaleFlag;			// 拡大縮小フラグ
};
#endif

