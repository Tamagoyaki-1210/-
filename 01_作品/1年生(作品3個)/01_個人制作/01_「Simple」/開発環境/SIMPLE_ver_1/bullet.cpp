//============================================================================
//
//�e�̏���[bullet.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "bullet.h"		//�쐬����bullet.h���C���N���[�h����
#include "explosion.h"	//�쐬����explosion.h���C���N���[�h����
#include "block.h"		//�쐬����block.h���C���N���[�h����
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "enemy.h"		//�쐬����enemy.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];						//�e�̏��
float g_fLengthBullet;								//�Ίp���̒���
float g_fAngleBullet;								//�Ίp���̊p�x

//============================================================================
//�e�̏���������
//============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\bullet000.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureBullet
	);

	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//�\���̂̏���������
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.5f);		//�ʒu�̏���������
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ̏���������
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����������������
		g_aBullet[nCntBullet].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);	//�e�̏����z�F
		g_aBullet[nCntBullet].nLife = MAX_BULLET_LIFE;					//�e�̏����̗�
		g_aBullet[nCntBullet].bUse = false;								//�e�̎g�p����
	}

	//�e�̑Ίp���̒������Z�o����
	g_fLengthBullet = sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HEIGHT * BULLET_HEIGHT) / 2.0f;

	//�e�̑Ίp���̊p�x���Z�o����
	g_fAngleBullet = atan2f(BULLET_WIDTH, BULLET_HEIGHT);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BULLET,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_�� * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
		pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
		pVtx[0].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

		pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
		pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
		pVtx[1].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

		pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
		pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
		pVtx[2].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

		pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
		pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
		pVtx[3].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//============================================================================
//�e�̏I������
//============================================================================
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//============================================================================
//�e�̍X�V����
//============================================================================
void UpdateBullet(void)
{
	int nCntBullet;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	BLOCK *pBlock;			//�u���b�N���ւ̃|�C���^
	pBlock = GetBlock();	//�u���b�N���̎擾
	int nCntBlock;

	ENEMY *pEnemy;			//�G���ւ̃|�C���^
	pEnemy = GetEnemy();	//�G���̎擾
	int nCntEnemy;

	ITEM *pItem;			//�G���ւ̃|�C���^
	pItem = GetItem();		//���̎擾
	int nCntItem;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���

			//�e�̈ʒu�̍X�V����
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[0].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[1].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[2].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[3].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�e����ʊO�ɏo�����̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aBullet[nCntBullet].pos.x <= -BULLET_WIDTH / 2)
			{//��ʍ��[�܂ŗ������̏���
				g_aBullet[nCntBullet].pos.x = SCREEN_WIDTH + BULLET_WIDTH / 2;
			}
			else if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH + BULLET_WIDTH / 2)
			{//��ʉE�[�܂ŗ������̏���
				g_aBullet[nCntBullet].pos.x = -BULLET_WIDTH / 2;
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�e�̎������s�������̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�e�̎����J�E���g�_�E������
			g_aBullet[nCntBullet].nLife--;

			if(g_aBullet[nCntBullet].nLife <= 0)
			{//�e�̎������s�������̏���
				g_aBullet[nCntBullet].bUse = false;
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�e���u���b�N�ɓ����������̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
			{
				if (pBlock->type != BLOCK_TYPE_WHITE )
				{
					if (pBlock->bUse == true)
					{//�u���b�N���g�p����Ă���ꍇ�̏���
						if (g_aBullet[nCntBullet].pos.x - BULLET_WIDTH <= pBlock->pos.x + pBlock->fWidth / 2
							&& g_aBullet[nCntBullet].pos.x + BULLET_WIDTH >= pBlock->pos.x - pBlock->fWidth / 2
							&& g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT < pBlock->pos.y + pBlock->fHeight / 2
							&& g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT > pBlock->pos.y - pBlock->fHeight / 2)
						{//�u���b�N�Ɣ��������������ꍇ�̏���
							g_aBullet[nCntBullet].nLife = 0;
						}
					}
				}
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�e���G�ɓ����������̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->type != ENEMY_TYPE_WHITE && pEnemy->bHide == false)
				{
					if (pEnemy->bUse == true)
					{//�u���b�N���g�p����Ă���ꍇ�̏���
						if (g_aBullet[nCntBullet].pos.x - BULLET_WIDTH <= pEnemy->pos.x + ENEMY_WIDTH / 2
							&& g_aBullet[nCntBullet].pos.x + BULLET_WIDTH >= pEnemy->pos.x - ENEMY_WIDTH / 2
							&& g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT < pEnemy->pos.y + ENEMY_HEIGHT / 2
							&& g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT > pEnemy->pos.y - ENEMY_HEIGHT / 2)
						{//�u���b�N�Ɣ��������������ꍇ�̏���
							g_aBullet[nCntBullet].nLife = 0;
						}
					}
				}
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�e���A�C�e���ɓ����������̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
			{
				if (pItem->type != ITEM_TYPE_WHITE)
				{
					if (pItem->bUse == true)
					{//�u���b�N���g�p����Ă���ꍇ�̏���
						if (g_aBullet[nCntBullet].pos.x - BULLET_WIDTH <= pItem->pos.x + ITEM_WIDTH / 2
							&& g_aBullet[nCntBullet].pos.x + BULLET_WIDTH >= pItem->pos.x - ITEM_WIDTH / 2
							&& g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT < pItem->pos.y + ITEM_HEIGHT / 2
							&& g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT > pItem->pos.y - ITEM_HEIGHT / 2)
						{//�u���b�N�Ɣ��������������ꍇ�̏���
							g_aBullet[nCntBullet].nLife = 0;
						}
					}
				}
			}
			
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//============================================================================
//�e�̕`�揈��
//============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBullet,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă��� 
			 //�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntBullet * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}
}
//============================================================================
////�e�̐ݒ菈��
//============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	int nCntBullet;
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
 	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_aBullet[nCntBullet].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[0].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[1].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[2].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[3].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nLife = MAX_BULLET_LIFE;
			g_aBullet[nCntBullet].bUse = true;	//�g�p��Ԃ�true�ɂ���
			break;								//for���𔲂��鏈��
		}
		pVtx += 4;								//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}