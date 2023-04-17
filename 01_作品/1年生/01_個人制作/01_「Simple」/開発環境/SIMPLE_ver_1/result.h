//============================================================================
//
//���U���g��ʂ̏���[result.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_RESULT_IMAGE	(3)					//���U���g��ʂ̉摜�ő喇��
#define RESULT_BG_WIDTH		(SCREEN_WIDTH)		//�w�i�̉��T�C�Y
#define RESULT_BG_HEIGHT	(SCREEN_HEIGHT)		//�w�i�̏c�T�C�Y
#define RESULT_ENTER_WIDTH	(SCREEN_WIDTH / 4)	//ENTER�̉��T�C�Y
#define RESULT_ENTER_HEIGHT	(SCREEN_HEIGHT / 8)	//ENTER�̏c�T�C�Y
#define RESULT_LOGO_WIDTH	(SCREEN_WIDTH)		//���S�̉��T�C�Y
#define RESULT_LOGO_HEIGHT	(SCREEN_HEIGHT)		//���S�̏c�T�C�Y

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//��]
	D3DXCOLOR	col;	//�F
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
}Result;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif