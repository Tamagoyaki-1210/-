//==============================================================================
//
// OreManager.cpp	(�z�΃}�l�[�W���[.cpp)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// �C���N���[�h
//==============================================================================
#include <time.h>
#include "application.h"
#include "renderer.h"
#include "oremanager.h"
#include "objectX.h"
#include "input.h"
#include "ore.h"
#include"character.h"
#include"game.h"
#include"fever_time.h"

//==============================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================
const float COreManager::Ore_Radius = 50.0f;		// �z�΂̓����蔻��̑傫��(���a)
const int COreManager::REAPPEARANCE_MAX = 12;		// �z�΂̍ďo��
const int COreManager::CREATE_TIME = 500;			// �z�΂����������܂ł̎���

//==============================================================================
// �R���X�g���N�^
//==============================================================================
COreManager::COreManager()
{
	m_nStageNum = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
COreManager::~COreManager()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT COreManager::Init()
{
	//�N�����Ɉ�񂾂��s�����ߏ������ɏ���	
	srand((unsigned int)time(nullptr));

	//==================================================
	// �����o�ϐ��̏�����
	//==================================================
	m_CreateTime = CREATE_TIME;

	
	InitCreate();
	
	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void COreManager::Uninit()
{
	// ���X�g�̍폜
	m_pOreList.clear();

	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void COreManager::Update()
{
	// ���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();

	m_CreateTime--;
	if (m_CreateTime <= 0)
	{
		RandCreate();
		m_CreateTime = CREATE_TIME;
	}

	// �z�΂̃��X�g�𐮗����鏈��
	OreClear();
}

//==============================================================================
// �`�揈��
//==============================================================================
void COreManager::Draw()
{

}

//==============================================================================
// �z�΂̓����蔻�菈��
//==============================================================================
COre* COreManager::OreCollision(D3DXVECTOR3 pos, float radius)
{
	COre* pNearOre = nullptr;	// ��������ԋ߂��z�΂̃|�C���^
	float fNearDistance = 0.0f;

	//�C�e���[�^�[���[�v
	for (auto itr = m_pOreList.begin(); itr != m_pOreList.end();)
	{
		//�C�e���[�^�[����z�΂̃|�C���^�̑��
		COre* pOre = *itr;

		if (pOre != nullptr)
		{
			// �v���C���[�ƍz�΂��ڐG���Ă���ꍇ && �z�΂��o���������Ă���ꍇ
			if (CollisionCircle(pos, radius, pOre->GetPos(), Ore_Radius) && pOre->GetAppearance())
			{
				// �ʒu���m�̋����̌v�Z�p�ϐ�
				D3DXVECTOR2 distance = { pOre->GetPos().x - pos.x, pOre->GetPos().z - pos.z };
				float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

				// �������������ꍇ�A���͍ŏ��̏����̏ꍇ�m�ۂ���
				if (fNearDistance > fDistance || fNearDistance == 0)
				{
					fNearDistance = fDistance;
					pNearOre = pOre;
				}
			}
			itr++;
		}
		//���g�p�̍z�΃|�C���^�̉��
		else
		{
			pOre->Uninit();
			pOre = nullptr;
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_pOreList.erase(itr);
		}
	}

	// ��ԋ߂��z�΂̃|�C���^��Ԃ�
	return pNearOre;
}

//==============================================================================
// �����_���Ő������鏈��
//==============================================================================
void COreManager::RandCreate()
{
	
	for (int nCnt = 0; nCnt < REAPPEARANCE_MAX; nCnt++)
	{
		m_RandX = (float)(rand() % (1400) - 700) + 800;		//x�͈̔͂Ƀ����_��
		m_RandY = (float)(rand() % 200) + 100;				//y�͈̔͂Ƀ����_��
		m_RandZ = (float)(rand() % (1400) - 700) + 800;		//z�͈̔͂Ƀ����_��
		COre *pOre = COre::Create(D3DXVECTOR3(m_RandX, -m_RandY, m_RandZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_nStageNum);

		//���X�g�ɒǉ�
		m_pOreList.push_back(pOre);
	}
}

//==============================================================================
// ���������ɐ������鏈��
//==============================================================================
void COreManager::InitCreate()
{
	for (int nCnt = 0; nCnt < RANDCREATE_MAX; nCnt++)
	{
		m_RandX = (float)(rand() % (1400) - 700) + 800;		//x�͈̔͂Ƀ����_��
		m_RandY = (float)(rand() % 200) + 100;				//y�͈̔͂Ƀ����_��
		m_RandZ = (float)(rand() % (1400) - 700) + 800;		//z�͈̔͂Ƀ����_��
		COre *pOre = COre::Create(D3DXVECTOR3(m_RandX, -m_RandY, m_RandZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), m_nStageNum);

		//���X�g�ɒǉ�
		m_pOreList.push_back(pOre);
	}
}

//==============================================================================
// �z�΂̃��X�g�𐮗����鏈��
//==============================================================================
void COreManager::OreClear()
{
	// �C�e���[�^�[���[�v
	for (auto itr = m_pOreList.begin(); itr != m_pOreList.end();)
	{
		//�C�e���[�^�[����z�΂̃|�C���^�̑��
		COre *pOre = *itr;

		// �z�΂��I�������ꍇ
		if (pOre->GetOreDeath() == true)
		{
			pOre->Uninit();
			pOre = nullptr;
			//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
			itr = m_pOreList.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

//==============================================================================
// �z�΍폜����
//==============================================================================
void COreManager::OreDeath(COre* ore)
{
	// �I�������z�΂̏I������
	ore->Uninit();

	// �I�������z�΂�z�񂩂珜�O����
	m_pOreList.erase(std::find(m_pOreList.begin(), m_pOreList.end(), ore));
}

//==============================================================================
// ��������
//==============================================================================
COreManager *COreManager::Create(int nStageNum)
{
	COreManager *pOreManager = nullptr;

	pOreManager = new COreManager;

	if (pOreManager != nullptr)
	{
		pOreManager->SetStageNum(nStageNum);
		pOreManager->Init();
	}

	return pOreManager;
}
