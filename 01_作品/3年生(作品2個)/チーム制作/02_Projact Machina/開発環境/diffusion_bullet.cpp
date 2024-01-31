//==============================================================================================
//
// �g�U�e�@�@�@diffusion_bullet.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"diffusion_bullet.h"
#include"object3D.h"
#include"application.h"
#include "utility.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CDiffusion_Bullet::CDiffusion_Bullet(const PRIORITY priority) : CBullet(priority)
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CDiffusion_Bullet::~CDiffusion_Bullet()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CDiffusion_Bullet::Init()
{
	CBullet::Init();

	//SetSpeed(60.0f);
	//SetPower(8);

	D3DXVECTOR3 Move = GetMove();

	// �����_���Ȋp�x
	float Rand_Angle = 0.2f;

	// �e�̏o��p�x
	Move.x += utility::Random<float>(Rand_Angle, -Rand_Angle);
	Move.y += utility::Random<float>(Rand_Angle, -Rand_Angle);
	Move.z += utility::Random<float>(Rand_Angle, -Rand_Angle);

	SetMove(Move);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CDiffusion_Bullet::Uninit()
{
	CBullet::Uninit();
}

//==============================================================================================
// �X�V������
//==============================================================================================
void CDiffusion_Bullet::Update()
{
	CBullet::Update();
}

//==============================================================================================
// �`�扻����
//==============================================================================================
void CDiffusion_Bullet::Draw()
{
	CBullet::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CDiffusion_Bullet *CDiffusion_Bullet::Create(const D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXVECTOR3 move, int number, const bool side, const int power, const float speed, const int life)
{
	CDiffusion_Bullet* pDiffusion_Bullet = nullptr;

	for (int nCnt = 0; nCnt < number; nCnt++)
	{
		//�N���X�̐���
		pDiffusion_Bullet = new CDiffusion_Bullet(PRIORITY_BACK);

		if (pDiffusion_Bullet != nullptr)
		{
			pDiffusion_Bullet->SetPos(pos);
			pDiffusion_Bullet->SetSize(size);
			pDiffusion_Bullet->SetMove(move);
			pDiffusion_Bullet->SetPlayerSide(side);
			pDiffusion_Bullet->SetPower(power);
			pDiffusion_Bullet->SetSpeed(speed);
			pDiffusion_Bullet->SetLife(life);
			pDiffusion_Bullet->Init();

			// �e�I�u�W�F�N�g�̐���
			pDiffusion_Bullet->SetObj3D(CObject3D::Create(pos, size, PRIORITY_BACK));
			// �e�̃e�N�X�`��
			pDiffusion_Bullet->GetObj3D()->SetTexture(CTexture::TEXTURE_BULLET);

			// �r���{�[�h�̐ݒ�
			pDiffusion_Bullet->GetObj3D()->Setbillboard(true);
		}
		else
			assert(false);	
	}
	
	return pDiffusion_Bullet;
}