//==============================================================================================
//
// �ʏ�e�@�@�@normal_bullet.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"normal_bullet.h"
#include"object3D.h"
#include "application.h"
#include"enemy_manager.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "player_manager.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CNormal_Bullet::CNormal_Bullet(const PRIORITY priority) : CBullet(priority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CNormal_Bullet::~CNormal_Bullet()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CNormal_Bullet::Init()
{
	CBullet::Init();

	// �e���G�ɓ͂��܂ł̎���
	float fHitCnt = m_fHypotenuse / GetSpeed_XZ();

	if (m_bTarget && pChara != nullptr)
	{
		// �^�[�Q�b�g���Ă���G�̈ʒu
		D3DXVECTOR3 Enemy_Pos = pChara->GetCenterPos();
		// �ړ���
		D3DXVECTOR3 Enemy_Move = pChara->GetMove();

		// �e���͂��Ƃ��̓G�̈ʒu
		Enemy_Pos.x += (Enemy_Move.x * m_fSpeed * fHitCnt);
		Enemy_Pos.z += (Enemy_Move.z * m_fSpeed * fHitCnt);

		// �e����G�܂ł̃x�N�g��
		D3DXVECTOR3 Mob_Vec = Enemy_Pos - GetPos();
		D3DXVec3Normalize(&Mob_Vec, &Mob_Vec);
		SetMove(Mob_Vec);
	}

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CNormal_Bullet::Uninit()
{
	CBullet::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CNormal_Bullet::Update()
{
	CBullet::Update();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CNormal_Bullet::Draw()
{
	CBullet::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CNormal_Bullet *CNormal_Bullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, D3DXVECTOR3 move, float hypotenuse, CCharacter *chara, float enemy_speed, bool target, const bool side, const int power, const float speed, const int life)
{
	//�N���X�̐���
	CNormal_Bullet* pNormal_Bullet = new CNormal_Bullet(PRIORITY_BACK);

	//null�`�F�b�N
	if (pNormal_Bullet != nullptr)
	{
		pNormal_Bullet->SetPos(pos);
		pNormal_Bullet->SetSize(size);
		pNormal_Bullet->SetMove(move);
		pNormal_Bullet->SetPlayerSide(side);
		pNormal_Bullet->SetPower(power);
		pNormal_Bullet->m_fHypotenuse = hypotenuse;
		pNormal_Bullet->pChara = chara;
		pNormal_Bullet->m_fSpeed = enemy_speed;
		pNormal_Bullet->SetSpeed(speed);
		pNormal_Bullet->SetLife(life);
		pNormal_Bullet->m_bTarget = target;
		pNormal_Bullet->Init();

		// �e�I�u�W�F�N�g�̐���
		pNormal_Bullet->SetObj3D(CObject3D::Create(pos, size, PRIORITY_BACK));
		// �e�̃e�N�X�`��
		pNormal_Bullet->GetObj3D()->SetTexture(CTexture::TEXTURE_BULLET);

		// �r���{�[�h�̐ݒ�
		pNormal_Bullet->GetObj3D()->Setbillboard(true);
	}
	else
		assert(false);

	return pNormal_Bullet;
}