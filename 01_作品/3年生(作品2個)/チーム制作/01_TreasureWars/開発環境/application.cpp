//==============================================================================================
//
// �A�v���P�[�V����
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"application.h"
#include"object.h"
#include"camera.h"
#include"title.h"
#include"entry.h"
#include"game.h"
#include"result.h"
#include"texture.h"
#include"sound.h"
#include"light.h"
#include"camera.h"
#include "mode.h"
#include "fade.h"
#include"objectX.h"
#include "particle_manager.h"
#include "stage_manager.h"
#include "input.h"
#include "charmanager.h"
#include "menu.h"
#include "menugame.h"
#include "stage_select.h"
#include <time.h>

#ifdef _DEBUG
#include "debugProc.h"
#endif

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
CRenderer *CApplication::m_pRenderer = nullptr;
CApplication::MODE CApplication::m_mode = MODE_NONE;
CMode *CApplication::m_pGameMode = nullptr;
CTexture *CApplication::m_pTexture = nullptr;
CSound *CApplication::m_pSound = nullptr;
CLight *CApplication::m_pLight = nullptr;
CCamera *CApplication::m_pCamera = nullptr;
CFade *CApplication::m_pFade = nullptr;
CParticleManager *CApplication::m_pParticleManager = nullptr;
CStageManager* CApplication::m_pStageManager = nullptr;
CCharManager *CApplication::m_pCharManager = nullptr;
CMenu *CApplication::m_pMenu = nullptr;

bool CApplication::m_bGameStart = false;
bool CApplication::m_bPauce = false;
//int CApplication::m_nStage_Number = 0;

#ifdef _DEBUG
CDebugProc *CApplication::m_pDebugProc = nullptr;
#endif
//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CApplication::CApplication()
{
	m_pInput = nullptr;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CApplication::~CApplication()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �N���X�̐���
	m_pRenderer = new CRenderer;				// �����_�����O
	m_pTexture = new CTexture;					// �e�N�X�`��
	m_pLight = new CLight;						// ���C�g
	m_pCamera = new CCamera;					// �J����

	// ����������

	//���͏���
	m_pInput = CInput::Create();
	//���͏����̏���������
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�E�B���h�E�̐ݒ�
	BOOL bWindow = FALSE;

#ifdef _DEBUG
	bWindow = TRUE;
#endif

	m_pRenderer->Init(hWnd, bWindow);				// �����_�����O
	m_pLight->Init();							// ���C�g
	m_pCamera->Init();							// �J����

	m_pTexture->LoadAll();						// �S�Ẵe�N�X�`���̓ǂݍ���

	m_pSound = CSound::Create(hWnd);			// �T�E���h�̐���

	// �p�[�e�B�N���}�l�[�W���̐���
	m_pParticleManager = new CParticleManager;
	m_pParticleManager->LoadText("particle.txt");

	// �X�e�[�W�}�l�[�W���̐���
	m_pStageManager = new CStageManager;
	m_pStageManager->LoadText("stage.txt");

	m_pCharManager = CCharManager::Create();	// �L�����}�l�[�W���[�̐���

	m_pFade = CFade::Create();					// �t�F�[�h

	//�����[�X���̓}�E�X�|�C���^�[������
	m_pInput->SetCursorErase(false);
#ifdef _DEBUG
	m_pDebugProc = CDebugProc::Create();		// �f�o�b�O�e�L�X�g

	//�f�o�b�N���̓}�E�X�|�C���^�[��\��
	m_pInput->SetCursorErase(true);
#endif

	// �Q�[�����[�h
	m_mode = MODE_TITLE;

	// ���[�h�̒��
	SetMode(m_mode);

	srand((unsigned int)time(0));

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CApplication::Uninit()
{
	//���͏����̏I������
	m_pInput->Uninit();

	// �����_�����O�̏I��
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	//�e�N�X�`���̏I��
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// ���C�g�̏I��
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	// �J�����̏I��
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

#ifdef _DEBUG
	// �f�o�b�O�e�L�X�g�̔j��
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}
#endif

	// �p�[�e�B�N���}�l�[�W���̔j��
	if (m_pParticleManager != nullptr)
	{
		m_pParticleManager->ReleaseAll();
		delete m_pParticleManager;
		m_pParticleManager = nullptr;
	}

	// �X�e�[�W�}�l�[�W���̔j��
	if (m_pStageManager != nullptr)
	{
		delete m_pStageManager;
		m_pStageManager = nullptr;
	}

	// �L�����}�l�[�W���[�̏I������
	if (m_pCharManager != nullptr)
	{
		m_pCharManager->Uninit();
		delete m_pCharManager;
		m_pCharManager = nullptr;
	}

	// �t�F�[�h�̔j��
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	// ���[�h�̔j��
	if (m_pGameMode != nullptr)
	{
		m_pGameMode->Uninit();
		delete m_pGameMode;
		m_pGameMode = nullptr;
	}

	// ���j���[�̔j��
	if (m_pMenu != nullptr)
	{
		m_pMenu->Uninit();
		delete m_pMenu;
		m_pMenu = nullptr;
	}

	// �T�E���h�̏I��
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	CObject::ReleaseAll();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CApplication::Update()
{
	//���͏����̍X�V����
	m_pInput->Update();

	// �T�E���h�̍X�V����
	m_pSound->Update();

	m_pFade->Update();

	// �g�p���̃��j���[�X�V����
	if (m_pMenu != nullptr)
	{
		m_pMenu->Update();
	}

	m_pRenderer->Update();

	// �J�����̍X�V����
	m_pCamera->Update();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CApplication::Draw()
{
	m_pRenderer->Draw();
}

//==============================================================================================
// ���[�h�̐ݒ�
//==============================================================================================
void CApplication::SetMode(MODE mode)
{
	// �Q�[�����~������
	m_bGameStart = false;

	// �Q�[�����[�h���g�p����Ă���ꍇ�̏I������
	if (m_pGameMode != nullptr)
	{
		// �Q�[�����[�h�̏I������
		m_pGameMode->Uninit();
		delete m_pGameMode;
		m_pGameMode = nullptr;

		// ���j���[�̏I������
		if (m_pMenu != nullptr)
		{
			m_pMenu->Uninit();
			delete m_pMenu;
			m_pMenu = nullptr;
		}

		// �L�����̉������
		m_pCharManager->CharaReleaseAll();

		// �S�ẴI�u�W�F�N�g�̉������
		CObject::ReleaseAll();
		
		// �I�u�W�F�N�g�̔j��
		CObjectX::UninitObject();
	}
	// �J�����̏�����
	m_pCamera->Init();

	CModel::ModelReleaseAll();

	// ���[�h��؂�ւ���
	m_mode = mode;

	// �w��̃��[�h�̐ݒ�
	switch (m_mode)
	{
	case CApplication::MODE_TITLE:
		m_pGameMode = CTitle::Create();
		break;

	case CApplication::MODE_ENTRY:
		m_pGameMode = CEntry::Create();
		break;

	case CApplication::MODE_STAGE_SELECT:
		m_pGameMode = CStage_Select::Create();
		break;

	case CApplication::MODE_GAME:
		m_pGameMode = CGame::Create();
		m_pMenu = CMenuGame::Create();	// �Q�[�����j���[�̐���
		break;

	case CApplication::MODE_RESULT:
		m_pGameMode = CResult::Create();
		break;

	default:
		break;
	}
}