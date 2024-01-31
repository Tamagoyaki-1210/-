//==============================================================================================
//
// 曲射弾　　　parabola_bullet.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"parabola_bullet.h"
#include"objectX.h"
#include"application.h"
#include "utility.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CParabola_Bullet::CParabola_Bullet(const PRIORITY priority) : CBullet(priority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CParabola_Bullet::~CParabola_Bullet()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CParabola_Bullet::Init()
{
	CBullet::Init();

	// XZの弾の速度
	float Speed = m_fHyptenuse / HIT_COUNT;

	SetLife(1000);
	SetSpeed_XZ(Speed);
	SetSpeed_Y(80);
	SetPower(10);

	D3DXVECTOR3 Vec = GetMove();
	
	// Y座標のベクトル
	Vec.y = 1;

	D3DXVec3Normalize(&Vec, &Vec);

	SetMove(Vec);

	// 重力の設定
	m_fGravity= 0.03f;

	SetExplosion(true);
	
	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CParabola_Bullet::Uninit()
{
	CBullet::Uninit();
}

//==============================================================================================
// 更新化処理
//==============================================================================================
void CParabola_Bullet::Update()
{
	CBullet::Update();

	D3DXVECTOR3 Vec = GetMove();

	//重力で弾を落とす
	Vec.y -= m_fGravity;

	CObjectX *pLauncher = GetObjX();

	if (pLauncher != nullptr)
		// 角度の設定
		pLauncher->SetRot({ -Vec.y + D3DX_PI / 2,m_Rot.y - D3DX_PI,0.0f });

	D3DXVec3Normalize(&Vec, &Vec);

	SetMove(Vec);
}

//==============================================================================================
// 描画化処理
//==============================================================================================
void CParabola_Bullet::Draw()
{
	CBullet::Draw();
}

//=============================================================================
// 破壊処理
//=============================================================================
void CParabola_Bullet::Destroy()
{
	// 弾の破壊処理
	CBullet::Destroy();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CParabola_Bullet *CParabola_Bullet::Create(const D3DXVECTOR3 pos, D3DXVECTOR3 move, float hypotenuse, D3DXVECTOR3 rot, char *filename, const bool side, const int power, const float speed, const int life)
{
	CParabola_Bullet* pParabola_Bullet = nullptr;

	//クラスの生成
	pParabola_Bullet = new CParabola_Bullet(PRIORITY_BACK);

	if (pParabola_Bullet != nullptr)
	{
		pParabola_Bullet->SetPos(pos);
		pParabola_Bullet->SetMove(move);
		pParabola_Bullet->m_fHyptenuse = hypotenuse;
		pParabola_Bullet->m_Rot = rot;
		pParabola_Bullet->SetPlayerSide(side);
		pParabola_Bullet->SetPower(power);
		pParabola_Bullet->SetSpeed(speed);
		pParabola_Bullet->SetLife(life);
		pParabola_Bullet->Init();

		// 弾オブジェクトの生成
		pParabola_Bullet->SetObjX(CObjectX::Create(pos, { 0.0f,0.0f,0.0f }, nullptr, filename, PRIORITY_BACK));
	}
	else
		assert(false);

	return pParabola_Bullet;
}