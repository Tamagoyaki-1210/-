//==============================================================================================
//
// �R���W�����}�l�[�W���[�@�@�@collision_manager.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"collision_manager.h"
#include"move_object.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CCollision_Manager::CCollision_Manager()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CCollision_Manager::~CCollision_Manager()
{
}

//==============================================================================================
// �S�Ă̓����蔻��̉������
//==============================================================================================
void CCollision_Manager::ReleaseAllCollision()
{
	// �S�Ă̓����蔻��̍X�V����
	for (auto it = m_AllCollision.begin(); it != m_AllCollision.end();)
	{
		CCollision* pCollision = *it;

		it = m_AllCollision.erase(it);
		pCollision->GetParent()->CollisionDestroy();
		delete pCollision;
	}
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CCollision_Manager::UpdateAll()
{
	// �S�Ă̓����蔻��̍X�V����
	for (auto it = m_AllCollision.begin(); it != m_AllCollision.end();)
	{
		CCollision* pCollision = *it;

		// �e�Ǝq�̂ǂ�����A���S�t���O�������Ă��Ȃ��ꍇ
		if (!pCollision->GetDeath() && !pCollision->GetParent()->GetDeathFlag())
		{
			pCollision->Update();
			it++;
		}
		else
		{
			it = m_AllCollision.erase(it);
			delete pCollision;
		}
	}
}