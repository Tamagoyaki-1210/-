//=============================================================================
//
// アイテム
// Author : tanimoto kosuke
//
//=============================================================================
#include "item.h"
#include "application.h"
#include "texture.h"
#include "player.h"
#include "score.h"
#include "matrix.h"

//---------------------------
//マクロ関数
//---------------------------
#define ITEM_WIDTH	(50)
#define ITEM_HEIGHT	(50)

//=====================================
//デフォルトコンストラクタ
//=====================================
CItem::CItem() : CObject2D(OBJECT_PRIORITY_CENTER), m_nLife(0), m_move(D3DXVECTOR3(-2.0f, 0.0f, 0.0f))
{

}

//=====================================
//デストラクタ
//=====================================
CItem::~CItem()
{

}

//=====================================
//初期化処理
//=====================================
void CItem::Init()
{
	CObject2D::Init();
	m_fWidth = ITEM_WIDTH;
	m_fHeight = ITEM_HEIGHT;

	//オブジェクト設定処理
	SetObject2D(CObject::TYPE_ITEM, m_fWidth, m_fHeight);
}

//=====================================
//終了処理
//=====================================
void CItem::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CItem::Update()
{
	CObject2D::Update();

	//UV設定
	CObject2D::SetUV();

	//位置取得処理
	D3DXVECTOR3 pos = CObject2D::GetPos();

	//長さ取得処理
	float fLength = CObject2D::GetLength();

	//位置更新
	pos += m_move;

	//位置設定処理
	SetPos(pos);

	//位置設定処理
	SetLength(fLength);

	//寿命を減らす
	m_nLife--;

	//寿命計算
	if (m_nLife <= 0)
	{
		Destroy();
	}
	else if (m_nLife == 300)
	{
		m_type = ENERGY_NONE;
	}
	else if (m_nLife < 300)
	{
		BlinkColor();
	}

	//当たり判定設定処理
	Collision(pos, fLength);
}

//=====================================
//描画処理
//=====================================
void CItem::Draw()
{
	//画像取得処理
	CApplication::GetTexture()->TextureType(CTexture::TYPE_ITEM);
	CObject2D::Draw();
}


//=====================================
//種別設定処理
//=====================================
void CItem::TypeSet()
{
	switch (m_type)
	{
	case  ENERGY_01:
		CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_nLife = 600;
		break;
	case  ENERGY_02:
		CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		m_nLife = 600;
		break;
	case  ENERGY_03:
		CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		m_nLife = 1000;
		break;
	case  ENERGY_04:
		CObject2D::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_nLife = 800;
		break;
	default:
		break;
	}
}

//=====================================
//点滅色処理
//=====================================
void CItem::BlinkColor()
{
	if (m_bBlink)
	{
		m_fBlinkCounter--;
	}
	else
	{
		m_fBlinkCounter++;
	}

	if (m_fBlinkCounter >= 1.0f / 0.2f)
	{
		m_bBlink = true;
		m_fBlinkCounter = 1.0f / 0.2f;
		m_nBlinkLoop++;
	}
	else if (m_fBlinkCounter <= 0.0f)
	{
		m_bBlink = false;
		m_fBlinkCounter = 0.0f;
	}

	//点滅させる部分の色を計算
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fBlinkCounter * 0.2f));

	if (m_nBlinkLoop >= 4)
	{
		m_nBlinkLoop = 0;
	}
}

//=====================================
//消去処理
//=====================================
void CItem::Destroy()
{
	Uninit();
}

//=====================================
//当たり判定設定処理
//=====================================
void CItem::Collision(D3DXVECTOR3 pos, float fLength)
{
	CObject *pCenter = (CObject*)CObject2D::GetTop(OBJECT_PRIORITY_CENTER);	//オブジェクトの先頭ポインタ
	if (pCenter != nullptr)
	{
		//現在位置ポインタに先頭ポインタを代入
		CObject *pObject = pCenter;

		//現在位置ポインタが使用中の場合
		while (pObject != nullptr)
		{
			CObject::Object_Type objtype = pObject->GetType();

			if (objtype == CObject::TYPE_PLAYER)
			{//プレイヤーの場合
				if (CollisionCircle(pos, fLength, pObject->GetPos(), pObject->GetLength()))
				{//重なった場合
					CSound::PlaySound(SOUND_LABEL_SE_ITEM_GET);

					CPlayer *pPlayer = (CPlayer*)pObject;
					CScore *pScore = CScore::GetScore();

					switch (m_type)
					{
					case  ENERGY_NONE:
						pScore->AddScore(500);
						break;
					case  ENERGY_01:
						//被弾処理
						pPlayer->AddLife(20);
						break;
					case  ENERGY_02:
						//速度上昇処理
						pPlayer->AddSpeed(0.1f);
						break;
					case  ENERGY_03:
						//段数増加処理
						pPlayer->AddBullet();
						break;
					case  ENERGY_04:
						//段数増加処理
						pPlayer->AddMaxLife(10);
						break;
					default:
						break;
					}
					//アイテム破壊
					Destroy();
				}
			}
			//現在位置ポインタに次回ポインタを代入する(ポインタを進める処理)
			pObject = pObject->GetNext();
		}
	}
}

////=====================================
////円型当たり判定計算処理
////=====================================
//bool CItem::CollisionCircle(D3DXVECTOR3 pos, float fLength, D3DXVECTOR3 posSub, float fLengthSub)
//{
//	float fSumLengthRad = (fLength + fLengthSub) * 0.5f;				//円の半径同士の和
//
//	float fDiffX = pos.x - posSub.x;									//X座標の差分
//	float fDiffY = pos.y - posSub.y;									//Y座標の差分
//	float fDiffLength = sqrtf((fDiffX * fDiffX) + (fDiffY * fDiffY));	//現在の2点の距離
//
//	return fSumLengthRad >= fDiffLength;
//}

//=====================================
//敵キャラ弾生成処理
//=====================================
CItem *CItem::Create(D3DXVECTOR3 pos, Item_Type settype)
{
	CItem* pItem = new CItem;	//弾のポインタ

	if (pItem != nullptr)
	{
		pItem->Init();
		pItem->SetPos(pos);
		pItem->m_type = settype;
		pItem->TypeSet();	//種別処理
	}
	return pItem;
}