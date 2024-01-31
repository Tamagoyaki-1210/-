//=============================================================================
//
// itemmanager.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "item.h"
#include <vector>

//---------------------------
// クラス宣言
//---------------------------
class CItemManager
{
public:

	CItemManager();
	~CItemManager();

	void DestroyItem(CItem* item) { m_AllItem.erase(std::find(m_AllItem.begin(), m_AllItem.end(), item)); }
	void SetItem(CItem* item) { m_AllItem.push_back(item); }

	std::vector<CItem*> GetAllItem() { return m_AllItem; }
private:

	std::vector<CItem*> m_AllItem;		// 全ての敵キャラの情報
};

#endif// _ITEM_MANAGER_H_