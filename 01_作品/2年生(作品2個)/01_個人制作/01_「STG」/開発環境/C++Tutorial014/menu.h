//=============================================================================
//
// メニュー
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//クラス
//---------------------------
class CMenu : public CObject2D
{
public:
	//メニュー画像種類
	enum Menu_type
	{
		TYPE_NONE = 0,	// 何もしていない状態
		TYPE_FRAME,		// メニューの額縁
		TYPE_MAX
	};

	// メニューモード種類
	enum Menu_Mode
	{
		MODE_NONE = 0,
		MODE_TITLE,	//タイトル
		MODE_GAME,	//ゲーム
		MODE_MAX
	};

	CMenu();
	~CMenu() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetCreate(D3DXVECTOR3 pos);

	static CMenu *Create(D3DXVECTOR3 pos, Menu_type menu, Menu_Mode menuMode);
private:
	Menu_type m_menu;
	Menu_Mode m_menuMode;
};

#endif // !_MENU_H_
