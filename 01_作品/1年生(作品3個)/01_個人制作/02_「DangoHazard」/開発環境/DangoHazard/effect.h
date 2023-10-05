//============================================================================
//
//�G�t�F�N�g�̏���[effect.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _EFFECT_H_						//���̃}�N����`������ĂȂ�������
#define _EFFECT_H_						//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_EFFECT				(255)	//�G�t�F�N�g�̍ő吔
#define EFFECT_LIFE				(100)	//�G�t�F�N�g�̎���
#define EFFECT_WIDTH			(10)	//�G�t�F�N�g�̉���
#define EFFECT_HEIGHT			(10)	//�G�t�F�N�g�̏c��

//****************************************************************************
//�G�t�F�N�g�̎�ނ̒�`
//****************************************************************************
//�G�t�F�N�g�̎��
typedef enum
{
	EFFECT_TYPE_BULLET = 0,	//�r���{�[�h(�e)
	EFFECT_TYPE_MAX
}EFFECT_TYPE;

//****************************************************************************
//�\���̒�`
//****************************************************************************
//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 rot;	//����
	D3DXMATRIX Worldmtx;//���[���h�}�g���b�N�X
	D3DCOLOR col;		//���_�J���[
	int life;			//�G�t�F�N�g�̎���
	bool bUse;
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
	EFFECT_TYPE type;
}EFFECT;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXVECTOR3 rot, int nLife, EFFECT_TYPE type);

#endif 
