//==============================================================================================
//
// マップのオブジェクトマネージャー　　　map_object_manager.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _MAP_OBJECT_MANAGER_H_
#define	_MAP_OBJECT_MANAGER_H_

#include "map_object.h"
#include <vector>

class CMap_Object_Manager
{
public:
	CMap_Object_Manager();
	~CMap_Object_Manager();

	void ReleaseAllMapObject() { for (auto pMapObj : m_AllMap_Object) pMapObj->Uninit(); }
	void DestroyMapObject(CMap_Object* map_object) { m_AllMap_Object.erase(std::find(m_AllMap_Object.begin(), m_AllMap_Object.end(), map_object)); }
	void SetMapObject(CMap_Object* map_object) { m_AllMap_Object.push_back(map_object); }

	std::vector<CMap_Object*> GetAllMapObject() { return m_AllMap_Object; }
private:

	std::vector<CMap_Object*> m_AllMap_Object;		// 全てのマップオブジェクトの情報
};

#endif // !_MAP_OBJECT_MANAGER_H_
