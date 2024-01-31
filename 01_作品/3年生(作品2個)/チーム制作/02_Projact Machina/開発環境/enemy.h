//=============================================================================
//
// enemy.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "character.h"

//---------------------------
// �N���X�錾
//---------------------------
class CEnemy : public CCharacter
{

public:
	// �G�L�����̎��
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_MOB = 0,	// ���u�L����
		ENEMY_TYPE_BOSS		// �{�X�L����
	};

	CEnemy();
	virtual ~CEnemy() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	
	virtual void Move();
	void Hit(CMove_Object* pHit) override;

	virtual void Destroy() override;			// �j�󏈗�

	void SetEnemyType(ENEMY_TYPE type) { m_type = type; }	// �G�L�����̎�ނ�ݒ�
	void SetTracking(bool tracking) { m_bTracking = tracking; }

	ENEMY_TYPE GetEnemyType() { return m_type; }			// �G�L�����̎�ނ��擾
	bool GetTracking() { return m_bTracking; }				// �G���ǐՏ�Ԃ�
	float GetDistance() { return m_fDistance; }				// �G�ƃv���C���[�̋���
	float GetAngle() { return m_fAngle; }					// �v���C���[�܂ł̊p�x

private:
	ENEMY_TYPE m_type;		// �G�L�����̎��
	bool m_bTracking;		// �ǐՏ��
	float m_fDistance;		// �v���C���[�܂ł̋���
	float m_fAngle;			// �v���C���[�܂ł̊p�x
};

#endif// _ENEMY_H_