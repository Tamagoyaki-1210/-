//=============================================================================
//
// collision.cpp
// Author : Tutida Ryousei
//
//=============================================================================
#include "collision.h"
#include "collision_manager.h"
#include "application.h"
#include "tutorial.h"
#include "game.h"
#include "move_object.h"
#include "object3D.h"
#include "meshfield.h"

#include"debugProc.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CCollision::CCollision()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	// 生成時に自身のポインタを当たり判定マネージャーに設定
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CTutorial::GetCollision_Manager()->SetCollision(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CGame::GetCollision_Manager()->SetCollision(this);
	}
	m_bDeath = false;
	m_bNoneHit = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCollision::~CCollision()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCollision::Init()
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCollision::Uninit()
{
	// 親の当たり判定ポインタを消す処理
	m_pParent->CollisionDestroy();

#ifdef _DEBUG
	// デバッグオブジェクトが使用中の場合
	if (m_pDebugObj != nullptr)
	{
		m_pDebugObj->Uninit();
		m_pDebugObj = nullptr;
	}
#endif // _DEBUG

	Releace();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCollision::Update()
{
#ifdef _DEBUG
	DebugObj();
#endif // _DEBUG

	// 当たり判定
	Collision();
}

//=============================================================================
// 当たり判定
//=============================================================================
void CCollision::Collision()
{
	// ヒット処理を読み込む場合
	if (m_bNoneHit == false)
	{
		// 自分の位置
		D3DXVECTOR3 pos = m_pParent->GetCenterPos();
		// 半径
		float fRadius = m_pParent->GetRadius();
		// サイズ
		D3DXVECTOR3 Size = m_pParent->GetSize();

		// 現在のモード
		CApplication::MODE Mode = CApplication::GetModeType();

		CCollision_Manager* pCollision_Manager = nullptr;

		// モード毎にプレイヤーを読み込む
		if (Mode == CApplication::MODE_TUTORIAL)
		{
			pCollision_Manager = CTutorial::GetCollision_Manager();
		}
		else if (Mode == CApplication::MODE_GAME)
		{
			pCollision_Manager = CGame::GetCollision_Manager();
		}

		// 全ての当たり判定を個別に判定
		for (auto pCollision : pCollision_Manager->GetAllCollision())
		{
			// 当たり判定が存在する場合 && 当たり判定が自身ではない場合 && ヒット判定がある場合
			if (!pCollision->GetParent()->GetDeathFlag() && !pCollision->GetDeath() && pCollision != this && !pCollision->m_bNoneHit)
			{
				// 相手のmoveobjectの情報
				CMove_Object* pMove_Object = pCollision->GetParent();

				// 相手の位置
				D3DXVECTOR3 OtherPos = pMove_Object->GetCenterPos();

				// 当たっているかどうか
				bool bHit = false;

				// 当たり判定のタイプが円の場合
				if (pMove_Object->GetCollision_Type() == CMove_Object::COLLISION_TYPE_SHERER)
				{
					// 半径
					float fOtherRadius = pMove_Object->GetRadius();

					// 円同士の当たり判定の計算
					bHit = Sphere_Collision(pos, fRadius, OtherPos, fOtherRadius);
				}
				// 当たり判定のタイプが矩形の場合
				else if (pMove_Object->GetCollision_Type() == CMove_Object::COLLISION_TYPE_BLOCK
					&& pMove_Object->GetCollision_Type() != m_pParent->GetCollision_Type())
				{
					D3DXVECTOR3 Pos = m_pParent->GetPos();
					// 前回の位置
					D3DXVECTOR3 PosOld = m_pParent->GetPosOld();

					// 位置
					D3DXVECTOR3 Otherpos = pMove_Object->GetPos();
					// サイズ
					D3DXVECTOR3 OtherSize = pMove_Object->GetSize();

					// 矩形の当たり判定の計算
					bHit = Block_Collision(Pos, PosOld, Size, Otherpos, OtherSize, m_pParent, pMove_Object);
				}

				// ヒットした場合
				if (bHit)
				{
					// 自身の接触判定
					m_pParent->Hit(pMove_Object);

					// 相手の接触判定
					pMove_Object->Hit(m_pParent);
				}
			}
		}
	}
}

//==============================================================================================
// オブジェクトを継承したものの当たり判定
//==============================================================================================
bool CCollision::Sphere_Collision(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius)
{
	// 位置同士の距離の計算用変数
	D3DXVECTOR3 Distance = otherPos - pos;

	float Dis = sqrtf((otherPos.x - pos.x) * (otherPos.x - pos.x)
		+ (otherPos.z - pos.z) * (otherPos.z - pos.z));

	float DisY = otherPos.y - pos.y;
	if (DisY < 0)
		DisY *= -1;

	if (Dis <= radius + otherRadius
		&& DisY <= radius + otherRadius)
		return true;

	return false;
}

//=============================================================================
// 矩形の当たり判定
//=============================================================================
bool CCollision::Block_Collision(const D3DXVECTOR3 pos, const D3DXVECTOR3 posold, const D3DXVECTOR3 size, const D3DXVECTOR3 otherpos, const D3DXVECTOR3 othersize, CMove_Object *objParent, CMove_Object *objOther)
{
	D3DXVECTOR3 Pos = pos;
	bool bHit = false;

	// 矩形の上に乗った場合 又は 設置している判定
	if ((otherpos.y + othersize.y > pos.y && otherpos.y < pos.y + size.y + othersize.y)
		|| objParent->GetLandObj())
	{
		//// 上に乗る
		//if (otherpos.y + othersize.y <= posold.y
		//	&& otherpos.x + othersize.x > pos.x - size.x
		//	&& otherpos.x - othersize.x < pos.x + size.x
		//	&& otherpos.z + othersize.z > pos.z - size.z
		//	&& otherpos.z - othersize.z < pos.z + size.z)
		//{
		//	// オブジェクトに乗ってる数
		//	int nOnObj = objOther->GetOnObjCnt();
		//	bool bOn = false;

		//	for (int nCnt = 0; nCnt < nOnObj; nCnt++)
		//	{
		//		if (objOther->GetOnObj(nCnt) == objParent)
		//			bOn = true;
		//	}

		//	if (!bOn)
		//	{
		//		objOther->SetOnObj(objParent, nOnObj);
		//		nOnObj++;
		//	}

		//	objOther->SetOnObjCnt(nOnObj);

		//	objOther->SetLandObj(true);		// Move_Objectが上に乗ってるか
		//	objOther->SetObjXZ(true);

		//	objParent->SetOnObj(objOther, 0);
		//	objParent->SetLandObj(true);									// オブジェクトの上に乗ってるか
		//	objParent->SetObjXZ(true);										// オブジェクトとXZ軸が重なってるか
		//	objParent->SetObjY(otherpos.y + othersize.y);					// オブジェクトの高さ
		//	objParent->SetPos({ Pos.x,otherpos.y + othersize.y,Pos.z });	// 押し出し
		//	bHit = true;
		//}
		//// 降りる
		//else
		//{
		//	for (int nCnt = 0; nCnt < objOther->GetOnObjCnt(); nCnt++)
		//	{
		//		if (objOther->GetOnObj(nCnt) != nullptr
		//			&& objOther->GetLandObj() && objOther->GetObjXZ()
		//			&& objOther->GetOnObj(nCnt)->GetLandObj() && objOther->GetOnObj(nCnt)->GetObjXZ()
		//			&& objOther->GetOnObj(nCnt) == objParent)
		//		{
		//			objOther->SetLandObj(false);
		//			objOther->SetObjXZ(false);

		//			for (int nCnt2 = 0; nCnt2 < objOther->GetOnObjCnt(); nCnt2++)
		//			{
		//				objOther->SetOnObj(nullptr, nCnt2);
		//			}
		//			objOther->SetOnObjCnt(0);

		//			objParent->SetLandObj(false);
		//			objParent->SetGround(false);
		//			objParent->SetObjXZ(false);
		//			bHit = false;
		//		}
		//	}
		//}
		// 下からぶつかる
		/*if (otherpos.y > posold.y
			&& otherpos.x + othersize.x > pos.x - size.x
			&& otherpos.x - othersize.x < pos.x + size.x
			&& otherpos.z + othersize.z > pos.z - size.z
			&& otherpos.z - othersize.z < pos.z + size.z)
		{
			objParent->SetPos({ Pos.x,otherpos.y - size.y - othersize.y,Pos.z });	// 押し出し
		}
		// 着地している場合
		else */if (!objParent->GetLandObj())
		{
			D3DXVECTOR3 Index[4] = {};				// オブジェクトの4頂点の位置
			D3DXVECTOR3 Index_Vec[4] = {};			// 頂点から頂点までのベクトル
			D3DXVECTOR3 Char_Vec[4] = {};			// 頂点からキャラまでのベクトル
			D3DXVECTOR3 CharCenter_Vec[4] = {};		// 頂点からキャラの中心までのベクトル
			float fCp[4] = {};						// 外積
			float fCenter_Cp[4] = {};				// 外積
			int nCp_Count = 0;						// キャラが内側にいるベクトルのカウント
			int nHit_Index = 0;						// ヒットしたベクトルの番号

			D3DXVECTOR3 Posplus[4] = {};
			Posplus[0] = { pos.x + size.x,0.0f,pos.z };
			Posplus[1] = { pos.x,0.0f,pos.z - size.z };
			Posplus[2] = { pos.x - size.x,0.0f,pos.z };
			Posplus[3] = { pos.x,0.0f,pos.z + size.z };

			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				// オブジェクトの四頂点の位置
				Index[nCnt] = otherpos + objOther->GetIndex(nCnt);
			}
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				if (nCnt != 3)
					Index_Vec[nCnt] = Index[nCnt + 1] - Index[nCnt];
				else
					Index_Vec[nCnt] = Index[0] - Index[nCnt];

				Char_Vec[nCnt] = Posplus[nCnt] - Index[nCnt];
				CharCenter_Vec[nCnt] = pos - Index[nCnt];

				// プラスが外側
				fCp[nCnt] = Index_Vec[nCnt].x * Char_Vec[nCnt].z - Index_Vec[nCnt].z * Char_Vec[nCnt].x;
				fCenter_Cp[nCnt] = Index_Vec[nCnt].x * CharCenter_Vec[nCnt].z - Index_Vec[nCnt].z * CharCenter_Vec[nCnt].x;

				if (fCp[nCnt] <= 0)
					nCp_Count++;
			}

			// キャラがオブジェクトにヒットしている場合
			if (nCp_Count == 4)
			{
				for (int nCnt = 0; nCnt < 4; nCnt++)
				{
					if (fCenter_Cp[nCnt] >= 0)
					{
						float fInner = D3DXVec3Dot(&Index_Vec[nCnt], &CharCenter_Vec[nCnt]);

						if (fInner > 0.0f)
						{
							// 当たってる面
							nHit_Index = nCnt;
							bHit = true;

							break;
						}
					}
				}

				//if (objParent->GetCollision_Type() != CMove_Object::COLLISION_TYPE_BOSS)
				//{
				//	// 押し出し
				//	switch (nHit_Index)
				//	{
				//	case 0:
				//		objParent->SetPos({ Index[nHit_Index].x - size.x,pos.y,pos.z });
				//		break;
				//	case 1:
				//		objParent->SetPos({ pos.x,pos.y,Index[nHit_Index].z + size.z });
				//		break;
				//	case 2:
				//		objParent->SetPos({ Index[nHit_Index].x + size.x,pos.y,pos.z });
				//		break;
				//	case 3:
				//		objParent->SetPos({ pos.x,pos.y,Index[nHit_Index].z - size.z });
				//		break;
				//	default:
				//		break;
				//	}
				//}
			}
		}
	}

	return bHit;
}

