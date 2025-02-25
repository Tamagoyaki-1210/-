//==============================================================================================
//
// ゲージ　　　gauge.cpp
// tutida ryousei
//
//==============================================================================================
#include"gauge.h"

bool CGauge::m_bUse = false;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CGauge::CGauge()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CGauge::~CGauge()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CGauge::Init()
{
	m_fGauge_Heal = 1.0f;				// ゲージの回復速度
	m_bZero = false;					// ゲージがゼロになった

	// サイズの設定
	m_Size = m_Base_Size;

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CGauge::Uninit()
{
	// 使用中オブジェクトの終了
	if (m_pBar != nullptr)
	{
		m_pBar->Uninit();
	}
	if (m_pGauge != nullptr)
	{
		m_pGauge->Uninit();
	}

	m_bUse = false;

	CUi::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CGauge::Update()
{
	// 現在の横幅より元の横幅の方が大きい場合
	if (m_Size.x < m_Base_Size.x)
	{
		// ゲージを増やす
		GaugeUp();
	}

	m_pGauge->SetGaugeSize(m_Base_Size.x, m_Size.x);

	CUi::Update();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CGauge::Draw()
{

}

//==============================================================================================
// 生成処理
//==============================================================================================
CGauge *CGauge::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CGauge *pGauge = nullptr;

	pGauge = new CGauge;

	if (pGauge != nullptr)
	{
		pGauge->m_pBar = CObject2D::Create(pos, size);
		pGauge->m_pBar->SetCol({ 0.0f, 0.0f, 0.0f, 1.0f });
		pGauge->AddObj2D(pGauge->m_pBar);

		pGauge->m_pGauge = CObject2D::Create(pos, size);
		pGauge->m_pGauge->SetCol({ 1.0f,1.0f,0.0f,1.0f });
		pGauge->AddObj2D(pGauge->m_pGauge);

		pGauge->SetBase_Size({ size.x,size.y,0.0f });
		pGauge->Init();
	}

	return pGauge;
}

//==============================================================================================
// ゲージを0にする
//==============================================================================================
void CGauge::ZeroGauge()
{
	if (m_Size.x >= m_Base_Size.x
		&& !m_bUse)
	{
		m_Size.x -= m_Base_Size.x * 2;
		m_bZero = true;
	}

	// 誰かがスキルを使ったら全員にクールタイム
	m_bUse = true;

	//SetUiUpdate(true);
}

//==============================================================================================
// ゲージを増やす
//==============================================================================================
void CGauge::GaugeUp()
{
	// ゲージを増やす
	m_Size.x += m_fGauge_Heal;

	// 増加後のゲージの横幅が元のゲージの横幅以上の大きさになった場合
	if (m_Size.x >= m_Base_Size.x
		&& m_bZero)
	{
		// ゲージピッタリに代入する
		m_Size.x = m_Base_Size.x;
		m_bZero = false;
		m_bUse = false;
	}
	//SetUiUpdate(true);
}