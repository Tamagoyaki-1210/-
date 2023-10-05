//=============================================================================
//
// pointLight.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "light.h"

class CPointLight : public CLight
{
public:

	CPointLight();							//コンストラクタ
	~CPointLight() override;				//デストラクタ

	HRESULT Init(void) override;			//初期化処理
	void Uninit(void) override;				//終了処理
	void Update(void) override;				//更新処理

	static CPointLight* Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 attenuation, float fRange);			//the x component of attenuation is the attenuation 0 of D3DLIGHT9; the y component is attenuation 1 and the z component is attenuation 2

private:

	D3DLIGHT9 m_pPointLight;				//ライト
};

#endif // !_DIRECTIONAL_LIGHT_H_