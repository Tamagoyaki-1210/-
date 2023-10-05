//=============================================================================
//
// score.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "score.h"


std::vector <CScore*> CScore::m_vScore;

//コンストラクタ
CScore::CScore()
{
	//メンバー変数をクリアする
	m_nScore = 0;				//スコア
	m_nPlayerIdx = 0;			//プレイヤーインデックス
}

//デストラクタ
CScore::~CScore()
{

}

//初期化処理
HRESULT CScore::Init(void)
{
	//初期化処理
	m_nScore = 50;					//スコア
	m_nPlayerIdx = -1;				//プレイヤーインデックス

	return S_OK;
}

//終了処理
void CScore::Uninit(void)
{

}

//スコアを足す処理
void CScore::AddScore(const int nScore)
{
	m_nScore += nScore;				//スコアを加算する

	if (m_nScore < 0)
	{//スコアが0以下になったら、0に戻す
		m_nScore = 0;
	}
}

//スコアを取得処理
const int CScore::GetScore(void)
{
	return m_nScore;
}



//=============================================================================
//
//								静的関数
//
//=============================================================================




//スコアを足す処理
void CScore::AddScore(const int nPlayerIdx, const int nScore)
{
	m_vScore.data()[nPlayerIdx]->AddScore(nScore);				//スコアを加算する

	int score = m_vScore.data()[nPlayerIdx]->GetScore();		//スコアの取得

	if (score < 0)
	{//スコアが0以下になったら、0に戻す
		m_vScore.data()[nPlayerIdx]->AddScore(-score);
	}
}

//スコアを取得処理
const int CScore::GetScore(const int nPlayerIdx)
{
	return m_vScore.data()[nPlayerIdx]->m_nScore;
}


//クリア処理
void CScore::Clear(void)
{
	//スコアのインスタンスを全部破棄する
	for (int nCnt = 0; nCnt < (int)m_vScore.size(); nCnt++)
	{
		m_vScore.data()[nCnt]->Uninit();
		delete m_vScore.data()[nCnt];
		m_vScore.data()[nCnt] = nullptr;
	}

	m_vScore.clear();
}

//生成処理
CScore* CScore::Create(const int nPlayerIdx)
{
	CScore* pScore = new CScore;			//インスタンスを生成する

	if (FAILED(pScore->Init()))
	{//初期化処理
		return nullptr;
	}

	pScore->m_nPlayerIdx = nPlayerIdx;			//プレイヤーインデックスの設定

	m_vScore.push_back(pScore);					//スコアを保存する
	
	return pScore;								//生成したインスタンスを返す
}