//============================================================================
//
//���f���̏���[model.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "camera.h"		//�쐬����camera.h���C���N���[�h����
#include "shadow.h"		//�쐬����shadow.h���C���N���[�h����
#include "effect.h"		//�쐬����effect.h���C���N���[�h����
#include "model.h"		//�쐬����model.h���C���N���[�h����
#include "meshfield.h"	//�쐬����meshfield.h���C���N���[�h����
#include "enemy.h"		//�쐬����enemy.h���C���N���[�h����
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "attack.h"		//�쐬����attack.h���C���N���[�h����
#include "fade.h"		//�쐬����fade.h���C���N���[�h����
#include "game.h"		//�쐬����game.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
PLAYER g_aPlayer;
D3DXVECTOR3 g_rotDestPlayer;
int g_nIndxShadowPlayer;				//�e�̃C���f�b�N�X

										//============================================================================
										//�v���C���[�̏���������
										//============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, -300.0f);
	g_aPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aPlayer.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);
	g_aPlayer.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;
	g_aPlayer.nLife = PLAYER_LIFE;
	g_aPlayer.nPower = 0;
	g_aPlayer.fFlame = 0;
	g_aPlayer.fAttack = 0;
	g_aPlayer.bUse = true;
	g_aPlayer.bDamege = false;
	g_aPlayer.bLoop = false;
	g_aPlayer.bGround = false;
	g_aPlayer.tex = PLAYER_TEXTURE_MOFU;
	g_aPlayer.motion = PLAYER_MOTION_NUTRAL;
	g_aPlayer.Worldmtx = D3DXMATRIX();

	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYER_PARTS; nCntPlayerParts++)
	{
		g_aPlayer.aParts[nCntPlayerParts].Worldmtx = D3DXMATRIX();	//���[���h�}�g���b�N�X
	}

	//��(�e)�̃p�[�c
	g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.aParts[0].nIdxModelParent = -1;

	//��(�q)�̃p�[�c
	g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
	g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.aParts[1].nIdxModelParent = 0;

	//����(�q)�̃p�[�c
	g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
	g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.aParts[2].nIdxModelParent = 0;

	//�E��(�q)�̃p�[�c
	g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
	g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.aParts[3].nIdxModelParent = 0;

	//X�t�@�C���̓ǂݍ���(��)
	D3DXLoadMeshFromX("Data\\Model\\00_niwamaru_body.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPlayer.aParts[0].pBuffMat,
		NULL,
		&g_aPlayer.aParts[0].nNumMat,
		&g_aPlayer.aParts[0].pMesh
	);

	//X�t�@�C���̓ǂݍ���(��)
	D3DXLoadMeshFromX("Data\\Model\\01_niwamaru_head.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPlayer.aParts[1].pBuffMat,
		NULL,
		&g_aPlayer.aParts[1].nNumMat,
		&g_aPlayer.aParts[1].pMesh
	);

	//X�t�@�C���̓ǂݍ���(����)
	D3DXLoadMeshFromX("Data\\Model\\02_niwamaru_legL.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPlayer.aParts[2].pBuffMat,
		NULL,
		&g_aPlayer.aParts[2].nNumMat,
		&g_aPlayer.aParts[2].pMesh
	);

	//X�t�@�C���̓ǂݍ���(�E��)
	D3DXLoadMeshFromX("Data\\Model\\03_niwamaru_legR.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPlayer.aParts[3].pBuffMat,
		NULL,
		&g_aPlayer.aParts[3].nNumMat,
		&g_aPlayer.aParts[3].pMesh
	);

	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYER_PARTS; nCntPlayerParts++)
	{
		//���݂̃}�e���A���̃|�C���^���擾
		D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^
		pMat = (D3DXMATERIAL*)g_aPlayer.aParts[nCntPlayerParts].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aParts[nCntPlayerParts].nNumMat; nCntMat++)
		{
			//�e�N�X�`���̓ǂݏo��
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//�e�N�X�`���̃t�@�C����
				&g_aPlayer.aParts[nCntPlayerParts].g_pTexture[PLAYER_TEXTURE_MOFU]
			);
		}
	}
	g_nIndxShadowPlayer = SetShadow(g_aPlayer.pos, g_aPlayer.rot);
}

