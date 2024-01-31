//=============================================================================
//
// �G�L�����̊Ǘ��@�@�@enemy_manager.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "enemy.h"
#include <vector>

//---------------------------
// �N���X�錾
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

	std::vector<CEnemy*> m_AllEnemy;		// �S�Ă̓G�L�����̏��
};

#endif// _ENEMY_MANAGER_H_