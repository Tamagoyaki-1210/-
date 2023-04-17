//=============================================================================
//
// �X�e�[�W
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

//---------------------------
//�O���錾
//---------------------------
class CPlayer;
class CBG;

enum Stage_Mode
{
	Stage_1 = 0,	//�X�e�[�W1
	Stage_2,		//�X�e�[�W2
	Stage_3,		//�X�e�[�W3
	Stage_4,		//�X�e�[�W4
	Stage_Boss,		//�{�X�X�e�[�W
	Stage_MAX,		//�ő�l
};

//---------------------------
//�N���X�錾
//---------------------------
class CStage
{
public:
	CStage();
	~CStage();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void GameManager();
	void ArrayStage();
	void ChangeStage();
	void BlinkStage();

	static int GetLoop() { return m_nLoop; }
	static int GetGameTimer() { return m_nGameTimer; }		// �Q�[���o�ߎ����擾
	static bool GetClear() { return m_bStageClear; }
	static bool GetBomm() { return m_bBomm; }
	static Stage_Mode GetStageMode() { return m_mode; }		//�X�e�[�W���[�h�擾
private:
	int m_nBlinkCounter;
	int m_nDirectLoop;			//�����_
	bool m_bWall;
	bool m_bBlink;				//�_�Ŕ���
	bool m_bChangeColor;
	static int m_nLoop;			//���[�v��
	static bool m_bBomm;		//�{������
	static bool m_bStageClear;	//�X�e�[�W�N���A����
	static int m_nGameTimer;
	static Stage_Mode m_mode;
	static Stage_Mode m_modeNext;
	static CBG *m_pBG;
};

#endif // !_STAGE_H_
