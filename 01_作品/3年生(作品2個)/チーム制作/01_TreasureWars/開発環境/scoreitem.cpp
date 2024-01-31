//=============================================================================
//
// scoreitem.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include <time.h>
#include "scoreitem.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "time.h"
#include "charmanager.h"
#include "scoreitem_stack.h"
#include "shadow.h"
#include "shop.h"
#include "stage_select.h"
#include "scoreitemmanager.h"

const char* CScoreItem::STAGE_SCOREITEM_01[] =
{
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Diamond.x",
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Ruby.x",
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Sapphire.x",
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Emerald.x",
	"Data\\model\\Drop_Score_Item\\Stage_01\\Gem_Topaz.x",
};

const char* CScoreItem::STAGE_SCOREITEM_02[] =
{
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Head.x",
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Amber.x",
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Bone.x",
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Footprints.x",
	"Data\\model\\Drop_Score_Item\\Stage_02\\Fossil_Ammonite.x",
};

const char* CScoreItem::STAGE_SCOREITEM_03[] =
{
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_CPUChip.x",
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_SolarPanel.x",
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_ParabolaAntenna.x",
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_RocketEngine.x",
	"Data\\model\\Drop_Score_Item\\Stage_03\\Debris_OxygenBomb.x",
};

const float CScoreItem::ScoreItem_Radius = 45.0f;		// スコアアイテムのサイズ
const float CScoreItem::ScoreItem_Gravity = 20.0f;		// スコアアイテムの重力
const float CScoreItem::ScoreItem_Inertia = 0.90f;		// スコアアイテムの慣性
const float CScoreItem::ScoreItem_Launch = 60.0f;		// スコアアイテムが縦に打ち上がる力
const float CScoreItem::ScoreItem_Blink_Speed = 0.1f;	// スコアアイテムの点滅速度
											//=====================================
											// デフォルトコンストラクタ
											//=====================================
CScoreItem::CScoreItem() : m_bGround(false)
{
	// アイテム生成時に自身のポインタをアイテムマネージャーに設定
	CGame::GetScoreItemManager()->SetScoreItem(this);

	m_nDestroyCounter = 0;
	m_nFloatingCounter = 0;
}

