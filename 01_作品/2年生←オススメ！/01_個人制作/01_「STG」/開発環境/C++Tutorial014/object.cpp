//=============================================================================
//
// オブジェクト
// Author : tanimoto kosuke
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include "game.h"
#include "Application.h"
#include "mode.h"

CObject *CObject::m_pTop[MAX_OBJECT_PRIORITY] = {};
CObject *CObject::m_pCurrent[MAX_OBJECT_PRIORITY] = {};
//=====================================
// デフォルトコンストラクタ
//=====================================
CObject::CObject(int priority) : m_bDeath(false)
{
	// 現在のオブジェクトが使用中の場合
	if (m_pCurrent[priority] != nullptr)
	{
		m_pCurrent[priority]->m_pNext = this;
		m_pPrev = m_pCurrent[priority];
		m_pCurrent[priority] = this;
	}
	else
	{
		m_pTop[priority] = this;
		m_pCurrent[priority] = this;
		m_pPrev = nullptr;
	}
	m_pNext = nullptr;
}

//=====================================
// デストラクタ
//=====================================
CObject::~CObject()
{
	
}

//=====================================
// 解放処理
//=====================================
void CObject::Release()
{
	//Uninit();
	m_bDeath = true;
}

//=====================================
// 全ての解放処理
//=====================================
void CObject::ReleaseAll(void)
{
	for (int i = 0; i < MAX_OBJECT_PRIORITY; i++)
	{
		// 先頭ポインタが使用中の場合
		if (m_pTop[i] != nullptr)
		{
			//現在位置ポインタに先頭ポインタを代入
			CObject *pObject = m_pTop[i];
			//現在位置ポインタが未使用になるまで
			while (pObject != nullptr)
			{
				//CObject *pObjectNext = pObject->m_pNext;
				pObject->Uninit();
				pObject = pObject->m_pNext;
			}
		}
	}
	for (int i = 0; i < MAX_OBJECT_PRIORITY; i++)
	{
		// 先頭ポインタが使用中の場合
		if (m_pTop[i] != nullptr)
		{
			//現在位置ポインタに先頭ポインタを代入
			CObject *pObject = m_pTop[i];
			//現在位置ポインタが使用中の場合
			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;

				// 前回ポインタが使用中の場合
				if (pObject->m_pPrev != nullptr)
				{
					// 次回ポインタが使用中の場合
					if (pObject->m_pNext != nullptr)
					{
						pObject->m_pPrev->m_pNext = pObject->m_pNext;
						pObject->m_pNext->m_pPrev = pObject->m_pPrev;
					}
					else
					{
						pObject->m_pPrev->m_pNext = nullptr;
						m_pCurrent[i] = pObject->m_pPrev;
					}
				}
				else
				{
					// 次回ポインタが使用中の場合
					if (pObject->m_pNext != nullptr)
					{
						pObject->m_pNext->m_pPrev = nullptr;
						m_pTop[i] = pObject->m_pNext;
					}
					else
					{
						m_pTop[i] = nullptr;
						m_pCurrent[i] = nullptr;
					}
				}
				delete pObject;
				pObject = pObjectNext;
			}
		}
	}
}

//=====================================
// 全ての更新処理
//=====================================
void CObject::UpdateAll()
{
	for (int i = 0; i < MAX_OBJECT_PRIORITY; i++)
	{
		// 先頭ポインタが使用中の場合
		if (m_pTop[i] != nullptr)
		{
			//現在位置ポインタに先頭ポインタを代入
			CObject *pObject = m_pTop[i];

			//現在のモード
			CMode::MODE mode = CApplication::GetMode()->GetMode();

			//現在位置ポインタが使用中の場合
			while (pObject != nullptr)
			{
				//死亡フラグが建っていない場合
				if (!pObject->m_bDeath)
				{
					//ゲーム中であり、描画順がUIより下の場合
					if (mode == CMode::MODE_GAME && i < UI_PRIORITY_UI)
					{
						if (CGame::GetPause() == false)
						{
							pObject->Update();
						}
					}
					else
					{
						pObject->Update();
					}
				}
				pObject = pObject->m_pNext;
			}
		}
	}
	for (int i = 0; i < MAX_OBJECT_PRIORITY; i++)
	{
		// 先頭ポインタが使用中の場合
		if (m_pTop[i] != nullptr)
		{
			//現在位置ポインタに先頭ポインタを代入
			CObject *pObject = m_pTop[i];
			//現在位置ポインタが使用中の場合
			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;
				//死亡フラグが建っている場合
				if (pObject->m_bDeath)
				{
					// 前回ポインタが使用中の場合
					if (pObject->m_pPrev != nullptr)
					{
						// 次回ポインタが使用中の場合
						if (pObject->m_pNext != nullptr)
						{
							pObject->m_pPrev->m_pNext = pObject->m_pNext;
							pObject->m_pNext->m_pPrev = pObject->m_pPrev;
						}
						else
						{
							pObject->m_pPrev->m_pNext = nullptr;
							m_pCurrent[i] = pObject->m_pPrev;
						}
					}
					else
					{
						// 次回ポインタが使用中の場合
						if (pObject->m_pNext != nullptr)
						{
							pObject->m_pNext->m_pPrev = nullptr;
							m_pTop[i] = pObject->m_pNext;
						}
						else
						{
							m_pTop[i] = nullptr;
							m_pCurrent[i] = nullptr;
						}
					}
					delete pObject;
				}
				pObject = pObjectNext;
			}
		}
	}
}

//=====================================
// 全ての描画処理
//=====================================
void CObject::DrawAll()
{
	for (int i = 0; i < MAX_OBJECT_PRIORITY; i++)
	{
		// 先頭ポインタが使用中の場合
		if (m_pTop[i] != nullptr)
		{
			//現在位置ポインタに先頭ポインタを代入
			CObject *pObject = m_pTop[i];

			//現在位置ポインタが使用中の場合
			while (pObject != nullptr)
			{
				CObject *pObjectNext = pObject->m_pNext;
				pObject->Draw();
				pObject = pObjectNext;
			}
		}
	}
}

//=====================================
// オブジェクト設定処理
//=====================================
void CObject::SetType(Object_Type type)
{
	m_type = type;
}

//=====================================
// 種別取得処理
//=====================================
const CObject::Object_Type& CObject::GetType() const
{
	return m_type;
}