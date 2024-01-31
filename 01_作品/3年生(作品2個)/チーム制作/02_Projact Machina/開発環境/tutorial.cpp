//==============================================================================================
//
// チュートリアル　　　tutorial.cpp
// tutida ryousei
//
//==============================================================================================
#include"tutorial.h"
#include"camera.h"
#include"application.h"
#include "player_manager.h"
#include "enemy_manager.h"
#include "drop_manager.h"
#include "collision_manager.h"
#include"halfsphere.h"
#include"meshfield.h"
#include"energy_gauge.h"
#include"input.h"
#include"fade.h"
#include"fontString.h"
#include"pause.h"
#include "parts_file.h"
#include "motion.h"
#include "map_object_manager.h"

CPlayerManager* CTutorial::m_pPlayerManager = nullptr;
CEnemyManager* CTutorial::m_pEnemyManager = nullptr;
CDropManager* CTutorial::m_pDropManager = nullptr;
CCollision_Manager* CTutorial::m_pCollision_Manager = nullptr;
CMeshField *CTutorial::pMeshField = nullptr;			// メッシュフィールド
CPause *CTutorial::m_pPause = nullptr;
CMap_Object_Manager *CTutorial::m_pMap_Object_Manager = nullptr;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CTutorial::CTutorial()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CTutorial::~CTutorial()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CTutorial::Init()
{
	CFontString::Create({ 390.0f, SCREEN_HEIGHT / 2, 0.0f }, { 50.0f, 50.0f }, "チュートリアル");

	// カメラのポインタ
	CCamera* pCamera = CApplication::GetCamera();

	// 視点、注視点の設定
	pCamera->SetPosV({ 0.0f, 500.0f, -1000.0f });
	pCamera->SetPosR({ 0.0f, 250.0f, 1000.0f });

	// 全てのモデルパーツの読み込み
	CApplication::GetPartsFile()->LoadAllFile();

	// 全てのモデルパーツの読み込み
	CApplication::GetMotion()->LoadAllFile();

	m_pPlayerManager = CPlayerManager::Create();	// プレイヤーマネージャーの生成
	m_pEnemyManager = new CEnemyManager;			// 敵キャラマネージャーの生成
	m_pDropManager = new CDropManager;				// 落とし物マネージャーの生成
	m_pCollision_Manager = new CCollision_Manager;	// 当たり判定マネージャーの生成
	m_pMap_Object_Manager = new CMap_Object_Manager;

	// 全てのモデルパーツの読み込み
	CDrop_Weapon* pWeaponDummer = new CDrop_Weapon;
	pWeaponDummer->LoadAllFile();
	pWeaponDummer->Uninit();

	// プレイヤーの生成(テスト)
	m_pPlayerManager->SetPlayer({ 0.0f, 0.0f, 0.0f }, CPlayerManager::TYPE_PC, 0);

	// ハーフスフィアの生成
	CHalfSphere *pHalfSphere = CHalfSphere::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20000.0f, 20000.0f, 20000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CHalfSphere::SPHERE_UP);
	pHalfSphere->LoadTexture("Data/texture/sky000.jpg");

	// メッシュフィールドの生成
	pMeshField = CMeshField::Create({ 0.0f, 0.0f, 0.0f }, 10, 10, 4000.0f);

	// ポーズ画面
	m_pPause = CPause::Create();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CTutorial::Uninit()
{
	// プレイヤーマネージャーの破棄
	if (m_pPlayerManager != nullptr)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = nullptr;
	}

	// 敵キャラマネージャーの破棄
	if (m_pEnemyManager != nullptr)
	{
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}

	// 落とし物マネージャーの破棄
	if (m_pDropManager != nullptr)
	{
		delete m_pDropManager;
		m_pDropManager = nullptr;
	}

	// コリジョンマネージャーの破棄
	if (m_pCollision_Manager != nullptr)
	{
		// 全ての当たり判定の解放処理
		m_pCollision_Manager->ReleaseAllCollision();
		delete m_pCollision_Manager;
		m_pCollision_Manager = nullptr;
	}
	
	//マップオブジェクトの破棄
	if (m_pMap_Object_Manager != nullptr)
	{
		delete m_pMap_Object_Manager;
		m_pMap_Object_Manager = nullptr;
	}
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CTutorial::Update()
{
	CInput* pInput = CInput::GetKey();

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if ((pInput->Trigger(DIK_RETURN) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
			|| pInput->Trigger(JOYPAD_START, nCnt))
			&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			CFade::SetFade(CApplication::MODE_GAME, 0.05f);
		}
	}
}

//==============================================================================================
// 生成処理
//==============================================================================================
CTutorial *CTutorial::Create()
{
	CTutorial* pTutorial = new CTutorial;

	if (FAILED(pTutorial->Init()))
	{
		return nullptr;
	}

	return pTutorial;
}