//=====================================
// デストラクタ
//=====================================
CScoreItem::~CScoreItem()
{
	CScoreItemManager* pManager = CGame::GetScoreItemManager();

	// マネージャーが使用中の場合、解放時に自身のポインタを配列から除外する
	if (pManager != nullptr) pManager->DestroyScoreItem(this);
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CScoreItem::Init()
{
	// オブジェクトXの初期化処理
	CObjectX::Init();

	Bounce();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CScoreItem::Uninit()
{
	// オブジェクトXの終了処理
	CObjectX::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CScoreItem::Update()
{
	// 影の位置を更新する
	m_pShadow->SetShadow(GetPos());

	// 宝石を回転させる
	AddRot({ 0.0f, 0.1f, 0.0f });

	// オブジェクトXの更新処理
	CObjectX::Update();

	// 着地中であれば当たり判定を作る
	if (!m_bGround)
	{
		Move();
	}
	else
	{
		Floating();

		Blink();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CScoreItem::Draw()
{
	// オブジェクトXの描画処理
	CObjectX::Draw();
}

//============================================================================
// 消去の処理
//============================================================================
void CScoreItem::Destroy()
{
	// 影の終了処理
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}
	// 自身の終了処理
	Uninit();
}

//============================================================================
// 移動量の処理
//============================================================================
void CScoreItem::Move()
{
	D3DXVECTOR3 pos = GetPos();
	// 重力の追加
	pos.y -= ScoreItem_Gravity;

	// 移動量に慣性を入れる
	m_move = m_move * ScoreItem_Inertia;

	// 移動量の追加
	pos += m_move;

	// 地面以下の位置に落ちた場合
	if (pos.y <= 0.0f)
	{
		// 着地させ位置を固定する
		m_bGround = true;
		pos.y = 0.0f;
	}

	// 位置を更新する
	SetPos(pos);

	// 壁との当たり判定
	WallColl();
}

//============================================================================
// キャラクターとの当たり判定
//============================================================================
void CScoreItem::CharColl()
{
	// 全てのキャラクターを読み込む
	for (int index = 0; index < CCharManager::Max_Character; index++)
	{
		// キャラクターのポインタ
		CCharacter* pChara = CApplication::GetCharManager()->GetChara(index);

		// キャラクターが使用中の場合
		if (pChara != nullptr)
		{
			// 当たり判定の範囲内にいる場合
			if (CollisionCircle(GetPos(), ScoreItem_Radius, pChara->GetPos(), pChara->GetRadius()))
			{
				CScoreItemStack* pScoreItemStack = pChara->GetGemStack();

				// 現在のスタックが最大スタック未満の場合
				if (pChara->GetNum_StackScoreItem() < pChara->GetNumGemStack())
				{
					// スコアアイテムを持つ
					pChara->SetStackScoreItem();

					// 宝石_獲得SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_GEM_GET);

					// アイテムスコアを獲得させて自身を終了させる
					pScoreItemStack->SetScoreItem(m_nType);

					Destroy();

					break;
				}
			}
		}
	}
}

//============================================================================
// 壁との当たり判定
//============================================================================
void CScoreItem::WallColl()
{
	const D3DXVECTOR3 pos = GetPos();

	// 左右の壁へ飛んだ場合
	if (pos.x < ScoreItem_Radius * 2.0f || pos.x > 1500.0f - ScoreItem_Radius * 2.0f)
	{
		// 横軸の移動量を反転させる
		m_move.x = -m_move.x;
	}

	// 上下の壁へ飛んだ場合
	if (pos.z < ScoreItem_Radius * 2.0f || pos.z > 1500.0f - ScoreItem_Radius * 2.0f)
	{
		// 縦軸の移動量を反転させる
		m_move.z = -m_move.z;
	}
}

//============================================================================
// 宝石を点滅させる処理
//============================================================================
void CScoreItem::Blink()
{
	// 点滅させる時間に到達した場合
	if (m_nDestroyCounter > Blink_Timer)
	{
		// 消えるまでの時間に達した場合
		if (m_nDestroyCounter >= Max_Destroy_Timer)
		{
			// 消去処理
			Destroy();
			return;
		}
		// アルファ値を0～9でデクリメント、10～19でインクリメント
		AddAlpha(m_nDestroyCounter % (ScoreItem_Blink_interval * 2) < ScoreItem_Blink_interval ? -ScoreItem_Blink_Speed : ScoreItem_Blink_Speed);

	}
	// 点滅が始まったタイミング
	else if (m_nDestroyCounter == Blink_Timer)
	{
		// 影の描画を消す
		m_pShadow->SetDrawFlag(false);
	}

	m_nDestroyCounter++;

	// 当たり判定を読み込む
	CharColl();
}

//============================================================================
// 跳ねる処理
//============================================================================
void CScoreItem::Bounce()
{
	// ランダムな値の変数
	const float fRand = ((rand() % 629) - 314) / 100.0f;

	// 跳ねる力の値の変数
	const float fBounce = (rand() % 100) / 10.0f + 10;

	// 移動量を設定
	m_move = { sinf(fRand) * fBounce, ScoreItem_Launch, cosf(fRand) * fBounce };
}

//============================================================================
// 浮遊する処理
//============================================================================
void CScoreItem::Floating()
{
	// 浮遊させる
	AddPos({ 0.0f, m_nFloatingCounter++ % (Floating_Timer * 2) < Floating_Timer ? 1.0f : -1.0f, 0.0f });

	// 浮遊ループが成功したら初期化
	if (m_nFloatingCounter >= Floating_Timer * 2)
	{
		// 浮遊時間の初期化
		m_nFloatingCounter = 0;
	}
}

//============================================================================
// 影の生成処理
//============================================================================
void CScoreItem::SetShadow(const D3DXVECTOR3 pos)
{
	m_pShadow = CShadow::Create({ pos.x, 5.0f, pos.z }, { ScoreItem_Radius, ScoreItem_Radius, 0.0f });
}

//============================================================================
// 生成処理
//============================================================================
CScoreItem* CScoreItem::RandomCreate(const D3DXVECTOR3 pos)
{
	CScoreItem* pScoreItem = new CScoreItem;

	if (FAILED(pScoreItem->Init()))
	{
		return nullptr;
	}

	// ランダムな値 % 最大の値
	int nRand = rand() % (int)(0.5f * (CScoreItem::MAX_SCOREITEM_MODEL * (CScoreItem::MAX_SCOREITEM_MODEL + 1)));

	// スコアアイテムの種類
	int nType = 0;

	// 登録された降順に連れ確率Up
	for (int nCnt = 0, i = 1; nCnt < CScoreItem::MAX_SCOREITEM_MODEL; nCnt++, i += (nCnt + 1), nType++)
	{
		// 確率に当選している場合
		if (nRand < i)
		{
			break;
		}
	}

	// スコアアイテムの種類を設定
	pScoreItem->m_nType = nType;

	// ステージ番号でモデルを決める
	switch (CStage_Select::GetStage_Number())
	{
	case CStage_Select::STAGE_1:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_01[nType]));
		break;
	case CStage_Select::STAGE_2:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_02[nType]));
		break;
	case CStage_Select::STAGE_3:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_03[nType]));
		break;
	default:
		break;
	}

	pScoreItem->SetPos(pos);
	pScoreItem->SetRot({ 0.0f, D3DX_PI / 2, 0.0f });
	pScoreItem->SetSize({ 1.0f,1.0f,1.0f });
	pScoreItem->SetShadow(pos);

	return pScoreItem;
}

//============================================================================
// 生成処理
//============================================================================
CScoreItem* CScoreItem::Create(const D3DXVECTOR3 pos, const int type)
{
	CScoreItem* pScoreItem = new CScoreItem;

	if (FAILED(pScoreItem->Init()))
	{
		return nullptr;
	}

	pScoreItem->m_nType = type;

	// ステージ番号でモデルを決める
	switch (CStage_Select::GetStage_Number())
	{
	case CStage_Select::STAGE_1:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_01[type]));
		break;
	case CStage_Select::STAGE_2:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_02[type]));
		break;
	case CStage_Select::STAGE_3:
		pScoreItem->SetIndex(pScoreItem->ReadObject(STAGE_SCOREITEM_03[type]));
		break;
	default:
		break;
	}

	pScoreItem->SetPos(pos);
	pScoreItem->SetRot({ 0.0f, D3DX_PI / 2, 0.0f });
	pScoreItem->SetSize({ 1.0f,1.0f,1.0f });
	pScoreItem->SetShadow(pos);

	return pScoreItem;
}