//==============================================================================================
//
// �C��	�@�@cannon.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"cannon.h"
#include"objectX.h"
#include"restraint_switch.h"
#include"map.h"
#include"game.h"
#include"tutorial.h"
#include"enemy_manager.h"
#include"application.h"
#include "particle_emitter.h"

bool CCannon::m_bRestrain = false;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CCannon::CCannon(CObject::PRIORITY priority) : CMove_Object(priority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CCannon::~CCannon()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CCannon::Init()
{
	CMove_Object::Init();

	// �����蔻��̐ݒ�
	SetCollision();

	// �����蔻��̃^�C�v
	SetCollision_Type(CMove_Object::COLLISION_TYPE_BLOCK);

	// �^�O�̐ݒ�
	SetTag(TAG_MAP_OBJECT);

	D3DXVECTOR3 Max = GetObjectX()->GetMaxSize();
	D3DXVECTOR3 Min = GetObjectX()->GetMinSize();

	SetIndex({ Min.x,0.0f,Min.z }, 0);
	SetIndex({ Min.x,0.0f,Max.z }, 1);
	SetIndex({ Max.x,0.0f,Max.z }, 2);
	SetIndex({ Max.x,0.0f,Min.z }, 3);

	SetCenterPos({ 0.0f,Max.y / 2,0.0f });
	SetSize(GetObjectX()->GetMaxSize());

	// �����蔻�������
	SetCollisionNoneHit(true);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CCannon::Uninit()
{
	CMove_Object::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CCannon::Update()
{
	CMove_Object::Update();

	// �S���X�C�b�`�̎擾
	CRestraint_Switch *pRestraint = CGame::GetMap()->GetRestraint_Switch(m_nIndex);

	if (pRestraint->GetPush())
		m_bTarget = true;

	float fRotDest = 0.0f;
	D3DXVECTOR3 Vec = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 Boss_Pos = { 0.0f,0.0f,0.0f };

	if (m_bTarget)
	{
		// ���݂̃��[�h
		CApplication::MODE Mode = CApplication::GetModeType();

		CEnemyManager* pEnemyManager = nullptr;

		// ���[�h���ɓG�L������ǂݍ���
		if (Mode == CApplication::MODE_TUTORIAL)
			pEnemyManager = CTutorial::GetEnemyManager();
		else if (Mode == CApplication::MODE_GAME)
			pEnemyManager = CGame::GetEnemyManager();

		// �G�̏��
		for (auto pEnemy : pEnemyManager->GetAllEnemy())
		{
			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_BOSS
				&& pEnemy->GetLife() != 0)
			{
				Boss_Pos = pEnemy->GetCenterPos();
				Boss_Pos.y += 500.0f;

				// �{�X�܂ł̃x�N�g��
				Vec = Boss_Pos - GetPos();

				// �ړI�̊p�x
				fRotDest = atan2(Vec.x, Vec.z);

				// �ړI�̊p�x�ɂ���
				m_fRot += (fRotDest - m_fRot) * 0.05f;

				// �C�䂪�{�X�̕�������
				GetObjectX()->SetRot({ 0.0f,m_fRot,0.0f });

				// �C�䂩��G�̋���
				float Dis = sqrt(Vec.x * Vec.x + Vec.z * Vec.z);

				if (pRestraint->GetCountSwitch() == CRestraint_Switch::SWITHC_NUM
					&& m_pChain_Manager == nullptr)
				{
					m_nChain_Count++;

					if (m_nChain_Count >= 120)
					{
						m_bRestrain = true;

						// ���̔���
						m_pChain_Manager = CChain_Manager::Create(GetPos(), Vec, Dis, fRotDest + D3DX_PI / 2, 500);
						// �U���p�[�e�B�N��
						std::move(CParticleEmitter::Create("spark", GetPos()));
					}
				}

				break;
			}
		}
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CCannon::Draw()
{
}

//==============================================================================================
// �q�b�g����
//==============================================================================================
void CCannon::Hit(CMove_Object* /*pHit*/)
{
}

//==============================================================================================
// ��������
//==============================================================================================
CCannon *CCannon::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, int index, const PRIORITY priority)
{
	CCannon *pCannon = new	CCannon(priority);

	if (pCannon != nullptr)
	{
		pCannon->SetObjectX(CObjectX::Create(pos, rot, parent, Xfilename, priority));
		pCannon->SetPos(pos);
		pCannon->m_nIndex = index;
		pCannon->Init();
	}

	return pCannon;
}