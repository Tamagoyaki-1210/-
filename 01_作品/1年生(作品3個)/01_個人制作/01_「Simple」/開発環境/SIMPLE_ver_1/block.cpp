//============================================================================
//
//�u���b�N�̏���[block.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "block.h"		//�쐬����block.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "sound.h"

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBlock[NUM_BLOCK] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;			//���_�o�b�t�@�ւ̃|�C���^
BLOCK g_aBlock[MAX_BLOCK];								//�u���b�N�̏��

//============================================================================
//�u���b�N�̏���������
//============================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�ʏ폰�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block000.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureBlock[BLOCK_TYPE_NORMAL]
	);
	//���蔲�����e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block004.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureBlock[BLOCK_TYPE_EVADE]
	);
	//�ړ����e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block005.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureBlock[BLOCK_TYPE_MOVE]
	);
	//�ړ����e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureBlock[BLOCK_TYPE_NONE]
	);
	//�ړ����e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureBlock[BLOCK_TYPE_WHITE]
	);

	//�u���b�N�̏��̏�����
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//�\���̂̏���������
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏���������
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʂ̏���������
		g_aBlock[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����������������
		g_aBlock[nCntBlock].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�u���b�N�̏����z�F
		g_aBlock[nCntBlock].setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ݒu�ʒu�̏���������
		g_aBlock[nCntBlock].type = BLOCK_TYPE_NORMAL;					//�u���b�N�̎��
		g_aBlock[nCntBlock].state = BLOCK_STATE_NORMAL;					//�u���b�N�̏��
		g_aBlock[nCntBlock].fWidth = BLOCK_WIDTH;						//��
		g_aBlock[nCntBlock].fHeight = BLOCK_HEIGHT;						//����
		g_aBlock[nCntBlock].fAngle = atan2f(BLOCK_WIDTH, BLOCK_HEIGHT);
		g_aBlock[nCntBlock].fLength = sqrtf(BLOCK_WIDTH * BLOCK_WIDTH + BLOCK_HEIGHT * BLOCK_HEIGHT) / 2.0f;
		g_aBlock[nCntBlock].bUse = false;								//�u���b�N�̎g�p����
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_�� * MAX_BLOCK
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
		pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
		pVtx[0].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

		pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
		pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
		pVtx[1].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

		pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
		pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
		pVtx[2].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

		pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
		pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
		pVtx[3].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

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
	g_pVtxBuffBlock->Unlock();
}

//============================================================================
//�u���b�N�̏I������
//============================================================================
void UninitBlock(void)
{
	int nCntBlockType;	//�u���b�N�̎�ނ��J�E���g����

	for (nCntBlockType = 0; nCntBlockType < NUM_BLOCK; nCntBlockType++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBlock[nCntBlockType] != NULL)
		{
			g_pTextureBlock[nCntBlockType]->Release();
			g_pTextureBlock[nCntBlockType] = NULL;
		}
		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffBlock != NULL)
		{
			g_pVtxBuffBlock->Release();
			g_pVtxBuffBlock = NULL;
		}
	}
}

