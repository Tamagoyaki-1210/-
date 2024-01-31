//==============================================================================================
//
// リザルト　　　result.cpp
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#include "result.h"
#include "input.h"
#include "application.h"
#include "fade.h"
#include "camera.h"
#include "fontString.h"
#include "objectX.h"
#include "halfsphere.h"
#include "char_select.h"
#include "confirmation_window.h"
#include "game.h"
#include "menu.h"
#include "title_menu.h"
#include "player_manager.h"
#include "player_parameter.h"
#include "weapon_parameter.h"
#include "score.h"
#include "pause.h"
#include "parts.h"
#include "time.h"
#include "number.h"
#include "mob.h"
#include "sound.h"
#include "texture.h"

//==============================================================================================
// 静的メンバ変数宣言
//==============================================================================================
//CPlayer_Parameter* CResult::m_pPlayerParameter = nullptr;
CPlayerManager* CResult::m_pPlayerManager = nullptr;

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

	//=======================================================
	// メンバ変数の初期化
	//=======================================================
	m_move = D3DXVECTOR3(4.25, 0.1f, 6.25f);
	m_pos = D3DXVECTOR3(-200.0f, 250.0f, 525.0f);
	m_bGetFlag = false;
	m_bCreateFlag = false;
	m_MoveCount = 120;
	m_PlayerIndex = 0;
	m_nZeroCountDeathCount = 0;
	m_nZeroCountTotalDamage = 0;

	// カメラのポインタ
	CCamera* pCamera = CApplication::GetCamera();
	pCamera->SetPosV({ 0.0f, 4.0f,-3.0f });
	pCamera->SetPosR({ 0.0f, 0.0f, 3.0f });

	// 全てのモデルパーツの読み込み
	CApplication::GetPartsFile()->LoadAllFile();

	// 全てのモデルパーツの読み込み
	CApplication::GetMotion()->LoadAllFile();

	//// プレイヤーパラメーターの生成
	//m_pPlayerParameter = new CPlayer_Parameter;
	//m_pPlayerParameter->Init();

	// 武器パラメーターの生成
	m_pWeaponParameter = new CWeapon_Parameter;
	m_pWeaponParameter->Init();

	// プレイヤーマネージャーの生成
	m_pPlayerManager = CPlayerManager::Create();	

	// プレイヤーの生成(テスト)
	m_pPlayerManager->SetPlayer({ m_pos.x, m_pos.y, m_pos.z }, CPlayerManager::TYPE_PC, m_PlayerIndex);

	// ハーフスフィアの生成
	m_pHalfSphere = CHalfSphere::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), D3DXVECTOR3(2500.0f, 2500.0f, 2500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CHalfSphere::SPHERE_UP);
	m_pHalfSphere->LoadTexture("Data/texture/map.png");

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		//ナンバーの初期化
		m_apTotalDamage[nCnt] = nullptr;
	}
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		//ナンバーの初期化
		m_apDeathCount[nCnt] = nullptr;
	}

	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_RESULT);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CResult::Uninit()
{
	//// プレイヤーパラメーターの破棄
	//if (m_pPlayerParameter != nullptr)
	//{
	//	m_pPlayerParameter->Uninit();
	//	delete m_pPlayerParameter;
	//	m_pPlayerParameter = nullptr;
	//}

	// 武器パラメーターの破棄
	if (m_pWeaponParameter != nullptr)
	{
		m_pWeaponParameter->Uninit();
		delete m_pWeaponParameter;
		m_pWeaponParameter = nullptr;
	}

	// プレイヤーマネージャーの破棄
	if (m_pPlayerManager != nullptr)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = nullptr;
	}

	// タイマーの終了処理
	if (m_pTime != nullptr)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = nullptr;
	}

	//ナンバーの破棄
	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		if (m_apTotalDamage[nCnt] != nullptr)
		{
			m_apTotalDamage[nCnt]->Uninit();
			m_apTotalDamage[nCnt] = nullptr;
		}
	}

	//ナンバーの破棄
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_apDeathCount[nCnt] != nullptr)
		{
			m_apDeathCount[nCnt]->Uninit();
			m_apDeathCount[nCnt] = nullptr;
		}
	}
}

