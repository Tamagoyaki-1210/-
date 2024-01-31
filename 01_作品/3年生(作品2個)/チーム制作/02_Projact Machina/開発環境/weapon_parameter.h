//=============================================================================
//
// weapon_parameter.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _WEAPON_PARAMETER_H_
#define _WEAPON_PARAMETER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "d3dx9.h"
#include "weapon.h"

//---------------------------
// �N���X�錾
//---------------------------
class CWeapon_Parameter
{

public:
	static const int WEAPON_RARITY = 3;				// �p�[�c�̃��A���e�B

	// �ߐڕ���̑���
	enum MELEE_WEAPON
	{
		MELEE_WEAPON_NONE = 0,		// �f��	(1���)
		MELEE_WEAPON_FIST = 1,		// ��	(4���)
		MELEE_WEAPON_SLASH = 5,		// �a��	(4���)
		MELEE_WEAPON_POKE = 9,		// �h��	(4���)
		MELEE_WEAPON_MAX = 13,		// �ߐڕ���̍ő�l
	};

	// �e����̑���
	enum GUN_WEAPON
	{
		GUN_WEAPON_AR = 0,			// �A�T���g���C�t��		(5���)
		GUN_WEAPON_SMG = 5,			// �T�u�}�V���K��		(4���)
		GUN_WEAPON_HG = 9,			// �n���h�K��			(3���)
		GUN_WEAPON_MG = 12,			// �}�V���K��			(2���)
		GUN_WEAPON_SG = 14,			// �V���b�g�K��			(2���)
		GUN_WEAPON_SR = 16,			// �X�i�C�p�[���C�t��	(2���)
		GUN_WEAPON_MAX = 18,		// �e����̍ő�l
	};

	// �ߐڕ���p�����[�^�[
	struct MELEE_WEAPON_PARAMETERS
	{
		int nPower;					// �З�
		int nAttack_Rate;			// �U�����x
		int nGravity;				// �d��
		int nLength;				// �˒�
	};

	// �e����p�����[�^�[
	struct GUN_WEAPON_PARAMETERS
	{
		int nPower;					// �З�
		float fFiring_Speed;		// ���ˑ��x
		int nGravity;				// �d��
		int nBullet_Speed;			// �e��
		int nLife;					// �̗�
	};


	CWeapon_Parameter();
	~CWeapon_Parameter();

	HRESULT Init();
	void Uninit();

	void LoadMeleeWeaponFile();	// �ߐڕ���̓ǂݍ���
	void LoadGunWeaponFile();	// �e����̓ǂݍ���

	// �ߐڕ���p�����[�^�[�̎擾
	MELEE_WEAPON_PARAMETERS GetParameterMeleeWeapon(const int type, const int rearity) { return m_Melee_Weapon[type][rearity]; }
	
	// �ߐڕ���p�����[�^�[�̎擾
	GUN_WEAPON_PARAMETERS GetParameterGunWeapon(const int type, const int rearity) { return m_Gun_Weapon[type][rearity]; }

private:

	MELEE_WEAPON_PARAMETERS m_Melee_Weapon[MELEE_WEAPON_MAX][WEAPON_RARITY];	// �ߐڕ���̃p�����[�^�[

	GUN_WEAPON_PARAMETERS m_Gun_Weapon[GUN_WEAPON_MAX][WEAPON_RARITY];			// �e����̃p�����[�^�[
};

#endif// _WEAPON_PARAMETER_H_