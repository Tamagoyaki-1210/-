//==============================================================================================
//
// �z�[�~���O�e�@�@�@homing_bullet.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"homing_bullet.h"
#include"application.h"
#include"objectX.h"
#include"enemy_manager.h"
#include "game.h"
#include "tutorial.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CHoming_Bullet::CHoming_Bullet(const PRIORITY priority) : CBullet(priority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CHoming_Bullet::~CHoming_Bullet()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CHoming_Bullet::Init()
{
	CBullet::Init();

	SetLife(180);
	SetSpeed(30.0f);
	SetPower(30);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CHoming_Bullet::Uninit()
{
	CBullet::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CHoming_Bullet::Update()
{
	CBullet::Update();

	D3DXVECTOR3 Mob_Pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 BulletVec = { 0.0f,0.0f,0.0f };
	float fTarget_Scope = 2000.0f;
	float fNearDistance = fTarget_Scope;
	CEnemy* pNearEnemy = nullptr;

	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CEnemyManager* pManager = nullptr;

	// ���[�h���ɓG�L������ǂݍ���
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pManager = CTutorial::GetEnemyManager();
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pManager = CGame::GetEnemyManager();
	}
	// �G���G�̏��
	for (auto pEnemy : pManager->GetAllEnemy())
	{
		if (pEnemy->GetLife() > 0 && !m_bTarget)
		{
			// �G�̈ʒu�̎擾
			Mob_Pos = pEnemy->GetCenterPos();

			// �e����G�̋���
			BulletVec = Mob_Pos - GetPos();
			
			// �����̎Z�o
			float DistanceXZ = sqrtf(BulletVec.x * BulletVec.x
				+ BulletVec.z * BulletVec.z);

			// �v���C���[����G�̒�������
			float m_fHypotenuse = sqrt((BulletVec.y * BulletVec.y) + (DistanceXZ * DistanceXZ));

			// ����2000�ȏ�
			if (m_fHypotenuse > fTarget_Scope)
				continue;

			// �������ׂ�
			if (fNearDistance >= m_fHypotenuse)
			{
				// �Z�����̋����ƈʒu����
				fNearDistance = m_fHypotenuse;
				pNearEnemy = pEnemy;
				m_bTarget = true;
			}
		}
	}

	// ��ԋ߂��G�L�������g�p����Ă���ꍇ
	if (pNearEnemy != nullptr)
	{
		// �^�[�Q�b�g���Ă���G�̈ʒu
		D3DXVECTOR3 Enemy_Pos = pNearEnemy->GetPos();

		// �e����G�܂ł̃x�N�g��
		D3DXVECTOR3 Mob_Vec = Enemy_Pos - GetPos();
		D3DXVec3Normalize(&Mob_Vec, &Mob_Vec);
		SetMove(Mob_Vec);
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CHoming_Bullet::Draw()
{
	CBullet::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CHoming_Bullet *CHoming_Bullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 nearmob_pos, char *filename, const bool side, const int power, const int life)
{
	//�N���X�̐���
	CHoming_Bullet* pHoming_Bullet = new CHoming_Bullet(PRIORITY_BACK);

	if (pHoming_Bullet != nullptr)
	{
		pHoming_Bullet->SetPos(pos);
		pHoming_Bullet->SetMove(move);
		pHoming_Bullet->SetPlayerSide(side);
		pHoming_Bullet->SetPower(power);
		pHoming_Bullet->SetLife(life);
		pHoming_Bullet->m_NearMob_Pos = nearmob_pos;
		pHoming_Bullet->Init();

		// �e�I�u�W�F�N�g�̐���
		pHoming_Bullet->SetObjX(CObjectX::Create(pos, rot, nullptr, filename, PRIORITY_BACK));
	}
	else
		assert(false);

	return pHoming_Bullet;
}