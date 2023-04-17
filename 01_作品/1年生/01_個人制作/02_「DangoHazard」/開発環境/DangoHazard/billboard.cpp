//============================================================================
//
//�r���{�[�h�̏���[billboard.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "billboard.h"	//�쐬����billboard.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "shadow.h"		//�쐬����shadow.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBillBoard[IMAGE_BILLBOARD] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillBoard = NULL;	//���_�o�b�t�@�ւ̃|�C���^
BILLBOARD g_aBillBoard[MAX_BILLBOARD];
float g_fAngleBullet = atan2f(BILLBOARD_BULLET_WIDTH, BILLBOARD_BULLET_HEIGHT);
float g_fLengthBullet = sqrtf(BILLBOARD_BULLET_WIDTH * BILLBOARD_BULLET_WIDTH + BILLBOARD_BULLET_HEIGHT * BILLBOARD_BULLET_HEIGHT) / 2.0f;
float g_fAngleTree = atan2f(BILLBOARD_TREE_WIDTH, BILLBOARD_TREE_HEIGHT);
float g_fLengthTree = sqrtf(BILLBOARD_TREE_WIDTH * BILLBOARD_TREE_WIDTH + BILLBOARD_TREE_HEIGHT * BILLBOARD_TREE_HEIGHT) / 2.0f;
bool g_ZBillBoardDebug = false;						//Z�o�b�t�@�̃f�o�b�O

//============================================================================
//�r���{�[�h�̏���������
//============================================================================
void InitBillBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

										//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\bullet000.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureBillBoard[BILLBOARD_TYPE_BULLET]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\tree000.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureBillBoard[BILLBOARD_TYPE_TREE]
	);

	for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	{
		//�ʒu�̏���������
		g_aBillBoard[nCntBillBoard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillBoard[nCntBillBoard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillBoard[nCntBillBoard].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillBoard[nCntBillBoard].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aBillBoard[nCntBillBoard].IndxShadow = SetShadow(g_aBillBoard[nCntBillBoard].pos, g_aBillBoard[nCntBillBoard].rot);
		g_aBillBoard[nCntBillBoard].fAngle = 0.0f;
		g_aBillBoard[nCntBillBoard].fLength = 0.0f;
		g_aBillBoard[nCntBillBoard].Worldmtx = D3DXMATRIX();
		g_aBillBoard[nCntBillBoard].bUse = false;
		g_aBillBoard[nCntBillBoard].type = BILLBOARD_TYPE_BULLET;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillBoard,
		NULL);


	SetBillBoard(D3DXVECTOR3(20.0f, 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARD_TYPE_BULLET, g_fAngleBullet, g_fLengthBullet);
	SetBillBoard(D3DXVECTOR3(-48.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARD_TYPE_TREE, g_fAngleTree, g_fLengthTree);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillBoard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = -sinf(g_aBillBoard[nCntBillBoard].fAngle + g_aBillBoard[nCntBillBoard].rot.x) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[0].pos.y = +cosf(g_aBillBoard[nCntBillBoard].fAngle + g_aBillBoard[nCntBillBoard].rot.z) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.x) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[1].pos.y = +cosf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.z) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = -sinf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.x) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[2].pos.y = -cosf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.z) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(g_aBillBoard[nCntBillBoard].fAngle + g_aBillBoard[nCntBillBoard].rot.x) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[3].pos.y = -cosf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.z) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[3].pos.z = 0.0f;

		//�e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffBillBoard->Unlock();
}

//============================================================================
//�r���{�[�h�̏I������
//============================================================================
void UninitBillBoard(void)
{
	for (int nCntBillBoard = 0; nCntBillBoard < IMAGE_BILLBOARD; nCntBillBoard++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBillBoard[nCntBillBoard] != NULL)
		{
			g_pTextureBillBoard[nCntBillBoard]->Release();
			g_pTextureBillBoard[nCntBillBoard] = NULL;

		}
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffBillBoard != NULL)
		{
			g_pVtxBuffBillBoard->Release();
			g_pVtxBuffBillBoard = NULL;
		}
	}
}