//============================================================================
//�v���C���[�̏���������
//============================================================================
void UninitPlayer(void)
{
	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYER_PARTS; nCntPlayerParts++)
	{
		//���b�V���̔j��
		if (g_aPlayer.aParts[nCntPlayerParts].pMesh != NULL)
		{
			g_aPlayer.aParts[nCntPlayerParts].pMesh->Release();
			g_aPlayer.aParts[nCntPlayerParts].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aPlayer.aParts[nCntPlayerParts].pBuffMat != NULL)
		{
			g_aPlayer.aParts[nCntPlayerParts].pBuffMat->Release();
			g_aPlayer.aParts[nCntPlayerParts].pBuffMat = NULL;
		}

		// x���f���̎g�p�e�N�X�`����S�ĉ������(���������[�N�C��_2023/7/12)
		for (int nMatTex = 0; nMatTex < PLAYER_TYPE_MAX; nMatTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aPlayer.aParts[nCntPlayerParts].g_pTexture[nMatTex] != NULL)
			{
				g_aPlayer.aParts[nCntPlayerParts].g_pTexture[nMatTex]->Release();
				g_aPlayer.aParts[nCntPlayerParts].g_pTexture[nMatTex] = NULL;
			}
		}
	}
}

//============================================================================
//�v���C���[�̍X�V����
//============================================================================
void UpdatePlayer(void)
{
	FADE fade;
	fade = GetFade();
	CAMERA *pCamera = GetCamera();
	ENEMY *pEnemy = GetEnemy();
	ATTACK *pAttack = GetAttack();

	if (g_aPlayer.bUse == true)
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//�v���C���[�ړ�����
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (GetKeyboardPress(DIK_W) == true)
		{//��O�ړ�
			g_aPlayer.rot.y += (g_aPlayer.Destrot.y - g_aPlayer.rot.y) / 10;
			if (g_aPlayer.bLoop == false && g_aPlayer.bGround == true)
			{
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_WARK;
				g_aPlayer.bLoop = true;
			}

			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y - D3DX_PI / 4) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y - D3DX_PI / 4) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y + D3DX_PI * 3 / 4;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y + D3DX_PI / 4) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y + D3DX_PI / 4) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y - D3DX_PI * 3 / 4;
			}
			else
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y) * PLAYER_SPEAD;

				if (g_aPlayer.rot.y >= pCamera->rot.y)
				{
					g_aPlayer.Destrot.y = pCamera->rot.y + D3DX_PI;
				}
				else if (g_aPlayer.rot.y < pCamera->rot.y)
				{
					g_aPlayer.Destrot.y = pCamera->rot.y - D3DX_PI;
				}
			}
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//���ړ�
			g_aPlayer.rot.y += (g_aPlayer.Destrot.y - g_aPlayer.rot.y) / 10;
			if (g_aPlayer.bLoop == false && g_aPlayer.bGround == true)
			{
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_WARK;
				g_aPlayer.bLoop = true;
			}

			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y - D3DX_PI * 3 / 4) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y - D3DX_PI * 3 / 4) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y + D3DX_PI / 4;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y + D3DX_PI * 3 / 4) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y + D3DX_PI * 3 / 4) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y - D3DX_PI / 4;
			}
			else
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y - D3DX_PI) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y - D3DX_PI) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//���ړ�
			g_aPlayer.rot.y += (g_aPlayer.Destrot.y - g_aPlayer.rot.y) / 10;
			if (g_aPlayer.bLoop == false && g_aPlayer.bGround == true)
			{
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_WARK;
				g_aPlayer.bLoop = true;
			}

			g_aPlayer.move.x += sinf(pCamera->rot.y - D3DX_PI / 2.0f) * PLAYER_SPEAD;
			g_aPlayer.move.z += cosf(pCamera->rot.y - D3DX_PI / 2.0f) * PLAYER_SPEAD;
			g_aPlayer.Destrot.y = pCamera->rot.y + D3DX_PI / 2;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//�E�ړ�
			g_aPlayer.rot.y += (g_aPlayer.Destrot.y - g_aPlayer.rot.y) / 10;
			if (g_aPlayer.bLoop == false && g_aPlayer.bGround == true)
			{
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_WARK;
				g_aPlayer.bLoop = true;
			}

			g_aPlayer.move.x += sinf(pCamera->rot.y + D3DX_PI / 2.0f) * PLAYER_SPEAD;
			g_aPlayer.move.z += cosf(pCamera->rot.y + D3DX_PI / 2.0f) * PLAYER_SPEAD;
			g_aPlayer.Destrot.y = pCamera->rot.y - D3DX_PI / 2;
		}

		if (g_aPlayer.bGround == true)
		{//�ڒn���̏���
			if (GetKeyboardTrigger(DIK_SPACE) == true)
			{//�W�����v
				g_aPlayer.motion = PLAYER_MOTION_JUMP;
				g_aPlayer.bLoop = true;
				g_aPlayer.fFlame = 0;
				g_aPlayer.move.y += 100.0f;
				g_aPlayer.bGround = false;		//���n������U�ɂ���
			}
		}

		if (g_aPlayer.nPower < 3)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{//�U�����[�V����
				g_aPlayer.motion = PLAYER_MOTION_ATTACK;
				g_aPlayer.bLoop = true;
				g_aPlayer.fFlame = 0;
			}
		}
		else if (g_aPlayer.fFlame >= 6.0f)
		{
			if (GetKeyboardPress(DIK_RETURN) == true)
			{//�U�����[�V����
				g_aPlayer.motion = PLAYER_MOTION_ATTACK;
				g_aPlayer.bLoop = true;
				g_aPlayer.fFlame = 0;
			}
		}
		else
		{
			g_aPlayer.fFlame++;
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//�p�x�̐��K������
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_aPlayer.rot.y > D3DX_PI)
		{//���݂̊p�x��(PI)���傫���ꍇ
			g_aPlayer.rot.y -= D3DX_PI * 2;
		}
		else if (g_aPlayer.rot.y < -D3DX_PI)
		{//���݂̊p�x��(-PI)��菬�����ꍇ
			g_aPlayer.rot.y += D3DX_PI * 2;
		}

		if (g_aPlayer.Destrot.y - g_aPlayer.rot.y > D3DX_PI)
		{//���݂̖ړI�l��(PI)���傫���ꍇ
			g_aPlayer.Destrot.y -= 2 * D3DX_PI;
		}
		else if (g_aPlayer.Destrot.y - g_aPlayer.rot.y < -D3DX_PI)
		{//���݂̖ړI�n��(-PI)��菬�����ꍇ
			g_aPlayer.Destrot.y += 2 * D3DX_PI;
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//�v���C���[���[�V��������
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//�ʏ���
		if (g_aPlayer.motion == PLAYER_MOTION_NUTRAL)
		{
			g_aPlayer.fFlame++;

			if (g_aPlayer.fFlame <= 40)
			{
				//��(�e)�̃p�[�c
				g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.002f, 0.0f, 0.0f);
				//��(�q)�̃p�[�c
				g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.002f, 0.0f, 0.0f);
				//����(�q)�̃p�[�c
				g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.003f, 0.0f, 0.0f);
				//�E��(�q)�̃p�[�c
				g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.003f, 0.0f, 0.0f);
			}
			else if (g_aPlayer.fFlame <= 80)
			{
				//��(�e)�̃p�[�c
				g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.002f, 0.0f, 0.0f);
				//��(�q)�̃p�[�c
				g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.002f, 0.0f, 0.0f);
				//����(�q)�̃p�[�c
				g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.003f, 0.0f, 0.0f);
				//�E��(�q)�̃p�[�c
				g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.003f, 0.0f, 0.0f);
			}
			else
			{//���[�V�������Z�b�g
				g_aPlayer.fFlame = 0;
				//��(�e)�̃p�[�c
				g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
				g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//��(�q)�̃p�[�c
				g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
				g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//����(�q)�̃p�[�c
				g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
				g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//�E��(�q)�̃p�[�c
				g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
				g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
		//�ʏ��ԈȊO�̏���
		else if (g_aPlayer.bLoop == true)
		{
			//���s���
			if (g_aPlayer.motion == PLAYER_MOTION_WARK)
			{
				g_aPlayer.fFlame++;

				if (g_aPlayer.fFlame <= 5)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.01f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.01f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.1f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 10)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.01f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.01f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 15)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.01f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.01f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 20)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.01f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.01f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.1f, 0.0f, 0.0f);
				}
				else
				{//���[�V�������Z�b�g
					g_aPlayer.fFlame = 0;
					g_aPlayer.bLoop = false;
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			//�W�����v���
			if (g_aPlayer.motion == PLAYER_MOTION_JUMP)
			{
				if (g_aPlayer.fFlame == 0)
				{
					g_aPlayer.fFlame++;
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 2)
				{
					g_aPlayer.fFlame++;
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.5f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.5f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 4)
				{
					g_aPlayer.fFlame++;
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.5f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.5f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 6)
				{
					g_aPlayer.fFlame++;
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(-0.1f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos += D3DXVECTOR3(0.0f, -0.6f, 0.0f);
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(-0.6f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos += D3DXVECTOR3(0.0f, -0.6f, 0.0f);
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(-0.6f, 0.0f, 0.0f);
				}
			}
			//���n���
			if (g_aPlayer.motion == PLAYER_MOTION_LANDING)
			{
				g_aPlayer.fFlame++;

				if (g_aPlayer.move.x >= 1.0f || g_aPlayer.move.z >= 1.0f)
				{
					//���[�V�������Z�b�g
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					g_aPlayer.fFlame = 0;
					g_aPlayer.bLoop = false;
					g_aPlayer.motion = PLAYER_MOTION_WARK;
				}
				else if (g_aPlayer.fFlame == 1)
				{//���[�V�������Z�b�g
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 5)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].pos += D3DXVECTOR3(0.0f, -0.4f, 0.0f);
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.08f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos += D3DXVECTOR3(0.0f, 0.4f, 0.0f);
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.10f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos += D3DXVECTOR3(0.0f, 0.4f, 0.0f);
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.10f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 15)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].pos -= D3DXVECTOR3(0.0f, -0.2f, 0.0f);
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.04f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos -= D3DXVECTOR3(0.0f, 0.2f, 0.0f);
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.05f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos -= D3DXVECTOR3(0.0f, 0.2f, 0.0f);
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.05f, 0.0f, 0.0f);
				}
				else
				{
					g_aPlayer.fFlame = 0;
					g_aPlayer.bLoop = false;
				}
			}
			//�U�����
			if (g_aPlayer.motion == PLAYER_MOTION_ATTACK)
			{
					if (g_aPlayer.fFlame == 0)
					{//�U������쐬(�ʒu, �ړ���, ��������, ���)
						SetAttack
						(
							D3DXVECTOR3(g_aPlayer.pos.x - sinf(g_aPlayer.rot.y) * 12.0f, (g_aPlayer.pos.y + 10.0f), g_aPlayer.pos.z - cosf(g_aPlayer.rot.y) * 12.0f),
							D3DXVECTOR3(0.0f, -10.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							PLAYER_ATTACK,
							ATTACK_TYPE_PLAYER
						);
					}
				g_aPlayer.fFlame++;
				if (g_aPlayer.fFlame <= 4)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.4f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.5f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.5f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 8)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.4f, 0.0f, 0.0f);
					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.5f, 0.0f, 0.0f);
					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.5f, 0.0f, 0.0f);
				}
				else
				{//���[�V�������Z�b�g
					g_aPlayer.fFlame = 0;
					g_aPlayer.bLoop = false;
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//�e���I��������
					pAttack->bUse = false;
				}
			}
			//��e���
			if (g_aPlayer.motion == PLAYER_MOTION_DAMEGE)
			{
				if (g_aPlayer.fFlame <= 0)
				{//���[�V�������Z�b�g
					g_aPlayer.fFlame = 0;
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
				g_aPlayer.fFlame++;

				if (g_aPlayer.fFlame <= 5)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.08f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 10)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.28f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 20)
				{
					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.14f, 0.0f, 0.0f);
				}
				else
				{//���[�V�������Z�b�g
					g_aPlayer.bLoop = false;
					g_aPlayer.fFlame = 0;

					//��(�e)�̃p�[�c
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//��(�q)�̃p�[�c
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//����(�q)�̃p�[�c
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//�E��(�q)�̃p�[�c
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
		}
		else
		{//�ʏ��ԈȊO�̃��[�V�������I�������ꍇ
			g_aPlayer.fFlame = 0;
			//��(�e)�̃p�[�c
			g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
			g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//��(�q)�̃p�[�c
			g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
			g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//����(�q)�̃p�[�c
			g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
			g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//�E��(�q)�̃p�[�c
			g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
			g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlayer.motion = PLAYER_MOTION_NUTRAL;		//�ʏ��Ԃɖ߂�
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//�������畜�A���鏈��
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_aPlayer.pos.y <= -100)
		{
			g_aPlayer.nLife--;
			g_aPlayer.pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//���̑��̏���
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//�v���C���[�̑O��ʒu�𔽉f
		g_aPlayer.posOld = g_aPlayer.pos;

		//�v���C���[�̈ʒu�X�V
		g_aPlayer.pos += g_aPlayer.move * 0.1f;

		//�d�͂��X�V(����������)
		g_aPlayer.move.y -= 4.0f;

		//�ړ��ʂ��X�V(����������)
		g_aPlayer.move -= g_aPlayer.move * 0.1f;

		g_aPlayer.nPower = (g_aPlayer.nLife / PLAYER_LIFE) + 1;

		if (g_aPlayer.nLife >= MAX_PLAYER_LIFE)
		{
			g_aPlayer.nLife = MAX_PLAYER_LIFE;
		}
		else if (g_aPlayer.nLife < 0)
		{
			g_aPlayer.nLife = 0;
		}

		//���C�t��0�ɂȂ�����
		if (g_aPlayer.nLife == 0)
		{
			if (fade == FADE_NONE)
			{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
				SetGameState(GAMESTATE_GAMEOVER);
			}
			g_aPlayer.bUse = false;
		}

		//�v���C���[�̃T�C�Y�����߂鏈��
		D3DXVECTOR3 size = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

		//�v���C���[�̃T�C�Y�̔��a�����߂鏈��
		D3DXVECTOR3 r_size = size / 2;

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//�A�C�e�������蔻��̏���
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		bool bItem = ItemCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, &size);
		
		if (bItem == true)
		{
			g_aPlayer.nLife += 1;
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//��e���菈��
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (pAttack->type != ATTACK_TYPE_PLAYER)
		{
			g_aPlayer.bDamege = AttackCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, &r_size);

			if (g_aPlayer.bDamege == true)
			{

				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_DAMEGE;
				g_aPlayer.bLoop = true;
				g_aPlayer.move =
					D3DXVECTOR3(-sinf(pEnemy->rot.y) * 60.0f, 100.0f, -cosf(pEnemy->rot.y) * 60.0f);
				g_aPlayer.nLife -= 1;
			}
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//���̔��菈��
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//���b�V���t�B�[���h�����蔻��̏���
		bool bIsGroundField = FieldCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move);

		//�G�L���������蔻��̏���
		bool bIsGroundEnemy = EnemyCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, &r_size);

		//���f�������蔻��̏���
		bool bIsGroundModel = ModelCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, &r_size);

		if (bIsGroundField == true || bIsGroundEnemy == true || bIsGroundModel == true)
		{
			if (g_aPlayer.bGround == false)
			{
				g_aPlayer.bGround = true;	//���n�����^�ɂ���
				g_aPlayer.bLoop = true;
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_LANDING;
			}
		}
		//�e�̑傫���������Őݒ�
		SetpositionShadow(g_nIndxShadowPlayer, D3DXVECTOR3(g_aPlayer.pos.x, -2.0f, g_aPlayer.pos.z),
			SHADOW_WIDTH + (g_aPlayer.pos.y / 2), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1 - (g_aPlayer.pos.y / 100)));
	}
}

