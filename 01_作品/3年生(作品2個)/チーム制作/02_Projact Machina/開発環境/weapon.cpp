//=============================================================================
//
// weapon.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "weapon.h"
#include "application.h"

const char* CWeapon::m_cWeaponFileName[] =
{
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

	"Data/model/SG_01/DummyParent.x",

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
};

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CWeapon::CWeapon(const CObject::PRIORITY priority) : CObjectX(priority)
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CWeapon::~CWeapon()
{
}

//============================================================================
// ����������
//============================================================================
HRESULT CWeapon::Init()
{
	CObjectX::Init();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CWeapon::Uninit()
{
	CObjectX::Uninit();
}

//============================================================================
// �X�V����
//============================================================================
void CWeapon::Update()
{
	CObjectX::Update();
}

//============================================================================
// �`�揈��
//============================================================================
void CWeapon::Draw()
{
	CObjectX::Draw();
}

//============================================================================
// ����̕ύX
//============================================================================
void CWeapon::ChangeWeapon(const int type)
{
	SetModel(CApplication::GetModel()->ReadObject(m_cWeaponFileName[type]));
}

//============================================================================
// ��������
//============================================================================
CWeapon* CWeapon::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, int type, CObjectX* parent)
{
	CWeapon* pParts = new CWeapon;

	if (FAILED(pParts->Init()))
	{
		return nullptr;
	}

	pParts->SetPos(pos);
	pParts->SetRot(rot);
	pParts->ChangeWeapon(type);
	pParts->SetParent(parent);

	return pParts;
}
