//=============================================================================
//
// enemy.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "enemy.h"
#include "enemy_manager.h"
#include "application.h"
#include "tutorial.h"
#include "game.h"
#include "player.h"
#include "player_manager.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CEnemy::CEnemy()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	// 生成時に自身のポインタを敵キャラマネージャーに設定
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CTutorial::GetEnemyManager()->SetEnemy(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CGame::GetEnemyManager()->SetEnemy(this);
	}
}

//=====================================
// デストラクタ
//=====================================
CEnemy::~CEnemy()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	// マネージャーが使用中の場合、解放時に自身のポインタを配列から除外する
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CEnemyManager* pManager = CTutorial::GetEnemyManager();
		if(pManager != nullptr) pManager->DestroyEnemy(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CEnemyManager* pManager = CGame::GetEnemyManager();
		if (pManager != nullptr) pManager->DestroyEnemy(this);
	}
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CEnemy::Init()
{
	// 敵側に設定
	SetPlayerSide(false);

	// サイズの設定
	SetSize({ GetRadius(),GetRadius(),GetRadius() });

	// 当たり判定の生成
	SetCollision();

	CCharacter::Init();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CEnemy::Uninit()
{
	CCharacter::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CEnemy::Update()
{
	// キャラクターの更新
	CCharacter::Update();

	// 移動処理
	Move();
}

//============================================================================
// 描画処理
//============================================================================
void CEnemy::Draw()
{
	CCharacter::Draw();
}

//============================================================================
// 移動処理
//============================================================================
void CEnemy::Move()
{
	CPlayer* pPlayer = nullptr;

	// プレイヤーの情報
	pPlayer = CGame::GetPlayerManager()->GetPlayer(0);

	// 位置の格納先
	D3DXVECTOR3 Player_Pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 Enemy_Pos = { 0.0f,0.0f,0.0f };

	if (pPlayer != nullptr)
	{
		// 位置の取得
		Player_Pos = pPlayer->GetPos();
		Enemy_Pos = GetPos();
	}

	// プレイヤーから敵の距離
	D3DXVECTOR3 Vec = Player_Pos - Enemy_Pos;

	// 距離の算出
	m_fDistance = sqrtf(Vec.x * Vec.x + Vec.z * Vec.z);

	// 角度の算出
	m_fAngle = atan2(Vec.x, Vec.z);
}

//============================================================================
// 被弾処理
//============================================================================
void CEnemy::Hit(CMove_Object* pHit)
{
	// ヒットした移動オブジェクトが使用中の場合
	if (pHit != nullptr && GetPlayerSide() != pHit->GetPlayerSide())
	{
		// ヒットした移動オブジェクトのタグを取得
		TAG tag = pHit->GetTag();

		// タグから種類を選ぶ
		switch (tag)
		{
		case TAG_CHARACTER:
			break;
		case TAG_BULLET:
			// 弾のダメージを返す
			Damage(pHit->GetPower());

			// ダメージ
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_BULLET_DAMAGE);
			break;
		case TAG_ATTACK:
			// 弾のダメージを返す
			Damage(pHit->GetPower());

			// 無敵状態を付与する
			SetCollisionNoneHit(true);

			// ダメージ
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);
			break;
		case TAG_EXPLOSION:
			// 爆発のダメージを返す
			Damage(pHit->GetPower());

			// 無敵状態を付与する
			SetCollisionNoneHit(true);
			break;
		case TAG_MAP_OBJECT:
			break;
		default:
			break;
		}
	}
}

//============================================================================
// 破壊処理
//============================================================================
void CEnemy::Destroy()
{
	CCharacter::Destroy();
}