// デバッグ用関数の処理
#ifdef _DEBUG
//=============================================================================
// デバッグオブジェクトの処理
//=============================================================================
void CCollision::DebugObj()
{
	// デバッグオブジェクトが使用中の場合
	if (m_pDebugObj != nullptr)
	{
		// 位置を設定
		m_pDebugObj->SetPos(m_pParent->GetCenterPos());
	}
}

//=============================================================================
// デバッグオブジェクトの処理
//=============================================================================
void CCollision::SetDebugObj()
{
	const float fRadius = m_pParent->GetRadius() * 2.0f;

	// デバッグオブジェクトの生成
	m_pDebugObj = CObject3D::Create(m_pParent->GetCenterPos(), D3DXVECTOR2(fRadius, fRadius));

	// 色の設定(プレイヤー側かどうか ? 水色 : 赤色)
	m_pDebugObj->SetCol(m_pParent->GetPlayerSide() ? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	m_pDebugObj->SetTexture(CTexture::TEXTURE_CIRCLE);
	m_pDebugObj->Setbillboard(true);
}
#endif // _DEBUG

//=============================================================================
// 生成処理
//=============================================================================
CCollision* CCollision::Create(CMove_Object* pParent)
{
	CCollision* pCollision = new CCollision;

	if (pCollision != nullptr)
	{
		pCollision->Init();

		// 親ポインタが使用中の場合
		if (pParent != nullptr)
		{
			pCollision->m_pParent = pParent;

#ifdef _DEBUG
			// デバッグオブジェクトの設定
			pCollision->SetDebugObj();
#endif // _DEBUG

		}
	}

	return pCollision;
}