//=============================================================================
//
// ボスキャラ　　　boss.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "boss.h"
#include "application.h"
#include "game.h"
#include "particle_emitter.h"
#include "boss_life_gauge.h"
#include "camera.h"
#include "particle_emitter.h"
#include "normal_bullet.h"
#include "player_manager.h"
#include "cannon.h"
#include "weapon_attack.h"
#include "parabola_bullet.h"

const float CBoss::BOSS_COLLISION_RADIUS = 1000.0f;	// ボスの当たり判定の大きさ
//=====================================
// デフォルトコンストラクタ
//=====================================
CBoss::CBoss()
{
	// ボスキャラ
	SetEnemyType(ENEMY_TYPE_BOSS);

	// ボスの初期値を設定
	SetMaxLife(FIRST_MAX_LIFE);
	SetLife(FIRST_MAX_LIFE);
	SetRadius(BOSS_COLLISION_RADIUS);
}

//=====================================
// デストラクタ
//=====================================
CBoss::~CBoss()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CBoss::Init()
{
	// ボスのモデルパーツを設定
	SetParts(PARTS_BODY, CParts_File::PARTS_BOSS, CMotion::MOTION_BOSS);
	GetParts(PARTS_BODY)->GetModel(0)->SetSize({ 5.0f, 5.0f, 5.0f });

	SetGaugeManager(CBoss_Life_Gauge::Create({ 1280 / 2, 100.0f,0.0f }, { 800.0f,15.0f }));

	CEnemy::Init();

	CParts* pBody = GetParts(PARTS_BODY);
	pBody->SetMotion(MOTION_NEUTRAL);

	m_bOpening = true;

	CCamera *pCamera = CApplication::GetCamera();
	pCamera->SetOpening(m_bOpening);

	pCamera->SetPosV({ 0.0f,200.0f,-1000.0f });
	pCamera->SetPosR({ 0.0f, 3000.0f, -500.0f });
	pCamera->SetRot({ 0.0f,0.0f,0.0f });

	// 当たり判定のタイプ
	SetCollision_Type(COLLISION_TYPE_BOSS);

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CBoss::Uninit()
{
	CEnemy::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CBoss::Update()
{
	// キャラクターの更新
	CEnemy::Update();

	ChangeMotion();

	// カメラのポインタ
	bool bOpening = CApplication::GetCamera()->GetOpening();

	if (m_bOpening != bOpening)
	{
		// オープニング終了時に着地する
		D3DXVECTOR3 pos = GetPos();
		SetPos({ pos.x,0.0f,pos.z });

		CParts* pBody = GetParts(PARTS_BODY);
		pBody->SetMotion(MOTION_NEUTRAL);
	}

	m_bOpening = bOpening;
}

//============================================================================
// 描画処理
//============================================================================
void CBoss::Draw()
{
	CEnemy::Draw();
}

//============================================================================
// 破壊処理
//============================================================================
void CBoss::Destroy()
{
	// ボス用撃破パーティクル
	for (int nCnt = 0; nCnt < 30; nCnt++)
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y += 50.0f * nCnt;

		// 撃破時パーティクル
		std::move(CParticleEmitter::Create("Destroy", pos));

		// 爆発SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION);
	}

	// ゲーム終了を設定する
	CGame::SetGameEnd();

	CEnemy::Destroy();

	// BGMを停止する
	CApplication::GetSound()->StopAllBGM();
}

//============================================================================
// モーション変更処理
//============================================================================
void CBoss::ChangeMotion()
{
	// ニュートラルモーションモーションを設定
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		// パーツ
		CParts* pParts = GetParts(nCnt);

		// ATTACK1の次にATTACK2を出す
		if (pParts->GetMotion() == MOTION_ATTACK1 && pParts->GetMotionStop() == true)
		{
			pParts->SetMotion(MOTION_ATTACK2);
			CWeapon_Attack::Create(GetPos(), 1100, false, 600, 20);
		}
		// モーションがループしない場合
		else if (pParts->GetMotionLoop() == false && pParts->GetMotionStop() == true)
		{
			// ニュートラルモーションにする
			pParts->SetMotion(MOTION_NEUTRAL);
		}
	}
}

//============================================================================
// 着地処理
//============================================================================
void CBoss::Landing(const D3DXVECTOR3 pos)
{
	// カメラのポインタ
	bool bOpening = CApplication::GetCamera()->GetOpening();

	if (bOpening)
	{
		// 着地モーションを設定
		for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
		{
			GetParts(nCnt)->SetMotion(MOTION_NEUTRAL);
			// 着地時パーティクル
			std::move(CParticleEmitter::Create("Shock", pos));
			std::move(CParticleEmitter::Create("Dust", pos));
		}
	}

	// キャラクターの着地処理
	CCharacter::Landing(pos);
}

