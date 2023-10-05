//==============================================================================================
//
// �g���b�R�}�l�[�W���[�@�@�@trolley_manager.h
// tutida ryousei
//
//==============================================================================================
#ifndef _TROLLEY_MANAGER_H_
#define	_TROLLEY_MANAGER_H_

#include"object.h"

class CGimmick_Model;

class CTrolley_Manager : public CObject
{
public:
	static const int Max_TurnPos = 10;	// ����n�_�̐�
	static const int Max_StartPos = 5;		// �����ʒu�̐�
	static const int MaxLevel = 4;			// �g���b�R�̍ő僌�x��

	CTrolley_Manager();
	~CTrolley_Manager();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TrolleyDeath();	// �g���b�R�̍폜

	static CTrolley_Manager *Create(int type, int index, const char *Xfilename);

	void SetMove(bool move) { m_bMove = move; }
	void SetEnd(bool end) { m_bEnd = end; }
	void SetFileName(const char *Xfilename) { m_FileName = Xfilename; }

	bool GetMove() { return m_bMove; }
	D3DXVECTOR3 GetTurn_Pos(int cnt) { return m_Turn_Pos[cnt]; }
	D3DXVECTOR3 GetStart_Pos(int cnt) { return m_Start_Pos[cnt]; }
	int GetStartTurn_Index() { return m_nStartTurn_Index; }
	int GetTurnNum() { return m_nTurnNum; }
	int GetStartNum() { return m_nStartNum; }
	int GetRandStart() { return m_nRand_StartPos; }
	float GetSpeed(int cnt) { return m_fSpeed[cnt]; }
	float GetRadius() { return m_fRadius; }
	CGimmick_Model *GetGimmick_Model() { return m_pGimmick_Model; }

private:
	void SetTrolley();		// �g���b�R�̐ݒ�
	void SetStartPos();		// �����ʒu�̐ݒ�

	void SetType(int type) { m_nType = type; }
	void SetIndex(int index) { m_nIndex = index; }

	D3DXVECTOR3 m_Pos;						// �ʒu
	D3DXVECTOR3 m_Start_Pos[Max_StartPos];	// �����ʒu
	D3DXVECTOR3 m_Turn_Pos[Max_TurnPos];	// ����n�_
	int m_nFirst_Index[Max_StartPos][2];	// �����ʒu���Ƃ̍ŏ��̏���n�_
	int m_nStartTurn_Index;		// �m�肵�������ʒu
	int m_nType;				// �^�C�v
	int m_nStartNum;			// �����ʒu�̐�
	int m_nTurnNum;				// ����n�_�̐�
	int m_nFirstNum;			// �ŏ��̖ړI�n
	int m_nRand_StartPos;		// �ŏ��̃X�^�[�g�n�_
	int m_nCoolTime;			// �N�[���^�C��
	int m_nCoolTime_Count;		// �N�[���^�C���̃J�E���g
	int m_nIndex;				// �g���b�R�̔ԍ�
	float m_fSpeed[MaxLevel];			// �ړ����x
	float m_fRadius;			// �����蔻��̔��a
	bool m_bMove;				// �ړ����Ă邩
	bool m_bEnd;				// �I�������
	CGimmick_Model *m_pGimmick_Model;			// �g���b�R�̏��
	const char *m_FileName;
};

#endif // !_TROLLEY_H_