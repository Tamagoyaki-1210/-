//============================================================================
//
//�A�C�e���̏���[item.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "camera.h"		//�쐬����camera.h���C���N���[�h����
#include "shadow.h"		//�쐬����shadow.h���C���N���[�h����
#include "effect.h"		//�쐬����effect.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "model.h"		//�쐬����model.h���C���N���[�h����
#include "meshfield.h"	//�쐬����meshfield.h���C���N���[�h����
#include "score.h"		//�쐬����score.h���C���N���[�h����
#include <math.h>		//���������_�̌v�Z�ɕK�v

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
ITEM g_aItem[MAX_ITEM];			//�G�L�����\����		
X_ITEM g_aX_Item[IMAGE_ITEM];	//x�t�@�C���\����

									//============================================================================
									//�A�C�e���̏���������
									//============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("Data\\Model\\Corn.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Item[ITEM_TYPE_CORN].pBuffMat,
		NULL,
		&g_aX_Item[ITEM_TYPE_CORN].nNumMat,
		&g_aX_Item[ITEM_TYPE_CORN].pMesh
	);

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntImgItem = 0; nCntImgItem < IMAGE_ITEM; nCntImgItem++)
	{//�p�[�c�̏���
		D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
		pMat = (D3DXMATERIAL*)g_aX_Item[nCntImgItem].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aX_Item[nCntImgItem].nNumMat; nCntMat++)
		{
			//�e�N�X�`���̓ǂݏo��
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\Corn.jpg",	//�e�N�X�`���̃t�@�C����
				&g_aX_Item[nCntImgItem].g_pTexture[ITEM_TYPE_CORN]
			);
		}

		g_aX_Item[nCntImgItem].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aX_Item[nCntImgItem].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���_���̎擾
		nNumVtx = g_aX_Item[nCntImgItem].pMesh->GetNumVertices();
		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_aX_Item[nCntImgItem].pMesh->GetFVF());
		//���_�o�b�t�@�̃��b�N
		g_aX_Item[nCntImgItem].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑�\
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ă��f���̍ő�l���擾
			if (vtx.x > g_aX_Item[nCntImgItem].vtxMax.x)
			{//x���W�̍ő�l
				g_aX_Item[nCntImgItem].vtxMax.x = floorf(vtx.x);
			}
			if (vtx.y > g_aX_Item[nCntImgItem].vtxMax.y)
			{//y���W�̍ő�l
				g_aX_Item[nCntImgItem].vtxMax.y = floorf(vtx.y);
			}
			if (vtx.z > g_aX_Item[nCntImgItem].vtxMax.z)
			{//z���W�̍ő�l
				g_aX_Item[nCntImgItem].vtxMax.z = floorf(vtx.z);
			}

			//���_���W���r���ă��f���̍ŏ��l���擾
			if (vtx.x < g_aX_Item[nCntImgItem].vtxMin.x)
			{//x���W�̍ŏ��l
				g_aX_Item[nCntImgItem].vtxMin.x = floorf(vtx.x);
			}
			if (vtx.y < g_aX_Item[nCntImgItem].vtxMin.y)
			{//y���W�̍ŏ��l
				g_aX_Item[nCntImgItem].vtxMin.y = floorf(vtx.y);
			}
			if (vtx.z < g_aX_Item[nCntImgItem].vtxMin.z)
			{//z���W�̍ŏ��l
				g_aX_Item[nCntImgItem].vtxMin.z = floorf(vtx.z);
			}

			//���_���W�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}
		//���_�o�b�t�@�̃A�����b�N
		g_aX_Item[nCntImgItem].pMesh->UnlockVertexBuffer();
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//�ʒu�̏���������
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������
		g_aItem[nCntItem].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fAngle = atan2f(ITEM_WIDTH, ITEM_HEIGHT);
		g_aItem[nCntItem].fLength = sqrtf(ITEM_WIDTH * ITEM_WIDTH + ITEM_HEIGHT * ITEM_HEIGHT) / 2.0f;
		g_aItem[nCntItem].bUse = false;
		g_aItem[nCntItem].Worldmtx = D3DXMATRIX();
		g_aItem[nCntItem].type = ITEM_TYPE_CORN;
		g_aItem[nCntItem].aParts[ITEM_TYPE_CORN].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_aItem[nCntItem].aParts[ITEM_TYPE_CORN].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//============================================================================
