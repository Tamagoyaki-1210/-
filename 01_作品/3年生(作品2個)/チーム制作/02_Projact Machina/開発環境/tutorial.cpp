//==============================================================================================
//
// �`���[�g���A���@�@�@tutorial.cpp
// tutida ryousei
//
//==============================================================================================
#include"tutorial.h"
#include"camera.h"
#include"application.h"
#include "player_manager.h"
#include "enemy_manager.h"
#include "drop_manager.h"
#include "collision_manager.h"
#include"halfsphere.h"
#include"meshfield.h"
#include"energy_gauge.h"
#include"input.h"
#include"fade.h"
#include"fontString.h"
#include"pause.h"
#include "parts_file.h"
#include "motion.h"
#include "map_object_manager.h"

CPlayerManager* CTutorial::m_pPlayerManager = nullptr;
CEnemyManager* CTutorial::m_pEnemyManager = nullptr;
CDropManager* CTutorial::m_pDropManager = nullptr;
CCollision_Manager* CTutorial::m_pCollision_Manager = nullptr;
CMeshField *CTutorial::pMeshField = nullptr;			// ���b�V���t�B�[���h
CPause *CTutorial::m_pPause = nullptr;
CMap_Object_Manager *CTutorial::m_pMap_Object_Manager = nullptr;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CTutorial::CTutorial()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CTutorial::~CTutorial()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CTutorial::Init()
{
	CFontString::Create({ 390.0f, SCREEN_HEIGHT / 2, 0.0f }, { 50.0f, 50.0f }, "�`���[�g���A��");

	// �J�����̃|�C���^
	CCamera* pCamera = CApplication::GetCamera();

	// ���_�A�����_�̐ݒ�
	pCamera->SetPosV({ 0.0f, 500.0f, -1000.0f });
	pCamera->SetPosR({ 0.0f, 250.0f, 1000.0f });

	// �S�Ẵ��f���p�[�c�̓ǂݍ���
	CApplication::GetPartsFile()->LoadAllFile();

	// �S�Ẵ��f���p�[�c�̓ǂݍ���
	CApplication::GetMotion()->LoadAllFile();

	m_pPlayerManager = CPlayerManager::Create();	// �v���C���[�}�l�[�W���[�̐���
	m_pEnemyManager = new CEnemyManager;			// �G�L�����}�l�[�W���[�̐���
	m_pDropManager = new CDropManager;				// ���Ƃ����}�l�[�W���[�̐���
	m_pCollision_Manager = new CCollision_Manager;	// �����蔻��}�l�[�W���[�̐���
	m_pMap_Object_Manager = new CMap_Object_Manager;

	// �S�Ẵ��f���p�[�c�̓ǂݍ���
	CDrop_Weapon* pWeaponDummer = new CDrop_Weapon;
	pWeaponDummer->LoadAllFile();
	pWeaponDummer->Uninit();

	// �v���C���[�̐���(�e�X�g)
	m_pPlayerManager->SetPlayer({ 0.0f, 0.0f, 0.0f }, CPlayerManager::TYPE_PC, 0);

	// �n�[�t�X�t�B�A�̐���
	CHalfSphere *pHalfSphere = CHalfSphere::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20000.0f, 20000.0f, 20000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CHalfSphere::SPHERE_UP);
	pHalfSphere->LoadTexture("Data/texture/sky000.jpg");

	// ���b�V���t�B�[���h�̐���
	pMeshField = CMeshField::Create({ 0.0f, 0.0f, 0.0f }, 10, 10, 4000.0f);

	// �|�[�Y���
	m_pPause = CPause::Create();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CTutorial::Uninit()
{
	// �v���C���[�}�l�[�W���[�̔j��
	if (m_pPlayerManager != nullptr)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = nullptr;
	}

	// �G�L�����}�l�[�W���[�̔j��
	if (m_pEnemyManager != nullptr)
	{
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}

	// ���Ƃ����}�l�[�W���[�̔j��
	if (m_pDropManager != nullptr)
	{
		delete m_pDropManager;
		m_pDropManager = nullptr;
	}

	// �R���W�����}�l�[�W���[�̔j��
	if (m_pCollision_Manager != nullptr)
	{
		// �S�Ă̓����蔻��̉������
		m_pCollision_Manager->ReleaseAllCollision();
		delete m_pCollision_Manager;
		m_pCollision_Manager = nullptr;
	}
	
	//�}�b�v�I�u�W�F�N�g�̔j��
	if (m_pMap_Object_Manager != nullptr)
	{
		delete m_pMap_Object_Manager;
		m_pMap_Object_Manager = nullptr;
	}
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CTutorial::Update()
{
	CInput* pInput = CInput::GetKey();

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if ((pInput->Trigger(DIK_RETURN) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
			|| pInput->Trigger(JOYPAD_START, nCnt))
			&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CFade::SetFade(CApplication::MODE_GAME, 0.05f);
		}
	}
}

//==============================================================================================
// ��������
//==============================================================================================
CTutorial *CTutorial::Create()
{
	CTutorial* pTutorial = new CTutorial;

	if (FAILED(pTutorial->Init()))
	{
		return nullptr;
	}

	return pTutorial;
}