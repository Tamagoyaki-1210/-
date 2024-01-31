//=============================================================================
//
// player.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "player.h"
#include "application.h"
#include "input.h"
#include "bullet.h"
#include "normal_bullet.h"
#include "homing_bullet.h"
#include "diffusion_bullet.h"
#include "parabola_bullet.h"
#include "player_manager.h"
#include "enemy_manager.h"
#include "drop_manager.h"
#include "game.h"
#include "tutorial.h"
#include "energy_gauge.h"
#include "camera.h"
#include <vector>
#include"debugProc.h"
#include"object3D.h"
#include "drop_weapon.h"
#include "weapon.h"
#include "player_parameter.h"
#include "weapon_parameter.h"
#include "weapon_attack.h"
#include "result.h"

const float CPlayer::PLAYER_COLLISION_RADIUS = 30.0f;	// �v���C���[�̓����蔻��̑傫��
const float CPlayer::PLAYER_JUMP_POWER = 10.0f;			// �v���C���[�̃W�����v��
const float CPlayer::VIEW_SCOPE_ANGLE = 44.3f;		// �v���C���[�̎���p
const float CPlayer::RETICLE_TRANSPARENCY_SIZE = 300.0f;
const float CPlayer::RETICLE_SIZE = 200.0f;
const float CPlayer::BULLET_SPEED_SCALE = 50.0f;			// �e���̔{��
//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CPlayer::CPlayer()
{
	// �v���C���[�̏����l��ݒ�
	//SetMaxLife(FIRST_MAX_LIFE);
	//SetLife(FIRST_MAX_LIFE);
	SetRadius(PLAYER_COLLISION_RADIUS);
}

//=====================================
// �f�X�g���N�^
//=====================================
CPlayer::~CPlayer()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CPlayer::Init()
{
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		m_Parts_Job[nCnt] = (JOB)CApplication::GetPlayerJobIndex(nCnt);
	}

	// �v���C���[�̃��f����ǂݍ���
	SetParts(PARTS_BODY, CParts_File::PARTS_PLAYER_BODY_1 + m_Parts_Job[PARTS_BODY], CMotion::MOTION_PLAYER_BODY);

	SetParts(PARTS_ARMS, /*CParts_File::PARTS_PLAYER_ARMS_0*/
		CParts_File::PARTS_PLAYER_ARMS_1 + m_Parts_Job[PARTS_ARMS], CMotion::MOTION_PLAYER_ARMS);

	SetParts(PARTS_LEG, /*CParts_File::PARTS_PLAYER_LEG_0*/
		CParts_File::PARTS_PLAYER_LEG_1 + m_Parts_Job[PARTS_LEG], CMotion::MOTION_PLAYER_LEG);

	SetPlayerWeapon(CApplication::GetPlayerWeaponIndex(), 0);

	// �p�����[�^�̐ݒ�
	SettingParameter();

	//// ���݂̃��[�h
	//CApplication::MODE Mode = CApplication::GetModeType();

	//// �������Ɏ��g�̃|�C���^��G�L�����}�l�[�W���[�ɐݒ�
	///*if (Mode == CApplication::MODE_TUTORIAL)
	//{
	//CTutorial::SetPlayerUI(CPlayerUi::UITYPE_SUPPORT, m_Job[PARTS_BODY]);
	//}
	//else*/ if (Mode == CApplication::MODE_GAME)
	//{
	//	CGame::SetPlayerUI(CPlayerUi::UITYPE_SUPPORT, m_Parts_Job[PARTS_BODY]);
	//}

	// �^�O�̐ݒ�
	SetTag(TAG_CHARACTER);

	// �v���C���[���ɐݒ�
	SetPlayerSide(true);

	// �T�C�Y�̐ݒ�
	SetSize({ GetRadius(),GetRadius(),GetRadius() });

	// �����蔻��̐���
	SetCollision();

	m_bTarget = false;
	m_bReticle_Reset = true;
	m_bDrop_Contact = false;
	m_bDrop_Get = false;
	m_bChange_Parameter = false;
	m_Reticle_Size = { RETICLE_TRANSPARENCY_SIZE,RETICLE_TRANSPARENCY_SIZE };
	m_fReticle_Alpha = 0.0f;

	CCharacter::Init();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CPlayer::Uninit()
{
	// �v���C���[���̋L�^
	CApplication::SetPlayerJobIndex(m_Parts_Job[CPlayer::PARTS_BODY], CPlayer::PARTS_BODY);
	CApplication::SetPlayerJobIndex(m_Parts_Job[CPlayer::PARTS_ARMS], CPlayer::PARTS_ARMS);
	CApplication::SetPlayerJobIndex(m_Parts_Job[CPlayer::PARTS_LEG], CPlayer::PARTS_LEG);
	CApplication::SetPlayerWeaponIndex(m_nWeapon_type);

	// �E�蕐��̏I��
	if(m_pRightWeapon != nullptr)
	{
		m_pRightWeapon->Uninit();
		m_pRightWeapon = nullptr;
	}
	// ���蕐��̏I��
	if (m_pLeftWeapon != nullptr)
	{
		m_pLeftWeapon->Uninit();
		m_pLeftWeapon = nullptr;
	}

	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CPlayerManager* pPlayerManager = nullptr;

	// �v���C���[�}�l�[�W���[�̎��g���폜
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pPlayerManager = CTutorial::GetPlayerManager();
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pPlayerManager = CGame::GetPlayerManager();
	}
	else if (Mode == CApplication::MODE_RESULT)
	{
		pPlayerManager = CResult::GetPlayerManager();
	}
	if (pPlayerManager != nullptr)
	{
		pPlayerManager->PlayerRelease(m_nCharaIndex);

		if (m_pEnergy_Gauge != nullptr)
		{
			// �G�l���M�[�Q�[�W�̔j��
			m_pEnergy_Gauge->Uninit();
			m_pEnergy_Gauge = nullptr;
		}

		CCharacter::Uninit();
	}
}

