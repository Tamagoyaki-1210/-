//=============================================================================
//
// weapon.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "objectX.h"

//---------------------------
// �N���X�錾
//---------------------------
class CWeapon : public CObjectX
{
public:

	enum WEAPON_TYPE
	{
		GUN_WEAPON_AR_AR40 = 0,				// AR40
		GUN_WEAPON_AR_TEC_A3,				// Tec-A3
		GUN_WEAPON_AR_DUMPMAN,				// DumpMan
		GUN_WEAPON_AR_STARVERSE_AR,			// StarVerseAR
		GUN_WEAPON_AR_MM400JINX,			// MM400�W���N�X

		GUN_WEAPON_SMG_MPC50,				// MPC50
		GUN_WEAPON_SMG_PDW_920,				// PDW-920
		GUN_WEAPON_SMG_PEARL_SMG,			// �p�[��SMG
		GUN_WEAPON_SMG_SILVERCOMET,			// SilverComet

		GUN_WEAPON_HG_HG37,					// HG37
		GUN_WEAPON_HG_REVOLVER,				// Revolver
		GUN_WEAPON_HG_AKIMBO20,				// �A�L���{2.0

		GUN_WEAPON_MG_LA2000,				// LA2000
		GUN_WEAPON_MG_METALLICA255,			// ���^���J255

		GUN_WEAPON_SG_12PUMP,				// 12Pump
		GUN_WEAPON_SG_WR2154,				// WR2154

		GUN_WEAPON_SR_WINTER5000,			// Winter5000
		GUN_WEAPON_SR_ATHENA,				// �A�e�i

		WEAPON_KNUCKLE,						// �f��

		MELEE_WEAPON_FIST_KNUCKLE,			// �i�b�N��
		MELEE_WEAPON_FIST_BUNKER,			// �p�C���o���J�[
		MELEE_WEAPON_FIST_BRAND_MARKERS,	// �u�����h�}�[�J�[
		MELEE_WEAPON_FIST_STUN_BATON,		// �X�^���o�g��

		MELEE_WEAPON_SLASH_SAMURAI_SWORD,	// ����
		MELEE_WEAPON_SLASH_HAMMER,			// �n���}�[
		MELEE_WEAPON_SLASH_BEAM_SABER,		// �r�[���T�[�x��
		MELEE_WEAPON_SLASH_BASTARD_SWORD,	// �o�X�^�[�\�[�h

		MELEE_WEAPON_POKE_LANCE,			// �����X
		MELEE_WEAPON_POKE_RAPIER,			// ���C�s�A
		MELEE_WEAPON_POKE_PARTISAN,			// �p���`�U��
		MELEE_WEAPON_POKE_BEAM_JAVELIN,		// �r�[���W���x����

		WEAPON_MAX							// �ő�l
	};


	CWeapon(const CObject::PRIORITY priority = CObject::PRIORITY_MODEL);
	virtual ~CWeapon() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ChangeWeapon(const int type);

	static CWeapon* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type, CObjectX* parent = nullptr);

private:
	static const char* m_cWeaponFileName[WEAPON_MAX];		// ����̃t�@�C����
};

#endif// _WEAPON_H_