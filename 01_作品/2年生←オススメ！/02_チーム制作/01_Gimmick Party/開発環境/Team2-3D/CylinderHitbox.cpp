//=============================================================================
//
// hitbox.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "CylinderHitbox.h"
#include "line.h"
#include "score.h"

//コンストラクタ
CCylinderHitbox::CCylinderHitbox()
{
	//メンバー変数をクリアする

	//デバッグ用の線
	for (int nCnt = 0; nCnt < 18; nCnt++)
	{
		m_pLine[nCnt] = nullptr;
	}
}

//デストラクタ
CCylinderHitbox::~CCylinderHitbox()
{

}

//初期化処理
HRESULT CCylinderHitbox::Init(void)
{
	if (FAILED(CHitbox::Init()))
	{//基本クラスの初期化処理
		return -1;
	}

	//デバッグ用の線
	for (int nCnt = 0; nCnt < 18; nCnt++)
	{
		m_pLine[nCnt] = nullptr;
	}

	return S_OK;
}

//終了処理
void CCylinderHitbox::Uninit(void)
{
	//デバッグ用の線の破棄
	for (int nCnt = 0; nCnt < 18; nCnt++)
	{
		if (m_pLine[nCnt] != nullptr)
		{
			m_pLine[nCnt]->Release();
			m_pLine[nCnt] = nullptr;
		}
	}

	//基本クラスの終了処理
	CHitbox::Uninit();
}

//更新処理
void CCylinderHitbox::Update(void)
{
	if (GetType() == TYPE_PLAYER)
	{//プレイヤーのヒットボックスだったら、
		std::vector <CHitbox*>* pHbx = GetAllHitbox();			//全部のヒットボックスを取得する

		for (int nCnt = 0; nCnt < (int)pHbx->size(); nCnt++)
		{//取得したヒットボックスを全部判定する

			if (pHbx->data()[nCnt] != this && pHbx->data()[nCnt]->GetParent() != GetParent())
			{//親が同じではなかったら

				HITBOX_SHAPE shape = pHbx->data()[nCnt]->GetShape();			//形の取得

				switch (shape)
				{
				case CHitbox::SHAPE_SPHERE:
					break;

				case CHitbox::SHAPE_BOX:

				{//矩形の場合

					if (PointBoxHit(pHbx->data()[nCnt]->GetPos(), pHbx->data()[nCnt]->GetRot(), pHbx->data()[nCnt]->GetSize()))
					{//当たった場合

						if (!GetInvincibility())
						{//無敵状態ではなかったら

							if (GetType() == TYPE_PLAYER && (pHbx->data()[nCnt]->GetType() == TYPE_VANISHING || pHbx->data()[nCnt]->GetType() == TYPE_OBSTACLE))
							{
								pHbx->data()[nCnt]->SetCollisionState(true);
							}

							if (GetType() == TYPE_PLAYER && pHbx->data()[nCnt]->GetScore() != 0)
							{//スコアがあったら、スコアを設定する
								if (GetPlayerIdx() >= 0)
								{
									CScore::AddScore(GetPlayerIdx(), pHbx->data()[nCnt]->GetScore());
								}
							}

							if (GetEffect() == EFFECT_MAX && pHbx->data()[nCnt]->GetEffect() != EFFECT_MAX)
							{//エフェクトがあったら、エフェクトを設定する
								SetEffect(pHbx->data()[nCnt]->GetEffect());
							}

							if (pHbx->data()[nCnt]->GetEffect() == CHitbox::EFFECT_BOUNCE)
							{
								D3DXVECTOR3 dir = GetParent()->GetPos() - pHbx->data()[nCnt]->GetPos();
								pHbx->data()[nCnt]->SetDirection(dir);
							}

							if (pHbx->data()[nCnt]->GetDirection() != Vec3Null)
							{
								SetDirection(pHbx->data()[nCnt]->GetDirection());
							}
						}
					}
				}

				break;

				case CHitbox::SHAPE_CYLINDER:

				{//シリンダーの場合

					if (CylinderCylinderHit(pHbx->data()[nCnt]->GetPos(), pHbx->data()[nCnt]->GetSize()))
					{//当たった場合

						if (!GetInvincibility())
						{//無敵状態ではなかったら

							pHbx->data()[nCnt]->SetCollisionState(true);		//当たった状態にする

							if (GetType() == TYPE_PLAYER && pHbx->data()[nCnt]->GetScore() != 0)
							{//スコアがあったら、スコアの設定

								if (GetPlayerIdx() >= 0)
								{
									CScore::AddScore(GetPlayerIdx(), pHbx->data()[nCnt]->GetScore());
								}
							}

							if (GetEffect() == EFFECT_MAX && pHbx->data()[nCnt]->GetEffect() != EFFECT_MAX)
							{//エフェクトがあったら、エフェクトを設定する
								SetEffect(pHbx->data()[nCnt]->GetEffect());
							}

							if (pHbx->data()[nCnt]->GetEffect() == CHitbox::EFFECT_BOUNCE)
							{
								D3DXVECTOR3 dir = GetParent()->GetPos() - pHbx->data()[nCnt]->GetPos();
								pHbx->data()[nCnt]->SetDirection(dir);
							}

							if (pHbx->data()[nCnt]->GetDirection() != Vec3Null)
							{
								SetDirection(pHbx->data()[nCnt]->GetDirection());
							}
						}
					}
				}

				break;

				default:
					break;
				}
			}
		}
	}

	//デバッグ用の線の更新
	for (int nCnt = 0; nCnt < 18; nCnt++)
	{
		if (m_pLine[nCnt] != nullptr)
		{
			m_pLine[nCnt]->SetPos(GetPos());
		}
	}

	//基本クラスの更新
	CHitbox::Update();
}


