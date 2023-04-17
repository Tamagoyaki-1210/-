//=============================================================================
//
// ランキング
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//前方宣言
//---------------------------
class CNumber;

//---------------------------
//クラス
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
	int m_naScore[RANKING_NUM];		//スコア
	int m_nHighScore;
	int m_nThisNumber;				//今回のスコア
	float m_fWidth;					//長さ横
	float m_fHeight;				//長さ縦
	float m_fBlinkCounter;			//点滅カウンター
	float m_bBlink;					//点滅判定
	static CRanking *m_pRanking;
	CNumber *m_apNumber[RANKING_NUM][MAX_DIGIT];
};

#endif // !_RANKING_H_
