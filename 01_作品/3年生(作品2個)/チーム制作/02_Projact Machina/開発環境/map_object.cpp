//==============================================================================================
//
// �}�b�v�̃I�u�W�F�N�g�@�@�@map_object.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"map_object.h"
#include"objectX.h"
#include"application.h"
#include"mode.h"
#include"enemy_manager.h"
#include"player_manager.h"
#include"game.h"
#include"tutorial.h"
#include"map_object_manager.h"
#include"drop_manager.h"
#include "particle_emitter.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CMap_Object::CMap_Object(const PRIORITY priority) : CMove_Object(priority)
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	// �������Ɏ��g�̃|�C���^���}�b�v�I�u�W�F�N�g�}�l�[�W���[�ɐݒ�
	if (Mode == CApplication::MODE_TUTORIAL)
		CTutorial::GetMap_Object_Manager()->SetMapObject(this);
	else if (Mode == CApplication::MODE_GAME)
		CGame::GetMap_Object_Manager()->SetMapObject(this);
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CMap_Object::~CMap_Object()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	// �}�l�[�W���[���g�p���̏ꍇ�A������Ɏ��g�̃|�C���^��z�񂩂珜�O����
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CMap_Object_Manager* pManager = CTutorial::GetMap_Object_Manager();
		if (pManager != nullptr) pManager->DestroyMapObject(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CMap_Object_Manager* pManager = CGame::GetMap_Object_Manager();
		if (pManager != nullptr) pManager->DestroyMapObject(this);
	}
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CMap_Object::Init()
{
	CMove_Object::Init();

	// �����蔻��̐ݒ�
	SetCollision();

	// �����蔻��̃^�C�v
	SetCollision_Type(CMove_Object::COLLISION_TYPE_BLOCK);

	// �^�O�̐ݒ�
	SetTag(TAG_MAP_OBJECT);

	CObjectX *Obj = GetObjectX();
	
	D3DXVECTOR3 Max = Obj->GetMaxSize();
	D3DXVECTOR3 Min = Obj->GetMinSize();

	SetIndex({ Min.x,0.0f,Min.z }, 0);
	SetIndex({ Min.x,0.0f,Max.z }, 1);
	SetIndex({ Max.x,0.0f,Max.z }, 2);
	SetIndex({ Max.x,0.0f,Min.z }, 3);

	SetCenterPos({ 0.0f,Max.y / 2,0.0f });

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CMap_Object::Uninit()
{
	CMove_Object::Uninit();

	GetObjectX()->Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CMap_Object::Update()
{
	CMove_Object::Update();

	//�����蔻��̐ݒ�
	Collision_Hit();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CMap_Object::Draw()
{
}

//==============================================================================================
// �q�b�g����
//==============================================================================================
void CMap_Object::Hit(CMove_Object* /*pHit*/)
{
	Uninit();

	std::move(CParticleEmitter::Create("smoke", GetPos()));

	// �r���|��SE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BREAKBILL);
}

//==============================================================================================
// �����蔻��̐ݒ�
//==============================================================================================
void CMap_Object::Collision_Hit()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CEnemyManager *pEnemy_Manager = nullptr;
	CPlayer *pPlayer = nullptr;

	CDropManager *pDrop_Manager = nullptr;

	// ���[�h���ɃL������ǂݍ���
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pEnemy_Manager = CTutorial::GetEnemyManager();
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
		pDrop_Manager = CTutorial::GetDropManager();
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pEnemy_Manager = CGame::GetEnemyManager();
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
		pDrop_Manager = CGame::GetDropManager();
	}

	D3DXVECTOR3 OtherPos = { 0.0f,0.0f,0.0f };

	// �}�b�v�I�u�W�F�N�g�̃T�C�Y
	D3DXVECTOR3 SizeMax = GetObjectX()->GetMaxSize();
	D3DXVECTOR3 SizeMin = GetObjectX()->GetMinSize();

	// �}�b�v�I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 ObjPos = GetPos();

	// �L��������}�b�v�I�u�W�F�N�g�܂ł̃x�N�g��
	D3DXVECTOR3 ObjVec = { 0.0f,0.0f,0.0f };

	if (pPlayer != nullptr)
	{
		// �v���C���[�̈ʒu�̎擾
		OtherPos = pPlayer->GetPos();
		// ���a�̎擾
		float fPlayer_Radius = pPlayer->GetRadius();

		// �v���C���[����}�b�v�I�u�W�F�N�g�܂ł̃x�N�g��
		ObjVec = ObjPos - OtherPos;

		// �v���C���[����}�b�v�I�u�W�F�N�g�܂ł̋���
		float Distance = sqrtf(ObjVec.x * ObjVec.x + ObjVec.z * ObjVec.z);

		// �v���C���[����߂�
		if (Distance < SizeMax.x * 1.5f + fPlayer_Radius || Distance < SizeMax.z * 1.5f + fPlayer_Radius
			|| Distance < SizeMin.x * 1.5f + fPlayer_Radius || Distance < SizeMin.z * 1.5f + fPlayer_Radius)
			SetCollisionNoneHit(false);
		// �v���C���[���牓��
		else
			SetCollisionNoneHit(true);
	}

	// �G�̏��
	for (auto pEnemy : pEnemy_Manager->GetAllEnemy())
	{
		if (pEnemy->GetLife() > 0 && GetCollisionNoneHit())
		{
			// �G�̈ʒu�̎擾
			OtherPos = pEnemy->GetCenterPos();
			// ���a�̎擾
			float fRadius = pEnemy->GetRadius();

			// �G����}�b�v�I�u�W�F�N�g�܂ł̃x�N�g��
			ObjVec = ObjPos - OtherPos;

			// �G����}�b�v�I�u�W�F�N�g�܂ł̋���
			float Distance = sqrtf(ObjVec.x * ObjVec.x + ObjVec.z * ObjVec.z);

			// �G����߂�
			if (Distance < SizeMax.x * 1.5f + fRadius || Distance < SizeMax.z * 1.5f + fRadius
				|| Distance < SizeMin.x * 1.5f + fRadius || Distance < SizeMin.z * 1.5f + fRadius)
				SetCollisionNoneHit(false);
			// �G���牓��
			else
				SetCollisionNoneHit(true);
		}
	}

	// �h���b�v�p�[�c�̏��
	for (auto pDrop : pDrop_Manager->GetAllDrop())
	{
		if (GetCollisionNoneHit())
		{
			// �ʒu�̎擾
			OtherPos = pDrop->GetPos();
			// ���a�̎擾
			float fRadius = pDrop->GetRadius();

			// �h���b�v�p�[�c����}�b�v�I�u�W�F�N�g�܂ł̃x�N�g��
			ObjVec = ObjPos - OtherPos;

			// �h���b�v�p�[�c����}�b�v�I�u�W�F�N�g�܂ł̋���
			float Distance = sqrtf(ObjVec.x * ObjVec.x + ObjVec.z * ObjVec.z);

			// �h���b�v�p�[�c����߂�
			if (Distance < SizeMax.x * 1.5f + fRadius || Distance < SizeMax.z * 1.5f + fRadius
				|| Distance < SizeMin.x * 1.5f + fRadius || Distance < SizeMin.z * 1.5f + fRadius)
				SetCollisionNoneHit(false);
			// �h���b�v�p�[�c���牓��
			else
				SetCollisionNoneHit(true);
		}
	}
}

//==============================================================================================
// ��������
//==============================================================================================
CMap_Object *CMap_Object::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, const PRIORITY priority)
{
	CMap_Object *pMap_Object = new CMap_Object(priority);

	if (pMap_Object != nullptr)
	{
		pMap_Object->SetObjectX(CObjectX::Create(pos, rot, parent, Xfilename, priority));
		pMap_Object->SetSize(pMap_Object->GetObjectX()->GetMaxSize());
		pMap_Object->SetPos(pos);
		pMap_Object->Init();
	}

	return pMap_Object;
}