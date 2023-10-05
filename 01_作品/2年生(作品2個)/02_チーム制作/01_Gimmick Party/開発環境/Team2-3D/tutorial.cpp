//=============================================================================
//
// tutorial.cpp
// Author : tanimoto kosuke
//
//=============================================================================
#include "tutorial.h"
#include "inputKeyboard.h"
#include "inputPad.h"
#include "application.h"
#include "meshfield.h"
#include "player.h"
#include "coin.h"
#include "camera.h"
#include "SpikeBall.h"
#include "lavaFloor.h"
#include "score.h"
#include "trampoline.h"
#include "bouncePole.h"
#include "stoneSpawner.h"
#include "icePillarSpawner.h"
#include "stage.h"
#include "object2D.h"
#include "message.h"
#include "fontString.h"

const D3DXVECTOR2 CTutorial::m_fontSize = D3DXVECTOR2(30.0f, 30.0f);
const D3DXCOLOR CTutorial::m_fontColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
CStage* CTutorial::m_pStage = nullptr;

//=====================================
// デフォルトコンストラクタ
//=====================================
CTutorial::CTutorial()
{
	m_pUi = nullptr;
	m_pFont = nullptr;
	m_bChange = false;
}

//=====================================
// デストラクタ
//=====================================
CTutorial::~CTutorial()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CTutorial::Init(void)
{
	CGame::Init();
	m_pFont = nullptr;

	m_pStage = CStage::Create();

	CMeshfield* pField = CMeshfield::Create(D3DXVECTOR3(-135.0f, -150.0f, -1100.0f), Vec3Null, D3DXVECTOR2(30.0f, 70.0f), 20, 10, 3);
	pField->SetTexture(CObject::TEXTURE_BLOCK);
	pField->SetTextureTiling(0.33f);
	pField->SetPriority(1);



	// カウントダウンメッセージ表示
	//CApplication::GetMsg()->SetCountDown(3);

	if (CApplication::GetCamera() != nullptr)
	{
		CApplication::GetCamera()->SetPos(D3DXVECTOR3(0.0f, 0.0f, ((47 - 21) * -70.0f) - 500.0f), D3DXVECTOR3(0.0f, -200.0f, 100.0f));
	}

	for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
	{
		if (CStage::GetPlayer(nCnt) != nullptr)
		{
			CStage::GetPlayer(nCnt)->SetPos(D3DXVECTOR3(-75.0f + (37 * nCnt), -150.0f, ((60 - 20) * -70.0f) - 200.0f));
		}
	}

	m_pUi = CObject_2D::Create();
	m_pUi->SetPos(D3DXVECTOR3(640.0f, 90.0f, 0.0f));
	m_pUi->SetSize(D3DXVECTOR2(105.0f, 54.0f));
	m_pUi->SetTexture(CObject::TEXTURE_TUTORIAL_MOVE);
	m_pUi->SetTextureParameter(8, 2, 4, 20);
	m_pUi->SetPriority(5);
	m_bChange = false;

	SetObject();

	CMessage::SetStart(true);

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CTutorial::Uninit(void)
{
	CGame::Uninit();

	if (m_pStage != nullptr)
	{
		m_pStage->Uninit();
		delete m_pStage;
		m_pStage = nullptr;
	}

	if (m_pUi != nullptr)
	{
		m_pUi->Release();
		m_pUi = nullptr;
	}

	if (m_pFont != nullptr)
	{
		m_pFont->Uninit();
		m_pFont = nullptr;
	}
}

//=====================================
// 更新処理
//=====================================
void CTutorial::Update(void)
{
	CGame::Update();

	if (m_pStage != nullptr)
	{
		m_pStage->Update();
	}

	D3DXVECTOR3 cameraPos = CApplication::GetCamera()->GetPos();

	if (cameraPos.z >= -2200.0f && cameraPos.z < -1750.0f && !m_bChange)
	{
		m_bChange = true;

		if (m_pUi != nullptr)
		{
			m_pUi->SetSize(D3DXVECTOR2(200.0f, 66.7f));
			m_pUi->SetTexture(CObject::TEXTURE_TUTORIAL_BUTTON);
			m_pUi->SetTextureParameter(2, 1, 2, 20);
		}
	}
	else if (cameraPos.z >= -1750.0f && cameraPos.z < -1300.0f && m_bChange)
	{
		m_bChange = false;

		m_pUi->Release();
		m_pUi = nullptr;

		m_pFont = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 90.0f, 0.0f), m_fontSize, "コインにさわるとスコアがふえます");
		m_pFont->SetColor(m_fontColor);
		m_pFont->SetSellect();
		m_pFont->SizeScale();
	}
	else if (cameraPos.z >= -1300.0f && cameraPos.z < -1000.0f && !m_bChange)
	{
		m_bChange = true;

		m_pFont->Uninit();
		m_pFont = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 90.0f, 0.0f), m_fontSize, "おちるとプレイヤーがリスポーンします");
		m_pFont->SetColor(m_fontColor);
		m_pFont->SetSellect();
		m_pFont->SizeScale();
	}
	else if (cameraPos.z >= -1000.0f && cameraPos.z < -600.0f && m_bChange)
	{
		m_bChange = false;

		m_pFont->Uninit();
		m_pFont = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 90.0f, 0.0f), m_fontSize, "さまざまなギミックがあります");
		m_pFont->SetColor(m_fontColor);
		m_pFont->SetSellect();
		m_pFont->SizeScale();
	}
	else if (cameraPos.z >= -600.0f && cameraPos.z < 0.0f && !m_bChange)
	{
		m_bChange = true;

		m_pFont->Uninit();
		m_pFont = nullptr;
	}
	else if (cameraPos.z >= 0.0f && cameraPos.z < 400.0f && m_bChange)
	{
		m_bChange = false;

		if (m_pFont == nullptr)
		{
			m_pFont = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 90.0f, 0.0f), m_fontSize, "さきにゴールするほどスコアがふえます");
			m_pFont->SetColor(m_fontColor);
			m_pFont->SetSellect();
			m_pFont->SizeScale();
		}
	}
	else if (cameraPos.z >= 400.0f && cameraPos.z < 790.0f && !m_bChange)
	{
		m_bChange = true;

		m_pFont->Uninit();
		m_pFont = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 90.0f, 0.0f), m_fontSize, "スコアがいちばんのプレイヤーのかちです");
		m_pFont->SetColor(m_fontColor);
		m_pFont->SetSellect();
		m_pFont->SizeScale();
	}
	else if (cameraPos.z >= 790.0f && m_bChange)
	{
		m_bChange = false;

		if (m_pFont != nullptr)
		{
			m_pFont->Uninit();
			m_pFont = nullptr;
		}
	}
	if (!CApplication::GetPause())
	{
		if (m_pFont != nullptr)
		{
			m_pFont->Update();
		}
	}
}

