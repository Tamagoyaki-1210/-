//=============================================================================
//
// タイトルメニュー
// title_menu.cpp
// Author : Tanimoto Kosuke
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "title_menu.h"
#include "application.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "confirmation_window.h"
#include "menu_window.h"
#include "connect.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CConfirmation_Window* CTitleMenu::m_pConfirmationWindow = nullptr;

//============================================================================
// コンストラクタ
//============================================================================
CTitleMenu::CTitleMenu()
{
}

//============================================================================
// デストラクタ
//============================================================================
CTitleMenu::~CTitleMenu()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CTitleMenu::Init()
{
	// 選択肢の設定処理
	SetChoice(CFontString::Create({ 400.0f, 400.0f, 0.0f}, { 40.0f, 40.0f }, "ゲームをはじめる"));
	SetChoice(CFontString::Create({ 800.0f / 2, 500.0f ,0.0f }, { 40.0f, 40.0f }, "ゲームをやめる"));

	// 全ての選択肢を表示する
	for (auto pChoice : GetChoiceAll())
	{
		pChoice->SetFontDraw(false);
	}

	// メニューの初期化処理
	CMenu::Init();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CTitleMenu::Uninit()
{
	// メニューの終了処理
	CMenu::Uninit();

	// 確認ウィンドウの終了処理
	if (m_pConfirmationWindow != nullptr)
	{
		m_pConfirmationWindow->Uninit();
		delete m_pConfirmationWindow;
		m_pConfirmationWindow = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CTitleMenu::Update()
{
	// メニューの更新処理
	CMenu::Update();

	// 選択肢の処理
	Choice();

	// 更新処理
	if (m_pConfirmationWindow != nullptr)
	{
		// 確認ウィンドウの更新処理
		m_pConfirmationWindow->Update();
	}
}

//============================================================================
// 選択肢の処理
//============================================================================
void CTitleMenu::Choice()
{
	// メニューの選択肢の処理
	CMenu::Choice();

	// 決定された場合
	if (GetDecition() == true)
	{
		// 現在選択されている選択肢の処理を読み込む
		switch ((Choice_Type)GetSelectChoice())
		{
			// ゲームスタート
		case Choice_GameStart:
		{
			// 決定SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
			// ゲームに飛ぶ
			CFade::SetFade(CApplication::MODE_CHAR_SELECT, 0.05f);
			CTitle::SetUninitWindow(false);
			//CApplication::GetClient()->Init("10.70.41.123", 15678);



			break;
		}
			// ウィンドウを閉じる
		case Choice_Window_Exit:
			// 決定SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

			if (m_pConfirmationWindow == nullptr)
			{// nullチェック

				// 確認ウィンドウの生成
				m_pConfirmationWindow = CConfirmation_Window::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f),500.0f, 320.0f, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));
				return;
			}
			break;
		default:
			break;
		}
	}
	// 決定を破棄する
	SetDecition(false);
}

//============================================================================
// 生成処理
//============================================================================
CTitleMenu* CTitleMenu::Create()
{
	CTitleMenu* pTitleMenu = new CTitleMenu;

	if (FAILED(pTitleMenu->Init()))
	{
		return nullptr;
	}

	return pTitleMenu;
}

//============================================================================
// 確認ウィンドウを破棄する処理
//============================================================================
void CTitleMenu::UninitComfirmationWindow()
{
	if (m_pConfirmationWindow != nullptr)
	{
		delete m_pConfirmationWindow;
		m_pConfirmationWindow = nullptr;
	}
}
