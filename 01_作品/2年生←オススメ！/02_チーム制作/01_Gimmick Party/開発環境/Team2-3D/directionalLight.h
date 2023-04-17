//=============================================================================
//
// directionalLight.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _DIRECTIONAL_LIGHT_H_
#define _DIRECTIONAL_LIGHT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "light.h"

class CDirectionalLight : public CLight
{
public:

	CDirectionalLight();					//コンストラクタ
	~CDirectionalLight() override;			//デストラクタ

	HRESULT Init(void) override;			//初期化処理
	void Uninit(void) override;				//終了処理
	void Update(void) override;				//更新処理

	static void SetPrincipalLightDir(const D3DXVECTOR3 dir);		//一番強いライトの設定処理
	static const D3DXVECTOR3 GetPrincipalLightDir(void);			//一番強いライトの取得処理

	static CDirectionalLight* Create(D3DXCOLOR col, D3DXVECTOR3 dir);		//生成処理

private:

	D3DLIGHT9 m_pDirLight;					//ライト

	static D3DXVECTOR3 m_PrincipalLightDir;	//一番強いライトのアドレス
};

#endif // !_DIRECTIONAL_LIGHT_H_