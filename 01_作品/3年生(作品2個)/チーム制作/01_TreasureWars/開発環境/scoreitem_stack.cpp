//=============================================================================
//
// scoreitem_stack.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include <time.h>
#include "scoreitem_stack.h"
#include "application.h"
#include "renderer.h"
#include "game.h"
#include "time.h"
#include "receipt.h"
#include "shop.h"
#include "character.h"
#include "stage_select.h"

// スコアアイテムの色一覧
const D3DXCOLOR CScoreItemStack::Col_Yellow = D3DCOLOR_RGBA(255, 255, 16, 255);
const D3DXCOLOR CScoreItemStack::Col_Green = D3DCOLOR_RGBA(34, 255, 25, 255);
const D3DXCOLOR CScoreItemStack::Col_Bule = D3DCOLOR_RGBA(40, 72, 255, 255);
const D3DXCOLOR CScoreItemStack::Col_Red = D3DCOLOR_RGBA(252, 20, 20, 255);
const D3DXCOLOR CScoreItemStack::Col_Gray = D3DCOLOR_RGBA(224, 224, 224, 255);
const D3DXCOLOR CScoreItemStack::Col_White = D3DCOLOR_RGBA(255, 255, 255, 255);

const float CScoreItemStack::ScoreItem_Stack_Size = 30.0f;	// スコアアイテム画像のサイズ

//=====================================
// デフォルトコンストラクタ
//=====================================
CScoreItemStack::CScoreItemStack()
{

}

//=====================================
// デストラクタ
//=====================================
CScoreItemStack::~CScoreItemStack()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CScoreItemStack::Init()
{
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CScoreItemStack::Uninit()
{
	// 領収書の終了処理
	if (m_pReceipt != nullptr)
	{
		m_pReceipt->Uninit();
		m_pReceipt = nullptr;
	}

	// 配列の中身の全て削除する
	m_listScoreItem.clear();

	// UIの終了処理
	CUi::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CScoreItemStack::Update()
{
	CUi::Update();
}

//============================================================================
// 描画処理
//============================================================================
void CScoreItemStack::Draw()
{

}

//============================================================================
// 宝石の紛失処理
//============================================================================
void CScoreItemStack::LostScoreItem(const int lost, const D3DXVECTOR3 pos)
{
	for (int nCnt = 0; nCnt < lost; nCnt++)
	{
		// 配列を使用している場合
		if (!m_listScoreItem.empty())
		{
			// 配列の一番後ろの情報
			ScoreItem gem = m_listScoreItem.back();

			// スコアアイテムの生成
			CScoreItem::Create(pos, gem.nType);

			// スコアアイテムのデフォルト画像を描画する
			m_apScoreItem[m_listScoreItem.size() - 1]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_NONE);

			// 配列の一番後ろの削除
			m_listScoreItem.pop_back();
		}
		else
		{
			break;
		}
	}
}

//============================================================================
// スコアアイテムの売却処理
//============================================================================
int CScoreItemStack::SaleScoreItem()
{
	// 宝石の値段の返り値用の変数
	int nValue = 0;

	// 配列を使用している場合
	if (!m_listScoreItem.empty())
	{
		// 配列の一番後ろの情報
		ScoreItem scoreItem = m_listScoreItem.back();

		// スコアアイテムのデフォルト画像を描画する
		m_apScoreItem[m_listScoreItem.size() - 1]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_NONE);

		// 配列の一番後ろの削除
		m_listScoreItem.pop_back();

		// 文字列の変数
		std::string str;
		std::string strCol;

		strCol += std::to_string(scoreItem.col.r * 255) + "," + std::to_string(scoreItem.col.g * 255) + "," + std::to_string(scoreItem.col.b * 255) + "," + std::to_string(scoreItem.col.a * 255) + ",";
		str += "<c=" + strCol + ">" + scoreItem.name + "</>";
		nValue = scoreItem.value;

		// 数字をchar*型に変換str += "<c=0,0,0,255,>$" + std::to_string(value) + "</>";
		str += "<c=255,255,255,255,>$</><c=255,255,255,255,>" + std::to_string(nValue) + "</>";

		// 領収書に文字列を記述(設定)する
		m_pReceipt->SetTotalValue(nValue);
		m_pReceipt->SetString(str);
	}

	// スコアアイテムの値段の返り値
	return nValue;
}

