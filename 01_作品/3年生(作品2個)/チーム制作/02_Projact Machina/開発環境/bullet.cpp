//=============================================================================
//
// �e.cpp (Bullet.cpp)
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "bullet.h"
#include "game.h"
#include "meshfield.h"
#include "object3D.h"
#include "objectX.h"
#include "particle_emitter.h"
#include "tutorial.h"
#include "normal_explosion.h"
#include "map_object_manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
const float CBullet::BULLET_SPEED_XZ = 50.0f;				// �e�̑��x
const float CBullet::BULLET_SPEED_Y = 50.0f;				// �e�̑��x
const float CBullet::BULLET_COLLISION_RADIUS = 30.0f;	// �e�̓����蔻��̑傫��
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBullet::CBullet(const CObject::PRIORITY priority) : CMove_Object(priority)
{
	m_fSpeed_XZ = BULLET_SPEED_XZ;
	m_fSpeed_Y = BULLET_SPEED_Y;
	SetRadius(BULLET_COLLISION_RADIUS);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init()
{
	// �^�O�̐ݒ�
	SetTag(TAG_BULLET);

	// �����蔻��̃^�C�v
	SetCollision_Type(CMove_Object::COLLISION_TYPE_SHERER);

	// �T�C�Y�̐ݒ�
	CMove_Object::SetSize({ GetRadius(),GetRadius(),GetRadius() });

	// �I�u�W�F�N�g3D�̏���������
	CMove_Object::Init();

	//==================================================
	// �����o�ϐ��̏�����
	//==================================================
	// �����蔻��̐���
	SetCollision();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit()
{
	// 3D�I�u�W�F�N�g���g�p����Ă���ꍇ
	if (m_pObj3D != nullptr)
	{
		m_pObj3D->Uninit();
		m_pObj3D = nullptr;
	}
	if (m_pObjX != nullptr)
	{
		m_pObjX->Uninit();
		m_pObjX = nullptr;
	}

	// �I�u�W�F�N�g3D�̏I������
	CMove_Object::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update()
{
	CMove_Object::Update();

	// �擾
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR2 size = GetSize();

	//�O��̈ʒu��ۑ�
	m_nPosOld = pos;

	SetPosOld(m_nPosOld);

	//�e�̈ʒu�X�V
	pos.x += move.x * m_fSpeed_XZ;
	pos.z += move.z * m_fSpeed_XZ;
	pos.y += move.y * m_fSpeed_Y;

	// �U���p�[�e�B�N��
	std::move(CParticleEmitter::Create("Locus", pos));


	// �ʒu�̐ݒ�
	SetPos(pos);

	// �I�u�W�F�N�g3D�̈ʒu��ݒ�
	if (m_pObj3D != nullptr)
	{
		m_pObj3D->SetPos(pos);
		m_pObj3D->SetSize(size);
	}
	if (m_pObjX != nullptr)
	{
		m_pObjX->SetPos(pos);
	}

	// ���������炷
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}

	// ���̓����蔻��
	FieldCollision();

	// �}�b�v�I�u�W�F�N�g�̓����蔻��
	Map_Object_Collision();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw()
{

}

//============================================================================
// ���̓����蔻��
//============================================================================
void CBullet::FieldCollision()
{
	// ���݂̈ʒu��萔�Ƃ��Ď擾
	const D3DXVECTOR3 pos = GetPos();

	CMeshField* pMeshField = nullptr;

	if(CApplication::GetModeType() == CApplication::MODE_GAME)
		pMeshField = CGame::GetMeshField();
	else if(CApplication::GetModeType() == CApplication::MODE_TUTORIAL)
		pMeshField = CTutorial::GetMeshField();

	if (pMeshField != nullptr)
	{
		// ���̓����蔻�肩�獂����萔�Ƃ��Ď擾
		const float a = pMeshField->MeshCollision(pos);

		// ���b�V���t�B�[���h��艺�̈ʒu�ɂ���ꍇ
		if (a >= pos.y)
		{
			//pMeshField->Ground_Broken(pos, 50.0f, 5);

			// �e��j�󂷂�
			Destroy();
		}
	}
}

//=============================================================================
// ��e����
//=============================================================================
void CBullet::Hit(CMove_Object* pHit)
{
	// �e�ł͖����ꍇ && �����T�C�h�ł͂Ȃ��ꍇ
	if ((pHit->GetTag() == TAG_CHARACTER && GetPlayerSide() != pHit->GetPlayerSide())
		|| pHit->GetTag() == TAG_MAP_OBJECT)
	{
		Destroy();
	}
}

//=============================================================================
// �j�󏈗�
//=============================================================================
void CBullet::Destroy()
{
	if (m_bExplosion)
	{
		// ���e���̔���
		CNormal_Explosion::Create(GetPos(), 500, 70, GetPlayerSide(), CObject::PRIORITY_BACK);
	}

	// �U���p�[�e�B�N��
	std::move(CParticleEmitter::Create("Attack", GetPos()));

	Uninit();
}

//=============================================================================
// �}�b�v�I�u�W�F�N�g�Ƃ̓����蔻��
//=============================================================================
void CBullet::Map_Object_Collision()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CMap_Object_Manager *pMap_Object_Manager = nullptr;

	// ���[�h���ɃL������ǂݍ���
	if (Mode == CApplication::MODE_TUTORIAL)
		pMap_Object_Manager = CTutorial::GetMap_Object_Manager();
	else if (Mode == CApplication::MODE_GAME)
		pMap_Object_Manager = CGame::GetMap_Object_Manager();

	D3DXVECTOR3 MapObjec_Pos = {};
	D3DXVECTOR3 ObjVec = {};

	// �}�b�v�I�u�W�F�N�g�̏��
	for (auto pMap_Object : pMap_Object_Manager->GetAllMapObject())
	{
		if (pMap_Object->GetCollisionNoneHit())
		{
			// �e�̈ʒu�̎擾
			MapObjec_Pos = pMap_Object->GetCenterPos();

			D3DXVECTOR3 Max = pMap_Object->GetObjectX()->GetMaxSize();
			D3DXVECTOR3 Min = pMap_Object->GetObjectX()->GetMinSize();

			// �e����}�b�v�I�u�W�F�N�g�܂ł̃x�N�g��
			ObjVec = GetPos() - MapObjec_Pos;

			// �e����}�b�v�I�u�W�F�N�g�܂ł̋���
			float Distance = sqrtf(ObjVec.x * ObjVec.x + ObjVec.z * ObjVec.z);

			// �e����߂�
			if (Distance < Max.x * 1.5f + GetRadius() || Distance < Max.z * 1.5f + GetRadius()
				|| Distance < Min.x * 1.5f + GetRadius() || Distance < Min.z * 1.5f + GetRadius())
				pMap_Object->SetCollisionNoneHit(false);
			// �e���牓��
			else
				pMap_Object->SetCollisionNoneHit(true);
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CBullet* CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, D3DXVECTOR3 move, const bool side, const int power, const float speed, const int life)
{
	//�N���X�̐���
	CBullet* pBullet = new CBullet(PRIORITY_BACK);

	//null�`�F�b�N
	if (pBullet != nullptr)
	{
		//����������
		pBullet->Init();
		//�ݒ菈��
		pBullet->SetPos(pos);
		pBullet->SetMove(move);
		pBullet->SetSize(size);
		pBullet->SetPlayerSide(side);
		pBullet->SetPower(power);
		pBullet->SetSpeed(speed);
		pBullet->SetLife(life);

		// �e�I�u�W�F�N�g�̐���
		pBullet->m_pObj3D = CObject3D::Create(pos, size, PRIORITY_BACK);
		// �e�̃e�N�X�`��
		pBullet->m_pObj3D->SetTexture(CTexture::TEXTURE_BULLET);

		// �r���{�[�h�̐ݒ�
		pBullet->m_pObj3D->Setbillboard(true);
	}
	else
	{
		assert(false);
	}

	return pBullet;
}