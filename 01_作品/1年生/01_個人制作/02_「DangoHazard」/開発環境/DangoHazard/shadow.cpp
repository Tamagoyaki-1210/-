//============================================================================
//
//�e�`��̏���[shadow.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "shadow.h"		//�쐬����shadow.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
SHADOW g_aShadow[MAX_SHADOW];									

//============================================================================
//�e�̏���������
//============================================================================
void InitShadow(void)
{
	int nCntShadow;
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

										//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\shadow000.jpg",	//�e�N�X�`���̃t�@�C����
		&g_pTextureShadow
	);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//�ʒu�̏���������
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].bUse = false;
		g_aShadow[nCntShadow].fAngle = atan2f(SHADOW_WIDTH, SHADOW_HEIGHT);
		g_aShadow[nCntShadow].fLength = sqrtf(SHADOW_WIDTH * SHADOW_WIDTH + SHADOW_HEIGHT * SHADOW_HEIGHT) / 2.0f;
		g_aShadow[nCntShadow].Worldmtx = D3DXMATRIX();
	}

		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffShadow,
			NULL);

		//���_���ւ̃|�C���^
		VERTEX_3D *pVtx = NULL;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
		{
			//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
			pVtx[0].pos.x = -sinf(g_aShadow[nCntShadow].fAngle + g_aShadow[nCntShadow].rot.x) * g_aShadow[nCntShadow].fLength;
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = cosf(g_aShadow[nCntShadow].fAngle + g_aShadow[nCntShadow].rot.z) * g_aShadow[nCntShadow].fLength;

			pVtx[1].pos.x = sinf(g_aShadow[nCntShadow].fAngle - g_aShadow[nCntShadow].rot.x) * g_aShadow[nCntShadow].fLength;
			pVtx[1].pos.y = 0.0f;
			pVtx[1].pos.z = cosf(g_aShadow[nCntShadow].fAngle - g_aShadow[nCntShadow].rot.z) * g_aShadow[nCntShadow].fLength;

			pVtx[2].pos.x = -sinf(g_aShadow[nCntShadow].fAngle - g_aShadow[nCntShadow].rot.x) * g_aShadow[nCntShadow].fLength;
			pVtx[2].pos.y = 0.0f;
			pVtx[2].pos.z = -cosf(g_aShadow[nCntShadow].fAngle - g_aShadow[nCntShadow].rot.z) * g_aShadow[nCntShadow].fLength;

			pVtx[3].pos.x = sinf(g_aShadow[nCntShadow].fAngle + g_aShadow[nCntShadow].rot.x) * g_aShadow[nCntShadow].fLength;
			pVtx[3].pos.y = 0.0f;
			pVtx[3].pos.z = -cosf(g_aShadow[nCntShadow].fAngle + g_aShadow[nCntShadow].rot.z) * g_aShadow[nCntShadow].fLength;

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
		g_pVtxBuffShadow->Unlock();
	
}

//============================================================================
//�e�̏I������
//============================================================================
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;

	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//============================================================================
//�e�̍X�V����
//============================================================================
void UpdateShadow(void)
{}

//============================================================================
//�e�̕`�揈��
//============================================================================
void DrawShadow(void)
{
	int nCntShadow;
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//g_ColorShadow = 1.0f - g_ColorShadow * g_ColorShadow.a;
	 
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureShadow);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].Worldmtx);
			//�����𔽉f(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].Worldmtx, &g_aShadow[nCntShadow].Worldmtx, &mtxRot);
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].Worldmtx, &g_aShadow[nCntShadow].Worldmtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].Worldmtx);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}
	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�X�`���̏�����(�o�O�C��_2023/7/12)
	pDevice->SetTexture(0, NULL);
}

//============================================================================
//�e�̐ݒ菈��
//============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntShadow;
	
	for(nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].bUse = true;
			break;
		}
	}
	return nCntShadow;	//�e�̔ԍ�(index)��Ԃ�
}
																																																											
//============================================================================
//�e�̈ʒu�X�V����
//============================================================================
void SetpositionShadow(int nIdxShadow, D3DXVECTOR3 pos , float fLength, D3DXCOLOR col)
{
	g_aShadow[nIdxShadow].pos = pos;		//�����Ŏw�肳�ꂽ�ԍ���pos��ݒ�

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	g_aShadow[nIdxShadow].fLength = fLength;

	if (fLength <= 0)
	{//�e�̑傫����0�ȉ��ɂȂ����ꍇ
		g_aShadow[nIdxShadow].fLength = 0;
	}

	pVtx += 4 * nIdxShadow;

	//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
	pVtx[0].pos.x = -sinf(g_aShadow[nIdxShadow].fAngle + g_aShadow[nIdxShadow].rot.x) * g_aShadow[nIdxShadow].fLength;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = cosf(g_aShadow[nIdxShadow].fAngle + g_aShadow[nIdxShadow].rot.z) * g_aShadow[nIdxShadow].fLength;

	pVtx[1].pos.x = sinf(g_aShadow[nIdxShadow].fAngle - g_aShadow[nIdxShadow].rot.x) * g_aShadow[nIdxShadow].fLength;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = cosf(g_aShadow[nIdxShadow].fAngle - g_aShadow[nIdxShadow].rot.z) * g_aShadow[nIdxShadow].fLength;

	pVtx[2].pos.x = -sinf(g_aShadow[nIdxShadow].fAngle - g_aShadow[nIdxShadow].rot.x) * g_aShadow[nIdxShadow].fLength;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = -cosf(g_aShadow[nIdxShadow].fAngle - g_aShadow[nIdxShadow].rot.z) * g_aShadow[nIdxShadow].fLength;

	pVtx[3].pos.x = sinf(g_aShadow[nIdxShadow].fAngle + g_aShadow[nIdxShadow].rot.x) * g_aShadow[nIdxShadow].fLength;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z = -cosf(g_aShadow[nIdxShadow].fAngle + g_aShadow[nIdxShadow].rot.z) * g_aShadow[nIdxShadow].fLength;

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}