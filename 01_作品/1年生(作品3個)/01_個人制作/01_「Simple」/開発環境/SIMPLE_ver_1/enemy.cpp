//============================================================================
//
//�G�̏���[enemy.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "enemy.h"			//�쐬����enemy.h���C���N���[�h����
#include "bullet.h"			//�쐬����bullet.h���C���N���[�h����
#include "explosion.h"		//�쐬����explosion.h���C���N���[�h����
#include "score.h"			//�쐬����score.h���C���N���[�h����
#include "player.h"			//�쐬����player.h���C���N���[�h����
#include "game.h"			//�쐬����game.h���C���N���[�h����
#include "arrow.h"		//�쐬����life.h���C���N���[�h����
#include "sound.h"

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEnemy[NUM_ENEMY] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
ENEMY g_aEnemy[MAX_ENEMY];	//�G�̏��
int g_nDown;				//�G�̌��j��
int g_nPop;					//�G�̏o����
float g_fLengthEnemy;		//�Ίp���̒���
float g_fAngleEnemy;		//�Ίp���̊p�x

//============================================================================
//�G�̏���������
//============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//�G�̑Ίp���̒������Z�o����
	g_fLengthEnemy = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) / 2.0f;

	//�G�̑Ίp���̊p�x���Z�o����
	g_fAngleEnemy = atan2f(ENEMY_WIDTH, ENEMY_HEIGHT);

	g_nDown = 0;
	g_nPop = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���G1�̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\enemy000.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureEnemy[ENEMY_TYPE_1]
	);
	//�e�N�X�`���G2�̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\enemy001.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureEnemy[ENEMY_TYPE_2]
	);
	//�e�N�X�`���G3�̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\enemy002.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureEnemy[ENEMY_TYPE_3]
	);
	//�e�N�X�`���G4�̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\enemy003.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureEnemy[ENEMY_TYPE_4]
	);

	//�ړ����e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureEnemy[ENEMY_TYPE_WHITE]
	);

	//�S�G�̏��̏�����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�\���̂̏���������
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].type = ENEMY_TYPE_1;
		g_aEnemy[nCntEnemy].state = ENEMY_STATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = MAX_ENEMY_LIFE;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bMove = false;
		g_aEnemy[nCntEnemy].bHide = false;
		g_aEnemy[nCntEnemy].pBlock = NULL;							//�u���b�N����NULL����
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);
	//���_���̐ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
		pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
		pVtx[0].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

		pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
		pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
		pVtx[1].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

		pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
		pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
		pVtx[2].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

		pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
		pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
		pVtx[3].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

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

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//============================================================================
//�G�̏I������
//============================================================================
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		//�S�G�̃e�N�X�`���j��
		if (g_pTextureEnemy[nCntEnemy] != NULL)
		{
			g_pTextureEnemy[nCntEnemy]->Release();
			g_pTextureEnemy[nCntEnemy] = NULL;
		}

		//���_�o�b�t�@�̔j��
		if (g_pVtxBuffEnemy != NULL)
		{
			g_pVtxBuffEnemy->Release();
			g_pVtxBuffEnemy = NULL;
		}
	}
}

