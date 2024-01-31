//==============================================================================================
//
// アプリケーション
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"application.h"
#include"object.h"
#include"camera.h"
#include"title.h"
#include"entry.h"
#include"game.h"
#include"result.h"
#include"texture.h"
#include"sound.h"
#include"light.h"
#include"camera.h"
#include "mode.h"
#include "fade.h"
#include"objectX.h"
#include "particle_manager.h"
#include "stage_manager.h"
#include "input.h"
#include "charmanager.h"
#include "menu.h"
#include "menugame.h"
#include "stage_select.h"
#include <time.h>

#ifdef _DEBUG
#include "debugProc.h"
#endif

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
CRenderer *CApplication::m_pRenderer = nullptr;
CApplication::MODE CApplication::m_mode = MODE_NONE;
CMode *CApplication::m_pGameMode = nullptr;
CTexture *CApplication::m_pTexture = nullptr;
CSound *CApplication::m_pSound = nullptr;
CLight *CApplication::m_pLight = nullptr;
CCamera *CApplication::m_pCamera = nullptr;
CFade *CApplication::m_pFade = nullptr;
CParticleManager *CApplication::m_pParticleManager = nullptr;
CStageManager* CApplication::m_pStageManager = nullptr;
CCharManager *CApplication::m_pCharManager = nullptr;
CMenu *CApplication::m_pMenu = nullptr;

bool CApplication::m_bGameStart = false;
bool CApplication::m_bPauce = false;
//int CApplication::m_nStage_Number = 0;

#ifdef _DEBUG
CDebugProc *CApplication::m_pDebugProc = nullptr;
#endif
//==============================================================================================
// コンストラクタ
//==============================================================================================
CApplication::CApplication()
{
	m_pInput = nullptr;
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CApplication::~CApplication()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	// クラスの生成
	m_pRenderer = new CRenderer;				// レンダリング
	m_pTexture = new CTexture;					// テクスチャ
	m_pLight = new CLight;						// ライト
	m_pCamera = new CCamera;					// カメラ

	// 初期化処理

	//入力処理
	m_pInput = CInput::Create();
	//入力処理の初期化処理
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ウィンドウの設定
	BOOL bWindow = FALSE;

#ifdef _DEBUG
	bWindow = TRUE;
#endif

	m_pRenderer->Init(hWnd, bWindow);				// レンダリング
	m_pLight->Init();							// ライト
	m_pCamera->Init();							// カメラ

	m_pTexture->LoadAll();						// 全てのテクスチャの読み込み

	m_pSound = CSound::Create(hWnd);			// サウンドの生成

	// パーティクルマネージャの生成
	m_pParticleManager = new CParticleManager;
	m_pParticleManager->LoadText("particle.txt");

	// ステージマネージャの生成
	m_pStageManager = new CStageManager;
	m_pStageManager->LoadText("stage.txt");

	m_pCharManager = CCharManager::Create();	// キャラマネージャーの生成

	m_pFade = CFade::Create();					// フェード

	//リリース時はマウスポインターを消す
	m_pInput->SetCursorErase(false);
#ifdef _DEBUG
	m_pDebugProc = CDebugProc::Create();		// デバッグテキスト

	//デバック時はマウスポインターを表示
	m_pInput->SetCursorErase(true);
#endif

	// ゲームモード
	m_mode = MODE_TITLE;

	// モードの定設
	SetMode(m_mode);

	srand((unsigned int)time(0));

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CApplication::Uninit()
{
	//入力処理の終了処理
	m_pInput->Uninit();

	// レンダリングの終了
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	//テクスチャの終了
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// ライトの終了
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	// カメラの終了
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

#ifdef _DEBUG
	// デバッグテキストの破棄
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}
#endif

	// パーティクルマネージャの破棄
	if (m_pParticleManager != nullptr)
	{
		m_pParticleManager->ReleaseAll();
		delete m_pParticleManager;
		m_pParticleManager = nullptr;
	}

	// ステージマネージャの破棄
	if (m_pStageManager != nullptr)
	{
		delete m_pStageManager;
		m_pStageManager = nullptr;
	}

	// キャラマネージャーの終了処理
	if (m_pCharManager != nullptr)
	{
		m_pCharManager->Uninit();
		delete m_pCharManager;
		m_pCharManager = nullptr;
	}

	// フェードの破棄
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	// モードの破棄
	if (m_pGameMode != nullptr)
	{
		m_pGameMode->Uninit();
		delete m_pGameMode;
		m_pGameMode = nullptr;
	}

	// メニューの破棄
	if (m_pMenu != nullptr)
	{
		m_pMenu->Uninit();
		delete m_pMenu;
		m_pMenu = nullptr;
	}

	// サウンドの終了
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	CObject::ReleaseAll();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CApplication::Update()
{
	//入力処理の更新処理
	m_pInput->Update();

	// サウンドの更新処理
	m_pSound->Update();

	m_pFade->Update();

	// 使用中のメニュー更新処理
	if (m_pMenu != nullptr)
	{
		m_pMenu->Update();
	}

	m_pRenderer->Update();

	// カメラの更新処理
	m_pCamera->Update();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CApplication::Draw()
{
	m_pRenderer->Draw();
}

//==============================================================================================
// モードの設定
//==============================================================================================
void CApplication::SetMode(MODE mode)
{
	// ゲームを停止させる
	m_bGameStart = false;

	// ゲームモードが使用されている場合の終了処理
	if (m_pGameMode != nullptr)
	{
		// ゲームモードの終了処理
		m_pGameMode->Uninit();
		delete m_pGameMode;
		m_pGameMode = nullptr;

		// メニューの終了処理
		if (m_pMenu != nullptr)
		{
			m_pMenu->Uninit();
			delete m_pMenu;
			m_pMenu = nullptr;
		}

		// キャラの解放処理
		m_pCharManager->CharaReleaseAll();

		// 全てのオブジェクトの解放処理
		CObject::ReleaseAll();
		
		// オブジェクトの破棄
		CObjectX::UninitObject();
	}
	// カメラの初期化
	m_pCamera->Init();

	CModel::ModelReleaseAll();

	// モードを切り替える
	m_mode = mode;

	// 指定のモードの設定
	switch (m_mode)
	{
	case CApplication::MODE_TITLE:
		m_pGameMode = CTitle::Create();
		break;

	case CApplication::MODE_ENTRY:
		m_pGameMode = CEntry::Create();
		break;

	case CApplication::MODE_STAGE_SELECT:
		m_pGameMode = CStage_Select::Create();
		break;

	case CApplication::MODE_GAME:
		m_pGameMode = CGame::Create();
		m_pMenu = CMenuGame::Create();	// ゲームメニューの生成
		break;

	case CApplication::MODE_RESULT:
		m_pGameMode = CResult::Create();
		break;

	default:
		break;
	}
}