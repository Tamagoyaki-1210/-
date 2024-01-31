//==============================================================================================
//
// コリジョンマネージャー　　　collision_manager.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"collision_manager.h"
#include"move_object.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CCollision_Manager::CCollision_Manager()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CCollision_Manager::~CCollision_Manager()
{
}

//==============================================================================================
// 全ての当たり判定の解放処理
//==============================================================================================
void CCollision_Manager::ReleaseAllCollision()
{
	// 全ての当たり判定の更新処理
	for (auto it = m_AllCollision.begin(); it != m_AllCollision.end();)
	{
		CCollision* pCollision = *it;

		it = m_AllCollision.erase(it);
		pCollision->GetParent()->CollisionDestroy();
		delete pCollision;
	}
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CCollision_Manager::UpdateAll()
{
	// 全ての当たり判定の更新処理
	for (auto it = m_AllCollision.begin(); it != m_AllCollision.end();)
	{
		CCollision* pCollision = *it;

		// 親と子のどちらも、死亡フラグが建っていない場合
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