//�A�C�e���̏���������
//============================================================================
void UninitItem(void)
{
	for (int nCntImgItem = 0; nCntImgItem < IMAGE_ITEM; nCntImgItem++)
	{
		//�A�C�e���̔j��
		if (g_aX_Item[nCntImgItem].pMesh != NULL)
		{
			g_aX_Item[nCntImgItem].pMesh->Release();
			g_aX_Item[nCntImgItem].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aX_Item[nCntImgItem].pBuffMat != NULL)
		{
			g_aX_Item[nCntImgItem].pBuffMat->Release();
			g_aX_Item[nCntImgItem].pBuffMat = NULL;
		}

		// x���f���̎g�p�e�N�X�`����S�ĉ������(���������[�N�C��_2023/7/12)
		for (int nMatTex = 0; nMatTex < ITEM_TYPE_MAX; nMatTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aX_Item[nCntImgItem].g_pTexture[nMatTex] != NULL)
			{
				g_aX_Item[nCntImgItem].g_pTexture[nMatTex]->Release();
				g_aX_Item[nCntImgItem].g_pTexture[nMatTex] = NULL;
			}
		}
	}
}

//============================================================================
//�A�C�e���̍X�V����
//============================================================================
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�p�x�̐��K������
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aItem[nCntItem].rot.y > D3DX_PI)
			{//���݂̊p�x��(PI)���傫���ꍇ
				g_aItem[nCntItem].rot.y -= D3DX_PI * 2;
			}
			else if (g_aItem[nCntItem].rot.y < -D3DX_PI)
			{//���݂̊p�x��(-PI)��菬�����ꍇ
				g_aItem[nCntItem].rot.y += D3DX_PI * 2;
			}

			if (g_aItem[nCntItem].Destrot.y - g_aItem[nCntItem].rot.y > D3DX_PI)
			{//���݂̖ړI�l��(PI)���傫���ꍇ
				g_aItem[nCntItem].Destrot.y -= 2 * D3DX_PI;
			}
			else if (g_aItem[nCntItem].Destrot.y - g_aItem[nCntItem].rot.y < -D3DX_PI)
			{//���݂̖ړI�n��(-PI)��菬�����ꍇ
				g_aItem[nCntItem].Destrot.y += 2 * D3DX_PI;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�������畜�A���鏈��
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aItem[nCntItem].pos.y <= -100)
			{
				g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//���̑��̏���
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�A�C�e������]�����鏈��
			g_aItem[nCntItem].rot.y += 0.015f;

			//�A�C�e���̑O��ʒu�𔽉f
			g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;

			//�A�C�e���̈ʒu�X�V
			g_aItem[nCntItem].pos += g_aItem[nCntItem].move * 0.1f;

			//�d�͂��X�V(����������)
			g_aItem[nCntItem].move.y -= 4.0f;

			//�ړ��ʂ��X�V(����������)
			g_aItem[nCntItem].move -= g_aItem[nCntItem].move * 0.1f;

			//�A�C�e���̃T�C�Y�̔��a�����߂鏈��
			D3DXVECTOR3 r_size = (g_aX_Item[g_aItem[nCntItem].type].vtxMax - g_aX_Item[g_aItem[nCntItem].type].vtxMin) / 2;

			//���f�������蔻��̏���
			ModelCollision(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, &g_aItem[nCntItem].move, &r_size);

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//���̔��菈��
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//���b�V���t�B�[���h�����蔻��̏���
			FieldCollision(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, &g_aItem[nCntItem].move);
		}
	}
}

