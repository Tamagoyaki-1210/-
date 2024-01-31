//=============================================================================
//
// confirmation_window.h (確認ウィンドウ.h)
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "confirmation_window.h"
#include "object2D.h"
#include "menu.h"
#include "fade.h"
#include "input.h"
#include "game.h"
#include "title.h"
#include "menu_window.h"
#include "title_menu.h"
#include "char_select.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CCharSelect_Window* CConfirmation_Window::m_pCharSelectWindow = nullptr;
const float CConfirmation_Window::BLINK_SPEED = 0.02f;	// 選択肢の点滅速度
const float CConfirmation_Window::MINI_ALPHA = 0.5f;	// 選択肢の最低透明値
bool CConfirmation_Window::m_bConfimationSelectChoice = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CConfirmation_Window::CConfirmation_Window() : m_fConfimationBlinkSpeed(BLINK_SPEED)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CConfirmation_Window::~CConfirmation_Window()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CConfirmation_Window::Init()
{
	//==================================================
	// メンバ変数の初期化
	//==================================================
	VariableInit();
	m_Size = D3DXVECTOR2(0.0f, 0.0f);
	m_pObject2D = CObject2D::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), D3DXVECTOR2(0.0f, 0.0f), CObject::PRIORITY_SCREEN);
	m_pObject2D->SetCol(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_Color.a));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CConfirmation_Window::Uninit()
{
	//メンバ変数の破棄
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Uninit();
		m_pObject2D = nullptr;
	}

	//// キャラ選択画面の破棄
	//if (m_pCharSelectWindow != nullptr)
	//{
	//	m_pCharSelectWindow->Uninit();
	//	delete m_pCharSelectWindow;
	//	m_pCharSelectWindow = nullptr;
	//}

	// フォントの削除
	if (m_pFont != nullptr)
	{
		m_pFont->Uninit();
		m_pFont = nullptr;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CConfirmation_Window::Update()
{
	if (m_bConfimationScale == false && m_bConfirmasionWindowUse == false)
	{ // 拡大されていない時 && ウィンドウが使われていないとき

		// 確認ウィンドウの拡大処理
		ConfirmatiomnMenuScale();
	}
	else if(m_bConfimationScale == true && m_bConfirmasionWindowUse == false)
	{ // 拡大した時 && ウィンドウが使われていないとき

		// 確認ウィンドウの縮小処理
		ConfirmatiomnMenuScaleReduce();
	}
	// 選択肢処理
	Choice();
}

//=============================================================================
// 確認ウィンドウのサイズ拡大処理
//=============================================================================
void CConfirmation_Window::ConfirmatiomnMenuScale()
{
	if (m_pObject2D != nullptr)
	{ // nullチェック

		// サイズの拡大
		m_Size.x += SizeXScaleSpeed;
		m_Size.y += SizeYScaleSpeed;

		if (m_Size.x >= m_MaxSize.x)
		{ // Xサイズの最大
			m_Size.x = m_MaxSize.x;
		}
		if (m_Size.y >= m_MaxSize.y)
		{ // Yサイズの最大
			m_Size.y = m_MaxSize.y;
		}

		// ウィンドウが最大値まで行ったら
		if (m_Size.x >= m_MaxSize.x && m_Size.y >= m_MaxSize.y && m_bConfimationMaxSize == false)
		{
			// ゲームモードの取得
			CApplication::MODE Mode = CApplication::GetModeType();

			if (Mode == CApplication::MODE_TITLE || Mode == CApplication::MODE_GAME)
			{ // タイトルかゲーム画面だったら
				
				// 文字の生成
				if (m_pFont == nullptr)                     
				{
					m_pFont = CFontString::Create({ 420.0f, 350.0f, 0.0f }, { 30.0f, 30.0f }, "やめますか?");
				}

				// 選択肢の生成
				SetChoice(CFontString::Create({ 400.0f, 450.0f, 0.0f }, { 25.0f, 25.0f }, "はい"));
				SetChoice(CFontString::Create({ 700.0f, 450.0f, 0.0f }, { 25.0f, 25.0f }, "いいえ"));
			}

			// 選択肢が使用されている場合
			if (!m_vpListChoice.empty())
			{
				for (auto pChoice : GetChoiceAll())
				{
					pChoice->SetAlpha(MINI_ALPHA);
				}
				// 選択されている選択肢
				m_vpListChoice[m_bConfimationSelectChoice]->SetBlink(BLINK_SPEED);
				m_vpListChoice[m_bConfimationSelectChoice]->SetAlpha(1.0f);
			}

			if (m_bConfimationBack == true)
			{ // ウィンドウが戻ってきたら
				VariableInit();					// メンバ変数の初期化処理
				ChangeChoice(0);				// 選択肢の情報を初期化する
				UninitCharSelectWindow();		// キャラ選択ウィンドウの破棄処理
			}

			SetDisplay(true);					// 選択肢の表示判定を使用
			m_bConfimationMaxSize = true;		// 最大サイズに到達
			return;
		}

		// サイズの設定
		m_pObject2D->SetSize(D3DXVECTOR2(m_Size.x, m_Size.y));
	}
}

//=============================================================================
// 確認ウィンドウのサイズ縮小処理処理
//=============================================================================
bool CConfirmation_Window::ConfirmatiomnMenuScaleReduce()
{
	if (m_pObject2D != nullptr)
	{ // nullチェック

		// 現在のモード
		CApplication::MODE Mode = CApplication::GetModeType();

		// サイズの縮小
		m_Size.x -= SizeXScaleSpeed;
		m_Size.y -= SizeYScaleSpeed;

		if (m_Size.x <= 0.0f)
		{ // Xサイズの最小
			m_Size.x = 0.0f;
		}
		if (m_Size.y <= 0.0f)
		{ // Yサイズの最小
			m_Size.y = 0.0f;
		}

		// フォントの削除
		if (m_pFont != nullptr)
		{
			m_pFont->Uninit();
			m_pFont = nullptr;
		}

		// ウィンドウが最小値まで行ったら
		if (m_Size.x <= 0.0f && m_Size.y <= 0.0f)
		{
			// ゲーム終了
			if (m_bConfimationSelectChoice == true && Mode == CApplication::MODE_GAME)
			{ // やめる選択肢を選んだ && モードがゲーム画面だったら

				m_bConfimationUninitFlag = true;				// ゲーム内ウィンドウの破棄
				m_bConfimationSpawnWindow = true;				// メニューウィンドウの破棄
				DestroyWindow(CApplication::GetWindow());		// ウィンドウの破棄
			}
			else if (m_bConfimationSelectChoice == false && Mode == CApplication::MODE_GAME)
			{ // やめない選択肢を選んだ && モードがゲーム画面だったら

				m_bConfimationUninitFlag = true;				// ゲーム内ウィンドウの破棄
				m_bConfimationSpawnWindow = true;				// メニューウィンドウの破棄
				m_bConfimationScale = false;					// 拡大縮小フラグを初期化			
				Uninit();										// メニューウィンドウの削除			
			}
			if (Mode == CApplication::MODE_TITLE)
			{ // タイトル画面だったら

				m_bConfimationScale = false;					// 拡大縮小フラグを初期化	
				m_bConfimationSpawnWindow = true;				// メニューウィンドウの破棄
				Uninit();										// メニューウィンドウの削除
			}
			if (Mode == CApplication::MODE_CHAR_SELECT && m_bConfirmasionWindowUse == false)
			{ // キャラ選択画面だったら && このウィンドウが使われていなかったら

				m_bConfirmasionWindowUse = true;	// 使用している様態にする
			}
			return true;
		}

		// サイズの設定
		m_pObject2D->SetSize(D3DXVECTOR2(m_Size.x, m_Size.y));
	}
	return false;
}

//============================================================================
// 選択肢の処理
//============================================================================
void CConfirmation_Window::Choice()
{
	// 入力デバイスの情報
	CInput* pInput = CInput::GetKey();
	
	// フェード中では無い場合 && 表示中の場合
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE && m_bConfimationDisplay == true)
	{
		// 選択肢が使用されている場合
		if (!m_vpListChoice.empty())
		{
			// 選択肢が2つ以上使用されている場合
			if (m_vpListChoice.size() >= 2)
			{
				// 上に移動する
				if (pInput->Trigger(DIK_A) || (pInput->Trigger(JOYPAD_UP)))
				{
					// 選択SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// 現在の番号が0以下である場合
					ChangeChoice(false);
				}
				// 下に移動する
				else if (pInput->Trigger(DIK_D) || (pInput->Trigger(JOYPAD_DOWN)))
				{
					// 選択SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// 現在の番号が最大数以上の場合
					ChangeChoice(true);
				}
			}

			if (pInput->Trigger(DIK_RETURN) || (pInput->Trigger(JOYPAD_B) || pInput->Trigger(JOYPAD_A)))
			{
				// 決定SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

				// 選択を決定する
				m_bConfimationDecition = true;
			}
			// 決定された場合
			if (m_bConfimationDecition == true)
			{
				// 左の選択肢の場合
				if (m_bConfimationSelectChoice == false)
				{
					CreateCharSelect();					// キャラ選択画面の生成
					m_bConfimationScale = true;			// 拡大縮小フラグが使われている
					SetDisplay(false);					// 選択肢の表示判定を使用していない状態にする
					m_bConfimationSelectChoice = true;	// 左の選択肢を選んだ
				}
				// 右の選択肢の場合
				else
				{
					CreateCharSelect();					// キャラ選択画面の生成
					m_bConfimationScale = true;         // 拡大縮小フラグが使われている
					SetDisplay(false);					// 選択肢の表示判定を使用していない状態にする
					m_bConfimationSelectChoice = false; // 左の選択肢を選んだ
				}
			}
		}
	}

	//if (m_pCharSelectWindow != nullptr)
	//{ //	キャラ選択ウィンドウが使われていたら

	//	 キャラ選択ウィンドウの更新処理
	//	m_pCharSelectWindow->Update();
	//}

	if (m_bConfimationSpawnWindow == true && CTitle::GetMenuWindow() != nullptr)
	{ // メニューウィンドウが使われている時 &&  メニューウィンドウが使われている時

		CApplication::GetMenu()->SetDisplay(false);				// 文字を消す
		CTitle::GetMenuWindow()->MenuWindowScaleReduce();		// ゲーム開閉時のウィンドウの縮小
	}
}

