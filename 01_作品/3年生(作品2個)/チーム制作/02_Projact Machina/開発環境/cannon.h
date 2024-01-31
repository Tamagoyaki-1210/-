//==============================================================================================
//
// ñCë‰	Å@Å@cannon.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _CANNON_H_
#define	_CANNON_H_

#include"move_object.h"
#include"chain_manager.h"

class CCannon : public CMove_Object
{
public:
	CCannon(CObject::PRIORITY priority);
	~CCannon();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Hit(CMove_Object* pHit) override;

	static CCannon *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, int index, const PRIORITY priority = PRIORITY_MODEL);

	static void SetRestrain(bool restrain) { m_bRestrain = restrain; }
	static bool GetRestrain() { return m_bRestrain; }

private:
	static bool m_bRestrain;
	int m_nIndex;
	int m_nChain_Count;
	float m_fRot;
	bool m_bTarget;
	CChain_Manager *m_pChain_Manager;
};

#endif // !_CANNON_H_
