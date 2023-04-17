//=============================================================================
//
// �^�C�g��
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//---------------------------
//�O���錾
//---------------------------
class CBG;

enum Title_Mode
{
	Title_1 = 0,	//�X�e�[�W1
	Title_2,		//�X�e�[�W2
	Title_3,		//�X�e�[�W3
	Title_4,		//�X�e�[�W4
	Title_MAX,		//�ő�l
};

//---------------------------
//�N���X�錾
//---------------------------
class CTitle
{
public:
	CTitle();
	~CTitle();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ArrayStage();
	void ChangeStage();
	void BlinkStage();
private:
	int m_nLoop;			//���[�v��
	int m_nModeCounter;		//���[�h��������
	int m_nBlinkCounter;
	int m_nGameTimer;
	bool m_bBlink;			//�_�Ŕ���
	bool m_bChangeColor;
	static Title_Mode m_mode;
	static Title_Mode m_modeNext;
	static CBG *m_pBG;
};

#endif // !_TITLE_H_
