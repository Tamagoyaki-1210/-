//=============================================================================
//
// application.cpp
// Author : Ricci Alex
//
//=============================================================================

//インクルードファイル
#include "application.h"
#include "rendering.h"
#include "inputKeyboard.h"
#include "inputMouse.h"
#include "inputPad.h"
#include "directionalLight.h"
#include "camera.h"
#include "model.h"
#include "debugProc.h"
#include "animator.h"
#include "title.h"
#include "playerSelect.h"
#include "stageSelect.h"
#include "gamerace.h"
#include "fade.h"
#include "menu.h"
#include "hitbox.h"
#include "score.h"
#include "tutorial.h"
#include "message.h"

//静的メンバー変数の宣言
HWND CApplication::m_hWnd;													//ウインドウ
CRenderer* CApplication::m_pRenderer = nullptr;								//レンディングインスタンスへのポインタ
CInput* CApplication::m_pInput[2] = {};										//インプットインスタンスへのポインタ
CInputMouse* CApplication::m_pMouse = nullptr;								//マウスインスタンスへのポインタ
CInputPad* CApplication::m_pPad = nullptr;									//パッドのインスタンス
CSound* CApplication::m_pSound = nullptr;									//サウンドのインスタンス
CFade* CApplication::m_pFade = nullptr;										//フェードのインスタンス
CCamera* CApplication::m_pCamera = nullptr;									//カメラのインスタンス
CMode* CApplication::m_pMode = nullptr;										// モードへのポインタ
CMenu* CApplication::m_pMenu = nullptr;										// メニューへのポインタ
CMessage* CApplication::m_pMessage = nullptr;								// メッセージへのポインタ
CDebugProc* CApplication::m_pDebug = nullptr;								//デバッグ表示
CApplication::Mode CApplication::m_mode = CApplication::Mode_Title;			//モード
CApplication::Mode CApplication::m_modeNext = CApplication::Mode_Title;		//次のモード
bool CApplication::m_bPause = false;										//ポーズ状態であるかどうか
int CApplication::m_nStageSelect = 0;										// ステージ選択番号

//コンストラクタ
CApplication::CApplication()
{
	
}

//デストラクタ
CApplication::~CApplication()
{
	
}

//初期化処理
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_hWnd = hWnd;				//ウインドウの設定処理

	//レンディングインスタンスの生成処理
	if (m_pRenderer == nullptr)
	{//ポインタはnullではなかったら
		m_pRenderer = new CRenderer;
	}

	//レンディングインスタンスの初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{//第2引数はFALSEだったら、フルスクリーンになります。
		return -1;
	}

	m_pDebug = CDebugProc::Create();			//デバッグテキストの生成処理

	//テクスチャ全部をロード処理
	CObject_2D::LoadTextures();

	//モデル全部をロード処理
	CModel::LoadAllModels();

	//全部のアニメーションの読み込み
	CAnimator::LoadAllAnimation();		

	//サウンドの生成
	m_pSound = CSound::Create(hWnd);

	CLight::ReleaseAll();																						//全部のライトのリリース処理
	CDirectionalLight::Create(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(2, -5, 2));						//メインライトの生成
	CDirectionalLight::Create(D3DXCOLOR(0.75f, 0.75f, 0.75f, 0.75f), D3DXVECTOR3(-0.2f, 0.2f, 1.0f));			//ライトの生成

	m_pCamera = CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, -200.0f, 100.0f));			//カメラの生成

	// モードインスタンスの生成処理
	m_pMode = CTitle::Create();
	m_mode = Mode_Title;
	m_modeNext = Mode_Title;
	m_pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);

	// フェード生成
	if (m_pFade == nullptr)
	{
		m_pFade = CFade::Create();
		m_pFade->SetFade();
	}

	//キーボードインスタンスの生成処理 
	m_pInput[0] = new CInputKeyboard;

	//キーボードの初期化処理
	if (FAILED(m_pInput[0]->Init(hInstance, hWnd, GUID_SysKeyboard)))
	{
		return -1;
	}

	//マウスの生成
	m_pMouse = new CInputMouse;			

	if (m_pMouse != nullptr)
	{//マウスの初期化処理
		m_pMouse->Init(hInstance, hWnd, GUID_SysMouse);
	}

	//パッドの生成
	m_pPad = new CInputPad;

	if (m_pPad != nullptr)
	{//パッドの初期化処理
		m_pPad->Init(hInstance, hWnd, GUID_SysMouse);
	}

	// メニュー生成
	if (m_pMenu == nullptr)
	{
		m_pMenu = CMenu::Create();
	}

	// メッセージ生成
	if (m_pMessage == nullptr)
	{
		m_pMessage = CMessage::Create();
	}

	m_bPause = false;	//ポーズを未使用にする

	m_nStageSelect = 0;	// ステージ番号初期化

	return S_OK;
}

