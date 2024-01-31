//=============================================================================
//
// menugame.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "menugame.h"
#include "Application.h"
#include "input.h"
#include "fade.h"
#include "ui.h"
#include "game.h"
#include "time.h"
#include "entry.h"
#include "charmanager.h"

//=====================================
// コンストラクタ
//=====================================
CMenuGame::CMenuGame()
{
	m_bDisplay = false;			// 画面表示の判定
	m_bCurrentDisplay = false;	// 前回の画面表示の判定
}

//=====================================
// デストラクタ
//=====================================
CMenuGame::~CMenuGame()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CMenuGame::Init()
{
	// フェード擬きの生成
	m_pFade = CObject2D::Create({ 640.0f, 360.0f, 0.0f }, { 910.0f, 510.0f, 0.0f }, 5);
	m_pFade->SetCol({0.0f, 0.0f, 0.0f, 0.25f});
	m_pFade->SetDrawFlag(false);

	// 選択肢の設定処理
	SetChoice(CFontString::Create({ 420.0f, 400.0f, 0.0f }, { 40.0f, 40.0f, 0.0f }, "ゲームをつづける", 5));
	SetChoice(CFontString::Create({ 360.0f, 480.0f, 0.0f }, { 40.0f, 40.0f, 0.0f }, "キャラセレクトにもどる", 5));
	SetChoice(CFontString::Create({ 420.0f, 560.0f, 0.0f }, { 40.0f, 40.0f, 0.0f }, "タイトルにもどる", 5));

	// 全ての選択肢を表示する
	for (auto pChoice : GetChoice())
	{
		pChoice->SetFontDraw(false);
	}

	// ポーズロゴの生成
	m_pPauceLogo = CFontString::Create({ 440.0f, 200.0f, 0.0f }, { 100.0f, 100.0f, 0.0f }, "PAUSE", 5);
	for(auto pFont : m_pPauceLogo->GetFontAll()) pFont->SetDrawFlag(false);

	// メニューの初期化処理
	CMenu::Init();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMenuGame::Uninit()
{
	// メニューの終了処理
	CMenu::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CMenuGame::Update()
{
	// ポーズの処理
	Pauce();

	// 画面表示の処理
	Display();

	// ポーズ中のみ更新する処理
	if (CApplication::GetPauce() == true)
	{
		// メニューの更新処理
		CMenu::Update();

		// 選択肢の処理
		Choice();

		// 停止中のポーズオブジェクトの更新処理
		PauceObjUpdate();
	}
}

//============================================================================
// 停止中のポーズオブジェクトの更新処理
//============================================================================
void CMenuGame::PauceObjUpdate()
{
	// ポーズロゴの更新
	m_pPauceLogo->Update();

	// フェード擬きの更新
	m_pFade->Update();

	// 選択肢の更新
	for (auto pChoice : GetChoice())
	{
		pChoice->Update();
		for (auto pFont : pChoice->GetFontAll())
		{
			pFont->Update();
		}
	}
}

//============================================================================
// ポーズの処理
//============================================================================
void CMenuGame::Pauce()
{
	// フェード中では無い && ゲーム時間が終了していない場合
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE && CGame::GetTime()->GetTime() != 0)
	{
		// 入力デバイスの情報
		CInput* pInput = CInput::GetKey();

		// ボタンが押されたかどうか
		bool bPressP = false;

		// ポーズ判定
		const bool bPauce = CApplication::GetPauce();

		// ポーズ中ではない場合
		if (bPauce == false)
		{
			// コントローラー4つ分の処理
			for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
			{
				// ポーズの設定処理
				if ((pInput->Trigger(DIK_P) && nCnt == 0) || pInput->Trigger(JOYPAD_START, nCnt))
				{
					bPressP = true;

					// メニューの主導権を設定
					SetMenuInitiative(nCnt);

					// ゲーム停止(笛_停止SE)
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_STOP);

					break;
				}
			}
		}
		// ポーズ中の場合
		else
		{
			// ポーズの設定処理
			if ((pInput->Trigger(DIK_P)) || pInput->Trigger(JOYPAD_START, GetMenuInitiative()))
			{
				bPressP = true;

				// ゲーム開始(笛_開始SE)
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_START);
			}
		}

		// ポーズが押された場合
		if (bPressP == true)
		{
			// 現在のポーズ判定を反転させる
			CApplication::SetPauce(!bPauce);

			// ポーズ中で無い場合は表示しない
			m_bDisplay = !m_bDisplay;

			// 選択肢の情報を初期化する
			ChangeChoice(0);
		}
	}
}

//============================================================================
// 画面表示の処理
//============================================================================
void CMenuGame::Display()
{
	// 表示の更新があった場合
	if (m_bDisplay != m_bCurrentDisplay)
	{
		// 現在の表示状態を更新する
		m_bCurrentDisplay = m_bDisplay;

		// ポーズロゴが使用中の場合
		if (m_pPauceLogo != nullptr)
		{
			// 各メニュー内のオブジェクトの表示状態を更新する
			for (auto pFont : m_pPauceLogo->GetFontAll()) pFont->SetDrawFlag(m_bDisplay);

			// フェード擬きの表示を設定する
			m_pFade->SetDrawFlag(m_bDisplay);

			// 選択肢の表示を設定する
			for (auto pChoice : GetChoice())
			{
				pChoice->SetFontDraw(m_bDisplay);
			}
		}
	}
}

//============================================================================
// 選択肢の処理
//============================================================================
void CMenuGame::Choice()
{
	// 決定された場合
	if (GetDecition() == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		// 現在選択されている選択肢の処理を読み込む
		switch ((Choice_Type)GetSelectChoice() + 1)
		{
		// ゲームを続ける
		case Choice_Continue:
			// 笛_開始SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_START);

			// 現在のポーズ状態を解除する
			CApplication::SetPauce(false);

			// ポーズ中で無い場合は表示しない
			m_bDisplay = !m_bDisplay;

			// 決定を破棄する
			SetDecition(false);
			break;

		// エントリー画面へ
		case Choice_Entry:
			// 笛_停止SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_STOP);

			CFade::SetFade(CApplication::MODE_ENTRY, 0.05f);
			break;

		// タイトル画面へ
		case Choice_Title:
			// 笛_停止SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_STOP);

			CFade::SetFade(CApplication::MODE_TITLE, 0.05f);
			break;
		default:
			break;
		}
	}
}

//============================================================================
// 生成処理
//============================================================================
CMenuGame* CMenuGame::Create()
{
	CMenuGame* pMenu = new CMenuGame;

	if (FAILED(pMenu->Init()))
	{
		return nullptr;
	}

	return pMenu;
}
