//==============================================================================================
//
// ���U���g�@�@�@result.h
// tutida ryousei
//
//==============================================================================================
#ifndef _RESULT_H_
#define	_RESULT_H_

#include"mode.h"
#include"charmanager.h"

class CResultObj;

class CResult : public CMode
{
	static const int			RESULTORE_MAX = 20;				//�����������̍ő吔 
	static const int			Ranking_Max = 4;
	static const D3DXVECTOR3	Piller_Pos[Ranking_Max];
public:
	static const float			Moving_Piller[Ranking_Max];

	CResult();
	~CResult();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Comparison();			// ��r����

	static CResult *Create();

	static void SetFlag(const bool flag) { m_bFlag = flag; }
	static void SetScore(int score, int index) { m_nScore[index] = score; }

	static const bool GetFlag() { return m_bFlag; }
	static int *GetScore() { return m_nScore; }

private:

	static bool m_bFlag;				// �I������
	static int m_nScore[Ranking_Max];	// �X�R�A

	int m_TitleObjModel;				// �z�΂̃��f��
	int m_ModelIdx;						// �����L���O���̃��f��
};

#endif // !_GAME_H_
