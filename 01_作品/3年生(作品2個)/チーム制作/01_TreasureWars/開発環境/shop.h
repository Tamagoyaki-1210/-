//==============================================================================================
//
// shop.h
//
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#ifndef _SHOP_H_
#define	_SHOP_H_

#include"objectX.h"
#include"object3D.h"

class CShop : public CObjectX
{
public:
	CShop();
	~CShop() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetSaleUx(const bool draw) { m_pSaleUX->SetDrawFlag(draw); }	// ”„‹p’†‚ÌUX‚Ì•\¦İ’è

	static CShop *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

private:
	CObject3D* m_pSaleUX;	// ”„‹p’†‚ÌUX
};

#endif // !_SHOP_H_