//============================================================================
// �X�V����
//============================================================================
void CPlayer::Update()
{
	// �p�����[�^�[�̕ύX���������ꍇ�Ƀp�����[�^�[��ݒ肵��������
	if (m_bChange_Parameter == true) SettingParameter();

	if (!CApplication::GetCamera()->GetOpening())
		// �^�[�Q�b�g
		Target();

	// �����Ă镐��̓����蔻��
	CollisionDropWeapon();

	// ���[�V�����ԍ��̐ݒ�
	ChangeMotion();

	// �v���C���[�̍U������
	PlayerAttack();

	// �L�����N�^�[�̍X�V
	CCharacter::Update();
	//CDebugProc::Print("�v���C���[���C�t�F%d / %d\n", GetLife(), GetMaxLife());
	//CDebugProc::Print("%f %f %f\n", GetPos().x, GetPos().y, GetPos().z);
}

//============================================================================
// �`�揈��
//============================================================================
void CPlayer::Draw()
{
	CCharacter::Draw();
}

//============================================================================
// ���[�V�����ύX����
//============================================================================
void CPlayer::ChangeMotion()
{
	// ���n���[�V������ݒ�
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		// �p�[�c
		CParts* pParts = GetParts(nCnt);

		// ���[�V���������[�v���Ȃ��ꍇ
		if (pParts->GetMotionLoop() == false && pParts->GetMotionStop() == true && !GetBoost()
			&& pParts->GetMotion() == MOTION_LANDING)
		{
			pParts->SetMotion(MOTION_NEUTRAL);
		}
	}
}

//============================================================================
// �v���C���[�̍U������
//============================================================================
void CPlayer::PlayerAttack()
{
	// �U�����肪�^�̏ꍇ�A�U��������ǂݍ��ݑ�����
	if (m_bPlayer_Attack == true)
	{
		const int nWeaponType = m_nWeapon_type + CDrop_Weapon::GUN_WEAPON_AR_AR40;

		if (nWeaponType >= CDrop_Weapon::GUN_WEAPON_MAX)
		{
			MeleeWeaponAttack();
		}
		else
		{
			GunWeaponAttack();

		}
			// �Q�[�W�����Ԃ̐ݒ�
			m_pEnergy_Gauge->SetConsumption(true);
	}
}

//============================================================================
// �ߐڕ���̍U������
//============================================================================
void CPlayer::MeleeWeaponAttack()
{
	// ����ԍ�
	int nWeaponNumber = 0;

	// �ߐڕ���̍Œ�l���傫���ꍇ
	if (m_nWeapon_type >= CWeapon::MELEE_WEAPON_POKE_LANCE)
	{
		// ����̍Œ�l�������l�ɐݒ�
		nWeaponNumber = MOTION_POKE_1;
	}
	// �ߐڕ���̍Œ�l���傫���ꍇ
	else if (m_nWeapon_type >= CWeapon::MELEE_WEAPON_SLASH_SAMURAI_SWORD)
	{
		// ����̍Œ�l�������l�ɐݒ�
		nWeaponNumber = MOTION_SLASH_1;
	}
	// �f��̍Œ�l���傫���ꍇ
	else
	{
		// ����̍Œ�l�������l�ɐݒ�
		nWeaponNumber = MOTION_FIST_1;
	}

	// ���[�V�����ԍ�
	const int nMotion = nWeaponNumber + m_nAdditional_Attack;

	// ���[�V�������
	CMotion* pMotion = CApplication::GetMotion();

	// �U���J�n����
	if (m_fAttackRate_Counter == 0)
	{
		// (�r���[�V�����̑S�t���[���� - 1) ���v��
		CMotion::MotionPattern pMotionPattern = pMotion->GetMotionPattern(nMotion, CMotion::m_cMotionFileName[PARTS_ARMS]);

		// ���[�V�����S�̂̕b����ݒ�
		for (int nCnt = 0; nCnt < pMotionPattern.nMaxKey - 1; nCnt++)
		{
			m_fAttackRate_Max_Counter += pMotionPattern.aKeySet[nCnt].nFrame;
		}

		// �U�����[�V������ݒ�
		for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
		{
			// �U�����[�V���� + �ǉ��U���̉�
			GetParts(nCnt)->SetMotion(nMotion);
		}
	}
	// 0�Ԗڂ̍U���I�����ɍU�����s��
	else if (m_fAttackRate_Counter == pMotion->GetMotionPattern(nMotion, CMotion::m_cMotionFileName[PARTS_ARMS]).aKeySet[0].nFrame - 1)
	{
		// ���݂̃��[�h
		CApplication::MODE Mode = CApplication::GetModeType();

		CWeapon_Parameter::MELEE_WEAPON_PARAMETERS Melee_Parameter = {};

		CWeapon_Parameter* pWeapon_Parameter = nullptr;

		//�p�����[�^�̎擾
		/*if (Mode == CApplication::MODE_TUTORIAL)
		{
		pWeapon_Parameter = CTutorial::GetWeaponParameter();
		}
		else*/
		if (Mode == CApplication::MODE_GAME)
		{
			pWeapon_Parameter = CGame::GetWeaponParameter();
		}

		// ����p�����[�^�[
		Melee_Parameter = pWeapon_Parameter->GetParameterMeleeWeapon(m_nWeapon_type - CWeapon::WEAPON_KNUCKLE, m_nWeapon_Rarity);

		// ���̎擾
		D3DXVECTOR3 pos = GetCenterPos();
		D3DXVECTOR3 rot = GetRot();

		D3DXVECTOR3 pos_vec = { -sinf(rot.y), sinf(rot.x), -cosf(rot.y) };

		// �����蔻��̑傫��
		float radiusSize = 80.0f * (Melee_Parameter.nLength + 3);

		// �˒��̉��Z
		pos += pos_vec * radiusSize / 2.0f;

		// ����U��(�ʒu�A�傫���A�T�C�h�A�З�, ����)
		CWeapon_Attack::Create(pos, radiusSize, true, Melee_Parameter.nPower, 20);

		// �ǉ��U���̑ҋ@
		m_bStandby_Attack = true;

		int nPlayLabel = CSound::SOUND_LABEL_SE_PUNCH + ((nWeaponNumber - MOTION_FIST_1) / 4);
		// �ߐڍU��SE
		CApplication::GetSound()->Play((CSound::SOUND_LABEL)nPlayLabel);
	}

	// ���[�V�������I�������ꍇ
	if (m_fAttackRate_Counter >= m_fAttackRate_Max_Counter)
	{
		// �U���������ɍU���R�}���h���I�����ꂽ�ꍇ�ɒǉ��U��������
		if (m_bAdditional_Attack == true && m_nAdditional_Attack < 2)
		{
			// �U���񐔂𑝂₷
			m_nAdditional_Attack++;
			m_bStandby_Attack = false;
			m_bAdditional_Attack = false;
			m_fAttackRate_Counter = 0;
			m_fAttackRate_Max_Counter = 0;
		}
		// ���[�V�����I�����ɒǉ��R�}���h�������ꍇ�ɏI������
		else
		{
			m_nAdditional_Attack = 0;

			// �ʏ탂�[�V������ݒ�
			for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
			{
				// �U�����[�V�����̐ݒ�
				GetParts(nCnt)->SetMotion(MOTION_NEUTRAL);
			}
			// �U������������
			AttackInit();
		}
	}
	else
	{
		m_fAttackRate_Counter++;
	}
}

