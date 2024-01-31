//==============================================================================================
//
// ������ 		smoke_explosion.cpp
// tutida ryousei
//
//==============================================================================================
#include"smoke_explosion.h"
#include "particle_emitter.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CSmoke_Explosion::CSmoke_Explosion(const PRIORITY priority) : CExplosion(priority)
{
	m_nContinuation = 0;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CSmoke_Explosion::~CSmoke_Explosion()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CSmoke_Explosion::Init()
{
	CExplosion::Init();

	// �����蔻��̃q�b�g������ǂݍ��܂Ȃ�����̐ݒ�
	SetCollisionNoneHit(true);

	// �����蔻��̐���
	SetDelayCollision();

	// �U���p�[�e�B�N��
	std::move(CParticleEmitter::Create("Attack", { GetPos().x,GetPos().y + 100,GetPos().z }));

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CSmoke_Explosion::Uninit()
{
	CMove_Object::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CSmoke_Explosion::Update()
{
	CMove_Object::Update();

	// �����̎�������
	if (m_nContinuation < Explosion_Timer)
	{
		m_nContinuation++;
	}
	else
	{
		Uninit();
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CSmoke_Explosion::Draw()
{
}

//==============================================================================================
// ��������
//==============================================================================================
CSmoke_Explosion *CSmoke_Explosion::Create(D3DXVECTOR3 pos, float radius, int power, const bool side, const CObject::PRIORITY priority)
{
	CSmoke_Explosion *pExplosion = new CSmoke_Explosion(priority);

	if (pExplosion != nullptr)
	{
		pExplosion->SetPos(pos);
		pExplosion->SetRadius(radius);
		pExplosion->SetPlayerSide(side);
		pExplosion->SetPower(power);
		pExplosion->Init();
	}

	return pExplosion;
}