//=====================================
// 生成処理
//=====================================
CTutorial* CTutorial::Create(void)
{
	CTutorial* pTutorial = new CTutorial;

	if (FAILED(pTutorial->Init()))
	{
		return nullptr;
	}

	return pTutorial;
}



//=====================================
// プライベート関数
//=====================================


//オブジェクトの配置処理
void CTutorial::SetObject(void)
{
	CCoin* pCoin = nullptr;
	CLavaFloor* pLava = nullptr;
	CMeshfield* pField = nullptr;

	for (int nCnt = 0; nCnt < 18; nCnt++)
	{
		pCoin = CCoin::Create(D3DXVECTOR3(-100.0f + 25.0f * (nCnt % 9) ,-90.0f ,-1350.0f + 100.0f * (nCnt / 9)), (CCoin::COIN_TYPE)CObject::random((int)CCoin::COIN_0, (int)CCoin::COIN_3));
		pCoin->SetShadowHeight(-99.0f);
	}

	CSpikeBall::Create(D3DXVECTOR3(-75.0f, -135.0f, -600.0f));
	CSpikeBall::Create(D3DXVECTOR3(-25.0f, -135.0f, -600.0f));
	CSpikeBall::Create(D3DXVECTOR3( 25.0f, -135.0f, -600.0f));
	CSpikeBall::Create(D3DXVECTOR3( 75.0f, -135.0f, -600.0f));

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pLava = CLavaFloor::Create(D3DXVECTOR3(-110.0f + 50.0f * (nCnt), -149.0f, -400.0f));
		pLava->SetShadowDraw(false);
		pLava = CLavaFloor::Create(D3DXVECTOR3( 110.0f - 50.0f * (nCnt), -149.0f, -400.0f));
		pLava->SetShadowDraw(false);
	}


	pField = CMeshfield::Create(D3DXVECTOR3(-135.0f, -149.9f, 100.0f), Vec3Null, D3DXVECTOR2(30.0f, 70.0f), 6, 10, 0.01f);
	pField->SetTexture(CObject::TEXTURE_ICE);
	pField->ChangeHeight(0, 100, 0.1f);
	pField->SetPriority(0);

	CBouncePole::Create(D3DXVECTOR3(-85.0f, -150.0f, 0.0f));
	CBouncePole::Create(D3DXVECTOR3(-85.0f, -150.0f, -200.0f));

	CTrampoline::Create(D3DXVECTOR3(-75.0f, -150.0f, -100.0f));
	CTrampoline::Create(D3DXVECTOR3(-25.0f, -150.0f, -100.0f));
	CTrampoline::Create(D3DXVECTOR3( 25.0f, -150.0f, -100.0f));
	CTrampoline::Create(D3DXVECTOR3( 75.0f, -150.0f, -100.0f));

	CStoneSpawner::Create(D3DXVECTOR3(0.0f, 150.0f, 400.0f), -149.9f, 135.0f, 100.0f, 30);

	//pCoin = CCoin::Create(D3DXVECTOR3(), (CCoin::COIN_TYPE)CObject::random((int)CCoin::COIN_0, (int)CCoin::COIN_3));
}