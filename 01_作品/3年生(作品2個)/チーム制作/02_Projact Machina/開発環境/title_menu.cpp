//=============================================================================
//
// �^�C�g�����j���[
// title_menu.cpp
// Author : Tanimoto Kosuke
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// �C���N���[�h
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
// �ÓI�����o�ϐ��錾
//=============================================================================
CConfirmation_Window* CTitleMenu::m_pConfirmationWindow = nullptr;

//============================================================================
// �R���X�g���N�^
//============================================================================
CTitleMenu::CTitleMenu()
{
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CTitleMenu::~CTitleMenu()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CTitleMenu::Init()
{
	// �I�����̐ݒ菈��
	SetChoice(CFontString::Create({ 400.0f, 400.0f, 0.0f}, { 40.0f, 40.0f }, "�Q�[�����͂��߂�"));
	SetChoice(CFontString::Create({ 800.0f / 2, 500.0f ,0.0f }, { 40.0f, 40.0f }, "�Q�[������߂�"));

	// �S�Ă̑I������\������
	for (auto pChoice : GetChoiceAll())
	{
		pChoice->SetFontDraw(false);
	}

	// ���j���[�̏���������
	CMenu::Init();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CTitleMenu::Uninit()
{
	// ���j���[�̏I������
	CMenu::Uninit();

	// �m�F�E�B���h�E�̏I������
	if (m_pConfirmationWindow != nullptr)
	{
		m_pConfirmationWindow->Uninit();
		delete m_pConfirmationWindow;
		m_pConfirmationWindow = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CTitleMenu::Update()
{
	// ���j���[�̍X�V����
	CMenu::Update();

	// �I�����̏���
	Choice();

	// �X�V����
	if (m_pConfirmationWindow != nullptr)
	{
		// �m�F�E�B���h�E�̍X�V����
		m_pConfirmationWindow->Update();
	}
}

//============================================================================
// �I�����̏���
//============================================================================
void CTitleMenu::Choice()
{
	// ���j���[�̑I�����̏���
	CMenu::Choice();

	// ���肳�ꂽ�ꍇ
	if (GetDecition() == true)
	{
		// ���ݑI������Ă���I�����̏�����ǂݍ���
		switch ((Choice_Type)GetSelectChoice())
		{
			// �Q�[���X�^�[�g
		case Choice_GameStart:
		{
			// ����SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
			// �Q�[���ɔ��
			CFade::SetFade(CApplication::MODE_CHAR_SELECT, 0.05f);
			CTitle::SetUninitWindow(false);
			//CApplication::GetClient()->Init("10.70.41.123", 15678);



			break;
		}
			// �E�B���h�E�����
		case Choice_Window_Exit:
			// ����SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

			if (m_pConfirmationWindow == nullptr)
			{// null�`�F�b�N

				// �m�F�E�B���h�E�̐���
				m_pConfirmationWindow = CConfirmation_Window::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f),500.0f, 320.0f, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));
				return;
			}
			break;
		default:
			break;
		}
	}
	// �����j������
	SetDecition(false);
}

//============================================================================
// ��������
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
// �m�F�E�B���h�E��j�����鏈��
//============================================================================
void CTitleMenu::UninitComfirmationWindow()
{
	if (m_pConfirmationWindow != nullptr)
	{
		delete m_pConfirmationWindow;
		m_pConfirmationWindow = nullptr;
	}
}
