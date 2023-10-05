//=============================================================================
//
// menugame.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
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
// �R���X�g���N�^
//=====================================
CMenuGame::CMenuGame()
{
	m_bDisplay = false;			// ��ʕ\���̔���
	m_bCurrentDisplay = false;	// �O��̉�ʕ\���̔���
}

//=====================================
// �f�X�g���N�^
//=====================================
CMenuGame::~CMenuGame()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CMenuGame::Init()
{
	// �t�F�[�h�[���̐���
	m_pFade = CObject2D::Create({ 640.0f, 360.0f, 0.0f }, { 910.0f, 510.0f, 0.0f }, 5);
	m_pFade->SetCol({0.0f, 0.0f, 0.0f, 0.25f});
	m_pFade->SetDrawFlag(false);

	// �I�����̐ݒ菈��
	SetChoice(CFontString::Create({ 420.0f, 400.0f, 0.0f }, { 40.0f, 40.0f, 0.0f }, "�Q�[�����Â���", 5));
	SetChoice(CFontString::Create({ 360.0f, 480.0f, 0.0f }, { 40.0f, 40.0f, 0.0f }, "�L�����Z���N�g�ɂ��ǂ�", 5));
	SetChoice(CFontString::Create({ 420.0f, 560.0f, 0.0f }, { 40.0f, 40.0f, 0.0f }, "�^�C�g���ɂ��ǂ�", 5));

	// �S�Ă̑I������\������
	for (auto pChoice : GetChoice())
	{
		pChoice->SetFontDraw(false);
	}

	// �|�[�Y���S�̐���
	m_pPauceLogo = CFontString::Create({ 440.0f, 200.0f, 0.0f }, { 100.0f, 100.0f, 0.0f }, "PAUSE", 5);
	for(auto pFont : m_pPauceLogo->GetFontAll()) pFont->SetDrawFlag(false);

	// ���j���[�̏���������
	CMenu::Init();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CMenuGame::Uninit()
{
	// ���j���[�̏I������
	CMenu::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CMenuGame::Update()
{
	// �|�[�Y�̏���
	Pauce();

	// ��ʕ\���̏���
	Display();

	// �|�[�Y���̂ݍX�V���鏈��
	if (CApplication::GetPauce() == true)
	{
		// ���j���[�̍X�V����
		CMenu::Update();

		// �I�����̏���
		Choice();

		// ��~���̃|�[�Y�I�u�W�F�N�g�̍X�V����
		PauceObjUpdate();
	}
}

//============================================================================
// ��~���̃|�[�Y�I�u�W�F�N�g�̍X�V����
//============================================================================
void CMenuGame::PauceObjUpdate()
{
	// �|�[�Y���S�̍X�V
	m_pPauceLogo->Update();

	// �t�F�[�h�[���̍X�V
	m_pFade->Update();

	// �I�����̍X�V
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
// �|�[�Y�̏���
//============================================================================
void CMenuGame::Pauce()
{
	// �t�F�[�h���ł͖��� && �Q�[�����Ԃ��I�����Ă��Ȃ��ꍇ
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE && CGame::GetTime()->GetTime() != 0)
	{
		// ���̓f�o�C�X�̏��
		CInput* pInput = CInput::GetKey();

		// �{�^���������ꂽ���ǂ���
		bool bPressP = false;

		// �|�[�Y����
		const bool bPauce = CApplication::GetPauce();

		// �|�[�Y���ł͂Ȃ��ꍇ
		if (bPauce == false)
		{
			// �R���g���[���[4���̏���
			for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
			{
				// �|�[�Y�̐ݒ菈��
				if ((pInput->Trigger(DIK_P) && nCnt == 0) || pInput->Trigger(JOYPAD_START, nCnt))
				{
					bPressP = true;

					// ���j���[�̎哱����ݒ�
					SetMenuInitiative(nCnt);

					// �Q�[����~(�J_��~SE)
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_STOP);

					break;
				}
			}
		}
		// �|�[�Y���̏ꍇ
		else
		{
			// �|�[�Y�̐ݒ菈��
			if ((pInput->Trigger(DIK_P)) || pInput->Trigger(JOYPAD_START, GetMenuInitiative()))
			{
				bPressP = true;

				// �Q�[���J�n(�J_�J�nSE)
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_START);
			}
		}

		// �|�[�Y�������ꂽ�ꍇ
		if (bPressP == true)
		{
			// ���݂̃|�[�Y����𔽓]������
			CApplication::SetPauce(!bPauce);

			// �|�[�Y���Ŗ����ꍇ�͕\�����Ȃ�
			m_bDisplay = !m_bDisplay;

			// �I�����̏�������������
			ChangeChoice(0);
		}
	}
}

//============================================================================
// ��ʕ\���̏���
//============================================================================
void CMenuGame::Display()
{
	// �\���̍X�V���������ꍇ
	if (m_bDisplay != m_bCurrentDisplay)
	{
		// ���݂̕\����Ԃ��X�V����
		m_bCurrentDisplay = m_bDisplay;

		// �|�[�Y���S���g�p���̏ꍇ
		if (m_pPauceLogo != nullptr)
		{
			// �e���j���[���̃I�u�W�F�N�g�̕\����Ԃ��X�V����
			for (auto pFont : m_pPauceLogo->GetFontAll()) pFont->SetDrawFlag(m_bDisplay);

			// �t�F�[�h�[���̕\����ݒ肷��
			m_pFade->SetDrawFlag(m_bDisplay);

			// �I�����̕\����ݒ肷��
			for (auto pChoice : GetChoice())
			{
				pChoice->SetFontDraw(m_bDisplay);
			}
		}
	}
}

//============================================================================
// �I�����̏���
//============================================================================
void CMenuGame::Choice()
{
	// ���肳�ꂽ�ꍇ
	if (GetDecition() == true && CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		// ���ݑI������Ă���I�����̏�����ǂݍ���
		switch ((Choice_Type)GetSelectChoice() + 1)
		{
		// �Q�[���𑱂���
		case Choice_Continue:
			// �J_�J�nSE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_START);

			// ���݂̃|�[�Y��Ԃ���������
			CApplication::SetPauce(false);

			// �|�[�Y���Ŗ����ꍇ�͕\�����Ȃ�
			m_bDisplay = !m_bDisplay;

			// �����j������
			SetDecition(false);
			break;

		// �G���g���[��ʂ�
		case Choice_Entry:
			// �J_��~SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_STOP);

			CFade::SetFade(CApplication::MODE_ENTRY, 0.05f);
			break;

		// �^�C�g����ʂ�
		case Choice_Title:
			// �J_��~SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_STOP);

			CFade::SetFade(CApplication::MODE_TITLE, 0.05f);
			break;
		default:
			break;
		}
	}
}

//============================================================================
// ��������
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
