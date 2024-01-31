//=============================================================================
//
// drop_weapon.h
// Author : tutida ryousei
//			tanimoto kosuke
//
//=============================================================================
#ifndef _DORP_WAPON_H_
#define _DORP_WAPON_H_

#include "move_object.h"
#include "player.h"

class CObject3D;

class CDrop_Weapon : public CMove_Object
{
	static const float PARTS_FLOTIONG_POS;			// �����Ă镐��̕��V�ʒu
public:
	
	static const float PARTS_COLLISION_RADIUS;		// �����蔻��̑傫��

	CDrop_Weapon(const PRIORITY priority = PRIORITY_MODEL);
	~CDrop_Weapon();

	// �p�[�c�̎��
	enum Drop_Parts_FileName
	{
		// [1]�r
		ARMS_SG01 = 0,			// SG01�̘r
		ARMS_SG02,				// SG02�̘r
		ARMS_SG03,				// SG03�̘r
		ARMS_SG04,				// SG04�̘r
		ARMS_MAX,				// �r�̍ő吔

		// [2]�r
		LEG_SG01,				// SG01�̋r
		LEG_SG02,				// SG02�̋r
		LEG_SG03,				// SG03�̋r
		LEG_SG04,				// SG04�̋r
		LEG_MAX,				// �r�̍ő吔

		GUN_WEAPON_AR_AR40,					// AR40
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

		GUN_WEAPON_MAX,						// �e����̍ő吔

		// ����
		MELEE_WEAPON_NONE,					// �f��

		MELEE_WEAPON_FIST_KNUCKLE,			// �i�b�N��
		MELEE_WEAPON_FIST_BUNKER,			// �p�C���o���J�[
		MELEE_WEAPON_FIST_BRAND_MARKERS,	// �u�����h�}�[�J�[
		MELEE_WEAPON_FIST_STUN_BATON,		// �X�^���o�g��

		MELEE_WEAPON_SLASH_SAMURAI_SWORD,	// ����
		MELEE_WEAPON_SLASH_HAMMER,			// �n���}�[
		MELEE_WEAPON_SLASH_BEAM_SABER,		// �r�[���T�[�x��
		MELEE_WEAPON_SLASH_BASTARD_SWORD,	// �o�X�^�[�\�[�h

		MELEE_WEAPON_STABBING_LANCE,		// �����X
		MELEE_WEAPON_STABBING_RAPIER,		// ���C�s�A
		MELEE_WEAPON_STABBING_PARTISAN,		// �p���`�U��
		MELEE_WEAPON_STABBING_BEAM_JAVELIN,	// �r�[���W���x����

		MELEE_WEAPON_MAX,					// �ߐڕ���̍ő吔

		DROP_PARTS_MAX,
		DROP_PARTS_NONE
	};

	static const char* s_Weapon_FileName[];	// �t�@�C���p�X

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Hit(CMove_Object* pHit) override;

	void ItemPointerMove();// �A�C�e���|�C���^�[�̈ړ�����
	void Pick_Up_Weapon();		// ������E��
	void Parts_Type();			// �p�[�c�̕��ʂ̐ݒ�
	void FieldCollision();		// ���Ƃ̓����蔻��

	void SetPick_Up(const bool pick_Up) { m_bPick_Up = pick_Up; }

	const int GetRarity() { return m_nRarity; }
	const D3DXVECTOR3 GetCentePos() { return m_CenterPos; }
	const int GetWeaponType() { return m_nWeapon_Type; }
	CPlayer::PARTS GetPartsType() { return m_Parts; }

	static CDrop_Weapon *Create(D3DXVECTOR3 pos, int weapon, int rarity);


	static void LoadAllFile();			// �S�Ẵh���b�v�p�[�c�̓ǂݍ���

private:

	D3DXVECTOR3 m_CenterPos;	// ���S�ʒu
	int m_nWeapon_Type;			// ����̃^�C�v
	int m_nRarity;				// ���A���e�B
	CObject3D *m_pPointer;		// �A�C�e���|�C���^�[�̕\��
	CObject3D *m_pPick_Up;		// �E�����̕\��
	float m_fMove;				// �������x
	bool m_bPick_Up;			// �E����͈͂ɂ��邩
	CPlayer::PARTS m_Parts;		// �p�[�c���
};

#endif // !_DORP_WAPON_H_
