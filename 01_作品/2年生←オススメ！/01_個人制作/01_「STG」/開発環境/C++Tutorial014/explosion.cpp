//=============================================================================
//
// 爆発
// Author : tanimoto kosuke
//
//=============================================================================
#include "explosion.h"
#include "application.h"
#include "texture.h"
#include "stage.h"

//---------------------------
//マクロ関数
//---------------------------
#define EXPLOSION_ANIM_X		(8)
#define EXPLOSION_ANIM_Y		(1)

//=====================================
//デフォルトコンストラクタ
//=====================================
CExplosion::CExplosion() : CObject2D(EFFECT_PRIORITY_BACK), m_nLife(0)
{

}

//=====================================
//デストラクタ
//=====================================
CExplosion::~CExplosion()
{

}

//=====================================
//初期化処理
//=====================================
void CExplosion::Init()
{
	CObject2D::Init();
	m_nLife = 30;

	SetColor(D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f));
	//オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_EFFECT, m_fWidth, m_fHeight);
} 

//=====================================
//終了処理
//=====================================
void CExplosion::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CExplosion::Update()
{
	CObject2D::Update();

	//UV設定
	CObject2D::SetUV();

	CObject2D::SetAnim(EXPLOSION_ANIM_X, EXPLOSION_ANIM_Y,1);

	//弾移動
	m_move.x = -2.0f;

	//減衰
	m_move = m_move - (m_move * 0.05f);

	//位置取得
	D3DXVECTOR3 pos = GetPos();

	//位置更新
	pos += m_move;

	//位置設定処理
	SetPos(pos);

	//寿命計算
	if (m_nLife <= 0)
	{
		Uninit();
	}
	else
	{//寿命を減らす
		m_nLife--;
	}
}

//=====================================
//描画処理
//=====================================
void CExplosion::Draw()
{
	//画像取得処理
	CApplication::GetTexture()->TextureType(CTexture::TYPE_EXPLOSION);
	CObject2D::Draw();
}

//=====================================
//爆発生成処理
//=====================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CExplosion* pExplosion;	//爆発ポインタ

	pExplosion = new CExplosion;

	if (pExplosion != nullptr)
	{
		pExplosion->m_fWidth = fWidth;
		pExplosion->m_fHeight = fHeight;
		pExplosion->Init();
		pExplosion->SetPos(pos);
		if (CStage::GetBomm() == false)
		{
			CSound::PlaySound(SOUND_LABEL_SE_BULLET_LASER_DAMAGE);
		}
	}
	return pExplosion;
}