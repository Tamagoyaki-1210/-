//==============================================================================================
//
// �Q�[���@�@�@game.cpp
// tutida ryousei
//
//==============================================================================================
#include"game.h"
#include"application.h"
#include"fade.h"
#include"camera.h"
#include"map.h"
#include"objectX.h"
#include"score.h"
#include"time.h"
#include"countdown.h"
#include"skybox.h"
#include"oremanager.h"
#include"shop.h"
#include"ghost.h"
#include"pteranodon.h"
#include"alien.h"
#include "area.h"
#include"charmanager.h"
#include"input.h"
#include"fever_time.h"
#include"enemymanager.h"
#include"enemyspawner.h"
#include"scoreitemmanager.h"
#include"itemmanager.h"
#include"stage_select.h"
#include"fontString.h"

CMap *CGame::m_pMap = nullptr;
CTime *CGame::m_pTime = nullptr;
CShop *CGame::m_pShop[] = {};
COreManager *CGame::m_pOreManager = nullptr;
CEnemyManager *CGame::m_pEnemyManager = nullptr;
CScoreItemManager *CGame::m_pScoreItemManager = nullptr;
CItemManager *CGame::m_pItemManager = nullptr;
CEnemySpawner *CGame::m_pEnemySpawner = nullptr;
CFever_Time *CGame::m_pFever_Time = nullptr;
CArea *CGame::m_pArea = nullptr;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CGame::CGame() : m_nEnemySpawnerCounter(0)
{
	m_nTop_Score = 0;	// �ő�X�R�A�̏�����
	m_nTopCameraNum = -1;//���ʊm�F
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CGame::~CGame()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CGame::Init()
{
	// �G�L�����}�l�[�W���[�̐���
	m_pEnemyManager = new CEnemyManager;

	// �X�R�A�A�C�e���}�l�[�W���[�̐���
	m_pScoreItemManager = new CScoreItemManager;

	// �A�C�e���}�l�[�W���[�̐���
	m_pItemManager = new CItemManager;

	// �I�񂾃X�e�[�W�̔ԍ�
	m_nStage_Number = CStage_Select::GetStage_Number();

	// �J�����̃|�C���^
	CCamera* pCamera = CApplication::GetCamera();

	// ���_�A�����_�̐ݒ�
	pCamera->SetPosV({ 800.0f, 1200.0f, -250.0f });
	pCamera->SetPosR({ 800.0f, 200.0f, 500.0f });

	// ����
	m_pMap = new CMap;						// �}�b�v
	m_pTime = CTime::Create();				// �^�C���̐���

	// �S�ẴL�����N�^�[�̐���
	CApplication::GetCharManager()->SetAllChara(m_nStage_Number);

	// �����̐���
	m_pShop[0] = CShop::Create({ 130.0f,0.0f,1470.0f }, { 0.0f,0.0f,0.0f });
	m_pShop[1] = CShop::Create({ 1470.0f,0.0f,1470.0f }, { 0.0f,0.0f,0.0f });
	m_pShop[2] = CShop::Create({ 130.0f,0.0f,130.0f }, { 0.0f,D3DX_PI,0.0f });
	m_pShop[3] = CShop::Create({ 1470.0f,0.0f,130.0f }, { 0.0f,D3DX_PI,0.0f });

	// �X�J�C�{�b�N�X�̐���
	CSkyBox::Create(D3DXVECTOR3(800.0f, 0.0f, 800.0f), m_nStage_Number);

	// �z�΂̐���
	m_pOreManager = COreManager::Create(m_nStage_Number);

	// �}�b�v�̏�����
	m_pMap->Init(m_nStage_Number);

	//�@�G���A�̐���
	m_pArea = CArea::Create();

	// �G�L�����X�|�i�[�̐���
	m_pEnemySpawner = CEnemySpawner::Create({ 800.0f, 0.0f, 800.0f }, CStage_Select::GetStage_Number());

	// �X�e�[�W���̐ݒ�
	switch (CStage_Select::GetStage_Number())
	{
	case CStage_Select::STAGE_1:
		// �h���b�v�A�C�e���̃��f������ǂݍ���
		for (int nCnt = 0; nCnt < CScoreItem::MAX_SCOREITEM_MODEL; nCnt++)
		{
			CObjectX::ReadObject(CScoreItem::STAGE_SCOREITEM_01[nCnt]);
		}
		// �S�[�X�g�̃��f���̓ǂݍ���
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Ghost/Tresure/Ghost.x");

		// �Q�[�����BGM1
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME1);
		break;
	case CStage_Select::STAGE_2:
		// �h���b�v�A�C�e���̃��f������ǂݍ���
		for (int nCnt = 0; nCnt < CScoreItem::MAX_SCOREITEM_MODEL; nCnt++)
		{
			CObjectX::ReadObject(CScoreItem::STAGE_SCOREITEM_02[nCnt]);
		}
		// �v�e���m�h���̃��f���̓ǂݍ���
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/body.x");		// [0]��
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/head.x");		// [1]��
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/arm_left.x");	// [2]���r
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/arm_right.x");	// [3]�E�r
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/leg.x");		// [4]��

		// �Q�[�����BGM2
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME2);
		break;
	case CStage_Select::STAGE_3:
		// �h���b�v�A�C�e���̃��f������ǂݍ���
		for (int nCnt = 0; nCnt < CScoreItem::MAX_SCOREITEM_MODEL; nCnt++)
		{
			CObjectX::ReadObject(CScoreItem::STAGE_SCOREITEM_03[nCnt]);
		}
		// �G�C���A���̃��f���̓ǂݍ���
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Alien/Alien/ufo.x");		// [0]UFO
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Alien/Alien/alien.x");		// [1]�G�C���A��

		// �Q�[�����BGM3
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME3);
		break;
	default:
		break;
	}

	// �Q�[���̊J�n��������������J�E���g�_�E���̐���
	CCountdown::Create();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CGame::Uninit()
{
	// �G���A�̏I������
	if (m_pArea != nullptr)
	{
		m_pArea->Uninit();
		m_pArea = nullptr;
	}

	// �}�b�v�̏I������
	if (m_pMap != nullptr)
	{
		m_pMap->Uninit();
		delete m_pMap;
		m_pMap = nullptr;
	}

	// �^�C�}�[�̏I������
	if (m_pTime != nullptr)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = nullptr;
	}

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		// �^�C�}�[�̏I������
		if (m_pShop[nCnt] != nullptr)
		{
			m_pShop[nCnt]->Uninit();
			m_pShop[nCnt] = nullptr;
		}
	}

	// �z�΃}�l�[�W���[�̏I������
	if (m_pOreManager != nullptr)
	{
		m_pOreManager->Uninit();
		m_pOreManager = nullptr;
	}

	// �G�L�����}�l�[�W���[�̏I������
	if (m_pEnemyManager != nullptr)
	{
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}

	// �G�L�����X�|�i�[�̏I������
	if (m_pEnemySpawner != nullptr)
	{
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}

	// �X�R�A�A�C�e���}�l�[�W���[�̏I������
	if (m_pScoreItemManager != nullptr)
	{
		delete m_pScoreItemManager;
		m_pScoreItemManager = nullptr;
	}

	// �A�C�e���}�l�[�W���[�̏I������
	if (m_pItemManager != nullptr)
	{
		delete m_pItemManager;
		m_pItemManager = nullptr;
	}

	// �t�B�[�o�[�^�C���̏I������
	if (m_pFever_Time != nullptr)
	{
		m_pFever_Time->Uninit();
		m_pFever_Time = nullptr;
	}

	// �|�[�Y���~�߂�
	CApplication::SetPauce(false);

	// �X�e�[�W����BGM���I��
	switch (CStage_Select::GetStage_Number())
	{
	case CStage_Select::STAGE_1:
		// �Q�[�����BGM1�̏I��
		CApplication::GetSound()->StopLabel(CSound::SOUND_LABEL_BGM_GAME1);
		break;
	case CStage_Select::STAGE_2:
		// �Q�[�����BGM2�̏I��
		CApplication::GetSound()->StopLabel(CSound::SOUND_LABEL_BGM_GAME2);
		break;
	case CStage_Select::STAGE_3:
		// �Q�[�����BGM3�̏I��
		CApplication::GetSound()->StopLabel(CSound::SOUND_LABEL_BGM_GAME3);
		break;
	default:
		break;
	}
}

