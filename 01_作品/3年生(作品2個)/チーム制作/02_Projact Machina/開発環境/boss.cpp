//=============================================================================
//
// �{�X�L�����@�@�@boss.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "boss.h"
#include "application.h"
#include "game.h"
#include "particle_emitter.h"
#include "boss_life_gauge.h"
#include "camera.h"
#include "particle_emitter.h"
#include "normal_bullet.h"
#include "player_manager.h"
#include "cannon.h"
#include "weapon_attack.h"
#include "parabola_bullet.h"

const float CBoss::BOSS_COLLISION_RADIUS = 1000.0f;	// �{�X�̓����蔻��̑傫��
//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CBoss::CBoss()
{
	// �{�X�L����
	SetEnemyType(ENEMY_TYPE_BOSS);

	// �{�X�̏����l��ݒ�
	SetMaxLife(FIRST_MAX_LIFE);
	SetLife(FIRST_MAX_LIFE);
	SetRadius(BOSS_COLLISION_RADIUS);
}

//=====================================
// �f�X�g���N�^
//=====================================
CBoss::~CBoss()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CBoss::Init()
{
	// �{�X�̃��f���p�[�c��ݒ�
	SetParts(PARTS_BODY, CParts_File::PARTS_BOSS, CMotion::MOTION_BOSS);
	GetParts(PARTS_BODY)->GetModel(0)->SetSize({ 5.0f, 5.0f, 5.0f });

	SetGaugeManager(CBoss_Life_Gauge::Create({ 1280 / 2, 100.0f,0.0f }, { 800.0f,15.0f }));

	CEnemy::Init();

	CParts* pBody = GetParts(PARTS_BODY);
	pBody->SetMotion(MOTION_NEUTRAL);

	m_bOpening = true;

	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetOpening(m_bOpening);

	pCamera->SetPosV({ 0.0f,200.0f,-1000.0f });
	pCamera->SetPosR({ 0.0f, 3000.0f, -500.0f });
	pCamera->SetRot({ 0.0f,0.0f,0.0f });

	// �����蔻��̃^�C�v
	SetCollision_Type(COLLISION_TYPE_BOSS);

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CBoss::Uninit()
{
	CEnemy::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CBoss::Update()
{
	// �L�����N�^�[�̍X�V
	CEnemy::Update();

	ChangeMotion();

	// �J�����̃|�C���^
	bool bOpening = CApplication::GetCamera()->GetOpening();

	if (m_bOpening != bOpening)
	{
		// �I�[�v�j���O�I�����ɒ��n����
		D3DXVECTOR3 pos = GetPos();
		SetPos({ pos.x,0.0f,pos.z });

		CParts* pBody = GetParts(PARTS_BODY);
		pBody->SetMotion(MOTION_NEUTRAL);
	}

	m_bOpening = bOpening;
}

//============================================================================
// �`�揈��
//============================================================================
void CBoss::Draw()
{
	CEnemy::Draw();
}

//============================================================================
// �j�󏈗�
//============================================================================
void CBoss::Destroy()
{
	// �{�X�p���j�p�[�e�B�N��
	for (int nCnt = 0; nCnt < 30; nCnt++)
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 50.0f * nCnt;

		// ���j���p�[�e�B�N��
		std::move(CParticleEmitter::Create("Destroy", pos));

		// ����SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
	}

	// �Q�[���I����ݒ肷��
	CGame::SetGameEnd();

	CEnemy::Destroy();

	// BGM���~����
	CApplication::GetSound()->StopAllBGM();
}

//============================================================================
// ���[�V�����ύX����
//============================================================================
void CBoss::ChangeMotion()
{
	// �j���[�g�������[�V�������[�V������ݒ�
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		// �p�[�c
		CParts* pParts = GetParts(nCnt);

		// ATTACK1�̎���ATTACK2���o��
		if (pParts->GetMotion() == MOTION_ATTACK1 && pParts->GetMotionStop() == true)
		{
			pParts->SetMotion(MOTION_ATTACK2);
			CWeapon_Attack::Create(GetPos(), 1100, false, 600, 20);
		}
		// ���[�V���������[�v���Ȃ��ꍇ
		else if (pParts->GetMotionLoop() == false && pParts->GetMotionStop() == true)
		{
			// �j���[�g�������[�V�����ɂ���
			pParts->SetMotion(MOTION_NEUTRAL);
		}
	}
}

//============================================================================
// ���n����
//============================================================================
void CBoss::Landing(const D3DXVECTOR3 pos)
{
	// �J�����̃|�C���^
	bool bOpening = CApplication::GetCamera()->GetOpening();

	if (bOpening)
	{
		// ���n���[�V������ݒ�
		for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
		{
			GetParts(nCnt)->SetMotion(MOTION_NEUTRAL);
			// ���n���p�[�e�B�N��
			std::move(CParticleEmitter::Create("Shock", pos));
			std::move(CParticleEmitter::Create("Dust", pos));
		}
	}

	// �L�����N�^�[�̒��n����
	CCharacter::Landing(pos);
}

