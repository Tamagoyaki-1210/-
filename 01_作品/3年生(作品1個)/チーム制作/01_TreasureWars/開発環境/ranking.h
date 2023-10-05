//==============================================================================================
//
// ランキング　　　ranking.h
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
		int m_nScore;			// スコア
		int m_RankingScore;		// ランキング用スコア
		int m_nIndex;			// プレイヤー番号
		int m_nRank;			// 順位
	};

	CRanking::Result *GetResult() { return m_Result; }

private:
	static const int Result_Score_RoolSpeed = 200;
	static const int m_nNum_Ranking = 4;
	D3DXVECTOR3 m_Pos;					// 位置
	D3DXVECTOR3 m_Size;					// サイズ
	D3DXCOLOR m_Col;					// 色
	int m_aRankingData[m_nNum_Ranking];	// ランキング
	CScore *m_pScore[m_nNum_Ranking];	// スコアの情報
	CNumber *m_pNumber[m_nNum_Ranking];	// プレイヤー番号の数字
	CNumber *m_pRank[m_nNum_Ranking];	// 順位
	Result m_Result[m_nNum_Ranking];	// ランキング

	int ScoreCount;

	float m_Alpha;
};

#endif // !_RANKING_H_
