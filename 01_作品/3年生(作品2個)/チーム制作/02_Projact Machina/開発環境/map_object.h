//==============================================================================================
//
// マップのオブジェクト　　　map_object.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _MAP_OBJECT_H_
#define	_MAP_OBJECT_H_

#include"move_object.h"

class CObjectX;

class CMap_Object : public CMove_Object
{
public:
	CMap_Object(const PRIORITY priority);
	~CMap_Object();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Hit(CMove_Object* pHit) override;

	void Collision_Hit();

	static CMap_Object *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, const PRIORITY priority = PRIORITY_MODEL);
	
private:
};

#endif // !_MAP_OBJECT_H_
