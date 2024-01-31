//=============================================================================
//
// parts.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PARTS_H_
#define _PARTS_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "object.h"
#include "objectX.h"
#include "motion.h"
#include "parts_file.h"

class CCharacter;

//---------------------------
// �N���X�錾
//---------------------------
class CParts : public CObject
{
public:

	CParts(const CObject::PRIORITY priority = CObject::PRIORITY_MODEL);
	virtual ~CParts() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Motion();							// ���[�V�����̐ݒ�
	void ChangeMotion();					// ���[�V�����̐؂�ւ�
	void Rotation();								// ��]�����֐U��������鏈��
	void NormalizeRot();							// �p�x�̐��K��

	void SetMotion(const int motion) { m_nMotion = motion; }
	void SetCurrentMotion(const int motion) { m_nCurrentMotion = motion; }
	
	void AddPos(const D3DXVECTOR3 pos) { m_pos += pos; }				// �ʒu�̉��Z
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }					// �ʒu�̐ݒ�
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetRotDest(const D3DXVECTOR3 rot) { m_rotDest = rot; }
	void AddRotDest(const D3DXVECTOR3 rot) { m_rotDest += rot; }

	void SetParts(const int partsIndex);
	void SetParent(CCharacter* parent) { m_pParent = parent; }
	void SetModelParent(CObjectX* parts, const bool parent = false);
	void SetMotionName(const std::string name) { m_sMotionName = name; }
	void SetMotionSpeed(const float speed) { m_fMotion_Speed = speed; }
	void SetMotionStop() { m_bMotionStop = true; }

	const bool GetMotionLoop() { return m_bLoop; }			// ���݃��[�V�����̃��[�v����
	const bool GetMotionStop() { return m_bMotionStop; }	// ���݃��[�V�����̏I�������ǂݍ���
	const int GetMotion() { return m_nMotion; }
	const int GetCurrentMotion() { return m_nCurrentMotion; }

	const D3DXVECTOR3 GetPos() { return m_pos; }						// �ʒu�̎擾
	const D3DXVECTOR3 GetRot() { return m_rot; }
	const D3DXVECTOR3 GetRotDest() { return m_rotDest; }
	CObjectX* GetModel(const int index) { return m_vpModel[index]; }
	std::vector<CObjectX*> GetModelAll() { return m_vpModel; }	// �S�Ẵ��f���̎擾����

	static CParts* Create(const D3DXVECTOR3 pos, const int partsIndex, CMotion::MOTION motion, CCharacter* parent = nullptr);

private:
	int m_nPartsIndex;					// ���g�̔ԍ�

	D3DXMATRIX m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_posOld;				// �ߋ��̈ʒu
	D3DXVECTOR3 m_rot;					// ���݂̊p�x
	D3DXVECTOR3 m_rotDest;				// �ړI�̊p�x

	D3DXCOLOR m_col;					// ���f���̐F

	bool m_bLoop;						// ���[�v����
	bool m_bMotionStop;					// ���݂̃��[�V�������~�܂�����
	int m_nMotion;						// ���[�V����
	int m_nCurrentMotion;				// ���݂̃��[�V�����ԍ�
	int m_nCountMotion;					// ���[�V�����J�E���^�[
	int m_nCurrentKey;					// ���݂̃L�[�ԍ�
	float m_fMotion_Speed;				// ���[�V�����X�s�[�h

	float m_fRotSpeed;					// ��]�̃X�s�[�h

	std::string m_sMotionName;			// �p�[�c�̃��[�V������
	std::vector<CObjectX*> m_vpModel;	// ���f��

	CCharacter* m_pParent;				// ���g�̐e�L�����̃|�C���^
};

#endif// _PARTS_H_