//============================================================================
// スコアアイテムの設定処理
//============================================================================
void CScoreItemStack::SetScoreItem(const int type)
{
	// 最大数を超えない場合
	if ((int)m_listScoreItem.size() < CCharacter::Max_GemStack)
	{
		// 配列の最大数
		int listSize = (int)m_listScoreItem.size();

		// 宝石構造体の初期化
		ScoreItem scoreItem = {};

		// 宝石構造体に情報を代入する
		scoreItem.getTime = CGame::GetTime()->GetTime();
		scoreItem.nType = type;

		// スコアアイテムの画像を設定する
		switch (CStage_Select::GetStage_Number())
		{
		case CStage_Select::STAGE_1:
			// 種類毎の処理
			switch (scoreItem.nType)
			{
			case GEM_DIAMOND:
				scoreItem.name = "ダイヤモンド";
				scoreItem.col = Col_Gray;
				scoreItem.value = 500;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_DIAMOND);
				break;
			case GEM_RUBY:
				scoreItem.name = "ルビー　　　";
				scoreItem.col = Col_Red;
				scoreItem.value = 400;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_RUBY);
				break;
			case GEM_SAPPHIRE:
				scoreItem.name = "サファイア　";
				scoreItem.col = Col_Bule;
				scoreItem.value = 300;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_SAPPHIRE);
				break;
			case GEM_EMERALD:
				scoreItem.name = "エメラルド　";
				scoreItem.col = Col_Green;
				scoreItem.value = 200;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_EMERALD);
				break;
			case GEM_TOPAZ:
				scoreItem.name = "トパーズ　　";
				scoreItem.col = Col_Yellow;
				scoreItem.value = 100;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_TOPAZ);
				break;
			default:
				break;
			}
			break;
		case CStage_Select::STAGE_2:
			// 種類毎の処理
			switch (scoreItem.nType)
			{
			case FOSSIL_HEAD:
				scoreItem.name = "あたま　　　";
				scoreItem.col = Col_Gray;
				scoreItem.value = 1200;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_HEAD);
				break;
			case FOSSIL_AMBER:
				scoreItem.name = "コハク　　　";
				scoreItem.col = Col_Yellow;
				scoreItem.value = 800;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_AMBER);
				break;
			case FOSSIL_BONE:
				scoreItem.name = "ホネ　　　　";
				scoreItem.col = Col_Red;
				scoreItem.value = 500;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_BONE);
				break;
			case FOSSIL_FOOTPRINS:
				scoreItem.name = "あしあと　　";
				scoreItem.col = Col_Green;
				scoreItem.value = 300;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_FOOTPRINTS);
				break;
			case FOSSIL_AMMONITE:
				scoreItem.name = "アンモナイト";
				scoreItem.col = Col_Bule;
				scoreItem.value = 200;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_AMMONITE);
				break;
			default:
				break;
			}
			break;
		case CStage_Select::STAGE_3:
			// 種類毎の処理
			switch (scoreItem.nType)
			{
			case DEBRIS_CPUCHIP:
				scoreItem.name = "CPUチップ";
				scoreItem.col = Col_Gray;
				scoreItem.value = 2677;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_CPU);
				break;
			case DEBRIS_PANEL:
				scoreItem.name = "パネル　　　";
				scoreItem.col = Col_Bule;
				scoreItem.value = 1453;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_SOLARPANEL);
				break;
			case DEBRIS_ANTENNA:
				scoreItem.name = "アンテナ　　";
				scoreItem.col = Col_Green;
				scoreItem.value = 997;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_ANTENNA);
				break;
			case DEBRIS_ENGINE:
				scoreItem.name = "エンジン　　";
				scoreItem.col = Col_Red;
				scoreItem.value = 521;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_ENGINE);
				break;
			case DEBRIS_OXYGENBONB:
				scoreItem.name = "さんそボンベ";
				scoreItem.col = Col_Yellow;
				scoreItem.value = 53;
				m_apScoreItem[m_listScoreItem.size()]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_OXYGENTANK);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		// 配列の一番後ろの次の場所に追加する
		m_listScoreItem.push_back(scoreItem);
	}
}

//============================================================================
// プレイヤーから最大所持数を獲得
//============================================================================
void CScoreItemStack::SetDefault(const int num)
{
	// 現在未使用のスタック分を追加
	for (int nCnt = 0; nCnt < num; nCnt++)
	{
		m_apScoreItem[nCnt]->SetDrawFlag(true);
	}
}

//============================================================================
// 生成処理
//============================================================================
CScoreItemStack* CScoreItemStack::Create(const D3DXVECTOR3 pos, const int index)
{
	CScoreItemStack* pScoreItemStack = new CScoreItemStack;

	if (FAILED(pScoreItemStack->Init()))
	{
		return nullptr;
	}

	pScoreItemStack->m_pos = pos;
	pScoreItemStack->m_pReceipt = CReceipt::Create(index);	// スコアアイテムスタックのレシート

	for (int nCnt = 0; nCnt < 20; nCnt++)
	{
		pScoreItemStack->m_apScoreItem[nCnt] = CObject2D::Create(pos +
			D3DXVECTOR3(
			ScoreItem_Stack_Size * 0.5f * (nCnt % (CCharacter::Max_GemStack / 2)) + ((int)(nCnt / (CCharacter::Max_GemStack / 2)) * ScoreItem_Stack_Size * 0.25f),
			ScoreItem_Stack_Size * 0.5f * (int)(nCnt / (CCharacter::Max_GemStack / 2)),
			0.0f),
			{ ScoreItem_Stack_Size, ScoreItem_Stack_Size, 0.0f }, 5);
		pScoreItemStack->m_apScoreItem[nCnt]->SetTexture(CTexture::TEXTURE_DROP_SCORE_ITEM_NONE);
		pScoreItemStack->m_apScoreItem[nCnt]->SetDrawFlag(false);
	}

	return pScoreItemStack;
}
