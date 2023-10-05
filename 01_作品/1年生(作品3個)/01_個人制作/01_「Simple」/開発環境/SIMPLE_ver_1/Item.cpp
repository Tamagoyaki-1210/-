//============================================================================
//
//�A�C�e���̏���[item.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "score.h"		//�쐬����score.h���C���N���[�h����
#include "game.h"		//�쐬����game.h���C���N���[�h����
#include "fade.h"		//�쐬����fade.h���C���N���[�h����
#include "white.h"		//�쐬����white.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureItem[NUM_ITEM] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		//���_�o�b�t�@�ւ̃|�C���^
ITEM g_aItem[MAX_ITEM];								//�A�C�e���̏��
int nDeleteCounter;									//�폜�����A�C�e���̌����J�E���g

//============================================================================
//�A�C�e���̏���������
//============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�ʏ�A�C�e���̃e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\coin000.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureItem[ITEM_TYPE_COIN]
	);

	//�ʏ�A�C�e���̃e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\effect001.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureItem[ITEM_TYPE_EFFECT]
	);
	//�ړ����e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureItem[ITEM_TYPE_WHITE]
	);

	//�A�C�e���̏��̏�����
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//�\���̂̏���������
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏���������
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ̏���������
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����������������
		g_aItem[nCntItem].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�A�C�e���̏����z�F
		g_aItem[nCntItem].setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ݒu�ʒu�̏���������
		g_aItem[nCntItem].nCounterAnim = 0;
		g_aItem[nCntItem].nCounterAnimBreak = 0;
		g_aItem[nCntItem].nPatternAnim = 0;
		g_aItem[nCntItem].nDeleteItemCounter = 0;					//�A�C�e����������b��
		g_aItem[nCntItem].type = ITEM_TYPE_COIN;
		g_aItem[nCntItem].fWidth = ITEM_WIDTH;						//��
		g_aItem[nCntItem].fHeight = ITEM_HEIGHT;					//����
		g_aItem[nCntItem].fAngle = atan2f(ITEM_WIDTH, ITEM_HEIGHT);
		g_aItem[nCntItem].fLength = sqrtf(ITEM_WIDTH * ITEM_WIDTH + ITEM_HEIGHT * ITEM_HEIGHT) / 2.0f;
		g_aItem[nCntItem].bUse = false;								//�A�C�e���̎g�p����
		g_aItem[nCntItem].pBlock = NULL;							//�u���b�N����NULL����
		g_aItem[nCntItem].state = ITEM_STATE_NORMAL;
	}
	nDeleteCounter = 0;								//�폜�����A�C�e���̌����J�E���g

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_�� * MAX_ITEM
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
		pVtx[0].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
		pVtx[0].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

		pVtx[1].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
		pVtx[1].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
		pVtx[1].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

		pVtx[2].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
		pVtx[2].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
		pVtx[2].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

		pVtx[3].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
		pVtx[3].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
		pVtx[3].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

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
		pVtx[0].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 0.0f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 1.0f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 0.0f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 1.0f), 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//============================================================================
//�A�C�e���̏I������
//============================================================================
void UninitItem(void)
{
	int nCntItemType;	//�A�C�e���̎�ނ��J�E���g����

	for (nCntItemType = 0; nCntItemType < NUM_ITEM; nCntItemType++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureItem[nCntItemType] != NULL)
		{
			g_pTextureItem[nCntItemType]->Release();
			g_pTextureItem[nCntItemType] = NULL;
		}
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffItem != NULL)
		{
			g_pVtxBuffItem->Release();
			g_pVtxBuffItem = NULL;
		}
	}
}

