//==============================================================================================
//
// �R���W�����}�l�[�W���[�@�@�@collision_manager.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _COLLISION_MANAGER_H_
#define	_COLLISION_MANAGER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"collision.h"
#include <vector>

class CCollision_Manager
{
public:
	CCollision_Manager();
	~CCollision_Manager();

	void UpdateAll();

	void ReleaseAllCollision();
	void DestroyCollision(CCollision* collision) { m_AllCollision.erase(std::find(m_AllCollision.begin(), m_AllCollision.end(), collision)); }
	void SetCollision(CCollision* collision) { m_AllCollision.push_back(collision); }

	std::vector<CCollision*> GetAllCollision() { return m_AllCollision; }
private:

	std::vector<CCollision*> m_AllCollision;		// �S�ẴR���W�����̏��
};

#endif // !_COLLISION_MANAGER_H_