//============================================================================
// 選択肢を変更した時の処理
//============================================================================
void CConfirmation_Window::ChangeChoice(const bool nextChoice)
{
	// 現在の選択肢の処理
	m_vpListChoice[m_bConfimationSelectChoice]->BlinkFinish();
	m_vpListChoice[m_bConfimationSelectChoice]->SetAlpha(MINI_ALPHA);

	// 選択肢の変更
	m_bConfimationSelectChoice = (int)nextChoice;

	// 変更後の選択肢の処理
	m_vpListChoice[m_bConfimationSelectChoice]->SetBlink(m_fConfimationBlinkSpeed);
	m_vpListChoice[m_bConfimationSelectChoice]->SetAlpha(1.0f);
}

//============================================================================
// メンバ変数の初期化
//============================================================================
void CConfirmation_Window::VariableInit()
{
	// メンバ変数の初期化
	m_bConfimationScale = false;
	m_bConfimationMaxSize = false;
	m_bConfimationUninitFlag = false;
	m_bConfimationSpawnWindow = false;
	m_bConfimationSelectChoice = false;
	m_bConfimationDecition = false;
	m_bConfirmasionWindowUse = false;
	m_bConfimationBack = false;
	m_bConfimationDisplay = false;
}

//============================================================================
// キャラ選択ウィンドウの破棄処理
//============================================================================
void CConfirmation_Window::UninitCharSelectWindow()
{	
	//// キャラ選択画面の破棄
	//if (m_pCharSelectWindow != nullptr)
	//{
	//	m_pCharSelectWindow->Uninit();
	//	delete m_pCharSelectWindow;
	//	m_pCharSelectWindow = nullptr;
	//}
}

