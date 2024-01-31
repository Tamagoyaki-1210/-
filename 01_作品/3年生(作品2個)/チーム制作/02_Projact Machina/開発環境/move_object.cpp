//==============================================================================================
//
// 動くオブジェクト　　　move_object.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"move_object.h"
#include "application.h"
#include "tutorial.h"
#include "game.h"
#include "collision_manager.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CMove_Object::CMove_Object(const PRIORITY priority) : CObject(priority)
{
	m_tag = TAG_NONE;
	m_bPlayerSide = false;
	m_bCollision_Delay = false;		// 当たり判定の遅延設置判定
	m_bCollision_NoneHit = false;	// 当たり判定のヒット処理を読み込まない判定
	m_CenterPos = { 0.0f, 0.0f, 0.0f };
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CMove_Object::~CMove_Object()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CMove_Object::Init()
{
	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CMove_Object::Uninit()
{
	if (m_pCollision != nullptr)
	{
		m_pCollision->Uninit();
		m_pCollision = nullptr;
	}

	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CMove_Object::Update()
{
	// 当たり判定を遅延して設定する処理
	DelayCollision();

	if (m_pCollision != nullptr)
		m_pCollision->SetNoneHit(m_bCollision_NoneHit);
}

//==============================================================================================
// 当たり判定ポインタを消す処理
//==============================================================================================
void CMove_Object::DelayCollision()
{
	// 遅延して設定する場合
	if (m_bCollision_Delay == true)
	{
		SetCollision();
	}
}

//==============================================================================================
// 当たり判定ポインタを消す処理
//==============================================================================================
void CMove_Object::CollisionDestroy()
{
	if (m_pCollision != nullptr)
	{
		m_pCollision = nullptr;
	}
}

//==============================================================================================
// 当たり判定の生成
//==============================================================================================
void CMove_Object::SetCollision()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	CCollision_Manager* pManager = nullptr;

	// 生成時に自身のポインタを当たり判定マネージャーに設定
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pManager = CTutorial::GetCollision_Manager();
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pManager = CGame::GetCollision_Manager();
	}

	// 当たり判定マネージャーが使用されている場合 && 当たり判定が使用されていない場合
	if (pManager != nullptr && m_pCollision == nullptr)
	{
		m_pCollision = CCollision::Create(this);
		m_pCollision->SetNoneHit(m_bCollision_NoneHit);
	}
}
