//============================================================================
//
//���f���̏���[model.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "model.h"		//�쐬����model.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "camera.h"		//�쐬����camera.h���C���N���[�h����
#include "shadow.h"		//�쐬����shadow.h���C���N���[�h����
#include "effect.h"		//�쐬����effect.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include <math.h>		//���������_�̌v�Z�ɕK�v

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
MODEL g_aModel[MAX_MODEL];			//���f���\����		
X_MODEL g_aX_Model[IMAGE_MODEL];	//x�t�@�C���\����

//============================================================================
//���f���̏���������
//============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

	for (int nCntModel = 0;nCntModel < MAX_MODEL; nCntModel++)
	{
		//�ʒu�̏���������
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_aModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������
		g_aModel[nCntModel].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].fAngle = atan2f(MODEL_WIDTH, MODEL_HEIGHT);
		g_aModel[nCntModel].fLength = sqrtf(MODEL_WIDTH * MODEL_WIDTH + MODEL_HEIGHT * MODEL_HEIGHT) / 2.0f;
		g_aModel[nCntModel].bUse = false;
		g_aModel[nCntModel].Worldmtx = D3DXMATRIX();
		g_aModel[nCntModel].type = MODEL_TYPE_ROCK;
	}

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("Data\\Model\\rock000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Model[MODEL_TYPE_ROCK].pBuffMat,
		NULL,
		&g_aX_Model[MODEL_TYPE_ROCK].nNumMat,
		&g_aX_Model[MODEL_TYPE_ROCK].pMesh
	);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("Data\\Model\\stone000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Model[MODEL_TYPE_STONE].pBuffMat,
		NULL,
		&g_aX_Model[MODEL_TYPE_STONE].nNumMat,
		&g_aX_Model[MODEL_TYPE_STONE].pMesh
	);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("Data\\Model\\tree000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Model[MODEL_TYPE_TREE].pBuffMat,
		NULL,
		&g_aX_Model[MODEL_TYPE_TREE].nNumMat,
		&g_aX_Model[MODEL_TYPE_TREE].pMesh
	);

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntImgModel = 0; nCntImgModel < IMAGE_MODEL; nCntImgModel++)
	{
		D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
		pMat = (D3DXMATERIAL*)g_aX_Model[nCntImgModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aX_Model[nCntImgModel].nNumMat; nCntMat++)
		{
			//�e�N�X�`���̓ǂݏo��
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//�e�N�X�`���̃t�@�C����
				&g_aX_Model[nCntImgModel].g_pTexture[MODEL_TYPE_ROCK]
			);

			//�e�N�X�`���̓ǂݏo��
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//�e�N�X�`���̃t�@�C����
				&g_aX_Model[nCntImgModel].g_pTexture[MODEL_TYPE_STONE]
			);

			//�e�N�X�`���̓ǂݏo��
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//�e�N�X�`���̃t�@�C����
				&g_aX_Model[nCntImgModel].g_pTexture[MODEL_TYPE_TREE]
			);
		}

		g_aX_Model[nCntImgModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aX_Model[nCntImgModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
		//���_���̎擾
		nNumVtx = g_aX_Model[nCntImgModel].pMesh->GetNumVertices();
		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_aX_Model[nCntImgModel].pMesh->GetFVF());
		//���_�o�b�t�@�̃��b�N
		g_aX_Model[nCntImgModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑�\
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ă��f���̍ő�l���擾
			if (vtx.x > g_aX_Model[nCntImgModel].vtxMax.x)
			{//x���W�̍ő�l
				g_aX_Model[nCntImgModel].vtxMax.x = floorf(vtx.x);
			}
			if (vtx.y > g_aX_Model[nCntImgModel].vtxMax.y)
			{//y���W�̍ő�l
				g_aX_Model[nCntImgModel].vtxMax.y = floorf(vtx.y);
			}
			if (vtx.z > g_aX_Model[nCntImgModel].vtxMax.z)
			{//z���W�̍ő�l
				g_aX_Model[nCntImgModel].vtxMax.z = floorf(vtx.z);
			}

			//���_���W���r���ă��f���̍ŏ��l���擾
			if (vtx.x < g_aX_Model[nCntImgModel].vtxMin.x)
			{//x���W�̍ŏ��l
				g_aX_Model[nCntImgModel].vtxMin.x = floorf(vtx.x);
			}
			if (vtx.y < g_aX_Model[nCntImgModel].vtxMin.y)
			{//y���W�̍ŏ��l
				g_aX_Model[nCntImgModel].vtxMin.y = floorf(vtx.y);
			}
			if (vtx.z < g_aX_Model[nCntImgModel].vtxMin.z)
			{//z���W�̍ŏ��l
				g_aX_Model[nCntImgModel].vtxMin.z = floorf(vtx.z);
			}

			//���_���W�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}
		//���_�o�b�t�@�̃A�����b�N
		g_aX_Model[nCntImgModel].pMesh->UnlockVertexBuffer();
	}

	//�ʒu�A�p�x�A���
	SetModel(D3DXVECTOR3(100.0f, -10.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_TYPE_ROCK);
	SetModel(D3DXVECTOR3(-100.0f, -10.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_TYPE_STONE);
	SetModel(D3DXVECTOR3(100.0f, -10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_TYPE_TREE);
}

//============================================================================
//���f���̏���������
//============================================================================
void UninitModel(void)
{
	for (int nCntImgModel = 0; nCntImgModel < IMAGE_MODEL; nCntImgModel++)
	{
		//���b�V���̔j��
		if (g_aX_Model[nCntImgModel].pMesh != NULL)
		{
			g_aX_Model[nCntImgModel].pMesh->Release();
			g_aX_Model[nCntImgModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aX_Model[nCntImgModel].pBuffMat != NULL)
		{
			g_aX_Model[nCntImgModel].pBuffMat->Release();
			g_aX_Model[nCntImgModel].pBuffMat = NULL;
		}

		// x���f���̎g�p�e�N�X�`����S�ĉ������(���������[�N�C��_2023/7/12)
		for (int nMatTex = 0; nMatTex < MODEL_TYPE_MAX; nMatTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aX_Model[nCntImgModel].g_pTexture[nMatTex] != NULL)
			{
				g_aX_Model[nCntImgModel].g_pTexture[nMatTex]->Release();
				g_aX_Model[nCntImgModel].g_pTexture[nMatTex] = NULL;
			}
		}
	}
}

//============================================================================
//���f���̍X�V����
//============================================================================
void UpdateModel(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{

		}
	}
}

//============================================================================
//���f���̕`�揈��
//============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
	
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aModel[nCntModel].Worldmtx);
			//���݂̃}�e���A�����擾����
			pDevice->GetMaterial(&matDef);

			//�����𔽉f(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].Worldmtx, &g_aModel[nCntModel].Worldmtx, &mtxRot);
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].Worldmtx, &g_aModel[nCntModel].Worldmtx, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].Worldmtx);

			//���݂̃}�e���A���̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aX_Model[g_aModel[nCntModel].type].pBuffMat->GetBufferPointer();

			//�e�N�X�`���̓ǂݏo��
			for (int nCntMat = 0; nCntMat < (int)g_aX_Model[g_aModel[nCntModel].type].nNumMat; nCntMat++)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aX_Model[g_aModel[nCntModel].type].g_pTexture[g_aModel[nCntModel].type]);
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				//���f���p�[�c�̕`��
				g_aX_Model[g_aModel[nCntModel].type].pMesh->DrawSubset(nCntMat);
			}
			//�e�N�X�`���̏�����(�o�O�C��_2023/7/12)
			pDevice->SetTexture(0, NULL);

			//�ێ����Ă��}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//============================================================================
