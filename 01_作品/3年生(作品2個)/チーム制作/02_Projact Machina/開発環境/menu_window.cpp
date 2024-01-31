//=============================================================================
//
// menu_window.h (���j���[�E�B���h�E.h)
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "menu_window.h"
#include "object2D.h"
#include "menu.h"
#include "title.h"
#include "title_menu.h"
#include "confirmation_window.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMenuWindow::CMenuWindow()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMenuWindow::~CMenuWindow()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMenuWindow::Init()
{
	//==================================================
	// �����o�ϐ��̏�����
	//==================================================  
	m_MenuWindowSize = D3DXVECTOR2(0.0f, 0.0f);
	m_bMenuWindowScaleFlag = false;
	m_pMenuWindow = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 450.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),CObject::PRIORITY_FRONT);
	m_pMenuWindow->SetCol(D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMenuWindow::Uninit()
{
	//�����o�ϐ��̏�����
	if (m_pMenuWindow != nullptr)
	{
		m_pMenuWindow->Uninit();
		m_pMenuWindow = nullptr;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CMenuWindow::Update()
{
	if (CTitle::GetUninitWindow() == false && m_bMenuWindowScaleFlag == false)
	{ 
		// ���j���[�̃T�C�Y�g�又��
		MenuWindowScale();
	}
}

//=============================================================================
// ���j���[�̃T�C�Y�g�又��
//=============================================================================
void CMenuWindow::MenuWindowScale()
{
	if (m_pMenuWindow != nullptr)
	{// null�`�F�b�N
		// �T�C�Y�̊g��
		m_MenuWindowSize.x += SizeXScaleSpeed;
		m_MenuWindowSize.y += SizeYScaleSpeed;

		if (m_MenuWindowSize.x >= SizeXScaleMax)
		{// X�T�C�Y�̍ő�
			m_MenuWindowSize.x = SizeXScaleMax;
		}
		if (m_MenuWindowSize.y >= SizeYScaleMax)
		{// Y�T�C�Y�̍ő�
			m_MenuWindowSize.y = SizeYScaleMax;
		}

		// �E�B���h�E���ő�l�܂ōs������
		if (m_MenuWindowSize.x >= SizeXScaleMax && m_MenuWindowSize.y >= SizeYScaleMax)
		{
			CApplication::GetMenu()->SetDisplay(true);		// ���j���[��ʂ̕\��
			CTitle::SetWindow(true);						// �E�B���h�E���g�p���Ă����Ԃɂ���
			m_bMenuWindowScaleFlag = true;					// �g�傳�ꂽ
		}

		// �T�C�Y�̐ݒ�
		m_pMenuWindow->SetSize(D3DXVECTOR2(m_MenuWindowSize.x, m_MenuWindowSize.y));
	}
}

//=============================================================================
// ���j���[�̃T�C�Y�k����������
//=============================================================================
bool CMenuWindow::MenuWindowScaleReduce()
{
	if (m_pMenuWindow != nullptr)
	{// null�`�F�b�N
		// �T�C�Y�̏k��
		m_MenuWindowSize.x -= SizeXScaleSpeed;
		m_MenuWindowSize.y -= SizeYScaleSpeed;

		if (m_MenuWindowSize.x <= 0.0f)
		{// X�T�C�Y�̍ŏ�
			m_MenuWindowSize.x = 0.0f;
		}
		if (m_MenuWindowSize.y <= 0.0f)
		{// Y�T�C�Y�̍ŏ�
			m_MenuWindowSize.y = 0.0f;
		}

		// �E�B���h�E���ŏ��l�܂ōs������
		if (m_MenuWindowSize.x <= 0.0f && m_MenuWindowSize.y <= 0.0f)
		{
			// �E�B���h�E�����̐ݒ菈��
			CTitleMenu::GetComfirmationWindow()->SetSapawnWindow(false);

			if (CTitleMenu::GetComfirmationWindow()->GetSelectChoice() == true)
			{// �Q�[������߂���

				// �E�B���h�E�̍폜
				DestroyWindow(CApplication::GetWindow());
			}

			CTitle::UninitMenuWindow();					// ���j���[�E�B���h�E��j�����鏈��		
			CTitleMenu::UninitComfirmationWindow();		// �m�F�E�B���h�E��j�����鏈��
			return true;
		}

		// �T�C�Y�̐ݒ�
		m_pMenuWindow->SetSize(D3DXVECTOR2(m_MenuWindowSize.x, m_MenuWindowSize.y));
	}
	return false;
}

//=============================================================================
// �^�C���̐���
//=============================================================================
CMenuWindow* CMenuWindow::Create()
{
	//�N���X�̐���
	CMenuWindow* pMenuWindow = new CMenuWindow;

	//null�`�F�b�N
	if (pMenuWindow != nullptr)
	{
		//����������
		pMenuWindow->Init();
	}
	else
	{
		assert(false);
	}

	return pMenuWindow;
}