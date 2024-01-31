//=============================================================================
//
// モブ　　　mob.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "mob.h"
#include "application.h"
#include "tutorial.h"
#include "game.h"
#include "particle_emitter.h"
#include "mob_life_gauge.h"
#include "player.h"
#include "player_manager.h"
#include "boss.h"
#include "utility.h"
#include "normal_bullet.h"
#include "sound.h"

const float CMob::MOB_COLLISION_RADIUS = 200.0f;	// モブキャラの当たり判定の大きさ
const float CMob::MOB_BULLET_SPEED = 15.0f;		// 敵キャラの弾の速度
//=====================================
// デフォルトコンストラクタ
//=====================================
CMob::CMob()
{
	// モブキャラ
	SetEnemyType(ENEMY_TYPE_MOB);

	// プレイヤーの初期値を設定
	SetMaxLife(FIRST_MAX_LIFE);
	SetLife(FIRST_MAX_LIFE);
	SetRadius(MOB_COLLISION_RADIUS);
}

//=====================================
// デストラクタ
//=====================================
CMob::~CMob()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CMob::Init()
{
	// モブのモデルパーツを設定
	SetParts(PARTS_BODY, CParts_File::PARTS_MOB, CMotion::MOTION_MOB);

	CEnemy::Init();

	// 体力ゲージ
	SetGaugeManager(CMob_Life_Gauge::Create({0.0f, 0.0f, 0.0f}, { 200.0f,30.0f }));

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CMob::Uninit()
{
	CEnemy::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CMob::Update()
{
		D3DXVECTOR3 Life_Gauge_Pos = GetPos();
		Life_Gauge_Pos.y += 400.0f;

		CGauge_Manager *GaugeManager = GetGaugeManager();

		if (GaugeManager != nullptr)
		{
			// 体力ゲージ
			GaugeManager->SetGaugePos(Life_Gauge_Pos);

			// 体力ゲージの表示
			DrawLifeGauge();
		}

		// 全てのパーツの処理
		for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
		{
			// パーツの情報
			CParts* pParts = GetParts(nCnt);

			// パーツが持つ全てのモデル
			for (auto pModel : pParts->GetModelAll())
			{
				// 距離7000以上で敵を表示
				if (m_fDistance > DRAW_DISTANCE)
				{
					pModel->SetDrawFlag(false);
				}
				else
				{
					pModel->SetDrawFlag(true);
				}
			}
		}

	// キャラクターの更新
	CEnemy::Update();

	// ゲーム終了前まで行動する
	if (CGame::GetGameEnd() == false)
	{
		if (!CApplication::GetCamera()->GetOpening())
		// 回避
		Avoidance();
	}
	// ゲームが終了した場合、透明になって消える
	else
	{
		Clear();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CMob::Draw()
{
	CEnemy::Draw();
}

//============================================================================
// 破壊処理
//============================================================================
void CMob::Destroy()
{
	// 撃破パーティクル
	std::move(CParticleEmitter::Create("burst", GetPos()));

	CEnemy::Destroy();

	// 武器、パーツのドロップ
	CGame::SetDrop_Parts(1, GetPos());

	// 撃墜数をカウント
	CGame::AddDeathCount();
}

//============================================================================
// 体力ゲージの表示
//============================================================================
void CMob::DrawLifeGauge()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	CPlayer* pPlayer = nullptr;

	// モード毎にプレイヤーを読み込む
	if (Mode == CApplication::MODE_TUTORIAL)
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
	else if (Mode == CApplication::MODE_GAME)
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);

	D3DXVECTOR3 Player_Pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 Mob_Pos = { 0.0f,0.0f,0.0f };

	if (pPlayer != nullptr)
	{
		// 位置の取得
		Player_Pos = pPlayer->GetPos();
		Mob_Pos = GetPos();
	}

	// プレイヤーから敵の距離
	D3DXVECTOR3 Vec = Player_Pos - Mob_Pos;

	// 距離の算出
	m_fDistance = sqrtf(Vec.x * Vec.x + Vec.z * Vec.z);

	// 距離3000以上
	if (m_fDistance > DRAW_HP_DISTANCE || GetLife() == FIRST_MAX_LIFE)
	{
		GetGaugeManager()->SetDrawGauge(false);
	}
	else
	{
		GetGaugeManager()->SetDrawGauge(true);
	}
}

//============================================================================
// 移動処理
//============================================================================
void CMob::Move()
{
	CEnemy::Move();

	// モブ敵とプレイヤーの距離
	float fDistance = GetDistance();

	// 距離が3000より近い場合に追跡状態にする
	if (fDistance <= 3000)
		SetTracking(true);
	else
		SetTracking(false);

		// 追跡状態
	if (GetTracking() && !CApplication::GetCamera()->GetOpening())
	{
		// 攻撃
		//Attack();

		// プレイヤーまでの角度
		float fAngle = GetAngle();

		// 敵の移動量
		D3DXVECTOR3 move = { 0.0f,GetMove().y,0.0f };

		// 速度の設定
		SetSpeed(CGame::GetSpawnBoss() ? 2.0f : 0.2f);

		if (fDistance >= 2000 && !m_bAvoidance)
		{
			move.x = sinf(fAngle) * GetSpeed();
			move.z = cosf(fAngle) * GetSpeed();
		}
		else if(fDistance < 2000 && !m_bAvoidance)
		{
			move.x = sinf(fAngle + D3DX_PI) * GetSpeed();
			move.z = cosf(fAngle + D3DX_PI) * GetSpeed();
		}
		
		// 移動量の設定
		SetMove(move);

		// 角度の設定
		SetRot({ 0.0f,fAngle + D3DX_PI,0.0f });		

		// 回避した時からカウント
		if (m_bAvoidance)
		{
			if (m_fAvoidance_Count == 1000)
			{
				m_bAvoidance = false;
			}
			else
			{
				// 回避のカウント
				m_fAvoidance_Count++;
			}
		}
		else
		{
			// 一定時間ごとに確率で回避する
			int nRand = rand() % 20;

			if (nRand == 0)
			{
				// 回避
				m_bAvoidance = true;
				m_fAvoidance_Speed = 20.0f;
				m_fAvoidance_Count = 0;
			}
		}
	}
}

//============================================================================
// 攻撃処理
//============================================================================
void CMob::Attack()
{
	// モブ敵とプレイヤーの距離
	float fDistance = GetDistance();

	CPlayer* pPlayer = nullptr;

	// プレイヤーの情報
	pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
	
	// 弾を撃つ間隔
	m_nBullet_Interval++;

	if (m_nBullet_Interval >= 120)
	{
		// 10/1の確立で攻撃
		if (rand() % 30 == 0)
		{
			// 弾
			CNormal_Bullet::Create(GetPos(), { 60.0f,60.0f }, { 0.0f,0.0f,0.0f }, fDistance, pPlayer, 0.0f, true, false,
				MOB_BULLET_POWER, MOB_BULLET_SPEED, MOB_BULLET_LIFE);
			m_nBullet_Interval = 0;
		}
	}
}

//============================================================================
// 回避処理
//============================================================================
void CMob::Avoidance()
{
	if (m_bAvoidance)
	{
		// プレイヤーまでの角度
		float fAngle = GetAngle();

		// 敵の移動量
		D3DXVECTOR3 move = { 0.0f,GetMove().y,0.0f };

		// 右か左に回避
		int nRand = rand() % 2;

		if (nRand == 0 && !m_bfAvo_Step)
		{
			m_fStep = fAngle + D3DX_PI / 2;
			m_bfAvo_Step = true;
		}
		else if(nRand == 1 && !m_bfAvo_Step)
		{
			m_fStep = fAngle - D3DX_PI / 2;
			m_bfAvo_Step = true;
		}

		// 移動量の設定
		move.x = sinf(m_fStep) * m_fAvoidance_Speed;
		move.z = cosf(m_fStep) * m_fAvoidance_Speed;

		// 移動量の減衰
		m_fAvoidance_Speed -= 0.1f;

		if (m_fAvoidance_Speed <= 0)
		{
			m_fAvoidance_Speed = 0.0f;
			m_bfAvo_Step = true;

			// 攻撃
			Attack();
		}

		// 移動量の設定
		SetMove(move);
	}
}

//============================================================================
// 透明化処理
//============================================================================
void CMob::Clear()
{
	// 300f経つまでの処理
	if (m_nClear_Counter < 300)
	{
		float fAlpha = 1.0f - (m_nClear_Counter++ / 300);

		// 全てのパーツの処理
		for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
		{
			// パーツの情報
			CParts* pParts = GetParts(nCnt);

			// パーツが持つ全てのモデル
			for (auto pModel : pParts->GetModelAll())
			{
				pModel->SetAlpha(fAlpha);
			}
		}
	}
	// 300fしたら消える処理
	else
	{
		Uninit();
	}
}

//============================================================================
// 生成処理
//============================================================================
CMob* CMob::Create(const D3DXVECTOR3 pos)
{
	CMob* pMob = new CMob;

	if (FAILED(pMob->Init()))
	{
		return nullptr;
	}
	pMob->SetPos(pos);

	return pMob;
}