//============================================================================
//�u���b�N�̍X�V����
//============================================================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	int nCntBlock;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//�u���b�N���g�p����Ă��� 
		//�O��̈ʒu��ۑ�
			g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;

			//�ʒu���X�V
			g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
			g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

			if (g_aBlock[nCntBlock].type == BLOCK_TYPE_MOVE)
			{//�ړ��u���b�N�̏���
				if (g_aBlock[nCntBlock].pos.x >= g_aBlock[nCntBlock].setpos.x + 200.0f)
				{//�u���b�N���݈ʒu���w��̈ʒu(�E��)�ɗ������̏���
					g_aBlock[nCntBlock].move.x -= 1.0f;
				}
				else if (g_aBlock[nCntBlock].pos.x <= g_aBlock[nCntBlock].setpos.x - 200.0f)
				{//�u���b�N���݈ʒu���w��̈ʒu(����)�ɗ������̏���
					g_aBlock[nCntBlock].move.x += 1.0f;
				}
				if (g_aBlock[nCntBlock].pos.y >= g_aBlock[nCntBlock].setpos.y + 100.0f)
				{//�u���b�N���݈ʒu���w��̈ʒu(�㑤)�ɗ������̏���
					g_aBlock[nCntBlock].move.y -= 1.0f;
				}
				else if (g_aBlock[nCntBlock].pos.y <= g_aBlock[nCntBlock].setpos.y - 100.0f)
				{//�u���b�N���݈ʒu���w��̈ʒu(����)�ɗ������̏���
					g_aBlock[nCntBlock].move.y += 1.0f;
				}
			}
			//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[0].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[1].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[2].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[3].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//�u���b�N�̏�ԊǗ�����
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			switch (g_aBlock[nCntBlock].state)
			{//�G�̏�Ԃɂ�邻�ꂼ��̏���
			case BLOCK_STATE_NORMAL:
				g_aBlock[nCntBlock].bUse = true;
				break;
			case BLOCK_STATE_WHITE:
				g_aBlock[nCntBlock].type = BLOCK_TYPE_WHITE;
				g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			}
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//============================================================================
//�u���b�N�̕`�揈��
//============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBlock,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//�u���b�N���g�p����Ă��� 
		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].type]);

		 //�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntBlock * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}
}
//============================================================================
//�u���b�N�̐ݒ菈��(set = �ݒu��),(cur = �|�C���^���݈ʒu)
//============================================================================
void SetBlock(D3DXVECTOR3 pos, BLOCK_TYPE type, BLOCK_SIDE side, int set, int cur)
{
	int nCntBlock;
	float fSide;
	VERTEX_2D *pVtx = 0;			//���_���ւ̃|�C���^

	pVtx += cur * 4;		//�|�C���^�𓮂�������?

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = cur; nCntBlock < set + cur; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{//�u���b�N���g�p����Ă��Ȃ��ꍇ
			g_aBlock[nCntBlock].bUse = true;
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].type = type;
			g_aBlock[nCntBlock].side = side;
//****************************************************************************
//�u���b�N�T�C�h���Ƃ̏���
//****************************************************************************
			if (g_aBlock[nCntBlock].side == BLOCK_SIDE_C)
			{//�������u���b�N
				fSide = SCREEN_WIDTH / 2 + (BLOCK_WIDTH * (nCntBlock - cur)) - (BLOCK_WIDTH / 2 * set);
			}
			else if (g_aBlock[nCntBlock].side == BLOCK_SIDE_L)
			{//�����u���b�N
				fSide = BLOCK_WIDTH / 2 + (BLOCK_WIDTH * (nCntBlock - cur));
			}
			else if (g_aBlock[nCntBlock].side == BLOCK_SIDE_R)
			{//�E���u���b�N
				fSide = SCREEN_WIDTH - BLOCK_WIDTH / 2 - (BLOCK_WIDTH * (nCntBlock - cur));
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�u���b�N�^�C�v���Ƃ̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aBlock[nCntBlock].type == BLOCK_TYPE_MOVE)	//�ʏ�u���b�N
			{
				g_aBlock[nCntBlock].move.x = 1.0f;
			}
			g_aBlock[nCntBlock].pos.x = fSide;
			g_aBlock[nCntBlock].setpos = g_aBlock[nCntBlock].pos;

			//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[0].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[1].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[2].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[3].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;
		}
		pVtx += 4;											//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//============================================================================
