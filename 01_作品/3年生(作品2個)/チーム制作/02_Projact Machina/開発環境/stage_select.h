//==============================================================================================
//
// �X�e�[�W�Z���N�g�@�@�@stage_select.h
// tutida ryousei
//
//==============================================================================================
#ifndef _STAGE_SERECT_H_
#define	_STAGE_SERECT_H_

#include"mode.h"

class CObject2D;

class CStage_Select : public CMode
{
public:
	CStage_Select();
	~CStage_Select();

	enum STAGE
	{
		STAGE_1 = 0,
		STAGE_2,
		STAGE_3,
		STAGE_TUTORIAL,
		STAGE_MAX,
		STAGE_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	static CStage_Select* Create();
	void Select();			// �X�e�[�W�I��
	void Stage_Start();		// �X�e�[�W�J�n����
	void Stage_Slide();		// �X�e�[�W���X���C�h������
	void Select_Arrow();	// �I���̖��
	void Min_Stage();		// �X�e�[�W(��)�̐ݒ�
	void Arrow_Flash();		// ���̓_��

	static int GetStage_Number() { return m_nStage_Number; }

private:
	static const int m_nScreen_Width = 1280;
	static const int m_nScreen_Height = 720;

	CObject2D *m_pStage[2][STAGE_MAX];		// �I������X�e�[�W
	CObject2D *m_pMin_Stage[2][STAGE_MAX];	// �I������X�e�[�W(��)
	CObject2D *m_pFloor;					// �w�i
	CObject2D *m_pSelect_Arrow[2];			// ���
	D3DXVECTOR3 m_StagePos[STAGE_MAX];		// �X�e�[�W�̈ʒu
	D3DXVECTOR3 m_StagePosDest[STAGE_MAX];	// �X�e�[�W�̖ړI�̈ʒu
	D3DXVECTOR3 m_Select_Arrow[2];			// ���
	static int m_nStage_Number;				// �X�e�[�W�̔ԍ�
	int m_nStartCount;						// �n�܂�܂ł̃J�E���g
	bool m_bSelect;							// �X�e�[�W�����߂���
	float m_fAlpha;							// �A���t�@�l
	bool m_Flash;							// �_�ł��Ă邩
	float m_fRed;
	bool m_bMin;
};

#endif // !_STAGE_SERECT_H_
