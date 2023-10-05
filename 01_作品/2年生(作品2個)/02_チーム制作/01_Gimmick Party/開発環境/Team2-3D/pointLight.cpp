//=============================================================================
//
// pointLight.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "pointLight.h"
#include "application.h"
#include "rendering.h"

//コンストラクタ
CPointLight::CPointLight()
{
	ZeroMemory(&m_pPointLight, sizeof(D3DLIGHT9));
}

//デストラクタ
CPointLight::~CPointLight()
{

}

//初期化処理
HRESULT CPointLight::Init(void)
{
	ZeroMemory(&m_pPointLight, sizeof(m_pPointLight));

	return S_OK;
}

//終了処理
void CPointLight::Uninit(void)
{

}

//更新処理
void CPointLight::Update(void)
{

}



CPointLight* CPointLight::Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 attenuation, float fRange)
{
	CPointLight* pLight = new CPointLight;

	if (FAILED(pLight->Init()))
	{
		return nullptr;
	}

	pLight->m_pPointLight.Type = D3DLIGHT_POINT;
	pLight->m_pPointLight.Position = pos;
	pLight->m_pPointLight.Diffuse = col;
	pLight->m_pPointLight.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pLight->m_pPointLight.Attenuation0 = attenuation.x;
	pLight->m_pPointLight.Attenuation1 = attenuation.y;
	pLight->m_pPointLight.Attenuation2 = attenuation.z;
	pLight->m_pPointLight.Range = fRange;

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	pDevice->SetLight(pLight->GetLightIdx(), &pLight->m_pPointLight);
	pDevice->LightEnable(pLight->GetLightIdx(), TRUE);

	return pLight;
}