//============================================================================
//
//��������[explosion.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "bullet.h"		//�쐬����bullet.h���C���N���[�h����
#include "explosion.h"	//�쐬����explosion.h���C���N���[�h����
#include "block.h"		//�쐬����block.h���C���N���[�h����
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "score.h"		//�쐬����score.h���C���N���[�h����
#include "enemy.h"			//�쐬����enemy.h���C���N���[�h����
#include "white.h"		//�쐬����white.h���C���N���[�h����
#include "sound.h"

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffExplosion = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Explosion	g_aExplosion[MAX_EXPLOSION];	//�����̏��

float g_fLengthExplosion;								//�Ίp���̒���
float g_fAngleExplosion;								//�Ίp���̊p�x

//============================================================================
//�����̏���������
//============================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\explosion000.png",		//�e�N�X�`���̃t�@�C����
		&g_pTextureExplosion
	);

	//�e�̏��̏�����
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//�\���̂̏���������
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
		g_aExplosion[nCntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].bUse = false;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;				//�p�^�[��No.������������
	}

	//�e�̑Ίp���̒������Z�o����
	g_fLengthExplosion = sqrtf(EXPLOSION_WIDTH * EXPLOSION_WIDTH + EXPLOSION_HEIGHT * EXPLOSION_HEIGHT) / 2.0f;

	//�e�̑Ίp���̊p�x���Z�o����
	g_fAngleExplosion = atan2f(EXPLOSION_WIDTH, EXPLOSION_HEIGHT);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_�� * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
		pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y - cosf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
		pVtx[0].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

		pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
		pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
		pVtx[1].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

		pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x - sinf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
		pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y - cosf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
		pVtx[2].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

		pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x - sinf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
		pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
		pVtx[3].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

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

		//�e�N�X�`�����W�̐ݒ�:X���W = (1.0f / x�p�^�[����) * (�p�^�[��No. % x�p�^�[����), Y���W = (1.0f / y�p�^�[����) * (�p�^�[��No. / x�p�^�[����) : int�^�Ȃ̂ŏ����_�͔��f����Ȃ�
		pVtx[0].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 0.0f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 1.0f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 0.0f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 1.0f), 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//============================================================================
