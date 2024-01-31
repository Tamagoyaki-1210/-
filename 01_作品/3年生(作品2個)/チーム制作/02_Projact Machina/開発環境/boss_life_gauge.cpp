//==============================================================================================
//
// ボスの体力ゲージ	　　boss_life_gauge.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"boss_life_gauge.h"
#include"game.h"
#include"character.h"
#include "object2D.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CBoss_Life_Gauge::CBoss_Life_Gauge()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CBoss_Life_Gauge::~CBoss_Life_Gauge()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CBoss_Life_Gauge::Init()
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
	m_FrontGauge->SetCol({ 1.0f,0.0f,0.0f,1.0f });

	// ゲージの元の長さ
	SetBeaseSize((int)GetGaugeSize().x);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CBoss_Life_Gauge::Uninit()
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
void CBoss_Life_Gauge::Update()
{
	CGauge_Manager::Update();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CBoss_Life_Gauge::Draw()
{
	CGauge_Manager::Draw();
}

//==============================================================================================
// ゲージの増減
//==============================================================================================
void CBoss_Life_Gauge::Fluctuation()
{
	// 現在の体力の割合
	float Life_Percent = (float)GetLife() / GetBeaseLife() * 100;

	// ゲージサイズを同じ割合にする
	float Gauge_Percent = GetGaugeSize().x * Life_Percent / 100;

	// ゲージの増減
	m_FrontGauge->SetSubSize({ GetBeaseSize() - Gauge_Percent, 0.0f });
}

//==============================================================================================
// ゲージ描画の設定
//==============================================================================================
void CBoss_Life_Gauge::SetDrawGauge(bool draw)
{
	m_BackGauge->SetDrawFlag(draw);
	m_FrontGauge->SetDrawFlag(draw);
}

//==============================================================================================
// 生成処理
//==============================================================================================
CBoss_Life_Gauge *CBoss_Life_Gauge::Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size)
{
	CBoss_Life_Gauge *pBoss_Life_Gauge = new CBoss_Life_Gauge;
	
	if (pBoss_Life_Gauge != nullptr)
	{
		pBoss_Life_Gauge->SetGaugePos(pos);
		pBoss_Life_Gauge->SetGaugeSize(size);
		pBoss_Life_Gauge->Init();
	}

	return pBoss_Life_Gauge;
}