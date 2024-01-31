//==============================================================================================
//
// オブジェクト　　　object.cpp
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"object.h"
#include <assert.h>

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
CObject *CObject::m_pTop[] = {};
CObject *CObject::m_pCurrent[] = {};

//==============================================================================================
// コンストラクタ
//==============================================================================================
CObject::CObject(const PRIORITY priority)
{
	// Topがnullの場合
	if (m_pTop[priority] == nullptr)
	{
		m_pTop[priority] = this;
		m_pTop[priority]->m_pNext = nullptr;
		m_pTop[priority]->m_pPrev = nullptr;
	}

	// Currentがnullじゃない場合
	if (m_pCurrent[priority] != nullptr)
	{
		m_pCurrent[priority]->m_pNext = this;
		m_pCurrent[priority]->m_pNext->m_pNext = nullptr;
		m_pCurrent[priority]->m_pNext->m_pPrev = m_pCurrent[priority];
	}

	m_pCurrent[priority] = this;

	// プライオリティーの設定
	m_nPriority = priority;

	m_bDraw = true;
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CObject::~CObject()
{
}

//==============================================================================================
// 全体の解放処理
//==============================================================================================
void CObject::ReleaseAll()
{
	for (int nPri = 0; nPri < PRIORITY_MAX; nPri++)
	{
		if (m_pTop[nPri] != nullptr)
		{
			CObject *pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				pObject->Uninit();
				pObject->Death(nPri);

				pObject = pObjectNext;
			}
		}
	}
}

//==============================================================================================
// 全体の更新処理
//==============================================================================================
void CObject::UpdateAll()
{
	for (int nPri = 0; nPri < PRIORITY_MAX; nPri++)
	{
		if (m_pTop[nPri] != nullptr)
		{
			CObject *pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				// 死亡フラグの確認
				if (!pObject->m_bDeath)
				{
					pObject->Update();
				}

				pObject = pObjectNext;
			}

			pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				// 死亡フラグの確認
				if (pObject->m_bDeath)
				{
					pObject->Death(nPri);
				}

				pObject = pObjectNext;
			}
		}
	}
}

//==============================================================================================
// 全体の描画処理
//==============================================================================================
void CObject::DrawAll()
{
	for (int nPri = 0; nPri < PRIORITY_MAX; nPri++)
	{
		if (m_pTop[nPri] != nullptr)
		{
			CObject *pObject = m_pTop[nPri];

			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				if (pObject->GetDrawFlag())
				{
					if (!pObject->m_bDeath)
					{
						pObject->Draw();
					}
				}

				pObject = pObjectNext;
			}
		}
	}
}

//==============================================================================================
// オブジェクトの解放
//==============================================================================================
void CObject::Death(const int nPriority)
{
	// オブジェクト(自分自身)をリストから削除
	// Topが消えた
	if (this == m_pTop[nPriority])
	{
		m_pTop[nPriority] = m_pTop[nPriority]->m_pNext;

		if (m_pTop[nPriority] != nullptr)
		{
			m_pTop[nPriority]->m_pPrev = nullptr;
		}
		else
		{
			m_pCurrent[nPriority] = nullptr;
		}
	}
	// Currentが消えた
	else if (this == m_pCurrent[nPriority])
	{
		m_pCurrent[nPriority] = m_pCurrent[nPriority]->m_pPrev;
		m_pCurrent[nPriority]->m_pNext = nullptr;
	}
	else
	{
		this->m_pPrev->m_pNext = this->m_pNext;
		this->m_pNext->m_pPrev = this->m_pPrev;
	}

	// オブジェクト(自分自身)を破棄

	delete this;
}

//==============================================================================================
// オブジェクトを継承したものの当たり判定
//==============================================================================================
bool CObject::Sphere_Collision(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius)
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