//============================================================================
// 選択肢の表示判定を設定する
//============================================================================
void CConfirmation_Window::SetDisplay(const bool decition)
{
	m_bConfimationDisplay = decition;
	for (auto pChoice : GetChoiceAll()) { pChoice->SetFontDraw(decition); }

	// 表示を消す場合
	if (decition == false)
	{
		// 選択肢の情報を初期化する
		ChangeChoice(0);
	}
}

//=============================================================================
// キャラ選択画面の生成
//=============================================================================
void CConfirmation_Window::CreateCharSelect()
{
	//// 現在のモード
	//CApplication::MODE Mode = CApplication::GetModeType();		

	//if (Mode == CApplication::MODE_CHAR_SELECT)
	//{ // キャラ選択宅画面だったら

	//	// nullチェック
	//	if (m_pCharSelectWindow == nullptr)
	//	{
	//		// キャラ選択ウィンドウの生成
	//		m_pCharSelectWindow = CCharSelect_Window::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), 900.0f, 500.0f, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	//	}
	//}
}

//=============================================================================
// メニューの生成
//=============================================================================
CConfirmation_Window* CConfirmation_Window::Create(D3DXVECTOR3 pos, float xsize, float ysize, D3DXCOLOR col)
{
	//クラスの生成
	CConfirmation_Window* pMenuWindow = new CConfirmation_Window;

	//nullチェック
	if (pMenuWindow != nullptr)
	{
		// 設定処理
		pMenuWindow->m_pos = pos;
		pMenuWindow->m_Color = col;
		pMenuWindow->m_MaxSize.x = xsize;
		pMenuWindow->m_MaxSize.y = ysize;
		// 初期化処理
		pMenuWindow->Init();
	}
	else
	{
		assert(false);
	}

	return pMenuWindow;
}