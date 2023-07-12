//============================================================================
//
//�U������̏���[attack.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "attack.h"		//�쐬����attack.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "camera.h"		//�쐬����camera.h���C���N���[�h����
#include "shadow.h"		//�쐬����shadow.h���C���N���[�h����
#include "effect.h"		//�쐬����effect.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "model.h"		//�쐬����model.h���C���N���[�h����
#include "meshfield.h"	//�쐬����meshfield.h���C���N���[�h����
#include <math.h>		//���������_�̌v�Z�ɕK�v

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
ATTACK g_aAttack[MAX_ATTACK];			//�G�L�����\����		
X_ATTACK g_aX_Attack[IMAGE_ATTACK];	//x�t�@�C���\����

								//============================================================================
								//�U������̏���������
								//============================================================================
void InitAttack(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

										//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("Data\\Model\\Player_collision.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Attack[ATTACK_TYPE_PLAYER].pBuffMat,
		NULL,
		&g_aX_Attack[ATTACK_TYPE_PLAYER].nNumMat,
		&g_aX_Attack[ATTACK_TYPE_PLAYER].pMesh
	);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("Data\\Model\\Player_collision.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Attack[ATTACK_TYPE_ENEMY].pBuffMat,
		NULL,
		&g_aX_Attack[ATTACK_TYPE_ENEMY].nNumMat,
		&g_aX_Attack[ATTACK_TYPE_ENEMY].pMesh
	);

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntImgAttack = 0; nCntImgAttack < IMAGE_ATTACK; nCntImgAttack++)
	{//�p�[�c�̏���
		D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
		pMat = (D3DXMATERIAL*)g_aX_Attack[nCntImgAttack].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aX_Attack[nCntImgAttack].nNumMat; nCntMat++)
		{
			//�e�N�X�`���̓ǂݏo��
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//�e�N�X�`���̃t�@�C����
				&g_aX_Attack[nCntImgAttack].g_pTexture[ATTACK_TYPE_PLAYER]
			);
			//�e�N�X�`���̓ǂݏo��
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//�e�N�X�`���̃t�@�C����
				&g_aX_Attack[nCntImgAttack].g_pTexture[ATTACK_TYPE_ENEMY]
			);
		}

		g_aX_Attack[nCntImgAttack].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aX_Attack[nCntImgAttack].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���_���̎擾
		nNumVtx = g_aX_Attack[nCntImgAttack].pMesh->GetNumVertices();
		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_aX_Attack[nCntImgAttack].pMesh->GetFVF());
		//���_�o�b�t�@�̃��b�N
		g_aX_Attack[nCntImgAttack].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑�\
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ă��f���̍ő�l���擾
			if (vtx.x > g_aX_Attack[nCntImgAttack].vtxMax.x)
			{//x���W�̍ő�l
				g_aX_Attack[nCntImgAttack].vtxMax.x = floorf(vtx.x);
			}
			if (vtx.y > g_aX_Attack[nCntImgAttack].vtxMax.y)
			{//y���W�̍ő�l
				g_aX_Attack[nCntImgAttack].vtxMax.y = floorf(vtx.y);
			}
			if (vtx.z > g_aX_Attack[nCntImgAttack].vtxMax.z)
			{//z���W�̍ő�l
				g_aX_Attack[nCntImgAttack].vtxMax.z = floorf(vtx.z);
			}

			//���_���W���r���ă��f���̍ŏ��l���擾
			if (vtx.x < g_aX_Attack[nCntImgAttack].vtxMin.x)
			{//x���W�̍ŏ��l
				g_aX_Attack[nCntImgAttack].vtxMin.x = floorf(vtx.x);
			}
			if (vtx.y < g_aX_Attack[nCntImgAttack].vtxMin.y)
			{//y���W�̍ŏ��l
				g_aX_Attack[nCntImgAttack].vtxMin.y = floorf(vtx.y);
			}
			if (vtx.z < g_aX_Attack[nCntImgAttack].vtxMin.z)
			{//z���W�̍ŏ��l
				g_aX_Attack[nCntImgAttack].vtxMin.z = floorf(vtx.z);
			}

			//���_���W�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}
		//���_�o�b�t�@�̃A�����b�N
		g_aX_Attack[nCntImgAttack].pMesh->UnlockVertexBuffer();
	}

	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		//�ʒu�̏���������
		g_aAttack[nCntAttack].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������
		g_aAttack[nCntAttack].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].nLife = 0;
		g_aAttack[nCntAttack].bUse = false;
		g_aAttack[nCntAttack].Worldmtx = D3DXMATRIX();
		g_aAttack[nCntAttack].type = ATTACK_TYPE_PLAYER;
		g_aAttack[nCntAttack].aParts[ATTACK_TYPE_PLAYER].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_aAttack[nCntAttack].aParts[ATTACK_TYPE_PLAYER].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].aParts[ATTACK_TYPE_ENEMY].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_aAttack[nCntAttack].aParts[ATTACK_TYPE_ENEMY].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//============================================================================
