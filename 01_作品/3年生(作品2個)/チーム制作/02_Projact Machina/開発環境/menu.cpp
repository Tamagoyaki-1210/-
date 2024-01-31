//=============================================================================
//
// menu.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "menu.h"
#include "Application.h"
#include "input.h"
#include "fade.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
const float CMenu::CHOICE_BLINK_SPEED = 0.02f;	// �I�����̓_�ő��x
const float CMenu::CHOICE_MINI_ALPHA = 0.5f;	// �I�����̍Œᓧ���l

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMenu::CMenu() : m_nSelectChoice(0), m_fBlinkSpeed(CHOICE_BLINK_SPEED), m_bDecition(false)
{
	// �����o�ϐ��̃N���A
	m_nMenuInitiative = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMenu::~CMenu()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CMenu::Init()
{
	// �I�������g�p����Ă���ꍇ
	if (!m_vpListChoice.empty())
	{
		for (auto pChoice : GetChoiceAll())
		{
			pChoice->SetAlpha(CHOICE_MINI_ALPHA);
		}
		// �I������Ă���I����
		m_vpListChoice[m_nSelectChoice]->SetBlink(CHOICE_BLINK_SPEED);
		m_vpListChoice[m_nSelectChoice]->SetAlpha(1.0f);
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

}

//============================================================================
// �I�����̏���
//============================================================================
void CMenu::Choice()
{
	// �t�F�[�h���ł͖����ꍇ && �\�����̏ꍇ
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE && m_bDisplay == true)
	{
		// �I�������g�p����Ă���ꍇ
		if (!m_vpListChoice.empty())
		{
			// ���̓f�o�C�X�̏��
			CInput* pInput = CInput::GetKey();

			// �I������2�ȏ�g�p����Ă���ꍇ
			if (m_vpListChoice.size() >= 2)
			{
				// ��Ɉړ�����
				if (pInput->Trigger(DIK_W) || (pInput->Trigger(JOYPAD_UP, m_nMenuInitiative)))
				{
					// �I��SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// ���݂̔ԍ���0�ȉ��ł���ꍇ
					ChangeChoice(m_nSelectChoice <= 0 ? m_vpListChoice.size() - 1 : m_nSelectChoice - 1);
				}
				// ���Ɉړ�����
				else if (pInput->Trigger(DIK_S) || (pInput->Trigger(JOYPAD_DOWN, m_nMenuInitiative)))
				{
					// �I��SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// ���݂̔ԍ����ő吔�ȏ�̏ꍇ
					ChangeChoice(m_nSelectChoice >= ((int)m_vpListChoice.size() - 1) ? 0 : m_nSelectChoice + 1);
				}
			}

			if (pInput->Trigger(DIK_RETURN) || (pInput->Trigger(JOYPAD_B, m_nMenuInitiative) || pInput->Trigger(JOYPAD_A, m_nMenuInitiative)))
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
	m_vpListChoice[m_nSelectChoice]->BlinkFinish();
	m_vpListChoice[m_nSelectChoice]->SetAlpha(CHOICE_MINI_ALPHA);

	// �I�����̕ύX
	m_nSelectChoice = nextChoice;

	// �ύX��̑I�����̏���
	m_vpListChoice[m_nSelectChoice]->SetBlink(m_fBlinkSpeed);
	m_vpListChoice[m_nSelectChoice]->SetAlpha(1.0f);
}

//============================================================================
// �I�����̕\�������ݒ肷��
//============================================================================
void CMenu::SetDisplay(const bool display)
{
	m_bDisplay = display;
	for (auto pChoice : GetChoiceAll()) { pChoice->SetFontDraw(display); }

	// �\���������ꍇ
	if (display == false)
	{
		// �I�����̏�������������
		ChangeChoice(0);
	}
}