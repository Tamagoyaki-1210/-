//============================================================================
//
//��������[explosion.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_
#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define EXPLOSION_WIDTH			(200)	//�����摜�̉���
#define EXPLOSION_HEIGHT		(200)	//�����摜�̏c��
#define MAX_EXPLOSION			(128)	//�����̍ő吔
#define EXPLOSION_ANIM_CNT		(8)		//�����A�j���[�V�����J�E���^�[�̍ő��

//****************************************************************************
//�e�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR	col;	//�F
	D3DXVECTOR3 rot;	//����
	int nPatternAnim;	//�A�j���[�V�����p�^�[��
	bool bUse;			//�g�p����
}Explosion;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos,D3DXCOLOR col);

#endif