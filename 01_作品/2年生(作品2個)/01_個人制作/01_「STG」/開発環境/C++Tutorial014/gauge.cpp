//=============================================================================
//
// ゲージ
// Author : tanimoto kosuke
//
//=============================================================================
#include "gauge.h"
#include "application.h"
#include "texture.h"
#include "life.h"

//=====================================
//デフォルトコンストラクタ
//=====================================
CGauge::CGauge() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//デストラクタ
//=====================================
CGauge::~CGauge()
{

}

//=====================================
//初期化処理
//=====================================
void CGauge::Init()
{
	CObject2D::Init();

	m_fWidth = GAUGE_WIDTH;
	m_fHeight = GAUGE_HEIGHT;

	//オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);

	CObject2D::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.8f));
}

//=====================================
//終了処理
//=====================================
void CGauge::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CGauge::Update()
{
	CObject2D::Update();

	//UV設定
	CObject2D::SetUV();

	//位置取得
	m_pos = CObject2D::GetPos();

	SetPos(m_pos);
}

//=====================================
//描画処理
//=====================================
void CGauge::Draw()
{
	//画像取得処理
	CApplication::GetTexture()->TextureType(CTexture::TYPE_GAUGE);
	CObject2D::Draw();
}

//=====================================
//ゲージ設定処理
//=====================================
void CGauge::SetGauge(int life, int maxlife, int fastmaxlife)
{
	int nDeffMaxLife = maxlife - fastmaxlife;	//最大体力分ゲージを横に伸ばす
	float fGauge = (float)life / (float)maxlife;
	float fWidth = (m_fWidth + (float)nDeffMaxLife) * fGauge;

	m_move.x = m_fWidth - fWidth;
	float fLength = sqrtf((fWidth * fWidth) + (m_fHeight * m_fHeight)) * 0.5f;
	CObject2D::SetLength(fLength);

	float fAngle = atan2f(fWidth, m_fHeight);
	CObject2D::SetAngle(fAngle);

	m_pos = m_basepos - m_move / 2.0f;
	SetPos(m_pos);

	if (nDeffMaxLife > 0)
	{
		CObject2D::SetColor(D3DXCOLOR(0.5f, 1.0f, 0.5f, 0.8f));
	}
}

//=====================================
//生成処理
//=====================================
CGauge *CGauge::Create(D3DXVECTOR3 pos)
{
	CGauge* pGauge = new CGauge;	//敵キャラポインタ

	if (pGauge != nullptr)
	{
		pGauge->Init();
		pGauge->SetPos(pos);
		pGauge->m_basepos = pos;
	}
	return pGauge;
}