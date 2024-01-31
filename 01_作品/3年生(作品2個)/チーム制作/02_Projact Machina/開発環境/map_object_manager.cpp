//==============================================================================================
//
// マップのオブジェクトマネージャー　　　map_object_manager.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"map_object_manager.h"

CMap_Object_Manager::CMap_Object_Manager()
{
}

CMap_Object_Manager::~CMap_Object_Manager()
{
	// 全てのマップオブジェクトの解放
	m_AllMap_Object.clear();
}