//============================================================================
// �ړ�����
//============================================================================
void CBoss::Move()
{
	// �S�����͓����Ȃ�
	if (!CCannon::GetRestrain())
	{
		CEnemy::Move();

		// ��ɒǐՏ��
		SetTracking(true);

		// �{�X�ƃv���C���[�̋���
		float fDistance = GetDistance();

		if (GetTracking() && !CApplication::GetCamera()->GetOpening())
		{

			// �v���C���[�܂ł̊p�x
			float fAngle = GetAngle();

			// �G�̈ړ���
			D3DXVECTOR3 move = { 0.0f,GetMove().y,0.0f };

			// ���x�̐ݒ�
			SetSpeed(3.0f);

			if (fDistance >= 1000)
			{
				move.x = sinf(fAngle) * GetSpeed();
				move.z = cosf(fAngle) * GetSpeed();
			}
			else if (fDistance < 1000)
			{
				move.x = sinf(fAngle + D3DX_PI) * GetSpeed();
				move.z = cosf(fAngle + D3DX_PI) * GetSpeed();
			}

			// �U������
			if (fDistance <= 1500)
				Slash_Attack();
			else
			Bullet_Attack();

			// �ړ��ʂ̐ݒ�
			SetMove(move);

			// �p�x�̐ݒ�
			SetRot({ 0.0f,fAngle + D3DX_PI,0.0f });
		}
	}
}

//============================================================================
// �ߐڍU��
//============================================================================
void CBoss::Slash_Attack()
{
	// �{�X�ƃv���C���[�̋���
	//float fDistance = GetDistance();

	CParts* pBody = GetParts(PARTS_BODY);

	// �ߐڍU���̊Ԋu
	m_nAttack_Cooltime++;

	int nRand_Slash = rand() % 2;

	// �ߐڍU��
	if (m_nAttack_Cooltime >= 100 && nRand_Slash == 0)
	{
		pBody->SetMotion(MOTION_ATTACK1);
		CWeapon_Attack::Create(GetPos(), 1100, false, 600, 20);
		m_nAttack_Cooltime = 0;
	}
	else if (m_nAttack_Cooltime >= 100 && nRand_Slash == 1)
	{
		pBody->SetMotion(MOTION_ATTACK3);
		CWeapon_Attack::Create(GetPos(), 1100, false, 400, 80);
		m_nAttack_Cooltime = 0;
	}
}

//============================================================================
// �������U��
//============================================================================
void CBoss::Bullet_Attack()
{
	// �{�X�ƃv���C���[�̋���
	float fDistance = GetDistance();

	// �������U���̊Ԋu
	m_nBullet_Cooltime++;

	CPlayer* pPlayer = nullptr;

	// �v���C���[�̏��
	pPlayer = CGame::GetPlayerManager()->GetPlayer(0);

	D3DXVECTOR3 pos = GetPos();

	// �e�����˂����ʒu
	if (m_nBullet_Cooltime >= 60)
	{
		int nRnd = rand() % 3;

		if (nRnd == 0)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				// �������U��
				CNormal_Bullet::Create({ pos.x,pos.y + (700.0f / 10.0f * nCnt),pos.z }, { 60.0f,60.0f }, { 0.0f,0.0f,0.0f }, fDistance, pPlayer, 0.0f, true, false,
					240, 120, 60);
			}
		}
		if (nRnd == 1)
		{
			// �������U��
			CNormal_Bullet::Create({ pos.x,pos.y + 700.0f,pos.z }, { 1000.0f,1000.0f }, { 0.0f,0.0f,0.0f }, fDistance, pPlayer, 0.0f, true, false,
				1200, 60, 1000);
		}
		if (nRnd == 2)
		{
			// ���̎擾
			D3DXVECTOR3 rot = GetBulletRot();

			D3DXVECTOR3 pos_vec = { -sinf(rot.y), sinf(rot.x), -cosf(rot.y) };

			CParabola_Bullet::Create({ pos.x,pos.y + 700.0f,pos.z }, pos_vec, fDistance, rot, "Data/model/Mob/mob000.x", false, 1200, 60, 1000);
		}

		m_nBullet_Cooltime = 0;
	}
}

//============================================================================
// ��������
//============================================================================
CBoss* CBoss::Create(const D3DXVECTOR3 pos)
{
	CBoss* pBoss = new CBoss;

	if (FAILED(pBoss->Init()))
	{
		return nullptr;
	}

	pBoss->SetPos(pos);

	return pBoss;
}