//==============================================================================================
//�X�V����
//==============================================================================================
void CGame::Update()
{
	// �}�b�v�̍X�V
	m_pMap->Update();

	// �^�C�}�[�̍X�V
	m_pTime->Update();

	//�@�G���A�ϓ��p�̃v���C���[�̏��ʂ̕ۑ�
	m_pArea->SetTopCharaNum(m_nTopCameraNum);

	// �Q�[�����J�n����Ă���ꍇ
	if (CApplication::GetGameStart() == true)
	{
		// �L�����̏��ʏ���
		CharaRanking();

		// �G�L�����o������
		EnemySpawn();

		// �t�B�[�o�[�^�C���̐ݒ�
		SetFeverTime();
	}

#ifdef _DEBUG
	CInput* pInput = CInput::GetKey();

	// ���U���g�ɔ�ԏ���(�f�o�b�O�p)
	if (pInput->Trigger(DIK_F1))
	{
		CFade::SetFade(CApplication::MODE_RESULT, 0.05f);
	}
#endif
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CGame::Draw()
{
	// �}�b�v�̕`�揈��
	m_pMap->Draw();
}

//==============================================================================================
// �L�����̏��ʏ���
//==============================================================================================
void CGame::CharaRanking()
{
	// �S�L�����̃|�C���^�̊i�[��
	CCharacter* pChara[CCharManager::Max_Character] = {};
	int nScore[CCharManager::Max_Character] = {};

	int nLow_Score = 1;						// ��ԒႢ�X�R�A(100000000 - 1 = 99999999)
	for (int nCnt = 0; nCnt < SCORE_DIGIT; nLow_Score *= 10, nCnt++) {}
	nLow_Score--;
	int nTopScore = 0;

	// �S�L�����̃X�R�A���r����
	for (int index = 0; index < CCharManager::Max_Character; index++)
	{
		// �e�L�����̃|�C���^���i�[
		pChara[index] = CApplication::GetCharManager()->GetChara(index);

		// �L�����̃X�R�A
		 nScore[index] = pChara[index]->GetScore()->GetScore();

		// �X�R�A���ő�l���傫���ꍇ
		if(m_nTop_Score < nScore[index])
		{
			// �ő�l�̍X�V
			m_nTop_Score = nScore[index];

			//���ʂ����邩�ǂ���
			if (nTopScore != m_nTop_Score)
			{
				nTopScore = m_nTop_Score;

				//���ʂ̕ۑ�
				m_nTopCameraNum = index;
			}
			else
			{
				//���ʂ̕ۑ�
				m_nTopCameraNum = -1;
			}
			
		}
		// �X�R�A���ő�l��菬�����ꍇ ���� �ŏ��l���X�V����Ă��Ȃ��ꍇ
		else if (nLow_Score > nScore[index])
		{
			// �ŏ��l�̍X�V
			nLow_Score = nScore[index];
		}
	}

	// �ő�X�R�A����x�ł��X�V���ꂽ�ꍇ
	if (m_nTop_Score != 0)
	{
		// �S�L�����̃X�R�A�ŉ����̕\����؂�ւ���
		for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
		{
			// �t�B�[�o�[�^�C���ł͂Ȃ��ꍇ
			if (m_pFever_Time == nullptr)
			{
				// �ő�l���ŏ��l�Ɉ�v�����ꍇ
				pChara[nCnt]->SetCrown(nScore[nCnt] == m_nTop_Score);
			}
		}
	}
}

//==============================================================================================
// �G�L�����̐�������
//==============================================================================================
void CGame::EnemySpawn()
{
	// �G�L�������N���g�p����Ă��Ȃ��ꍇ
	if (m_pEnemySpawner == nullptr)
	{
		if (m_nEnemySpawnerCounter++ >= EnemySpawner_Timer)
		{
			m_pEnemySpawner = CEnemySpawner::Create({ 800.0f, 0.0f, 800.0f }, CStage_Select::GetStage_Number());

			m_nEnemySpawnerCounter = 0;
		}
	}
}

//==============================================================================================
// �t�B�[�o�[�^�C���̐ݒ�
//==============================================================================================
void CGame::SetFeverTime()
{
	// �c�莞�Ԃ��t�B�[�o�[�^�C���ȉ��ɂȂ����ꍇ && �t�B�[�o�[�^�C���ł͂Ȃ��ꍇ
	if (m_pTime->GetTime() <= FeverStart_Count && m_pFever_Time == nullptr)
	{
		// �t�B�[�o�[�^�C��
		m_pFever_Time = CFever_Time::Create(FeverStart_Count, GemCreate_Count);

		// �L�����N�^�[�̏���ǂݍ���
		for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
		{
			CCharacter* pChara = CApplication::GetCharManager()->GetChara(nCnt);

			// �t�B�[�o�[�^�C�����ɉ����ƃX�R�A���\���ɂ��鏈��
			pChara->SetCrown(false);
			pChara->GetScore()->SetCompulsDispray(false);

			// �t�B�[�o�[�p�̃A�C�R���𐶐�
			pChara->GetScore()->SetFiver();
		}
	}
}

//==============================================================================================
// �G�L�����X�|�i�[����������
//==============================================================================================
void CGame::EnemySpawnerDeath()
{
	m_pEnemySpawner->Uninit();
	m_pEnemySpawner = nullptr;
}

//==============================================================================================
// ��������
//==============================================================================================
CGame *CGame::Create()
{
	CGame *pGame = nullptr;

	pGame = new CGame;

	if (pGame != nullptr)
	{
		pGame->Init();
	}

	return pGame;
}