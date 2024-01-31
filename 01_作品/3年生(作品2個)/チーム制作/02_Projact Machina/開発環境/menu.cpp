//=============================================================================
//
// menu.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "menu.h"
#include "Application.h"
#include "input.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
const float CMenu::CHOICE_BLINK_SPEED = 0.02f;	// 選択肢の点滅速度
const float CMenu::CHOICE_MINI_ALPHA = 0.5f;	// 選択肢の最低透明値

//=============================================================================
// コンストラクタ
//=============================================================================
CMenu::CMenu() : m_nSelectChoice(0), m_fBlinkSpeed(CHOICE_BLINK_SPEED), m_bDecition(false)
{
	// メンバ変数のクリア
	m_nMenuInitiative = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMenu::~CMenu()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CMenu::Init()
{
	// 選択肢が使用されている場合
	if (!m_vpListChoice.empty())
	{
		for (auto pChoice : GetChoiceAll())
		{
			pChoice->SetAlpha(CHOICE_MINI_ALPHA);
		}
		// 選択されている選択肢
		m_vpListChoice[m_nSelectChoice]->SetBlink(CHOICE_BLINK_SPEED);
		m_vpListChoice[m_nSelectChoice]->SetAlpha(1.0f);
	}

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMenu::Uninit()
{

}

//============================================================================
// 更新処理
//============================================================================
void CMenu::Update()
{

}

//============================================================================
// 選択肢の処理
//============================================================================
void CMenu::Choice()
{
	// フェード中では無い場合 && 表示中の場合
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE && m_bDisplay == true)
	{
		// 選択肢が使用されている場合
		if (!m_vpListChoice.empty())
		{
			// 入力デバイスの情報
			CInput* pInput = CInput::GetKey();

			// 選択肢が2つ以上使用されている場合
			if (m_vpListChoice.size() >= 2)
			{
				// 上に移動する
				if (pInput->Trigger(DIK_W) || (pInput->Trigger(JOYPAD_UP, m_nMenuInitiative)))
				{
					// 選択SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// 現在の番号が0以下である場合
					ChangeChoice(m_nSelectChoice <= 0 ? m_vpListChoice.size() - 1 : m_nSelectChoice - 1);
				}
				// 下に移動する
				else if (pInput->Trigger(DIK_S) || (pInput->Trigger(JOYPAD_DOWN, m_nMenuInitiative)))
				{
					// 選択SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// 現在の番号が最大数以上の場合
					ChangeChoice(m_nSelectChoice >= ((int)m_vpListChoice.size() - 1) ? 0 : m_nSelectChoice + 1);
				}
			}

			if (pInput->Trigger(DIK_RETURN) || (pInput->Trigger(JOYPAD_B, m_nMenuInitiative) || pInput->Trigger(JOYPAD_A, m_nMenuInitiative)))
			{
				// 決定SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

				// 選択を決定する
				m_bDecition = true;
			}
		}
	}
}

//============================================================================
// 選択肢を変更した時の処理
//============================================================================
void CMenu::ChangeChoice(const int nextChoice)
{
	// 現在の選択肢の処理
	m_vpListChoice[m_nSelectChoice]->BlinkFinish();
	m_vpListChoice[m_nSelectChoice]->SetAlpha(CHOICE_MINI_ALPHA);

	// 選択肢の変更
	m_nSelectChoice = nextChoice;

	// 変更後の選択肢の処理
	m_vpListChoice[m_nSelectChoice]->SetBlink(m_fBlinkSpeed);
	m_vpListChoice[m_nSelectChoice]->SetAlpha(1.0f);
}

//============================================================================
// 選択肢の表示判定を設定する
//============================================================================
void CMenu::SetDisplay(const bool display)
{
	m_bDisplay = display;
	for (auto pChoice : GetChoiceAll()) { pChoice->SetFontDraw(display); }

	// 表示を消す場合
	if (display == false)
	{
		// 選択肢の情報を初期化する
		ChangeChoice(0);
	}
}