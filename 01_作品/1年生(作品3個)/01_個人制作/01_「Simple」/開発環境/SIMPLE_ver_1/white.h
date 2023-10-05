//============================================================================
//
//���̏���[white.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _WHITE_H_
#define _WHITE_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_SCORE_WHITE			(99999999)	//���̍ő吔
#define MAX_WHITE_DIGIT			(4)			//���̍ő包��
#define BASE_WHITE_NUMBER		(10)		//�i��
#define WHITE_WIDTH				(40)		//���̉��T�C�Y
#define WHITE_HEIGHT			(60)		//���̏c�T�C�Y

//****************************************************************************
//�e�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 rot;	//��]
	D3DXCOLOR	col;	//�F
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
}White;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitWhite(void);
void UninitWhite(void);
void UpdateWhite(void);
void DrawWhite(void);
void SetWhite(int nWhite);
void AddWhite(int nValue);

#endif