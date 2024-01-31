//==============================================================================================
//
// 砲台	　　cannon.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"cannon.h"
#include"objectX.h"
#include"restraint_switch.h"
#include"map.h"
#include"game.h"
#include"tutorial.h"
#include"enemy_manager.h"
#include"application.h"
#include "particle_emitter.h"

bool CCannon::m_bRestrain = false;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CCannon::CCannon(CObject::PRIORITY priority) : CMove_Object(priority)
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CCannon::~CCannon()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CCannon::Init()
{
	CMove_Object::Init();

	// 当たり判定の設定
	SetCollision();

	// 当たり判定のタイプ
	SetCollision_Type(CMove_Object::COLLISION_TYPE_BLOCK);

	// タグの設定
	SetTag(TAG_MAP_OBJECT);

	D3DXVECTOR3 Max = GetObjectX()->GetMaxSize();
	D3DXVECTOR3 Min = GetObjectX()->GetMinSize();

	SetIndex({ Min.x,0.0f,Min.z }, 0);
	SetIndex({ Min.x,0.0f,Max.z }, 1);
	SetIndex({ Max.x,0.0f,Max.z }, 2);
	SetIndex({ Max.x,0.0f,Min.z }, 3);

	SetCenterPos({ 0.0f,Max.y / 2,0.0f });
	SetSize(GetObjectX()->GetMaxSize());

	// 当たり判定を消す
	SetCollisionNoneHit(true);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CCannon::Uninit()
{
	CMove_Object::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CCannon::Update()
{
	CMove_Object::Update();

	// 拘束スイッチの取得
	CRestraint_Switch *pRestraint = CGame::GetMap()->GetRestraint_Switch(m_nIndex);

	if (pRestraint->GetPush())
		m_bTarget = true;

	float fRotDest = 0.0f;
	D3DXVECTOR3 Vec = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 Boss_Pos = { 0.0f,0.0f,0.0f };

	if (m_bTarget)
	{
		// 現在のモード
		CApplication::MODE Mode = CApplication::GetModeType();

		CEnemyManager* pEnemyManager = nullptr;

		// モード毎に敵キャラを読み込む
		if (Mode == CApplication::MODE_TUTORIAL)
			pEnemyManager = CTutorial::GetEnemyManager();
		else if (Mode == CApplication::MODE_GAME)
			pEnemyManager = CGame::GetEnemyManager();

		// 敵の情報
		for (auto pEnemy : pEnemyManager->GetAllEnemy())
		{
			if (pEnemy->GetEnemyType() == CEnemy::ENEMY_TYPE_BOSS
				&& pEnemy->GetLife() != 0)
			{
				Boss_Pos = pEnemy->GetCenterPos();
				Boss_Pos.y += 500.0f;

				// ボスまでのベクトル
				Vec = Boss_Pos - GetPos();

				// 目的の角度
				fRotDest = atan2(Vec.x, Vec.z);

				// 目的の角度にする
				m_fRot += (fRotDest - m_fRot) * 0.05f;

				// 砲台がボスの方を向く
				GetObjectX()->SetRot({ 0.0f,m_fRot,0.0f });

				// 砲台から敵の距離
				float Dis = sqrt(Vec.x * Vec.x + Vec.z * Vec.z);

				if (pRestraint->GetCountSwitch() == CRestraint_Switch::SWITHC_NUM
					&& m_pChain_Manager == nullptr)
				{
					m_nChain_Count++;

					if (m_nChain_Count >= 120)
					{
						m_bRestrain = true;

						// 鎖の発射
						m_pChain_Manager = CChain_Manager::Create(GetPos(), Vec, Dis, fRotDest + D3DX_PI / 2, 500);
						// 攻撃パーティクル
						std::move(CParticleEmitter::Create("spark", GetPos()));
					}
				}

				break;
			}
		}
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CCannon::Draw()
{
}

//==============================================================================================
// ヒット処理
//==============================================================================================
void CCannon::Hit(CMove_Object* /*pHit*/)
{
}

//==============================================================================================
// 生成処理
//==============================================================================================
CCannon *CCannon::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, int index, const PRIORITY priority)
{
	CCannon *pCannon = new	CCannon(priority);

	if (pCannon != nullptr)
	{
		pCannon->SetObjectX(CObjectX::Create(pos, rot, parent, Xfilename, priority));
		pCannon->SetPos(pos);
		pCannon->m_nIndex = index;
		pCannon->Init();
	}

	return pCannon;
}