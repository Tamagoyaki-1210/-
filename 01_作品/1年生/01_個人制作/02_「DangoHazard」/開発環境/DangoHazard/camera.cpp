//============================================================================
//
//�J�����̏���[camera.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "camera.h"		//�쐬����camera.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
CAMERA g_aCamera;									

//============================================================================
//�J�����̏���������
//============================================================================
void InitCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();				//�f�o�C�X�̎擾

	//�ʒu�̏���������
	g_aCamera.posV = D3DXVECTOR3(0.0f, 60.0f, -400.0f);
	g_aCamera.posR = D3DXVECTOR3(0.0f,20.0f,-300.0f);
	g_aCamera.posVDest = D3DXVECTOR3(0.0f, 60.0f, -100.0f);
	g_aCamera.posRDest = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	g_aCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_aCamera.rot = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	g_aCamera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aCamera.fDistance = sqrtf((g_aCamera.posV.z - g_aCamera.posR.z) * (g_aCamera.posV.z - g_aCamera.posR.z)
		+ (g_aCamera.posV.x - g_aCamera.posR.x) * (g_aCamera.posV.x - g_aCamera.posR.x)) / 2;		//���_���璍���_�̋���
	g_aCamera.bFollowUse = false;
}

//============================================================================
//�J�����̏I������
//============================================================================
void UninitCamera(void)
{

}

//============================================================================
//�J�����̍X�V����
//============================================================================
void UpdateCamera(void)
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�|���S���𓮂�������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//���_ & �����_�̓����ړ�
	if (GetKeyboardPress(DIK_LEFT) == true || GetKeyboardPress(DIK_J) == true)
	{//�J�������ړ�
		g_aCamera.posV.x -= sinf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posR.x -= sinf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posV.z -= cosf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posR.z -= cosf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true || GetKeyboardPress(DIK_L) == true)
	{//�J�����E�ړ�
		g_aCamera.posV.x += sinf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posR.x += sinf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posV.z += cosf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posR.z += cosf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
	}
	if (GetKeyboardPress(DIK_UP) == true || GetKeyboardPress(DIK_I) == true)
	{//�J�����O���ړ�
		g_aCamera.posV.x += sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.x += sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posV.z += cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.z += cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true || GetKeyboardPress(DIK_K) == true)
	{//�J��������ړ�
		g_aCamera.posV.x -= sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.x -= sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posV.z -= cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.z -= cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
	}
	if (GetKeyboardPress(DIK_U) == true)
	{//�J������ړ�
		g_aCamera.posV.y+= 1;
		g_aCamera.posR.y++;
		g_aCamera.posV.x -= sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.x -= sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posV.z -= cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.z -= cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
	}
	else if (GetKeyboardPress(DIK_O) == true)
	{//�J�������ړ�
		g_aCamera.posV.y-= 1;
		g_aCamera.posR.y--;
		g_aCamera.posV.x += sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.x += sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posV.z += cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.z += cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
	}
	//if (GetKeyboardPress(DIK_M) == true)
	//{//�J������ړ�
	//	g_aCamera.posV.y += 1.0f;
	//	//g_aCamera.posR.y += 1.0f;
	//}
	//else if (GetKeyboardPress(DIK_N) == true)
	//{//�J�������ړ�
	//	g_aCamera.posV.y -= 1.0f;
	//	//g_aCamera.posR.y -= 1.0f;
	//}
	//if (GetKeyboardPress(DIK_U) == true)
	//{//�J���������_��ړ�
	//	g_aCamera.posR.y++;
	//}
	//else if (GetKeyboardPress(DIK_I) == true)
	//{//�J���������_���ړ�
	//	g_aCamera.posR.y--;
	//}

	////�����_�̐���
	//if (GetKeyboardPress(DIK_Z) == true)
	//{//�����
	//	g_aCamera.rot.y -= 0.1f;
	//	g_aCamera.posR.x = g_aCamera.posV.x + sinf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//	g_aCamera.posR.z = g_aCamera.posV.z + cosf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_X) == true)
	//{//�E���
	//	g_aCamera.rot.y += 0.1f;
	//	g_aCamera.posR.x = g_aCamera.posV.x + sinf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//	g_aCamera.posR.z = g_aCamera.posV.z + cosf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//}

	////���_�̐���
	//if (GetKeyboardPress(DIK_C) == true)
	//{//�����
	//	g_aCamera.rot.y += 0.1f;
	//	g_aCamera.posV.x = g_aCamera.posR.x - sinf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//	g_aCamera.posV.z = g_aCamera.posR.z - cosf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_V) == true)
	//{//�E���
	//	g_aCamera.rot.y -= 0.1f;
	//	g_aCamera.posV.x = g_aCamera.posR.x - sinf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//	g_aCamera.posV.z = g_aCamera.posR.z - cosf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//}

	PLAYER *pPlayer = GetPlayer();

	////�J�����̒Ǐ]����
	//if (g_aCamera.bFollowUse == true)
	//{
	//	g_aCamera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.x) * pPlayer->fLength;
	//	g_aCamera.posRDest.z = pPlayer->pos.z + sinf(pPlayer->rot.z) * pPlayer->fLength;

	//	g_aCamera.posVDest.x = pPlayer->pos.x + sinf(g_aCamera.rot.x) * g_aCamera.fDistance;
	//	g_aCamera.posVDest.z = pPlayer->pos.z + sinf(g_aCamera.rot.z) * g_aCamera.fDistance;

	//	g_aCamera.posR.x += (g_aCamera.posRDest.x - g_aCamera.posR.x) * 0.1f;
	//	g_aCamera.posV.x += (g_aCamera.posVDest.x - g_aCamera.posV.x) * 0.1f;
	//	g_aCamera.posR.z += (g_aCamera.posRDest.z - g_aCamera.posR.z) * 0.1f;
	//	g_aCamera.posV.z += (g_aCamera.posVDest.z - g_aCamera.posV.z) * 0.1f;
	//	//g_aCamera.posR += (g_aCamera.posRDest - g_aCamera.posR) * 0.1f;
	//	//g_aCamera.posV += (g_aCamera.posVDest - g_aCamera.posV) * 0.1f;
	//}

	if (GetKeyboardTrigger(DIK_F3) == true)
	{//�J�����Ǐ]�g�p����
		g_aCamera.rot.y = 0.0f;
		g_aCamera.bFollowUse = !g_aCamera.bFollowUse;
	}

	if (g_aCamera.rot.y > D3DX_PI)
	{//�J���������̐��K������
		g_aCamera.rot.y = -D3DX_PI;
	}
	else if(g_aCamera.rot.y < -D3DX_PI)
	{
		g_aCamera.rot.y = D3DX_PI;
	}
}

//============================================================================
//�J�����̐ݒ菈��
//============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCamera.mtxView);
	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(
		&g_aCamera.mtxView,
		&g_aCamera.posV,
		&g_aCamera.posR,
		&g_aCamera.vecU
	);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_aCamera.mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCamera.mtxProjection);
	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(
		&g_aCamera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//�A�X�y�N�g��
		CAMERA_NEAR,CAMERA_FAR
	);
	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_aCamera.mtxProjection);

	//g_aCamera.bFollowUse = true;
}

//============================================================================
//�J�����̎擾����
//============================================================================
CAMERA *GetCamera(void)
{
	return &g_aCamera;
}