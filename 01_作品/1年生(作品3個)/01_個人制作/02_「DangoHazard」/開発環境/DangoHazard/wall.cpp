//============================================================================
//
//�Ǖ`��̏���[wall.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "wall.h"		//�쐬����wall.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "polygon.h"	//�쐬����polygon.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//���_�o�b�t�@�ւ̃|�C���^
WALL g_aWall[MAX_WALL];									

//============================================================================
//�ǂ̏���������
//============================================================================
void InitWall(void)
{
	int nCntWall;
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

										//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\block000.jpg",	//�e�N�X�`���̃t�@�C����
		&g_pTextureWall
	);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//�ʒu�̏���������
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������
		g_aWall[nCntWall].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].bUse = false;
		g_aWall[nCntWall].fAngle = atan2f(WALL_WIDTH, WALL_HEIGHT);
		g_aWall[nCntWall].fLength = sqrtf(WALL_WIDTH * WALL_WIDTH + WALL_HEIGHT * WALL_HEIGHT) / 2.0f;
		g_aWall[nCntWall].Worldmtx = D3DXMATRIX();
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4  * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = -sinf(g_aWall[nCntWall].fAngle + g_aWall[nCntWall].rot.x) * g_aWall[nCntWall].fLength;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = cosf(g_aWall[nCntWall].fAngle + g_aWall[nCntWall].rot.z) * g_aWall[nCntWall].fLength;

		pVtx[1].pos.x = sinf(g_aWall[nCntWall].fAngle - g_aWall[nCntWall].rot.x) * g_aWall[nCntWall].fLength;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = cosf(g_aWall[nCntWall].fAngle - g_aWall[nCntWall].rot.z) * g_aWall[nCntWall].fLength;

		pVtx[2].pos.x = -sinf(g_aWall[nCntWall].fAngle - g_aWall[nCntWall].rot.x) * g_aWall[nCntWall].fLength;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = -cosf(g_aWall[nCntWall].fAngle - g_aWall[nCntWall].rot.z) * g_aWall[nCntWall].fLength;

		pVtx[3].pos.x = sinf(g_aWall[nCntWall].fAngle + g_aWall[nCntWall].rot.x) * g_aWall[nCntWall].fLength;
		pVtx[3].pos.y = 0.0f;
		pVtx[3].pos.z = -cosf(g_aWall[nCntWall].fAngle + g_aWall[nCntWall].rot.z) * g_aWall[nCntWall].fLength;

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

		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();

	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT / 2, -WALL_WIDTH / 2), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT / 2, WALL_WIDTH / 2), D3DXVECTOR3(D3DX_PI / 2, D3DX_PI, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(WALL_WIDTH / 2, WALL_HEIGHT / 2, 0.0f), D3DXVECTOR3(D3DX_PI / 2, -D3DX_PI / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));
	SetWall(D3DXVECTOR3(-WALL_WIDTH / 2, WALL_HEIGHT / 2, 0.0f), D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//============================================================================
//�ǂ̏I������
//============================================================================
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;

	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//============================================================================
//�ǂ̍X�V����
//============================================================================
void UpdateWall(void)
{

}

//============================================================================
//�ǂ̕`�揈��
//============================================================================
void DrawWall(void)
{
	int nCntWall;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWall[nCntWall].Worldmtx);
			//�����𔽉f(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].Worldmtx, &g_aWall[nCntWall].Worldmtx, &mtxRot);
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].Worldmtx, &g_aWall[nCntWall].Worldmtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].Worldmtx);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
	}
}

//============================================================================
//�ǂ̐ݒ菈��
//============================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	int nCntWall;

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;

			//���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aWall[nCntWall].bUse = true;
		}
		pVtx += 4;
		nCntWall++;

		//�ǂ̗����̏���
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = -rot;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(col, col, col, 0.4f);
			pVtx[1].col = D3DXCOLOR(col, col, col, 0.4f);
			pVtx[2].col = D3DXCOLOR(col, col, col, 0.4f);
			pVtx[3].col = D3DXCOLOR(col, col, col, 0.4f);

			g_aWall[nCntWall].bUse = true;
			break;
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}