//============================================================================
// 移動処理
//============================================================================
void CBoss::Move()
{
	// 拘束中は動かない
	if (!CCannon::GetRestrain())
	{
		CEnemy::Move();

		// 常に追跡状態
		SetTracking(true);

		// ボスとプレイヤーの距離
		float fDistance = GetDistance();

		if (GetTracking() && !CApplication::GetCamera()->GetOpening())
		{

			// プレイヤーまでの角度
			float fAngle = GetAngle();

			// 敵の移動量
			D3DXVECTOR3 move = { 0.0f,GetMove().y,0.0f };

			// 速度の設定
			SetSpeed(3.0f);

			if (fDistance >= 1000)
			{
				move.x = sinf(fAngle) * GetSpeed();
				move.z = cosf(fAngle) * GetSpeed();
			}
			else if (fDistance < 1000)
			{
				move.x = sinf(fAngle + D3DX_PI) * GetSpeed();
				move.z = cosf(fAngle + D3DX_PI) * GetSpeed();
			}

			// 攻撃処理
			if (fDistance <= 1500)
				Slash_Attack();
			else
			Bullet_Attack();

			// 移動量の設定
			SetMove(move);

			// 角度の設定
			SetRot({ 0.0f,fAngle + D3DX_PI,0.0f });
		}
	}
}

//============================================================================
// 近接攻撃
//============================================================================
void CBoss::Slash_Attack()
{
	// ボスとプレイヤーの距離
	//float fDistance = GetDistance();

	CParts* pBody = GetParts(PARTS_BODY);

	// 近接攻撃の間隔
	m_nAttack_Cooltime++;

	int nRand_Slash = rand() % 2;

	// 近接攻撃
	if (m_nAttack_Cooltime >= 100 && nRand_Slash == 0)
	{
		pBody->SetMotion(MOTION_ATTACK1);
		CWeapon_Attack::Create(GetPos(), 1100, false, 600, 20);
		m_nAttack_Cooltime = 0;
	}
	else if (m_nAttack_Cooltime >= 100 && nRand_Slash == 1)
	{
		pBody->SetMotion(MOTION_ATTACK3);
		CWeapon_Attack::Create(GetPos(), 1100, false, 400, 80);
		m_nAttack_Cooltime = 0;
	}
}

//============================================================================
// 遠距離攻撃
//============================================================================
void CBoss::Bullet_Attack()
{
	// ボスとプレイヤーの距離
	float fDistance = GetDistance();

	// 遠距離攻撃の間隔
	m_nBullet_Cooltime++;

	CPlayer* pPlayer = nullptr;

	// プレイヤーの情報
	pPlayer = CGame::GetPlayerManager()->GetPlayer(0);

	D3DXVECTOR3 pos = GetPos();

	// 弾が発射される位置
	if (m_nBullet_Cooltime >= 60)
	{
		int nRnd = rand() % 3;

		if (nRnd == 0)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				// 遠距離攻撃
				CNormal_Bullet::Create({ pos.x,pos.y + (700.0f / 10.0f * nCnt),pos.z }, { 60.0f,60.0f }, { 0.0f,0.0f,0.0f }, fDistance, pPlayer, 0.0f, true, false,
					240, 120, 60);
			}
		}
		if (nRnd == 1)
		{
			// 遠距離攻撃
			CNormal_Bullet::Create({ pos.x,pos.y + 700.0f,pos.z }, { 1000.0f,1000.0f }, { 0.0f,0.0f,0.0f }, fDistance, pPlayer, 0.0f, true, false,
				1200, 60, 1000);
		}
		if (nRnd == 2)
		{
			// 情報の取得
			D3DXVECTOR3 rot = GetBulletRot();

			D3DXVECTOR3 pos_vec = { -sinf(rot.y), sinf(rot.x), -cosf(rot.y) };

			CParabola_Bullet::Create({ pos.x,pos.y + 700.0f,pos.z }, pos_vec, fDistance, rot, "Data/model/Mob/mob000.x", false, 1200, 60, 1000);
		}

		m_nBullet_Cooltime = 0;
	}
}

//============================================================================
// 生成処理
//============================================================================
CBoss* CBoss::Create(const D3DXVECTOR3 pos)
{
	CBoss* pBoss = new CBoss;

	if (FAILED(pBoss->Init()))
	{
		return nullptr;
	}

	pBoss->SetPos(pos);

	return pBoss;
}