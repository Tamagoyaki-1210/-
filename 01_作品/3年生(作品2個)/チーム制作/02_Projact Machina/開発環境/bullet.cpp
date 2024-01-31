//=============================================================================
//
// 弾.cpp (Bullet.cpp)
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "bullet.h"
#include "game.h"
#include "meshfield.h"
#include "object3D.h"
#include "objectX.h"
#include "particle_emitter.h"
#include "tutorial.h"
#include "normal_explosion.h"
#include "map_object_manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
const float CBullet::BULLET_SPEED_XZ = 50.0f;				// 弾の速度
const float CBullet::BULLET_SPEED_Y = 50.0f;				// 弾の速度
const float CBullet::BULLET_COLLISION_RADIUS = 30.0f;	// 弾の当たり判定の大きさ
//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet(const CObject::PRIORITY priority) : CMove_Object(priority)
{
	m_fSpeed_XZ = BULLET_SPEED_XZ;
	m_fSpeed_Y = BULLET_SPEED_Y;
	SetRadius(BULLET_COLLISION_RADIUS);
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init()
{
	// タグの設定
	SetTag(TAG_BULLET);

	// 当たり判定のタイプ
	SetCollision_Type(CMove_Object::COLLISION_TYPE_SHERER);

	// サイズの設定
	CMove_Object::SetSize({ GetRadius(),GetRadius(),GetRadius() });

	// オブジェクト3Dの初期化処理
	CMove_Object::Init();

	//==================================================
	// メンバ変数の初期化
	//==================================================
	// 当たり判定の生成
	SetCollision();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit()
{
	// 3Dオブジェクトが使用されている場合
	if (m_pObj3D != nullptr)
	{
		m_pObj3D->Uninit();
		m_pObj3D = nullptr;
	}
	if (m_pObjX != nullptr)
	{
		m_pObjX->Uninit();
		m_pObjX = nullptr;
	}

	// オブジェクト3Dの終了処理
	CMove_Object::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update()
{
	CMove_Object::Update();

	// 取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR2 size = GetSize();

	//前回の位置を保存
	m_nPosOld = pos;

	SetPosOld(m_nPosOld);

	//弾の位置更新
	pos.x += move.x * m_fSpeed_XZ;
	pos.z += move.z * m_fSpeed_XZ;
	pos.y += move.y * m_fSpeed_Y;

	// 攻撃パーティクル
	std::move(CParticleEmitter::Create("Locus", pos));


	// 位置の設定
	SetPos(pos);

	// オブジェクト3Dの位置を設定
	if (m_pObj3D != nullptr)
	{
		m_pObj3D->SetPos(pos);
		m_pObj3D->SetSize(size);
	}
	if (m_pObjX != nullptr)
	{
		m_pObjX->SetPos(pos);
	}

	// 寿命を減らす
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
		return;
	}

	// 床の当たり判定
	FieldCollision();

	// マップオブジェクトの当たり判定
	Map_Object_Collision();
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw()
{

}

//============================================================================
// 床の当たり判定
//============================================================================
void CBullet::FieldCollision()
{
	// 現在の位置を定数として取得
	const D3DXVECTOR3 pos = GetPos();

	CMeshField* pMeshField = nullptr;

	if(CApplication::GetModeType() == CApplication::MODE_GAME)
		pMeshField = CGame::GetMeshField();
	else if(CApplication::GetModeType() == CApplication::MODE_TUTORIAL)
		pMeshField = CTutorial::GetMeshField();

	if (pMeshField != nullptr)
	{
		// 床の当たり判定から高さを定数として取得
		const float a = pMeshField->MeshCollision(pos);

		// メッシュフィールドより下の位置にいる場合
		if (a >= pos.y)
		{
			//pMeshField->Ground_Broken(pos, 50.0f, 5);

			// 弾を破壊する
			Destroy();
		}
	}
}

//=============================================================================
// 被弾処理
//=============================================================================
void CBullet::Hit(CMove_Object* pHit)
{
	// 弾では無い場合 && 同じサイドではない場合
	if ((pHit->GetTag() == TAG_CHARACTER && GetPlayerSide() != pHit->GetPlayerSide())
		|| pHit->GetTag() == TAG_MAP_OBJECT)
	{
		Destroy();
	}
}

//=============================================================================
// 破壊処理
//=============================================================================
void CBullet::Destroy()
{
	if (m_bExplosion)
	{
		// 着弾時の爆発
		CNormal_Explosion::Create(GetPos(), 500, 70, GetPlayerSide(), CObject::PRIORITY_BACK);
	}

	// 攻撃パーティクル
	std::move(CParticleEmitter::Create("Attack", GetPos()));

	Uninit();
}

//=============================================================================
// マップオブジェクトとの当たり判定
//=============================================================================
void CBullet::Map_Object_Collision()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	CMap_Object_Manager *pMap_Object_Manager = nullptr;

	// モード毎にキャラを読み込む
	if (Mode == CApplication::MODE_TUTORIAL)
		pMap_Object_Manager = CTutorial::GetMap_Object_Manager();
	else if (Mode == CApplication::MODE_GAME)
		pMap_Object_Manager = CGame::GetMap_Object_Manager();

	D3DXVECTOR3 MapObjec_Pos = {};
	D3DXVECTOR3 ObjVec = {};

	// マップオブジェクトの情報
	for (auto pMap_Object : pMap_Object_Manager->GetAllMapObject())
	{
		if (pMap_Object->GetCollisionNoneHit())
		{
			// 弾の位置の取得
			MapObjec_Pos = pMap_Object->GetCenterPos();

			D3DXVECTOR3 Max = pMap_Object->GetObjectX()->GetMaxSize();
			D3DXVECTOR3 Min = pMap_Object->GetObjectX()->GetMinSize();

			// 弾からマップオブジェクトまでのベクトル
			ObjVec = GetPos() - MapObjec_Pos;

			// 弾からマップオブジェクトまでの距離
			float Distance = sqrtf(ObjVec.x * ObjVec.x + ObjVec.z * ObjVec.z);

			// 弾から近い
			if (Distance < Max.x * 1.5f + GetRadius() || Distance < Max.z * 1.5f + GetRadius()
				|| Distance < Min.x * 1.5f + GetRadius() || Distance < Min.z * 1.5f + GetRadius())
				pMap_Object->SetCollisionNoneHit(false);
			// 弾から遠い
			else
				pMap_Object->SetCollisionNoneHit(true);
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CBullet* CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, D3DXVECTOR3 move, const bool side, const int power, const float speed, const int life)
{
	//クラスの生成
	CBullet* pBullet = new CBullet(PRIORITY_BACK);

	//nullチェック
	if (pBullet != nullptr)
	{
		//初期化処理
		pBullet->Init();
		//設定処理
		pBullet->SetPos(pos);
		pBullet->SetMove(move);
		pBullet->SetSize(size);
		pBullet->SetPlayerSide(side);
		pBullet->SetPower(power);
		pBullet->SetSpeed(speed);
		pBullet->SetLife(life);

		// 弾オブジェクトの生成
		pBullet->m_pObj3D = CObject3D::Create(pos, size, PRIORITY_BACK);
		// 弾のテクスチャ
		pBullet->m_pObj3D->SetTexture(CTexture::TEXTURE_BULLET);

		// ビルボードの設定
		pBullet->m_pObj3D->Setbillboard(true);
	}
	else
	{
		assert(false);
	}

	return pBullet;
}