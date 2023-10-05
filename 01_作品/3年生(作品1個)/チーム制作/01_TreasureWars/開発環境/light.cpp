//==============================================
//
//���C�g�̏���[light.cpp]
//Author�Ftutida ryousei
//
//==============================================
#include"light.h"
#include"application.h"

const D3DXCOLOR CLight::Light_Diffuse_Color = { 1.0f, 1.0f, 1.0f, 1.0f };	// ���C�g�̊����̐F
//==============================================
//�R���X�g���N�^
//==============================================
CLight::CLight()
{

}

//==============================================
//�f�X�g���N�^
//==============================================
CLight::~CLight()
{

}

//==============================================
//���C�g�̏���������
//==============================================
void CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���C�g�̕����x�N�g��
	D3DXVECTOR3 vecDir[Max_Light];

	//���C�g���N���A����
	ZeroMemory(&m_light[0], sizeof(D3DLIGHT9));

	//���C�g�̊g�U����ݒ�
	for (int nCnt = 0; nCnt < Max_Light; nCnt++)
	{
		m_light[nCnt].Diffuse = Light_Diffuse_Color;
	}

	// ���C�g�̊p�x
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

		//���C�g�̎��
		m_light[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		//���K������(�傫��1�̃x�N�g���ɂ���)
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_light[nCnt].Direction = vecDir[nCnt];

		//���C�g��ݒ�
		pDevice->SetLight(nCnt, &m_light[nCnt]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCnt, TRUE);
	}
}

//==============================================
//���C�g�̏I������
//==============================================
void CLight::Uninit(void)
{

}

//==============================================
//���C�g�̍X�V����
//==============================================
void CLight::Update(void)
{

}