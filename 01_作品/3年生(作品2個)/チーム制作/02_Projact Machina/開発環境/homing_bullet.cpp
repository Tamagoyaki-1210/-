//==============================================================================================
//
// ホーミング弾　　　homing_bullet.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"homing_bullet.h"
#include"application.h"
#include"objectX.h"
#include"enemy_manager.h"
#include "game.h"
#include "tutorial.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CHoming_Bullet::CHoming_Bullet(const PRIORITY priority) : CBullet(priority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CHoming_Bullet::~CHoming_Bullet()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CHoming_Bullet::Init()
{
	CBullet::Init();

	SetLife(180);
	SetSpeed(30.0f);
	SetPower(30);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CHoming_Bullet::Uninit()
{
	CBullet::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CHoming_Bullet::Update()
{
	CBullet::Update();

	D3DXVECTOR3 Mob_Pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 BulletVec = { 0.0f,0.0f,0.0f };
	float fTarget_Scope = 2000.0f;
	float fNearDistance = fTarget_Scope;
	CEnemy* pNearEnemy = nullptr;

	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	CEnemyManager* pManager = nullptr;

	// モード毎に敵キャラを読み込む
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pManager = CTutorial::GetEnemyManager();
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pManager = CGame::GetEnemyManager();
	}
	// 雑魚敵の情報
	for (auto pEnemy : pManager->GetAllEnemy())
	{
		if (pEnemy->GetLife() > 0 && !m_bTarget)
		{
			// 敵の位置の取得
			Mob_Pos = pEnemy->GetCenterPos();

			// 弾から敵の距離
			BulletVec = Mob_Pos - GetPos();
			
			// 距離の算出
			float DistanceXZ = sqrtf(BulletVec.x * BulletVec.x
				+ BulletVec.z * BulletVec.z);

			// プレイヤーから敵の直線距離
			float m_fHypotenuse = sqrt((BulletVec.y * BulletVec.y) + (DistanceXZ * DistanceXZ));

			// 距離2000以上
			if (m_fHypotenuse > fTarget_Scope)
				continue;

			// 距離を比べる
			if (fNearDistance >= m_fHypotenuse)
			{
				// 短い方の距離と位置を代入
				fNearDistance = m_fHypotenuse;
				pNearEnemy = pEnemy;
				m_bTarget = true;
			}
		}
	}

	// 一番近い敵キャラが使用されている場合
	if (pNearEnemy != nullptr)
	{
		// ターゲットしている敵の位置
		D3DXVECTOR3 Enemy_Pos = pNearEnemy->GetPos();

		// 弾から敵までのベクトル
		D3DXVECTOR3 Mob_Vec = Enemy_Pos - GetPos();
		D3DXVec3Normalize(&Mob_Vec, &Mob_Vec);
		SetMove(Mob_Vec);
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CHoming_Bullet::Draw()
{
	CBullet::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CHoming_Bullet *CHoming_Bullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 nearmob_pos, char *filename, const bool side, const int power, const int life)
{
	//クラスの生成
	CHoming_Bullet* pHoming_Bullet = new CHoming_Bullet(PRIORITY_BACK);

	if (pHoming_Bullet != nullptr)
	{
		pHoming_Bullet->SetPos(pos);
		pHoming_Bullet->SetMove(move);
		pHoming_Bullet->SetPlayerSide(side);
		pHoming_Bullet->SetPower(power);
		pHoming_Bullet->SetLife(life);
		pHoming_Bullet->m_NearMob_Pos = nearmob_pos;
		pHoming_Bullet->Init();

		// 弾オブジェクトの生成
		pHoming_Bullet->SetObjX(CObjectX::Create(pos, rot, nullptr, filename, PRIORITY_BACK));
	}
	else
		assert(false);

	return pHoming_Bullet;
}