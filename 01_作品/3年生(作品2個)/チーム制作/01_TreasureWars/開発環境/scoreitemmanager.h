//=============================================================================
//
// scoreitemmanager.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _SCOREITEM_MANAGER_H_
#define _SCOREITEM_MANAGER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "scoreitem.h"
#include <vector>

//---------------------------
// クラス宣言
//---------------------------
class CScoreItemManager
{
public:
	CScoreItemManager();
	~CScoreItemManager();

	void DestroyScoreItem(CScoreItem* scoreitem) { m_AllScoreItem.erase(std::find(m_AllScoreItem.begin(), m_AllScoreItem.end(), scoreitem)); }
	void SetScoreItem(CScoreItem* scoreitem) { m_AllScoreItem.push_back(scoreitem); }

	std::vector<CScoreItem*> GetAllScoreItem() { return m_AllScoreItem; }
private:

	std::vector<CScoreItem*> m_AllScoreItem;		// 全てのスコアアイテムの情報
};

#endif// _SCOREITEM_MANAGER_H_