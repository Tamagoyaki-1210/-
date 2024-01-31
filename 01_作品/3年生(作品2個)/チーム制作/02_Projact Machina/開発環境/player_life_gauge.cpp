//==============================================================================================
//
// プレイヤーの体力ゲージ	　　player_life_gauge.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"player_life_gauge.h"
#include"player_manager.h"
#include"application.h"
#include "tutorial.h"
#include "game.h"
#include "object2D.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CPlayer_Life_Gauge::CPlayer_Life_Gauge()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CPlayer_Life_Gauge::~CPlayer_Life_Gauge()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CPlayer_Life_Gauge::Init()
{
	CGauge_Manager::Init();

	const D3DXVECTOR3 pos = GetGaugePos();
	const D3DXVECTOR2 size = GetGaugeSize();

	// 後ろのゲージ
	m_BackGauge = CObject2D::Create(pos, size, PRIORITY_FRONT);
	// 後ろのゲージの色
	m_BackGauge->SetCol({ 0.0f,0.0f,0.0f,1.0f });

	// 前方のゲージ
	m_FrontGauge = CObject2D::Create(pos, size, PRIORITY_FRONT);
	// 前方のゲージの色
	m_FrontGauge->SetCol({ 0.0f,1.0f,1.0f,1.0f });

	// ゲージの元の長さ
	SetBeaseSize((int)GetGaugeSize().y);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CPlayer_Life_Gauge::Uninit()
{
	// 後ろのゲージが使用中の場合
	if (m_BackGauge != nullptr)
	{
		m_BackGauge->Uninit();
		m_BackGauge = nullptr;
	}

	// 前方のゲージが使用中の場合
	if (m_FrontGauge != nullptr)
	{
		m_FrontGauge->Uninit();
		m_FrontGauge = nullptr;
	}

	CGauge_Manager::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CPlayer_Life_Gauge::Update()
{
	CGauge_Manager::Update();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CPlayer_Life_Gauge::Draw()
{
	CGauge_Manager::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CPlayer_Life_Gauge *CPlayer_Life_Gauge::Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size)
{
	CPlayer_Life_Gauge *pPlayer_Life_Gauge = new CPlayer_Life_Gauge;

	if (pPlayer_Life_Gauge != nullptr)
	{
		pPlayer_Life_Gauge->SetGaugePos(pos);
		pPlayer_Life_Gauge->SetGaugeSize(size);
		pPlayer_Life_Gauge->Init();
	}

	return pPlayer_Life_Gauge;
}

//==============================================================================================
// ゲージの増減
//==============================================================================================
void CPlayer_Life_Gauge::Fluctuation()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	CPlayer* pPlayer = nullptr;

	// モード毎にプレイヤーを読み込む
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
	}

	if (pPlayer != nullptr)
	{
		// 現在の体力の割合
		m_fLife_Percent = (float)pPlayer->GetLife() / GetBeaseLife() * 100;

		// ゲージサイズを同じ割合にする
		float Gauge_Percent = GetGaugeSize().y * m_fLife_Percent / 100;

		// ゲージの増減
		m_FrontGauge->SetSubSize({ 0.0f, -GetBeaseSize() + Gauge_Percent });
	}

	// 色の設定
	Col();
}

//==============================================================================================
// 色の設定
//==============================================================================================
void CPlayer_Life_Gauge::Col()
{
	// 現在の体力の割合
	if (m_fLife_Percent <= 25.0f)
		m_FrontGauge->SetCol({ 1.0f,0.0f,0.0f,1.0f });
	else
		m_FrontGauge->SetCol({ 0.0f,1.0f,1.0f,1.0f });
}

//==============================================================================================
// ゲージ描画の設定
//==============================================================================================
void CPlayer_Life_Gauge::SetDrawGauge(bool draw)
{
	m_BackGauge->SetDrawFlag(draw);
	m_FrontGauge->SetDrawFlag(draw);
}