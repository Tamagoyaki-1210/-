//=============================================================================
//
// 敵キャラの管理　　　enemy_manager.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "enemy.h"
#include <vector>

//---------------------------
// クラス宣言
//---------------------------
class CEnemyManager
{
public:

	CEnemyManager();
	~CEnemyManager();

	void ReleaseAllEnemy() { for (auto pEnemy : m_AllEnemy) pEnemy->Uninit(); }
	void DestroyEnemy(CEnemy* enemy) { m_AllEnemy.erase(std::find(m_AllEnemy.begin(), m_AllEnemy.end(), enemy)); }
	void SetEnemy(CEnemy* enemy) { m_AllEnemy.push_back(enemy); }

	std::vector<CEnemy*> GetAllEnemy() { return m_AllEnemy; }
private:

	std::vector<CEnemy*> m_AllEnemy;		// 全ての敵キャラの情報
};

#endif// _ENEMY_MANAGER_H_