//============================================================================
//�v���C���[�̕`�揈��
//============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;						//�e�p�}�g���N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p

	if (g_aPlayer.bUse)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.Worldmtx);

		//���݂̃}�e���A�����擾����
		pDevice->GetMaterial(&matDef);

		//�����𔽉f(YaW : y,Pitch : x,Roll : z)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.Worldmtx, &g_aPlayer.Worldmtx, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.Worldmtx, &g_aPlayer.Worldmtx, &mtxTrans);

		//�p�[�c�̏���
		for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYER_PARTS; nCntPlayerParts++)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayer.aParts[nCntPlayerParts].Worldmtx);

			//�����𔽉f(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aParts[nCntPlayerParts].rot.y, g_aPlayer.aParts[nCntPlayerParts].rot.x, g_aPlayer.aParts[nCntPlayerParts].rot.z);
			D3DXMatrixMultiply(&g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aParts[nCntPlayerParts].pos.x, g_aPlayer.aParts[nCntPlayerParts].pos.y, g_aPlayer.aParts[nCntPlayerParts].pos.z);
			D3DXMatrixMultiply(&g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &mtxTrans);

			if (g_aPlayer.aParts[nCntPlayerParts].nIdxModelParent == -1)
			{//�e���f���̃C���f�b�N�X��-1�̏ꍇ
				mtxParent = g_aPlayer.Worldmtx;				//�v���C���[�}�g���N�X��o�^
			}
			else
			{
				mtxParent = g_aPlayer.aParts[g_aPlayer.aParts[nCntPlayerParts].nIdxModelParent].Worldmtx;		//�e�̃}�g���N�X��o�^
			}
			//�e���f���̃}�g���b�N�X�Ƃ̊|���Z
			D3DXMatrixMultiply(&g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &mtxParent);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aParts[nCntPlayerParts].Worldmtx);

			//���݂̃}�e���A���̃|�C���^���擾
			D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_aPlayer.aParts[nCntPlayerParts].pBuffMat->GetBufferPointer();

			//�e�N�X�`���̓ǂݏo��
			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aParts[nCntPlayerParts].nNumMat; nCntMat++)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aPlayer.aParts[nCntPlayerParts].g_pTexture[g_aPlayer.tex]);

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				//���f���p�[�c�̕`��
				g_aPlayer.aParts[nCntPlayerParts].pMesh->DrawSubset(nCntMat);
			}
		}
		//�e�N�X�`���̏�����(�o�O�C��_2023/7/12)
		pDevice->SetTexture(0, NULL);

		//�ێ����Ă��}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//============================================================================
//�v���C���[�̎擾����
//============================================================================
PLAYER *GetPlayer(void)
{
	return &g_aPlayer;
}

