//=============================================================================
//
// �X�R�A
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�O���錾
//---------------------------
class CNumber;

//---------------------------
//�}�N���֐�
//---------------------------
#define SCORE_WIDTH		(240)
#define SCORE_HEIGHT	(80)

//---------------------------
//�N���X
//---------------------------
class CScore : public CObject2D
{
public:
	CScore();
	~CScore() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetScore();
	void AddScore(int nValue);
	static CScore *GetScore() { return m_pScore; }
	static CScore *Create(D3DXVECTOR3 pos);
	void Save(void);
private:
	int m_nScore;		//�X�R�A
	float m_fWidth;		//������
	float m_fHeight;	//�����c
	static CScore *m_pScore;
	CNumber *m_apNumber[MAX_DIGIT];
};

#endif // !_SCORE_H_
