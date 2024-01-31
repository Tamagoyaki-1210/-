//=============================================================================
//
// ���Ƃ����̊Ǘ��@�@�@drop_manager.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _DROP_MANAGER_H_
#define _DROP_MANAGER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "drop_weapon.h"
#include <vector>

//---------------------------
// �N���X�錾
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

	std::vector<CDrop_Weapon*> m_AllDrop;		// �S�Ă̗��Ƃ����̏��
};

#endif// _DROP_MANAGER_H_