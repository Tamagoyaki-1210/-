//=============================================================================
//
// ライフ
// Author : tanimoto kosuke
//
//=============================================================================
#include "life.h"
#include "application.h"
#include "texture.h"
#include "gauge.h"

//=====================================
//デフォルトコンストラクタ
//=====================================
CLife::CLife() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//デストラクタ
//=====================================
CLife::~CLife()
{

}

//=====================================
//初期化処理
//=====================================
void CLife::Init()
{
	CObject2D::Init();

	m_fWidth = LIFE_WIDTH;
	m_fHeight = LIFE_HEIGHT;

	//オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);

	//ゲージより前に描画された場合、透明にする
	CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
}

//=====================================
//終了処理
//=====================================
void CLife::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CLife::Update()
{
	CObject2D::Update();

	//位置取得
	D3DXVECTOR3 pos = CObject2D::GetPos();

	SetPos(pos);
}

//=====================================
//描画処理
//=====================================
void CLife::Draw()
{
	//画像取得処理
	CApplication::GetTexture()->TextureType(CTexture::TYPE_LIFE);
	CObject2D::Draw();
}


//=====================================
//位置設定処理
//=====================================
void  CLife::SetLife(int life, int maxlife, int fastmaxlife)
{
	m_pGauge->SetGauge(life, maxlife, fastmaxlife);
	CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f));
}

//=====================================
//生成処理
//=====================================
CLife *CLife::Create(D3DXVECTOR3 pos)
{
	CLife* pLife = new CLife;	//敵キャラポインタ

	if (pLife != nullptr)
	{
		pLife->Init();
		pLife->SetPos(pos);
		pLife->m_pGauge = CGauge::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f));
		pLife->CObject2D::SetUV();
	}
	return pLife;
}