//終了処理
void CApplication::Uninit(void)
{
	CScore::Clear();		//スコアのクリア処理

	//Rendererインスタンスの破棄
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();			//終了処理
		delete m_pRenderer;				//破棄する
		m_pRenderer = nullptr;			//nullにする
	}

	// モードの破棄処理
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();				//終了処理
		delete m_pMode;					//破棄する
		m_pMode = nullptr;				//nullにする
	}

	//インプットデバイスの破棄
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pInput[nCnt] != nullptr)
		{
			m_pInput[nCnt]->Uninit();	//終了処理
			delete m_pInput[nCnt];		//破棄する
			m_pInput[nCnt] = nullptr;	//nullにする
		}
	}

	//マウスの破棄処理
	if (m_pMouse != nullptr)
	{
		m_pMouse->Uninit();				//終了処理
		delete m_pMouse;				//破棄する
		m_pMouse = nullptr;				//nullにする
	}

	//パッドの破棄処理
	if (m_pPad != nullptr)
	{
		m_pPad->Uninit();				//終了処理
		delete m_pPad;					//破棄する
		m_pPad = nullptr;				//nullにする
	}

	//サウンドの破棄処理
	if (m_pSound != nullptr)
	{
		m_pSound->Uninit();				//終了処理
		delete m_pSound;				//破棄する
		m_pSound = nullptr;				//nullにする
	}

	//フェードの破棄処理
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();				//終了処理
		delete m_pFade;					//破棄する
		m_pFade = nullptr;				//nullにする
	}

	//メニューの破棄処理
	if (m_pMenu != nullptr)
	{
		m_pMenu->Uninit();				//終了処理
		delete m_pMenu;					//破棄する
		m_pMenu = nullptr;				//nullにする
	}

	//メッセージの破棄処理
	if (m_pMessage != nullptr)
	{
		m_pMessage->Uninit();			//終了処理
		delete m_pMessage;				//破棄する
		m_pMessage = nullptr;			//nullにする
	}

	//カメラの破棄処理
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();			//終了処理
		delete m_pCamera;				//破棄する
		m_pCamera = nullptr;			//nullにする
	}

	//デバッグテキストの破棄処理
	if (m_pDebug != nullptr)
	{
		delete m_pDebug;				//破棄する
		m_pDebug = nullptr;				//nullにする
	}

	//オブジェクト全体の終了処理
	CObject::ReleaseAll();

	//ヒットボックスの破棄処理
	CHitbox::ReleaseAll();

	//ライトの破棄処理
	CLight::ReleaseAll();

	//テクスチャ全部の破棄
	CObject_2D::DestroyLoadedTextures();

	//全部のアニメーションの破棄処理
	CAnimator::DestroyLoadedAnimation();

	//モデル全部破棄の破棄処理
	CModel::DestroyAllModels();
}