//============================================================================
// �e����̍U������
//============================================================================
void CPlayer::GunWeaponAttack()
{
	// ����ԍ�
	int nWeaponNumber = 0;

	// �e����̍Œ�l���傫���ꍇ
	if (m_nWeapon_type >= CWeapon::GUN_WEAPON_SR_WINTER5000)
	{
		// ����̍Œ�l�������l�ɐݒ�
		nWeaponNumber = MOTION_SNIPER_RIFLE;
	}
	// �e����̍Œ�l���傫���ꍇ
	else if (m_nWeapon_type >= CWeapon::GUN_WEAPON_SG_12PUMP)
	{
		// ����̍Œ�l�������l�ɐݒ�
		nWeaponNumber = MOTION_SHOT_GUN;
	}
	// �e����̍Œ�l���傫���ꍇ
	else if (m_nWeapon_type >= CWeapon::GUN_WEAPON_MG_LA2000)
	{
		// ����̍Œ�l�������l�ɐݒ�
		nWeaponNumber = MOTION_MACHIN_GUN;
	}
	// �e����̍Œ�l���傫���ꍇ
	else if (m_nWeapon_type >= CWeapon::GUN_WEAPON_HG_HG37)
	{
		// ����̍Œ�l�������l�ɐݒ�
		nWeaponNumber = MOTION_HUND_GUN;
	}
	// �e����̍Œ�l���傫���ꍇ
	else if (m_nWeapon_type >= CWeapon::GUN_WEAPON_SMG_MPC50)
	{
		// ����̍Œ�l�������l�ɐݒ�
		nWeaponNumber = MOTION_SUB_MACHIN_GUN;
	}
	// �e����̍Œ�l���傫���ꍇ
	else
	{
		//�e����̍Œ�l�������l�ɐݒ�
		nWeaponNumber = MOTION_ASSAULT_RIFLE;
	}

	// ���[�V�������
	CMotion* pMotion = CApplication::GetMotion();

	// �U���J�n����
	if (m_fAttackRate_Counter == 0.0f)
	{
		// �ǉ��U���������ꍇ
		if (m_nAdditional_Attack == 0)
		{
			// �U�����[�V������ݒ�
			for (int nCnt = 0; nCnt < PARTS_MAX - 1; nCnt++)
			{
				// �U�����[�V�����̐ݒ�
				GetParts(nCnt)->SetMotion(nWeaponNumber);
			}

			// (�r���[�V�����̑S�t���[���� - 1) ���v��
			CMotion::MotionPattern pMotionPattern = pMotion->GetMotionPattern(nWeaponNumber, CMotion::m_cMotionFileName[PARTS_ARMS]);

			// ���[�V�����S�̂̕b����ݒ�
			for (int nCnt = 0; nCnt < pMotionPattern.nMaxKey - 1; nCnt++)
			{
				m_fAttackRate_Max_Counter += pMotionPattern.aKeySet[nCnt].nFrame;
			}

			// �e_�\��
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_GUN_STANDBY);
		}
	}
	// 0�Ԗڂ̍U���I�����ɍU�����s�� && �ŏ��̍U���̏ꍇ
	else if (m_fAttackRate_Counter == m_fAttackRate_Max_Counter
		&& m_bStandby_Attack == false && m_nAdditional_Attack == 0)
	{
		// �e�U������
		BulletAttack(nWeaponNumber);

		// �U�����[�V������ݒ�
		for (int nCnt = 0; nCnt < PARTS_MAX - 1; nCnt++)
		{
			// ���[�V�����̒�~��ݒ�
			GetParts(nCnt)->SetMotionStop();
		}
	}

	// ���[�V�������I�������ꍇ
	if (m_fAttackRate_Counter >= m_fAttackRate_Max_Counter)
	{
		// �ǉ��U��������ꍇ
		if (m_bAdditional_Attack == true)
		{
			// �e�U������
			BulletAttack(nWeaponNumber);
		}
		// �ǉ��U�������݂��Ȃ��ꍇ
		else
		{
			m_bPlayer_Attack = false;
			m_bStandby_Attack = false;
			m_fAttackRate_Counter = 0.0f;
			m_fAttackRate_Max_Counter = 0.0f;

			// �ʏ탂�[�V������ݒ�
			for (int nCnt = 0; nCnt < PARTS_MAX - 1; nCnt++)
			{
				// �U�����[�V�����̐ݒ�
				GetParts(nCnt)->SetMotion(MOTION_NEUTRAL);
			}
		}
		// �U���������ɍU���R�}���h���I�����ꂽ�ꍇ�ɒǉ��U��������
		m_nAdditional_Attack = (int)m_bAdditional_Attack;

		// ������
		m_bAdditional_Attack = false;
	}
	// ���[�V�������̏ꍇ
	else
	{
		m_fAttackRate_Counter++;
	}
}

