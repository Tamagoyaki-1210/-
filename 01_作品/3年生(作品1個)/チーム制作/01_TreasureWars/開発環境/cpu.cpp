//==============================================================================================
//
// CPU　　　cpu.cpp
// tutida ryousei
//
//==============================================================================================
#include"cpu.h"
#include"application.h"
#include"game.h"
#include"charManager.h"
#include"shop.h"
#include"ore.h"
#include"oreManager.h"
#include"scoreitemmanager.h"
#include"scoreitem_stack.h"
#include"itemmanager.h"
#include"enemymanager.h"
#include"debugProc.h"
#include "particle_emitter.h"

const float CCpu::Radius_Search_Ore = 1200.0f;		// 鉱石の検索のサイズ(半径)
const float CCpu::Radius_Search_ScoreItem = 300.0f;	// スコアアイテムの検索のサイズ(半径)
const float CCpu::Radius_Search_Chara = 150.0f;		// キャラの検索のサイズ(半径)
const float CCpu::Radius_Search_Enemy = 100.0f;		// 敵キャラの検索のサイズ(半径)
//==============================================================================================
// コンストラクタ
//==============================================================================================
CCpu::CCpu()
{
	// 攻撃力の初期値を設定
	SetMinePower(First_Mine_Power);

	// 色々考える思考
	m_trialRoutine = TRIAL_ROUTINE_NEUTRAL;

	m_targetPos = { 0.0f, 0.0f, 0.0f };

	m_bSetAttack = false;
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CCpu::~CCpu()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CCpu::Init()
{
	CCharacter::Init();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CCpu::Uninit()
{
	CCharacter::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CCpu::Update()
{
	// モーション番号の設定
	ChangeMotion();

	// モーション
	Motion();

	// ゲームが開始されている場合
	if (CApplication::GetGameStart())
	{
		// ノックバック中か
		bool bKnockBack = CCharacter::GetKnockBack();

		if (!bKnockBack)
		{
			// 移動処理
			Move();

			// AI処理
			Ai();
		}
		else
		{
			// スコアアイテムを所持している場合
			if (GetNum_StackScoreItem() != 0)
			{
				// スコアアイテムを売却する思考
				m_trialRoutine = TRIAL_ROUTINE_SALE;
			}
		}

		CCharacter::Update();
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CCpu::Draw()
{
	CCharacter::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CCpu *CCpu::Create(D3DXVECTOR3 pos, int index)
{
	CCpu *pCpu = nullptr;

	pCpu = new CCpu;

	if (pCpu != nullptr)
	{
		pCpu->SetPos(pos);
		pCpu->SetIndex(index);
		pCpu->Init();
	}

	return pCpu;
}

//==============================================================================================
// 移動処理
//==============================================================================================
void CCpu::Move()
{
	// 攻撃中ではない場合
	if (!GetAttack())
	{
		// 動きがあった場合
		if (m_targetPos != D3DXVECTOR3(0.0f, 0.0, 0.0f))
		{
			// 歩き
			SetMotion(MOTION_WALK);
			SetMove(true);

			// 目的位置と自身の位置の差分
			D3DXVECTOR3 distance = { m_targetPos.x - GetPos().x, 0.0f, m_targetPos.z - GetPos().z };

			// 向かう方向へ向く
			float fAngle = (float)atan2(-distance.x, -distance.z);

			// キャラクターの方を向く
			CCharacter::SetRotDest({ 0.0f, fAngle, 0.0f });

			// 移動量
			D3DXVECTOR3 move = { 0.0f, 0.0f, 0.0f };

			// 移動量の初期化
			D3DXVec3Normalize(&move, &distance);

			// 移動量を更新
			CCharacter::SetMove(move);

			// 移動処理
			CCharacter::Move();
		}
		// 動きが無い場合
		else
		{
			// 歩きを終了させる
			SetMotion(MOTION_NEUTRAL);
		}
		// 目的位置の初期化
		m_targetPos = { 0.0f, 0.0f, 0.0f };
	}
}

//==============================================================================================
// アクション
//==============================================================================================
void CCpu::Action()
{
	// 攻撃中ではない場合
	if (GetMotion_Type() != MOTION_MINE)
	{
		// 攻撃モーション
		SetMotion(MOTION_MINE);

		// 攻撃処理
		SetAttack(true);
	}

	// タイムスリップ
	if (d % 300 == 0 && !GetInvocation())
	{
		/*SetTimeSlip(true);
		SetInvocation(true);*/
	}
}

//==============================================================================================
// AI処理
//==============================================================================================
void CCpu::Ai()
{
	// 検索値
	int nSearch = 0;

	// 思考ルーチンの種類でAIを判断
	switch (m_trialRoutine)
	{
	// 色々考える思考
	case TRIAL_ROUTINE_NEUTRAL:

		// 質屋以外の全てを検索する
		nSearch = SEARCH_CHARA | SEARCH_ORE | SEARCH_SCOREITEM | SEARCH_ITEM | SEARCH_ENEMY;
		break;

	// 各プレイヤーが近くにいる場合、真っすぐに自分から攻撃する思考
	case TRIAL_ROUTINE_ATTACK:

		// キャラとスコアアイテムと敵キャラを検索する
		nSearch = SEARCH_CHARA | SEARCH_SCOREITEM | SEARCH_ITEM | SEARCH_ENEMY;
		break;

	// 所持している宝石を売り切る思考
	case TRIAL_ROUTINE_SALE:

		// 質屋のみを検索する
		nSearch = SEARCH_SHOP;
		break;
	default:
		break;
	}

	// 検索をする場合
	if (nSearch != 0)
	{
		// 検索処理
		Search(nSearch);

		// 攻撃をする場合
		if (m_bSetAttack == true)
		{
			// 攻撃をする
			Action();

			// 初期化する
			m_bSetAttack = false;
		}
	}
}

//==============================================================================================
// 探索処理(優先度 : 降順に上書き)
//==============================================================================================
void CCpu::Search(const int search)
{
	const D3DXVECTOR3 pos = GetPos();
	const float radius = GetRadius();

	// キャラの検索処理
	if (((search & SEARCH_CHARA) == SEARCH_CHARA))
	{
		CharaSearch(pos, radius);
	}
	// 攻撃中ではない場合
	if (!m_bSetAttack)
	{
		// 鉱石の検索処理
		if (((search & SEARCH_ORE) == SEARCH_ORE))
		{
			OreSearch(pos, 0.0f);
		}
		// スコアアイテムの検索処理
		if (((search & SEARCH_SCOREITEM) == SEARCH_SCOREITEM))
		{
			ScoreItemSearch(pos);
		}
		// アイテムの検索処理
		if (((search & SEARCH_ITEM) == SEARCH_ITEM))
		{
			ItemSearch(pos);
		}
		// 敵キャラの検索処理
		if (((search & SEARCH_ENEMY) == SEARCH_ENEMY))
		{
			EnemySearch(pos, radius);
		}
		// 質屋の検索処理
		if (((search & SEARCH_SHOP) == SEARCH_SHOP))
		{
			ShopSearch(pos, radius);
		}
	}
}

//==============================================================================================
// キャラの探索処理
//==============================================================================================
void CCpu::CharaSearch(const D3DXVECTOR3 pos, const float radius)
{
	CCharacter* pNearChara = nullptr;	// 距離が一番近いキャラのポインタ
	float fNearDistance = 0.0f;

	// 全てのキャラを読み込む
	for (int nIndex = 0; nIndex < CCharManager::Max_Character; nIndex++)
	{
		// 自身の番号ではない場合
		if (nIndex != GetIndex())
		{
			CCharacter* pChara = CApplication::GetCharManager()->CCharManager::GetChara(nIndex);

			// 相手のキャラが宝石を所持している場合 && 相手が安全地帯にいない場合
			if (pChara->GetNum_StackScoreItem() > 0 && !pChara->GetSafe())
			{
				const D3DXVECTOR3 atherPos = pChara->GetPos();

				// 検索範囲に相手のキャラがいる場合
				if (CollisionCircle(pos, Radius_Search_Chara, atherPos, pChara->GetRadius()))
				{
					// 位置同士の距離の計算用変数
					D3DXVECTOR2 distance = { atherPos.x - pos.x, atherPos.z - pos.z };
					float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

					// 距離が小さい場合、又は最初の処理の場合確保する
					if (fNearDistance > fDistance || fNearDistance == 0.0f)
					{
						fNearDistance = fDistance;
						pNearChara = pChara;
					}
				}
			}
		}
	}
	// キャラの検索に成功した場合
	if (pNearChara != nullptr)
	{
		// キャラの攻撃情報
		const float fAttack_Length = GetAttackLength();
		const float fAttack_Size = GetAttackSize();

		// 攻撃(円形)設置位置
		const D3DXVECTOR3 attackPos = pos - D3DXVECTOR3(sinf(GetRot().y) * fAttack_Length, 0.0f, cosf(GetRot().y) * fAttack_Length);

		// 相手の位置
		const D3DXVECTOR3 atherPos = pNearChara->GetPos();

		// 相手を攻撃する思考
		m_trialRoutine = TRIAL_ROUTINE_ATTACK;

		// 自身の攻撃当たり判定内に相手のキャラがいる場合
		if(CollisionCircle(attackPos, fAttack_Size, atherPos, radius)
			|| AttackCollision(pos, GetRot().y, fAttack_Length, CCharacter::Attack_Angle, atherPos, radius))
		{
			// 攻撃をする
			m_bSetAttack = true;
		}
		// 範囲にいない場合
		else
		{
			// 目的位置にその位置を設定
			m_targetPos = atherPos;
		}
	}
	else
	{
		// 色々考える思考
		m_trialRoutine = TRIAL_ROUTINE_NEUTRAL;
	}
}

//==============================================================================================
// 鉱石の探索処理
//==============================================================================================
void CCpu::OreSearch(const D3DXVECTOR3 pos, const float radius)
{
	// 宝石の所持数に空きがある場合
	if (GetNum_StackScoreItem() < GetNumGemStack())
	{
		COre* pOre = CGame::GetOreManager()->OreCollision(pos, Radius_Search_Ore);

		// 鉱石を発見している場合
		if (pOre != nullptr)
		{
			// キャラの攻撃情報
			const float fAttack_Length = GetAttackLength();
			const float fAttack_Size = GetAttackSize();

			// 攻撃(円形)設置位置
			const D3DXVECTOR3 attackPos = pos - D3DXVECTOR3(sinf(GetRot().y) * fAttack_Length, 0.0f, cosf(GetRot().y) * fAttack_Length);

			// マップ内のオブジェクトの情報
			const D3DXVECTOR3 atherPos = pOre->GetPos();

			// 衝突していない場合
			if (!CollisionCircle(attackPos, fAttack_Size, atherPos, radius)
				&& !AttackCollision(pos, GetRot().y, fAttack_Length, CCharacter::Attack_Angle, atherPos, radius))
			{
				// ベクトルの算出
				D3DXVECTOR3 vec = pos - atherPos;

				// ベクトルの方向を計算
				D3DXVECTOR3 vecNolmalize;
				D3DXVec3Normalize(&vecNolmalize, &vec);

				// 半径の合計の大きさ
				const float fTotalRadius = COreManager::Ore_Radius + radius;

				// 半径同士の大きさ分のベクトル
				D3DXVECTOR3 vec2 = fTotalRadius * vecNolmalize;

				// 大きさ分で減算したベクトル
				const D3DXVECTOR3 add = vec2 - vec;

				// 目的位置にその位置を設定
				m_targetPos = atherPos;
			}
			// 鉱石が攻撃範囲に入った場合
			else
			{
				// 攻撃をする
				m_bSetAttack = true;
			}
		}
		// 鉱石が見つからない場合
		else
		{
			// スコアアイテムを売却する思考
			m_trialRoutine = TRIAL_ROUTINE_SALE;
		}
	}
	// スコアアイテムの所持数が満タンの場合
	else
	{
		// スコアアイテムを売却する思考
		m_trialRoutine = TRIAL_ROUTINE_SALE;
	}
}

//==============================================================================================
// スコアアイテムの探索処理
//==============================================================================================
void CCpu::ScoreItemSearch(const D3DXVECTOR3 pos)
{
	CScoreItem* pNearScoreItem = nullptr;	// 距離が一番近いスコアアイテムのポインタ
	float fNearDistance = 0.0f;

	// 全てのスコアアイテムのポインタ
	for (auto pScoreItem : CGame::GetScoreItemManager()->GetAllScoreItem())
	{
		const D3DXVECTOR3 atherPos = pScoreItem->GetPos();

		// スコアアイテムの検索範囲にいる場合
		if (CollisionCircle(pos, Radius_Search_ScoreItem, atherPos, CScoreItem::ScoreItem_Radius))
		{
			// スコアアイテムが地面に接地している場合
			if (pScoreItem->GetGround())
			{
				// 位置同士の距離の計算用変数
				D3DXVECTOR2 distance = { atherPos.x - pos.x, atherPos.z - pos.z };
				float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

				// 距離が小さい場合、又は最初の処理の場合確保する
				if (fNearDistance > fDistance || fNearDistance == 0.0f)
				{
					fNearDistance = fDistance;
					pNearScoreItem = pScoreItem;
				}
			}
		}
	}
	// スコアアイテムの検索に成功した場合
	if (pNearScoreItem != nullptr)
	{
		// 攻撃をしない
		m_bSetAttack = false;

		// 目的位置にその位置を設定
		m_targetPos = pNearScoreItem->GetPos();
	}
}

//==============================================================================================
// アイテムの探索処理
//==============================================================================================
void CCpu::ItemSearch(const D3DXVECTOR3 pos)
{
	CItem* pNearItem = nullptr;	// 距離が一番近いスコアアイテムのポインタ
	float fNearDistance = 0.0f;

	// 全てのアイテムのポインタ
	for (auto pItem : CGame::GetItemManager()->GetAllItem())
	{
		const D3DXVECTOR3 atherPos = pItem->GetPos();

		// スコアアイテムの検索範囲にいる場合
		if (CollisionCircle(pos, Radius_Search_ScoreItem, atherPos, pItem->GetRadius()))
		{
			// 位置同士の距離の計算用変数
			D3DXVECTOR2 distance = { atherPos.x - pos.x, atherPos.z - pos.z };
			float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

			// 距離が小さい場合、又は最初の処理の場合確保する
			if (fNearDistance > fDistance || fNearDistance == 0.0f)
			{
				fNearDistance = fDistance;
				pNearItem = pItem;
			}
		}
	}
	// スコアアイテムの検索に成功した場合
	if (pNearItem != nullptr)
	{
		// 攻撃をしない
		m_bSetAttack = false;

		// 目的位置にその位置を設定
		m_targetPos = pNearItem->GetPos();
	}
}

//==============================================================================================
// 敵キャラの探索処理
//==============================================================================================
void CCpu::EnemySearch(const D3DXVECTOR3 pos, const float radius)
{
	for (auto pChara : CGame::GetEnemyManager()->GetAllEnemy())
	{
		const D3DXVECTOR3 atherPos = pChara->GetPos();
		const float atherRadius = pChara->GetRadius();

		// 宝石の検索範囲にいる場合
		if (CollisionCircle(pos, Radius_Search_Enemy, atherPos, atherRadius))
		{
			// 宝石を売却する思考
			m_trialRoutine = TRIAL_ROUTINE_SALE;

			break;
		}
	}
}

//==============================================================================================
// 質屋の探索処理
//==============================================================================================
void CCpu::ShopSearch(const D3DXVECTOR3 pos, const float radius)
{
	CShop* pShop = CGame::GetShop(GetIndex());

	// 質屋が存在している場合
	if (pShop != nullptr)
	{
		const D3DXVECTOR3 atherPos = pShop->GetPos();

		// 質屋の外にいる場合
		if (!CollisionCircle(pos, radius, atherPos, 30.0f))
		{
			// 目的位置にその位置を設定
			m_targetPos = atherPos;
		}
		// 質屋の中にいる場合
		else
		{
			// 売却可能の場合
			if (GetNum_StackScoreItem() != 0)
			{
				// 宝石を売却する思考
				m_trialRoutine = TRIAL_ROUTINE_SALE;
			}
			// 売却する物が無い場合
			else
			{
				// 色々考える思考
				m_trialRoutine = TRIAL_ROUTINE_NEUTRAL;
			}
		}
	}
}