//============================================================================
//�A�C�e���̕`�揈��
//============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aItem[nCntItem].Worldmtx);

			//���݂̃}�e���A�����擾����
			pDevice->GetMaterial(&matDef);

			//�����𔽉f(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].Worldmtx, &g_aItem[nCntItem].Worldmtx, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].Worldmtx, &g_aItem[nCntItem].Worldmtx, &mtxTrans);

			for (int nCntImgItem = 0; nCntImgItem < IMAGE_ITEM; nCntImgItem++)
			{
				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx);

				//�����𔽉f(YaW : y,Pitch : x,Roll : z)
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].aParts[nCntImgItem].rot.y, g_aItem[nCntItem].aParts[nCntImgItem].rot.x, g_aItem[nCntItem].aParts[nCntImgItem].rot.z);
				D3DXMatrixMultiply(&g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &mtxRot);

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].aParts[nCntImgItem].pos.x, g_aItem[nCntItem].aParts[nCntImgItem].pos.y, g_aItem[nCntItem].aParts[nCntImgItem].pos.z);
				D3DXMatrixMultiply(&g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &mtxTrans);

				//�e���f���̃}�g���b�N�X�Ƃ̊|���Z
				D3DXMatrixMultiply(&g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &g_aItem[nCntItem].Worldmtx);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx);

				//���݂̃}�e���A���̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aX_Item[g_aItem[nCntItem].type].pBuffMat->GetBufferPointer();

				//�e�N�X�`���̓ǂݏo��
				for (int nCntMat = 0; nCntMat < (int)g_aX_Item[g_aItem[nCntItem].type].nNumMat; nCntMat++)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aX_Item[g_aItem[nCntItem].type].g_pTexture[g_aItem[nCntItem].type]);

					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					//���f���p�[�c�̕`��
					g_aX_Item[g_aItem[nCntItem].type].pMesh->DrawSubset(nCntMat);
				}
			}
			//�e�N�X�`���̏�����(�o�O�C��_2023/7/12)
			pDevice->SetTexture(0, NULL);

			//�ێ����Ă��}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//============================================================================
//�A�C�e���̓����蔻�菈��
//============================================================================
bool ItemCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size)
{
	bool bItem = false;
	PLAYER *pPlayer = GetPlayer();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//�A�C�e���̈ʒu�����߂鏈��(�A�C�e���̃T�C�Y + �A�C�e���̈ʒu)
			float ItemL = g_aX_Item[g_aItem[nCntItem].type].vtxMin.x + g_aItem[nCntItem].pos.x;		//�A�C�e���̍���
			float ItemR = g_aX_Item[g_aItem[nCntItem].type].vtxMax.x + g_aItem[nCntItem].pos.x;		//�A�C�e���̉E��

			float ItemFront = g_aX_Item[g_aItem[nCntItem].type].vtxMin.z + g_aItem[nCntItem].pos.z;	//�A�C�e���̎�O��
			float ItemBack = g_aX_Item[g_aItem[nCntItem].type].vtxMax.z + g_aItem[nCntItem].pos.z;	//�A�C�e���̉���

			float ItemDown = g_aX_Item[g_aItem[nCntItem].type].vtxMin.y + g_aItem[nCntItem].pos.y;	//�A�C�e���̉���
			float ItemTop = g_aX_Item[g_aItem[nCntItem].type].vtxMax.y + g_aItem[nCntItem].pos.y;	//�A�C�e���̓���

																									//�G�L�����̓����蔻��
			if (pos->x + r_size->x > ItemL		&& pos->x - r_size->x < ItemR
				&&	pos->z + r_size->z > ItemFront	&& pos->z - r_size->z < ItemBack
				&&	pos->y + r_size->y > ItemDown	&& pos->y - r_size->y < ItemTop)
			{
				bItem = true;
				g_aItem[nCntItem].bUse = false;	//�g�p������U�ɂ���
				AddScore(100);
			}
		}
	}
	return bItem;
}

//============================================================================
//�A�C�e���̐ݒ菈��
//============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ITEM_TYPE type)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].rot = rot;
			g_aItem[nCntItem].type = type;
			g_aItem[nCntItem].bUse = true;
			break;
		}
	}
}

//============================================================================
//�A�C�e���̎擾����
//============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}