//============================================================================
// �e�U������
//============================================================================
void CPlayer::BulletAttack(const int weapon)
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CWeapon_Parameter::GUN_WEAPON_PARAMETERS Gun_Parameter = {};

	CWeapon_Parameter* pWeapon_Parameter = nullptr;

	//�p�����[�^�̎擾
	/*if (Mode == CApplication::MODE_TUTORIAL)
	{
	pWeapon_Parameter = CTutorial::GetWeaponParameter();
	}
	else*/
	if (Mode == CApplication::MODE_GAME)
	{
		pWeapon_Parameter = CGame::GetWeaponParameter();
	}

	// ���̎擾
	D3DXVECTOR3 pos = GetCenterPos();
	D3DXVECTOR3 rot = GetBulletRot();

	D3DXVECTOR3 pos_vec = { -sinf(rot.y), sinf(rot.x), -cosf(rot.y) };

	// ����p�����[�^�[(nPower�F�З�, fFiring_Speed�F���ˑ��x, nGravity�F�d��, nBullet_Speed�F�e��, nLife�F����)
	Gun_Parameter = pWeapon_Parameter->GetParameterGunWeapon(m_nWeapon_type, m_nWeapon_Rarity);

	int nBulletPower = Gun_Parameter.nPower;

	if (weapon == MOTION_HUND_GUN)
	{
		nBulletPower *= 2;
	}
	else if (weapon == MOTION_SHOT_GUN)
	{
		nBulletPower *= 10;
	}

	// �З͕��Q�[�W�����炷
	m_pEnergy_Gauge->SetEnergyConsumed((float)nBulletPower);

	// �Q�[�W���c���Ă���ꍇ
	if (!m_pEnergy_Gauge->GetConsumption())
	{
		// �e�̑��x���グ��
		float fSpeed = BULLET_SPEED_SCALE * (1 + Gun_Parameter.nBullet_Speed);

		// �e�̐���
		if (weapon == MOTION_HUND_GUN)
		{
			pos.x -= 30.0f;
			CNormal_Bullet::Create(pos, { 15.0f,15.0f }, pos_vec, m_fHypotenuse, m_pEnemy, m_fEnemy_Speed, m_bReticle_Draw, true, Gun_Parameter.nPower, fSpeed, Gun_Parameter.nLife);
			pos.x += 60.0f;
			CNormal_Bullet::Create(pos, { 15.0f,15.0f }, pos_vec, m_fHypotenuse, m_pEnemy, m_fEnemy_Speed, m_bReticle_Draw, true, Gun_Parameter.nPower, fSpeed, Gun_Parameter.nLife);
			// �e����SE2��
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_AR);
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_AR);
		}
		else if (weapon == MOTION_SHOT_GUN)
		{
			CDiffusion_Bullet::Create(pos, { 30.0f,30.0f }, pos_vec, 10, true, Gun_Parameter.nPower, fSpeed, Gun_Parameter.nLife);
			
			// �e����SE3��
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_SG);
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_SG);
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_SG);
		}
		else
		{
			CNormal_Bullet::Create(pos, { 60.0f,60.0f }, pos_vec, m_fHypotenuse, m_pEnemy, m_fEnemy_Speed, m_bReticle_Draw, true, Gun_Parameter.nPower, fSpeed, Gun_Parameter.nLife);
			
			// �}�V���K���n��
			if (weapon == MOTION_MACHIN_GUN || weapon == MOTION_SUB_MACHIN_GUN)
			{
				// �e����SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_MG);
			}
			// �X�i�C�p�[���C�t��
			else if (weapon == MOTION_SNIPER_RIFLE)
			{
				// �e����SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_SR);
			}
			// ���̑�
			else
			{
				// �e����SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_AR);
			}
		}
	}
	// �e�؂�̏ꍇ
	else
	{
		// �e�؂�
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_OUT_OF_BULLET);
	}

	// �N�[���^�C���̔h��
	float fBulletSpeed = 1.0f / Gun_Parameter.fFiring_Speed * 60.0f;
	m_fAttackRate_Max_Counter = fBulletSpeed != 0.0f ? fBulletSpeed : 1.0f;

	// �ŏ��̏ꍇ
	if(m_nAdditional_Attack == 0)
	{
		m_fAttackRate_Counter = 0;
	}
	// �e������������ꍇ
	else
	{
		// �U���Ԋu���獷������
		m_fAttackRate_Counter -= m_fAttackRate_Max_Counter;
	}

	// �ǉ��U���̑ҋ@
	m_bStandby_Attack = true;
}

//============================================================================
// �U���I������
//============================================================================
void CPlayer::AttackInit()
{
	m_bPlayer_Attack = false;
	m_bStandby_Attack = false;
	m_bAdditional_Attack = false;
	m_fAttackRate_Counter = 0;
	m_fAttackRate_Max_Counter = 0;
}

//============================================================================
// �v���C���[�̃W�����v����
//============================================================================
void CPlayer::JumpStart()
{
	// �ڒn���Ă���ꍇ�̂�
	if (GetGround())
	{
		// �W�����vSE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_PLAYERWALK);

		// �W�����v���[�V������ݒ�
		GetParts(PARTS_LEG)->SetMotion(MOTION_JUMP);

		// ������Ԃɂ���
		SetGround(false);
		SetLandObj(false);
		SetObjXZ(false);

		// �㏸����
		AddMove({ 0.0f, PLAYER_JUMP_POWER, 0.0f });
	}
}

//============================================================================
// �v���C���[�̃W�����v�u�[�X�g����
//============================================================================
void CPlayer::JumpBoost()
{
	if (m_pEnergy_Gauge != nullptr)
	{
		// �󒆂ɂ���ꍇ
		if (!GetGround())
		{
			// �㏸����
			AddMove({ 0.0f, 0.5f, 0.0f });

			// �G�l���M�[�������
			m_pEnergy_Gauge->Consumption_Gauge();
		}
	}
}

//============================================================================
// �v���C���[�̒��n����
//============================================================================
void CPlayer::Landing(const D3DXVECTOR3 pos)
{
		// ���n���[�V������ݒ�
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		CParts* pParts = GetParts(nCnt);

		// �ʏ탂�[�V�����̏ꍇ
		if (pParts->GetMotion() == MOTION_NEUTRAL || pParts->GetMotion() == MOTION_JUMP)
		{
			pParts->SetMotion(MOTION_LANDING);
		}
	}
	// �L�����N�^�[�̒��n����
	CCharacter::Landing(pos);
}