//�u���b�N�̓����蔻��
//============================================================================
BLOCK_COLLISION ColisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight , BLOCK **pBlock)
{
	int nCntBlock;
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	BLOCK_COLLISION blockCollision = BLOCK_COLLISION_NONE;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//�u���b�N���g�p����Ă���ꍇ
			//�u���b�N�㑤�̏���
			if (pPosOld->y <= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2
				&& pPos->y >= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2
				&& g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 < pPos->x + fWidth / 2
				&& g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2 > pPos->x - fWidth / 2)
			{//�v���C���[�O��ʒu�̑����A�u���b�N�̓���荂���ʒu�ɂ���ꍇ(P�O���0�u.y <= B���݈ʒu.y)
				//�v���C���[���݈ʒu�̑����u���b�N���ɂ߂肱�񂾏ꍇ
				blockCollision = BLOCK_COLLISION_TOP;

				*pBlock = &g_aBlock[nCntBlock];	//�u���b�N�̃|�C���^�����鏈��
			}
			else if (g_aBlock[nCntBlock].type != BLOCK_TYPE_EVADE && g_aBlock[nCntBlock].type != BLOCK_TYPE_WHITE)
			{//���蔲���u���b�N�ȊO�̏ꍇ
				//�u���b�N�����̏���
				if (pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2
					&& pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2
					&& g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 < pPos->x + fWidth / 2
					&& g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2 > pPos->x - fWidth / 2)
				{//�v���C���[�O��ʒu�̓����A�u���b�N���݈ʒu�̉�����ʒu�̏ꍇ(P�O��ʒu.y >= B���݈ʒu.y)
					//�v���C���[�̓����A�u���b�N���ɂ߂肱�񂾏ꍇ
					 //�v���C���[���݈ʒu���u���b�N�̉��ɌŒ肷��
					pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2 + fHeight;
					pMove->y = 0;
				}
				//�u���b�N�����̏���
				else if (pPosOld->x + fWidth / 2 <= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2
					&& pPos->x + fWidth / 2 >= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2
					&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2
					&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2)
				{//�v���C���[�O��ʒu�E�����A�u���b�N���݈ʒu��荶�ɂ���ꍇ(P�O��ʒu.x <= B���݈ʒu.x)
				//�v���C���[���݈ʒu�E�����A�u���b�N���ɂ߂肱�񂾏ꍇ
				 //�v���C���[���݈ʒu���u���b�N�̍��ɌŒ肷��
					pPos->x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 - fWidth / 2;
					pMove->x = 0;

					blockCollision = BLOCK_COLLISION_LEFT;
				}
				//�u���b�N�E���̏���
				else if (pPosOld->x - fWidth / 2 >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2
					&& pPos->x - fWidth / 2 <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2
					&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2
					&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2)
				{//�v���C���[�O��ʒu�̍������A�u���b�N�̉E����荶�ɂ���ꍇ(P�O��ʒu.x >= B���݈ʒu.x)
					//�v���C���[�������A�u���b�N���ɂ߂肱�񂾏ꍇ
					//�v���C���[���݈ʒu���u���b�N�̉E�ɌŒ肷��
					pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2 + fWidth / 2;
					pMove->x = 0.0f;

					blockCollision = BLOCK_COLLISION_RIGHT;
				}
			}
			if (g_aBlock[nCntBlock].type == BLOCK_TYPE_MOVE)
			{//�ړ��u���b�N�̏���
			 //�c�ړ��u���b�N�̏���
				if (g_aBlock[nCntBlock].posOld.y - (g_aBlock[nCntBlock].fHeight / 2) >= pPosOld->y
					&& g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2) <= pPos->y
					&& pPos->x - (fWidth / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2)
					&& pPos->x + (fWidth / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2))
				{//�u���b�N�O��ʒu�̓����A�v���C���[�̑�����ʒu�̏ꍇ(B�O��ʒu.y >= P�O��ʒu.y)
				 //�u���b�N�̓����A�v���C���[���ɂ߂肱�񂾏ꍇ
				 //�v���C���[���݈ʒu���u���b�N�̓��ɌŒ肷��
					pPos->y = g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2);
					pMove->y = 0.0f;
					blockCollision = BLOCK_COLLISION_TOP;
					*pBlock = &g_aBlock[nCntBlock];	//�u���b�N�̃|�C���^�����鏈��
				}
				if (g_aBlock[nCntBlock].posOld.y + (g_aBlock[nCntBlock].fHeight / 2) <= pPosOld->y - fHeight
					&& g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2) >= pPos->y - fHeight
					&& pPos->x - (fWidth / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2)
					&& pPos->x + (fWidth / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2))
				{//�u���b�N�O��ʒu�̉������A�v���C���[�̓���荂�ʒu�̏ꍇ(B�O��ʒu.y <= P�O��ʒu.y)
				 //�u���b�N�̉������A�v���C���[���ɂ߂肱�񂾏ꍇ
				 //�v���C���[���݈ʒu���u���b�N�̉����ɌŒ肷��
					pPos->y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2 + fHeight);
					pMove->y = 0.0f;
				}
				//���ړ��u���b�N�̏���
				if (g_aBlock[nCntBlock].posOld.x - (g_aBlock[nCntBlock].fWidth / 2) >= pPosOld->x + fWidth / 2
					&& g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2) <= pPos->x + fWidth / 2
					&& g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2) < pPos->y
					&& g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2) > pPos->y - fHeight)
				{//�u���b�N�O��ʒu(����)���A�v���C���[�O��ʒu�̉E����荶�ɂ���ꍇ(B�O��ʒu.x >= P�O��ʒu.x)
				 //�u���b�N�̍������A�v���C���[�ɂ߂肱�񂾏ꍇ
					pPos->x = g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2) - fWidth / 2;
				}
				if (g_aBlock[nCntBlock].posOld.x + (g_aBlock[nCntBlock].fWidth / 2) <= pPosOld->x - fWidth / 2
					&& g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2) >= pPos->x - fWidth / 2
					&& g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2) < pPos->y
					&& g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2) > pPos->y - fHeight)
				{//�u���b�N�O��ʒu(�E��)���A�v���C���[�O��ʒu�̍������E�ɂ���ꍇ(B�O��ʒu.x <= P�O��ʒu.x)
				 //�u���b�N�̍������A�v���C���[�ɂ߂肱�񂾏ꍇ
					pPos->x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2) + fWidth / 2;
				}
			}
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
	return blockCollision;
}
//============================================================================
//�u���b�N�̎擾����
//============================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];	//�u���b�N���̐擪�A�h���X��Ԃ�
}

//============================================================================
//�u���b�N�̔�e����
//============================================================================
void HitBlock(int nCntBlock)
{
	g_aBlock[nCntBlock].state = BLOCK_STATE_WHITE;
}