//=============================================================================
//
// ランキング
// Author : tanimoto kosuke
//
//=============================================================================
#include "ranking.h"
#include "rank.h"
#include "application.h"
#include "texture.h"
#include "number.h"
#include "message.h"
#include <stdio.h>

//---------------------------
//マクロ関数
//---------------------------
#define RANKING_WIDTH			(1120)
#define RANKING_HEIGHT			(630)
#define RANKING_BLINK_SPEED		(0.05f)

CRanking *CRanking::m_pRanking = nullptr;
//=====================================
//デフォルトコンストラクタ
//=====================================
CRanking::CRanking() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//デストラクタ
//=====================================
CRanking::~CRanking()
{

}

//=====================================
//初期化処理
//=====================================
void CRanking::Init()
{
	CObject2D::Init();

	for (int nID = 0; nID < RANKING_NUM; nID++)
	{
		m_naScore[nID] = 0;
	}
	Load();

	//ハイスコア更新時
	if (m_nHighScore != 0)
	{
		CSound::PlaySound(SOUND_LABEL_SE_HIGHSCORE);
	}
	//外部ファイルで変数に値を代入したい枠
	m_fWidth = RANKING_WIDTH;
	m_fHeight = RANKING_HEIGHT;
	m_fBlinkCounter = 0.0f;
	m_bBlink = false;

	//オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);
}

//=====================================
//終了処理
//=====================================
void CRanking::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CRanking::Update()
{
	CObject2D::Update();

	//UV設定処理
	CObject2D::SetUV();

	HighScore();
}

//=====================================
//描画処理
//=====================================
void CRanking::Draw()
{
	CApplication::GetTexture()->TextureType(CTexture::TYPE_RANKING);

	CObject2D::Draw();
}

//=====================================
//ハイスコア処理
//=====================================
void CRanking::HighScore()
{
	if (m_bBlink)
	{
		m_fBlinkCounter--;
	}
	else
	{
		m_fBlinkCounter++;
	}

	if (m_fBlinkCounter >= 1.0f / RANKING_BLINK_SPEED)
	{
		m_bBlink = true;
		m_fBlinkCounter = 1.0f / RANKING_BLINK_SPEED;
	}
	else if (m_fBlinkCounter <= 0.5f)
	{
		m_bBlink = false;
		m_fBlinkCounter = 0.5f;
	}

	if (m_nHighScore != 0)
	{
		for (int nInd = 0; nInd < MAX_DIGIT; nInd++)
		{
			m_apNumber[m_nHighScore - 1][nInd]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fBlinkCounter * RANKING_BLINK_SPEED));
		}
	}
}

//=====================================
//点数の設定処理
//=====================================
void CRanking::SetScore(int nID)
{
	//指定の桁の数値を抽出
	for (int nInd = 0; nInd < MAX_DIGIT; nInd++)
	{
		//計算(スコア % 10のべき乗 / ひとつ下の10のべき乗)
		int A = (int)pow(10.0f, (MAX_DIGIT - nInd));
		int B = (int)pow(10.0f, (MAX_DIGIT - nInd - 1));
		m_apNumber[nID][nInd]->SetNumber(m_naScore[nID] % A / B);
	}
}

//=====================================
//スコア生成処理
//=====================================
CRanking *CRanking::Create(D3DXVECTOR3 pos)
{
	m_pRanking = new CRanking;

	if (m_pRanking != nullptr)
	{
		m_pRanking->Init();
		m_pRanking->SetPos(pos);

		//全てのランキングナンバーを生成する
		for (int nID = 0; nID < RANKING_NUM; nID++)
		{
			for (int nInd = 0; nInd < MAX_DIGIT; nInd++)
			{
				m_pRanking->m_apNumber[nID][nInd] = CNumber::Create(pos, CNumber::TYPE_RANKING);

				float fWidth = m_pRanking->m_apNumber[nID][nInd]->GetWidth();
				float fHeight = m_pRanking->m_apNumber[nID][nInd]->GetHeight();

				m_pRanking->m_apNumber[nID][nInd]->SetPos(D3DXVECTOR3
				(pos.x - (m_pRanking->m_fWidth / 5) - (MAX_DIGIT / 2 * fWidth - fWidth / 2) + (fWidth * nInd) + (fWidth * (MAX_DIGIT + 2) * (nID / 5) * 3 / 2),
					pos.y - (m_pRanking->m_fHeight / 4) + ((fHeight * 5 / 4) * (nID % 5)), 0.0f));
			}
			m_pRanking->SetScore(nID);
			
			D3DXVECTOR3 posRank = m_pRanking->m_apNumber[nID][0]->GetPos() + D3DXVECTOR3(-COMMA_WIDTH, 0.0f, 0.0f);

			CRank::Create(posRank, nID);
		}

		//今回のスコア反映処理
		CMessage::Create(D3DXVECTOR3(SCREEN_WIDTH / 3, SCREEN_HEIGHT - 90, 0.0f), CMessage::TYPE_THISNUMBER);
		for (int nInd = 0; nInd < MAX_DIGIT; nInd++)
		{
			//計算(スコア % 10のべき乗 / ひとつ下の10のべき乗)
			int A = (int)pow(10.0f, (MAX_DIGIT - nInd));
			int B = (int)pow(10.0f, (MAX_DIGIT - nInd - 1));
			CNumber *pNumber = CNumber::Create(D3DXVECTOR3(SCREEN_WIDTH / 2+ (m_pRanking->m_apNumber[0][0]->GetWidth() * (nInd + 1)), SCREEN_HEIGHT - 90, 0.0f), CNumber::TYPE_RANKING);
			pNumber->SetNumber(m_pRanking->m_nThisNumber % A / B);
		}
	}
	return m_pRanking;
}

//=====================================
//ランキングの読み込み処理
//=====================================
void CRanking::Load(void)
{
	FILE* pFile = fopen(RNKING_FILE, "r");	//ファイルを開く

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%d", &m_nHighScore);
	 //ランキングファイルから読み込み
		for (int nID = 0; nID < RANKING_NUM; nID++)
		{
			fscanf(pFile, "%d", &m_naScore[nID]);
		}
	}
	else
	{
		printf("エラー : ファイルが開けませんでした\n");
	}
	//ファイルを閉じる
	fclose(pFile);

	//現在スコア読み込み
	FILE* pFileThisScore = fopen(THIS_SCORE_FILE, "r");

	if (pFileThisScore != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%d", &m_nThisNumber);
	}
	else
	{
		printf("エラー : ファイルが開けませんでした\n");
	}
	//ファイルを閉じる
	fclose(pFileThisScore);
}
