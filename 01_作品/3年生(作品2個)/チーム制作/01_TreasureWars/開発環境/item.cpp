//==============================================================================================
//
// アイテム　　　	item.cpp
// tutida ryousei
//
//==============================================================================================
#include"item.h"
#include"application.h"
#include"charmanager.h"
#include"itemmanager.h"
#include"game.h"
#include "particle_emitter.h"
#include"shadow.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CItem::CItem()
{
	// アイテム生成時に自身のポインタをアイテムマネージャーに設定
	CGame::GetItemManager()->SetItem(this);

	m_fRadius = 0.0f;
	m_nLife = 0;
	m_emitTime = 0;
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CItem::~CItem()
{
	CItemManager* pManager = CGame::GetItemManager();

	// マネージャーが使用中の場合、解放時に自身のポインタを配列から除外する
	if (pManager != nullptr) pManager->DestroyItem(this);
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CItem::Init()
{
	// ランダムでアイテムの種類を設定
	m_nRand_Type = (Item_Type)(rand() % ITEM_MAX);

	// アイテム名の格納先
	char *Item_Name = "\0";

	// タイプ別の色の設定
	switch (m_nRand_Type)
	{
	case CItem::ITEM_SPEEDUP:
		Item_Name = "Data/model/Item/item_rollershoes.x";
		break;
	case CItem::ITEM_KNOCKBACK:
		Item_Name = "Data/model/Item/item_hammer.x";
		break;
	case CItem::ITEM_STACK:
		Item_Name = "Data/model/Item/item_basket.x";
		break;
	default:
		break;
	}
	// モデル設定
	SetIndex(this->ReadObject(Item_Name));

	m_fRadius = 45.0f;
	m_nLife = 480;
	m_Rot = { 0.0f, 0.0f, 0.0f };
	m_bAlpha = false;

	m_pShadow = CShadow::Create({ m_Pos.x, 5.0f, m_Pos.z }, { 50.0f, 50.0f, 0.0f });

	CObjectX::Init();
	CObjectX::SetSize(m_Size);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CItem::Uninit()
{
	CObjectX::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CItem::Update()
{
	CObjectX::Update();

	// アイテムの周りに出るパーティクル
	m_emitTime++;
	if ((m_emitTime % 40) == 0) CParticleEmitter::Create("ItemHighlight", m_Pos);

	// キャラクターとの当たり判定
	CollItem();

	// 点滅
	Flash_Item();

	// 時間経過で消滅
	Life();

	// 回転
	m_Rot.y += 0.01f;

	CObjectX::SetPos(m_Pos);
	CObjectX::SetRot(m_Rot);
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CItem::Draw()
{
	CObjectX::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CItem *pItem = nullptr;

	pItem = new CItem;

	if (pItem != nullptr)
	{
		pItem->SetItemPos(pos);		// 位置の設定
		pItem->SetItemSize(size);	// サイズの設定
		pItem->Init();
	}

	return pItem;
}

//==============================================================================================
// キャラクターとの当たり判定
//==============================================================================================
void CItem::CollItem()
{
	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		D3DXVECTOR3 CharPos = { 0.0f,0.0f,0.0f };
		float fCharRadius = 0.0f;

		// キャラクターの情報
		CCharacter* pChara = CApplication::GetCharManager()->GetChara(nCnt);

		if (pChara != nullptr)
		{
			CharPos = pChara->GetPos();				// 位置
			fCharRadius = pChara->GetRadius();		// 当たり判定の半径

		// 円の当たり判定
			bool Hit = CObject::CollisionCircle(m_Pos, m_fRadius, CharPos, fCharRadius);

			if (Hit)
			{
				// プレイヤーの位置にパーティクル
				CParticleEmitter::Create("ItemGet", m_Pos);

				// タイプ別の処理
				Type(pChara);

				// アイテムを消す
				Uninit();

				// 影を消す処理
				if (m_pShadow != nullptr)
				{
					m_pShadow->Uninit();
					m_pShadow = nullptr;
				}

				// アイテム取得効果を発揮する
				pChara->Item_Effect(m_nRand_Type);
			}
		}
	}
}

//==============================================================================================
// アイテムのタイプ別の処理
//==============================================================================================
void CItem::Type(CCharacter* pChara)
{
	int nItemLevel = 0;

	// アイテム取得SE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_ITEM);

	switch (m_nRand_Type)
	{
	case CItem::ITEM_SPEEDUP:
		// 現在のレベルを取得
		nItemLevel = pChara->GetMoveSpeed_Level();
		nItemLevel++;

		// 移動速度一段階上昇
		pChara->SetMoveSpeed_Level(nItemLevel);
		break;

	case CItem::ITEM_KNOCKBACK:
		// 現在のレベルを取得
		nItemLevel = pChara->GetKnockBack_Level();
		nItemLevel++;

		// ノックバックの距離を伸ばす
		pChara->SetKnockBack_Level(nItemLevel);
		break;
	case CItem::ITEM_STACK:
		// 現在のレベルを取得
		nItemLevel = pChara->GetGemStack_Level();
		nItemLevel++;

		// 一度に持てる鉱石の量を増やす
		pChara->SetGemStack_Level(nItemLevel);
		break;
	default:
		break;
	}
}

//==============================================================================================
// アイテムの点滅
//==============================================================================================
void CItem::Flash_Item()
{
	if (m_nLife < 260)
	{
		// 点滅するか
		if (m_fAlpha >= 1)
			m_bAlpha = false;
		if (m_fAlpha <= 0)
			m_bAlpha = true;

		// アルファ値
		if (!m_bAlpha)
			m_fAlpha -= 0.05f;
		else
			m_fAlpha += 0.05f;

		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			// アイテムの色
			SetAlpha(m_fAlpha);
		}
	}
}

//==============================================================================================
// 時間経過で消滅
//==============================================================================================
void CItem::Life()
{
	m_nLife--;

	// 体力が0になったら
	if (m_nLife <= 0)
	{
		Uninit();
		// 影を消す処理
		if (m_pShadow != nullptr)
		{
			m_pShadow->Uninit();
			m_pShadow = nullptr;
		}
	}
}