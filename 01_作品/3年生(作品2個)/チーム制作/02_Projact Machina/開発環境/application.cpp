//==============================================================================================
//
// アプリケーション
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include "application.h"
#include "object.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "light.h"
#include "mode.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "title_menu.h"
#include "particle_manager.h"
#include "char_select.h"
#include "stage_select.h"
#include "tutorial.h"
#include "confirmation_window.h"
#include "parts_file.h"
#include "time.h"
#include "connect.h"
#include <time.h>
#include "character.h"
#include "player_manager.h"

#ifdef _DEBUG
#include "debugProc.h"
#endif

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
HWND CApplication::Window = nullptr;
CRenderer* CApplication::m_pRenderer = nullptr;
CApplication::MODE CApplication::m_modeType = MODE_NONE;
CMode* CApplication::m_pGameMode = nullptr;
CTexture* CApplication::m_pTexture = nullptr;
CModel* CApplication::m_pModel = nullptr;
CMotion* CApplication::m_pMotion = nullptr;
CSound* CApplication::m_pSound = nullptr;
CLight* CApplication::m_pLight = nullptr;
CCamera* CApplication::m_pCamera = nullptr;
CFade* CApplication::m_pFade = nullptr;
CMenu* CApplication::m_pMenu = nullptr;
CParts_File* CApplication::m_pPartsFile = nullptr;
CParticleManager* CApplication::m_pParticleManager = nullptr;
ConnectManager*CApplication::m_pClient = nullptr;
bool CApplication::m_bGameStart = false;
bool CApplication::m_bPauce = false;
bool CApplication::m_nSkill = false;
int CApplication::m_nPlayerJobIndex[3] = {};
int CApplication::m_nPlayerWeaponIndex = 0;
int CApplication::m_nDestroyTime = 0;
int CApplication::m_nDamage = 0;
int CApplication::m_nDefeats = 0;
int CApplication::m_nTotalDamage = 0;

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
	Window = hWnd;

	// クラスの生成
	m_pRenderer = new CRenderer;					// レンダリングの生成
	m_pTexture = new CTexture;						// テクスチャの生成
	m_pModel = new CModel;							// モデルの生成
	m_pMotion = new CMotion;						// モーションの生成
	m_pLight = new CLight;							// ライトの生成
	m_pCamera = new CCamera;						// カメラの生成
	m_pPartsFile = new CParts_File;					// パーツファイルの生成
	//m_pClient = new CClient;						// サーバーのコネクター
	//入力処理
	m_pInput = CInput::Create();
	//入力処理の初期化処理
	if (FAILED(m_pInput->Init(hInstance, Window)))
	{
		return E_FAIL;
	}

	//ウィンドウの設定
	BOOL bWindow = FALSE;

#ifdef _DEBUG
	bWindow = TRUE;
#endif

	m_pRenderer->Init(Window, bWindow);			// レンダリング
	m_pLight->Init();							// ライト
	m_pCamera->Init();							// カメラ

	m_pTexture->LoadAll();						// 全てのテクスチャの読み込み

	m_pSound = CSound::Create(Window);			// サウンドの生成

	m_pFade = CFade::Create();					// フェード

	// パーティクルマネージャの生成
	m_pParticleManager = new CParticleManager;
	m_pParticleManager->LoadText("particle.txt");

	//リリース時はマウスポインターを消す
	m_pInput->SetCursorErase(false);
#ifdef _DEBUG
	m_pDebugProc = CDebugProc::Create();		// デバッグテキスト

	//デバック時はマウスポインターを表示
	m_pInput->SetCursorErase(true);
#endif

	// ゲームモード
	m_modeType = MODE_TITLE;

	// モードの定設
	SetMode(m_modeType);

	srand((unsigned int)time(0));

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CApplication::Uninit()
{
	// 入力処理の終了処理
	m_pInput->Uninit();

	// テクスチャの終了
	if (m_pTexture != nullptr)
	{
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// モデルの終了
	if (m_pModel != nullptr)
	{
		m_pModel->ReleaseAll();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// モーションの終了
	if (m_pMotion != nullptr)
	{
		m_pMotion->ReleaseAll();
		delete m_pMotion;
		m_pMotion = nullptr;
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

	// パーツファイルの終了
	if (m_pPartsFile != nullptr)
	{
		m_pPartsFile->Uninit();
		delete m_pPartsFile;
		m_pPartsFile = nullptr;
	}

	// 全てのオブジェクトの解放処理
	CObject::ReleaseAll();

	// パーティクルマネージャの破棄
	if (m_pParticleManager != nullptr)
	{
		m_pParticleManager->ReleaseAll();
		delete m_pParticleManager;
		m_pParticleManager = nullptr;
	}

	while (m_pClient != nullptr)
	{
		
		if (!m_pClient->GetMainConnecter() && !m_pClient->GetTutorialConnecter())
		{
			m_pClient->Uninit();
			delete m_pClient;
			m_pClient = nullptr;
		}
	}

	// レンダリングの終了
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
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

	m_nSkill = CConfirmation_Window::GetSelectChoice();

	CPlayer *pPlayer = nullptr;

	if (m_modeType == MODE_GAME)
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);

	if (CGame::GetTime() != nullptr)
	{
		m_nDestroyTime = CGame::GetTime()->GetTime();
	}
	if (CGame::GetPlayerManager() != nullptr && pPlayer != nullptr)
	{
		m_nTotalDamage = pPlayer->GetDamage();
	}
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
	// ゲーム開始判定を停止させる
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

		// 全てのオブジェクトの解放処理
		CObject::ReleaseAll();

		// 全てのモデルセットの終了
		m_pPartsFile->ReleaseAllFile();

		// 全BGMの終了処理
		m_pSound->StopAllBGM();
	}
	// カメラの初期化
	m_pCamera->Init();

	// 全てのモデル情報の解放処理
	m_pModel->ReleaseAll();

	// 全てのモーション情報の解放処理
	m_pMotion->ReleaseAll();

	// モードを切り替える
	m_modeType = mode;

	// 指定のモードの設定
	switch (m_modeType)
	{
	case CApplication::MODE_TITLE:
		m_pGameMode = CTitle::Create();
		m_pMenu = CTitleMenu::Create();
		m_pMenu->SetDisplay(false);
		break;
	case CApplication::MODE_CHAR_SELECT:
		m_pGameMode = CChar_Select::Create();
		break;
	case CApplication::MODE_STAGE_SELECT:
		m_pGameMode = CStage_Select::Create();
		break;
	case CApplication::MODE_TUTORIAL:
		m_pGameMode = CTutorial::Create();
		break;
	case CApplication::MODE_GAME:
		m_pGameMode = CGame::Create();
		break;
	case CApplication::MODE_RESULT:
		m_pGameMode = CResult::Create();
		break;
	default:
		break;
	}
}