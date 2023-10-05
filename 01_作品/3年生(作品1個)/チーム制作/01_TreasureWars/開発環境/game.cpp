//==============================================================================================
//
// ゲーム　　　game.cpp
// tutida ryousei
//
//==============================================================================================
#include"game.h"
#include"application.h"
#include"fade.h"
#include"camera.h"
#include"map.h"
#include"objectX.h"
#include"score.h"
#include"time.h"
#include"countdown.h"
#include"skybox.h"
#include"oremanager.h"
#include"shop.h"
#include"ghost.h"
#include"pteranodon.h"
#include"alien.h"
#include "area.h"
#include"charmanager.h"
#include"input.h"
#include"fever_time.h"
#include"enemymanager.h"
#include"enemyspawner.h"
#include"scoreitemmanager.h"
#include"itemmanager.h"
#include"stage_select.h"
#include"fontString.h"

CMap *CGame::m_pMap = nullptr;
CTime *CGame::m_pTime = nullptr;
CShop *CGame::m_pShop[] = {};
COreManager *CGame::m_pOreManager = nullptr;
CEnemyManager *CGame::m_pEnemyManager = nullptr;
CScoreItemManager *CGame::m_pScoreItemManager = nullptr;
CItemManager *CGame::m_pItemManager = nullptr;
CEnemySpawner *CGame::m_pEnemySpawner = nullptr;
CFever_Time *CGame::m_pFever_Time = nullptr;
CArea *CGame::m_pArea = nullptr;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CGame::CGame() : m_nEnemySpawnerCounter(0)
{
	m_nTop_Score = 0;	// 最大スコアの初期化
	m_nTopCameraNum = -1;//順位確認
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CGame::~CGame()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CGame::Init()
{
	// 敵キャラマネージャーの生成
	m_pEnemyManager = new CEnemyManager;

	// スコアアイテムマネージャーの生成
	m_pScoreItemManager = new CScoreItemManager;

	// アイテムマネージャーの生成
	m_pItemManager = new CItemManager;

	// 選んだステージの番号
	m_nStage_Number = CStage_Select::GetStage_Number();

	// カメラのポインタ
	CCamera* pCamera = CApplication::GetCamera();

	// 視点、注視点の設定
	pCamera->SetPosV({ 800.0f, 1200.0f, -250.0f });
	pCamera->SetPosR({ 800.0f, 200.0f, 500.0f });

	// 生成
	m_pMap = new CMap;						// マップ
	m_pTime = CTime::Create();				// タイムの生成

	// 全てのキャラクターの生成
	CApplication::GetCharManager()->SetAllChara(m_nStage_Number);

	// 質屋の生成
	m_pShop[0] = CShop::Create({ 130.0f,0.0f,1470.0f }, { 0.0f,0.0f,0.0f });
	m_pShop[1] = CShop::Create({ 1470.0f,0.0f,1470.0f }, { 0.0f,0.0f,0.0f });
	m_pShop[2] = CShop::Create({ 130.0f,0.0f,130.0f }, { 0.0f,D3DX_PI,0.0f });
	m_pShop[3] = CShop::Create({ 1470.0f,0.0f,130.0f }, { 0.0f,D3DX_PI,0.0f });

	// スカイボックスの生成
	CSkyBox::Create(D3DXVECTOR3(800.0f, 0.0f, 800.0f), m_nStage_Number);

	// 鉱石の生成
	m_pOreManager = COreManager::Create(m_nStage_Number);

	// マップの初期化
	m_pMap->Init(m_nStage_Number);

	//　エリアの生成
	m_pArea = CArea::Create();

	// 敵キャラスポナーの生成
	m_pEnemySpawner = CEnemySpawner::Create({ 800.0f, 0.0f, 800.0f }, CStage_Select::GetStage_Number());

	// ステージ毎の設定
	switch (CStage_Select::GetStage_Number())
	{
	case CStage_Select::STAGE_1:
		// ドロップアイテムのモデル情報を読み込む
		for (int nCnt = 0; nCnt < CScoreItem::MAX_SCOREITEM_MODEL; nCnt++)
		{
			CObjectX::ReadObject(CScoreItem::STAGE_SCOREITEM_01[nCnt]);
		}
		// ゴーストのモデルの読み込み
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Ghost/Tresure/Ghost.x");

		// ゲーム画面BGM1
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME1);
		break;
	case CStage_Select::STAGE_2:
		// ドロップアイテムのモデル情報を読み込む
		for (int nCnt = 0; nCnt < CScoreItem::MAX_SCOREITEM_MODEL; nCnt++)
		{
			CObjectX::ReadObject(CScoreItem::STAGE_SCOREITEM_02[nCnt]);
		}
		// プテラノドンのモデルの読み込み
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/body.x");		// [0]体
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/head.x");		// [1]頭
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/arm_left.x");	// [2]左腕
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/arm_right.x");	// [3]右腕
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Pteranodon/Dino/leg.x");		// [4]足

		// ゲーム画面BGM2
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME2);
		break;
	case CStage_Select::STAGE_3:
		// ドロップアイテムのモデル情報を読み込む
		for (int nCnt = 0; nCnt < CScoreItem::MAX_SCOREITEM_MODEL; nCnt++)
		{
			CObjectX::ReadObject(CScoreItem::STAGE_SCOREITEM_03[nCnt]);
		}
		// エイリアンのモデルの読み込み
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Alien/Alien/ufo.x");		// [0]UFO
		CObjectX::ReadObject("Data/model/Motion_Enemy/Motion_Alien/Alien/alien.x");		// [1]エイリアン

		// ゲーム画面BGM3
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME3);
		break;
	default:
		break;
	}

	// ゲームの開始条件が整ったらカウントダウンの生成
	CCountdown::Create();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CGame::Uninit()
{
	// エリアの終了処理
	if (m_pArea != nullptr)
	{
		m_pArea->Uninit();
		m_pArea = nullptr;
	}

	// マップの終了処理
	if (m_pMap != nullptr)
	{
		m_pMap->Uninit();
		delete m_pMap;
		m_pMap = nullptr;
	}

	// タイマーの終了処理
	if (m_pTime != nullptr)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = nullptr;
	}

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		// タイマーの終了処理
		if (m_pShop[nCnt] != nullptr)
		{
			m_pShop[nCnt]->Uninit();
			m_pShop[nCnt] = nullptr;
		}
	}

	// 鉱石マネージャーの終了処理
	if (m_pOreManager != nullptr)
	{
		m_pOreManager->Uninit();
		m_pOreManager = nullptr;
	}

	// 敵キャラマネージャーの終了処理
	if (m_pEnemyManager != nullptr)
	{
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}

	// 敵キャラスポナーの終了処理
	if (m_pEnemySpawner != nullptr)
	{
		m_pEnemySpawner->Uninit();
		m_pEnemySpawner = nullptr;
	}

	// スコアアイテムマネージャーの終了処理
	if (m_pScoreItemManager != nullptr)
	{
		delete m_pScoreItemManager;
		m_pScoreItemManager = nullptr;
	}

	// アイテムマネージャーの終了処理
	if (m_pItemManager != nullptr)
	{
		delete m_pItemManager;
		m_pItemManager = nullptr;
	}

	// フィーバータイムの終了処理
	if (m_pFever_Time != nullptr)
	{
		m_pFever_Time->Uninit();
		m_pFever_Time = nullptr;
	}

	// ポーズを止める
	CApplication::SetPauce(false);

	// ステージ毎にBGMを終了
	switch (CStage_Select::GetStage_Number())
	{
	case CStage_Select::STAGE_1:
		// ゲーム画面BGM1の終了
		CApplication::GetSound()->StopLabel(CSound::SOUND_LABEL_BGM_GAME1);
		break;
	case CStage_Select::STAGE_2:
		// ゲーム画面BGM2の終了
		CApplication::GetSound()->StopLabel(CSound::SOUND_LABEL_BGM_GAME2);
		break;
	case CStage_Select::STAGE_3:
		// ゲーム画面BGM3の終了
		CApplication::GetSound()->StopLabel(CSound::SOUND_LABEL_BGM_GAME3);
		break;
	default:
		break;
	}
}

