//=============================================================================
//
// menu.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "menu.h"
#include "Application.h"
#include "input.h"
#include "fade.h"
#include "charmanager.h"
#include "entry.h"

const float CMenu::Choice_Blink_Speed = 0.02f;	// �I�����̓_�ő��x
const float CMenu::Choice_Mini_Alpha = 0.5f;	// �I�����̍Œᓧ���l
//=====================================
// �R���X�g���N�^
//=====================================
CMenu::CMenu() : m_nSelectChoice(0), m_fBlinkSpeed(Choice_Blink_Speed), m_bDecition(false)
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CMenu::~CMenu()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CMenu::Init()
{
	// �I�������g�p����Ă���ꍇ
	if (!m_pListChoice.empty())
	{
		for (auto pChoice : GetChoice())
		{
			pChoice->SetAlpha(Choice_Mini_Alpha);
		}
		// �I������Ă���I����
		m_pListChoice[m_nSelectChoice]->SetBlink(Choice_Blink_Speed);
		m_pListChoice[m_nSelectChoice]->SetAlpha(1.0f);
	}
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CMenu::Uninit()
{

}

//============================================================================
// �X�V����
//============================================================================
void CMenu::Update()
{
	Input();
}

//============================================================================
// ���͏���
//============================================================================
void CMenu::Input()
{
	// �t�F�[�h���ł͖����ꍇ
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		// �I�������g�p����Ă���ꍇ
		if (!m_pListChoice.empty())
		{
			// ���̓f�o�C�X�̏��
			CInput* pInput = CInput::GetKey();

			// �I������2�ȏ�g�p����Ă���ꍇ
			if (m_pListChoice.size() >= 2)
			{
				// ��Ɉړ�����
				if (pInput->Trigger(DIK_W) || (pInput->Trigger(JOYPAD_UP, m_nMenuInitiative)))
				{
					// �I��SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// ���݂̔ԍ���0�ȉ��ł���ꍇ
					ChangeChoice(m_nSelectChoice <= 0 ? m_pListChoice.size() - 1 : m_nSelectChoice - 1);
				}
				// ���Ɉړ�����
				else if (pInput->Trigger(DIK_S) || (pInput->Trigger(JOYPAD_DOWN, m_nMenuInitiative)))
				{
					// �I��SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// ���݂̔ԍ����ő吔�ȏ�̏ꍇ
					ChangeChoice(m_nSelectChoice >= ((int)m_pListChoice.size() - 1) ? 0 : m_nSelectChoice + 1);
				}
			}

			if (pInput->Trigger(DIK_RETURN) || (pInput->Trigger(JOYPAD_B, m_nMenuInitiative) || pInput->Press(JOYPAD_A, m_nMenuInitiative)))
			{
				// ����SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

				// �I�������肷��
				m_bDecition = true;
			}
		}
	}
}
//============================================================================
// �I������ύX�������̏���
//============================================================================
void CMenu::ChangeChoice(const int nextChoice)
{
	// ���݂̑I�����̏���
	m_pListChoice[m_nSelectChoice]->BlinkFinish();
	m_pListChoice[m_nSelectChoice]->SetAlpha(Choice_Mini_Alpha);

	// �I�����̕ύX
	m_nSelectChoice = nextChoice;

	// �ύX��̑I�����̏���
	m_pListChoice[m_nSelectChoice]->SetBlink(m_fBlinkSpeed);
	m_pListChoice[m_nSelectChoice]->SetAlpha(1.0f);
}