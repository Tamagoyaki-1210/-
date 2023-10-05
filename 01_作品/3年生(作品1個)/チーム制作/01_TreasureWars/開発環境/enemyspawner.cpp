//=============================================================================
//
// enemyspawner.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "enemyspawner.h"
#include "application.h"
#include "enemymanager.h"
#include "particle_emitter.h"
#include "ghost.h"
#include "pteranodon.h"
#include "alien.h"
#include "game.h"
#include "item.h"
#include "scoreitem.h"

const float CEnemySpawner::First_Pos = -300.0f;	// スポナーの初期位置
//=====================================
// デフォルトコンストラクタ
//=====================================
CEnemySpawner::CEnemySpawner() : CObjectX(2)
{
}

//=====================================
// デストラクタ
//=====================================
CEnemySpawner::~CEnemySpawner()
{
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CEnemySpawner::Init()
{
	// オブジェクトXの初期化処理
	CObjectX::Init();

	m_bAppearance = false;
	m_nSpawn_Counter = 0;

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CEnemySpawner::Uninit()
{
	// オブジェクトXの終了処理
	CObjectX::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CEnemySpawner::Update()
{
	// オブジェクトXの更新処理
	CObjectX::Update();

	// 出現しきっていない場合
	if (m_bAppearance == false)
	{
		// 出現までの処理
		Appearance();
	}
	// 出現しきった場合
	else
	{
		// 生成までの時間を計る処理
		SpawnCount();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CEnemySpawner::Draw()
{
	// オブジェクトXの描画処理
	CObjectX::Draw();
}

//============================================================================
// 出現までの処理
//============================================================================
void CEnemySpawner::Appearance()
{
	D3DXVECTOR3 pos = GetPos();
 	pos.y += 4.0f;

	// 上に上げた時に原点よりも上の場合
	if (pos.y >= 0.0f)
	{
		SetAlpha(1.0f);
  		pos.y = 0.0f;
		m_bAppearance = true;

		// 鉱石出現時パーティクル
		CParticleEmitter::Create("PopOre", pos);

		// 最初の生成時間を指定
		m_nSpawn_Counter = 0;
	}

	SetPos(pos);	// 位置の設定を行う
}

//============================================================================
// 生成までの時間を計る処理
//============================================================================
void CEnemySpawner::SpawnCount()
{
	// 現在の敵キャラがいない場合
	if (CGame::GetEnemyManager()->GetAllEnemy().empty())
	{
		if (m_nSpawn_Counter++ >= Enemy_Spawn_Timer)
		{
			// 決められた回数だけ生成する
			for (int nCnt = 0; nCnt < m_nMaxSpawn; nCnt++)
			{
				// 敵キャラを出現させる処理
				EnemySpawn();
			}
			m_nSpawn_Counter = 0;
		}
	}
}

//============================================================================
// 敵キャラを出現させる処理
//============================================================================
void CEnemySpawner::EnemySpawn()
{
	// 幽霊出現時パーティクル
	CParticleEmitter::Create("PopGhost", GetPos());

	switch (m_type)
	{
	case CEnemySpawner::EnemySpawn_1:
		// 幽霊の生成
		CGhost::Create(GetPos());
		break;
	case CEnemySpawner::EnemySpawn_2:
		// プテラノドンの生成
		CPteranodon::Create(GetPos());
		break;
	case CEnemySpawner::EnemySpawn_3:
		// エイリアンの生成
		CAlien::Create(GetPos());
		break;
	default:
		break;
	}
}

//============================================================================
// 被弾処理
//============================================================================
void CEnemySpawner::Damage()
{
	const D3DXVECTOR3 pos = GetPos();

	// 掘るパーティクル
	std::move(CParticleEmitter::Create("Mine", pos));

	// 採掘SE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_MINE);

	switch (m_type)
	{
	case CEnemySpawner::EnemySpawn_1:
		// 赤みを追加
		AddColor(D3DXCOLOR(0.1f, 0.0f, 0.0f, 0.0f));
		break;
	case CEnemySpawner::EnemySpawn_2:
		// 赤みを追加
		AddColor(D3DXCOLOR(0.1f, 0.0f, 0.0f, 0.0f));
		break;
	case CEnemySpawner::EnemySpawn_3:
		// 赤みを追加
		AddColor(D3DXCOLOR(0.02f, 0.0f, 0.0f, 0.0f));
		break;
	default:
		break;
	}

	// 体力を減らして0になったら消す
	if (--m_nLife <= 0)
	{
		// 鉱石消失時パーティクル
		CParticleEmitter::Create("Disappear", pos);

		for (int nCnt = 0; nCnt < 5 + (int)m_type * 2; nCnt++)
		{
			// ランダムにスコアアイテムをばら撒く処理
			CScoreItem::RandomCreate(pos);
		}

		// 確率でアイテムの生成
		CItem::Create(D3DXVECTOR3(pos.x, pos.y + 10.0f, pos.z), { 2.0f,2.0f,2.0f });

		// 全ての敵キャラの解放処理
		CGame::GetEnemyManager()->ReleaseAllEnemy();

		// 敵キャラスポナーを消す
		CGame::EnemySpawnerDeath();
	}
}

//============================================================================
// 生成処理
//============================================================================
CEnemySpawner* CEnemySpawner::Create(const D3DXVECTOR3 pos, const int type)
{
	CEnemySpawner* pEnemySpawner = new CEnemySpawner;

	if (FAILED(pEnemySpawner->Init()))
	{
		return nullptr;
	}
	pEnemySpawner->SetPos(D3DXVECTOR3(pos.x, First_Pos, pos.z));
	pEnemySpawner->m_type = (EnemySpawn_Type)type;

	// 種類毎の初期値の違い
	switch (type)
	{
	case CEnemySpawner::EnemySpawn_1:
		pEnemySpawner->SetIndex(ReadObject("Data/model/Enemy_Spawn/tombstone.x"));
		pEnemySpawner->m_nMaxSpawn = 4;
		pEnemySpawner->m_nLife = 3;
		pEnemySpawner->m_fRadius = 50.0f;
		break;
	case CEnemySpawner::EnemySpawn_2:
		pEnemySpawner->SetIndex(ReadObject("Data/model/Enemy_Spawn/ptera_spawn.x"));
		pEnemySpawner->m_nMaxSpawn = 2;
		pEnemySpawner->m_nLife = 4;
		pEnemySpawner->m_fRadius = 50.0f;
		break;
	case CEnemySpawner::EnemySpawn_3:
		pEnemySpawner->SetIndex(ReadObject("Data/model/Enemy_Spawn/ufo.x"));
		pEnemySpawner->m_nMaxSpawn = 5;
		pEnemySpawner->m_nLife = 5;
		pEnemySpawner->m_fRadius = 151.0f;
		break;
	default:
		break;
	}
	pEnemySpawner->SetAlpha(0.5f);

	return pEnemySpawner;
}