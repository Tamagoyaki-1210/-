//=============================================================================
//
// player.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "character.h"

class CBullet;
class CEnergy_Gauge;
class CObject3D;
class CDrop_Weapon;
class CWeapon;
class CEnemy;

//---------------------------
// �N���X�錾
//---------------------------
class CPlayer : public CCharacter
{

	static const int FIRST_MAX_LIFE = 100;			// �����ő�̗͂̒l
	static const float PLAYER_COLLISION_RADIUS;		// �v���C���[�̓����蔻��̑傫��
	static const float PLAYER_JUMP_POWER;			// �v���C���[�̃W�����v��
	static const float VIEW_SCOPE_ANGLE;			// ��ʂɉf�邬�肬��̈ʒu
	static const float RETICLE_TRANSPARENCY_SIZE;	// ���e�B�N���������̃T�C�Y
	static const float RETICLE_SIZE;				// ���e�B�N���̃T�C�Y
	static const float BULLET_SPEED_SCALE;			// �e���̔{��

public:

	// �p�[�c�̎��
	enum PARTS
	{
		PARTS_BODY = 0,			// ����
		PARTS_ARMS,				// �r
		PARTS_LEG,				// �r

		PARTS_MAX,
		PARTS_WEAPON,			// ����
	};

	// ���[�V�����̃^�C�v
	enum MOTION_TYPE
	{
		MOTION_NEUTRAL = 0,			// �j���[�g����
		MOTION_WALK,				// ����
		MOTION_JUMP,				// �W�����v
		MOTION_LANDING,				// ���n
		MOTION_BOOST_RUN,			// �u�[�X�g�_�b�V��

		MOTION_ASSAULT_RIFLE,		// �A�T���g���C�t���ˌ�
		MOTION_SUB_MACHIN_GUN,		// �T�u�}�V���K���ˌ�
		MOTION_HUND_GUN,			// �n���h�K��(�񒚌��e)
		MOTION_MACHIN_GUN,			// �}�V���K���ˌ�
		MOTION_SHOT_GUN,			// �V���b�g�K���ˌ�
		MOTION_SNIPER_RIFLE,		// �X�i�C�p�[���C�t���ˌ�

		MOTION_FIST_1,				// �����[�V����1
		MOTION_FIST_2,				// �����[�V����2
		MOTION_FIST_3,				// �����[�V����3
		MOTION_FIST_4,				// �����[�V����4
		MOTION_FIST_5,				// �����[�V����5
		//MOTION_FIST_6,			// �����[�V����6

		MOTION_SLASH_1,				// �a�����[�V����1
		MOTION_SLASH_2,				// �a�����[�V����2
		MOTION_SLASH_3,				// �a�����[�V����3
		MOTION_SLASH_4,				// �a�����[�V����4
		MOTION_SLASH_5,				// �a�����[�V����5
		MOTION_SLASH_6,				// �a�����[�V����6

		MOTION_POKE_1,				// �h�˃��[�V����1
		MOTION_POKE_2,				// �h�˃��[�V����2
		MOTION_POKE_3,				// �h�˃��[�V����3
		MOTION_HEAVY_POKE_1,		// �d�h�˃��[�V����1
		MOTION_HEAVY_POKE_2,		// �d�h�˃��[�V����2
		MOTION_HEAVY_POKE_3,		// �d�h�˃��[�V����3

		MOTION_MAX,
	};

	// �W���u�̎���
	enum JOB
	{
		JOB_STANDART = 0,	// �X�^���_�[�h
		JOB_RUSH ,			// ���b�V��
		JOB_VANGUARD,		// ���@���K�[�h
		JOB_EAGLEEYE,		// �C�[�O���A�C
		JOB_MARSHALL,		// �}�[�V����
		JOB_MAX
	};

	CPlayer();
	virtual ~CPlayer() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void ChangeMotion();		// ���[�V�����̐؂�ւ�
	void Hit(CMove_Object* pHit) override;
	virtual void Destroy() override;			// �j�󏈗�

