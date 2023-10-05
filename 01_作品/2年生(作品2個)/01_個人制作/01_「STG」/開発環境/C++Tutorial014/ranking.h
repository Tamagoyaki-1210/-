//=============================================================================
//
// �����L���O
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�O���錾
//---------------------------
class CNumber;

//---------------------------
//�N���X
//---------------------------
class CRanking : public CObject2D
{
public:
	CRanking();
	~CRanking() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void HighScore();
	void SetScore(int nID);
	static CRanking *GetRanking() { return m_pRanking; }
	static CRanking *Create(D3DXVECTOR3 pos);
	void Load(void);
private:
	int m_naScore[RANKING_NUM];		//�X�R�A
	int m_nHighScore;
	int m_nThisNumber;				//����̃X�R�A
	float m_fWidth;					//������
	float m_fHeight;				//�����c
	float m_fBlinkCounter;			//�_�ŃJ�E���^�[
	float m_bBlink;					//�_�Ŕ���
	static CRanking *m_pRanking;
	CNumber *m_apNumber[RANKING_NUM][MAX_DIGIT];
};

#endif // !_RANKING_H_
