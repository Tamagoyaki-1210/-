//=============================================================================
//
// マネージャー
// Author : tanimoto kosuke
//
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"
#include "fade.h"
#include "mode.h"

CRenderer *CApplication::m_pRenderer = nullptr;	// レンダラーポインタ
CInput *CApplication::m_pInput = nullptr;		// 入力ポインタ
CTexture *CApplication::m_pTexture = nullptr;	// 画像ポインタ
CMode *CApplication::m_pMode = nullptr;			// モードポインタ
CSound *CApplication::m_pSound = nullptr;		// サウンドポインタ

//=====================================
// デフォルトコンストラクタ
//=====================================
CApplication::CApplication()
{

}

//=====================================
// デストラクタ
//=====================================
CApplication::~CApplication()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_pRenderer = new CRenderer;
	// true : ウィンドウサイズ, false : 全画面
	if (FAILED(m_pRenderer->Init(hWnd, true)))
	{
		return -1;
	}

	m_pInput = new CInput;
	// キーボードの初期化処理
	if (FAILED(m_pInput->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	m_pTexture = new CTexture;
	m_pTexture->Load();

	m_pMode = new CMode;
	//最初のモード
	m_pMode->FirstMode(CMode::MODE_TITLE);

	m_pSound = new CSound;
	m_pSound->Init(hWnd);

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CApplication::Uninit()
{
	// 画像
	if (m_pTexture != nullptr)
	{
		m_pTexture->Uninit();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// 入力
	if (m_pInput != nullptr)
	{
		m_pInput->Uninit();
		delete m_pInput;
		m_pInput = nullptr;
	}

	// レンダリング
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// モード
	if (m_pMode != nullptr)
	{
		m_pMode->Uninit();
		delete m_pMode;
		m_pMode = nullptr;
	}

	// サウンド
	if (m_pSound != nullptr)
	{
		CSound::StopSound();
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}
}

//=====================================
//更新処理
//=====================================
void CApplication::Update()
{
	m_pInput->Update();
	m_pRenderer->Update();
	m_pMode->Update();
	m_pMode->SetMode();
}

//=====================================
//描画処理
//=====================================
void CApplication::Draw()
{
	m_pRenderer->Draw();
	m_pMode->Draw();
}