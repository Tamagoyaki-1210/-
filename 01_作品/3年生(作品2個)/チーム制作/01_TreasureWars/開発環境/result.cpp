//==============================================================================================
//
// リザルト　　　result.cpp
// tutida ryousei
//
//==============================================================================================
#include<time.h>
#include"result.h"
#include"input.h"
#include"application.h"
#include"fade.h"
#include"game.h"
#include"camera.h"
#include"3Dpolygon.h"
#include"Player.h"
#include"file.h"
#include"cpu.h"
#include"objectX.h"
#include"ore.h"
#include"result_obj.h"
#include"ranking.h"
#include"score.h"
#include"piller.h"
#include"logo.h"
#include"title_obj.h"
#include"entry.h"
#include"stage_select.h"

const float CResult::Moving_Piller[Ranking_Max] = { 425.0f, 325.0f, 225.0f, 125.0f };

// 柱の位置情報
const D3DXVECTOR3 CResult::Piller_Pos[Ranking_Max] = {
	D3DXVECTOR3(-510.0f, -700.0f, 0.0f),
	D3DXVECTOR3(-170.0f, -700.0f, 0.0f),
	D3DXVECTOR3(170.0f, -700.0f, 0.0f),
	D3DXVECTOR3(510.0f, -700.0f, 0.0f),
};

int CResult::m_nScore[Ranking_Max] = {};
bool CResult::m_bFlag = false;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CResult::CResult()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CResult::~CResult()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CResult::Init()
{
	// リザルト画面BGM
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_RESULT);

	// カメラのポインタ
	CCamera* pCamera = CApplication::GetCamera();

	pCamera->SetPosV(D3DXVECTOR3(0.0f, 330.0f, -1000.0f));
	pCamera->SetPosR(D3DXVECTOR3(0.0f, 330.0f, 0.0f));

	//起動時に一回だけ行うため初期化に書く	
	srand((unsigned int)time(nullptr));

	//==================================================
	// メンバ変数の初期化処理
	//==================================================
	//背景
	C3DPolygon *pPolygon = C3DPolygon::Create(D3DXVECTOR3(0.0f, 0.0f, 900.0f), D3DXVECTOR3(D3DXToRadian(-90), 0.0f, 0.0f), D3DXVECTOR2(2000.0f, 2000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	// 鉱石のモデル番号
	int nIndexOre = 0;

	//背景のテクスチャ設定
	switch (CStage_Select::GetStage_Number())
	{
	case 0:
		pPolygon->SetTextIndex(CTexture::TEXTURE_FLOOR);
		nIndexOre = CResultObj::ReadObject("Data/model/ore.x");
		break;
	case 1:
		pPolygon->SetTextIndex(CTexture::TEXTURE_JUNGLE);
		nIndexOre = CResultObj::ReadObject("Data/model/Collecting_Point/Stage_02/Fossil_01.x");
		break;
	case 2:
		pPolygon->SetTextIndex(CTexture::TEXTURE_MOON);
		nIndexOre = CResultObj::ReadObject("Data/model/Collecting_Point/Stage_03/Artificial_Satellite_01.x");
		break;
	default:
		break;
	}

	//鉱石の挙動
	for (int nCnt = 0; nCnt < RESULTORE_MAX; nCnt++)
	{
		CResultObj::Create(D3DXVECTOR3(0.0f, -495.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), nIndexOre);
	}

	//====================================================
	// 比較
	//====================================================
	Comparison();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CResult::Uninit()
{
	// リザルト画面BGMの終了
	CApplication::GetSound()->StopLabel(CSound::SOUND_LABEL_BGM_RESULT);

	// 終了フラグを偽にする
	m_bFlag = false;
}

//==============================================================================================
//更新処理
//==============================================================================================
void CResult::Update()
{
	// 終了フラグが設定されている場合
	if (m_bFlag == true)
	{
		CInput* pInput = CInput::GetKey();

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if ((pInput->Trigger(DIK_RETURN) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
				|| pInput->Trigger(JOYPAD_START, nCnt))
				&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CApplication::MODE_TITLE, 0.05f);
			}
		}
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CResult::Draw()
{

}

//==============================================================================
// 比較処理
//==============================================================================
void CResult::Comparison()
{
	//===================================
	// 変数宣言
	//===================================
	int no_1 = 0;
	int no_2 = 0;
	int no_3 = m_nScore[0];
	int min = m_nScore[0];
	int min2 = m_nScore[1];

	//===================================
	// 比較
	//===================================
	for (int i = 0; i < Ranking_Max; i++)
	{
		//最大値
		if (no_1 < m_nScore[i])
		{//スコアが0より大きかったら
			no_2 = no_1;				//二番目に大きい値格納変数に最大値を格納
			no_1 = m_nScore[i];			//最大値格納変数そのスコアを格納
		}
		//二番目に大きい数値
		else if (no_2 < m_nScore[i])
		{//スコアが二番目に大きい値より大きかったら
			no_2 = m_nScore[i];			//二番目に大きい値格納変数にそのスコアを格納
		}

		//最小値
		if (no_3 > m_nScore[i])
		{
			no_3 = m_nScore[i];
		}
	}

	//二番目に小さい数値
	for (int i = 1; i < 4; i++)
	{
		if (m_nScore[i] < min)
		{
			min2 = min;
			min = m_nScore[i];
		}
		else if (m_nScore[i] < min2)
		{
			min2 = m_nScore[i];
		}
	}

	//===================================
	// スコア格納
	//===================================
	const int nKeepScoreMax = no_1;		//最大値
	const int nKeepScoreMax2 = no_2;	//二番目に大きい値
	const int nKeepScoreMin = no_3;		//最小値
	const int nKeepScoreMin2 = min2;	//二番目に小さい値

	CRanking* pRanking = CRanking::Create({ -325.0f,650.0f,0.0f }, { 20.0f,35.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });

	int nMovingIndex = 0;	// ランキング用スコア
	const int nIndexPiller = CObjectX::ReadObject("Data/model/01_RankingPillar.x");

	//一位から四位までのスコア
	for (int nCnt = 0; nCnt < Ranking_Max; nCnt++)
	{
		if (m_nScore[nCnt] == nKeepScoreMax)
		{//最大値
			nMovingIndex = 0;
		}
		else if (m_nScore[nCnt] == nKeepScoreMax2)
		{//二番目に大きい値
			nMovingIndex = 1;
		}
		else if (m_nScore[nCnt] == nKeepScoreMin2)
		{//二番目に小さい値
			nMovingIndex = 2;
		}
		else if (m_nScore[nCnt] == nKeepScoreMin)
		{//最小値
			nMovingIndex = 3;
		}

		// 柱の生成
		CPiller::Create(Piller_Pos[nCnt], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), nIndexPiller, Moving_Piller[nMovingIndex], nCnt);
		CLogo::Create(D3DXVECTOR3(200.0f + (nCnt * 295.0f), 1300.0f, 0.0f), 50.0f, 50.0f, Moving_Piller[nMovingIndex]);
		pRanking->SetScore(m_nScore[nCnt], nCnt);
	}
}

//==============================================================================================
// 生成処理
//==============================================================================================
CResult *CResult::Create()
{
	CResult *pResult = nullptr;

	pResult = new CResult;

	if (pResult != nullptr)
	{
		pResult->Init();
	}

	return pResult;
}
