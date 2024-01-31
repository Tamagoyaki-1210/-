//==============================================================================================
//
// マップのオブジェクト　　　map_object.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"map_object.h"
#include"objectX.h"
#include"application.h"
#include"mode.h"
#include"enemy_manager.h"
#include"player_manager.h"
#include"game.h"
#include"tutorial.h"
#include"map_object_manager.h"
#include"drop_manager.h"
#include "particle_emitter.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CMap_Object::CMap_Object(const PRIORITY priority) : CMove_Object(priority)
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	// 生成時に自身のポインタをマップオブジェクトマネージャーに設定
	if (Mode == CApplication::MODE_TUTORIAL)
		CTutorial::GetMap_Object_Manager()->SetMapObject(this);
	else if (Mode == CApplication::MODE_GAME)
		CGame::GetMap_Object_Manager()->SetMapObject(this);
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CMap_Object::~CMap_Object()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	// マネージャーが使用中の場合、解放時に自身のポインタを配列から除外する
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CMap_Object_Manager* pManager = CTutorial::GetMap_Object_Manager();
		if (pManager != nullptr) pManager->DestroyMapObject(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CMap_Object_Manager* pManager = CGame::GetMap_Object_Manager();
		if (pManager != nullptr) pManager->DestroyMapObject(this);
	}
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CMap_Object::Init()
{
	CMove_Object::Init();

	// 当たり判定の設定
	SetCollision();

	// 当たり判定のタイプ
	SetCollision_Type(CMove_Object::COLLISION_TYPE_BLOCK);

	// タグの設定
	SetTag(TAG_MAP_OBJECT);

	CObjectX *Obj = GetObjectX();
	
	D3DXVECTOR3 Max = Obj->GetMaxSize();
	D3DXVECTOR3 Min = Obj->GetMinSize();

	SetIndex({ Min.x,0.0f,Min.z }, 0);
	SetIndex({ Min.x,0.0f,Max.z }, 1);
	SetIndex({ Max.x,0.0f,Max.z }, 2);
	SetIndex({ Max.x,0.0f,Min.z }, 3);

	SetCenterPos({ 0.0f,Max.y / 2,0.0f });

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CMap_Object::Uninit()
{
	CMove_Object::Uninit();

	GetObjectX()->Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CMap_Object::Update()
{
	CMove_Object::Update();

	//当たり判定の設定
	Collision_Hit();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CMap_Object::Draw()
{
}

//==============================================================================================
// ヒット処理
//==============================================================================================
void CMap_Object::Hit(CMove_Object* /*pHit*/)
{
	Uninit();

	std::move(CParticleEmitter::Create("smoke", GetPos()));

	// ビル倒壊SE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BREAKBILL);
}

//==============================================================================================
// 当たり判定の設定
//==============================================================================================
void CMap_Object::Collision_Hit()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	CEnemyManager *pEnemy_Manager = nullptr;
	CPlayer *pPlayer = nullptr;

	CDropManager *pDrop_Manager = nullptr;

	// モード毎にキャラを読み込む
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pEnemy_Manager = CTutorial::GetEnemyManager();
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
		pDrop_Manager = CTutorial::GetDropManager();
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pEnemy_Manager = CGame::GetEnemyManager();
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
		pDrop_Manager = CGame::GetDropManager();
	}

	D3DXVECTOR3 OtherPos = { 0.0f,0.0f,0.0f };

	// マップオブジェクトのサイズ
	D3DXVECTOR3 SizeMax = GetObjectX()->GetMaxSize();
	D3DXVECTOR3 SizeMin = GetObjectX()->GetMinSize();

	// マップオブジェクトの位置
	D3DXVECTOR3 ObjPos = GetPos();

	// キャラからマップオブジェクトまでのベクトル
	D3DXVECTOR3 ObjVec = { 0.0f,0.0f,0.0f };

	if (pPlayer != nullptr)
	{
		// プレイヤーの位置の取得
		OtherPos = pPlayer->GetPos();
		// 半径の取得
		float fPlayer_Radius = pPlayer->GetRadius();

		// プレイヤーからマップオブジェクトまでのベクトル
		ObjVec = ObjPos - OtherPos;

		// プレイヤーからマップオブジェクトまでの距離
		float Distance = sqrtf(ObjVec.x * ObjVec.x + ObjVec.z * ObjVec.z);

		// プレイヤーから近い
		if (Distance < SizeMax.x * 1.5f + fPlayer_Radius || Distance < SizeMax.z * 1.5f + fPlayer_Radius
			|| Distance < SizeMin.x * 1.5f + fPlayer_Radius || Distance < SizeMin.z * 1.5f + fPlayer_Radius)
			SetCollisionNoneHit(false);
		// プレイヤーから遠い
		else
			SetCollisionNoneHit(true);
	}

	// 敵の情報
	for (auto pEnemy : pEnemy_Manager->GetAllEnemy())
	{
		if (pEnemy->GetLife() > 0 && GetCollisionNoneHit())
		{
			// 敵の位置の取得
			OtherPos = pEnemy->GetCenterPos();
			// 半径の取得
			float fRadius = pEnemy->GetRadius();

			// 敵からマップオブジェクトまでのベクトル
			ObjVec = ObjPos - OtherPos;

			// 敵からマップオブジェクトまでの距離
			float Distance = sqrtf(ObjVec.x * ObjVec.x + ObjVec.z * ObjVec.z);

			// 敵から近い
			if (Distance < SizeMax.x * 1.5f + fRadius || Distance < SizeMax.z * 1.5f + fRadius
				|| Distance < SizeMin.x * 1.5f + fRadius || Distance < SizeMin.z * 1.5f + fRadius)
				SetCollisionNoneHit(false);
			// 敵から遠い
			else
				SetCollisionNoneHit(true);
		}
	}

	// ドロップパーツの情報
	for (auto pDrop : pDrop_Manager->GetAllDrop())
	{
		if (GetCollisionNoneHit())
		{
			// 位置の取得
			OtherPos = pDrop->GetPos();
			// 半径の取得
			float fRadius = pDrop->GetRadius();

			// ドロップパーツからマップオブジェクトまでのベクトル
			ObjVec = ObjPos - OtherPos;

			// ドロップパーツからマップオブジェクトまでの距離
			float Distance = sqrtf(ObjVec.x * ObjVec.x + ObjVec.z * ObjVec.z);

			// ドロップパーツから近い
			if (Distance < SizeMax.x * 1.5f + fRadius || Distance < SizeMax.z * 1.5f + fRadius
				|| Distance < SizeMin.x * 1.5f + fRadius || Distance < SizeMin.z * 1.5f + fRadius)
				SetCollisionNoneHit(false);
			// ドロップパーツから遠い
			else
				SetCollisionNoneHit(true);
		}
	}
}

//==============================================================================================
// 生成処理
//==============================================================================================
CMap_Object *CMap_Object::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, const PRIORITY priority)
{
	CMap_Object *pMap_Object = new CMap_Object(priority);

	if (pMap_Object != nullptr)
	{
		pMap_Object->SetObjectX(CObjectX::Create(pos, rot, parent, Xfilename, priority));
		pMap_Object->SetSize(pMap_Object->GetObjectX()->GetMaxSize());
		pMap_Object->SetPos(pos);
		pMap_Object->Init();
	}

	return pMap_Object;
}