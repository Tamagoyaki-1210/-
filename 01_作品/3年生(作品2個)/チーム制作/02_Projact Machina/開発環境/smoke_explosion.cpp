//==============================================================================================
//
// 煙爆発 		smoke_explosion.cpp
// tutida ryousei
//
//==============================================================================================
#include"smoke_explosion.h"
#include "particle_emitter.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CSmoke_Explosion::CSmoke_Explosion(const PRIORITY priority) : CExplosion(priority)
{
	m_nContinuation = 0;
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CSmoke_Explosion::~CSmoke_Explosion()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CSmoke_Explosion::Init()
{
	CExplosion::Init();

	// 当たり判定のヒット処理を読み込まない判定の設定
	SetCollisionNoneHit(true);

	// 当たり判定の生成
	SetDelayCollision();

	// 攻撃パーティクル
	std::move(CParticleEmitter::Create("Attack", { GetPos().x,GetPos().y + 100,GetPos().z }));

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CSmoke_Explosion::Uninit()
{
	CMove_Object::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CSmoke_Explosion::Update()
{
	CMove_Object::Update();

	// 爆発の持続時間
	if (m_nContinuation < Explosion_Timer)
	{
		m_nContinuation++;
	}
	else
	{
		Uninit();
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CSmoke_Explosion::Draw()
{
}

//==============================================================================================
// 生成処理
//==============================================================================================
CSmoke_Explosion *CSmoke_Explosion::Create(D3DXVECTOR3 pos, float radius, int power, const bool side, const CObject::PRIORITY priority)
{
	CSmoke_Explosion *pExplosion = new CSmoke_Explosion(priority);

	if (pExplosion != nullptr)
	{
		pExplosion->SetPos(pos);
		pExplosion->SetRadius(radius);
		pExplosion->SetPlayerSide(side);
		pExplosion->SetPower(power);
		pExplosion->Init();
	}

	return pExplosion;
}