//==============================================================================================
//更新処理
//==============================================================================================
void CResult::Update()
{
	// インプットの取得処理
	CInput* pInput = CInput::GetKey();

	if (m_PlayerIndex <= 0)// ココの値を変えれば人数を増やしたり減らしたりできる
	{
		// プレイヤー情報の取得
		if (m_pPlayerManager != nullptr && m_bGetFlag == false)
		{
			// 位置の取得
			m_pos = m_pPlayerManager->GetPlayer(m_PlayerIndex)->GetPos();

			// 最大パーツ数分
			for (int nCnt = 0; nCnt < CPlayer::PARTS_MAX; nCnt++)
			{
				// パーツの取得と向きの設定
				m_pParts[nCnt] = m_pPlayerManager->GetPlayer(m_PlayerIndex)->GetParts(nCnt);
				m_pPlayerManager->GetPlayer(m_PlayerIndex)->GetParts(nCnt)->SetRot(D3DXVECTOR3(0.0f, -D3DX_PI / 3.75f, 0.0f));
			}

			// 取得完了
			m_bGetFlag = true;
		}

		// 移動時間の減算
		m_MoveCount--;
		if (m_MoveCount >= 0)
		{
			// 移動
			m_pos.x += m_move.x;
			m_pos.z -= m_move.z;

			// 歩きモーション
			for (int nCnt = 0; nCnt < CPlayer::PARTS_MAX; nCnt++)
			{
				m_pParts[nCnt]->SetMotion(CPlayer::MOTION_WALK);
			}

		}
		else if (m_MoveCount <= 0 && m_pObject2D == nullptr && m_pPlayerManager != nullptr)
		{
			m_pObject2D = CObject2D::Create(D3DXVECTOR3(400.0f, 360.0f, 0.0f), D3DXVECTOR2(m_size.x, m_size.y), CObject::PRIORITY_BACK_GROUND);
			m_pObject2D->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

			// 歩きを終了させる
			for (int nCnt = 0; nCnt < CPlayer::PARTS_MAX; nCnt++)
			{
				m_pParts[nCnt]->SetMotion(CPlayer::MOTION_NEUTRAL);
				m_pPlayerManager->GetPlayer(m_PlayerIndex)->GetParts(nCnt)->SetRot(D3DXVECTOR3(0.0f, 0.25f, 0.0f));
			}
		}

		ScaleExpansion();
		if (m_pPlayerManager != nullptr)
		{
			m_pPlayerManager->GetPlayer(m_PlayerIndex)->SetPos(m_pos);
		}

		if ((pInput->Trigger(DIK_RETURN) || pInput->Trigger(JOYPAD_B) || pInput->Trigger(JOYPAD_A) || pInput->Trigger(JOYPAD_START))
			&& m_bCreateFlag == true)
		{
			// 決定SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
			m_PlayerIndex++;
			// プレイヤー情報の削除
			if (m_PlayerIndex <= 3)
			{
				InformationUninit();
			}

			if (m_pPlayerManager == nullptr && m_PlayerIndex <= 0)// ココの値を変えれば人数を増やしたり減らしたりできる)
			{
				// プレイヤーマネージャーの生成
				m_pPlayerManager = CPlayerManager::Create();
				m_pPlayerManager->SetPlayer({ -200.0f, 250.0f, 525.0f }, CPlayerManager::TYPE_PC, m_PlayerIndex);
			}
		}
	}
	if (m_PlayerIndex >= 1)// ココの値を変えれば人数を増やしたり減らしたりできる
	{
		// タイトル画面に遷移
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if ((pInput->Trigger(DIK_RETURN) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
				|| pInput->Trigger(JOYPAD_START, nCnt))
				&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				InformationUninit();
				CFade::SetFade(CApplication::MODE_TITLE, 0.05f);
			}
		}
	}

	if (m_bCreateFlag == true)
	{
		// 受けたダメージの総数
		SetTotalDamage(CApplication::GetTotalDamage(), D3DXVECTOR3(150.0f, 375.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f));
		// 倒した敵の数
		SetDeathCount(CGame::GetDeathCount(), D3DXVECTOR3(240.0f,525.0f,0.0f), D3DXVECTOR2(30.0f, 30.0f));
		// タイマーの更新
		if (m_pTime != nullptr)
		{
			m_pTime->SetTime(CApplication::GetDestroyTime());
		}
	}
}