//============================================================================
//�G�̍X�V����
//============================================================================
void UpdateEnemy(void) 
{
	int nCntEnemy;
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�v���C���[�ƏՓ˂������̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Player *player;	//�v���C���[���ւ̃|�C���^
	//�v���C���[���̎擾
	player = GetPlayer();

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G�L�������g�p����Ă���ꍇ�̏���

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�G�̉B��鏈��
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (ARROW_WIDTH * 1.5>= g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2
				|| SCREEN_WIDTH - ARROW_WIDTH * 1.5 <= g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH)
			{
				if (ARROW_HEIGHT * 1.5 >= g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2
					|| SCREEN_HEIGHT - ARROW_HEIGHT * 1.5 <= g_aEnemy[nCntEnemy].pos.y)
				{
					g_aEnemy[nCntEnemy].bHide = true;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�G�̃��[�v����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					if (g_aEnemy[nCntEnemy].pos.x <= -ENEMY_WIDTH)
					{
						g_aEnemy[nCntEnemy].pos.y = ARROW_HEIGHT * 2 - 1;
						g_aEnemy[nCntEnemy].posOld.y = ARROW_HEIGHT * 2 - 3;
					}
					else if (g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH + ENEMY_WIDTH)
					{
						g_aEnemy[nCntEnemy].pos.y = ARROW_HEIGHT * 2 - 1;
						g_aEnemy[nCntEnemy].posOld.y = ARROW_HEIGHT * 2 - 3;
					}
				}
			}
			else
			{
				g_aEnemy[nCntEnemy].bHide = false;
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�G�̃v���C���[�ڐG������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].state != ENEMY_STATE_WHITE && g_aEnemy[nCntEnemy].bHide == false)
			{
				if (player->pos.x + PLAYER_WIDTH / 2 >= g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2
					&& player->pos.x - PLAYER_WIDTH / 2 <= g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2
					&& player->pos.y - PLAYER_HEIGHT <= g_aEnemy[nCntEnemy].pos.y
					&& player->pos.y >= g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT)
				{
					if (player->state != PLAYERSTATE_DAMAGE)
					{
						HitPlayer(1);
					}
				}
			}
			if (g_aEnemy[nCntEnemy].bMove == false)
			{//�G�������Ȃ��ꍇ�̏���
				g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�u���b�N�����蔻��̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			BLOCK_COLLISION blockCollision;

			blockCollision = (ColisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move,
				ENEMY_WIDTH, ENEMY_HEIGHT, &g_aEnemy[nCntEnemy].pBlock));
			{//�u���b�N�ɓ��������ꍇ�̏���
				if (blockCollision == BLOCK_COLLISION_TOP)
				{
					if (g_aEnemy[nCntEnemy].pBlock != NULL)
					{//�u���b�N�������Ă���ꍇ�A�v���C���[�Ɋ��������鏈��
						g_aEnemy[nCntEnemy].pos.y = g_aEnemy[nCntEnemy].pBlock->pos.y - g_aEnemy[nCntEnemy].pBlock->fHeight / 2;
						g_aEnemy[nCntEnemy].move.y = 0;
					}
				}
				if (blockCollision == BLOCK_COLLISION_LEFT)
				{//�u���b�N�̍����ɓ��������ꍇ
					g_aEnemy[nCntEnemy].state = ENEMY_STATE_TRUN_L;
				}
				else if (blockCollision == BLOCK_COLLISION_RIGHT)
				{//�u���b�N�̉E���ɓ��������ꍇ
					g_aEnemy[nCntEnemy].state = ENEMY_STATE_TRUN_R;
				} 
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�G�̈ړ��ʔ��f����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (4 * nCntEnemy);	//���_�f�[�^�̃|�C���^��4���i�߂�

			//�O��̈ʒu��ۑ�
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			//�ړ��ʔ��f
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			//�G�̏d��
			g_aEnemy[nCntEnemy].move.y += 0.1f;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[0].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[1].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[2].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[3].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�G����ʊO�ɏo�����̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].pos.x <= -ENEMY_WIDTH)
			{
				g_aEnemy[nCntEnemy].pos.x = SCREEN_WIDTH + ENEMY_WIDTH / 2;
				g_aEnemy[nCntEnemy].posOld.x = SCREEN_WIDTH + ENEMY_WIDTH;
			}
			else if(g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH + ENEMY_WIDTH)
			{
				g_aEnemy[nCntEnemy].pos.x = -ENEMY_WIDTH / 2;
				g_aEnemy[nCntEnemy].posOld.x = -ENEMY_WIDTH;
			}
			if (g_aEnemy[nCntEnemy].pos.y >= SCREEN_HEIGHT)
			{
				g_aEnemy[nCntEnemy].pos.y = SCREEN_HEIGHT;
				g_aEnemy[nCntEnemy].move.y = 0.0f;
			}
			
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�G�̏�ԊǗ�����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			switch (g_aEnemy[nCntEnemy].state)
			{//�G�̏�Ԃɂ�邻�ꂼ��̏���
			case ENEMY_STATE_NORMAL:
				break;
			case ENEMY_STATE_DAMEGE:
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMY_STATE_NORMAL;

					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				break;
			case ENEMY_STATE_WHITE:
				g_nDown++;		//�G�@�I�����𑝂₷
				g_aEnemy[nCntEnemy].type = ENEMY_TYPE_WHITE;
				g_aEnemy[nCntEnemy].bMove = false;
				break;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�G�̎�ޕʍs������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			case ENEMY_STATE_TRUN_R:
				if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_1)
				{//�G1�̏���()
					g_aEnemy[nCntEnemy].pos.x += ENEMY1_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_2)
				{//�G2�̏���()
					g_aEnemy[nCntEnemy].pos.x += ENEMY2_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_3)
				{//�G3�̏���()
					g_aEnemy[nCntEnemy].pos.x += ENEMY3_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_4)
				{//�G4�̏���()
					g_aEnemy[nCntEnemy].pos.x += ENEMY4_SPEAD;
				}
				break;
			case ENEMY_STATE_TRUN_L:
				if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_1)
				{//�G1�̏���()
					g_aEnemy[nCntEnemy].pos.x -= ENEMY1_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_2)
				{//�G2�̏���()
					g_aEnemy[nCntEnemy].pos.x -= ENEMY2_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_3)
				{//�G3�̏���()
					g_aEnemy[nCntEnemy].pos.x -= ENEMY3_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_4)
				{//�G4�̏���()
					g_aEnemy[nCntEnemy].pos.x -= ENEMY4_SPEAD;
				}
				break;
			}
			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffEnemy->Unlock();
		}
	}
	if (g_nDown == MAX_ENEMY)
	{//�G���S�����������̏���
		//SetGameState(GAMESTATE_END);
		g_nDown = 0;
	}
}

