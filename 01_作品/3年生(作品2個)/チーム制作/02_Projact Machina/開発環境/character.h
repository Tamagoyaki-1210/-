//=============================================================================
//
// character.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "move_object.h"
#include "parts.h"
#include <vector>
#include <map>

class CGauge_Manager;

//---------------------------
// �N���X�錾
//---------------------------
class CCharacter : public CMove_Object
{
	static const float CHARACTER_FIRST_MOVE_SPEED;		// �L�����N�^�[�̏����ړ����x

	static const float CHARACTER_ROT_SPEED;				// �L�����N�^�[�̉�]���鑬�x
	static const float CHARACTER_MOVE_INERTIE;			// �L�����N�^�[�̈ړ�����
	static const float CHARACTER_GRAVITY;				// �L�����N�^�[�̏d��
	static const int CHARACTER_INVINCIBLE_TIMER = 20;	// �L�����N�^�[���G����
	 const int CHARACTER_INVINCIBLE_SPEED = 6;			// �L�����N�^�[�̓������x

public:

	CCharacter(const CObject::PRIORITY priority = CObject::PRIORITY_MODEL);
	virtual ~CCharacter() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Move();
	void Damage(const int value);
	void Recovery(const int value);
	virtual void Destroy();							// ���g��j�󂷂鏈��
	virtual void Landing(const D3DXVECTOR3 pos);	// ���n����
	void FieldCollision();							// ���̓����蔻��
	void Invincible();								// �������
	void Rotation();								// ��]�����֐U��������鏈��
	void NormalizeRot();							// �p�x�̐��K��
	virtual void Hit(CMove_Object* pHit) override = 0;

	/*void SetGround(const bool ground) { m_bGround = ground; }*/
	void SetLife(const int life);
	void SetMaxLife(const int maxlife);
	void SetSpeed(const float speed) { m_fSpeed = speed; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void AddMove(const D3DXVECTOR3 move) { m_move += move; }
	void SetRotDest(const D3DXVECTOR3 rot) { m_rotDest = rot; }
	void AddRotDest(const D3DXVECTOR3 rot) { m_rotDest += rot; }
	void SetBulletRot(const D3DXVECTOR3 bulletrot) { m_BulletRot = bulletrot; }
	void SetBoost(const bool boost) { m_bBoost = boost; }
	void SetJump_Boost(const bool jumpboost) { m_bJump_Boost = jumpboost; }

	void SetParts(const int charaParts, const int partsIndex, CMotion::MOTION motion);				// �p�[�c�̐ݒ菈��

	void SetJump_PressCount(const int jumpcount) { m_nJump_PressCount = jumpcount; }
	void AddJump_PressCount(const int jumpcount) { m_nJump_PressCount += jumpcount; }
	void SetAvoidance(const bool avoidance) { m_bAvoidance = avoidance; }
	void SetAvoidanceCount(const int avoidancecount) { m_nAvoidance_Count = avoidancecount; }
	void SetHitExplosion(const bool hit) { m_bHit_Explosion = hit; }
	void SetGaugeManager(CGauge_Manager *gauge) { m_pGaugeManager = gauge; }

	/*const bool GetGround() { return m_bGround; }*/
	const int GetLife() { return m_nLife; }
	const int GetMaxLife() { return m_nMaxLife; }
	const float GetSpeed() { return m_fSpeed; }
	const D3DXVECTOR3 GetMove() { return m_move; }
	const D3DXVECTOR3 GetRot() { return m_rot; }
	const D3DXVECTOR3 GetRotDest() { return m_rotDest; }
	const D3DXVECTOR3 GetBulletRot() { return m_BulletRot; }
	const D3DXMATRIX GetWorldMtx() { return m_mtxWorld; }
	CParts* GetParts(const int index) { return m_Parts[index]; }
	std::map<int, CParts*> GetAllParts() { return m_Parts; }
	const bool GetBoost() { return m_bBoost; }
	const bool GetJump_Boost() { return m_bJump_Boost; }
	const int GetJump_PressCount() { return m_nJump_PressCount; }
	const bool GetAvoidance() { return m_bAvoidance; }
	const bool GetHitExplosion() { return m_bHit_Explosion; }
	CGauge_Manager *GetGaugeManager() { return m_pGaugeManager; }
	int GetDamage() { return m_fDamageReceived; }

private:

	D3DXMATRIX m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_posOld;				// �ߋ��̈ʒu
	D3DXVECTOR3 m_move;					// �ړ���
	D3DXVECTOR3 m_rot;					// ���݂̊p�x
	D3DXVECTOR3 m_rotDest;				// �ړI�̊p�x
	D3DXVECTOR3 m_BulletRot;			// �e�����p�x

	D3DXCOLOR m_col;					// ���f���̐F

	//bool m_bGround;						// �ڒn����

	int m_nLife;						// �̗�
	int m_nMaxLife;						// �ő�̗�

	float m_fSpeed;						// �ړ����x
	float m_fRotSpeed;					// ��]�̃X�s�[�h
	int m_fDamageReceived;				// �󂯂��_���[�W�̑���

	bool m_bBoost;						// �u�[�X�g���Ă��邩
	bool m_bJump_Boost;					// �W�����v�u�[�X�g���Ă��邩
	int m_nJump_PressCount;				// �W�����v�̒��������Ă鎞��
	bool m_bAvoidance;					// ������Ă��邩
	int m_nAvoidance_Count;				// ����̍d������

	bool m_bHit_Explosion;				// �����ɓ���������

	int m_nInvincible_Counter;			// ���G����

	std::string m_name;					// ���g�̖��O
	std::map<int, CParts*> m_Parts;		// �p�[�c���̃|�C���^

	CGauge_Manager *m_pGaugeManager;	// �Q�[�W�}�l�[�W���[
};

#endif// _CHARACTER_H_