//���f���̓����蔻�菈��
//============================================================================
bool ModelCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size)
{
	bool bIsGround = false;
	PLAYER *pPlayer = GetPlayer();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			//���f���̈ʒu�����߂鏈��(���f���̃T�C�Y + ���f���̈ʒu)
			float ModelL = g_aX_Model[g_aModel[nCntModel].type].vtxMin.x + g_aModel[nCntModel].pos.x;		//���f���̍���
			float ModelR = g_aX_Model[g_aModel[nCntModel].type].vtxMax.x + g_aModel[nCntModel].pos.x;		//���f���̉E��

			float ModelFront = g_aX_Model[g_aModel[nCntModel].type].vtxMin.z + g_aModel[nCntModel].pos.z;	//���f���̎�O��
			float ModelBack = g_aX_Model[g_aModel[nCntModel].type].vtxMax.z + g_aModel[nCntModel].pos.z;	//���f���̉���

			float ModelDown = g_aX_Model[g_aModel[nCntModel].type].vtxMin.y + g_aModel[nCntModel].pos.y;	//���f���̉���
			float ModelTop = g_aX_Model[g_aModel[nCntModel].type].vtxMax.y + g_aModel[nCntModel].pos.y;		//���f���̓���

			//���f���̓����蔻��
			if (	pos->x + r_size->x > ModelL		&& pos->x - r_size->x < ModelR
				&&	pos->z + r_size->z > ModelFront	&& pos->z - r_size->z < ModelBack
				&&	pos->y + r_size->y > ModelDown	&& pos->y - r_size->y < ModelTop)
			{
				//x�������蔻��
				if (pos->x + r_size->x > ModelL && posOld->x + r_size->x <= ModelL)
				{//�����̓����蔻��
					pos->x = ModelL - r_size->x;
					move->x = 0.0f;
				}
				else if (pos->x - r_size->x < ModelR && posOld->x - r_size->x >= ModelR)
				{//�E���̓����蔻��
					pos->x = ModelR + r_size->x;
					move->x = 0.0f;
				}
				//z�������蔻��
				if (pos->z + r_size->z > ModelFront && posOld->z + r_size->z <= ModelFront)
				{//��O���̓����蔻��
					pos->z = ModelFront - r_size->z;
					move->z = 0.0f;
				}
				else if (pos->z - r_size->z < ModelBack && posOld->z - r_size->z >= ModelBack)
				{//�����̓����蔻��
					pos->z = ModelBack + r_size->z;
					move->z = 0.0f;
				}
				//y�������蔻��
				if (pos->y + r_size->y > ModelDown && posOld->y + r_size->y <= ModelDown)
				{//�����̓����蔻��
					pos->y = ModelDown - r_size->y;
					move->y = 0.0f;
				}
				else if (pos->y - r_size->y < ModelTop && posOld->y - r_size->y >= ModelTop)
				{//�㑤�̓����蔻��
					pos->y = ModelTop + r_size->y;
					move->y = 0.0f;

					bIsGround = true;
				}
			}
		}
	}
	return bIsGround;
}

//============================================================================
//���f���̐ݒ菈��
//============================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL_TYPE type)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{
			g_aModel[nCntModel].pos = pos;
			g_aModel[nCntModel].rot = rot;
			g_aModel[nCntModel].type = type;
			g_aModel[nCntModel].bUse = true;
			break;
		}
	}
}

//============================================================================
//���f���̎擾����
//============================================================================
MODEL *GetModel(void)
{
		return &g_aModel[0];
}