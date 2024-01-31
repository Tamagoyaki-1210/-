//=============================================================================
//
// menu_window.h (メニューウィンドウ.h)
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "menu_window.h"
#include "object2D.h"
#include "menu.h"
#include "title.h"
#include "title_menu.h"
#include "confirmation_window.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CMenuWindow::CMenuWindow()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CMenuWindow::~CMenuWindow()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMenuWindow::Init()
{
	//==================================================
	// メンバ変数の初期化
	//==================================================  
	m_MenuWindowSize = D3DXVECTOR2(0.0f, 0.0f);
	m_bMenuWindowScaleFlag = false;
	m_pMenuWindow = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 450.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),CObject::PRIORITY_FRONT);
	m_pMenuWindow->SetCol(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMenuWindow::Uninit()
{
	//メンバ変数の初期化
	if (m_pMenuWindow != nullptr)
	{
		m_pMenuWindow->Uninit();
		m_pMenuWindow = nullptr;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CMenuWindow::Update()
{
	if (CTitle::GetUninitWindow() == false && m_bMenuWindowScaleFlag == false)
	{ 
		// メニューのサイズ拡大処理
		MenuWindowScale();
	}
}

//=============================================================================
// メニューのサイズ拡大処理
//=============================================================================
void CMenuWindow::MenuWindowScale()
{
	if (m_pMenuWindow != nullptr)
	{// nullチェック
		// サイズの拡大
		m_MenuWindowSize.x += SizeXScaleSpeed;
		m_MenuWindowSize.y += SizeYScaleSpeed;

		if (m_MenuWindowSize.x >= SizeXScaleMax)
		{// Xサイズの最大
			m_MenuWindowSize.x = SizeXScaleMax;
		}
		if (m_MenuWindowSize.y >= SizeYScaleMax)
		{// Yサイズの最大
			m_MenuWindowSize.y = SizeYScaleMax;
		}

		// ウィンドウが最大値まで行ったら
		if (m_MenuWindowSize.x >= SizeXScaleMax && m_MenuWindowSize.y >= SizeYScaleMax)
		{
			CApplication::GetMenu()->SetDisplay(true);		// メニュー画面の表示
			CTitle::SetWindow(true);						// ウィンドウを使用している状態にする
			m_bMenuWindowScaleFlag = true;					// 拡大された
		}

		// サイズの設定
		m_pMenuWindow->SetSize(D3DXVECTOR2(m_MenuWindowSize.x, m_MenuWindowSize.y));
	}
}

//=============================================================================
// メニューのサイズ縮小処理処理
//=============================================================================
bool CMenuWindow::MenuWindowScaleReduce()
{
	if (m_pMenuWindow != nullptr)
	{// nullチェック
		// サイズの縮小
		m_MenuWindowSize.x -= SizeXScaleSpeed;
		m_MenuWindowSize.y -= SizeYScaleSpeed;

		if (m_MenuWindowSize.x <= 0.0f)
		{// Xサイズの最小
			m_MenuWindowSize.x = 0.0f;
		}
		if (m_MenuWindowSize.y <= 0.0f)
		{// Yサイズの最小
			m_MenuWindowSize.y = 0.0f;
		}

		// ウィンドウが最小値まで行ったら
		if (m_MenuWindowSize.x <= 0.0f && m_MenuWindowSize.y <= 0.0f)
		{
			// ウィンドウ生成の設定処理
			CTitleMenu::GetComfirmationWindow()->SetSapawnWindow(false);

			if (CTitleMenu::GetComfirmationWindow()->GetSelectChoice() == true)
			{// ゲームをやめたら

				// ウィンドウの削除
				DestroyWindow(CApplication::GetWindow());
			}

			CTitle::UninitMenuWindow();					// メニューウィンドウを破棄する処理		
			CTitleMenu::UninitComfirmationWindow();		// 確認ウィンドウを破棄する処理
			return true;
		}

		// サイズの設定
		m_pMenuWindow->SetSize(D3DXVECTOR2(m_MenuWindowSize.x, m_MenuWindowSize.y));
	}
	return false;
}

//=============================================================================
// タイムの生成
//=============================================================================
CMenuWindow* CMenuWindow::Create()
{
	//クラスの生成
	CMenuWindow* pMenuWindow = new CMenuWindow;

	//nullチェック
	if (pMenuWindow != nullptr)
	{
		//初期化処理
		pMenuWindow->Init();
	}
	else
	{
		assert(false);
	}

	return pMenuWindow;
}