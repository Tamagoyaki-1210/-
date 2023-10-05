//=============================================================================
//
// directionalLight.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "directionalLight.h"
#include "application.h"
#include "rendering.h"

//=============================================================================
//
//							静的変数の初期化
//
//=============================================================================
D3DXVECTOR3 CDirectionalLight::m_PrincipalLightDir = Vec3Null;					//一番強いライト

//コンストラクタ
CDirectionalLight::CDirectionalLight()
{
	//メンバー変数をクリアする
	ZeroMemory(&m_pDirLight, sizeof(D3DLIGHT9));			//メモリをクリアする
}

//デストラクタ
CDirectionalLight::~CDirectionalLight()
{

}

//初期化処理
HRESULT CDirectionalLight::Init(void)
{
	//初期化処理
	ZeroMemory(&m_pDirLight, sizeof(m_pDirLight));

	return S_OK;
}

//終了処理
void CDirectionalLight::Uninit(void)
{

}

//更新処理
void CDirectionalLight::Update(void)
{

}

//一番強いライトの向きの設定
void CDirectionalLight::SetPrincipalLightDir(const D3DXVECTOR3 dir)
{
	m_PrincipalLightDir = dir;			//一番強いライトの向きの設定
}

//一番強いライトの向きの取得
const D3DXVECTOR3 CDirectionalLight::GetPrincipalLightDir(void)
{
	return m_PrincipalLightDir;
}


//=============================================================================
//
//								静的関数
//
//=============================================================================


//生成処理
CDirectionalLight* CDirectionalLight::Create(D3DXCOLOR col, D3DXVECTOR3 dir)
{
	CDirectionalLight* pLight = new CDirectionalLight;			//ライトを生成する

	if (FAILED(pLight->Init()))
	{//初期化処理
		return nullptr;
	}

	pLight->m_pDirLight.Type = D3DLIGHT_DIRECTIONAL;					//ライトの種類の設定
	pLight->m_pDirLight.Diffuse = col;									//ライトの色の設定
	pLight->m_pDirLight.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	

	//向きの設定
	D3DXVECTOR3 newDir;													
	D3DXVec3Normalize(&newDir, &dir);									
	pLight->m_pDirLight.Direction = dir;								

	if (m_PrincipalLightDir == Vec3Null)
	{
		m_PrincipalLightDir = dir;
	}

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();			//デバイスの取得

	pDevice->SetLight(pLight->GetLightIdx(), &pLight->m_pDirLight);					//ライトの設定
	pDevice->LightEnable(pLight->GetLightIdx(), TRUE);								//ライトを有効にする

	return pLight;					//生成したライトを返す
}