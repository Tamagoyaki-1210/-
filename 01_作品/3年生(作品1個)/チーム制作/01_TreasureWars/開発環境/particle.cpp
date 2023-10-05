#include "application.h"
#include "renderer.h"
#include "particle.h"
#include "trail.h"
#include "utility.h"
#include <algorithm>

namespace
{
	// パーティクルファイルのテクスチャタグ用
	const std::vector<std::string> strPath = { "STAR", "FLARE", "SMOKE", "BRIGHT", "BRIGHT2", "COIN", "JEWEL", "SOUL", "RAY",
	"UP"};
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// パーティクルのコンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
CParticle::CParticle(int nPriority) : CObject3D(nPriority),
	m_info({}),
	m_nTime(0)			// 時間
{
	m_mul = 0.25f;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// パーティクルのデストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
CParticle::~CParticle()
{
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// パーティクルの生成
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
CParticle * CParticle::Create(SInfo& info, const std::string url)
{
	CParticle *pParticle = new CParticle();

	if (pParticle != nullptr)
	{
		pParticle->SetInfo(info);
		pParticle->SetPath(url);
		pParticle->Init();
	}
	else
	{
		assert(false);
	}

	return pParticle;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// パーティクルの初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT CParticle::Init()
{
	// テクスチャの読み込み
	processPath();

	// オブジェクトの初期化
	CObject3D::Init();

	CObject3D::Setbillboard(true);
	CObject3D::SetPos(m_info.pos);
	CObject3D::SetCol(m_info.col);
	CObject3D::SetSize(m_info.scale);

	D3DXMATRIX mtx = CObject3D::GetMtxWorld();
	m_DestroyTime = m_info.popTime;

	if (m_info.useTrail)
	{
		m_pTrail = m_pTrail->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), mtx);
	}

	if (m_info.useRandom)
	{	// ランダムな値を使用
		m_info.weight = utility::Random<float>(m_info.random.randomWeight.x, m_info.random.randomWeight.y);
		m_info.velocity = D3DXVECTOR3(utility::Random<float>(m_info.random.randomVelocityMax.x, m_info.random.randomVelocityMin.x),
			utility::Random<float>(m_info.random.randomVelocityMax.y, m_info.random.randomVelocityMin.y),
			utility::Random<float>(m_info.random.randomVelocityMax.z, m_info.random.randomVelocityMin.z));
	}

	if (m_info.random.randomRotate != D3DXVECTOR2(0.0f, 0.0f))
	{	// ランダムな回転量を使用
		m_info.rotateValue = utility::Random<float>(m_info.random.randomRotate.y, m_info.random.randomRotate.x);
	}

	return S_OK;
}

void CParticle::Uninit()
{
	if (m_pTrail != nullptr)
	{
		//m_pTrail->Release();
		m_pTrail->Uninit();
		m_pTrail = nullptr;
	}

	CObject3D::Uninit();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// パーティクルの更新
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticle::Update()
{
	// テクスチャの読み込み
	processPath();

	CObject3D::Update();

	D3DXVECTOR3 pos = CObject3D::GetPos();
	D3DXVECTOR3 move = m_info.velocity;
	D3DXVECTOR3 scale = CObject3D::GetScale();
	D3DXCOLOR col = CObject3D::GetCol();
	D3DXMATRIX mtx = CObject3D::GetMtxWorld();

	// テクスチャの大きさ
	scale += m_info.scalingValue;

	// 落下
	m_info.velocity.y -= m_info.weight;

	if (m_info.rotate.useRotate)
	{
		m_info.rotate.randomMax = 10.0f;
		m_info.rotate.randomMin = 7.0f;
		m_info.rotate.radius = 0.1f;

		move = D3DXVECTOR3(m_info.velocity.x + (m_info.rotate.angle * m_info.rotate.radius) * sinf(D3DXToRadian(m_info.rotate.angle)),
			move.y,
			m_info.velocity.z + (m_info.rotate.angle * m_info.rotate.radius) * cosf(D3DXToRadian(m_info.rotate.angle))) * m_mul;

		m_info.rotate.angle += utility::Random(m_info.rotate.randomMax, m_info.rotate.randomMin);
	}

	// 色関係
	if (m_info.destCol.r >= 0.0f && m_info.destCol.g >= 0.0f && m_info.destCol.b >= 0.0f)
	{
		col.r += (m_info.destCol.r - col.r) / (m_info.popTime * m_info.colAttenuation);
		col.g += (m_info.destCol.g - col.g) / (m_info.popTime * m_info.colAttenuation);
		col.b += (m_info.destCol.b - col.b) / (m_info.popTime * m_info.colAttenuation);
	}
	if (m_info.destCol.a >= 0.0f)
	{
		col.a += (m_info.destCol.a - col.a) / (m_info.popTime * m_info.colAttenuation);
	}
	move += m_info.velocity * 0.4f;

	// 位置の更新
	pos += move;

	CObject3D::SetPos(pos);
	CObject3D::SetSize(scale);
	CObject3D::SetCol(col);

	m_nTime++;

	if (m_pTrail != nullptr)
	{
		m_pTrail->SetColor(m_info.destCol, m_info.col);
		m_pTrail->SetMtxParent(mtx);
	}

	m_DestroyTime--;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// パーティクルの描画
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticle::Draw()
{
	// デバイスのポインタを取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// 合成モードの設定
	Blending(m_info.blendState);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Zテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	CObject3D::Draw();

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// Zバッファの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// 設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// ブレンド設定処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticle::Blending(EBlend state)
{
	switch (state)
	{
	case Blend_Additive:
		Additive();
		break;

	case Blend_Subtractive:
		Subtractive();
		break;

	case Blend_Normal:
		DefaultBlending();
		break;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 通常のブレンド処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticle::DefaultBlending()
{
	//頂点バッファの生成
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 加算合成処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticle::Additive()
{
	//頂点バッファの生成
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 減算合成処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticle::Subtractive()
{
	//頂点バッファの生成
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void CParticle::processPath()
{
	// テクスチャcppの構造を変えたらこの処理消えるけどめんどいのでこれで
	auto it = std::find(strPath.begin(), strPath.end(), m_path);
	if (it == strPath.end())
	{
		return;
	}

	switch (std::distance(strPath.begin(), it))
	{
	case 0:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_STAR);
		break;

	case 1:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_FLARE);
		break;

	case 2:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_SMOKE);
		break;

	case 3:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_BRIGHTA);
		break;

	case 4:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_BRIGHTB);
		break;

	case 5:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_COIN);
		break;

	case 6:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_JEWEL);
		break;

	case 7:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_SOUL);
		break;

	case 8:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_RAY);
		break;

	case 9:
		CObject3D::SetTexture(CTexture::TEXTURE_EFFECT_UP);
		break;
	}
}