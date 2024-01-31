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
#include "object.h"
#include "model.h"
#include <vector>

//---------------------------
// �N���X�錾
//---------------------------
class CEnemy : public CObject
{
public:

	CEnemy();
	virtual ~CEnemy() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Move();						// �ړ�����
	void Motion();						// ���[�V�����̐ݒ�
	virtual void ChangeMotion();		// ���[�V�����̐؂�ւ�
	const float NormalizeRot();			// �p�x�̐��K��
	void Death();						// ���g�̏I������

	void LoadFileEnemy(const char *Xfilename);

	CModel* SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename);								// ���f���p�[�c�̐ݒ�
	void SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop);	// ���[�V�����l�̓ǂݍ���

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void AddMove(const D3DXVECTOR3 move) { m_move += move; }
	void SetRotDest(const D3DXVECTOR3 rot) { m_rotDest = rot; }
	void AddRotDest(const D3DXVECTOR3 rot) { m_rotDest += rot; }
	void SetKnockBack(const int power) { m_nKnockBack_Power = power; }
	void SetMotion(const int motion) { m_nMotion = motion; }
	void SetCurrentMotion(const int motion) { m_nCurrentMotion = motion; }
	void SetRadius(const float radius) { m_fColRadius = radius; }
	void SetSpeed(const float speed) { m_fSpeed = speed; }
	
	const D3DXVECTOR3 GetPos() { return m_pos; }
	const D3DXVECTOR3 GetRot() { return m_rot; }
	const int GetKnockBack() { return m_nKnockBack_Power; }
	const int GetMotion() { return m_nMotion; }
	const int GetCurrentMotion() { return m_nCurrentMotion; }
	const float GetRadius() { return m_fColRadius; }				// �T�C�Y(���a)
	std::vector<CModel*> GetModel() { return m_pModel; }

private:
	static const float Move_Inertia;	// �G�L�����̈ړ�����
	static const float Enemy_Rot_Speed;	// �G�L�����̉�]���鑬�x

	D3DXMATRIX m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_posOld;				// �ߋ��̈ʒu
	D3DXVECTOR3 m_move;					// �ړ���
	D3DXVECTOR3 m_rot;					// ���݂̊p�x
	D3DXVECTOR3 m_rotDest;				// �ړI�̊p�x
	std::vector<D3DXVECTOR3> m_InitPos;	// �p�[�c�̏����ʒu
	std::vector<D3DXVECTOR3> m_InitRot;	// �p�[�c�̏����p�x
	D3DXCOLOR m_col;					// ���f���̐F

	float m_fColRadius;					// �����蔻��̑傫��
	float m_fSpeed;						// �ړ����x
	float m_fRotSpeed;					// ��]�̃X�s�[�h

	int m_nKnockBack_Power;				// �m�b�N�o�b�N�������
	int m_nMotion;						// ���[�V����
	int m_nCurrentMotion;				// ���݂̃��[�V�����ԍ�
	int m_nCountMotion;					// ���[�V�����J�E���^�[
	int m_nCurrentKey;					// ���݂̃L�[�ԍ�
	bool m_bNullModel;					// ���f����null
	std::vector<CModel*> m_pModel;		// ���f���̏��

	// �L�[
	struct KEY
	{
		D3DXVECTOR3 KeyPos;
		D3DXVECTOR3 KeyRot;
	};

	// �L�[�Z�b�g
	struct KEY_SET
	{
		int nFrame;
		std::vector<KEY> aKey;
	};

	// ���[�V�����Z�b�g
	struct MOTION_SET
	{
		int nMaxKey;			// �L�[�̑���
		bool bLoop;				// ���[�v���邩
		bool bStop;				// ���[�V�������~�܂�����
		std::vector<KEY_SET> aKeySet;
	};
	std::vector<MOTION_SET> m_MotionSet;
};

#endif// _ENEMY_H_