//============================================================================
//�G�̕`�揈��
//============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffEnemy,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//�G���g�p����Ă���

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCntEnemy].type]);
			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntEnemy * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}
}

//============================================================================
////�G�̐ݒ菈��
//============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE type)
{
	int nCntEnemy;
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//�G�����j���ꂽ�ꍇ

			//�G�̏��̐ݒ�
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].move = move;
			g_aEnemy[nCntEnemy].type = type;
			g_aEnemy[nCntEnemy].state = ENEMY_STATE_NORMAL;
			g_aEnemy[nCntEnemy].nCounterState = 0;
			g_aEnemy[nCntEnemy].bUse = true;
			g_aEnemy[nCntEnemy].bMove = true;
			g_aEnemy[nCntEnemy].nLife = MAX_ENEMY_LIFE;

			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (4 * nCntEnemy);	//���_�f�[�^�̃|�C���^��4���i�߂�

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[0].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[1].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[2].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[3].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;
			break;

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffEnemy->Unlock();
		}
	}
}

//============================================================================
////�G�̎擾����
//============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];	//�G���̐擪�A�h���X��Ԃ�
}

//============================================================================
////�G�̔�e����
//============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	//�e�������������A�G�Ƀ_���[�W
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	
	if (g_aEnemy[nCntEnemy].nLife > 0)
	{//�G�̃��C�t��0����̏ꍇ�̏���
		g_aEnemy[nCntEnemy].state = ENEMY_STATE_DAMEGE;
	}
	else
	{//�̗͂�0�ȉ��̏ꍇ
		g_aEnemy[nCntEnemy].state = ENEMY_STATE_WHITE;

		if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_1)
		{
			AddScore(200);
		}
		else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_2)
		{
			AddScore(400);
		}
		else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_3)
		{
			AddScore(600);
		}
		else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_4)
		{
			AddScore(1200);
		}
	}
}