	void PlayerAttack();				// �v���C���[�̍U������
	void MeleeWeaponAttack();			// �ߐڕ���̍U������
	void GunWeaponAttack();				// �e����̍U������
	void BulletAttack(const int weapon);				// �e�U������
	void AttackInit();					// �U���I������
	void JumpStart();					// �v���C���[�̃W�����v����
	void JumpBoost();					// �v���C���[�̃W�����v�u�[�X�g����
	void Landing(const D3DXVECTOR3 pos) override;	// ���n����
	void Target();						// �^�[�Q�b�g
	bool Target_Scope(D3DXVECTOR3 nearpos);				// �^�[�Q�b�g��_���͈�
	void Reticle(D3DXVECTOR3 target);	// ���e�B�N��
	void DropGet(CDrop_Weapon* pDrop);		// ���Ƃ�������肷�鏈��

	void CollisionDropWeapon();			// �����Ă镐��̓����蔻��

	void SettingParameter();			// �p�����[�^�[�̐ݒ菈��

	void SetCharaIndex(const int index) { m_nCharaIndex = index; }
	void SetEnergyGauge(CEnergy_Gauge *pEnergy) { m_pEnergy_Gauge = pEnergy; }
	void SetDropGet(const bool drop_get) { m_bDrop_Get = drop_get; };
	void SetJobIndex(const int parts, const int index) { m_Parts_Job[parts] = (JOB)index; }
	void SetPlayerParts(const PARTS parts, const int weapon, const int rarity);
	void SetPlayerWeapon(const int weapon, const int rarity);
	void SetPlayerAttack(const bool attack) { m_bPlayer_Attack = attack; }
	void SetAdditionalAttack(const bool attack) { m_bAdditional_Attack = attack; }


	const bool GetDropContact() { return m_bDrop_Contact; }
	const int GetCharaIndex() { return m_nCharaIndex; }
	CEnergy_Gauge* GetEnergy_Gauge() { return m_pEnergy_Gauge; }
	const bool GetPlayerAttack() { return m_bPlayer_Attack; }
	const bool GetStandbyAttack() { return m_bStandby_Attack; }
	const int GetWeaponType() { return m_nWeapon_type; }
	const JOB GetJobIndex(const int index) { return m_Parts_Job[index]; }

private:
	int m_nCharaIndex;			// ���g�̔ԍ�
	float m_fTarget_Scope;		// �^�[�Q�b�g��_���͈�
	bool m_bTarget;				// �^�[�Q�b�g�����邩
	bool m_bDrop_Contact;		// ���Ƃ����Ƃ̐ڐG����
	bool m_bDrop_Get;			// ���Ƃ�������肷�锻��
	bool m_bChange_Parameter;	// �p�����[�^�[�̕ϓ�����

	CEnergy_Gauge* m_pEnergy_Gauge;		// �G�l���M�[�Q�[�W

	D3DXVECTOR3 m_NearMob_Pos;
	CObject3D *m_pReticle;				// ���e�B�N��
	D3DXVECTOR3 m_Reticle_Pos;
	D3DXVECTOR2 m_Reticle_Size;
	D3DXVECTOR2 m_Reticle_Tran_Size;
	CEnemy *m_pEnemy;
	float m_fReticle_Alpha;
	float m_fHypotenuse;
	bool m_bReticle_Draw;
	bool m_bReticle_Reset;
	int m_nEnemy_Count;
	float m_fEnemy_Speed;
	float m_fAngle;
	int m_nStan_Tolerance;				// �X�^�����e�l
	int m_nGravity;						// �d��
	bool m_bPlayer_Attack;				// �v���C���[����̍U������
	bool m_bStandby_Attack;				// �ǉ��U���̑ҋ@����
	bool m_bAdditional_Attack;			// �ǉ��U���̔���
	int m_nAdditional_Attack;			// �ǉ��U���̉�
	float m_fAttackRate_Counter;		// �U���Ԋu�̌o�ߎ���
	float m_fAttackRate_Max_Counter;	// �U���Ԋu�̍ő厞��

	JOB m_Parts_Job[PARTS_MAX];			// �v���C���[�̃p�[�c���̃W���u���
	int m_nArms_Rarity;					// �r�̃��A���e�B
	int m_nLeg_Rarity;					// �r�̃��A���e�B

	int m_nWeapon_type;					// ����̎��
	int m_nWeapon_Rarity;				// ����̃��A���e�B
	CWeapon* m_pRightWeapon;			// �E�蕐��
	CWeapon* m_pLeftWeapon;				// ���蕐��

};

#endif// _PLAYER_H_