//============================================================================
//
//�G�L�����̏���[enemy.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "enemy.h"		//�쐬����enemy.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "camera.h"		//�쐬����camera.h���C���N���[�h����
#include "shadow.h"		//�쐬����shadow.h���C���N���[�h����
#include "effect.h"		//�쐬����effect.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "model.h"		//�쐬����model.h���C���N���[�h����
#include "meshfield.h"	//�쐬����meshfield.h���C���N���[�h����
#include "attack.h"		//�쐬����attack.h���C���N���[�h����
#include "score.h"		//�쐬����score.h���C���N���[�h����
#include <math.h>		//���������_�̌v�Z�ɕK�v

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
ENEMY g_aEnemy[MAX_ENEMY];			//�G�L�����\����		
X_ENEMY g_aX_Enemy[IMAGE_ENEMY];	//x�t�@�C���\����

									//============================================================================
									//�G�L�����̏���������
									//============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("Data\\Model\\imodanngo.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Enemy[ENEMY_TYPE_POTATE].pBuffMat,
		NULL,
		&g_aX_Enemy[ENEMY_TYPE_POTATE].nNumMat,
		&g_aX_Enemy[ENEMY_TYPE_POTATE].pMesh
	);

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("Data\\Model\\kabotyadanngo.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Enemy[ENEMY_TYPE_PUNPKIN].pBuffMat,
		NULL,
		&g_aX_Enemy[ENEMY_TYPE_PUNPKIN].nNumMat,
		&g_aX_Enemy[ENEMY_TYPE_PUNPKIN].pMesh
	);

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntImgEnemy = 0; nCntImgEnemy < IMAGE_ENEMY; nCntImgEnemy++)
	{//�摜�̏���
		D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
		pMat = (D3DXMATERIAL*)g_aX_Enemy[nCntImgEnemy].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aX_Enemy[nCntImgEnemy].nNumMat; nCntMat++)
		{
			//�e�N�X�`���̓ǂݏo��
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//�e�N�X�`���̃t�@�C����
				&g_aX_Enemy[nCntImgEnemy].g_pTexture[ENEMY_TYPE_POTATE]
			);

			//�e�N�X�`���̓ǂݏo��
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//�e�N�X�`���̃t�@�C����
				&g_aX_Enemy[nCntImgEnemy].g_pTexture[ENEMY_TYPE_PUNPKIN]
			);
		}

		g_aX_Enemy[nCntImgEnemy].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aX_Enemy[nCntImgEnemy].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���_���̎擾
		nNumVtx = g_aX_Enemy[nCntImgEnemy].pMesh->GetNumVertices();
		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_aX_Enemy[nCntImgEnemy].pMesh->GetFVF());
		//���_�o�b�t�@�̃��b�N
		g_aX_Enemy[nCntImgEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑�\
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ă��f���̍ő�l���擾
			if (vtx.x > g_aX_Enemy[nCntImgEnemy].vtxMax.x)
			{//x���W�̍ő�l
				g_aX_Enemy[nCntImgEnemy].vtxMax.x = floorf(vtx.x);
			}
			if (vtx.y > g_aX_Enemy[nCntImgEnemy].vtxMax.y)
			{//y���W�̍ő�l
				g_aX_Enemy[nCntImgEnemy].vtxMax.y = floorf(vtx.y);
			}
			if (vtx.z > g_aX_Enemy[nCntImgEnemy].vtxMax.z)
			{//z���W�̍ő�l
				g_aX_Enemy[nCntImgEnemy].vtxMax.z = floorf(vtx.z);
			}

			//���_���W���r���ă��f���̍ŏ��l���擾
			if (vtx.x < g_aX_Enemy[nCntImgEnemy].vtxMin.x)
			{//x���W�̍ŏ��l
				g_aX_Enemy[nCntImgEnemy].vtxMin.x = floorf(vtx.x);
			}
			if (vtx.y < g_aX_Enemy[nCntImgEnemy].vtxMin.y)
			{//y���W�̍ŏ��l
				g_aX_Enemy[nCntImgEnemy].vtxMin.y = floorf(vtx.y);
			}
			if (vtx.z < g_aX_Enemy[nCntImgEnemy].vtxMin.z)
			{//z���W�̍ŏ��l
				g_aX_Enemy[nCntImgEnemy].vtxMin.z = floorf(vtx.z);
			}

			//���_���W�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}
		//���_�o�b�t�@�̃A�����b�N
		g_aX_Enemy[nCntImgEnemy].pMesh->UnlockVertexBuffer();
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�ʒu�̏���������
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	
		g_aEnemy[nCntEnemy].vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].vec1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].fAngle = atan2f(ENEMY_WIDTH, ENEMY_HEIGHT);
		g_aEnemy[nCntEnemy].fLength = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) / 2.0f;
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].fFlame = 0;
		g_aEnemy[nCntEnemy].fVecCnt = 0;
		g_aEnemy[nCntEnemy].bLoop = false;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bDamege = false;
		g_aEnemy[nCntEnemy].bBattle = false;
		g_aEnemy[nCntEnemy].Worldmtx = D3DXMATRIX();
		g_aEnemy[nCntEnemy].type = ENEMY_TYPE_POTATE;
		g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_NUTRAL;
		g_aEnemy[nCntEnemy].aParts[ENEMY_TYPE_POTATE].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		g_aEnemy[nCntEnemy].aParts[ENEMY_TYPE_POTATE].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].aParts[ENEMY_TYPE_PUNPKIN].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		g_aEnemy[nCntEnemy].aParts[ENEMY_TYPE_PUNPKIN].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//�ʒu�A�p�x�A���
	SetEnemy(D3DXVECTOR3(20.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMY_TYPE_PUNPKIN);
}