//============================================================================
// ��e����
//============================================================================
void CPlayer::Hit(CMove_Object* pHit)
{
	TAG tag = pHit->GetTag();

	// ���g�̏��
	const D3DXVECTOR3 Pos = GetPos();

	// ����̏��
	const D3DXVECTOR3 AtherPos = pHit->GetPos();

	// ���g�Ƒ���̋���
	D3DXVECTOR3 VecPos = AtherPos - Pos;

	// �����Ă�p�[�c�Ƃ̋���
	const float fDistanceNear = sqrtf(VecPos.x * VecPos.x + VecPos.z * VecPos.z);

	// ���肪�G�������ꍇ
	if (GetPlayerSide() != pHit->GetPlayerSide())
	{
		switch (tag)
		{
		case TAG_CHARACTER:
			break;
		case TAG_BULLET:
			// �e�̃_���[�W��Ԃ�
			Damage(pHit->GetPower());

			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_DAMAGE);
			break;
		case TAG_ATTACK:
			// �e�̃_���[�W��Ԃ�
			Damage(pHit->GetPower());

			D3DXVec3Normalize(&VecPos, &VecPos);
			AddMove(-VecPos * 20.0f);

			// ���G��Ԃ�t�^����
			SetCollisionNoneHit(true);

			// �_���[�W
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);
			break;
		case TAG_EXPLOSION:
			// �����̃_���[�W��Ԃ�
			Damage(pHit->GetPower());

			// ���G��Ԃ�t�^����
			SetCollisionNoneHit(true);
			break;
		case TAG_MAP_OBJECT:
			break;
		default:
			break;
		}
	}
}

//============================================================================
// �j�󏈗�
//============================================================================
void CPlayer::Destroy()
{
	CCharacter::Destroy();
}

//============================================================================
// �^�[�Q�b�g
//============================================================================
void CPlayer::Target()
{
	D3DXVECTOR3 Player_Pos = GetPos();				// �v���C���[�̈ʒu
	D3DXVECTOR3 Mob_Pos = { 0.0f,0.0f,0.0f };		// �G�̈ʒu
	m_NearMob_Pos = { 0.0f,0.0f,0.0f };				// ��ԋ߂��G�̈ʒu
	//m_fTarget_Scope = 6000.0f;						// �^�[�Q�b�g��_���͈�
	float NearDistance = m_fTarget_Scope;			// �G�Ƃ̋���
	float NextNearDistance = 0.0f;					// ���ɋ߂��G�Ƃ̋���
	m_bTarget = false;								// �߂��ɓG�����邩
	bool bScreen = false;							// ��ʂɉf���Ă��邩
	float DistanceXZ = 0.0f;						// �v���C���[�ƓG��XZ���W�̋���
	D3DXVECTOR3 BulletVec = {0.0f,0.0f,0.0f};
	m_nEnemy_Count = 0;
	int nCnt = 0;

	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CEnemyManager* pEnemyManager = nullptr;

	// ���[�h���ɓG�L������ǂݍ���
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pEnemyManager = CTutorial::GetEnemyManager();
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pEnemyManager = CGame::GetEnemyManager();
	}

	if (Mode == CApplication::MODE_GAME)
	{
		while (true)
		{
			// �G���G�̏��
			for (auto pEnemy : pEnemyManager->GetAllEnemy())
			{
				nCnt++;

				if (pEnemy->GetLife() > 0)
				{
					// �G�̈ʒu�̎擾
					Mob_Pos = pEnemy->GetCenterPos();

					// �v���C���[����G�̋���
					BulletVec = Mob_Pos - Player_Pos;

					// �����̎Z�o
					DistanceXZ = sqrtf(BulletVec.x * BulletVec.x
						+ BulletVec.z * BulletVec.z);

					// ����3000�ȏ�
					if (DistanceXZ > m_fTarget_Scope)
						continue;

					// �������ׂ�
					if (NearDistance >= DistanceXZ && NextNearDistance < DistanceXZ)
					{
						// �Z�����̋����ƈʒu����
						NearDistance = DistanceXZ;
						m_NearMob_Pos = Mob_Pos;
						m_nEnemy_Count = nCnt;
						m_fEnemy_Speed = pEnemy->GetSpeed();
						m_pEnemy = pEnemy;

						m_bTarget = true;

						// ��ʂɉf���Ă��鎞�����^�[�Q�b�g����
						bScreen = Target_Scope(m_NearMob_Pos);
					}
				}
			}

			if (m_bTarget != bScreen)
			{
				// �������߂�����ʂɉf���Ă��Ȃ��G�Ƃ̋���
				NextNearDistance = NearDistance;
				NearDistance = m_fTarget_Scope;
				nCnt = 0;
				m_bTarget = false;
			}
			else
				break;
		}

		if (m_bTarget && bScreen)
		{
			m_bReticle_Draw = true;

			// �v���C���[����G�̋���
			BulletVec = m_NearMob_Pos - GetPos();

			// �^�[�Q�b�g�����G�̕���
			m_fAngle = atan2(BulletVec.x, BulletVec.z);
			float AngleY = 0.0f;

			// �v���C���[����G�̒�������
			m_fHypotenuse = sqrt((BulletVec.y * BulletVec.y) + (NearDistance * NearDistance));

			// Y���W�̒Ǐ]
			AngleY = sinf(BulletVec.y / m_fHypotenuse);

			// �ړI�̊p�x�̐ݒ�
			CCharacter::SetBulletRot({ AngleY,m_fAngle + D3DX_PI,0.0f });
		}
		else
		{// �^�[�Q�b�g�����Ȃ��ꍇ�͐��ʂɒe������
			m_bReticle_Draw = false;

			// �J�����̊p�x
			CCamera *Camera = CApplication::GetCamera();
			D3DXVECTOR3 rotCamera = Camera->GetRot();

			// �ړI�̊p�x�̐ݒ�
			CCharacter::SetBulletRot({ rotCamera.x + D3DX_PI,rotCamera.y + D3DX_PI ,rotCamera.z + D3DX_PI });

			m_NearMob_Pos = { 0.0f,0.0f,0.0f };
		}

		// ���e�B�N���̐ݒ�
		Reticle(m_NearMob_Pos);
	}
}

