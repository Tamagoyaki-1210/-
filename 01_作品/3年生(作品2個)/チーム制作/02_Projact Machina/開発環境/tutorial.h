//==============================================================================================
//
// チュートリアル　　　tutorial.h
// tutida ryousei
//
//==============================================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include"mode.h"

class CPlayerManager;
class CEnemyManager;
class CDropManager;
class CCollision_Manager;
class CMeshField;
class CPause;
class CMap_Object_Manager;

class CTutorial : public CMode
{
public:
	CTutorial();
	~CTutorial();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	static CTutorial *Create();

	static CPlayerManager* GetPlayerManager() { return m_pPlayerManager; }
	static CEnemyManager* GetEnemyManager() { return m_pEnemyManager; }
	static CDropManager* GetDropManager() { return m_pDropManager; }
	static CCollision_Manager* GetCollision_Manager() { return m_pCollision_Manager; }
	static CMeshField *GetMeshField() { return pMeshField; }
	static CPause *GetPause() { return m_pPause; }
	static CMap_Object_Manager *GetMap_Object_Manager() { return m_pMap_Object_Manager; }

private:
	static CMeshField *pMeshField;			// メッシュフィールド
	static CPause *m_pPause;
	static CPlayerManager* m_pPlayerManager;
	static CEnemyManager* m_pEnemyManager;
	static CDropManager* m_pDropManager;
	static CCollision_Manager* m_pCollision_Manager;
	static CMap_Object_Manager *m_pMap_Object_Manager;
};

#endif // !_TUTORIAL_H_
