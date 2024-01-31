//=============================================================================
//
// 落とし物の管理　　　drop_manager.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _DROP_MANAGER_H_
#define _DROP_MANAGER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "drop_weapon.h"
#include <vector>

//---------------------------
// クラス宣言
//---------------------------
class CDropManager
{
public:

	CDropManager();
	~CDropManager();

	void ReleaseAllDrop() { for (auto pDrop : m_AllDrop) pDrop->Uninit(); }
	void DestroyDrop(CDrop_Weapon* enemy) { m_AllDrop.erase(std::find(m_AllDrop.begin(), m_AllDrop.end(), enemy)); }
	void SetDrop(CDrop_Weapon* enemy) { m_AllDrop.push_back(enemy); }

	std::vector<CDrop_Weapon*> GetAllDrop() { return m_AllDrop; }
private:

	std::vector<CDrop_Weapon*> m_AllDrop;		// 全ての落とし物の情報
};

#endif// _DROP_MANAGER_H_