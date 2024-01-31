//==============================================================================================
//
// �Ȏ˒e�@�@�@parabola_bullet.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"parabola_bullet.h"
#include"objectX.h"
#include"application.h"
#include "utility.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CParabola_Bullet::CParabola_Bullet(const PRIORITY priority) : CBullet(priority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CParabola_Bullet::~CParabola_Bullet()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CParabola_Bullet::Init()
{
	CBullet::Init();

	// XZ�̒e�̑��x
	float Speed = m_fHyptenuse / HIT_COUNT;

	SetLife(1000);
	SetSpeed_XZ(Speed);
	SetSpeed_Y(80);
	SetPower(10);

	D3DXVECTOR3 Vec = GetMove();
	
	// Y���W�̃x�N�g��
	Vec.y = 1;

	D3DXVec3Normalize(&Vec, &Vec);

	SetMove(Vec);

	// �d�͂̐ݒ�
	m_fGravity= 0.03f;

	SetExplosion(true);
	
	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CParabola_Bullet::Uninit()
{
	CBullet::Uninit();
}

//==============================================================================================
// �X�V������
//==============================================================================================
void CParabola_Bullet::Update()
{
	CBullet::Update();

	D3DXVECTOR3 Vec = GetMove();

	//�d�͂Œe�𗎂Ƃ�
	Vec.y -= m_fGravity;

	CObjectX *pLauncher = GetObjX();

	if (pLauncher != nullptr)
		// �p�x�̐ݒ�
		pLauncher->SetRot({ -Vec.y + D3DX_PI / 2,m_Rot.y - D3DX_PI,0.0f });

	D3DXVec3Normalize(&Vec, &Vec);

	SetMove(Vec);
}

//==============================================================================================
// �`�扻����
//==============================================================================================
void CParabola_Bullet::Draw()
{
	CBullet::Draw();
}

//=============================================================================
// �j�󏈗�
//=============================================================================
void CParabola_Bullet::Destroy()
{
	// �e�̔j�󏈗�
	CBullet::Destroy();
}

//==============================================================================================
// ��������
//==============================================================================================
CParabola_Bullet *CParabola_Bullet::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 move, float hypotenuse, D3DXVECTOR3 rot, char *filename, const bool side, const int power, const float speed, const int life)
{
	CParabola_Bullet* pParabola_Bullet = nullptr;

	//�N���X�̐���
	pParabola_Bullet = new CParabola_Bullet(PRIORITY_BACK);

	if (pParabola_Bullet != nullptr)
	{
		pParabola_Bullet->SetPos(pos);
		pParabola_Bullet->SetMove(move);
		pParabola_Bullet->m_fHyptenuse = hypotenuse;
		pParabola_Bullet->m_Rot = rot;
		pParabola_Bullet->SetPlayerSide(side);
		pParabola_Bullet->SetPower(power);
		pParabola_Bullet->SetSpeed(speed);
		pParabola_Bullet->SetLife(life);
		pParabola_Bullet->Init();

		// �e�I�u�W�F�N�g�̐���
		pParabola_Bullet->SetObjX(CObjectX::Create(pos, { 0.0f,0.0f,0.0f }, nullptr, filename, PRIORITY_BACK));
	}
	else
		assert(false);

	return pParabola_Bullet;
}