//============================================================================
// �^�[�Q�b�g��_���͈�
//============================================================================
bool CPlayer::Target_Scope(D3DXVECTOR3 nearpos)
{
	// �J�����̊p�x
	CCamera *Camera = CApplication::GetCamera();
	D3DXVECTOR3 rotCamera = Camera->GetRot();

	// �v���C���[���璍���_�܂ł̃x�N�g��
	D3DXVECTOR3 CameraVec = Camera->GetWorldPosR() - GetPos();

	// ���K��
	D3DXVec3Normalize(&CameraVec, &CameraVec);

	// �v���C���[����G�̃x�N�g��
	D3DXVECTOR3 EnemyVec = nearpos - GetPos();

	// ���K��
	D3DXVec3Normalize(&EnemyVec, &EnemyVec);

	// ����
	float fInner = D3DXVec3Dot(&EnemyVec, &CameraVec);

	// �J�����̌��ɓG������ꍇ
	if (fInner < 0)
		return false;

	// ����p
	float fView_Angle = VIEW_SCOPE_ANGLE;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// ��ʂɉf�邬�肬��̈ʒu
		D3DXVECTOR3 Reflected_Pos = {0.0f, 0.0f, 0.0f};
		D3DXVECTOR3 Reflected_PosVec = { 0.0f, 0.0f, 0.0f };

		// �^�[�Q�b�g�o���邬�肬��̈ʒu
		Reflected_Pos.x = Camera->GetWorldPosV().x + sinf(rotCamera.y + fView_Angle) * m_fTarget_Scope;
		Reflected_Pos.z = Camera->GetWorldPosV().z + cosf(rotCamera.y + fView_Angle) * m_fTarget_Scope;
		fView_Angle *= -1;

		// �J�����̎��_
		D3DXVECTOR3 WorldPosV = Camera->GetWorldPosV();

		// �J�����̎��_�����p�����炷
		if (nCnt == 0)
		{
			WorldPosV.x += sinf(rotCamera.y + D3DX_PI / 2) * 500;
			WorldPosV.z += cosf(rotCamera.y + D3DX_PI / 2) * 500;
		}
		else
		{
			WorldPosV.x += sinf(rotCamera.y - D3DX_PI / 2) * 500;
			WorldPosV.z += cosf(rotCamera.y - D3DX_PI / 2) * 500;
		}

		// �J�����̎��_����̃x�N�g��
		Reflected_PosVec = Reflected_Pos - WorldPosV;

		// ���K��
		D3DXVec3Normalize(&Reflected_PosVec, &Reflected_PosVec);

		// ���_����G�̃x�N�g��
		EnemyVec = nearpos - WorldPosV;

		// ���K��
		D3DXVec3Normalize(&EnemyVec, &EnemyVec);

		// �O�ς̊i�[��
		float fCp = Reflected_PosVec.x * EnemyVec.z - Reflected_PosVec.z * EnemyVec.x;

		// ��ʓ��ɉf���Ă��邩
		if (nCnt == 0)
		{
			if (fCp > 0.0f)
				continue;
			else
				break;
		}
		else
			if (fCp <= 0.0f)
				return true;
	}

	return false;
}

//============================================================================
// ���e�B�N��
//============================================================================
void CPlayer::Reticle(D3DXVECTOR3 target)
{
	// �^�[�Q�b�g�̈ʒu
	if (m_bReticle_Reset)
		m_Reticle_Pos = target;

	// �g��k���̑��x
	float Size_Speed = 10.0f;
	// �A���t�@�l�̉��Z���Z�̑��x
	float Alpha_Speed = 1.0f / ((RETICLE_TRANSPARENCY_SIZE - RETICLE_SIZE) / Size_Speed);

	// ���e�B�N���̐���
	if (m_pReticle == nullptr && m_bReticle_Draw)
	{
		m_pReticle = CObject3D::Create({ m_Reticle_Pos }, { m_Reticle_Size }, PRIORITY_CENTER, { 1.0f,1.0f,1.0f,m_fReticle_Alpha }, true);
		m_pReticle->SetTexture(CTexture::TEXTURE_RETICLE);
	}

	// ���e�B�N������������Ă���ꍇ
	if (m_pReticle != nullptr)
	{
		if (m_bReticle_Draw)
		{
			// ���e�B�N���̈ړ�
			if (m_Reticle_Pos != target)
				m_Reticle_Pos += (target - m_Reticle_Pos) *  0.1f;

			if (m_Reticle_Size.x > RETICLE_SIZE)
			{
				// �T�C�Y�ƃA���t�@�l�̐ݒ�
				m_fReticle_Alpha += Alpha_Speed;
				m_Reticle_Size.x -= Size_Speed;
				m_Reticle_Size.y -= Size_Speed;
			}

			// �ʒu�̐ݒ�
			m_pReticle->SetPos(m_Reticle_Pos);

			m_bReticle_Reset = false;
		}
		else
		{
			if (m_Reticle_Size.x < RETICLE_TRANSPARENCY_SIZE)
			{
				// �T�C�Y�ƃA���t�@�l�̐ݒ�
				m_fReticle_Alpha -= Alpha_Speed;
				m_Reticle_Size.x += Size_Speed;
				m_Reticle_Size.y += Size_Speed;
			}
			else
				// ���e�B�N���̃��Z�b�g
				m_bReticle_Reset = true;
		}

		// �T�C�Y�ƐF�̐ݒ�
		m_pReticle->SetSize({ m_Reticle_Size });
		m_pReticle->SetCol({ 1.0f,1.0f,1.0f,m_fReticle_Alpha });
		m_pReticle->SetTexture(CTexture::TEXTURE_EFFECT_RETICLE);
	}
}

//============================================================================
// ���Ƃ�������肷�鏈��
//============================================================================
void CPlayer::DropGet(CDrop_Weapon* pDrop)
{
	// �v���C���[�p�p�[�c�̏��
	CPlayer::PARTS Parts = pDrop->GetPartsType();

	// ����̏��
	int nWeapon = pDrop->GetWeaponType();

	// ���A���e�B�̎擾
	const int nRarity = pDrop->GetRarity();

	// ����p�[�c�ł͂Ȃ��ꍇ
	if (Parts != PARTS_WEAPON)
	{
		SetPlayerParts(Parts, nWeapon, nRarity);
	}
	// ����p�[�c�̏ꍇ
	else
	{
		// �ߐڕ���̏ꍇ�A�ԍ�������炷
		if (nWeapon > CDrop_Weapon::GUN_WEAPON_MAX)
		{
			nWeapon--;
		}

		// ����p�[�c�̕ύX����
		SetPlayerWeapon(nWeapon - CDrop_Weapon::GUN_WEAPON_AR_AR40, nRarity);
	}

	// �p�����[�^�̍X�V
	m_bChange_Parameter = true;

	// ���Ƃ����̏I������
	pDrop->Uninit();

	// �U���I������
	AttackInit();
}

