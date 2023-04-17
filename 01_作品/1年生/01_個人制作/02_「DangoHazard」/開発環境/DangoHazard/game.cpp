//============================================================================
//
//�Q�[����ʂ̏���[game.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "input.h"			//�쐬����input.h���C���N���[�h����
#include "title.h"			//�쐬����title.h���C���N���[�h����
#include "game.h"			//�쐬����game.h���C���N���[�h����
#include "result.h"			//�쐬����result.h���C���N���[�h����
#include "fade.h"			//�쐬����fade.h���C���N���[�h����

#include "timer.h"			//�쐬����timer.h���C���N���[�h����
#include "camera.h"			//�쐬����camera.h���C���N���[�h����
#include "light.h"			//�쐬����light.h���C���N���[�h����
#include "player.h"			//�쐬����player.h���C���N���[�h����
#include "model.h"			//�쐬����model.h���C���N���[�h����
#include "shadow.h"			//�쐬����shadow.h���C���N���[�h����
#include "wall.h"			//�쐬����wall.h���C���N���[�h����
#include "billboard.h"		//�쐬����billboard.h���C���N���[�h����
#include "meshfield.h"		//�쐬����meshfield.h���C���N���[�h����
#include "meshcylinder.h"	//�쐬����meshcylinder.h���C���N���[�h����
#include "effect.h"			//�쐬����effect.h���C���N���[�h����
#include "enemy.h"			//�쐬����enemy.h���C���N���[�h����
#include "item.h"			//�쐬����item.h���C���N���[�h����
#include "attack.h"			//�쐬����attack.h���C���N���[�h����
#include "score.h"			//�쐬����score.h���C���N���[�h����
#include "life.h"			//�쐬����life.h���C���N���[�h����
#include "background.h"	//�쐬����background.h���C���N���[�h����

#include <time.h>			//�^�C���֐����g�p���邽�߂ɕK�v
#include <stdlib.h>			//�����_���֐���system("cls")���g�p���邽�߂ɕK�v
	
//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGame = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Game g_aGame;										//�Q�[����ʂ̏��

GAMESTATE g_gameState = GAMESTATE_NONE;				//�Q�[���̏��
int g_nCounterGameState = 0;						//��ԊǗ��J�E���^�[
int g_SetItemCounter = 0;
int g_SetEnemyCounter = 0;


//============================================================================
//�Q�[����ʂ̏���������
//============================================================================
void InitGame(void)
{	
	//�ʏ��Ԃɂ���
	g_gameState = GAMESTATE_NORMAL;

	InitMeshField();	//���b�V���|���S���̏���������
	InitCamera();		//�J�����̏���������
	InitLight();		//���C�g�̏���������
	InitShadow();		//�e�̏���������
	InitPlayer();		//�v���C���[�̏���������
	InitEnemy();		//�G�L�����̏���
	InitItem();			//�A�C�e���̏���
	InitModel();		//���f���̏���������
	//InitWall();			//�ǂ̏���������
	//InitBillBoard();	//�r���{�[�h�̏���������
	InitAttack();		//�U������̏���������
	InitEffect();		//�G�t�F�N�g�̏���
	InitTimer();		//�^�C�}�[�̏���������
	InitScore();		//�X�R�A�̏���������
	InitLife();			//���C�t�̏���������
	InitBg();			//�w�i�̏���������

	srand((unsigned int)time(0));
	
	ENEMY *pEnemy;			//�G���ւ̃|�C���^
	pEnemy = GetEnemy();	//�G���̎擾
}

