//==============================================
//
//ライトの処理[light.cpp]
//Author：tutida ryousei
//
//==============================================
#include"light.h"
#include"application.h"

const D3DXCOLOR CLight::Light_Diffuse_Color = { 1.0f, 1.0f, 1.0f, 1.0f };	// ライトの環境光の色
//==============================================
//コンストラクタ
//==============================================
CLight::CLight()
{

}

//==============================================
//デストラクタ
//==============================================
CLight::~CLight()
{

}

//==============================================
//ライトの初期化処理
//==============================================
void CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//ライトの方向ベクトル
	D3DXVECTOR3 vecDir[Max_Light];

	//ライトをクリアする
	ZeroMemory(&m_light[0], sizeof(D3DLIGHT9));

	//ライトの拡散光を設定
	for (int nCnt = 0; nCnt < Max_Light; nCnt++)
	{
		m_light[nCnt].Diffuse = Light_Diffuse_Color;
	}

	// ライトの角度
	vecDir[0] = D3DXVECTOR3(-10.0f, -8.0f, 0.0f);
	vecDir[1] = D3DXVECTOR3(10.0f, -8.0f, 0.0f);
	vecDir[2] = D3DXVECTOR3(0.0f, -8.0f, 10.0f);
	vecDir[3] = D3DXVECTOR3(0.0f, -8.0f, -10.0f);
	vecDir[4] = D3DXVECTOR3(-10.0f, -8.0f, 0.0f);
	vecDir[5] = D3DXVECTOR3(10.0f, -8.0f, 0.0f);
	vecDir[6] = D3DXVECTOR3(0.0f, -8.0f, 10.0f);
	vecDir[7] = D3DXVECTOR3(0.0f, -8.0f, -10.0f);

	for (int nCnt = 0; nCnt < Max_Light; nCnt++)
	{
		m_light[nCnt].Position = { 0.0f,100.0f,0.0f };

		//ライトの種類
		m_light[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		//正規化する(大きさ1のベクトルにする)
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_light[nCnt].Direction = vecDir[nCnt];

		//ライトを設定
		pDevice->SetLight(nCnt, &m_light[nCnt]);

		//ライトを有効にする
		pDevice->LightEnable(nCnt, TRUE);
	}
}

//==============================================
//ライトの終了処理
//==============================================
void CLight::Uninit(void)
{

}

//==============================================
//ライトの更新処理
//==============================================
void CLight::Update(void)
{

}