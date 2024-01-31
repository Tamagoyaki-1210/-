//=============================================================================
//
// drop_weapon.p
// Author : tutida ryousei
//
//=============================================================================
#include"drop_weapon.h"
#include"drop_manager.h"
#include"objectX.h"
#include"meshfield.h"
#include"tutorial.h"
#include"game.h"
#include"input.h"
#include"object3D.h"
#include"debugProc.h"
#include"application.h"
#include"player_manager.h"
#include"model.h"


const float CDrop_Weapon::PARTS_COLLISION_RADIUS = 300.0f;	// �����Ă镐��̓����蔻��̑傫��
const float CDrop_Weapon::PARTS_FLOTIONG_POS = 100.0f;		// �����Ă镐��̕��V�ʒu

//=============================================================================
// ��`
//=============================================================================
const char* CDrop_Weapon::s_Weapon_FileName[] =
{
	// �r
	"Data/model/DropParts/SG_01/SG_01_DropArm.x",
	"Data/model/DropParts/SG_03/SG_03_DropArm.x",
	"Data/model/DropParts/SG_02/SG_02_DropArm.x",
	"Data/model/DropParts/SG_05/SG_05_DropArm.x",
	nullptr,

	// �r
	"Data/model/DropParts/SG_01/SG_01_DropLeg.x",
	"Data/model/DropParts/SG_03/SG_03_DropLeg.x",
	"Data/model/DropParts/SG_02/SG_02_DropLeg.x",
	"Data/model/DropParts/SG_05/SG_05_DropLeg.x",
	nullptr,

	"Data/model/Weapon/Gun_Weapon/AR40.x",				// AR40
	"Data/model/Weapon/Gun_Weapon/Tec-A3.x",			// Tec-A3
	"Data/model/Weapon/Gun_Weapon/DumpMan.x",			// DumpMan
	"Data/model/Weapon/Gun_Weapon/StarVerseAR.x",		// StarVerseAR
	"Data/model/Weapon/Gun_Weapon/MM400�W���N�X.x",		// MM400�W���N�X

	"Data/model/Weapon/Gun_Weapon/MPC50.x",				// MPC50
	"Data/model/Weapon/Gun_Weapon/PDW-920.x",			// PDW-920
	"Data/model/Weapon/Gun_Weapon/�p�[��SMG.x",			// �p�[��SMG
	"Data/model/Weapon/Gun_Weapon/SilverComet.x",		// SilverComet

	"Data/model/Weapon/Gun_Weapon/HG37.x",				// HG37
	"Data/model/Weapon/Gun_Weapon/Revolver.x",			// Revolver
	"Data/model/Weapon/Gun_Weapon/�A�L���{2.0.x",		// �A�L���{2.0

	"Data/model/Weapon/Gun_Weapon/LA2000.x",			// LA2000
	"Data/model/Weapon/Gun_Weapon/���^���J255.x",		// ���^���J255

	"Data/model/Weapon/Gun_Weapon/12Pump.x",			// 12Pump
	"Data/model/Weapon/Gun_Weapon/WR2154.x",			// WR2154

	"Data/model/Weapon/Gun_Weapon/Winter5000.x",		// Winter5000
	"Data/model/Weapon/Gun_Weapon/�A�e�i.x",			// �A�e�i
	nullptr,

	// ����
	"Data/model/SG_01/DummyParent.x",	// �f��

	"Data/model/Weapon/Melee_Weapon/knuckle.x",			// �i�b�N��
	"Data/model/Weapon/Melee_Weapon/pilebunker.x",		// �p�C���o���J�[
	"Data/model/Weapon/Melee_Weapon/brandmarkers.x",	// �u�����h�}�[�J�[
	"Data/model/Weapon/Melee_Weapon/stunbaton.x",		// �X�^���o�g��

	"Data/model/Weapon/Melee_Weapon/samuraiSword.x",	// ����
	"Data/model/Weapon/Melee_Weapon/hammer.x",			// �n���}�[
	"Data/model/Weapon/Melee_Weapon/beamSword.x",		// �r�[���T�[�x��
	"Data/model/Weapon/Melee_Weapon/basterSword.x",		// �o�X�^�[�\�[�h

	"Data/model/Weapon/Melee_Weapon/lance.x",			// �����X
	"Data/model/Weapon/Melee_Weapon/rapier.x",			// ���C�s�A
	"Data/model/Weapon/Melee_Weapon/partisan.x",		// �p���`�U��
	"Data/model/Weapon/Melee_Weapon/beamjavelin.x",		// �r�[���W���x����
	nullptr,
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDrop_Weapon::CDrop_Weapon(const PRIORITY priority) : CMove_Object(priority)
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	// �������Ɏ��g�̃|�C���^��G�L�����}�l�[�W���[�ɐݒ�
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CTutorial::GetDropManager()->SetDrop(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CGame::GetDropManager()->SetDrop(this);
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDrop_Weapon::~CDrop_Weapon()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	// �}�l�[�W���[���g�p���̏ꍇ�A������Ɏ��g�̃|�C���^��z�񂩂珜�O����
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CDropManager* pManager = CTutorial::GetDropManager();
		if (pManager != nullptr) pManager->DestroyDrop(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CDropManager* pManager = CGame::GetDropManager();
		if (pManager != nullptr) pManager->DestroyDrop(this);
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDrop_Weapon::Init()
{
	SetDelayCollision();

	SetRadius(PARTS_COLLISION_RADIUS);

	m_CenterPos = { 0.0f,PARTS_FLOTIONG_POS,0.0f };

	CObjectX* pObjectX = GetObjectX();

	// �p�x�̐ݒ�
	pObjectX->SetRot({ 0.4f,0.0f,0.0f });

	// �����Ă镐��̐���
	//pObjectX->SetSize({ 3.0f,3.0f,3.0f });

	// �p�[�c�̕��ʂ̐ݒ�
	Parts_Type();

   	m_bPick_Up = false;
	m_fMove = 0.0f;

	D3DXVECTOR3 ModelPos = GetPos();

	// �|�C���^�[UI�̐���
	m_pPointer = CObject3D::Create(GetPos(), { 200.0f,200.0f }, PRIORITY_CENTER, { 1.0f,1.0f,1.0f,1.0f }, true);
	m_pPointer->SetTexture(CTexture::TEXTURE_ITEM_POINTER);

	// �s�b�N�A�b�vUI�̐���
	m_pPick_Up = CObject3D::Create({ ModelPos.x, ModelPos.y + 120.0f, ModelPos.z }, { 70.0f,70.0f }, PRIORITY_CENTER, { 1.0f,1.0f,1.0f,1.0f }, true);
	m_pPick_Up->SetTexture(CTexture::TEXTURE_KEY_E);
	m_pPick_Up->SetDrawFlag(false);

	const float nAddCol = -0.05f * m_nRarity;

	pObjectX->AddColor({ nAddCol, nAddCol,nAddCol, 0.0f});


	CMove_Object::Init();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CDrop_Weapon::Uninit()
{
	if (m_pPointer != nullptr)
	{
		m_pPointer->Uninit();
		m_pPointer = nullptr;
	}

	if (m_pPick_Up != nullptr)
	{
		m_pPick_Up->Uninit();
		m_pPick_Up = nullptr;
	}

	if (GetObjectX() != nullptr)
		GetObjectX()->Uninit();

	//if (GetLandObj())
	//{
	//	for (int nCnt = 0; nCnt < GetOnObj(0)->GetOnObjCnt(); nCnt++)
	//	{
	//		GetOnObj(0)->SetOnObj(nullptr, nCnt);
	//	}
	//	GetOnObj(0)->SetOnObjCnt(0);
	//}

	CMove_Object::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CDrop_Weapon::Update()
{
	CMove_Object::Update();

	// ���Ƃ̓����蔻��
	FieldCollision();

	// �p�x�̉��Z
	GetObjectX()->AddRot({ 0.0f,0.03f,0.0f });

	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CPlayer* pPlayer = nullptr;

	// ���[�h���Ƀv���C���[��ǂݍ���
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
	}

	D3DXVECTOR3 Player_Pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 Mob_Pos = { 0.0f,0.0f,0.0f };

	if (pPlayer != nullptr)
	{
		// �ʒu�̎擾
		Player_Pos = pPlayer->GetPos();
		Mob_Pos = GetPos();
	}

	// �v���C���[���痎���Ă�p�[�c�̋���
	D3DXVECTOR3 Vec = Player_Pos - Mob_Pos;

	// �����̎Z�o
	float fDistance = sqrtf(Vec.x * Vec.x + Vec.z * Vec.z);

	// ����7000�ȉ��̏ꍇ�ɕ\������
	const bool bDisplay = fDistance <= DRAW_DROP_DISTANCE;

	GetObjectX()->SetDrawFlag(bDisplay);
	m_pPointer->SetDrawFlag(bDisplay);
	ItemPointerMove();
	// ������E��
	Pick_Up_Weapon();

	// �s�b�N�A�b�v��Ԃ����Z�b�g����
	m_bPick_Up = false;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDrop_Weapon::Draw()
{
	//CMove_Object::Draw();
}

//=============================================================================
// �A�C�e���|�C���^�[�̈ړ�����
//=============================================================================
void CDrop_Weapon::ItemPointerMove()
{
	m_pPointer->SetPos(GetPos());
}

//=============================================================================
// ������E��
//=============================================================================
void CDrop_Weapon::Pick_Up_Weapon()
{
	// �s�b�N�A�b�vUI�����݂���ꍇ
	if(m_pPick_Up != nullptr)
	{
		// �`�攻��
		m_pPick_Up->SetDrawFlag(m_bPick_Up);

		// �s�b�N�A�b�v����Ă���ꍇ
		if (m_bPick_Up)
		{
			D3DXVECTOR3 ModelPos = GetPos();

			// �F�̐ݒ�
			m_pPick_Up->SetCol({ 1.0f,1.0f,1.0f,1.0f });

			m_pPick_Up->SetPos({ ModelPos.x, ModelPos.y + 120.0f, ModelPos.z });
		}
		else
		{
			// �F�̐ݒ�
			m_pPick_Up->SetCol({ 1.0f,1.0f,1.0f,0.0f });
		}
	}
}

//=============================================================================
// �p�[�c�̕��ʂ̐ݒ�
//=============================================================================
void CDrop_Weapon::Parts_Type()
{
	if (ARMS_MAX > m_nWeapon_Type)
	m_Parts = CPlayer::PARTS_ARMS;
	else if(LEG_MAX > m_nWeapon_Type)
	m_Parts = CPlayer::PARTS_LEG;
	else if (MELEE_WEAPON_MAX > m_nWeapon_Type)
	m_Parts = CPlayer::PARTS_WEAPON;
}

//=============================================================================
// ���Ƃ̓����蔻��
//=============================================================================
void CDrop_Weapon::FieldCollision()
{
	CMeshField *pMesh = CGame::GetMeshField();

	if (pMesh != nullptr)
	{
		const D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 Pos = {};
		SetPosOld(pos);

		// ���b�V���t�B�[���h�Ƃ̓����蔻��
		float MeshY = pMesh->MeshCollision(pos);

		// ���b�V���t�B�[���h�ɏ���Ă�
		if (pMesh->GetHit())
		{
			m_fMove = 0.0f;
			// ���g�̈ʒu
			Pos = { pos.x, MeshY, pos.z };
		}
		else if (GetLandObj())
			Pos = { pos.x, GetObjY(), pos.z };
		else
		{
			// ���f���̈ʒu
			D3DXVECTOR3 Weapon_Pos = pos;
			// �������x
			m_fMove += 0.5f;
			// Y���W�̈ړ�
			Weapon_Pos.y -= m_fMove;

			// �����蔻��̈ʒu
			Pos = { Weapon_Pos.x, Weapon_Pos.y, Weapon_Pos.z };
		}

		SetPos(Pos);
		GetObjectX()->SetPos(Pos);
	}
}

void CDrop_Weapon::Hit(CMove_Object* /*pHit*/)
{
}

//=============================================================================
// ��������
//=============================================================================
CDrop_Weapon *CDrop_Weapon::Create(D3DXVECTOR3 pos, int weapon, int rarity)
{
	CDrop_Weapon *pDrop_Weapon = nullptr;

	pDrop_Weapon = new CDrop_Weapon(PRIORITY_MODEL);

	if (pDrop_Weapon != nullptr)
	{
		pDrop_Weapon->SetPos(pos);
		pDrop_Weapon->SetObjectX(CObjectX::Create(pos, { 0.0f,0.0f,0.0f }, nullptr, CApplication::GetModel()->ReadObject(s_Weapon_FileName[pDrop_Weapon->m_nWeapon_Type = weapon])));
		pDrop_Weapon->m_nRarity = rarity;
		pDrop_Weapon->Init();
	}

	return pDrop_Weapon;
}

//==============================================================================================
// �S�Ẵh���b�v�p�[�c�̓ǂݍ���
//==============================================================================================
void CDrop_Weapon::LoadAllFile()
{
	for (int nCnt = 0; nCnt < DROP_PARTS_MAX; nCnt++)
	{
		// ���O�����݂���ꍇ
		if (s_Weapon_FileName[nCnt] != nullptr)
		{
			CApplication::GetModel()->ReadObject(s_Weapon_FileName[nCnt]);
		}
	}
}