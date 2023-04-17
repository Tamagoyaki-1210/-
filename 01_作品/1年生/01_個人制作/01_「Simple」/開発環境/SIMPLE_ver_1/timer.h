//============================================================================
//
//�^�C�}�[�̏���[timer.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_TIMER				(99999999)	//�^�C�}�[�̍ő吔
#define MAX_TIMER_DIGIT			(4)			//�^�C�}�[�̌���
#define COUNT_TIMER				(80)		//�^�C�}�[���J�E���g���鐔��
#define MINIUT_TIMER_DIGIT		(2)			//�^�C�}�[�̌���(��)
#define SECOND_TIMER_DIGIT		(2)			//�^�C�}�[�̌���(�b)
#define TIMER_BASE_NUMBER		(10)		//�i��
#define TIMER_WIDTH				(40)		//�^�C�}�[�̉��T�C�Y
#define TIMER_HEIGHT			(60)		//�^�C�}�[�̏c�T�C�Y

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
}TIMER;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
void SetTimer(int nTimer);
void CounterTimer(int nValue);

#endif