//============================================================================
//�G�L�����̏���������
//============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		for (int nCntImgEnemy = 0; nCntImgEnemy < IMAGE_ENEMY; nCntImgEnemy++)
		{
			//�G�L�����̔j��
			if (g_aX_Enemy[nCntImgEnemy].pMesh != NULL)
			{
				g_aX_Enemy[nCntImgEnemy].pMesh->Release();
				g_aX_Enemy[nCntImgEnemy].pMesh = NULL;
			}

			//�}�e���A���̔j��
			if (g_aX_Enemy[nCntImgEnemy].pBuffMat != NULL)
			{
				g_aX_Enemy[nCntImgEnemy].pBuffMat->Release();
				g_aX_Enemy[nCntImgEnemy].pBuffMat = NULL;
			}
		}
	}
}

//============================================================================
//�G�L�����̍X�V����
//============================================================================
void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			CAMERA *pCamera = GetCamera();
			ATTACK *pAttack = GetAttack();
			PLAYER *pPlayer = GetPlayer();

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�x�N�g��(�G�Ǐ]����)
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			g_aEnemy[nCntEnemy].vec = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

			//���K������(�傫��1�̃x�N�g���ɂ���)
			D3DXVec3Normalize(&g_aEnemy[nCntEnemy].vec1, &g_aEnemy[nCntEnemy].vec);
			g_aEnemy[nCntEnemy].vec.y = 0;
			g_aEnemy[nCntEnemy].vec1.y = 0;

			//�G�L�����̃T�C�Y�����߂鏈��
			D3DXVECTOR3 size = (g_aX_Enemy[0].vtxMax - g_aX_Enemy[0].vtxMin);
			//�G�L�����̃T�C�Y�̔��a�����߂鏈��
			D3DXVECTOR3 r_size = size / 2.0f;
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�f�o�b�O�p�G�ړ�����
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
			//if (GetKeyboardPress(DIK_W) == true)
			//{//��O�ړ�
			//	g_aEnemy[0].rot.y += (g_aEnemy[0].Destrot.y - g_aEnemy[0].rot.y) / 10;
			//	
			//	if (g_aEnemy[0].bLoop == false)
			//	{
			//		g_aEnemy[0].motion = ENEMY_MOTION_WORK;
			//		g_aEnemy[0].bLoop = true;
			//	}

			//	if (GetKeyboardPress(DIK_A) == true)
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y - D3DX_PI / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y - D3DX_PI / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y + D3DX_PI * 3 / 4;
			//	}
			//	else if (GetKeyboardPress(DIK_D) == true)
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y + D3DX_PI / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y + D3DX_PI / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y - D3DX_PI * 3 / 4;
			//	}
			//	else
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y) * ENEMY_SPEAD;

			//		if (g_aEnemy[0].rot.y >= pCamera->rot.y)
			//		{
			//			g_aEnemy[0].Destrot.y = pCamera->rot.y + D3DX_PI;
			//		}
			//		else if (g_aEnemy[0].rot.y < pCamera->rot.y)
			//		{
			//			g_aEnemy[0].Destrot.y = pCamera->rot.y - D3DX_PI;
			//		}
			//	}
			//}
			//else if (GetKeyboardPress(DIK_S) == true)
			//{//���ړ�
			//	g_aEnemy[0].rot.y += (g_aEnemy[0].Destrot.y - g_aEnemy[0].rot.y) / 10;

			//	if (g_aEnemy[0].bLoop == false)
			//	{
			//		g_aEnemy[0].motion = ENEMY_MOTION_WORK;
			//		g_aEnemy[0].bLoop = true;
			//	}

			//	if (GetKeyboardPress(DIK_A) == true)
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y - D3DX_PI * 3 / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y - D3DX_PI * 3 / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y + D3DX_PI / 4;
			//	}
			//	else if (GetKeyboardPress(DIK_D) == true)
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y + D3DX_PI * 3 / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y + D3DX_PI * 3 / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y - D3DX_PI / 4;
			//	}
			//	else
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y - D3DX_PI) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y - D3DX_PI) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y;
			//	}
			//}
			//else if (GetKeyboardPress(DIK_A) == true)
			//{//���ړ�
			//	g_aEnemy[0].rot.y += (g_aEnemy[0].Destrot.y - g_aEnemy[0].rot.y) / 10;
			//	if (g_aEnemy[0].bLoop == false)
			//	{
			//		g_aEnemy[0].motion = ENEMY_MOTION_WORK;
			//		g_aEnemy[0].bLoop = true;
			//	}

			//	g_aEnemy[0].move.x += sinf(pCamera->rot.y - D3DX_PI / 2.0f) * ENEMY_SPEAD;
			//	g_aEnemy[0].move.z += cosf(pCamera->rot.y - D3DX_PI / 2.0f) * ENEMY_SPEAD;
			//	g_aEnemy[0].Destrot.y = pCamera->rot.y + D3DX_PI / 2;
			//}
			//else if (GetKeyboardPress(DIK_D) == true)
			//{//�E�ړ�
			//	g_aEnemy[0].rot.y += (g_aEnemy[0].Destrot.y - g_aEnemy[0].rot.y) / 10;
			//	if (g_aEnemy[0].bLoop == false)
			//	{
			//		g_aEnemy[0].motion = ENEMY_MOTION_WORK;
			//		g_aEnemy[0].bLoop = true;
			//	}

			//	g_aEnemy[0].move.x += sinf(pCamera->rot.y + D3DX_PI / 2.0f) * ENEMY_SPEAD;
			//	g_aEnemy[0].move.z += cosf(pCamera->rot.y + D3DX_PI / 2.0f) * ENEMY_SPEAD;
			//	g_aEnemy[0].Destrot.y = pCamera->rot.y - D3DX_PI / 2;
			//}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�f�o�b�O�p�G�ړ�����
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].vec.z > 2.0f)
			{//��O�ړ�
				g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y) / 10;
				
				if (g_aEnemy[nCntEnemy].bLoop == false)
				{
					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
					g_aEnemy[nCntEnemy].bLoop = true;
				}

				if (g_aEnemy[nCntEnemy].vec.x < 2.0f)
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y + D3DX_PI * 3 / 4;
				}
				else if (g_aEnemy[nCntEnemy].vec.x > 2.0f)
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y - D3DX_PI * 3 / 4;
				}
				else
				{

					if (g_aEnemy[nCntEnemy].rot.y >= pCamera->rot.y)
					{
						g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y + D3DX_PI;
					}
					else if (g_aEnemy[nCntEnemy].rot.y < pCamera->rot.y)
					{
						g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y - D3DX_PI;
					}
				}
			}
			else if (g_aEnemy[nCntEnemy].vec.z < 2.0f)
			{//���ړ�
				g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y) / 10;

				if (g_aEnemy[nCntEnemy].bLoop == false)
				{
					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
					g_aEnemy[nCntEnemy].bLoop = true;
				}

				if (g_aEnemy[nCntEnemy].vec.x < 2.0f)
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y + D3DX_PI / 4;
				}
				else if (g_aEnemy[nCntEnemy].vec.x > 2.0f)
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y - D3DX_PI / 4;
				}
				else
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y;
				}
			}
			else if (g_aEnemy[nCntEnemy].vec.x < 2.0f)
			{//���ړ�
				g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y) / 10;
				if (g_aEnemy[nCntEnemy].bLoop == false)
				{
					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
					g_aEnemy[nCntEnemy].bLoop = true;
				}
				g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y + D3DX_PI / 2;
			}
			else if (g_aEnemy[nCntEnemy].vec.x > 2.0f)
			{//�E�ړ�
				g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y) / 10;
				if (g_aEnemy[nCntEnemy].bLoop == false)
				{
					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
					g_aEnemy[nCntEnemy].bLoop = true;
				}
				g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y - D3DX_PI / 2;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�p�x�̐��K������
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{//���݂̊p�x��(PI)���傫���ꍇ
				g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
			}
			else if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{//���݂̊p�x��(-PI)��菬�����ꍇ
				g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}

			if (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{//���݂̖ړI�l��(PI)���傫���ꍇ
				g_aEnemy[nCntEnemy].Destrot.y -= 2 * D3DX_PI;
			}
			else if (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{//���݂̖ړI�n��(-PI)��菬�����ꍇ
				g_aEnemy[nCntEnemy].Destrot.y += 2 * D3DX_PI;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�G�L�������[�V��������
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			for (int nCntPartsEnemy = 0; nCntPartsEnemy < MAX_ENEMY_PARTS; nCntPartsEnemy++)
			{
				//�ʏ���
				if (g_aEnemy[nCntEnemy].motion == ENEMY_MOTION_NUTRAL)
				{
					g_aEnemy[nCntEnemy].fFlame++;

					if (g_aEnemy[nCntEnemy].fFlame <= 40)
					{
						//��(�e)�̃p�[�c
						g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(-0.002f, 0.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].fFlame <= 80)
					{
						//��(�e)�̃p�[�c
						g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(-0.002f, 0.0f, 0.0f);
					}
					else
					{//���[�V�������Z�b�g
						g_aEnemy[nCntEnemy].fFlame = 0;
						//��(�e)�̃p�[�c
						g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
						g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				//�ʏ��ԈȊO�̏���
				else if (g_aEnemy[nCntEnemy].bLoop == true)
				{
					//���s���
					if (g_aEnemy[nCntEnemy].motion == ENEMY_MOTION_WORK)
					{
						if (g_aEnemy[nCntEnemy].fFlame <= 0)
						{//���[�V�������Z�b�g
							g_aEnemy[nCntEnemy].fFlame = 0;
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
						g_aEnemy[nCntEnemy].fFlame++;

						if (g_aEnemy[nCntEnemy].fFlame <= 5)
						{
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(-0.02f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 10)
						{
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(-0.02f, 0.0f, 0.0f);
						}
						else
						{//���[�V�������Z�b�g
							g_aEnemy[nCntEnemy].fFlame = 0;
							g_aEnemy[nCntEnemy].bLoop = false;
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
					//�U�����
					else if (g_aEnemy[nCntEnemy].motion == ENEMY_MOTION_ATTACK)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						if (g_aEnemy[nCntEnemy].fFlame <= 0)
						{//���[�V�������Z�b�g
							g_aEnemy[nCntEnemy].fFlame = 0;
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
						g_aEnemy[nCntEnemy].fFlame++;

						if (g_aEnemy[nCntEnemy].fFlame <= 20)
						{
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(-0.03f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 30)
						{
							if (g_aEnemy[nCntEnemy].fFlame == 30)
							{//�U������쐬(�ʒu, �ړ���, ��������, ���)
								SetAttack
								(
									D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - sinf(g_aEnemy[nCntEnemy].rot.y) * 1.0f, (g_aEnemy[nCntEnemy].pos.y + 1.0f), g_aEnemy[nCntEnemy].pos.z - cosf(g_aEnemy[nCntEnemy].rot.y) * 1.0f),
									D3DXVECTOR3(-sinf(g_aEnemy[nCntEnemy].rot.y) * 2.5f, 1.0f, -cosf(g_aEnemy[nCntEnemy].rot.y) * 2.5f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									ENEMY_ATTACK,
									ATTACK_TYPE_ENEMY
								);
							}

							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(-0.06f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 40)
						{
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos += D3DXVECTOR3(0.0f, 2.0f, -2.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(0.03f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 50)
						{
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos -= D3DXVECTOR3(0.0f, 2.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 60)
						{
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos -= D3DXVECTOR3(0.0f, 0.0f, -2.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(0.03f, 0.0f, 0.0f);
						}
						else
						{//���[�V�������Z�b�g
							g_aEnemy[nCntEnemy].fFlame = 0;
							g_aEnemy[nCntEnemy].bLoop = false;
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
					//��e���
					else if (g_aEnemy[nCntEnemy].motion == ENEMY_MOTION_DAMEGE)
					{
						if (g_aEnemy[nCntEnemy].fFlame <= 0)
						{//���[�V�������Z�b�g
							g_aEnemy[nCntEnemy].fFlame = 0;
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
						g_aEnemy[nCntEnemy].fFlame++;

						if (g_aEnemy[nCntEnemy].fFlame <= 5)
						{
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(0.04f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 10)
						{
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(0.14f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 20)
						{
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(0.07f, 0.0f, 0.0f);
						}
						else
						{//���[�V�������Z�b�g
							g_aEnemy[nCntEnemy].bLoop = false;
							g_aEnemy[nCntEnemy].fFlame = 0;
							//��(�e)�̃p�[�c
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
				else
				{//�ʏ��ԈȊO�̃��[�V�������I�������ꍇ
					g_aEnemy[nCntEnemy].fFlame = 0;
					//��(�e)�̃p�[�c
					g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
					g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_NUTRAL;		//�ʏ��Ԃɖ߂�
				}
			}
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�������畜�A���鏈��
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].pos.y <= -100)
			{
				g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�G���Ƃ�AI
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].vec.x >= -(size.x + ENEMY_BATTLE_RANGE) || g_aEnemy[nCntEnemy].vec.x <= (size.x + ENEMY_BATTLE_RANGE)
				|| g_aEnemy[nCntEnemy].vec.z >= -(size.z + ENEMY_BATTLE_RANGE) || g_aEnemy[nCntEnemy].vec.z <= (size.z + ENEMY_BATTLE_RANGE))
			{
				g_aEnemy[nCntEnemy].bBattle = true;
			}
			else
			{
				g_aEnemy[nCntEnemy].bBattle = false;
			}

			//�C���_���S�̏���
			if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_POTATE)
			{
				if (g_aEnemy[nCntEnemy].bBattle == true)
				{
					g_aEnemy[nCntEnemy].fVecCnt++; 
					//�G�L�����̒Ǐ]�𐧌����鏈��
					if (g_aEnemy[nCntEnemy].fVecCnt >= 60)
					{
						//�x�N�g�����ړ��ʂɉ��Z���ĒǏ]�����鏈��
						g_aEnemy[nCntEnemy].move += ((g_aEnemy[nCntEnemy].vec1 * 0.4f) + (g_aEnemy[nCntEnemy].vec * 0.00001f));

						if (g_aEnemy[nCntEnemy].bLoop == false)
						{
							if (g_aEnemy[nCntEnemy].vec.x >= (size.x + ENEMY_ATTACK_RANGE) || g_aEnemy[nCntEnemy].vec.x <= -(size.x + ENEMY_ATTACK_RANGE)
								|| g_aEnemy[nCntEnemy].vec.z >= (size.z + ENEMY_ATTACK_RANGE) || g_aEnemy[nCntEnemy].vec.z <= -(size.z + ENEMY_ATTACK_RANGE))
							{
								g_aEnemy[nCntEnemy].fFlame = 0;
								g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
								g_aEnemy[nCntEnemy].bLoop = true;
							}
							else
							{
								g_aEnemy[nCntEnemy].fFlame = 0;
								g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_ATTACK;
								g_aEnemy[nCntEnemy].bLoop = true;
							}
						}
					}
					if (g_aEnemy[nCntEnemy].fVecCnt >= 90)
					{
						g_aEnemy[nCntEnemy].fVecCnt = 0;
					}
				}
			}
			//�J�{�`���_���S�̏���
			if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_PUNPKIN)
			{
				if (g_aEnemy[nCntEnemy].bBattle == true)
				{
					g_aEnemy[nCntEnemy].fVecCnt++;
					//�G�L�����̒Ǐ]�𐧌����鏈��
					if (g_aEnemy[nCntEnemy].fVecCnt >= 90)
					{
						//�x�N�g�����ړ��ʂɉ��Z���ĒǏ]�����鏈��
						g_aEnemy[nCntEnemy].move += ((g_aEnemy[nCntEnemy].vec1 * ENEMY_SPEAD) + (g_aEnemy[nCntEnemy].vec * 0.00001f));

						if (g_aEnemy[nCntEnemy].bLoop == false)
						{
							if (g_aEnemy[nCntEnemy].vec.x >= (size.x + ENEMY_ATTACK_RANGE) || g_aEnemy[nCntEnemy].vec.x <= -(size.x + ENEMY_ATTACK_RANGE)
								|| g_aEnemy[nCntEnemy].vec.z >= (size.z + ENEMY_ATTACK_RANGE) || g_aEnemy[nCntEnemy].vec.z <= -(size.z + ENEMY_ATTACK_RANGE))
							{
								g_aEnemy[nCntEnemy].fFlame = 0;
								g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
								g_aEnemy[nCntEnemy].bLoop = true;
							}
							else
							{
								g_aEnemy[nCntEnemy].fFlame = 0;
								g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_ATTACK;
								g_aEnemy[nCntEnemy].bLoop = true;
							}
						}
					}
					if (g_aEnemy[nCntEnemy].fVecCnt >= 120)
					{
						g_aEnemy[nCntEnemy].fVecCnt = 0;
					}
				}
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//���̑��̏���
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�v���C���[�̑O��ʒu�𔽉f
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			//�G�L�����̈ʒu�X�V
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.1f;

			//�d�͂��X�V(����������)
			g_aEnemy[nCntEnemy].move.y -= 4.0f;

			//�ړ��ʂ��X�V(����������)
			g_aEnemy[nCntEnemy].move -= g_aEnemy[nCntEnemy].move * 0.1f;
			
			//���C�t��0�ȉ��ɂȂ����ꍇ
			if (g_aEnemy[nCntEnemy].nLife <= 0)
			{
				g_aEnemy[nCntEnemy].bUse = false;
				AddScore(500);
			}

			//���f�������蔻��̏���
			ModelCollision(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, &r_size);

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//��e���菈��
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//��e����̏���
			if (pAttack->type != ATTACK_TYPE_ENEMY)
			{
				g_aEnemy[nCntEnemy].bDamege = AttackCollision(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, &r_size);
			}

			if (g_aEnemy[nCntEnemy].bDamege == true)
			{
				g_aEnemy[nCntEnemy].fFlame = 0;
				g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_DAMEGE;
				g_aEnemy[nCntEnemy].bLoop = true;

				g_aEnemy[nCntEnemy].move =
					D3DXVECTOR3(-sinf(pPlayer->rot.y) * 20.0f, 100.0f, -cosf(pPlayer->rot.y) * 20.0f);
				g_aEnemy[nCntEnemy].nLife -= pPlayer->nPower;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//���̔��菈��
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//���b�V���t�B�[���h�����蔻��̏���
			FieldCollision(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move);
		}
	}
}

//============================================================================
//�G�L�����̕`�揈��
//============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].Worldmtx);

			//���݂̃}�e���A�����擾����
			pDevice->GetMaterial(&matDef);

			//�����𔽉f(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].Worldmtx, &g_aEnemy[nCntEnemy].Worldmtx, &mtxRot);
			
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].Worldmtx, &g_aEnemy[nCntEnemy].Worldmtx, &mtxTrans);
			
			for (int nCntPartsEnemy = 0; nCntPartsEnemy < MAX_ENEMY_PARTS; nCntPartsEnemy++)
			{
				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx);

				//�����𔽉f(YaW : y,Pitch : x,Roll : z)
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot.y, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot.x, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &mtxRot);

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos.x, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos.y, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &mtxTrans);

				//�e���f���̃}�g���b�N�X�Ƃ̊|���Z
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &g_aEnemy[nCntEnemy].Worldmtx);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx);

				//���݂̃}�e���A���̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aX_Enemy[g_aEnemy[nCntEnemy].type].pBuffMat->GetBufferPointer();

				//�e�N�X�`���̓ǂݏo��
				for (int nCntMat = 0; nCntMat < (int)g_aX_Enemy[g_aEnemy[nCntEnemy].type].nNumMat; nCntMat++)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aX_Enemy[g_aEnemy[nCntEnemy].type].g_pTexture[g_aEnemy[nCntEnemy].type]);
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					//���f���p�[�c�̕`��
					g_aX_Enemy[g_aEnemy[nCntEnemy].type].pMesh->DrawSubset(nCntMat);
				}
			}
			//�ێ����Ă��}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//============================================================================
//�G�L�����̓����蔻�菈��
//============================================================================
bool EnemyCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size)
{
	bool bIsGround = false;
	PLAYER *pPlayer = GetPlayer();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			for (int nCntPartsEnemy = 0; nCntPartsEnemy < MAX_ENEMY_PARTS; nCntPartsEnemy++)
			{
				//�G�L�����̈ʒu�����߂鏈��(�G�L�����̃T�C�Y + �G�L�����̈ʒu)
				float EnemyL = g_aX_Enemy[nCntPartsEnemy].vtxMin.x + g_aEnemy[nCntEnemy].pos.x;		//�G�L�����̍���
				float EnemyR = g_aX_Enemy[nCntPartsEnemy].vtxMax.x + g_aEnemy[nCntEnemy].pos.x;		//�G�L�����̉E��

				float EnemyFront = g_aX_Enemy[nCntPartsEnemy].vtxMin.z + g_aEnemy[nCntEnemy].pos.z;	//�G�L�����̎�O��
				float EnemyBack = g_aX_Enemy[nCntPartsEnemy].vtxMax.z + g_aEnemy[nCntEnemy].pos.z;	//�G�L�����̉���

				float EnemyDown = g_aX_Enemy[nCntPartsEnemy].vtxMin.y + g_aEnemy[nCntEnemy].pos.y;	//�G�L�����̉���
				float EnemyTop = g_aX_Enemy[nCntPartsEnemy].vtxMax.y + g_aEnemy[nCntEnemy].pos.y;	//�G�L�����̓���

				//�G�L�����̓����蔻��
				if (pos->x + r_size->x > EnemyL		&& pos->x - r_size->x < EnemyR
					&&	pos->z + r_size->z > EnemyFront	&& pos->z - r_size->z < EnemyBack
					&&	pos->y + r_size->y > EnemyDown	&& pos->y - r_size->y < EnemyTop)
				{
					//x�������蔻��
					if (pos->x + r_size->x > EnemyL && posOld->x + r_size->x <= EnemyL)
					{//�����̓����蔻��
						pos->x = EnemyL - r_size->x;
						move->x = 0.0f;
						g_aEnemy[nCntEnemy].move.x = 0.0f;
					}
					else if (pos->x - r_size->x < EnemyR && posOld->x - r_size->x >= EnemyR)
					{//�E���̓����蔻��
						pos->x = EnemyR + r_size->x;
						move->x = 0.0f;
						g_aEnemy[nCntEnemy].move.x = 0.0f;
					}
					//z�������蔻��
					if (pos->z + r_size->z > EnemyFront && posOld->z + r_size->z <= EnemyFront)
					{//��O���̓����蔻��
						pos->z = EnemyFront - r_size->z;
						move->z = 0.0f;
						g_aEnemy[nCntEnemy].move.z = 0.0f;
					}
					else if (pos->z - r_size->z < EnemyBack && posOld->z - r_size->z >= EnemyBack)
					{//�����̓����蔻��
						pos->z = EnemyBack + r_size->z;
						move->z = 0.0f;
						g_aEnemy[nCntEnemy].move.z = 0.0f;
					}
					//y�������蔻��
					if (pos->y + r_size->y > EnemyDown && posOld->y + r_size->y <= EnemyDown)
					{//�����̓����蔻��
						pos->y = EnemyDown - r_size->y;
						move->y = 0.0f;
						g_aEnemy[nCntEnemy].move.y = 0.0f;
					}
					else if (pos->y - r_size->y < EnemyTop && posOld->y - r_size->y >= EnemyTop)
					{//�㑤�̓����蔻��
						pos->y = EnemyTop + r_size->y;
						move->y = 0.0f;
						g_aEnemy[nCntEnemy].move.y = 0.0f;

						bIsGround = true;
					}
				}
			}
		}
	}
	return bIsGround;
}

//============================================================================
//�G�L�����̐ݒ菈��
//============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;
			g_aEnemy[nCntEnemy].type = type;
			g_aEnemy[nCntEnemy].bUse = true;

			if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_POTATE)
			{
				g_aEnemy[nCntEnemy].nLife = 2;
			}
			else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_PUNPKIN)
			{
				g_aEnemy[nCntEnemy].nLife = 3;
			}
			break;
		}
	}
}

//============================================================================
//�G�L�����̎擾����
//============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}
