//==============================================================================================
//
// 鎖マネージャー	　　chain_manager.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _CHAIN_MANAGER_H_
#define _CHAIN_MANAGER_H_

#include"object.h"
#include <vector>
#include <map>

class CChain;
class CObjectX;

class CChain_Manager : public CObject
{
public:
	static const int CHAIN_SIZE_X = 700;
	static const int CHAIN_SIZE_Y = 150;

	CChain_Manager();
	~CChain_Manager();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CChain_Manager *Create(D3DXVECTOR3 pos, D3DXVECTOR3 vec, float Distance, float rot, int breakcount);

private:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Vec;
	int m_nRestraint_Count;
	int m_nRestraint_Break;
	float m_fRot;
	float m_fDistance;
	float m_nNumChain;
	float m_fChain_Size;
	std::vector<CChain*>m_pChain[2];
	CObjectX *m_pAnchor;
};

#endif // !_CHAIN_MANAGER_H_
