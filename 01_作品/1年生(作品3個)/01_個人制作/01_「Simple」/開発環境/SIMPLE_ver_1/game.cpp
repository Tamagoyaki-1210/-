//============================================================================
//
//�Q�[����ʂ̏���[game.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "player.h"			//�쐬����player.h���C���N���[�h����
#include "input.h"			//�쐬����input.h���C���N���[�h����
#include "background.h"		//�쐬����background.h���C���N���[�h����
#include "score.h"			//�쐬����score.h���C���N���[�h����
#include "title.h"			//�쐬����title.h���C���N���[�h����
#include "game.h"			//�쐬����game.h���C���N���[�h����
#include "result.h"			//�쐬����result.h���C���N���[�h����
#include "fade.h"			//�쐬����fade.h���C���N���[�h����
#include "block.h"		//�쐬����block.h���C���N���[�h����
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "bullet.h"		//�쐬����bullet.h���C���N���[�h����
#include "explosion.h"	//�쐬����explosion.h���C���N���[�h����
#include "enemy.h"			//�쐬����enemy.h���C���N���[�h����
#include "life.h"		//�쐬����life.h���C���N���[�h����
#include "timer.h"		//�쐬����timer,h���C���N���[�h����
#include "arrow.h"		//�쐬����life.h���C���N���[�h����
#include "white.h"		//�쐬����white.h���C���N���[�h����
#include "sound.h"
#include <time.h>		//�^�C���֐����g�p���邽�߂ɕK�v
#include <stdlib.h>		//�����_���֐���system("cls")���g�p���邽�߂ɕK�v

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGame = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Game g_aGame;										//�Q�[����ʂ̏��

GAMESTATE g_gameState = GAMESTATE_NONE;				//�Q�[���̏��
int g_nCounterGameState = 0;						//��ԊǗ��J�E���^�[
													//�u���b�N���̃f�[�^
BLOCKINFO g_aBlockInfo[] =
{
	{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),BLOCK_WIDTH ,BLOCK_HEIGHT }
};

//============================================================================
//�Q�[����ʂ̏���������
//============================================================================
void InitGame(void)
{
	g_gameState = GAMESTATE_NORMAL;	//�ʏ��Ԃɂ���

	InitArrow();				//���̏���������
	InitItem();					//�A�C�e���̏���������
	InitEnemy();				//�G�̏���������
	InitPlayer();				//�v���C���[�̏���������
	InitBg();					//�w�i�̏���������
	InitBlock();				//�u���b�N�̏���������
	InitScore();				//�_���̏���������
	InitBullet();				//�e�̏���������
	InitExplosion();			//�����̏���������
	InitLife();					//���C�t�̏���������
	InitTimer();				//�^�C�}�[�̏���������
	InitWhite();
	
	int nCntSetBlock = 0;
	int nSetDataBlock = 0;
	float nHeightBlock = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�u���b�N�̏���(����)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	nSetDataBlock = 4;		//�Z�b�g�����
	nHeightBlock = 9.0f;	//�Z�b�g����u���b�N�ʒu�̍���
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_MOVE, BLOCK_SIDE_C, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 8;		//�Z�b�g�����
	nHeightBlock = 5.0f;	//�Z�b�g����u���b�N�ʒu�̍���
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_EVADE, BLOCK_SIDE_C, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�u���b�N�̏���(����)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	nSetDataBlock = 10;		//�Z�b�g�����
	nHeightBlock = 3.0f;	//�Z�b�g����u���b�N�ʒu�̍���
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2) ,0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_L,nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 8;		//�Z�b�g�����
	nHeightBlock = 7.0f;	//�Z�b�g����u���b�N�ʒu�̍���
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_L, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 10;		//�Z�b�g�����
	nHeightBlock = 11.0f;	//�Z�b�g����u���b�N�ʒu�̍���
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_L, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�u���b�N�̏���(�E��)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	nSetDataBlock = 10;		//�Z�b�g�����
	nHeightBlock = 3.0f;	//�Z�b�g����u���b�N�ʒu�̍���
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_R, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 8;		//�Z�b�g�����
	nHeightBlock = 7.0f;	//�Z�b�g����u���b�N�ʒu�̍���
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_R, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 10;		//�Z�b�g�����
	nHeightBlock = 11.0f;	//�Z�b�g����u���b�N�ʒu�̍���
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_R, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