//============================================================================
//�A�C�e���̍X�V����
//============================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int nCntItem;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{//�A�C�e�����g�p����Ă���ꍇ

			//�O��̈ʒu��ۑ�
			g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;

			//�ʒu���X�V
			g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;
			g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

			//�A�C�e���̏d��
			g_aItem[nCntItem].move.y += 1.5f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�O�ʊO���[�v����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth / 2 < -ITEM_WIDTH / 2)
			{//�R�C���������̉�ʊO�ɏo���ꍇ
				g_aItem[nCntItem].pos.x = SCREEN_WIDTH + ITEM_WIDTH / 2;	//��ʉE���Ɉړ�
				g_aItem[nCntItem].posOld.x = SCREEN_WIDTH + ITEM_WIDTH;		//��ʉE���̏�����ɑO��ʒu��ݒ�
			}
			else if (g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth / 2 > SCREEN_WIDTH + ITEM_WIDTH / 2)
			{//�R�C�����E���̉�ʊO�ɏo���ꍇ1
				g_aItem[nCntItem].pos.x = -ITEM_WIDTH / 2;					//��ʍ����Ɉړ�
				g_aItem[nCntItem].posOld.x = -ITEM_WIDTH;					//��ʍ����̏�����ɑO��ʒu��ݒ�
			}
			if (g_aItem[nCntItem].pos.y >= SCREEN_HEIGHT)
			{//�R�C������ʉ��ɂ���ꍇ
				g_aItem[nCntItem].pos.y = SCREEN_HEIGHT;					//��ʉ����ɌŒ�
				g_aItem[nCntItem].move.y = 0.0f;
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�u���b�N�����蔻��̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			BLOCK_COLLISION blockCollision;
			g_aItem[nCntItem].pBlock = NULL;

			blockCollision = (ColisionBlock(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, &g_aItem[nCntItem].move, 
				g_aItem[nCntItem].fWidth, g_aItem[nCntItem].fHeight, &g_aItem[nCntItem].pBlock));
			{//�u���b�N�ɓ��������ꍇ�̏���
				if (blockCollision == BLOCK_COLLISION_TOP)
				{
					if (g_aItem[nCntItem].pBlock != NULL)
					{//�u���b�N�������Ă���ꍇ�A�v���C���[�Ɋ��������鏈��
						g_aItem[nCntItem].pos.y = g_aItem[nCntItem].pBlock->pos.y - g_aItem[nCntItem].pBlock->fHeight / 2;
						g_aItem[nCntItem].move.y = 0;
					}
				}
				else if (blockCollision != BLOCK_COLLISION_TOP)
				{
					g_aItem[nCntItem].pos.y += 1.0f;
				}
				if (blockCollision == BLOCK_COLLISION_LEFT)
				{//�A�C�e�����E�ɓ����Ă���ꍇ
					g_aItem[nCntItem].state = ITEM_STATE_TRUN_L;
				}
				else if(blockCollision == BLOCK_COLLISION_RIGHT)
				{//�A�C�e�������ɓ����Ă���ꍇ
					g_aItem[nCntItem].state = ITEM_STATE_TRUN_R;
				}
			}

			//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
			pVtx[0].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[0].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[0].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[1].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[1].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[1].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[2].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[2].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[2].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[3].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[3].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[3].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�A�j���[�V��������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			g_aItem[nCntItem].nCounterAnimBreak++;
			g_aItem[nCntItem].nPatternAnim++;

			if (g_aItem[nCntItem].type == ITEM_TYPE_COIN)
			{//�R�C���̏ꍇ
				if (g_aItem[nCntItem].nCounterAnimBreak == COIN_MAX_ANIMA * 2)
				{//�A�j���[�V������x�����鏈��
					g_aItem[nCntItem].nCounterAnim++;
					g_aItem[nCntItem].nCounterAnimBreak = 0;
				}
				if (g_aItem[nCntItem].nPatternAnim == COIN_MAX_ANIMA)
				{//�A�j���p�^�[�����ő�܂ŗ����ꍇ
					g_aItem[nCntItem].nPatternAnim = 0;
				}

				//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
				pVtx[0].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 0.0f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 1.0f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 0.0f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 1.0f), 1.0f);
			}
			else if (g_aItem[nCntItem].type == ITEM_TYPE_EFFECT)
			{//�G�t�F�N�g�̏ꍇ
				if (g_aItem[nCntItem].nCounterAnimBreak == EFFECT_MAX_ANIMA / 2)
				{//�A�j���[�V������x�����鏈��
					g_aItem[nCntItem].nCounterAnim++;
					g_aItem[nCntItem].nCounterAnimBreak = 0;
				}
				if (g_aItem[nCntItem].nPatternAnim == EFFECT_MAX_ANIMA)
				{//�A�j���p�^�[�����ő�܂ŗ����ꍇ
					g_aItem[nCntItem].nPatternAnim = 0;
				}
				//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
				pVtx[0].tex = D3DXVECTOR2((1.0f / EFFECT_ANIMX) * (g_aItem[nCntItem].nCounterAnim % EFFECT_MAX_ANIMA + 0.0f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((1.0f / EFFECT_ANIMX) * (g_aItem[nCntItem].nCounterAnim % EFFECT_MAX_ANIMA + 1.0f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2((1.0f / EFFECT_ANIMX) * (g_aItem[nCntItem].nCounterAnim % EFFECT_MAX_ANIMA + 0.0f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((1.0f / EFFECT_ANIMX) * (g_aItem[nCntItem].nCounterAnim % EFFECT_MAX_ANIMA + 1.0f), 1.0f);
			}

			if (g_aItem[nCntItem].type == ITEM_TYPE_WHITE)
			{
				g_aItem[nCntItem].move.x = 0.0f;
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�A�C�e���̏�ԑJ�ڏ���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			switch (g_aItem[nCntItem].state)
			{//�A�C�e����Ԃɂ�邻�ꂼ��̏���
			case ITEM_STATE_NORMAL:
				g_aItem[nCntItem].bUse = true;		//�g�p��Ԃɐ^��Ԃ�
				break;
			case ITEM_STATE_TRUN_R:		
				g_aItem[nCntItem].move.x = 1.0f;
				break;
			case ITEM_STATE_TRUN_L:
				g_aItem[nCntItem].move.x = -1.0f;
				break;
			case ITEM_STATE_DELETE:
				//�A�C�e����������b���J�E���g
				g_aItem[nCntItem].nDeleteItemCounter++;
				g_aItem[nCntItem].type = ITEM_TYPE_EFFECT;

				if (g_aItem[nCntItem].nDeleteItemCounter == 20)
				{//�w��̕b���ɂȂ����ꍇ
					DeleteItem(nCntItem);
					AddWhite(1);
				}
				break;
			}
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();

	FADE fade;
	fade = GetFade();

	if (nDeleteCounter == MAX_ITEM)
	{//�A�C�e����S�Ď�����ꍇ�Q�[���N���A
		if (fade == FADE_NONE)
		{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
			//SetGameState(GAMESTATE_END);
		}
	}
}

//============================================================================
//�A�C�e���̕`�揈��
//============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntItem;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffItem,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{//�A�C�e�����g�p����Ă��� 
		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureItem[g_aItem[nCntItem].type]);

			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntItem * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}
}

//============================================================================
//�A�C�e���̐ݒ菈��
//============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEM_TYPE type, float Width, float Height)
{
	int nCntItem;
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{//�A�C�e�����g�p����Ă��Ȃ��ꍇ
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].move = move;
			g_aItem[nCntItem].setpos = g_aItem[nCntItem].pos;
			g_aItem[nCntItem].bUse = true;				//�g�p��Ԃ�true�ɂ���
			g_aItem[nCntItem].type = type;
			g_aItem[nCntItem].fWidth = Width;
			g_aItem[nCntItem].fHeight = Height;
			g_aItem[nCntItem].fAngle = atan2f(Width, Height);
			g_aItem[nCntItem].fLength = sqrtf(Width * Width + Height * Height) / 2.0f;
			
			//�R�C���̏���
			if (g_aItem[nCntItem].type == ITEM_TYPE_COIN)
			{
			 //�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4, 1.0f);
			}

			//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
			pVtx[0].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[0].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[0].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[1].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[1].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[1].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[2].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[2].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[2].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[3].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[3].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[3].pos.z = g_aItem[nCntItem].pos.z + 0.0f;
			break;		//for���𔲂��鏈��
		}
		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//============================================================================
//�A�C�e���̓����蔻��
//============================================================================
void ColisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	ITEM *pItem;
	int nCntItem;
	pItem = GetItem();

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].type != ITEM_TYPE_WHITE)
		{//�A�C�e���������Ȃ��Ă��Ȃ��ꍇ
			if (pItem->bUse == true)
			{//�v���C���[�ƃA�C�e���̓����蔻��
				if (pPos->y >= pItem->pos.y - pItem->fHeight
					&& pPos->y - fHeight <= pItem->pos.y
					&& pPos->x + fWidth >= pItem->pos.x + pItem->fWidth / 2
					&& pPos->x - fWidth <= pItem->pos.x - pItem->fWidth / 2)
				{
					pItem->state = ITEM_STATE_DELETE;
				}
			}
		}
		pItem++;
	}
}

//============================================================================
//�A�C�e���̍폜����
//============================================================================
void DeleteItem(int nCntItem)
{
	if (g_aItem[nCntItem].bUse == true)
	{
		nDeleteCounter++;				//�A�C�e���폜�����J�E���g
		g_aItem[nCntItem].bUse = false;	//�A�C�e��������
		AddScore(100);					//�X�R�A�����Z����
	}
}
//============================================================================
//�A�C�e���̎擾����
//============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];	//�G���̐擪�A�h���X��Ԃ�
}

//============================================================================
//�A�C�e���̔�e����
//============================================================================
void HitItem(int nCntItem)
{
	g_aItem[nCntItem].type = ITEM_TYPE_WHITE;
}