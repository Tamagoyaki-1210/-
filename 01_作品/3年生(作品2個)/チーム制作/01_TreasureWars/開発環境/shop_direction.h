//==============================================================================================
//
// éøâÆÇÃï˚å¸Å@Å@Å@shop_direction.h
// tutida ryousei
//
//==============================================================================================
#ifndef _SHOP_DIRECTION_H_
#define	_SHOP_DIRECTION_H_

#include"object3D.h"

class CShop_Direction : public CObject3D
{
public:
	CShop_Direction();
	~CShop_Direction();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CShop_Direction *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);

	void SetPos_Direction(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot_Direction(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetShopVec(D3DXVECTOR3 vec) { m_ShopVec = vec; }
	void SetDaller(bool daller) { m_bDaller = daller; }

private:
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Rot;
	CObject3D *m_Daller;
	D3DXVECTOR3 m_ShopVec;
	bool m_bDaller;
};

#endif // !_SHOP_DIRECTION_H_
