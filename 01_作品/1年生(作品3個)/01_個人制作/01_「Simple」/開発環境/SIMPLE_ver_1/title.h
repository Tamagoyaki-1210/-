//============================================================================
//
//�^�C�g���̏���[title.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_TITLE_IMAGE	(3)						//�^�C�g���̉摜�ő喇��
#define TITLE_BG_WIDTH		(SCREEN_WIDTH)		//�w�i�̉��T�C�Y
#define TITLE_BG_HEIGHT		(SCREEN_HEIGHT)		//�w�i�̏c�T�C�Y
#define TITLE_ENTER_WIDTH	(SCREEN_WIDTH / 4)	//ENTER�̉��T�C�Y
#define TITLE_ENTER_HEIGHT	(SCREEN_HEIGHT / 8)	//ENTER�̏c�T�C�Y
#define TITLE_LOGO_WIDTH	(SCREEN_WIDTH)		//���S�̉��T�C�Y
#define TITLE_LOGO_HEIGHT	(SCREEN_HEIGHT)		//���S�̏c�T�C�Y

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
}Title;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);

#endif