//============================================================================
//
//�n�C�X�R�A�̏���[high_score.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _HIGH_SCORE_H_
#define _HIGH_SCORE_H_

#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_HIGH_SCORE			(99999999)	//�_���̍ő吔
#define HIGH_SCORE_WIDTH			(150)		//�_���̉��T�C�Y
#define HIGH_SCORE_HEIGHT			(100)		//�_���̏c�T�C�Y
#define FILENAME_DATA	"score,bin"	//�ǂݍ��ރt�@�C����

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
}HIGHSCORE;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitHighScore(void);
void UninitHighScore(void);
void UpdateHighScore(void);
void DrawHighScore(void);
void LoadHighScore(void);
void SaveHighScore(int nValue);

#endif