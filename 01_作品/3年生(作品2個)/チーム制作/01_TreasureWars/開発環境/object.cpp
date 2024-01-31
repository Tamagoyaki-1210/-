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

//**********************************************************************************************
// 静的メンバ変数の宣言
//**********************************************************************************************
CObject *CObject::m_pTop[] = {};
CObject *CObject::m_pCurrent[] = {};

const float CObject::AttackLength_Decreace = 50.0f;	// 攻撃射程の減少
//==============================================================================================
// コンストラクタ
//==============================================================================================
CObject::CObject(int nPriority)
{
	// Topがnullの場合
	if (m_pTop[nPriority] == nullptr)
	{
		m_pTop[nPriority] = this;
		m_pTop[nPriority]->m_pNext = nullptr;
		m_pTop[nPriority]->m_pPrev = nullptr;
	}

	// Currentがnullじゃない場合
	if (m_pCurrent[nPriority] != nullptr)
	{
		m_pCurrent[nPriority]->m_pNext = this;
		m_pCurrent[nPriority]->m_pNext->m_pNext = nullptr;
		m_pCurrent[nPriority]->m_pNext->m_pPrev = m_pCurrent[nPriority];
	}

	m_pCurrent[nPriority] = this;

	// プライオリティーの設定
	m_nPriority = nPriority;

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
	for (int nPri = 0; nPri < m_nMax_Pri; nPri++)
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
	for (int nPri = 0; nPri < m_nMax_Pri; nPri++)
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
	for (int nPri = 0; nPri < m_nMax_Pri; nPri++)
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
// 死亡フラグを立てる
//==============================================================================================
void CObject::Release()
{
	this->m_bDeath = true;
}

//==============================================================================================
// オブジェクトの解放
//==============================================================================================
void CObject::Death(int nPriority)
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
// 二つの位置の距離
//==============================================================================================
float CObject::Distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float Distance = sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.z - pos2.z) * (pos1.z - pos2.z));

	return Distance;
}

//==============================================================================================
// 円同士の当たり判定(自分側 : [位置、当たり判定のサイズ(半径)]、相手側 : [位置、当たり判定のサイズ(半径)])
//==============================================================================================
bool CObject::CollisionCircle(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius)
{
	// 位置同士の距離の計算用変数
	D3DXVECTOR2 distance = { otherPos.x - pos.x, otherPos.z - pos.z };

	// 距離と大きさから計算し、衝突している場合
	return sqrtf((distance.x * distance.x) + (distance.y * distance.y)) <= (radius + otherRadius);
}

//==============================================================================================
// 攻撃様の当たり判定(自分側 : [位置、向き、攻撃射程、攻撃範囲]、相手側 : [位置、当たり判定のサイズ(半径)])
//==============================================================================================
bool CObject::AttackCollision(const D3DXVECTOR3 pos, const float rot, const float length, const float angle, const D3DXVECTOR3 otherPos, const float otherRadius)
{
	// 攻撃の原点
	D3DXVECTOR3 startPos = pos + D3DXVECTOR3(sinf(rot) * AttackLength_Decreace, 0.0f, cosf(rot) * AttackLength_Decreace);

	// 点と扇のベクトル
	D3DXVECTOR2 vec_fan_to_point = { otherPos.x - startPos.x, otherPos.z - startPos.z };

	// 円(射程)と円(相手の大きさ)で当たっているかどうか
	if (!CollisionCircle(startPos, length, otherPos, otherRadius)) return false;

	// 扇を2等分する線のベクトルを求める
	D3DXVECTOR2 fan_dir = { -sinf(rot), -cosf(rot) };

	// 円と扇のベクトルを正規化
	D3DXVec2Normalize(&vec_fan_to_point, &vec_fan_to_point);

	// 円の内積を計算する
	float dot = (vec_fan_to_point.x * fan_dir.x) + (vec_fan_to_point.y * fan_dir.y);

	// 円の内積が扇の範囲内にあるかを比較する
	return cosf(D3DXToRadian(angle * 0.5f)) <= dot;
}

//==============================================================================================
// 指定Posから指定角度内に対象Posが存在するかどうか
//==============================================================================================
bool CObject::CheckInAngle(const D3DXVECTOR3 pos, const float rot, const float angle, const D3DXVECTOR3 otherPos)
{
	// 点と扇のベクトル
	D3DXVECTOR2 vec_fan_to_point = { otherPos.x - pos.x, otherPos.z - pos.z };

	// 扇を2等分する線のベクトルを求める
	D3DXVECTOR2 fan_dir = { sinf(rot), -cosf(rot) };

	// 円と扇のベクトルを正規化
	D3DXVec2Normalize(&vec_fan_to_point, &vec_fan_to_point);
	D3DXVec2Normalize(&fan_dir, &fan_dir);

	// 円の内積を計算する
	float dot = (vec_fan_to_point.x * fan_dir.x) + (vec_fan_to_point.y * fan_dir.y);

	// 円の内積が扇の範囲内にあるかを比較する
	return cosf(D3DXToRadian(angle * 0.5f)) <= dot;
}
