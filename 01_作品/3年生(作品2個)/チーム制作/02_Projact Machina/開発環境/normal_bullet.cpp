//==============================================================================================
//
// 通常弾　　　normal_bullet.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"normal_bullet.h"
#include"object3D.h"
#include "application.h"
#include"enemy_manager.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "player_manager.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CNormal_Bullet::CNormal_Bullet(const PRIORITY priority) : CBullet(priority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CNormal_Bullet::~CNormal_Bullet()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CNormal_Bullet::Init()
{
	CBullet::Init();

	// 弾が敵に届くまでの時間
	float fHitCnt = m_fHypotenuse / GetSpeed_XZ();

	if (m_bTarget && pChara != nullptr)
	{
		// ターゲットしている敵の位置
		D3DXVECTOR3 Enemy_Pos = pChara->GetCenterPos();
		// 移動量
		D3DXVECTOR3 Enemy_Move = pChara->GetMove();

		// 弾が届くときの敵の位置
		Enemy_Pos.x += (Enemy_Move.x * m_fSpeed * fHitCnt);
		Enemy_Pos.z += (Enemy_Move.z * m_fSpeed * fHitCnt);

		// 弾から敵までのベクトル
		D3DXVECTOR3 Mob_Vec = Enemy_Pos - GetPos();
		D3DXVec3Normalize(&Mob_Vec, &Mob_Vec);
		SetMove(Mob_Vec);
	}

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CNormal_Bullet::Uninit()
{
	CBullet::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CNormal_Bullet::Update()
{
	CBullet::Update();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CNormal_Bullet::Draw()
{
	CBullet::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CNormal_Bullet *CNormal_Bullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, D3DXVECTOR3 move, float hypotenuse, CCharacter *chara, float enemy_speed, bool target, const bool side, const int power, const float speed, const int life)
{
	//クラスの生成
	CNormal_Bullet* pNormal_Bullet = new CNormal_Bullet(PRIORITY_BACK);

	//nullチェック
	if (pNormal_Bullet != nullptr)
	{
		pNormal_Bullet->SetPos(pos);
		pNormal_Bullet->SetSize(size);
		pNormal_Bullet->SetMove(move);
		pNormal_Bullet->SetPlayerSide(side);
		pNormal_Bullet->SetPower(power);
		pNormal_Bullet->m_fHypotenuse = hypotenuse;
		pNormal_Bullet->pChara = chara;
		pNormal_Bullet->m_fSpeed = enemy_speed;
		pNormal_Bullet->SetSpeed(speed);
		pNormal_Bullet->SetLife(life);
		pNormal_Bullet->m_bTarget = target;
		pNormal_Bullet->Init();

		// 弾オブジェクトの生成
		pNormal_Bullet->SetObj3D(CObject3D::Create(pos, size, PRIORITY_BACK));
		// 弾のテクスチャ
		pNormal_Bullet->GetObj3D()->SetTexture(CTexture::TEXTURE_BULLET);

		// ビルボードの設定
		pNormal_Bullet->GetObj3D()->Setbillboard(true);
	}
	else
		assert(false);

	return pNormal_Bullet;
}