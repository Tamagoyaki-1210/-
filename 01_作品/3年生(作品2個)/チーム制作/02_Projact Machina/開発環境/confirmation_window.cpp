//=============================================================================
//
// confirmation_window.h (�m�F�E�B���h�E.h)
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// �C���N���[�h
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
// �ÓI�����o�ϐ��錾
//=============================================================================
CCharSelect_Window* CConfirmation_Window::m_pCharSelectWindow = nullptr;
const float CConfirmation_Window::BLINK_SPEED = 0.02f;	// �I�����̓_�ő��x
const float CConfirmation_Window::MINI_ALPHA = 0.5f;	// �I�����̍Œᓧ���l
bool CConfirmation_Window::m_bConfimationSelectChoice = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CConfirmation_Window::CConfirmation_Window() : m_fConfimationBlinkSpeed(BLINK_SPEED)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CConfirmation_Window::~CConfirmation_Window()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CConfirmation_Window::Init()
{
	//==================================================
	// �����o�ϐ��̏�����
	//==================================================
	VariableInit();
	m_Size = D3DXVECTOR2(0.0f, 0.0f);
	m_pObject2D = CObject2D::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), D3DXVECTOR2(0.0f, 0.0f), CObject::PRIORITY_SCREEN);
	m_pObject2D->SetCol(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_Color.a));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CConfirmation_Window::Uninit()
{
	//�����o�ϐ��̔j��
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Uninit();
		m_pObject2D = nullptr;
	}

	//// �L�����I����ʂ̔j��
	//if (m_pCharSelectWindow != nullptr)
	//{
	//	m_pCharSelectWindow->Uninit();
	//	delete m_pCharSelectWindow;
	//	m_pCharSelectWindow = nullptr;
	//}

	// �t�H���g�̍폜
	if (m_pFont != nullptr)
	{
		m_pFont->Uninit();
		m_pFont = nullptr;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CConfirmation_Window::Update()
{
	if (m_bConfimationScale == false && m_bConfirmasionWindowUse == false)
	{ // �g�傳��Ă��Ȃ��� && �E�B���h�E���g���Ă��Ȃ��Ƃ�

		// �m�F�E�B���h�E�̊g�又��
		ConfirmatiomnMenuScale();
	}
	else if(m_bConfimationScale == true && m_bConfirmasionWindowUse == false)
	{ // �g�債���� && �E�B���h�E���g���Ă��Ȃ��Ƃ�

		// �m�F�E�B���h�E�̏k������
		ConfirmatiomnMenuScaleReduce();
	}
	// �I��������
	Choice();
}

//=============================================================================
// �m�F�E�B���h�E�̃T�C�Y�g�又��
//=============================================================================
void CConfirmation_Window::ConfirmatiomnMenuScale()
{
	if (m_pObject2D != nullptr)
	{ // null�`�F�b�N

		// �T�C�Y�̊g��
		m_Size.x += SizeXScaleSpeed;
		m_Size.y += SizeYScaleSpeed;

		if (m_Size.x >= m_MaxSize.x)
		{ // X�T�C�Y�̍ő�
			m_Size.x = m_MaxSize.x;
		}
		if (m_Size.y >= m_MaxSize.y)
		{ // Y�T�C�Y�̍ő�
			m_Size.y = m_MaxSize.y;
		}

		// �E�B���h�E���ő�l�܂ōs������
		if (m_Size.x >= m_MaxSize.x && m_Size.y >= m_MaxSize.y && m_bConfimationMaxSize == false)
		{
			// �Q�[�����[�h�̎擾
			CApplication::MODE Mode = CApplication::GetModeType();

			if (Mode == CApplication::MODE_TITLE || Mode == CApplication::MODE_GAME)
			{ // �^�C�g�����Q�[����ʂ�������
				
				// �����̐���
				if (m_pFont == nullptr)                     
				{
					m_pFont = CFontString::Create({ 420.0f, 350.0f, 0.0f }, { 30.0f, 30.0f }, "��߂܂���?");
				}

				// �I�����̐���
				SetChoice(CFontString::Create({ 400.0f, 450.0f, 0.0f }, { 25.0f, 25.0f }, "�͂�"));
				SetChoice(CFontString::Create({ 700.0f, 450.0f, 0.0f }, { 25.0f, 25.0f }, "������"));
			}

			// �I�������g�p����Ă���ꍇ
			if (!m_vpListChoice.empty())
			{
				for (auto pChoice : GetChoiceAll())
				{
					pChoice->SetAlpha(MINI_ALPHA);
				}
				// �I������Ă���I����
				m_vpListChoice[m_bConfimationSelectChoice]->SetBlink(BLINK_SPEED);
				m_vpListChoice[m_bConfimationSelectChoice]->SetAlpha(1.0f);
			}

			if (m_bConfimationBack == true)
			{ // �E�B���h�E���߂��Ă�����
				VariableInit();					// �����o�ϐ��̏���������
				ChangeChoice(0);				// �I�����̏�������������
				UninitCharSelectWindow();		// �L�����I���E�B���h�E�̔j������
			}

			SetDisplay(true);					// �I�����̕\��������g�p
			m_bConfimationMaxSize = true;		// �ő�T�C�Y�ɓ��B
			return;
		}

		// �T�C�Y�̐ݒ�
		m_pObject2D->SetSize(D3DXVECTOR2(m_Size.x, m_Size.y));
	}
}

//=============================================================================
// �m�F�E�B���h�E�̃T�C�Y�k����������
//=============================================================================
bool CConfirmation_Window::ConfirmatiomnMenuScaleReduce()
{
	if (m_pObject2D != nullptr)
	{ // null�`�F�b�N

		// ���݂̃��[�h
		CApplication::MODE Mode = CApplication::GetModeType();

		// �T�C�Y�̏k��
		m_Size.x -= SizeXScaleSpeed;
		m_Size.y -= SizeYScaleSpeed;

		if (m_Size.x <= 0.0f)
		{ // X�T�C�Y�̍ŏ�
			m_Size.x = 0.0f;
		}
		if (m_Size.y <= 0.0f)
		{ // Y�T�C�Y�̍ŏ�
			m_Size.y = 0.0f;
		}

		// �t�H���g�̍폜
		if (m_pFont != nullptr)
		{
			m_pFont->Uninit();
			m_pFont = nullptr;
		}

		// �E�B���h�E���ŏ��l�܂ōs������
		if (m_Size.x <= 0.0f && m_Size.y <= 0.0f)
		{
			// �Q�[���I��
			if (m_bConfimationSelectChoice == true && Mode == CApplication::MODE_GAME)
			{ // ��߂�I������I�� && ���[�h���Q�[����ʂ�������

				m_bConfimationUninitFlag = true;				// �Q�[�����E�B���h�E�̔j��
				m_bConfimationSpawnWindow = true;				// ���j���[�E�B���h�E�̔j��
				DestroyWindow(CApplication::GetWindow());		// �E�B���h�E�̔j��
			}
			else if (m_bConfimationSelectChoice == false && Mode == CApplication::MODE_GAME)
			{ // ��߂Ȃ��I������I�� && ���[�h���Q�[����ʂ�������

				m_bConfimationUninitFlag = true;				// �Q�[�����E�B���h�E�̔j��
				m_bConfimationSpawnWindow = true;				// ���j���[�E�B���h�E�̔j��
				m_bConfimationScale = false;					// �g��k���t���O��������			
				Uninit();										// ���j���[�E�B���h�E�̍폜			
			}
			if (Mode == CApplication::MODE_TITLE)
			{ // �^�C�g����ʂ�������

				m_bConfimationScale = false;					// �g��k���t���O��������	
				m_bConfimationSpawnWindow = true;				// ���j���[�E�B���h�E�̔j��
				Uninit();										// ���j���[�E�B���h�E�̍폜
			}
			if (Mode == CApplication::MODE_CHAR_SELECT && m_bConfirmasionWindowUse == false)
			{ // �L�����I����ʂ������� && ���̃E�B���h�E���g���Ă��Ȃ�������

				m_bConfirmasionWindowUse = true;	// �g�p���Ă���l�Ԃɂ���
			}
			return true;
		}

		// �T�C�Y�̐ݒ�
		m_pObject2D->SetSize(D3DXVECTOR2(m_Size.x, m_Size.y));
	}
	return false;
}

//============================================================================
// �I�����̏���
//============================================================================
void CConfirmation_Window::Choice()
{
	// ���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();
	
	// �t�F�[�h���ł͖����ꍇ && �\�����̏ꍇ
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE && m_bConfimationDisplay == true)
	{
		// �I�������g�p����Ă���ꍇ
		if (!m_vpListChoice.empty())
		{
			// �I������2�ȏ�g�p����Ă���ꍇ
			if (m_vpListChoice.size() >= 2)
			{
				// ��Ɉړ�����
				if (pInput->Trigger(DIK_A) || (pInput->Trigger(JOYPAD_UP)))
				{
					// �I��SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// ���݂̔ԍ���0�ȉ��ł���ꍇ
					ChangeChoice(false);
				}
				// ���Ɉړ�����
				else if (pInput->Trigger(DIK_D) || (pInput->Trigger(JOYPAD_DOWN)))
				{
					// �I��SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

					// ���݂̔ԍ����ő吔�ȏ�̏ꍇ
					ChangeChoice(true);
				}
			}

			if (pInput->Trigger(DIK_RETURN) || (pInput->Trigger(JOYPAD_B) || pInput->Trigger(JOYPAD_A)))
			{
				// ����SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

				// �I�������肷��
				m_bConfimationDecition = true;
			}
			// ���肳�ꂽ�ꍇ
			if (m_bConfimationDecition == true)
			{
				// ���̑I�����̏ꍇ
				if (m_bConfimationSelectChoice == false)
				{
					CreateCharSelect();					// �L�����I����ʂ̐���
					m_bConfimationScale = true;			// �g��k���t���O���g���Ă���
					SetDisplay(false);					// �I�����̕\��������g�p���Ă��Ȃ���Ԃɂ���
					m_bConfimationSelectChoice = true;	// ���̑I������I��
				}
				// �E�̑I�����̏ꍇ
				else
				{
					CreateCharSelect();					// �L�����I����ʂ̐���
					m_bConfimationScale = true;         // �g��k���t���O���g���Ă���
					SetDisplay(false);					// �I�����̕\��������g�p���Ă��Ȃ���Ԃɂ���
					m_bConfimationSelectChoice = false; // ���̑I������I��
				}
			}
		}
	}

	//if (m_pCharSelectWindow != nullptr)
	//{ //	�L�����I���E�B���h�E���g���Ă�����

	//	 �L�����I���E�B���h�E�̍X�V����
	//	m_pCharSelectWindow->Update();
	//}

	if (m_bConfimationSpawnWindow == true && CTitle::GetMenuWindow() != nullptr)
	{ // ���j���[�E�B���h�E���g���Ă��鎞 &&  ���j���[�E�B���h�E���g���Ă��鎞

		CApplication::GetMenu()->SetDisplay(false);				// ����������
		CTitle::GetMenuWindow()->MenuWindowScaleReduce();		// �Q�[���J���̃E�B���h�E�̏k��
	}
}

