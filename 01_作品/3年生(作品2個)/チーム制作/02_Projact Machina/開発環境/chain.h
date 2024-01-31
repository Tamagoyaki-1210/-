//==============================================================================================
//
// çΩ	Å@Å@chain.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _CHAIN_H_
#define	_CHAIN_H_

#include"object3D.h"

class CChain : public CObject3D
{
public:
	CChain(const PRIORITY priority);
	~CChain();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CChain *Create(D3DXVECTOR3 pos, D3DXVECTOR3 vec, int index);

	void SetVec(D3DXVECTOR3 vec) { m_Vec = vec; }

private:
	D3DXVECTOR3 m_Vec;
	D3DXVECTOR3 m_Boss_Pos;
	int m_nIndex;
};

#endif // !_CHAIN_H_
