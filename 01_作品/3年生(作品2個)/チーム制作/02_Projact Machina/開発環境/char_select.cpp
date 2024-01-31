//==============================================================================================
//
// �L�����N�^�[�Z���N�g�@�@�@char_select.cpp
// tutida ryousei
//
//==============================================================================================
#include "char_select.h"
#include "input.h"
#include "application.h"
#include "fade.h"
#include "fontString.h"
#include "chardecision_window.h"
#include "confirmation_window.h"
#include "camera.h"
#include "halfsphere.h"
#include "playerdata.h"
#include "model.h"
#include "objectX.h"
#include "sound.h"
#include "texture.h"

//==============================================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================================
CCharDecision_Window* CChar_Select::m_pCharDecisionWindow = nullptr;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CChar_Select::CChar_Select()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CChar_Select::~CChar_Select()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CChar_Select::Init()
{
	//======================================
	// �����o�ϐ��̏���������
	//======================================
	m_nIndex = 0;
	m_nPlayerIndex = 0;

	m_pFontTexture = CObject2D::Create({ SCREEN_WIDTH / 2,100.0f,0.0f }, { 300.0f,100.0f });
	m_pFontTexture->SetTexture(CTexture::TEXTURE_AIRCRAAFTSELECTION);

	CCamera* pCamera = CApplication::GetCamera();	// �J�����̃|�C���^
	m_pPlayerData = new CPlayerData;				// �v���C���[�f�[�^�̃C���X�^���X

	pCamera->SetPosV({ 0.0f, 100.0f, -100.0f });
	pCamera->SetPosR({ 0.0f, 0.0f, 100.0f });

	// �v���C���[�f�[�^�̏���������
	m_pPlayerData->Init();

	// �v���C���[�f�[�^�̐�������
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_pObjectX[nCnt] = CObjectX::Create(D3DXVECTOR3(-80.0f + 55.0f * nCnt, 600.0f, 0.0f), D3DXVECTOR3(0.0f, -0.3f + 0.25f * nCnt, 0.0f), nullptr, "Data/model/SelectMode/view_Body_00.x");
	}

	// �q�Ƀ��f���̐���
	m_pWareHouse = CObjectX::Create(D3DXVECTOR3(0.0f, 300.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0, 0.0f), nullptr, "Data/model/CharacterSelect/Warehouse.x");

	//BGM
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_CHARACTER);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CChar_Select::Uninit()
{
	// ���j���[�E�B���h�E�̏I������
	if (m_pCharDecisionWindow != nullptr)
	{
		m_pCharDecisionWindow->Uninit();
		delete m_pCharDecisionWindow;
		m_pCharDecisionWindow = nullptr;
	}

	// �v���C���[�f�[�^
	if (m_pPlayerData != nullptr)
	{
		m_pPlayerData->Uninit();
		delete m_pPlayerData;
		m_pPlayerData = nullptr;
	}
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CChar_Select::Update()
{
	CInput* pInput = CInput::GetKey();

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if ((pInput->Trigger(DIK_RETURN) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
			|| pInput->Trigger(JOYPAD_START, nCnt))
			&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			if (m_pCharDecisionWindow == nullptr)
			{
				// ����SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);
				m_pCharDecisionWindow = CCharDecision_Window::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f), 900.0f, 500.0f, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			}
		}
	}

	if (m_pCharDecisionWindow != nullptr)
	{
		m_pCharDecisionWindow->Update();
	}

	// �v���C���[�f�[�^�̍X�V����
	if (m_pPlayerData != nullptr)
	{
		m_pPlayerData->Update();
	}

	// �L�����؂�ւ�����
	CharSwitching(0);
}

//==============================================================================================
//	�L�����؂�ւ�����
//==============================================================================================
void CChar_Select::CharSwitching(int PlayerIndex)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (nCnt == PlayerIndex)
		{// �w�肵���v���C���[�Ƃ��̃v���C��[�Ɠ����ԍ���������

			// ���݂̔ԍ��̎擾
			m_nIndex = m_pPlayerData->GetPlayerIndex();

			// null�`�F�b�N
			if (m_pCharDecisionWindow != nullptr)
			{
				// ���I��ł���ԍ��̎擾
				m_nIndex = m_pCharDecisionWindow->GetSelectIndex();
			}

			// �ԍ��̕ύX
			if (m_nIndex <= -1)
			{
				m_nIndex = 3;
			}
			if (m_nIndex >= 4)
			{
				m_nIndex = 0;
			}

			// ���݂̔ԍ��ƑO�̔ԍ�����v���Ă��Ȃ�������
			if (m_nIndex != m_nIndexKeep)
			{
				// �I�u�W�F�N�gX�̍폜
				if (m_pObjectX[nCnt] != nullptr)
				{
					m_pObjectX[nCnt]->Uninit();
					m_pObjectX[nCnt] = nullptr;
				}
			}

			// �ԍ��̕ۑ�
			m_nIndexKeep = m_nIndex;

			// ���f���̐ݒ�
			if (CChar_Select::GetCharDecisionWindow() != nullptr)
			{
				SetModel(0, "Data/model/SelectMode/view_Body_00.x");
				SetModel(1, "Data/model/SelectMode/view_Body_01.x");
				SetModel(2, "Data/model/SelectMode/view_Body_02.x");
				SetModel(3, "Data/model/SelectMode/view_Body_03.x");
			}
		}
	}
}

//==============================================================================================
// ���f���̐ݒ菈��
//==============================================================================================
void CChar_Select::SetModel(int index, const char * Xfilename)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pObjectX[nCnt] == nullptr)
		{
			if (m_nIndex == index)
			{
				m_pObjectX[nCnt] = CObjectX::Create(D3DXVECTOR3(-80.0f + 55.0f * nCnt, 600.0f, 0.0f), D3DXVECTOR3(0.0f, -0.3f, 0.0f), nullptr, Xfilename);
			}
		}
	}
}

//==============================================================================================
// ��������
//==============================================================================================
CChar_Select* CChar_Select::Create()
{
	CChar_Select* pChar_Select = new CChar_Select;

	if (FAILED(pChar_Select->Init()))
	{
		return nullptr;
	}

	return pChar_Select;
}
