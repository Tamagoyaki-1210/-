//=============================================================================
//
// タイトルメニュー
// title_menu.h
// Author : Tanimoto Kosuke
// Author : Saito Shian
//
//=============================================================================
#ifndef _TITLE_MENU_H_
#define _TITLE_MENU_H_

//=============================================================================
// インクルード
//=============================================================================
#include "menu.h"
#include "object2D.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CConfirmation_Window;

//=============================================================================
// タイトルメニュークラス宣言
//=============================================================================
class CTitleMenu : public CMenu
{
public:
	enum Choice_Type
	{
		Choice_GameStart,			// ゲームスタート
		Choice_Window_Exit,			// ウィンドウを閉じる
		Choice_Max,					// 最大選択数
		Choice_Invalid = -1,		// 何も呼ばれていない
	};

	CTitleMenu();					// コンストラクタ
	~CTitleMenu() override;			// デストラクタ

	HRESULT Init() override;		// 初期化処理
	void Uninit() override;			// 終了処理
	void Update() override;			// 更新処理

	void Choice() override;			// 選択肢の処理

	static CConfirmation_Window* GetComfirmationWindow() { return m_pConfirmationWindow; }	// メニューウィンドウの取得処理
	static void UninitComfirmationWindow();													// メニューウィンドウの破棄処理

	static CTitleMenu* Create();	// 生成

private:
	static CConfirmation_Window* m_pConfirmationWindow;	// メニューウィンドウのインスタンス
};

#endif // !_TITLE_MENU_H_