//�U������̏���������
//============================================================================
void UninitAttack(void)
{
	for (int nCntImgAttack = 0; nCntImgAttack < IMAGE_ATTACK; nCntImgAttack++)
	{
		//�U������̔j��
		if (g_aX_Attack[nCntImgAttack].pMesh != NULL)
		{
			g_aX_Attack[nCntImgAttack].pMesh->Release();
			g_aX_Attack[nCntImgAttack].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aX_Attack[nCntImgAttack].pBuffMat != NULL)
		{
			g_aX_Attack[nCntImgAttack].pBuffMat->Release();
			g_aX_Attack[nCntImgAttack].pBuffMat = NULL;
		}

		// x���f���̎g�p�e�N�X�`����S�ĉ������(���������[�N�C��_2023/7/12)
		for (int nMatTex = 0; nMatTex < ATTACK_TYPE_MAX; nMatTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aX_Attack[nCntImgAttack].g_pTexture[nMatTex] != NULL)
			{
				g_aX_Attack[nCntImgAttack].g_pTexture[nMatTex]->Release();
				g_aX_Attack[nCntImgAttack].g_pTexture[nMatTex] = NULL;
			}
		}
	}
}

//============================================================================
//�U������̍X�V����
//============================================================================
void UpdateAttack(void)
{
	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�p�x�̐��K������
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aAttack[nCntAttack].rot.y > D3DX_PI)
			{//���݂̊p�x��(PI)���傫���ꍇ
				g_aAttack[nCntAttack].rot.y -= D3DX_PI * 2;
			}
			else if (g_aAttack[nCntAttack].rot.y < -D3DX_PI)
			{//���݂̊p�x��(-PI)��菬�����ꍇ
				g_aAttack[nCntAttack].rot.y += D3DX_PI * 2;
			}

			if (g_aAttack[nCntAttack].Destrot.y - g_aAttack[nCntAttack].rot.y > D3DX_PI)
			{//���݂̖ړI�l��(PI)���傫���ꍇ
				g_aAttack[nCntAttack].Destrot.y -= 2 * D3DX_PI;
			}
			else if (g_aAttack[nCntAttack].Destrot.y - g_aAttack[nCntAttack].rot.y < -D3DX_PI)
			{//���݂̖ړI�n��(-PI)��菬�����ꍇ
				g_aAttack[nCntAttack].Destrot.y += 2 * D3DX_PI;
			}

			if (g_aAttack[nCntAttack].nLife <= 0)
			{
				g_aAttack[nCntAttack].bUse = false;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//���̑��̏���
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�U������̎��������炷����
			g_aAttack[nCntAttack].nLife--;

			//�U������̑O��ʒu�𔽉f
			g_aAttack[nCntAttack].posOld = g_aAttack[nCntAttack].pos;

			//�U������̈ʒu�X�V
			g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move;
		}
	}
}

