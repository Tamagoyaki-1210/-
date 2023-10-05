//============================================================================
//
//���C�g�̏���[light.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "light.h"		//�쐬����camera.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LIGHT g_aLight;
D3DLIGHT9 g_light[MAX_LIGHT];	//���C�g���

//============================================================================
//���C�g�̏���������
//============================================================================
void InitLight(void)
{
	int nCntLight;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;		//���C�g�̕����x�N�g��
	//���C�g���N���A����
	ZeroMemory(&g_light[0], sizeof(g_light));

	//���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	//���K������(�傫��1�̃x�N�g���ɂ���)
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_light[0].Direction = vecDir;

	g_light[1].Direction = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
	g_light[2].Direction = D3DXVECTOR3(0.0f, 0.8f, 0.0f);

	for (nCntLight = 0;nCntLight < MAX_LIGHT; nCntLight++)
	{
		//���C�g�̎�ނ�ݒ�(���s����)
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;		
		//���C�g�̊g�U����ݒ�(���C�g�̐F)
		g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//============================================================================
//���C�g�̏I������
//============================================================================
void UninitLight(void)
{

}

//============================================================================
//���C�g�̍X�V����
//============================================================================
void UpdateLight(void)
{

}