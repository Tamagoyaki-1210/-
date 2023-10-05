//==============================================================================================
//
// �g���b�R�@�@�@trolley.h
// tutida ryousei
//
//==============================================================================================
#ifndef _TROLLEY_H__
#define	_TROLLEY_H_

#include"trolley_manager.h"

class CObjectX;
class CModel;

class CTrolley : public CObject
{
public:
	CTrolley();
	~CTrolley();

	// ���[�V�����̃^�C�v
	enum Trolley_Motion_Type
	{
		TROLLEY_MOTION_MOVE = 0,			// �ړ�
		MOTION_MAX,
		MOTION_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetTrolleyInfomation();			// �g���b�R�̏�������ݒ�
	void SetStart(const int index);			// �����ʒu�̐ݒ�
	void SetMotionData(int maxkey, int key, int parts, int motion,
		int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop);				// ���[�V�����l�̓ǂݍ���
	void Motion();				// ���[�V�����̐ݒ�
	void SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename);
	void Trolley_Collision();	// �����蔻��
	void Turn();				// ����n�_�����
	void Level(const bool up);		// ���x���̏���
	void NormalizeRot();		// �p�x�̐��K��
	void LoadFileGimmick(const char *Xfilename);

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetTrolley_Index(int index) { m_nIndex = index; }

	D3DXVECTOR3 GetPos() { return m_Pos; }
	int GetLevel() { return m_nLevel; }
	float GetRadius() { return m_fRadius; }

private:
	static const int m_nNum_GimmickParts = 13;		// �p�[�c��
	static const int m_nNum_GimmickKey = 5;			// �L�[�̍ő吔
	static const int Level_Down_Timer = 300;		// ���x���_�E���܂ł̎���

	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_TurnPos[CTrolley_Manager::Max_TurnPos];		// ����n�_
	D3DXVECTOR3 m_Start_Pos[CTrolley_Manager::Max_StartPos];	// �����ʒu
	D3DXVECTOR3 InitPos[m_nNum_GimmickParts];					// �p�[�c�̏����ʒu
	D3DXVECTOR3 InitRot[m_nNum_GimmickParts];					// �p�[�c�̏����p�x
	D3DXVECTOR3 m_Pos;			// �ʒu
	D3DXVECTOR3 m_Rot;			// �p�x
	D3DXVECTOR3 m_Rot_Dest;		// �ړI�̊p�x
	D3DXVECTOR3 m_Next_Vec;		// ���̖ړI�n�܂ł̃x�N�g��
	int m_nLevel;				// �g���b�R�̃��x��
	int m_nLevelDownCounter;	// ���x���_�E���܂ł̎���
	int m_nTurnNum;				// ����n�_�̐�
	int m_nStartNum;			// �X�^�[�g�n�_�̐�
	float m_fSpeed[CTrolley_Manager::MaxLevel];	// �ړ����x
	int m_nNextTurn_Index;		// ���̖ړI�n�̔ԍ�
	int m_nNextTurn_IndexInit;	// �ŏ��̖ړI�n�̔ԍ�
	int m_nTurn;				// �ǂ�����]��
	int m_nIndex;				// �ԍ�
	float m_fCp;				// �O��
	float m_fRadius;			// ���a
	bool m_bReturn;				// �����ʒu�ɖ߂�
	bool m_bOneTurn;			// ���������
	bool m_bBack;				// ��ނ��Ă邩

	CModel *m_pModel[m_nNum_GimmickParts];	// ���f���̏��

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
		KEY aKey[m_nNum_GimmickParts];
	};

	// ���[�V�����Z�b�g
	struct MOTION_SET
	{
		int nMaxKey;			// �L�[�̑���
		bool bLoop;				// ���[�v���邩
		bool bStop;				// ���[�V�������~�܂�����
		KEY_SET aKeySet[m_nNum_GimmickKey];
	};
	MOTION_SET m_MotionSet;
	int m_nCurrentMotion;		// ���݂̃��[�V�����ԍ�
	int m_nCountMotion;							// ���[�V�����J�E���^�[
	int m_nCurrentKey;							// ���݂̃L�[�ԍ�
	Trolley_Motion_Type m_Motion;				// ���[�V����
};

#endif // !_TROLLEY_MANAGER_H_