//=============================================================================
// サイズ拡大処理
//=============================================================================
void CResult::ScaleExpansion()
{
	if (m_pObject2D != nullptr)
	{ // nullチェック

	  // サイズの拡大
		m_size.x += SizeXScaleSpeed;
		m_size.y += SizeYScaleSpeed;

		if (m_size.x >= 700.0f)
		{ // Xサイズの最大
			m_size.x = 700.0f;
		}
		if (m_size.y >= 500.0f)
		{ // Yサイズの最大
			m_size.y = 500.0f;
		}

		if (m_size.x >= 700.0f && m_size.y >= 500.0f && m_bCreateFlag == false)
		{
			// タイムの生成
			m_pTime = CTime::Create(D3DXVECTOR3(300.0f, 225.0f, 0.0f));
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				//ナンバーの初期化
				m_apTotalDamage[nCnt] = CNumber::Create(D3DXVECTOR3(0.0f,0.0f,0.0f),0.0f,0.0f,CObject::PRIORITY_SCREEN);
			}
			for (int nCnt = 0; nCnt < 3; nCnt++)
			{
				//ナンバーの初期化
				m_apDeathCount[nCnt] = CNumber::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, CObject::PRIORITY_SCREEN);
			}

			m_pFontTexture[0] = CObject2D::Create({ 400.0f, 175.0f, 0.0f }, { 200.0f, 50.0f }, CObject::PRIORITY_SCREEN);
			m_pFontTexture[0]->SetTexture(CTexture::TEXTURE_FASTESTDEFEATE);
			m_pFontTexture[1] = CObject2D::Create({ 400.0f, 325.0f, 0.0f }, { 300.0f, 50.0f }, CObject::PRIORITY_SCREEN);
			m_pFontTexture[1]->SetTexture(CTexture::TEXTURE_DURABILITYDAMAGE);
			m_pFontTexture[2] = CObject2D::Create({ 400.0f, 475.0f, 0.0f }, { 200.0f, 50.0f }, CObject::PRIORITY_SCREEN);
			m_pFontTexture[2]->SetTexture(CTexture::TEXTURE_NUMBEROFDEFEATS);

			m_bCreateFlag = true;
		}

		// サイズの設定
		m_pObject2D->SetSize(D3DXVECTOR2(m_size.x, m_size.y));
	}
}

//==============================================================================================
// 情報の破棄処理
//==============================================================================================
void CResult::InformationUninit()
{
	// プレイヤーマネージャーの破棄
	if (m_pPlayerManager != nullptr)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = nullptr;
	}

	// オブジェクト2Dの破棄
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Uninit();
		m_pObject2D = nullptr;
	}

	// フォントの破棄
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pFontTexture[nCnt] != nullptr)
		{
			m_pFontTexture[nCnt]->Uninit();
			m_pFontTexture[nCnt] = nullptr;
		}
	}

	// スコアの破棄
	if (m_pScore != nullptr)
	{
		m_pScore->Uninit();
		m_pScore = nullptr;
	}

	// タイマーの終了処理
	if (m_pTime != nullptr)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = nullptr;
	}

	//ナンバーの破棄
	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		if (m_apTotalDamage[nCnt] != nullptr)
		{
			m_apTotalDamage[nCnt]->Uninit();
			m_apTotalDamage[nCnt] = nullptr;
		}
	}

	//ナンバーの破棄
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_apDeathCount[nCnt] != nullptr)
		{
			m_apDeathCount[nCnt]->Uninit();
			m_apDeathCount[nCnt] = nullptr;
		}
	}

	m_bGetFlag = false;
	m_bCreateFlag = false;
	m_MoveCount = 120;
}

