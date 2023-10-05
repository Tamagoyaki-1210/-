//=============================================================================
//
// �^�C�g��
// Author : tanimoto kosuke
//
//=============================================================================
#include "title.h"
#include "mode.h"
#include "object.h"
#include "menu.h"
#include "Application.h"
#include "bg.h"
#include "message.h"

Title_Mode CTitle::m_mode = (Title_Mode)0;
Title_Mode CTitle::m_modeNext = (Title_Mode)0;
CBG *CTitle::m_pBG = nullptr;

//****************************************************************************
//�}�N����`
//****************************************************************************
#define TITLE_BLINK		(400)
#define TITLE_CHANGE	(TITLE_BLINK + 600)	

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CTitle::CTitle()
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CTitle::~CTitle()
{

}

//=====================================
//����������
//=====================================
void CTitle::Init()
{
	m_nBlinkCounter = 0;
	m_bChangeColor = false;
	m_bBlink = false;

	m_nLoop = 0;
	m_modeNext = (Title_Mode)(m_nLoop % Title_MAX);

	CMessage::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5, 0.0f), CMessage::TYPE_TITLE);

	//�w�i
	m_pBG = CBG::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), CBG::TYPE_BG01);
	ChangeStage();

	CMenu::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CMenu::TYPE_NONE, CMenu::MODE_TITLE);
	m_nModeCounter = 0;
}

//=====================================
//�I������
//=====================================
void CTitle::Uninit()
{
	CObject::ReleaseAll();
}

//=====================================
//�X�V����
//=====================================
void CTitle::Update()
{
	ArrayStage();

	if (m_nModeCounter > 3000)
	{
		CMode *pMode = CApplication::GetMode();

		pMode->ChangeMode(CMode::MODE_RESULT);
	}
	else
	{
		m_nModeCounter++;
	}
}

//=====================================
//�`�揈��
//=====================================
void CTitle::Draw()
{

}

//=====================================
//�X�e�[�W�؂�ւ�������
//=====================================
void CTitle::ArrayStage()
{
	if (m_nGameTimer % TITLE_CHANGE >= TITLE_CHANGE - TITLE_BLINK)
	{
		BlinkStage();
	}
	if (m_nGameTimer % TITLE_CHANGE == TITLE_CHANGE - 1)
	{
		m_nLoop++;
		m_modeNext = (Title_Mode)(m_nLoop % Title_MAX);
		ChangeStage();
	}
	m_nGameTimer++;
}

//=====================================
//�X�e�[�W�؂�ւ�������
//=====================================
void CTitle::ChangeStage()
{
	switch (m_mode)
	{
	case Title_1:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f));
		break;
	case Title_2:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f));
		break;
	case Title_3:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f));
		break;
	case Title_4:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
		break;
	default:
		break;
	}

	m_mode = m_modeNext;	//���̃��[�h�Ɉړ�

	switch (m_mode)
	{
	case Title_1:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f));
		break;
	case Title_2:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f));
		break;
	case Title_3:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f));
		break;
	case Title_4:
		//(��)
		m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f));
		break;
	default:
		break;
	}
	m_bChangeColor = false;
}

//=====================================
//�X�e�[�W�_�ŏ���
//=====================================
void CTitle::BlinkStage()
{
	float fAlpha = (m_nBlinkCounter * 0.001f);

	if (fAlpha > 0.2f)
	{
		m_bBlink = true;
		fAlpha = 0.2f;
		if (m_bChangeColor == false)
		{
			m_bChangeColor = true;
		}
		else
		{
			m_bChangeColor = false;
		}
	}
	else if (fAlpha < 0.0f)
	{
		m_bBlink = false;
		fAlpha = 0.0f;
	}

	switch (m_mode)
	{
	case Title_1:
		//(��)
		if (m_bChangeColor == false)
		{
			m_pBG->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 1.0f, 0.2f, 0.4f - fAlpha));
		}
		else
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f - fAlpha));
		}
		break;
	case Title_2:
		//(��)
		if (m_bChangeColor == false)
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.6f - fAlpha));
		}
		else
		{
			m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f - fAlpha));
		}
		break;
	case Title_3:
		//(��)
		if (m_bChangeColor == false)
		{
			m_pBG->SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f - fAlpha), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f - fAlpha));
		}
		else
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f - fAlpha));
		}
		break;
	case Title_4:
		//(��)
		if (m_bChangeColor == false)
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f - fAlpha), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f - fAlpha));
		}
		else
		{
			m_pBG->SetColor(D3DXCOLOR(1.0f, 0.4f, 0.4f, 1.0f - fAlpha), D3DXCOLOR(0.4f, 1.0f, 0.4f, 0.8f - fAlpha), D3DXCOLOR(0.4f, 0.4f, 1.0f, 0.8f - fAlpha), D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f - fAlpha));
		}
		break;
	default:
		break;
	}

	if (m_bBlink == true)
	{
		m_nBlinkCounter--;
	}
	else
	{
		m_nBlinkCounter++;
	}
}