//============================================================================
// �����Ă镐��̓����蔻��
//============================================================================
void CPlayer::CollisionDropWeapon()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CDropManager* pManager = nullptr;

	// ���[�h���ɗ��Ƃ�����ǂݍ���
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pManager = CTutorial::GetDropManager();
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pManager = CGame::GetDropManager();
	}

	// ���g�̏��
	const D3DXVECTOR3 pos = GetPos();
	const float fRadius = GetRadius();

	CDrop_Weapon* pNearDrop = nullptr;	// �ł��������߂����Ƃ���
	float fNearDistance = 0.0f;			// �ł��߂�����

	if (Mode == CApplication::MODE_GAME)
	{
		// �S�Ă̗��Ƃ�����ǂݍ���
		for (auto pDrop : pManager->GetAllDrop())
		{
			// �v���C���[���痎���Ă�p�[�c�̋���
			D3DXVECTOR3 Vec = pDrop->GetPos() - pos;

			// �����̎Z�o
			float fDistance = sqrtf(Vec.x * Vec.x + Vec.z * Vec.z);

			// �ŒZ�������X�V���ꂽ�ꍇ
			if (fNearDistance > fDistance)
			{
				fNearDistance = fDistance;
				pNearDrop = pDrop;
			}
			// �������i�[����Ă��Ȃ��ꍇ ���� �~�Ɖ~�̓����蔻��̒��ɂ���ꍇ
			else if (fNearDistance == 0.0f && fRadius + CDrop_Weapon::PARTS_COLLISION_RADIUS >= fDistance)
			{
				fNearDistance = fDistance;
				pNearDrop = pDrop;
			}
		}

		// �ڐG���Ă��闎�Ƃ��������݂���ꍇ
		m_bDrop_Contact = pNearDrop != nullptr;

		// ���Ƃ����ƐڐG���Ă���ꍇ
		if (m_bDrop_Contact == true)
		{
			// �s�b�N�A�b�v��Ԃ�Ԃ�
			pNearDrop->SetPick_Up(true);

			// ���Ƃ�������肷��ꍇ && �U�����ł͂Ȃ��ꍇ
			if (m_bDrop_Get == true && m_bPlayer_Attack == false)
			{
				// ���Ƃ�������肷�鏈��
				DropGet(pNearDrop);
			}
		}
		// ���Ƃ�������肷�锻��̃��Z�b�g
		m_bDrop_Get = false;
	}
}

//============================================================================
// �p�����[�^�[�̐ݒ菈��
//============================================================================
void CPlayer::SettingParameter()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();
	if (Mode != CApplication::MODE_RESULT)
	{
		// �p�����[�^�[�̏��
		CPlayer_Parameter::PARAMETERS Parameter = {};

		int nLife = 0;				// �ϋv�l
		int nStamina = 0;			// �X�^�~�i�e��
		int nStan_Tolerance = 0;	// �X�^�����e�l
		int nGravity = 0;			// �d��

		CPlayer_Parameter* pPlayer_Parameter = nullptr;
		CWeapon_Parameter* pWeapon_Parameter = nullptr;

		//�p�����[�^�̎擾
		/*if (Mode == CApplication::MODE_TUTORIAL)
		{
			pParameter = CTutorial::GetPlayerParameter();
			pWeapon_Parameter = CTutorial::GetWeaponParameter();
		}
		else*/
		if (Mode == CApplication::MODE_GAME)
		{
			pPlayer_Parameter = CGame::GetPlayerParameter();
			pWeapon_Parameter = CGame::GetWeaponParameter();
		}

		// �p�[�c���̃p�����[�^�[
		for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
		{
			switch (nCnt)
			{
			case 0:
				Parameter = pPlayer_Parameter->GetParameterJob(m_Parts_Job[nCnt] + 1);
				break;
			case 1:
				Parameter = pPlayer_Parameter->GetParameterArms(m_Parts_Job[nCnt] + 1, m_nArms_Rarity);
				break;
			case 2:
				Parameter = pPlayer_Parameter->GetParameterLeg(m_Parts_Job[nCnt] + 1, m_nLeg_Rarity);
				break;
			default:
				break;
			}

			nLife += Parameter.nLife;
			nStamina += Parameter.nStamina;
			nStan_Tolerance += Parameter.nStan_Tolerance;
			nGravity += Parameter.nGravity;
		}

		const int nWeaponType = m_nWeapon_type + CDrop_Weapon::GUN_WEAPON_AR_AR40;

		if (nWeaponType >= CDrop_Weapon::GUN_WEAPON_MAX)
		{
			CWeapon_Parameter::MELEE_WEAPON_PARAMETERS Melee_Parameter = {};

			// ����p�����[�^�[
			Melee_Parameter = pWeapon_Parameter->GetParameterMeleeWeapon(m_nWeapon_type, m_nWeapon_Rarity);
			m_nGravity += Melee_Parameter.nGravity;
			m_fTarget_Scope = 3000.0f;						// �^�[�Q�b�g��_���͈�
		}
		else
		{
			CWeapon_Parameter::GUN_WEAPON_PARAMETERS Gun_Parameter = {};

			// ����p�����[�^�[
			Gun_Parameter = pWeapon_Parameter->GetParameterGunWeapon(m_nWeapon_type, m_nWeapon_Rarity);
			m_nGravity += Gun_Parameter.nGravity;
			m_fTarget_Scope =(BULLET_SPEED_SCALE * (Gun_Parameter.nBullet_Speed + 1) * Gun_Parameter.nLife) - 1000.0f;						// �^�[�Q�b�g��_���͈�
			if (m_fTarget_Scope < 3000.0f)
			{
				m_fTarget_Scope = 3000.0f;
			}
		}

		// �e�p�����[�^�̐ݒ�
		SetMaxLife(nLife);
		if (m_pEnergy_Gauge != nullptr)
		{
			m_pEnergy_Gauge->SetMaxEnerugy(nStamina);
		}
		m_nStan_Tolerance = nStan_Tolerance;
		m_nGravity = nGravity;
	}

	// �X�V���~����
	m_bChange_Parameter = false;
}