//============================================================================
// �I������ύX�������̏���
//============================================================================
void CConfirmation_Window::ChangeChoice(const bool nextChoice)
{
	// ���݂̑I�����̏���
	m_vpListChoice[m_bConfimationSelectChoice]->BlinkFinish();
	m_vpListChoice[m_bConfimationSelectChoice]->SetAlpha(MINI_ALPHA);

	// �I�����̕ύX
	m_bConfimationSelectChoice = (int)nextChoice;

	// �ύX��̑I�����̏���
	m_vpListChoice[m_bConfimationSelectChoice]->SetBlink(m_fConfimationBlinkSpeed);
	m_vpListChoice[m_bConfimationSelectChoice]->SetAlpha(1.0f);
}

//============================================================================
// �����o�ϐ��̏�����
//============================================================================
void CConfirmation_Window::VariableInit()
{
	// �����o�ϐ��̏�����
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
// �L�����I���E�B���h�E�̔j������
//============================================================================
void CConfirmation_Window::UninitCharSelectWindow()
{	
	//// �L�����I����ʂ̔j��
	//if (m_pCharSelectWindow != nullptr)
	//{
	//	m_pCharSelectWindow->Uninit();
	//	delete m_pCharSelectWindow;
	//	m_pCharSelectWindow = nullptr;
	//}
}

//============================================================================
// �I�����̕\�������ݒ肷��
//============================================================================
void CConfirmation_Window::SetDisplay(const bool decition)
{
	m_bConfimationDisplay = decition;
	for (auto pChoice : GetChoiceAll()) { pChoice->SetFontDraw(decition); }

	// �\���������ꍇ
	if (decition == false)
	{
		// �I�����̏�������������
		ChangeChoice(0);
	}
}

//=============================================================================
// �L�����I����ʂ̐���
//=============================================================================
void CConfirmation_Window::CreateCharSelect()
{
	//// ���݂̃��[�h
	//CApplication::MODE Mode = CApplication::GetModeType();		

	//if (Mode == CApplication::MODE_CHAR_SELECT)
	//{ // �L�����I����ʂ�������

	//	// null�`�F�b�N
	//	if (m_pCharSelectWindow == nullptr)
	//	{
	//		// �L�����I���E�B���h�E�̐���
	//		m_pCharSelectWindow = CCharSelect_Window::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), 900.0f, 500.0f, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	//	}
	//}
}

//=============================================================================
// ���j���[�̐���
//=============================================================================
CConfirmation_Window* CConfirmation_Window::Create(D3DXVECTOR3 pos, float xsize, float ysize, D3DXCOLOR col)
{
	//�N���X�̐���
	CConfirmation_Window* pMenuWindow = new CConfirmation_Window;

	//null�`�F�b�N
	if (pMenuWindow != nullptr)
	{
		// �ݒ菈��
		pMenuWindow->m_pos = pos;
		pMenuWindow->m_Color = col;
		pMenuWindow->m_MaxSize.x = xsize;
		pMenuWindow->m_MaxSize.y = ysize;
		// ����������
		pMenuWindow->Init();
	}
	else
	{
		assert(false);
	}

	return pMenuWindow;
}