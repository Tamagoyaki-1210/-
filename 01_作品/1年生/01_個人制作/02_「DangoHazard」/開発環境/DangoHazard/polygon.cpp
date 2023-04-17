//============================================================================
//
//�|���S���`��̏���[polygon.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "polygon.h"		//�쐬����polygon.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePolygon = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//���_�o�b�t�@�ւ̃|�C���^
POLYGON g_aPolygon;									//�|���S���̏��

//============================================================================
//�|���S���̏���������
//============================================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

										//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\mofu.jpg",	//�e�N�X�`���̃t�@�C����
		&g_pTexturePolygon
	);

	//�ʒu�̏���������
	g_aPolygon.pos = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	g_aPolygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPolygon.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������
	g_aPolygon.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aPolygon.fAngle = atan2f(POLYGON_WIDTH, POLYGON_HEIGHT);
	g_aPolygon.fLength = sqrtf(POLYGON_WIDTH * POLYGON_WIDTH + POLYGON_HEIGHT * POLYGON_HEIGHT) / 2.0f;
	g_aPolygon.Worldmtx = D3DXMATRIX();


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	
	//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
	pVtx[0].pos.x = g_aPolygon.pos.x - sinf(g_aPolygon.fAngle + g_aPolygon.rot.x) * g_aPolygon.fLength;
	pVtx[0].pos.y = g_aPolygon.pos.y;
	pVtx[0].pos.z = g_aPolygon.pos.z + cosf(g_aPolygon.fAngle + g_aPolygon.rot.z) * g_aPolygon.fLength;

	pVtx[1].pos.x = g_aPolygon.pos.x + sinf(g_aPolygon.fAngle - g_aPolygon.rot.x) * g_aPolygon.fLength;
	pVtx[1].pos.y = g_aPolygon.pos.y;
	pVtx[1].pos.z = g_aPolygon.pos.z + cosf(g_aPolygon.fAngle - g_aPolygon.rot.z) * g_aPolygon.fLength;

	pVtx[2].pos.x = g_aPolygon.pos.x - sinf(g_aPolygon.fAngle - g_aPolygon.rot.x) * g_aPolygon.fLength;
	pVtx[2].pos.y = g_aPolygon.pos.y;
	pVtx[2].pos.z = g_aPolygon.pos.z - cosf(g_aPolygon.fAngle - g_aPolygon.rot.z) * g_aPolygon.fLength;

	pVtx[3].pos.x = g_aPolygon.pos.x + sinf(g_aPolygon.fAngle + g_aPolygon.rot.x) * g_aPolygon.fLength;
	pVtx[3].pos.y = g_aPolygon.pos.y;
	pVtx[3].pos.z = g_aPolygon.pos.z - cosf(g_aPolygon.fAngle + g_aPolygon.rot.z) * g_aPolygon.fLength;

	//�e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//============================================================================
//�|���S���̏I������
//============================================================================
void UninitPolygon(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;

	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//============================================================================
//�|���S���̍X�V����
//============================================================================
void UpdatePolygon(void)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�|���S���𓮂�������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (GetKeyboardPress(DIK_J) == true)
	{//���ړ�
		g_aPolygon.pos.x += 1.0f;
	}
	else if (GetKeyboardPress(DIK_L) == true)
	{//�E�ړ�
		g_aPolygon.pos.x -= 1.0f;
	}
	if (GetKeyboardPress(DIK_I) == true)
	{//��ړ�
		g_aPolygon.pos.y += 1.0f;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//���ړ�
		g_aPolygon.pos.y -= 1.0f;
	}
	if (GetKeyboardPress(DIK_U) == true)
	{//�O�ړ�
		g_aPolygon.pos.z += 1.0f;
	}
	else if (GetKeyboardPress(DIK_O) == true)
	{//���ړ�
		g_aPolygon.pos.z -= 1.0f;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//============================================================================
//�|���S���̕`�揈��
//============================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aPolygon.Worldmtx);
	//�����𔽉f(YaW : y,Pitch : x,Roll : z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPolygon.rot.y, g_aPolygon.rot.x, g_aPolygon.rot.z);
	D3DXMatrixMultiply(&g_aPolygon.Worldmtx, &g_aPolygon.Worldmtx, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_aPolygon.pos.x, g_aPolygon.pos.y, g_aPolygon.pos.z);
	D3DXMatrixMultiply(&g_aPolygon.Worldmtx, &g_aPolygon.Worldmtx, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aPolygon.Worldmtx);

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePolygon);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