//�����̏I������
//============================================================================
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//============================================================================
//�����̍X�V����
//============================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	BLOCK *pBlock;		//�u���b�N���ւ̃|�C���^
	pBlock = GetBlock();//�u���b�N���̎擾
	int nCntBlock;

	ENEMY *pEnemy;			//�G���ւ̃|�C���^
	pEnemy = GetEnemy();	//�G���̎擾
	int nCntEnemy;

	ITEM *pItem;			//�G���ւ̃|�C���^
	pItem = GetItem();		//���̎擾
	int nCntItem;

	Player *pPlayer;
	pPlayer = GetPlayer();
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�A�j���[�V�����̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�������g�p����Ă���
			g_aExplosion[nCntExplosion].nPatternAnim += 2;			//�p�^�[��No.���X�V����

			if (g_aExplosion[nCntExplosion].nPatternAnim >= EXPLOSION_ANIM_CNT * 6)
			{//���p�^�[�����𒴂������̏���
				g_aExplosion[nCntExplosion].nPatternAnim = 0;	//�p�^�[��No.�������l�ɖ߂�

				//�g�p����Ă��Ȃ���Ԃɂ���
				g_aExplosion[nCntExplosion].bUse = false;
			}

			//�e�N�X�`�����W�̐ݒ�:X���W = (1.0f / x�p�^�[����) * (�p�^�[��No. % x�p�^�[����), Y���W = (1.0f / y�p�^�[����) * (�p�^�[��No. / x�p�^�[����) : int�^�Ȃ̂ŏ����_�͔��f����Ȃ�
			pVtx[0].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 0.0f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 1.0f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 0.0f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 1.0f), 1.0f);

			if (pPlayer->state != PLAYERSTATE_DEATH)
			{
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//�������u���b�N�ɓ����������̏���
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
				{
					if (pBlock->bUse == true)
					{//�u���b�N���g�p����Ă���ꍇ�̏���
						if (g_aExplosion[nCntExplosion].pos.x - EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 <= pBlock->pos.x + pBlock->fWidth / 2
							&& g_aExplosion[nCntExplosion].pos.x + EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 >= pBlock->pos.x - pBlock->fWidth / 2
							&& g_aExplosion[nCntExplosion].pos.y - EXPLOSION_HEIGHT / 2 < pBlock->pos.y + pBlock->fHeight / 2
							&& g_aExplosion[nCntExplosion].pos.y + EXPLOSION_HEIGHT / 2 > pBlock->pos.y - pBlock->fHeight / 2)
						{//�u���b�N�Ɣ��������������ꍇ�̏���
							if (pBlock->type != BLOCK_TYPE_WHITE && pBlock->type != BLOCK_TYPE_NONE)
							{//�u���b�N�������Ȃ��ĂȂ��ꍇ
								HitBlock(nCntBlock);	//��e�����u���b�N��Ԃ�����
								AddScore(100);			//�X�R�A��Ԃ�����
								AddWhite(1);
							}
						}
					}
				}
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//�������G�ɓ����������̏���
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G���g�p����Ă���ꍇ�̏���
						if (g_aExplosion[nCntExplosion].pos.x - EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 <= pEnemy->pos.x + ENEMY_WIDTH / 2
							&& g_aExplosion[nCntExplosion].pos.x + EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 >= pEnemy->pos.x - ENEMY_WIDTH / 2
							&& g_aExplosion[nCntExplosion].pos.y - EXPLOSION_HEIGHT / 2 < pEnemy->pos.y + ENEMY_HEIGHT / 2
							&& g_aExplosion[nCntExplosion].pos.y + EXPLOSION_HEIGHT / 2 > pEnemy->pos.y - ENEMY_HEIGHT / 2)
						{//�G�Ɣ��������������ꍇ�̏���
							if (pEnemy->state != ENEMY_STATE_WHITE && pEnemy->bHide == false)
							{//�G�������Ȃ��ĂȂ��ꍇ
								HitEnemy(nCntEnemy, 100);	//��e�����u���b�N��Ԃ�����
								AddWhite(1);
							}
						}
					}
				}
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//�������A�C�e���ɓ����������̏���
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
				{
					if (pItem->bUse == true)
					{//�A�C�e�����g�p����Ă���ꍇ�̏���
						if (g_aExplosion[nCntExplosion].pos.x - EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 <= pItem->pos.x + pItem->fWidth / 2
							&& g_aExplosion[nCntExplosion].pos.x + EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 >= pItem->pos.x - pItem->fWidth / 2
							&& g_aExplosion[nCntExplosion].pos.y - EXPLOSION_HEIGHT / 2 < pItem->pos.y + pItem->fHeight / 2
							&& g_aExplosion[nCntExplosion].pos.y + EXPLOSION_HEIGHT / 2 > pItem->pos.y - pItem->fHeight / 2)
						{//�A�C�e���Ɣ��������������ꍇ�̏���
							if (pItem->type != ITEM_TYPE_WHITE && pItem->state != ITEM_STATE_DELETE)
							{//�A�C�e���������Ȃ��ĂȂ��ꍇ
								HitItem(nCntItem);	//��e�����u���b�N��Ԃ�����
								AddScore(500);		//�X�R�A��Ԃ�����
								AddWhite(1);
							}
						}
					}
				}
			}
		}
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

//============================================================================
//�����̕`�揈��
//============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntExplosion;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffExplosion,	//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//�e���g�p����Ă���
		 //�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntExplosion * 4,		//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}
}

//============================================================================
////�����̐ݒ菈��
//============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
 	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//�e���g�p����Ă��Ȃ��ꍇ
			g_aExplosion[nCntExplosion].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
			pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y - cosf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
			pVtx[0].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

			pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
			pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
			pVtx[1].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

			pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x - sinf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
			pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y - cosf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
			pVtx[2].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

			pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x - sinf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
			pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
			pVtx[3].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;	//�p�^�[��No.������������

			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].bUse = true;		//�g�p��Ԃ�true�ɂ���
			break;											//for���𔲂��鏈��
		}
		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}