//==============================================================================================
//
// �A�v���P�[�V����
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include "application.h"
#include "object.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "light.h"
#include "mode.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "title_menu.h"
#include "particle_manager.h"
#include "char_select.h"
#include "stage_select.h"
#include "tutorial.h"
#include "confirmation_window.h"
#include "parts_file.h"
#include "time.h"
#include "connect.h"
#include <time.h>
#include "character.h"
#include "player_manager.h"

#ifdef _DEBUG
#include "debugProc.h"
#endif

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
HWND CApplication::Window = nullptr;
CRenderer* CApplication::m_pRenderer = nullptr;
CApplication::MODE CApplication::m_modeType = MODE_NONE;
CMode* CApplication::m_pGameMode = nullptr;
CTexture* CApplication::m_pTexture = nullptr;
CModel* CApplication::m_pModel = nullptr;
CMotion* CApplication::m_pMotion = nullptr;
CSound* CApplication::m_pSound = nullptr;
CLight* CApplication::m_pLight = nullptr;
CCamera* CApplication::m_pCamera = nullptr;
CFade* CApplication::m_pFade = nullptr;
CMenu* CApplication::m_pMenu = nullptr;
CParts_File* CApplication::m_pPartsFile = nullptr;
CParticleManager* CApplication::m_pParticleManager = nullptr;
ConnectManager*CApplication::m_pClient = nullptr;
bool CApplication::m_bGameStart = false;
bool CApplication::m_bPauce = false;
bool CApplication::m_nSkill = false;
int CApplication::m_nPlayerJobIndex[3] = {};
int CApplication::m_nPlayerWeaponIndex = 0;
int CApplication::m_nDestroyTime = 0;
int CApplication::m_nDamage = 0;
int CApplication::m_nDefeats = 0;
int CApplication::m_nTotalDamage = 0;

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
	Window = hWnd;

	// �N���X�̐���
	m_pRenderer = new CRenderer;					// �����_�����O�̐���
	m_pTexture = new CTexture;						// �e�N�X�`���̐���
	m_pModel = new CModel;							// ���f���̐���
	m_pMotion = new CMotion;						// ���[�V�����̐���
	m_pLight = new CLight;							// ���C�g�̐���
	m_pCamera = new CCamera;						// �J�����̐���
	m_pPartsFile = new CParts_File;					// �p�[�c�t�@�C���̐���
	//m_pClient = new CClient;						// �T�[�o�[�̃R�l�N�^�[
	//���͏���
	m_pInput = CInput::Create();
	//���͏����̏���������
	if (FAILED(m_pInput->Init(hInstance, Window)))
	{
		return E_FAIL;
	}

	//�E�B���h�E�̐ݒ�
	BOOL bWindow = FALSE;

#ifdef _DEBUG
	bWindow = TRUE;
#endif

	m_pRenderer->Init(Window, bWindow);			// �����_�����O
	m_pLight->Init();							// ���C�g
	m_pCamera->Init();							// �J����

	m_pTexture->LoadAll();						// �S�Ẵe�N�X�`���̓ǂݍ���

	m_pSound = CSound::Create(Window);			// �T�E���h�̐���

	m_pFade = CFade::Create();					// �t�F�[�h

	// �p�[�e�B�N���}�l�[�W���̐���
	m_pParticleManager = new CParticleManager;
	m_pParticleManager->LoadText("particle.txt");

	//�����[�X���̓}�E�X�|�C���^�[������
	m_pInput->SetCursorErase(false);
#ifdef _DEBUG
	m_pDebugProc = CDebugProc::Create();		// �f�o�b�O�e�L�X�g

	//�f�o�b�N���̓}�E�X�|�C���^�[��\��
	m_pInput->SetCursorErase(true);
#endif

	// �Q�[�����[�h
	m_modeType = MODE_TITLE;

	// ���[�h�̒��
	SetMode(m_modeType);

	srand((unsigned int)time(0));

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CApplication::Uninit()
{
	// ���͏����̏I������
	m_pInput->Uninit();

	// �e�N�X�`���̏I��
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// ���f���̏I��
	if (m_pModel != nullptr)
	{
		m_pModel->ReleaseAll();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// ���[�V�����̏I��
	if (m_pMotion != nullptr)
	{
		m_pMotion->ReleaseAll();
		delete m_pMotion;
		m_pMotion = nullptr;
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

	// �p�[�c�t�@�C���̏I��
	if (m_pPartsFile != nullptr)
	{
		m_pPartsFile->Uninit();
		delete m_pPartsFile;
		m_pPartsFile = nullptr;
	}

	// �S�ẴI�u�W�F�N�g�̉������
	CObject::ReleaseAll();

	// �p�[�e�B�N���}�l�[�W���̔j��
	if (m_pParticleManager != nullptr)
	{
		m_pParticleManager->ReleaseAll();
		delete m_pParticleManager;
		m_pParticleManager = nullptr;
	}

	while (m_pClient != nullptr)
	{
		
		if (!m_pClient->GetMainConnecter() && !m_pClient->GetTutorialConnecter())
		{
			m_pClient->Uninit();
			delete m_pClient;
			m_pClient = nullptr;
		}
	}

	// �����_�����O�̏I��
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
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

	m_nSkill = CConfirmation_Window::GetSelectChoice();

	CPlayer *pPlayer = nullptr;

	if (m_modeType == MODE_GAME)
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);

	if (CGame::GetTime() != nullptr)
	{
		m_nDestroyTime = CGame::GetTime()->GetTime();
	}
	if (CGame::GetPlayerManager() != nullptr && pPlayer != nullptr)
	{
		m_nTotalDamage = pPlayer->GetDamage();
	}
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
	// �Q�[���J�n������~������
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

		// �S�ẴI�u�W�F�N�g�̉������
		CObject::ReleaseAll();

		// �S�Ẵ��f���Z�b�g�̏I��
		m_pPartsFile->ReleaseAllFile();

		// �SBGM�̏I������
		m_pSound->StopAllBGM();
	}
	// �J�����̏�����
	m_pCamera->Init();

	// �S�Ẵ��f�����̉������
	m_pModel->ReleaseAll();

	// �S�Ẵ��[�V�������̉������
	m_pMotion->ReleaseAll();

	// ���[�h��؂�ւ���
	m_modeType = mode;

	// �w��̃��[�h�̐ݒ�
	switch (m_modeType)
	{
	case CApplication::MODE_TITLE:
		m_pGameMode = CTitle::Create();
		m_pMenu = CTitleMenu::Create();
		m_pMenu->SetDisplay(false);
		break;
	case CApplication::MODE_CHAR_SELECT:
		m_pGameMode = CChar_Select::Create();
		break;
	case CApplication::MODE_STAGE_SELECT:
		m_pGameMode = CStage_Select::Create();
		break;
	case CApplication::MODE_TUTORIAL:
		m_pGameMode = CTutorial::Create();
		break;
	case CApplication::MODE_GAME:
		m_pGameMode = CGame::Create();
		break;
	case CApplication::MODE_RESULT:
		m_pGameMode = CResult::Create();
		break;
	default:
		break;
	}
}