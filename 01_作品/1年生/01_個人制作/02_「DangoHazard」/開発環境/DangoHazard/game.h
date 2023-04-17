//============================================================================
//
//�Q�[����ʂ̏���[game.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define GAME_WIDTH		(SCREEN_WIDTH)	//�^�C�g���̉��T�C�Y
#define GAME_HEIGHT		(SCREEN_HEIGHT)	//�^�C�g���̏c�T�C�Y

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//��]
	D3DXCOLOR	col;	//�F
}Game;

typedef enum
{
	GAMESTATE_NONE = 0,		//�������ĂȂ����
	GAMESTATE_NORMAL,		//�ʏ���
	GAMESTATE_GAMECLEAR,	//�Q�[���N���A���
	GAMESTATE_GAMEOVER,		//�Q�[���I�[�o�[���
	GAMESTATE_MAX
}GAMESTATE;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);

#endif