//*******************************************************
//�A�C�e���̏���
//*******************************************************
//�R�C��
	int nCntSetItem;
	srand((unsigned int)time(0));
	for (nCntSetItem = 0; nCntSetItem < MAX_ITEM; nCntSetItem++)
	{
		SetItem
		(D3DXVECTOR3(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 0.0f),
			D3DXVECTOR3(1.0f, 0.0f, 0.0f),
			ITEM_TYPE_COIN, ITEM_WIDTH, ITEM_HEIGHT); //�ʒu,����,���,�傫��
	}
	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�G�̃��X�|�[���ꏊ����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	ENEMY *pEnemy;			//�G���ւ̃|�C���^
	pEnemy = GetEnemy();	//�G���̎擾
	int nCntSetEnemy;

	for (nCntSetEnemy = 0; nCntSetEnemy < 4; nCntSetEnemy++)
	{//�G�̏o���ʒu�ݒ�
		SetEnemy(D3DXVECTOR3(ENEMY_WIDTH + rand() % (SCREEN_WIDTH - ENEMY_WIDTH * 2), ENEMY_HEIGHT + rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT * 2),1.0f), D3DXVECTOR3(ENEMY1_SPEAD, 0.0f, 0.0f), ENEMY_TYPE_1);
		SetEnemy(D3DXVECTOR3(ENEMY_WIDTH + rand() % (SCREEN_WIDTH - ENEMY_WIDTH * 2), ENEMY_HEIGHT + rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT * 2), 1.0f), D3DXVECTOR3(ENEMY2_SPEAD, 0.0f, 0.0f), ENEMY_TYPE_2);
		SetEnemy(D3DXVECTOR3(ENEMY_WIDTH + rand() % (SCREEN_WIDTH - ENEMY_WIDTH * 2), ENEMY_HEIGHT + rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT * 2), 1.0f), D3DXVECTOR3(ENEMY3_SPEAD, 0.0f, 0.0f), ENEMY_TYPE_3);
		SetEnemy(D3DXVECTOR3(ENEMY_WIDTH + rand() % (SCREEN_WIDTH - ENEMY_WIDTH * 2), ENEMY_HEIGHT + rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT * 2), 1.0f), D3DXVECTOR3(ENEMY4_SPEAD, 0.0f, 0.0f), ENEMY_TYPE_4);
	}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���J�E���^�[�ݒu
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	SetWhite(nCntSetBlock + nCntSetItem + 16);
}

//============================================================================
//�Q�[����ʂ̏I������
//============================================================================
void UninitGame(void)
{
	UninitArrow();
	UninitItem();		//�A�C�e���̏I������
	UninitEnemy();		//�G�̏I������
	UninitPlayer();		//�v���C���[�I�u�W�F�N�g�̏I������
	UninitBg();			//�w�i�̏I������
	UninitBlock();		//�u���b�N�̏I������
	UninitScore();		//�X�R�A�̏I������
	UninitBullet();		//�e�̏I������
	UninitExplosion();	//�����̏I������
	UninitLife();		//���C�t�̏I������
	UninitTimer();		//�^�C�}�[�̏I������
	UninitWhite();

	//�e�N�X�`���̔j��
	if (g_pTextureGame != NULL)
	{
		g_pTextureGame->Release();
		g_pTextureGame = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGame != NULL)
	{
		g_pVtxBuffGame->Release();
		g_pVtxBuffGame = NULL;
	}
}

//============================================================================
//�Q�[����ʂ̍X�V����
//============================================================================
void UpdateGame(void)
{
	FADE fade;
	fade = GetFade();

	//UpdateArrow();
	UpdateItem();		//�A�C�e���̍X�V����
	UpdateEnemy();		//�G�̍X�V����
	UpdatePlayer();		//�v���C���[�I�u�W�F�N�g�̍X�V����
	UpdateBg();			//�w�i�̍X�V����
	UpdateBlock();		//�u���b�N�̍X�V����
	UpdateScore();		//�X�R�A�̍X�V����
	UpdateBullet();		//�e�̍X�V����
	UpdateExplosion();	//�����̍X�V����
	UpdateLife();		//���C�t�̍X�V����
	UpdateTimer();		//�^�C�}�[�̍X�V����
	UpdateWhite();

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:	//�ʏ���
		break;
	case GAMESTATE_GAMECLEAR:		//�I�����
  		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;	//�������Ă��Ȃ����
		}
		SetFade(MODE_RESULT);
		break;
	case GAMESTATE_GAMEOVER:		//�I�����
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;	//�������Ă��Ȃ����
		}
		SetFade(MODE_GAMEOVER);
		break;
	}
}

//============================================================================
//�Q�[����ʂ̕`�揈��
//============================================================================
void DrawGame(void)
{
	DrawArrow();
	DrawBg();			//�w�i�̕`�揈��
	DrawItem();			//�A�C�e���̕`�揈��
	DrawEnemy();		//�G�̕`�揈��
	DrawPlayer();		//�v���C���[�I�u�W�F�N�g�̕`�揈��
	DrawBlock();		//�u���b�N�̕`�揈��
	DrawScore();		//�X�R�A�̕`�揈��
	DrawBullet();		//�e�̕`�揈��
	DrawExplosion();	//�����̕`�揈��
	DrawLife();			//���C�t�̕`�揈��
	DrawTimer();		//�^�C�}�[�̕`�揈��
	DrawWhite();
}

//============================================================================
//�Q�[���̏�Ԑݒ菈��
//============================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//============================================================================
//�Q�[���̏�Ԏ擾����
//============================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}