//============================================================================
//�Q�[����ʂ̏I������
//============================================================================
void UninitGame(void)
{
	UninitMeshField();		//���b�V���|���S���̏I������
	UninitCamera();			//�J�����̏I������
	UninitLight();			//���C�g�̏I������
	UninitPlayer();			//�v���C���[�̏I������
	UninitEnemy();			//�G�L�����̏I������
	UninitItem();			//�A�C�e���̏I������
	UninitModel();			//���f���̏I������
	UninitShadow();			//�e�̏I������
	//UninitWall();			//�ǂ̏I������
	//UninitBillBoard();		//�r���{�[�h�̏I������
	UninitAttack();			//�U���̏I������
	UninitEffect();			//�G�t�F�N�g�̏I������
	UninitTimer();			//�^�C�}�[�̏I������
	UninitScore();			//�X�R�A�̏I������
	UninitLife();			//���C�t�̏I������
	UninitBg();				//�w�i�̏I������

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

	UpdateMeshField();		//���b�V���|���S���̍X�V����
	UpdateCamera();			//�J�����̍X�V����
	UpdateLight();			//���C�g�̍X�V����
	UpdatePlayer();			//�v���C���[�̍X�V����
	UpdateEnemy();			//�G�L�����̍X�V����
	UpdateItem();			//�A�C�e���̍X�V����
	UpdateModel();			//���f���̍X�V����
	UpdateShadow();			//�e�̍X�V����
	//UpdateWall();			//�ǂ̍X�V����
	//UpdateBillBoard();		//�r���{�[�h�̍X�V����
	UpdateAttack();			//�U������̍X�V����
	UpdateEffect();			//�G�t�F�N�g�̍X�V����
	UpdateTimer();			//�^�C�}�[�̍X�V����
	UpdateScore();			//�X�R�A�̍X�V����
	UpdateLife();			//���C�t�̍X�V����
	UpdateBg();				//�w�i�̍X�V����
		
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
//*******************************************************
//�A�C�e���̏���
//*******************************************************
	g_SetItemCounter++;

	if (g_SetItemCounter == 50)
	{
		//�ʒu�A�p�x�A���
		SetItem(D3DXVECTOR3(250.0f - rand() % 500, 50.0f, 250.0f - rand() % 500),
			D3DXVECTOR3(0.5f, -1.0f, 0.0f), ITEM_TYPE_CORN);
		g_SetItemCounter = 0;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�G�̃��X�|�[���ꏊ����
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	g_SetEnemyCounter++;

	if (g_SetEnemyCounter == 25)
	{
		//�ʒu�A�p�x�A���
		SetEnemy(
			D3DXVECTOR3(250.0f - rand() % 500, 50.0f, 250.0f - rand() % 500),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			//(ENEMY_TYPE)(rand() % IMAGE_ENEMY)
			ENEMY_TYPE_POTATE
		);

		if (rand() % 10 == 0)
		{
			//�ʒu�A�p�x�A���
			SetEnemy(
				D3DXVECTOR3(250.0f - rand() % 500, 50.0f, 250.0f - rand() % 500),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//(ENEMY_TYPE)(rand() % IMAGE_ENEMY)
				ENEMY_TYPE_PUNPKIN
			);
		}
		g_SetEnemyCounter = 0;
	}
}

//============================================================================
//�Q�[����ʂ̕`�揈��
//============================================================================
void DrawGame(void)
{
	SetCamera();		//�J�����̐ݒ菈��
	//DrawWall();			//�ǂ̕`�揈��
	DrawMeshField();	//���b�V���|���S���̕`�揈��
	//DrawBillBoard();	//�r���{�[�h�̕`�揈��
	DrawAttack();		//�U������̕`�揈��
	DrawPlayer();		//�v���C���[�̕`�揈��
	DrawEnemy();		//�G�L�����̕`�揈��
	DrawItem();			//�A�C�e���̕`�揈��
	DrawModel();		//���f���̕`�揈��
	DrawShadow();		//�e�̕`�揈��
	DrawEffect();		//�G�t�F�N�g�̕`�揈��
	DrawTimer();		//�^�C�}�[�̕`�揈��
	DrawScore();		//�X�R�A�̕`�揈��
	DrawLife();			//���C�t�̕`�揈��
	DrawBg();			//�w�i�̕`�揈��
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