//=============================================================================
//
//							静的関数
//
//=============================================================================

//生成処理
CCylinderHitbox* CCylinderHitbox::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, CObject* pParent)
{
	CCylinderHitbox* pHitbox = new CCylinderHitbox;				//ヒットボックスの生成

	if (FAILED(pHitbox->Init()))
	{//初期化処理
		return nullptr;
	}

	pHitbox->SetRelativePos(RelativePos);				//相対位置の設定
	pHitbox->SetPos(pos);								//位置の設定
	pHitbox->SetLastPos(pos);							//前回の位置の設定
	pHitbox->SetSize(size);								//サイズの設定
	pHitbox->SetType(type);								//種類の設定
	pHitbox->SetShape(CHitbox::SHAPE_CYLINDER);			//形の設定
	pHitbox->SetParent(pParent);						//親の設定

#ifdef _DEBUG

	//デバッグ用の線の生成
	D3DXVECTOR3 Vtx[12];

	for (int nCnt2 = 0; nCnt2 < 2; nCnt2++)
	{
		for (int nCnt = 0; nCnt < 6; nCnt++)
		{
			Vtx[(nCnt2 * 6) + nCnt] = D3DXVECTOR3(size.x * cosf((2.0f * D3DX_PI * nCnt) / 6), size.y * nCnt2, size.z * sinf((2.0f * D3DX_PI * nCnt) / 6));
		}
	}

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		pHitbox->m_pLine[nCnt] = CLine::Create(pos, Vec3Null, Vtx[nCnt], Vtx[nCnt + 1], D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		pHitbox->m_pLine[nCnt + 6] = CLine::Create(pos, Vec3Null, Vtx[nCnt], Vtx[nCnt + 6], D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		pHitbox->m_pLine[nCnt + 12] = CLine::Create(pos, Vec3Null, Vtx[nCnt + 6], Vtx[nCnt + 7], D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}

	pHitbox->m_pLine[5] = CLine::Create(pos, Vec3Null, Vtx[5], Vtx[0], D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[11] = CLine::Create(pos, Vec3Null, Vtx[5], Vtx[11], D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[17] = CLine::Create(pos, Vec3Null, Vtx[11], Vtx[6], D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

#endif // !_DEBUG

	return pHitbox;
}

//生成処理
CCylinderHitbox* CCylinderHitbox::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, CObject* pParent, const int nPlayerIdx)
{
	CCylinderHitbox* pHitbox = new CCylinderHitbox;		//ヒットボックスの生成

	if (FAILED(pHitbox->Init()))
	{//初期化処理
		return nullptr;
	}

	pHitbox->SetRelativePos(RelativePos);				//相対位置の設定
	pHitbox->SetPos(pos);								//位置の設定
	pHitbox->SetLastPos(pos);							//前回の位置の設定
	pHitbox->SetSize(size);								//サイズの設定
	pHitbox->SetType(type);								//種類の設定
	pHitbox->SetShape(CHitbox::SHAPE_CYLINDER);			//形の設定
	pHitbox->SetParent(pParent);						//親の設定
	pHitbox->SetPlayerIdx(nPlayerIdx);					//プレイヤーインデックスの設定

#ifdef _DEBUG

	//デバッグ用の線の生成
	D3DXVECTOR3 Vtx[12];

	for (int nCnt2 = 0; nCnt2 < 2; nCnt2++)
	{
		for (int nCnt = 0; nCnt < 6; nCnt++)
		{
			Vtx[(nCnt2 * 6) + nCnt] = D3DXVECTOR3(size.x * cosf((2.0f * D3DX_PI * nCnt) / 6), size.y * nCnt2, size.z * sinf((2.0f * D3DX_PI * nCnt) / 6));
		}
	}

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		pHitbox->m_pLine[nCnt] = CLine::Create(pos, Vec3Null, Vtx[nCnt], Vtx[nCnt + 1], D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		pHitbox->m_pLine[nCnt + 6] = CLine::Create(pos, Vec3Null, Vtx[nCnt], Vtx[nCnt + 6], D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		pHitbox->m_pLine[nCnt + 12] = CLine::Create(pos, Vec3Null, Vtx[nCnt + 6], Vtx[nCnt + 7], D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	}

	pHitbox->m_pLine[5] = CLine::Create(pos, Vec3Null, Vtx[5], Vtx[0], D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pHitbox->m_pLine[11] = CLine::Create(pos, Vec3Null, Vtx[5], Vtx[11], D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pHitbox->m_pLine[17] = CLine::Create(pos, Vec3Null, Vtx[11], Vtx[6], D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

#endif // !_DEBUG

	return pHitbox;					//生成したインスタンスを返す
}

//生成処理
CCylinderHitbox* CCylinderHitbox::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, int nScore, CObject* pParent)
{
	CCylinderHitbox* pHitbox = new CCylinderHitbox;				//ヒットボックスの生成

	if (FAILED(pHitbox->Init()))
	{//初期化処理
		return nullptr;
	}

	pHitbox->SetRelativePos(RelativePos);				//相対位置の設定
	pHitbox->SetPos(pos);								//位置の設定
	pHitbox->SetLastPos(pos);							//前回の位置の設定
	pHitbox->SetSize(size);								//サイズの設定
	pHitbox->SetType(type);								//種類の設定
	pHitbox->SetShape(CHitbox::SHAPE_CYLINDER);			//形の設定
	pHitbox->SetParent(pParent);						//親の設定
	pHitbox->SetScore(nScore);							//スコアの設定

#ifdef _DEBUG

	//デバッグ用の線の生成
	D3DXVECTOR3 Vtx[12];

	for (int nCnt2 = 0; nCnt2 < 2; nCnt2++)
	{
		for (int nCnt = 0; nCnt < 6; nCnt++)
		{
			Vtx[(nCnt2 * 6) + nCnt] = D3DXVECTOR3(size.x * cosf((2.0f * D3DX_PI * nCnt) / 6), size.y * nCnt2, size.z * sinf((2.0f * D3DX_PI * nCnt) / 6));
		}
	}

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		pHitbox->m_pLine[nCnt] = CLine::Create(pos, Vec3Null, Vtx[nCnt], Vtx[nCnt + 1], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		pHitbox->m_pLine[nCnt + 6] = CLine::Create(pos, Vec3Null, Vtx[nCnt], Vtx[nCnt + 6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		pHitbox->m_pLine[nCnt + 12] = CLine::Create(pos, Vec3Null, Vtx[nCnt + 6], Vtx[nCnt + 7], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	pHitbox->m_pLine[5] = CLine::Create(pos, Vec3Null, Vtx[5], Vtx[0], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[11] = CLine::Create(pos, Vec3Null, Vtx[5], Vtx[11], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[17] = CLine::Create(pos, Vec3Null, Vtx[11], Vtx[6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

#endif // !_DEBUG

	return pHitbox;						//生成したインスタンスを返す
}

CCylinderHitbox* CCylinderHitbox::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, int nScore, CObject* pParent, CHitbox::INTERACTION_EFFECT effect)
{
	CCylinderHitbox* pHitbox = new CCylinderHitbox;				//ヒットボックスの生成

	if (FAILED(pHitbox->Init()))
	{//初期化処理
		return nullptr;
	}

	pHitbox->SetRelativePos(RelativePos);				//相対位置の設定
	pHitbox->SetPos(pos);								//位置の設定
	pHitbox->SetLastPos(pos);							//前回の位置の設定
	pHitbox->SetSize(size);								//サイズの設定
	pHitbox->SetType(type);								//種類の設定
	pHitbox->SetShape(CHitbox::SHAPE_CYLINDER);			//形の設定
	pHitbox->SetParent(pParent);						//親の設定
	pHitbox->SetScore(nScore);							//スコアの設定
	pHitbox->SetEffect(effect);							//エフェクトの設定

#ifdef _DEBUG

	//デバッグ用の線の生成
	D3DXVECTOR3 Vtx[12];

	for (int nCnt2 = 0; nCnt2 < 2; nCnt2++)
	{
		for (int nCnt = 0; nCnt < 6; nCnt++)
		{
			Vtx[(nCnt2 * 6) + nCnt] = D3DXVECTOR3(size.x * cosf((2.0f * D3DX_PI * nCnt) / 6), size.y * nCnt2, size.z * sinf((2.0f * D3DX_PI * nCnt) / 6));
		}
	}

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		pHitbox->m_pLine[nCnt] = CLine::Create(pos, Vec3Null, Vtx[nCnt], Vtx[nCnt + 1], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		pHitbox->m_pLine[nCnt + 6] = CLine::Create(pos, Vec3Null, Vtx[nCnt], Vtx[nCnt + 6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		pHitbox->m_pLine[nCnt + 12] = CLine::Create(pos, Vec3Null, Vtx[nCnt + 6], Vtx[nCnt + 7], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}

	pHitbox->m_pLine[5] = CLine::Create(pos, Vec3Null, Vtx[5], Vtx[0], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[11] = CLine::Create(pos, Vec3Null, Vtx[5], Vtx[11], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pHitbox->m_pLine[17] = CLine::Create(pos, Vec3Null, Vtx[11], Vtx[6], D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

#endif // !_DEBUG

	return pHitbox;					//生成したインスタンスを返す
}


//=============================================================================
//
//						　　　　プライベート関数
//
//=============================================================================



//シリンダーとシリンダーの当たり判定
bool CCylinderHitbox::CylinderCylinderHit(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	D3DXVECTOR3 thisPos, thisSize;				//このヒットボックスの前回位置の取得
	thisPos = GetPos();							//このヒットボックスの現在位置の取得
	thisSize = GetSize();						//このヒットボックスののサイズ取得

	float fTop, fBottom;				//他のヒットボックスの上側と下側

	fTop = pos.y + size.y;				//上側の座標
	fBottom = pos.y;					//下側の座標

	if (fTop >= thisPos.y && fBottom <= thisPos.y + thisSize.y)
	{//Y座標が重なった場合

		float fDistance, fRadius;
		fDistance = sqrtf(((pos.x - thisPos.x) * (pos.x - thisPos.x)) + ((pos.z - thisPos.z) * (pos.z - thisPos.z)));		//距離
		fRadius = size.x + thisSize.x;				//半径の足し算

		if (fDistance < fRadius)
		{//距離は半径の足し算より小さかったら

			D3DXVECTOR3 thisLast = GetLastPos();					//前回の位置の取得
			D3DXVECTOR3 parentPos = GetParent()->GetPos();			//親の取得

			if (thisLast.y + thisSize.y <= pos.y)
			{//下から当たった場合
				thisPos.y = (pos.y - thisSize.y - 0.01f);		//新しい位置の計算
				parentPos = thisPos - GetRelativePos();			
				GetParent()->SetPos(parentPos);					//親を押し出す
				SetLastPos(thisPos);							//前回の位置の設定
			}
			else if (thisLast.y >= pos.y + size.y)
			{//上から当たった場合
				thisPos.y = (pos.y + size.y + 0.01f);			//新しい位置の計算
				parentPos = thisPos - GetRelativePos();
				GetParent()->SetPos(parentPos);					//親を押し出す
				SetLastPos(thisPos);							//前回の位置の設定
			}
			else
			{//横から当たった場合
				D3DXVECTOR3 dist = thisPos - pos;					//距離を計算する
				float fHeight = parentPos.y;						//現在の高さ
				dist.y = 0.0f;										
				D3DXVec3Normalize(&dist, &dist);					//距離を正規化する
				dist.x *= fRadius + 0.001f;							
				dist.z *= fRadius + 0.001f;							
				dist.x += pos.x;									
				dist.z += pos.z;									
				parentPos = dist - GetRelativePos();				//新しい位置を計算する
				parentPos.y = fHeight;								//元の高さを設定する
				GetParent()->SetPos(parentPos);						//親の位置の設定
			}

			return true;				//trueを返す
		}
	}

	return false;						//falseを返す
}


//bool CCylinderHitbox::CylinderBoxHit(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
//{
//	D3DXVECTOR3 thisPos = GetPos();
//	D3DXVECTOR3 thisSize = GetSize();
//
//	if (thisPos.y + thisSize.y >= pos.y && thisPos.y <= pos.y + size.y)
//	{
//		D3DXVECTOR3 Vtx[4] = {};
//		Vtx[0] = D3DXVECTOR3(-size.x, 0.0f, size.z);
//		Vtx[1] = D3DXVECTOR3(size.x, 0.0f, size.z);
//		Vtx[2] = D3DXVECTOR3(size.x, 0.0f, -size.z);
//		Vtx[3] = D3DXVECTOR3(-size.x, 0.0f, -size.z);
//
//		D3DXMATRIX mtxW, mtxRot, MtxTrans;
//		D3DXMatrixIdentity(&mtxW);
//		D3DXMatrixTranslation(&MtxTrans, pos.x, pos.y, pos.z);
//		D3DXMatrixMultiply(&mtxW, &mtxW, &MtxTrans);
//		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
//		D3DXMatrixMultiply(&mtxW, &mtxW, &mtxRot);
//
//		for (int nCnt = 0; nCnt < 4; nCnt++)
//		{
//			D3DXVec3TransformCoord(&Vtx[nCnt], &Vtx[nCnt], &mtxW);
//		}
//
//		D3DXVECTOR3 edgeVector[4], ObjVector[4], Cross[4];
//
//		edgeVector[0] = Vtx[1] - Vtx[0];
//		edgeVector[1] = Vtx[2] - Vtx[1];
//		edgeVector[2] = Vtx[3] - Vtx[2];
//		edgeVector[3] = Vtx[0] - Vtx[3];
//
//		D3DXVECTOR3 YVector = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//		D3DXVECTOR3 Normal[4] = {};
//		D3DXVECTOR3 nearPoint[4] = {};
//
//		for (int nCnt = 0; nCnt < 4; nCnt++)
//		{
//			/*D3DXVECTOR3 vDis = thisPos - pos;
//			vDis.y = 0.0f;
//			D3DXVec3Normalize(&vDis, &vDis);*/
//			D3DXVECTOR3 normCross = Vec3Null;
//			D3DXVec3Cross(&Normal[nCnt], &edgeVector[nCnt], &YVector);
//			D3DXVec3Normalize(&Normal[nCnt], &Normal[nCnt]);
//			D3DXVec3Cross(&normCross, &edgeVector[nCnt], &Normal[nCnt]);
//
//			if (normCross.y > 0.0f)
//			{
//				Normal[nCnt].x *= -1.0f;
//				Normal[nCnt].y *= -1.0f;
//				Normal[nCnt].z *= -1.0f;
//			}
//
//			nearPoint[nCnt] = thisPos + D3DXVECTOR3(Normal[nCnt].x * thisSize.x /** 0.5f*/, 0.0f, Normal[nCnt].z * thisSize.z /** 0.5f*/);
//		}
//		int nIdx = 0;
//		int nVtxIdx = 0;
//		bool bInt = false;
//
//		for (int nCnt = 0; nCnt < 4; nCnt++)
//		{
//			ObjVector[0] = nearPoint[nCnt] - Vtx[0];
//			ObjVector[1] = nearPoint[nCnt] - Vtx[1];
//			ObjVector[2] = nearPoint[nCnt] - Vtx[2];
//			ObjVector[3] = nearPoint[nCnt] - Vtx[3];
//
//			ObjVector[0].y = 0.0f;
//			ObjVector[1].y = 0.0f;
//			ObjVector[2].y = 0.0f;
//			ObjVector[3].y = 0.0f;
//
//			D3DXVec3Cross(&Cross[0], &edgeVector[0], &ObjVector[0]);
//			D3DXVec3Cross(&Cross[1], &edgeVector[1], &ObjVector[1]);
//			D3DXVec3Cross(&Cross[2], &edgeVector[2], &ObjVector[2]);
//			D3DXVec3Cross(&Cross[3], &edgeVector[3], &ObjVector[3]);
//
//			if (Cross[0].y * Cross[1].y >= 0 && Cross[0].y * Cross[2].y >= 0 && Cross[0].y * Cross[3].y >= 0 && Cross[1].y * Cross[2].y >= 0 && Cross[1].y * Cross[3].y >= 0 && Cross[2].y * Cross[3].y >= 0)
//			{
//				nIdx = nCnt;
//				bInt = true;
//				nVtxIdx = nCnt + 2;
//
//				if (nVtxIdx >= 4)
//				{
//					nVtxIdx -= 4;
//				}
//
//				break;
//			}
//		}
//		if (bInt)
//		{
//			D3DXVECTOR3 LastPos = GetLastPos();
//			D3DXVECTOR3 parentPos = GetParent()->GetPos();
//
//			if (LastPos.y >= pos.y + size.y)
//			{
//				//D3DXVECTOR3 diff = Vec3Null;
//				//diff.y = (pos.y + size.y) - thisPos.y;
//				thisPos.y = pos.y + size.y + 0.001f;
//				//SetPos(thisPos);
//				parentPos = thisPos - GetRelativePos();
//				GetParent()->SetPos(parentPos);
//			}
//			else if (LastPos.y + thisSize.y <= pos.y)
//			{
//				//D3DXVECTOR3 diff = Vec3Null;
//				//diff.y = (pos.y - thisSize.y) - thisPos.y;
//				thisPos.y = (pos.y - thisSize.y) - 0.0001f;
//				//SetPos(thisPos);
//				parentPos = thisPos - GetRelativePos();
//				GetParent()->SetPos(parentPos);
//			}
//			else
//			{
//				D3DXVECTOR3 Center = thisPos - Vtx[nVtxIdx];
//				Center.y = 0.0f;
//				D3DXVECTOR3 radius = nearPoint[nIdx] - thisPos;
//				radius.y = 0.0f;
//				D3DXVECTOR3 versRad = Vec3Null;
//				D3DXVec3Normalize(&versRad, &radius);
//
//				D3DXVECTOR3 c[2] = {};
//				D3DXVec3Cross(&c[0], &Center, &edgeVector[nVtxIdx]);
//				D3DXVec3Cross(&c[1], &versRad, &edgeVector[nVtxIdx]);
//				float f1, f2;
//				f1 = D3DXVec3Length(&c[1]);
//				f2 = D3DXVec3Length(&c[0]);
//
//				if (f2 == 0.0f)
//				{
//					return false;
//				}
//
//				float fK = f1 / f2;
//
//				D3DXVECTOR3 edgeNormal = Vec3Null, dir = Vec3Null;
//				D3DXVec3Normalize(&edgeNormal, &edgeVector[nVtxIdx]);
//				dir = thisPos - GetLastPos();
//				dir.y = 0.0f;
//				D3DXVec3Normalize(&dir, &dir);
//				
//				float fAngle, fLength;
//
//				fAngle = acosf(D3DXVec3Dot(&edgeNormal, &dir));
//
//				D3DXVECTOR3 T = D3DXVECTOR3(thisPos.x + (versRad.x * fK), 0.0f, thisPos.z + (versRad.z * fK));
//				D3DXVECTOR3 TVec = nearPoint[nIdx] - T;
//				TVec.y = 0.0f;
//				fLength = D3DXVec3Length(&TVec);
//
//				D3DXVECTOR3 newPos = nearPoint[nIdx] + D3DXVECTOR3(0.001f + Normal[nVtxIdx].x/* * (thisSize.x * 0.25f)*/, 0.0f, 0.001f + Normal[nVtxIdx].z /** (thisSize.z * 0.25f)*/) + D3DXVECTOR3(Normal[nVtxIdx].x * (fLength + 0.001f) * sinf(fAngle), 0.0f, Normal[nVtxIdx].z * (fLength + 0.001f) * sinf(fAngle));
//
//				GetParent()->SetPos(newPos - GetRelativePos());
//
//			}
//
//
//			return true;
//		}
//	}
//
//	return false;
//}

//点と矩形の当たり判定
bool CCylinderHitbox::PointBoxHit(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 size)
{
	D3DXVECTOR3 thisPos = GetPos();						//このヒットボックスの前回位置の取得
	D3DXVECTOR3 thisSize = GetSize();					//このヒットボックスの現在位置の取得
	D3DXVECTOR3 thisLastPos = GetLastPos();				//このヒットボックスののサイズ取得

	if (thisPos.y + thisSize.y > pos.y && thisPos.y < pos.y + size.y)
	{//Y座標が重なった場合

		D3DXVECTOR3 Vtx[4], V[4], P[4], PCtrl, R[4], RCtrl, N;	//計算用のベクトル
		D3DXMATRIX mtxOut, mtxTrans, mtxRot;					//計算用のマトリックス
		float fResult;											//計算用の変数

		//頂点の相対座標の設定
		Vtx[0] = D3DXVECTOR3(-size.x, 0.0f, size.z);
		Vtx[1] = D3DXVECTOR3(size.x, 0.0f, size.z);
		Vtx[2] = D3DXVECTOR3(size.x, 0.0f, -size.z);
		Vtx[3] = D3DXVECTOR3(-size.x, 0.0f, -size.z);

		//相対座標から絶対座標に変換する
		D3DXMatrixIdentity(&mtxOut);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtxOut, &mtxOut, &mtxRot);
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtxOut, &mtxOut, &mtxTrans);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			D3DXVec3TransformCoord(&Vtx[nCnt], &Vtx[nCnt], &mtxOut);
		}

		//各頂点から次の頂点までのベクトルを計算する
		V[0] = Vtx[1] - Vtx[0];
		V[1] = Vtx[2] - Vtx[1];
		V[2] = Vtx[3] - Vtx[2];
		V[3] = Vtx[0] - Vtx[3];

		//各頂点からこのヒットボックスの位置までのベクトルを計算する
		P[0] = D3DXVECTOR3(thisPos.x - Vtx[0].x, 0.0f, thisPos.z - Vtx[0].z);
		P[1] = D3DXVECTOR3(thisPos.x - Vtx[1].x, 0.0f, thisPos.z - Vtx[1].z);
		P[2] = D3DXVECTOR3(thisPos.x - Vtx[2].x, 0.0f, thisPos.z - Vtx[2].z);
		P[3] = D3DXVECTOR3(thisPos.x - Vtx[3].x, 0.0f, thisPos.z - Vtx[3].z);

		//上のベクトルの外積を計算する
		D3DXVec3Cross(&R[0], &V[0], &P[0]);
		D3DXVec3Cross(&R[1], &V[1], &P[1]);
		D3DXVec3Cross(&R[2], &V[2], &P[2]);
		D3DXVec3Cross(&R[3], &V[3], &P[3]);

		if (R[0].y * R[1].y >= 0 && R[1].y * R[2].y >= 0 && R[2].y * R[3].y >= 0 && R[3].y * R[0].y >= 0)
		{//全部の外積の符号が同じだったら、当たったという意味です

			if (thisLastPos.y >= pos.y + size.y && thisPos.y < pos.y + size.y)
			{//上から当たった場合

				//新しい位置の計算
				thisPos.y = pos.y + size.y + 0.01f;
				SetLastPos(thisPos);									//前回の位置の設定

				GetParent()->SetPos(thisPos - GetRelativePos());		//親の位置の設定

				return true;			//trueを返す
			}
			else if (thisLastPos.y + thisSize.y <= pos.y && thisPos.y + thisSize.y > pos.y)
			{//下から当たった場合

				 //新しい位置の計算
				thisPos.y = pos.y - thisSize.y - 0.01f;

				SetLastPos(thisPos);									//前回の位置の設定
				GetParent()->SetPos(thisPos - GetRelativePos());		//親の位置の設定

				return true;			//trueを返す
			}
			else
			{//横から当たった場合

				for (int Count = 0; Count < 4; Count++)
				{
					//どこから当たったか求める

					PCtrl = D3DXVECTOR3(thisLastPos.x - Vtx[Count].x, 0.0f, thisLastPos.z - Vtx[Count].z);
					D3DXVec3Cross(&RCtrl, &V[Count], &PCtrl);

					D3DXVECTOR3 Ctrl;

					if (RCtrl.y * R[Count].y <= 0)
					{//どこから当たったか分かったら

						P[0] = D3DXVECTOR3(thisPos.x - thisLastPos.x, 0.0f, thisPos.z - thisLastPos.z);				//前回の位置から現在の位置までのベクトル
						P[1] = D3DXVECTOR3(Vtx[Count].x - thisLastPos.x, 0.0f, Vtx[Count].z - thisLastPos.z);		//前回の位置から頂点までのベクトル
						D3DXVec3Normalize(&N, &P[0]);			//前回の位置から現在の位置までのベクトルを正規化する

						//上のベクトルと矩形の辺のベクトルの外積を計算する
						D3DXVec3Cross(&R[0], &P[1], &V[Count]);
						D3DXVec3Cross(&R[1], &N, &V[Count]);

						if (R[1].y == 0)
						{
							break;
						}

						fResult = R[0].y / R[1].y;

						if (fResult > 0)
						{
							fResult -= 0.001f;
						}
						else if (fResult < 0)
						{
							fResult += 0.001f;
						}

						D3DXVECTOR3 Cross, VtxtoPos, VtxtoCross;
						float Dot, Lenght1, Lenght2, Alpha;

						Cross = D3DXVECTOR3(thisLastPos.x + (N.x * fResult), 0.0f, thisLastPos.z + (N.z * fResult));			//交点座標
						VtxtoPos = D3DXVECTOR3(thisPos.x - Vtx[Count].x, 0.0f, thisPos.z - Vtx[Count].z);						//頂点からこのヒットボックスの位置までのベクトル
						VtxtoCross = D3DXVECTOR3(Cross.x - Vtx[Count].x, 0.0f, Cross.z - Vtx[Count].z);							//頂点から交点までのベクトル

						D3DXVec3Normalize(&N, &VtxtoCross);			//頂点から交点までのベクトルを正規化する

						//頂点から交点までのベクトルと頂点からこのヒットボックスの位置までのベクトルの内積を計算する
						Dot = D3DXVec3Dot(&VtxtoCross, &VtxtoPos);
						Lenght1 = sqrtf((VtxtoCross.x * VtxtoCross.x) + (VtxtoCross.z * VtxtoCross.z));		//上のベクトルの長さを計算する
						Lenght2 = sqrtf((VtxtoPos.x * VtxtoPos.x) + (VtxtoPos.z * VtxtoPos.z));				//上のベクトルの長さを計算する

						if ((Lenght1 * Lenght2) * (Lenght1 * Lenght2) >= Dot * Dot && Lenght1 * Lenght2 != 0)
						{//一般
							//新しい位置を計算する
							Alpha = acosf((Dot) / (Lenght1 * Lenght2));

							fResult = (Lenght2 * cosf(Alpha));

							//新しい位置の設定
							thisPos.x = Vtx[Count].x + (N.x * (fResult));
							thisPos.z = Vtx[Count].z + (N.z * (fResult));
						}
						else
						{//エラーがあったら、新しい位置を交点座標に設定する

							thisPos.x = thisLastPos.x + (N.x * fResult);
							thisPos.z = thisLastPos.z + (N.z * fResult);
						}

						break;
					}
				}

				GetParent()->SetPos(thisPos - GetRelativePos());		//親の位置の設定
				SetLastPos(thisPos);									//前回の位置の設定
				SetPos(thisPos);										//現在の位置の設定

				return true;			//trueを返す
			}
		}
	}

	return false;						//falseを返す
}