//============================================================================
// �p�[�c�̐ݒ�
//============================================================================
void CPlayer::SetPlayerParts(const PARTS parts, const int weapon, const int rarity)
{
	int nPartsFileIndex = 0;	// �p�[�c�̔ԍ�

	// ���݂̃��A���e�B�̐F
	float fRarity_Color = 0;

	// �p�[�c�t�@�C���̍Œ�l��ݒ�
	switch (parts)
	{
	case CPlayer::PARTS_BODY:
		nPartsFileIndex = CParts_File::PARTS_PLAYER_BODY_1;
		break;
	case CPlayer::PARTS_ARMS:
		nPartsFileIndex = CParts_File::PARTS_PLAYER_ARMS_1;
		fRarity_Color = (float)m_nArms_Rarity;
		m_nArms_Rarity = rarity;
		break;
	case CPlayer::PARTS_LEG:
		nPartsFileIndex = CParts_File::PARTS_PLAYER_LEG_1;
		fRarity_Color = (float)m_nLeg_Rarity;
		m_nLeg_Rarity = rarity;
		break;
	default:
		break;
	}

	int nWeaponIndex = 0;	// ����̔ԍ�

							// �p�[�c�̍Œ�l��ݒ�
	if (weapon >= CDrop_Weapon::LEG_SG01)
	{
		nWeaponIndex = CDrop_Weapon::LEG_SG01;
	}
	else if (weapon >= CDrop_Weapon::ARMS_SG01)
	{
		nWeaponIndex = CDrop_Weapon::ARMS_SG01;
	}
	//else if (nWeapon >= CDrop_Weapon::BODY_SG01)
	//{
	//	nWeaponIndex = CParts_File::PARTS_PLAYER_BODY_1;
	//}

	// �p�[�c�̃W���u���̍X�V
	m_Parts_Job[parts] = (JOB)(weapon - nWeaponIndex);

	// �p�[�c�̔ԍ�(�p�[�c�t�@�C���̔ԍ� + �p�[�c�̔ԍ�(�p�[�c���̂܂܂̔ԍ� - �p�[�c�̍Œ�l))
	int nPartsIndex = nPartsFileIndex + (weapon - nWeaponIndex);

	// �w�肵���p�[�c�́A�p�[�c�ύX����
	GetParts(parts)->SetParts(nPartsIndex);

	// �ύX���郌�A���e�B�̐F
	//const float nChange_Color = 0.1f * rarity;

	// ���A���e�B�ɂ��F�̕ω�
	const float fAddCol = -(rarity - fRarity_Color) * 0.05f;

	for (auto pParts : GetParts(parts)->GetModelAll())
	{
		pParts->AddColor({ fAddCol, fAddCol,fAddCol, 0.0f });
	}
}

//============================================================================
// ����̐ݒ�
//============================================================================
void CPlayer::SetPlayerWeapon(const int weapon, const int rarity)
{
	m_nWeapon_type = weapon;
	m_nWeapon_Rarity = rarity;

	// �E�肪�g�p����Ă��Ȃ��ꍇ
	if (m_pRightWeapon == nullptr)
	{
		// �E��(�r[3])�ɕ����ݒ�
		m_pRightWeapon = CWeapon::Create({ 1.0f, -6.0f, 0.0f }, { -D3DX_PI / 2.0f, 0.0f, 0.0f }, weapon, GetParts(PARTS_ARMS)->GetModel(4));
		m_pRightWeapon->SetDrawFlag(true);
	}
	else
	{	// �E��(�r[3])�ɕ����ύX
		m_pRightWeapon->ChangeWeapon(weapon);
		m_pRightWeapon->SetDrawFlag(true);
	}

	// �n���h�K���̏ꍇ
	if ((weapon >= CWeapon::GUN_WEAPON_HG_HG37 && weapon <= CWeapon::GUN_WEAPON_HG_AKIMBO20)
		|| (weapon >= CWeapon::MELEE_WEAPON_FIST_KNUCKLE && weapon <= CWeapon::MELEE_WEAPON_FIST_STUN_BATON))
	{
		// ���肪�g�p����Ă��Ȃ��ꍇ
		if (m_pLeftWeapon == nullptr)
		{
			// ����(�r[6])�ɑf���ݒ�
			m_pLeftWeapon = CWeapon::Create({ -1.0f, -6.0f, 0.0f }, { -D3DX_PI / 2.0f, 0.0f, 0.0f }, weapon, GetParts(PARTS_ARMS)->GetModel(8));
		}
		else
		{
			// ����(�r[6])�ɑf���ݒ�
			m_pLeftWeapon->ChangeWeapon(weapon);
		}
		m_pLeftWeapon->SetDrawFlag(true);
	}
	// �n���h�K���ł͂Ȃ��ꍇ
	else
	{		// ���肪�g�p����Ă��Ȃ��ꍇ
		if (m_pLeftWeapon == nullptr)
		{
			// ����(�r[6])�ɑf���ݒ�
			m_pLeftWeapon = CWeapon::Create({ -1.0f, -6.0f, 0.0f }, { -D3DX_PI / 2.0f, 0.0f, 0.0f }, CWeapon::WEAPON_KNUCKLE, GetParts(PARTS_ARMS)->GetModel(8));
		}
		else
		{
			// ����(�r[6])�ɑf���ݒ�
			m_pLeftWeapon->ChangeWeapon(CWeapon::WEAPON_KNUCKLE);
		}
		m_pLeftWeapon->SetDrawFlag(false);
	}

	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	// �������Ƀv���C���[UI�𐶐�
	/*if (Mode == CApplication::MODE_TUTORIAL)
	{
		CTutorial::SetPlayerUI(CPlayerUi::UITYPE_ATTACK, weapon);
		CTutorial::SetPlayerUI(CPlayerUi::UITYPE_WEAPON, weapon);
	}
	else*/
	if (Mode == CApplication::MODE_GAME)
	{
		//CGame::SetPlayerUI(CPlayerUi::UITYPE_ATTACK, weapon);
		CGame::SetPlayerUI(CPlayerUi::UITYPE_WEAPON, weapon);
	}
}