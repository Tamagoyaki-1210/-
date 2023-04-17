//============================================================================
//
//�Q�[���I�[�o�[�̏���[gameover.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_GAMEOVER_IMAGE	(3)						//�^�C�g���̉摜�ő喇��
#define GAMEOVER_BG_WIDTH		(SCREEN_WIDTH)		//�w�i�̉��T�C�Y
#define GAMEOVER_BG_HEIGHT		(SCREEN_HEIGHT)		//�w�i�̏c�T�C�Y
#define GAMEOVER_ENTER_WIDTH	(SCREEN_WIDTH / 4)	//ENTER�̉��T�C�Y
#define GAMEOVER_ENTER_HEIGHT	(SCREEN_HEIGHT / 8)	//ENTER�̏c�T�C�Y
#define GAMEOVER_LOGO_WIDTH		(SCREEN_WIDTH / 2)	//���S�̉��T�C�Y
#define GAMEOVER_LOGO_HEIGHT	(SCREEN_HEIGHT / 4)	//���S�̏c�T�C�Y

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 rot;	//����
	D3DXCOLOR	col;	//�F
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
}GAMEOVER;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitGameOver(void);
void UninitGameOver(void);
void UpdateGameOver(void);
void DrawGameOver(void);

#endif#pragma once
