//=============================================================================
//
// スコア
// Author : tanimoto kosuke
//
//=============================================================================
#include "score.h"
#include "application.h"
#include "texture.h"
#include "number.h"
#include <stdio.h>

CScore *CScore::m_pScore = nullptr;
//=====================================
//デフォルトコンストラクタ
//=====================================
CScore::CScore() : CObject2D(UI_PRIORITY_UI), m_nScore(0)
{
	
}

//=====================================
//デストラクタ
//=====================================
CScore::~CScore()
{

}

//=====================================
//初期化処理
//=====================================
void CScore::Init()
{
	CObject2D::Init();

	//外部ファイルで変数に値を代入したい枠
	m_fWidth = SCORE_WIDTH;
	m_fHeight = SCORE_HEIGHT;

	//オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);
}

//=====================================
//終了処理
//=====================================
void CScore::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CScore::Update()
{
	CObject2D::Update();

	//UV設定処理
	CObject2D::SetUV();
}

//=====================================
//描画処理
//=====================================
void CScore::Draw()
{
	CApplication::GetTexture()->TextureType(CTexture::TYPE_SCORE);

	CObject2D::Draw();
}

//=====================================
//点数の設定処理
//=====================================
void CScore::SetScore()
{
	//指定の桁の数値を抽出
	for (int nInd = 0; nInd < MAX_DIGIT; nInd++)
	{
		//計算(スコア % 10のべき乗 / ひとつ下の10のべき乗)
		int A = (int)pow(10.0f, (MAX_DIGIT - nInd ));
		int B = (int)pow(10.0f, (MAX_DIGIT - nInd - 1));
		m_apNumber[nInd]->SetNumber(m_nScore % A / B);
	}
}

//=====================================
//点数の加算処理
//=====================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	if (m_nScore > MAX_SCORE)
	{
		m_nScore = MAX_SCORE;
	}

	SetScore();
}

//=====================================
//スコア生成処理
//=====================================
CScore *CScore::Create(D3DXVECTOR3 pos)
{
	m_pScore = new CScore;

	if (m_pScore != nullptr)
	{
		m_pScore->Init();
		m_pScore->SetPos(pos);
		for (int nInd = 0; nInd < MAX_DIGIT; nInd++)
		{
			m_pScore->m_apNumber[nInd] = CNumber::Create(D3DXVECTOR3(pos.x - (MAX_DIGIT / 2 * 20 - 10) + (20 * nInd), pos.y + 10.0f, 0.0f), CNumber::TYPE_SCORE);
		}
		m_pScore->SetScore();
	}
	return m_pScore;
}

//=====================================
//ランキングの書き込み処理
//=====================================
void CScore::Save(void)
{
	int aRankingData[RANKING_NUM] = {};

	//+++++++++++++++++++++++++++++++++++++
	//ランキング読み込み
	//+++++++++++++++++++++++++++++++++++++
	FILE* pFileLoad = fopen(RNKING_FILE, "r");	//ファイルを開く

	if (pFileLoad != NULL)
	{//ファイルが開いた場合
		fscanf(pFileLoad, "%d", &aRankingData[0]);
	 //ランキングファイルから読み込み
		for (int nID = 0; nID < RANKING_NUM; nID++)
		{
			fscanf(pFileLoad, "%d", &aRankingData[nID]);
		}
	}

	//ファイルを閉じる
	fclose(pFileLoad);

	//*************************************
	//バブルソートの全体処理
	//*************************************
	int nMaxID = 0;	//ハイスコアのID
	if (m_nScore > aRankingData[RANKING_NUM - 1])
	{
		aRankingData[RANKING_NUM - 1] = m_nScore;

		int nMax = 0;
		//ランキングデータを更新
		for (int nCnt1 = 0; nCnt1 < RANKING_NUM - 1; nCnt1++)
		{//1つずつ配列の位置を上げる処理
			for (int nCnt2 = nCnt1 + 1; nCnt2 < RANKING_NUM; nCnt2++)
			{
				if (aRankingData[nCnt1] < aRankingData[nCnt2])
				{
					nMax = aRankingData[nCnt2];
					aRankingData[nCnt2] = aRankingData[nCnt1];
					aRankingData[nCnt1] = nMax;
				}
			}
		}

		for (int nCnt1 = 0; nCnt1 < RANKING_NUM; nCnt1++)
		{
			if (aRankingData[nCnt1] == m_nScore)
			{
				nMaxID = nCnt1 + 1;
				break;
			}
		}
	}

	//+++++++++++++++++++++++++++++++++++++
	//ランキング書き込み
	//+++++++++++++++++++++++++++++++++++++
	FILE* pFileSave = fopen(RNKING_FILE, "w");	//ファイルを開く

	if (pFileSave != NULL)
	{//ファイルが開いた場合
		fprintf(pFileSave, "%d\n", nMaxID);
	 //ランキングファイルに書き込み
		for (int nID = 0; nID < RANKING_NUM; nID++)
		{
			fprintf(pFileSave, "%d\n", aRankingData[nID]);
		}
	}
	else
	{
		printf("エラー : ファイルが開けませんでした\n");
	}

	//ファイルを閉じる
	fclose(pFileSave);

	//+++++++++++++++++++++++++++++++++++++
	//ランキング書き込み
	//+++++++++++++++++++++++++++++++++++++
	FILE* pFileThisScore = fopen(THIS_SCORE_FILE, "w");	//ファイルを開く

	if (pFileThisScore != NULL)
	{//ファイルが開いた場合
		fprintf(pFileThisScore, "%d\n", m_nScore);
	}
	else
	{
		printf("エラー : ファイルが開けませんでした\n");
	}

	//ファイルを閉じる
	fclose(pFileThisScore);
}