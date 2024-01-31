//==============================================================================================
//
// 雑魚敵の体力ゲージ	　　mob_life_gauge.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"mob_life_gauge.h"
#include"game.h"
#include"character.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CMob_Life_Gauge::CMob_Life_Gauge()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CMob_Life_Gauge::~CMob_Life_Gauge()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CMob_Life_Gauge::Init()
{
	CGauge_Manager::Init();

	const D3DXVECTOR3 pos = GetGaugePos();
	const D3DXVECTOR2 size = GetGaugeSize();

	// ゲージの元の長さ
	SetBeaseSize((int)GetGaugeSize().x);

	// 後ろのゲージ
	m_BackGauge = CObject3D::Create(pos, size, PRIORITY_CENTER, { 0.0f,0.0f,0.0f,1.0f }, true);

	// 前方のゲージ
	m_FrontGauge = CObject3D::Create(pos, size, PRIORITY_FRONT, { 1.0f,1.0f,1.0f,1.0f }, true);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CMob_Life_Gauge::Uninit()
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
void CMob_Life_Gauge::Update()
{
	const D3DXVECTOR3 pos = GetGaugePos();

	m_BackGauge->SetPos(pos);
	m_FrontGauge->SetPos(pos);

	CGauge_Manager::Update();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CMob_Life_Gauge::Draw()
{
	CGauge_Manager::Draw();
}

//==============================================================================================
// ゲージの増減
//==============================================================================================
void CMob_Life_Gauge::Fluctuation()
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
void CMob_Life_Gauge::SetDrawGauge(bool draw)
{
	m_BackGauge->SetDrawFlag(draw);
	m_FrontGauge->SetDrawFlag(draw);
}

//==============================================================================================
// 生成処理
//==============================================================================================
CMob_Life_Gauge *CMob_Life_Gauge::Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size)
{
	CMob_Life_Gauge *pMob_Life_Gauge = new CMob_Life_Gauge;

	if (pMob_Life_Gauge != nullptr)
	{
		pMob_Life_Gauge->SetGaugePos(pos);
		pMob_Life_Gauge->SetGaugeSize(size);
		pMob_Life_Gauge->Init();
	}

	return pMob_Life_Gauge;
}