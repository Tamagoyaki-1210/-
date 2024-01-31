//==============================================================================================
//
// �����L���O�@�@�@ranking.h
// tutida ryousei
//
//==============================================================================================
#ifndef _RANKING_H_
#define	_RANKING_H_

#include"score.h"
#include"number.h"

class CPiller;

class CRanking : CObject
{
public:
	CRanking();
	~CRanking();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CRanking *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);
	void CompareScore(int score, int cnt);
	void DisplayRanking();

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetScore(int score, int cnt);
	void SetSize(D3DXVECTOR3 size) { m_Size.x = size.x; m_Size.y = size.y; }
	void SetCol(D3DXCOLOR col) { m_Col = col; }

	struct Result
	{
		int m_nScore;			// �X�R�A
		int m_RankingScore;		// �����L���O�p�X�R�A
		int m_nIndex;			// �v���C���[�ԍ�
		int m_nRank;			// ����
	};

	CRanking::Result *GetResult() { return m_Result; }

private:
	static const int Result_Score_RoolSpeed = 200;
	static const int m_nNum_Ranking = 4;
	D3DXVECTOR3 m_Pos;					// �ʒu
	D3DXVECTOR3 m_Size;					// �T�C�Y
	D3DXCOLOR m_Col;					// �F
	int m_aRankingData[m_nNum_Ranking];	// �����L���O
	CScore *m_pScore[m_nNum_Ranking];	// �X�R�A�̏��
	CNumber *m_pNumber[m_nNum_Ranking];	// �v���C���[�ԍ��̐���
	CNumber *m_pRank[m_nNum_Ranking];	// ����
	Result m_Result[m_nNum_Ranking];	// �����L���O

	int ScoreCount;

	float m_Alpha;
};

#endif // !_RANKING_H_