//============================================================================
//�r���{�[�h�̍X�V����
//============================================================================
void UpdateBillBoard(void)
{
	//for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	//{
	//	if (g_aBillBoard[nCntBillBoard].bUse == true)
	//	{
	//		SetpositionShadow(g_aBillBoard[nCntBillBoard].IndxShadow, D3DXVECTOR3(g_aBillBoard[nCntBillBoard].pos.x, 0.0f, g_aBillBoard[nCntBillBoard].pos.z),
	//			SHADOW_WIDTH + (g_aBillBoard[nCntBillBoard].pos.y / 2), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1 - (g_aBillBoard[nCntBillBoard].pos.y / 100)));
	//	}
	//}

	if (GetKeyboardTrigger(DIK_F7) == true)
	{//Z�o�b�t�@�e�X�g
		g_ZBillBoardDebug = !g_ZBillBoardDebug;
	}
}

//============================================================================
//�r���{�[�h�̕`�揈��
//============================================================================
void DrawBillBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	{
		if (g_aBillBoard[nCntBillBoard].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBillBoard[nCntBillBoard].Worldmtx);

			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�J�����̋t�s���ݒ�
			g_aBillBoard[nCntBillBoard].Worldmtx._11 = mtxView._11;
			g_aBillBoard[nCntBillBoard].Worldmtx._12 = mtxView._21;
			g_aBillBoard[nCntBillBoard].Worldmtx._13 = mtxView._31;

			if (g_aBillBoard[nCntBillBoard].type == BILLBOARD_TYPE_BULLET)
			{
				g_aBillBoard[nCntBillBoard].Worldmtx._21 = mtxView._12;
				g_aBillBoard[nCntBillBoard].Worldmtx._22 = mtxView._22;
				g_aBillBoard[nCntBillBoard].Worldmtx._23 = mtxView._32;
			}

			g_aBillBoard[nCntBillBoard].Worldmtx._31 = mtxView._13;
			g_aBillBoard[nCntBillBoard].Worldmtx._32 = mtxView._23;
			g_aBillBoard[nCntBillBoard].Worldmtx._33 = mtxView._33;

			////�����𔽉f(YaW : y,Pitch : x,Roll : z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBillBoard[nCntBillBoard].rot.y, g_aBillBoard[nCntBillBoard].rot.x, g_aBillBoard[nCntBillBoard].rot.z);
			//D3DXMatrixMultiply(&g_aBillBoard[nCntBillBoard].Worldmtx, &g_aBillBoard[nCntBillBoard].Worldmtx, &mtxRot);
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aBillBoard[nCntBillBoard].pos.x, g_aBillBoard[nCntBillBoard].pos.y, g_aBillBoard[nCntBillBoard].pos.z);
			D3DXMatrixMultiply(&g_aBillBoard[nCntBillBoard].Worldmtx, &g_aBillBoard[nCntBillBoard].Worldmtx, &mtxTrans);

			//���C�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//Z�o�b�t�@�̒l����O��Z�o�b�t�@�̒l��菬�����ꍇ
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			if (g_ZBillBoardDebug == false)
			{
				//Z�o�b�t�@��`�悷��
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
			else
			{
				//Z�o�b�t�@��`�悷��
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}

			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillBoard[nCntBillBoard].Worldmtx);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBillBoard, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBillBoard[g_aBillBoard[nCntBillBoard].type]);

			//�|���S����`�悷��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillBoard * 4, 2);

			//Z�o�b�t�@��`�悷��
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//Z�o�b�t�@�̒l����O��Z�o�b�t�@�̒l��菬�����ꍇ
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}
	//�A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================================
//�r���{�[�h�̐ݒ菈��
//============================================================================
void SetBillBoard(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BILLBOARD_TYPE type, float fAngle, float fLength)
{
	for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	{
		if (g_aBillBoard[nCntBillBoard].bUse == false)
		{
			g_aBillBoard[nCntBillBoard].pos = pos;
			g_aBillBoard[nCntBillBoard].rot = rot;
			g_aBillBoard[nCntBillBoard].type = type;
			g_aBillBoard[nCntBillBoard].fAngle = fAngle;
			g_aBillBoard[nCntBillBoard].fLength = fLength;
			g_aBillBoard[nCntBillBoard].bUse = true;
			break;
		}
	}
}
