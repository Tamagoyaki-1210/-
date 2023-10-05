//=============================================================================
//
// menu.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "menu.h"
#include "Application.h"
#include "input.h"
#include "fade.h"
#include "charmanager.h"
#include "entry.h"

const float CMenu::Choice_Blink_Speed = 0.02f;	// 選択肢の点滅速度
const float CMenu::Choice_Mini_Alpha = 0.5f;	// 選択肢の最低透明値
//=====================================
// コンストラクタ
//=====================================
CMenu::CMenu() : m_nSelectChoice(0), m_fBlinkSpeed(Choice_Blink_Speed), m_bDecition(false)
{

}

//=====================================
// デストラクタ
//=====================================
CMenu::~CMenu()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CMenu::Init()
{
	// 選択肢が使用されている場合
	if (!m_pListChoice.empty())
	{
		for (auto pChoice : GetChoice())
		{
			pChoice->SetAlpha(Choice_Mini_Alpha);
		}
		// 選択されている選択肢
		m_pListChoice[m_nSelectChoice]->SetBlink(Choice_Blink_Speed);
		m_pListChoice[m_nSelectChoice]->SetAlpha(1.0f);
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
	Input();
}

//============================================================================
// 入力処理
//============================================================================
void CMenu::Input()
{
	// フェード中では無い場合
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		// 選択肢が使用されている場合
		if (!m_pListChoice.empty())
		{
			// 入力デバイスの情報
			CInput* pInput = CInput::GetKey();

			// 選択肢が2つ以上使用されている場合
			if (m_pListChoice.size() >= 2)
			{
				// 上に移動する
				if (pInput->Trigger(DIK_W) || (pInput->Trigger(JOYPAD_UP, m_nMenuInitiative)))
				{
					// 選択SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// 現在の番号が0以下である場合
					ChangeChoice(m_nSelectChoice <= 0 ? m_pListChoice.size() - 1 : m_nSelectChoice - 1);
				}
				// 下に移動する
				else if (pInput->Trigger(DIK_S) || (pInput->Trigger(JOYPAD_DOWN, m_nMenuInitiative)))
				{
					// 選択SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// 現在の番号が最大数以上の場合
					ChangeChoice(m_nSelectChoice >= ((int)m_pListChoice.size() - 1) ? 0 : m_nSelectChoice + 1);
				}
			}

			if (pInput->Trigger(DIK_RETURN) || (pInput->Trigger(JOYPAD_B, m_nMenuInitiative) || pInput->Press(JOYPAD_A, m_nMenuInitiative)))
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
	m_pListChoice[m_nSelectChoice]->BlinkFinish();
	m_pListChoice[m_nSelectChoice]->SetAlpha(Choice_Mini_Alpha);

	// 選択肢の変更
	m_nSelectChoice = nextChoice;

	// 変更後の選択肢の処理
	m_pListChoice[m_nSelectChoice]->SetBlink(m_fBlinkSpeed);
	m_pListChoice[m_nSelectChoice]->SetAlpha(1.0f);
}