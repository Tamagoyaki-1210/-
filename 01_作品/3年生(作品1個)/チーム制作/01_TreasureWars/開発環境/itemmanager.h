//=============================================================================
//
// itemmanager.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "item.h"
#include <vector>

//---------------------------
// �N���X�錾
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

	std::vector<CItem*> m_AllItem;		// �S�Ă̓G�L�����̏��
};

#endif// _ITEM_MANAGER_H_