//==============================================================================================
//
// ランキング　　　ranking.cpp
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include<stdio.h>
#include"ranking.h"
#include "result.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CRanking::CRanking()
{
	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		m_pScore[nCnt] = nullptr;
		m_Result[nCnt].m_nScore = 0;		// スコアの初期値
		m_Result[nCnt].m_nRank = nCnt + 1;	// ランクの初期値
	}
}

//==============================================================================================
// デスラクタ
//==============================================================================================
CRanking::~CRanking()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CRanking::Init()
{
	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		// スコア
		m_pScore[nCnt] = CScore::Create({ 330.0f * nCnt, m_Pos.y, m_Pos.z }, m_Size, 25.0f);
		m_pScore[nCnt]->SetRollSpeed(Result_Score_RoolSpeed);
	}

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CRanking::Uninit()
{
	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		// スコア
		m_pScore[nCnt]->Uninit();
		m_pScore[nCnt] = nullptr;
	}
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CRanking::Update()
{
	// スコアの表示
	DisplayRanking();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CRanking::Draw()
{
}

//==============================================================================================
// 生成処理
//==============================================================================================
CRanking *CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CRanking *pRanking = new CRanking;

	if (pRanking != nullptr)
	{
		pRanking->SetSize(size);
		pRanking->SetCol(col);
		pRanking->SetPos(pos);
		pRanking->Init();
	}

	return pRanking;
}

//==============================================================================================
// スコアの設定
//==============================================================================================
void CRanking::SetScore(int score, int cnt)
{
	// スコアの代入
	m_pScore[cnt]->SetScore(score);

	// スコアの並び替え
	//CompareScore(m_pScore[cnt]->GetScore(), cnt);

	m_Result[cnt].m_nScore = score;

	
}

//==============================================================================================
// スコアを並び変える
//==============================================================================================
void CRanking::CompareScore(int score, int cnt)
{
	// スコアと番号の格納先
	int nSave_Score = 0;
	int nSave_Index = 0;

	if (score >= m_Result[m_nNum_Ranking - 1].m_nScore)
	{
		if (score != -1)
		{
			// スコアの代入
			m_Result[m_nNum_Ranking - 1].m_nScore = score;
		}

		// プレイヤー番号の代入
		m_Result[m_nNum_Ranking - 1].m_nIndex = cnt;
	}

	for (int nCnt = 0; nCnt < (m_nNum_Ranking - 1); nCnt++)
	{
		for (int nCnt2 = (nCnt + 1); nCnt2 < m_nNum_Ranking; nCnt2++)
		{
			if (m_Result[nCnt].m_nScore < m_Result[nCnt2].m_nScore)
			{
				// スコアの入れ替え
				nSave_Score = m_Result[nCnt].m_nScore;
				m_Result[nCnt].m_nScore = m_Result[nCnt2].m_nScore;
				m_Result[nCnt2].m_nScore = nSave_Score;

				// プレイヤー番号の入れ替え
				nSave_Score = m_Result[nCnt].m_nIndex;
				m_Result[nCnt].m_nIndex = m_Result[nCnt2].m_nIndex;
				m_Result[nCnt2].m_nIndex = nSave_Score;
			}
		}
	}

	// 同率の処理用
	int nContinue = -1;

	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		if (nCnt <= nContinue)
		{
			continue;
		}

		for (int nCnt2 = (nCnt + 1); nCnt2 < m_nNum_Ranking; nCnt2++)
		{
			if (m_Result[nCnt].m_nScore == m_Result[nCnt2].m_nScore)
			{
				// 同率の場合同じ順位にする
				m_Result[nCnt].m_nRank = nCnt + 1;
				m_Result[nCnt2].m_nRank = nCnt + 1;

				// どの順位まで同率になっているか
				nContinue = nCnt2 - 1;
			}
		}
	}
}

//==============================================================================================
// スコアの表示
//==============================================================================================
void CRanking::DisplayRanking()
{
	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		if (m_pScore[nCnt] != nullptr)
		{
			// スコアの表示
			m_pScore[nCnt]->SetScore(m_Result[nCnt].m_nScore);
		}
		if (m_pNumber[nCnt] != nullptr)
		{
			// テクスチャ座標の設定
			m_pNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * (m_Result[nCnt].m_nIndex + 1) + 0.1f, (m_Result[nCnt].m_nIndex + 1) * 0.1f);
		}
		if (m_pRank[nCnt] != nullptr)
		{
			// テクスチャ座標の設定
			m_pRank[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_Result[nCnt].m_nRank + 0.1f, m_Result[nCnt].m_nRank * 0.1f);
		}
		if (m_pScore[nCnt] != nullptr)
		{
			bool  f = CResult::GetFlag();

			// ランキング画面でのスコアの色
			if (f == true)
			{
				m_Alpha += 0.01f;

				m_pScore[0]->SetCol(1.0f, 0.0f, 0.0f, m_Alpha);
				m_pScore[1]->SetCol(0.0f, 0.0f, 1.0f, m_Alpha);
				m_pScore[2]->SetCol(0.0f, 1.0f, 0.0f, m_Alpha);
				m_pScore[3]->SetCol(1.0f, 1.0f, 0.0f, m_Alpha);
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					m_pScore[i]->SetCol(0.0f, 0.0f, 0.0f, 0.0f);
				}
			}
		}
	}
}