//============================================================================
//�U������̕`�揈��
//============================================================================
void DrawAttack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aAttack[nCntAttack].Worldmtx);

			//���݂̃}�e���A�����擾����
			pDevice->GetMaterial(&matDef);

			//�����𔽉f(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAttack[nCntAttack].rot.y, g_aAttack[nCntAttack].rot.x, g_aAttack[nCntAttack].rot.z);
			D3DXMatrixMultiply(&g_aAttack[nCntAttack].Worldmtx, &g_aAttack[nCntAttack].Worldmtx, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aAttack[nCntAttack].pos.x, g_aAttack[nCntAttack].pos.y, g_aAttack[nCntAttack].pos.z);
			D3DXMatrixMultiply(&g_aAttack[nCntAttack].Worldmtx, &g_aAttack[nCntAttack].Worldmtx, &mtxTrans);

			for (int nCntImgAttack = 0; nCntImgAttack < IMAGE_ATTACK; nCntImgAttack++)
			{
				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx);

				//�����𔽉f(YaW : y,Pitch : x,Roll : z)
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAttack[nCntAttack].aParts[nCntImgAttack].rot.y, g_aAttack[nCntAttack].aParts[nCntImgAttack].rot.x, g_aAttack[nCntAttack].aParts[nCntImgAttack].rot.z);
				D3DXMatrixMultiply(&g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &mtxRot);

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aAttack[nCntAttack].aParts[nCntImgAttack].pos.x, g_aAttack[nCntAttack].aParts[nCntImgAttack].pos.y, g_aAttack[nCntAttack].aParts[nCntImgAttack].pos.z);
				D3DXMatrixMultiply(&g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &mtxTrans);

				//�e���f���̃}�g���b�N�X�Ƃ̊|���Z
				D3DXMatrixMultiply(&g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &g_aAttack[nCntAttack].Worldmtx);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx);

				//���݂̃}�e���A���̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aX_Attack[g_aAttack[nCntAttack].type].pBuffMat->GetBufferPointer();

				//�e�N�X�`���̓ǂݏo��
				for (int nCntMat = 0; nCntMat < (int)g_aX_Attack[g_aAttack[nCntAttack].type].nNumMat; nCntMat++)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aX_Attack[g_aAttack[nCntAttack].type].g_pTexture[g_aAttack[nCntAttack].type]);

					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					//���f���p�[�c�̕`��
					g_aX_Attack[g_aAttack[nCntAttack].type].pMesh->DrawSubset(nCntMat);
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
//�U������̓����蔻�菈��
//============================================================================
bool AttackCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size)
{
	PLAYER *pPlayer = GetPlayer();

	bool bDamege = false;

	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//�U������̈ʒu�����߂鏈��(�A�C�e���̃T�C�Y + �A�C�e���̈ʒu)
			float AttackL = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMin.x + g_aAttack[nCntAttack].pos.x;		//�A�C�e���̍���
			float AttackR = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMax.x + g_aAttack[nCntAttack].pos.x;		//�A�C�e���̉E��

			float AttackFront = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMin.z + g_aAttack[nCntAttack].pos.z;	//�A�C�e���̎�O��
			float AttackBack = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMax.z + g_aAttack[nCntAttack].pos.z;	//�A�C�e���̉���

			float AttackDown = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMin.y + g_aAttack[nCntAttack].pos.y;	//�A�C�e���̉���
			float AttackTop = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMax.y + g_aAttack[nCntAttack].pos.y;	//�A�C�e���̓���

																									//�A�C�e���̓����蔻��
																									//���f���̓����蔻��
			if (pos->x + r_size->x > AttackL		&& pos->x - r_size->x < AttackR
				&&	pos->z + r_size->z > AttackFront	&& pos->z - r_size->z < AttackBack
				&&	pos->y + r_size->y > AttackDown	&& pos->y - r_size->y < AttackTop)
			{
				bDamege = true;
				if (pPlayer->nPower < 3)
				{
					g_aAttack[nCntAttack].bUse = false;	//�g�p������U�ɂ���
				}
				else
				{
					g_aAttack[nCntAttack].bUse = true;	//�g�p������U�ɂ���
				}
				//x�������蔻��
				if (pos->x + r_size->x > AttackL && posOld->x + r_size->x <= AttackL)
				{//�����̓����蔻��
					pos->x = AttackL - r_size->x;
					move->x = 0.0f;
				}
				else if (pos->x - r_size->x < AttackR && posOld->x - r_size->x >= AttackR)
				{//�E���̓����蔻��
					pos->x = AttackR + r_size->x;
					move->x = 0.0f;
				}
				//z�������蔻��
				if (pos->z + r_size->z > AttackFront && posOld->z + r_size->z <= AttackFront)
				{//��O���̓����蔻��
					pos->z = AttackFront - r_size->z;
					move->z = 0.0f;
				}
				else if (pos->z - r_size->z < AttackBack && posOld->z - r_size->z >= AttackBack)
				{//�����̓����蔻��
					pos->z = AttackBack + r_size->z;
					move->z = 0.0f;
				}
				//y�������蔻��
				if (pos->y + r_size->y > AttackDown && posOld->y + r_size->y <= AttackDown)
				{//�����̓����蔻��
					pos->y = AttackDown - r_size->y;
					move->y = 0.0f;
				}
				else if (pos->y - r_size->y < AttackTop && posOld->y - r_size->y >= AttackTop)
				{//�㑤�̓����蔻��
					pos->y = AttackTop + r_size->y;
					move->y = 0.0f;
				}
			}
		}
	}
	return bDamege;
}

//============================================================================
//�U������̐ݒ菈��
//============================================================================
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int life, ATTACK_TYPE type)
{
	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == false)
		{
			g_aAttack[nCntAttack].pos = pos;
			g_aAttack[nCntAttack].move = move;
			g_aAttack[nCntAttack].rot = rot;
			g_aAttack[nCntAttack].nLife = life;
			g_aAttack[nCntAttack].type = type;
			g_aAttack[nCntAttack].bUse = true;
			break;
		}
	}
}

//============================================================================
//�U������̎擾����
//============================================================================
ATTACK *GetAttack(void)
{
	return &g_aAttack[0];
}
