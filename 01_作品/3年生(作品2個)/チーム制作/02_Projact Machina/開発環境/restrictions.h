//==============================================================================================
//
// à⁄ìÆêßå¿Å@Å@Å@restrictions.h
// tutida ryousei
//
//==============================================================================================
#ifndef _RESTRICTIONS_H_
#define	_RESTRICTIONS_H_

#include"object.h"

class CObject3D;

class CRestrictions : public CObject
{
public:
	CRestrictions();
	~CRestrictions();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Collision();
	void Alpha();

	static CRestrictions *Create(const D3DXVECTOR3 center, const float scope, const D3DXVECTOR2 size, const D3DXCOLOR col, const CObject::PRIORITY priority = PRIORITY_MODEL);

private:
	D3DXVECTOR3 m_Center;
	float m_fScope;
	D3DXVECTOR2 m_Size;
	D3DXCOLOR m_Col;
	CObject3D *m_pDont_Enter[4][64];
	D3DXVECTOR3 m_pDont_EnterPos[4];
	float m_fAlpha;
	float m_fAlpha_Count;
	bool m_bHit;
};

#endif // !_RESTRICTIONS_H_
