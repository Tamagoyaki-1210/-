//=============================================================================
//
// 順位
// Author : tanimoto kosuke
//
//=============================================================================
#include "rank.h"
#include "number.h"
#include <stdio.h>
#include "score.h"
#include "application.h"
#include "texture.h"

//=====================================
//デフォルトコンストラクタ
//=====================================
CRank::CRank() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//デストラクタ
//=====================================
CRank::~CRank()
{

}

//=====================================
//初期化処理
//=====================================
void CRank::Init()
{
	CObject2D::Init();

	m_fWidth = COMMA_WIDTH;
	m_fHeight = COMMA_HEIGHT;

	//オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);
}

//=====================================
//終了処理
//=====================================
void CRank::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CRank::Update()
{
	CObject2D::Update();

	//UV設定処理
	CObject2D::SetUV();
}

//=====================================
//描画処理
//=====================================
void CRank::Draw()
{
	CApplication::GetTexture()->TextureType(CTexture::TYPE_COMMA);

	CObject2D::Draw();
}

//=====================================
//スコア生成処理
//=====================================
CRank *CRank::Create(D3DXVECTOR3 pos, int nID)
{
	CRank* pRank = new CRank;		//スコアポインタ

	if (pRank != nullptr)
	{
		pRank->Init();
		pRank->SetPos(pos);

		CNumber *pNumber2 = CNumber::Create(pos, CNumber::TYPE_RANKING);

		//2桁
		pNumber2->SetPos(D3DXVECTOR3(pos.x - pNumber2->GetWidth() * 2, pos.y, 0.0f));
		pNumber2->SetNumber((nID + 1) / 10);

		CNumber *pNumber = CNumber::Create(pos, CNumber::TYPE_RANKING);
		//1桁
		pNumber->SetPos(D3DXVECTOR3(pos.x - pNumber->GetWidth(), pos.y, 0.0f));
		pNumber->SetNumber((nID + 1) % 10);
	}
	return pRank;
}