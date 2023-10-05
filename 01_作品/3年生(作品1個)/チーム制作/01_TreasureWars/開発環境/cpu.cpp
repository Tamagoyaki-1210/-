//==============================================================================================
//
// CPU�@�@�@cpu.cpp
// tutida ryousei
//
//==============================================================================================
#include"cpu.h"
#include"application.h"
#include"game.h"
#include"charManager.h"
#include"shop.h"
#include"ore.h"
#include"oreManager.h"
#include"scoreitemmanager.h"
#include"scoreitem_stack.h"
#include"itemmanager.h"
#include"enemymanager.h"
#include"debugProc.h"
#include "particle_emitter.h"

const float CCpu::Radius_Search_Ore = 1200.0f;		// �z�΂̌����̃T�C�Y(���a)
const float CCpu::Radius_Search_ScoreItem = 300.0f;	// �X�R�A�A�C�e���̌����̃T�C�Y(���a)
const float CCpu::Radius_Search_Chara = 150.0f;		// �L�����̌����̃T�C�Y(���a)
const float CCpu::Radius_Search_Enemy = 100.0f;		// �G�L�����̌����̃T�C�Y(���a)
//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CCpu::CCpu()
{
	// �U���͂̏����l��ݒ�
	SetMinePower(First_Mine_Power);

	// �F�X�l����v�l
	m_trialRoutine = TRIAL_ROUTINE_NEUTRAL;

	m_targetPos = { 0.0f, 0.0f, 0.0f };

	m_bSetAttack = false;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CCpu::~CCpu()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CCpu::Init()
{
	CCharacter::Init();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CCpu::Uninit()
{
	CCharacter::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CCpu::Update()
{
	// ���[�V�����ԍ��̐ݒ�
	ChangeMotion();

	// ���[�V����
	Motion();

	// �Q�[�����J�n����Ă���ꍇ
	if (CApplication::GetGameStart())
	{
		// �m�b�N�o�b�N����
		bool bKnockBack = CCharacter::GetKnockBack();

		if (!bKnockBack)
		{
			// �ړ�����
			Move();

			// AI����
			Ai();
		}
		else
		{
			// �X�R�A�A�C�e�����������Ă���ꍇ
			if (GetNum_StackScoreItem() != 0)
			{
				// �X�R�A�A�C�e���𔄋p����v�l
				m_trialRoutine = TRIAL_ROUTINE_SALE;
			}
		}

		CCharacter::Update();
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CCpu::Draw()
{
	CCharacter::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CCpu *CCpu::Create(D3DXVECTOR3 pos, int index)
{
	CCpu *pCpu = nullptr;

	pCpu = new CCpu;

	if (pCpu != nullptr)
	{
		pCpu->SetPos(pos);
		pCpu->SetIndex(index);
		pCpu->Init();
	}

	return pCpu;
}

//==============================================================================================
// �ړ�����
//==============================================================================================
void CCpu::Move()
{
	// �U�����ł͂Ȃ��ꍇ
	if (!GetAttack())
	{
		// �������������ꍇ
		if (m_targetPos != D3DXVECTOR3(0.0f, 0.0, 0.0f))
		{
			// ����
			SetMotion(MOTION_WALK);
			SetMove(true);

			// �ړI�ʒu�Ǝ��g�̈ʒu�̍���
			D3DXVECTOR3 distance = { m_targetPos.x - GetPos().x, 0.0f, m_targetPos.z - GetPos().z };

			// �����������֌���
			float fAngle = (float)atan2(-distance.x, -distance.z);

			// �L�����N�^�[�̕�������
			CCharacter::SetRotDest({ 0.0f, fAngle, 0.0f });

			// �ړ���
			D3DXVECTOR3 move = { 0.0f, 0.0f, 0.0f };

			// �ړ��ʂ̏�����
			D3DXVec3Normalize(&move, &distance);

			// �ړ��ʂ��X�V
			CCharacter::SetMove(move);

			// �ړ�����
			CCharacter::Move();
		}
		// �����������ꍇ
		else
		{
			// �������I��������
			SetMotion(MOTION_NEUTRAL);
		}
		// �ړI�ʒu�̏�����
		m_targetPos = { 0.0f, 0.0f, 0.0f };
	}
}

//==============================================================================================
// �A�N�V����
//==============================================================================================
void CCpu::Action()
{
	// �U�����ł͂Ȃ��ꍇ
	if (GetMotion_Type() != MOTION_MINE)
	{
		// �U�����[�V����
		SetMotion(MOTION_MINE);

		// �U������
		SetAttack(true);
	}

	// �^�C���X���b�v
	if (d % 300 == 0 && !GetInvocation())
	{
		/*SetTimeSlip(true);
		SetInvocation(true);*/
	}
}

//==============================================================================================
// AI����
//==============================================================================================
void CCpu::Ai()
{
	// �����l
	int nSearch = 0;

	// �v�l���[�`���̎�ނ�AI�𔻒f
	switch (m_trialRoutine)
	{
	// �F�X�l����v�l
	case TRIAL_ROUTINE_NEUTRAL:

		// �����ȊO�̑S�Ă���������
		nSearch = SEARCH_CHARA | SEARCH_ORE | SEARCH_SCOREITEM | SEARCH_ITEM | SEARCH_ENEMY;
		break;

	// �e�v���C���[���߂��ɂ���ꍇ�A�^�������Ɏ�������U������v�l
	case TRIAL_ROUTINE_ATTACK:

		// �L�����ƃX�R�A�A�C�e���ƓG�L��������������
		nSearch = SEARCH_CHARA | SEARCH_SCOREITEM | SEARCH_ITEM | SEARCH_ENEMY;
		break;

	// �������Ă����΂𔄂�؂�v�l
	case TRIAL_ROUTINE_SALE:

		// �����݂̂���������
		nSearch = SEARCH_SHOP;
		break;
	default:
		break;
	}

	// ����������ꍇ
	if (nSearch != 0)
	{
		// ��������
		Search(nSearch);

		// �U��������ꍇ
		if (m_bSetAttack == true)
		{
			// �U��������
			Action();

			// ����������
			m_bSetAttack = false;
		}
	}
}

//==============================================================================================
// �T������(�D��x : �~���ɏ㏑��)
//==============================================================================================
void CCpu::Search(const int search)
{
	const D3DXVECTOR3 pos = GetPos();
	const float radius = GetRadius();

	// �L�����̌�������
	if (((search & SEARCH_CHARA) == SEARCH_CHARA))
	{
		CharaSearch(pos, radius);
	}
	// �U�����ł͂Ȃ��ꍇ
	if (!m_bSetAttack)
	{
		// �z�΂̌�������
		if (((search & SEARCH_ORE) == SEARCH_ORE))
		{
			OreSearch(pos, 0.0f);
		}
		// �X�R�A�A�C�e���̌�������
		if (((search & SEARCH_SCOREITEM) == SEARCH_SCOREITEM))
		{
			ScoreItemSearch(pos);
		}
		// �A�C�e���̌�������
		if (((search & SEARCH_ITEM) == SEARCH_ITEM))
		{
			ItemSearch(pos);
		}
		// �G�L�����̌�������
		if (((search & SEARCH_ENEMY) == SEARCH_ENEMY))
		{
			EnemySearch(pos, radius);
		}
		// �����̌�������
		if (((search & SEARCH_SHOP) == SEARCH_SHOP))
		{
			ShopSearch(pos, radius);
		}
	}
}

//==============================================================================================
// �L�����̒T������
//==============================================================================================
void CCpu::CharaSearch(const D3DXVECTOR3 pos, const float radius)
{
	CCharacter* pNearChara = nullptr;	// ��������ԋ߂��L�����̃|�C���^
	float fNearDistance = 0.0f;

	// �S�ẴL������ǂݍ���
	for (int nIndex = 0; nIndex < CCharManager::Max_Character; nIndex++)
	{
		// ���g�̔ԍ��ł͂Ȃ��ꍇ
		if (nIndex != GetIndex())
		{
			CCharacter* pChara = CApplication::GetCharManager()->CCharManager::GetChara(nIndex);

			// ����̃L��������΂��������Ă���ꍇ && ���肪���S�n�тɂ��Ȃ��ꍇ
			if (pChara->GetNum_StackScoreItem() > 0 && !pChara->GetSafe())
			{
				const D3DXVECTOR3 atherPos = pChara->GetPos();

				// �����͈͂ɑ���̃L����������ꍇ
				if (CollisionCircle(pos, Radius_Search_Chara, atherPos, pChara->GetRadius()))
				{
					// �ʒu���m�̋����̌v�Z�p�ϐ�
					D3DXVECTOR2 distance = { atherPos.x - pos.x, atherPos.z - pos.z };
					float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

					// �������������ꍇ�A���͍ŏ��̏����̏ꍇ�m�ۂ���
					if (fNearDistance > fDistance || fNearDistance == 0.0f)
					{
						fNearDistance = fDistance;
						pNearChara = pChara;
					}
				}
			}
		}
	}
	// �L�����̌����ɐ��������ꍇ
	if (pNearChara != nullptr)
	{
		// �L�����̍U�����
		const float fAttack_Length = GetAttackLength();
		const float fAttack_Size = GetAttackSize();

		// �U��(�~�`)�ݒu�ʒu
		const D3DXVECTOR3 attackPos = pos - D3DXVECTOR3(sinf(GetRot().y) * fAttack_Length, 0.0f, cosf(GetRot().y) * fAttack_Length);

		// ����̈ʒu
		const D3DXVECTOR3 atherPos = pNearChara->GetPos();

		// ������U������v�l
		m_trialRoutine = TRIAL_ROUTINE_ATTACK;

		// ���g�̍U�������蔻����ɑ���̃L����������ꍇ
		if(CollisionCircle(attackPos, fAttack_Size, atherPos, radius)
			|| AttackCollision(pos, GetRot().y, fAttack_Length, CCharacter::Attack_Angle, atherPos, radius))
		{
			// �U��������
			m_bSetAttack = true;
		}
		// �͈͂ɂ��Ȃ��ꍇ
		else
		{
			// �ړI�ʒu�ɂ��̈ʒu��ݒ�
			m_targetPos = atherPos;
		}
	}
	else
	{
		// �F�X�l����v�l
		m_trialRoutine = TRIAL_ROUTINE_NEUTRAL;
	}
}

//==============================================================================================
// �z�΂̒T������
//==============================================================================================
void CCpu::OreSearch(const D3DXVECTOR3 pos, const float radius)
{
	// ��΂̏������ɋ󂫂�����ꍇ
	if (GetNum_StackScoreItem() < GetNumGemStack())
	{
		COre* pOre = CGame::GetOreManager()->OreCollision(pos, Radius_Search_Ore);

		// �z�΂𔭌����Ă���ꍇ
		if (pOre != nullptr)
		{
			// �L�����̍U�����
			const float fAttack_Length = GetAttackLength();
			const float fAttack_Size = GetAttackSize();

			// �U��(�~�`)�ݒu�ʒu
			const D3DXVECTOR3 attackPos = pos - D3DXVECTOR3(sinf(GetRot().y) * fAttack_Length, 0.0f, cosf(GetRot().y) * fAttack_Length);

			// �}�b�v���̃I�u�W�F�N�g�̏��
			const D3DXVECTOR3 atherPos = pOre->GetPos();

			// �Փ˂��Ă��Ȃ��ꍇ
			if (!CollisionCircle(attackPos, fAttack_Size, atherPos, radius)
				&& !AttackCollision(pos, GetRot().y, fAttack_Length, CCharacter::Attack_Angle, atherPos, radius))
			{
				// �x�N�g���̎Z�o
				D3DXVECTOR3 vec = pos - atherPos;

				// �x�N�g���̕������v�Z
				D3DXVECTOR3 vecNolmalize;
				D3DXVec3Normalize(&vecNolmalize, &vec);

				// ���a�̍��v�̑傫��
				const float fTotalRadius = COreManager::Ore_Radius + radius;

				// ���a���m�̑傫�����̃x�N�g��
				D3DXVECTOR3 vec2 = fTotalRadius * vecNolmalize;

				// �傫�����Ō��Z�����x�N�g��
				const D3DXVECTOR3 add = vec2 - vec;

				// �ړI�ʒu�ɂ��̈ʒu��ݒ�
				m_targetPos = atherPos;
			}
			// �z�΂��U���͈͂ɓ������ꍇ
			else
			{
				// �U��������
				m_bSetAttack = true;
			}
		}
		// �z�΂�������Ȃ��ꍇ
		else
		{
			// �X�R�A�A�C�e���𔄋p����v�l
			m_trialRoutine = TRIAL_ROUTINE_SALE;
		}
	}
	// �X�R�A�A�C�e���̏����������^���̏ꍇ
	else
	{
		// �X�R�A�A�C�e���𔄋p����v�l
		m_trialRoutine = TRIAL_ROUTINE_SALE;
	}
}

//==============================================================================================
// �X�R�A�A�C�e���̒T������
//==============================================================================================
void CCpu::ScoreItemSearch(const D3DXVECTOR3 pos)
{
	CScoreItem* pNearScoreItem = nullptr;	// ��������ԋ߂��X�R�A�A�C�e���̃|�C���^
	float fNearDistance = 0.0f;

	// �S�ẴX�R�A�A�C�e���̃|�C���^
	for (auto pScoreItem : CGame::GetScoreItemManager()->GetAllScoreItem())
	{
		const D3DXVECTOR3 atherPos = pScoreItem->GetPos();

		// �X�R�A�A�C�e���̌����͈͂ɂ���ꍇ
		if (CollisionCircle(pos, Radius_Search_ScoreItem, atherPos, CScoreItem::ScoreItem_Radius))
		{
			// �X�R�A�A�C�e�����n�ʂɐڒn���Ă���ꍇ
			if (pScoreItem->GetGround())
			{
				// �ʒu���m�̋����̌v�Z�p�ϐ�
				D3DXVECTOR2 distance = { atherPos.x - pos.x, atherPos.z - pos.z };
				float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

				// �������������ꍇ�A���͍ŏ��̏����̏ꍇ�m�ۂ���
				if (fNearDistance > fDistance || fNearDistance == 0.0f)
				{
					fNearDistance = fDistance;
					pNearScoreItem = pScoreItem;
				}
			}
		}
	}
	// �X�R�A�A�C�e���̌����ɐ��������ꍇ
	if (pNearScoreItem != nullptr)
	{
		// �U�������Ȃ�
		m_bSetAttack = false;

		// �ړI�ʒu�ɂ��̈ʒu��ݒ�
		m_targetPos = pNearScoreItem->GetPos();
	}
}

//==============================================================================================
// �A�C�e���̒T������
//==============================================================================================
void CCpu::ItemSearch(const D3DXVECTOR3 pos)
{
	CItem* pNearItem = nullptr;	// ��������ԋ߂��X�R�A�A�C�e���̃|�C���^
	float fNearDistance = 0.0f;

	// �S�ẴA�C�e���̃|�C���^
	for (auto pItem : CGame::GetItemManager()->GetAllItem())
	{
		const D3DXVECTOR3 atherPos = pItem->GetPos();

		// �X�R�A�A�C�e���̌����͈͂ɂ���ꍇ
		if (CollisionCircle(pos, Radius_Search_ScoreItem, atherPos, pItem->GetRadius()))
		{
			// �ʒu���m�̋����̌v�Z�p�ϐ�
			D3DXVECTOR2 distance = { atherPos.x - pos.x, atherPos.z - pos.z };
			float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

			// �������������ꍇ�A���͍ŏ��̏����̏ꍇ�m�ۂ���
			if (fNearDistance > fDistance || fNearDistance == 0.0f)
			{
				fNearDistance = fDistance;
				pNearItem = pItem;
			}
		}
	}
	// �X�R�A�A�C�e���̌����ɐ��������ꍇ
	if (pNearItem != nullptr)
	{
		// �U�������Ȃ�
		m_bSetAttack = false;

		// �ړI�ʒu�ɂ��̈ʒu��ݒ�
		m_targetPos = pNearItem->GetPos();
	}
}

//==============================================================================================
// �G�L�����̒T������
//==============================================================================================
void CCpu::EnemySearch(const D3DXVECTOR3 pos, const float radius)
{
	for (auto pChara : CGame::GetEnemyManager()->GetAllEnemy())
	{
		const D3DXVECTOR3 atherPos = pChara->GetPos();
		const float atherRadius = pChara->GetRadius();

		// ��΂̌����͈͂ɂ���ꍇ
		if (CollisionCircle(pos, Radius_Search_Enemy, atherPos, atherRadius))
		{
			// ��΂𔄋p����v�l
			m_trialRoutine = TRIAL_ROUTINE_SALE;

			break;
		}
	}
}

//==============================================================================================
// �����̒T������
//==============================================================================================
void CCpu::ShopSearch(const D3DXVECTOR3 pos, const float radius)
{
	CShop* pShop = CGame::GetShop(GetIndex());

	// ���������݂��Ă���ꍇ
	if (pShop != nullptr)
	{
		const D3DXVECTOR3 atherPos = pShop->GetPos();

		// �����̊O�ɂ���ꍇ
		if (!CollisionCircle(pos, radius, atherPos, 30.0f))
		{
			// �ړI�ʒu�ɂ��̈ʒu��ݒ�
			m_targetPos = atherPos;
		}
		// �����̒��ɂ���ꍇ
		else
		{
			// ���p�\�̏ꍇ
			if (GetNum_StackScoreItem() != 0)
			{
				// ��΂𔄋p����v�l
				m_trialRoutine = TRIAL_ROUTINE_SALE;
			}
			// ���p���镨�������ꍇ
			else
			{
				// �F�X�l����v�l
				m_trialRoutine = TRIAL_ROUTINE_NEUTRAL;
			}
		}
	}
}