//==============================================================================================
//更新処理
//==============================================================================================
void CGame::Update()
{
	// マップの更新
	m_pMap->Update();

	// タイマーの更新
	m_pTime->Update();

	//　エリア変動用のプレイヤーの順位の保存
	m_pArea->SetTopCharaNum(m_nTopCameraNum);

	// ゲームが開始されている場合
	if (CApplication::GetGameStart() == true)
	{
		// キャラの順位処理
		CharaRanking();

		// 敵キャラ出現処理
		EnemySpawn();

		// フィーバータイムの設定
		SetFeverTime();
	}

#ifdef _DEBUG
	CInput* pInput = CInput::GetKey();

	// リザルトに飛ぶ処理(デバッグ用)
	if (pInput->Trigger(DIK_F1))
	{
		CFade::SetFade(CApplication::MODE_RESULT, 0.05f);
	}
#endif
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CGame::Draw()
{
	// マップの描画処理
	m_pMap->Draw();
}

//==============================================================================================
// キャラの順位処理
//==============================================================================================
void CGame::CharaRanking()
{
	// 全キャラのポインタの格納先
	CCharacter* pChara[CCharManager::Max_Character] = {};
	int nScore[CCharManager::Max_Character] = {};

	int nLow_Score = 1;						// 一番低いスコア(100000000 - 1 = 99999999)
	for (int nCnt = 0; nCnt < SCORE_DIGIT; nLow_Score *= 10, nCnt++) {}
	nLow_Score--;
	int nTopScore = 0;

	// 全キャラのスコアを比較する
	for (int index = 0; index < CCharManager::Max_Character; index++)
	{
		// 各キャラのポインタを格納
		pChara[index] = CApplication::GetCharManager()->GetChara(index);

		// キャラのスコア
		 nScore[index] = pChara[index]->GetScore()->GetScore();

		// スコアが最大値より大きい場合
		if(m_nTop_Score < nScore[index])
		{
			// 最大値の更新
			m_nTop_Score = nScore[index];

			//同位がいるかどうか
			if (nTopScore != m_nTop_Score)
			{
				nTopScore = m_nTop_Score;

				//順位の保存
				m_nTopCameraNum = index;
			}
			else
			{
				//順位の保存
				m_nTopCameraNum = -1;
			}
			
		}
		// スコアが最大値より小さい場合 又は 最小値が更新されていない場合
		else if (nLow_Score > nScore[index])
		{
			// 最小値の更新
			nLow_Score = nScore[index];
		}
	}

	// 最大スコアが一度でも更新された場合
	if (m_nTop_Score != 0)
	{
		// 全キャラのスコアで王冠の表示を切り替える
		for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
		{
			// フィーバータイムではない場合
			if (m_pFever_Time == nullptr)
			{
				// 最大値か最小値に一致した場合
				pChara[nCnt]->SetCrown(nScore[nCnt] == m_nTop_Score);
			}
		}
	}
}

//==============================================================================================
// 敵キャラの生成処理
//==============================================================================================
void CGame::EnemySpawn()
{
	// 敵キャラが誰も使用されていない場合
	if (m_pEnemySpawner == nullptr)
	{
		if (m_nEnemySpawnerCounter++ >= EnemySpawner_Timer)
		{
			m_pEnemySpawner = CEnemySpawner::Create({ 800.0f, 0.0f, 800.0f }, CStage_Select::GetStage_Number());

			m_nEnemySpawnerCounter = 0;
		}
	}
}

//==============================================================================================
// フィーバータイムの設定
//==============================================================================================
void CGame::SetFeverTime()
{
	// 残り時間がフィーバータイム以下になった場合 && フィーバータイムではない場合
	if (m_pTime->GetTime() <= FeverStart_Count && m_pFever_Time == nullptr)
	{
		// フィーバータイム
		m_pFever_Time = CFever_Time::Create(FeverStart_Count, GemCreate_Count);

		// キャラクターの情報を読み込む
		for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
		{
			CCharacter* pChara = CApplication::GetCharManager()->GetChara(nCnt);

			// フィーバータイム時に王冠とスコアを非表示にする処理
			pChara->SetCrown(false);
			pChara->GetScore()->SetCompulsDispray(false);

			// フィーバー用のアイコンを生成
			pChara->GetScore()->SetFiver();
		}
	}
}

//==============================================================================================
// 敵キャラスポナーを消す処理
//==============================================================================================
void CGame::EnemySpawnerDeath()
{
	m_pEnemySpawner->Uninit();
	m_pEnemySpawner = nullptr;
}

//==============================================================================================
// 生成処理
//==============================================================================================
CGame *CGame::Create()
{
	CGame *pGame = nullptr;

	pGame = new CGame;

	if (pGame != nullptr)
	{
		pGame->Init();
	}

	return pGame;
}