//============================================
// 値の設定処理
//============================================
void CResult::SetTotalDamage(int num, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	m_aPosTexU[0] = num % 100000 / 10000;
	m_aPosTexU[1] = num % 10000 / 1000;
	m_aPosTexU[2] = num % 1000 / 100;
	m_aPosTexU[3] = num % 100 / 10;
	m_aPosTexU[4] = num % 10 / 1;

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		//分割数,何番目か
		m_apTotalDamage[nCnt]->SetPos(D3DXVECTOR3(pos.x + (70.0f * nCnt) + 50.0f / 2, pos.y, 0.0f));
		m_apTotalDamage[nCnt]->SetSize(D3DXVECTOR2(size.x, size.y));
		m_apTotalDamage[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
		ZoroDamageCount();
	}
}

//============================================
// 倒した敵の設定処理
//============================================
void CResult::SetDeathCount(int num, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	m_aPosTexU[0] = num % 1000 / 100;
	m_aPosTexU[1] = num % 100 / 10;
	m_aPosTexU[2] = num % 10 / 1;

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		//分割数,何番目か
		m_apDeathCount[nCnt]->SetPos(D3DXVECTOR3(pos.x + (70.0f * nCnt) + 50.0f / 2, pos.y, 0.0f));
		m_apDeathCount[nCnt]->SetSize(D3DXVECTOR2(size.x, size.y));
		m_apDeathCount[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
		ZoroDeathCount();
	}
}

//=============================================================================
// 先頭の0を消す処理
//=============================================================================
void CResult::ZoroDamageCount()
{
	if (m_apTotalDamage[m_nZeroCountTotalDamage] != nullptr)
	{
		if (m_aPosTexU[m_nZeroCountTotalDamage] == 0 && CApplication::GetTotalDamage() != 0)
		{
			m_apTotalDamage[m_nZeroCountTotalDamage]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCountTotalDamage++;
		}
		else if (m_aPosTexU[m_nZeroCountTotalDamage] == 0 && CApplication::GetTotalDamage() == 0)
		{
			m_apTotalDamage[1]->SetTexPos(0.0f, 1.0f, 0.1f, 0.0f);
			m_apTotalDamage[m_nZeroCountTotalDamage]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCountTotalDamage++;
		}
		if (CApplication::GetTotalDamage() == 0)
		{
			m_apTotalDamage[5]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//=============================================================================
// 先頭の0を消す処理
//=============================================================================
void CResult::ZoroDeathCount()
{
	if (m_apDeathCount[m_nZeroCountDeathCount] != nullptr)
	{
		if (m_aPosTexU[m_nZeroCountDeathCount] == 0 && CGame::GetDeathCount() != 0 && m_nZeroCountDeathCount <= 2)
		{
			m_apDeathCount[m_nZeroCountDeathCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCountDeathCount++;
		}
		else if (m_aPosTexU[m_nZeroCountDeathCount] == 0 && CGame::GetDeathCount() == 0 && m_nZeroCountDeathCount <= 2)
		{
			m_apDeathCount[1]->SetTexPos(0.0f, 1.0f, 0.1f, 0.0f);
			m_apDeathCount[m_nZeroCountDeathCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCountDeathCount++;
		}
		if (CGame::GetDeathCount() == 0)
		{
			m_apDeathCount[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//==============================================================================================
// 生成処理
//==============================================================================================
CResult* CResult::Create()
{
	CResult* pResult = new CResult;

	if (FAILED(pResult->Init()))
	{
		return nullptr;
	}

	return pResult;
}