//更新処理
void CApplication::Update(void)
{
	//インプットデバイスの更新処理
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_pInput[nCnt] != nullptr)
		{
			m_pInput[nCnt]->Update();
		}
	}

	//Rendererの更新処理
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Update();
	}

	//フェードの更新処理
	if (m_pFade != nullptr)
	{
		//フェードが読み込まれていない場合
		if (m_pFade->GetFade() != CFade::FADE_NONE)
		{
			m_pFade->Update();

			if (m_pFade->GetFade() == CFade::FADE_CHANGE)
			{// フェード切り替え状態の場合
				ChangeMode();
			}
		}
	}

	//現在のモードの更新処理
	if (m_pMode != nullptr)
	{
		m_pMode->Update();
	}

	//メニューの更新処理
	if (m_pMenu != nullptr)
	{
		m_pMenu->Update();
	}

	//メッセージの更新処理
	if (m_pMessage != nullptr)
	{
		m_pMessage->Update();
	}

	//マウスの更新処理
	if (m_pMouse != nullptr)
	{
		m_pMouse->Update();
	}

	//パッドの更新処理
	if (m_pPad != nullptr)
	{
		m_pPad->Update();
	}

	// ポーズ中でない場合のみ更新
	if (m_bPause == false)
	{
		//カメラの更新処理
		if (m_pCamera != nullptr)
		{
			m_pCamera->Update();
		}
	}
}

//描画処理
void CApplication::Draw(void)
{
	//Rendererの描画処理
	if (m_pRenderer != nullptr)
	{//nullではなかったら
		m_pRenderer->Draw();
	}
}

//Rendererの取得処理
CRenderer* CApplication::GetRenderer(void)
{
	return m_pRenderer;
}

//マウスの取得処理
CInputMouse* CApplication::GetMouse(void)
{
	return m_pMouse;
}

//パッドの取得処理
CInputPad * CApplication::GetPad(void)
{
	return m_pPad;
}

//ウインドウの取得処理
HWND CApplication::GetWindow(void)
{
	return m_hWnd;
}

//サウンドの取得処理
CSound* CApplication::GetSound(void)
{
	return m_pSound;
}

//カメラの取得処理
CCamera* CApplication::GetCamera(void)
{
	return m_pCamera;
}

//フェードの取得処理
CFade* CApplication::GetFade(void)
{
	return m_pFade;
}

//=====================================
// モード取得処理
//=====================================
CApplication::Mode CApplication::GetMode(void)
{
	return m_mode;
}

//=====================================
// モード設定処理
//=====================================
void CApplication::SetMode(Mode mode)
{
	// フェード切り替え状態ではない場合
	if (m_pFade->GetFade() == CFade::FADE_NONE)
	{
		m_modeNext = mode;
		m_pFade->SetFade();
	}
}

//=====================================
// モード切り替え処理
//=====================================
void CApplication::ChangeMode()
{
	// 現在モードの終了
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = nullptr;
	}

	m_pMenu->Uninit();		// メニューの終了
	m_pMessage->Uninit();	// メッセージの終了

	CObject::ReleaseAll();	// 全てのオブジェクトの終了
	CHitbox::ReleaseAll();	// 全ての当たり判定の終了

	m_bPause = false;		// ポーズを未使用にする

	// 次のモードがステージ選択でなければ
	if (m_modeNext != Mode_StageSelect)
	{
		if (m_pSound != nullptr)
		{// 使用中のサウンド停止
			m_pSound->Stop();
		}
	}

	// モードを生成する
	switch (m_modeNext)
	{
	case CApplication::Mode_Title:
		m_pMode = CTitle::Create();
		m_pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);
		break;
	case CApplication::Mode_PlayerSelect:
		m_pMode = CPlayerSelect ::Create();
		m_pSound->Play(CSound::SOUND_LABEL_BGM_SELECT);
		break;
	case CApplication::Mode_StageSelect:
		m_pMode = CStageSelect::Create();
		break;
	case CApplication::Mode_Game_Race:
		m_pMode = CGameRace::Create();

		// 選択ステージ番号別の処理
		if (m_nStageSelect == 0)
		{
			m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME01);
		}
		else if (m_nStageSelect == 1)
		{
			m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME02);
		}
		else if (m_nStageSelect == 2)
		{
			m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME03);
		}
		break;
	case CApplication::Mode_Tutorial:
		m_pMode = CTutorial::Create();
		m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME01);
		break;
	default:
		break;
	}
	m_mode = m_modeNext;	// 次のモードに変更

	m_pMenu->Init();		// メニューの初期化処理
}

//=====================================
// ポーズ取得処理
//=====================================
bool CApplication::GetPause()
{
	return m_bPause;
}

//=====================================
// ポーズ設定処理
//=====================================
void CApplication::SetPause(const bool bPause)
{
	m_bPause = bPause;
}