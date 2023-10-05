//============================================================================
//
//�r���{�[�h�`��̏���[wall.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _BILLBOARD_H_						//���̃}�N����`������ĂȂ�������
#define _BILLBOARD_H_						//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_BILLBOARD				(4)		//�r���{�[�h�̍ő�ݒu��
#define IMAGE_BILLBOARD				(2)		//�r���{�[�h�̉摜��
#define BILLBOARD_BULLET_WIDTH		(10)	//�r���{�[�h�̉���(�e)
#define BILLBOARD_BULLET_HEIGHT		(10)	//�r���{�[�h�̏c��(�e)
#define BILLBOARD_TREE_WIDTH		(25)	//�r���{�[�h�̉���(��)
#define BILLBOARD_TREE_HEIGHT		(30)	//�r���{�[�h�̏c��(��)

//****************************************************************************
//�r���{�[�h�̎�ނ̒�`
//****************************************************************************
//�r���{�[�h�̎��
typedef enum
{
	BILLBOARD_TYPE_BULLET = 0,	//�r���{�[�h(�e)
	BILLBOARD_TYPE_TREE,		//�r���{�[�h(��)
	BILLBOARD_TYPE_MAX
}BILLBOARD_TYPE;

//****************************************************************************
//�\���̒�`
//****************************************************************************
//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 nor;		//�@���x�N�g��
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX Worldmtx;	//���[���h�}�g���b�N�X
	D3DCOLOR col;			//���_�J���[
	int IndxShadow;			//�e�̃C���f�b�N�X
	float fAngle;			//�p�x(�A�[�N�^���W�F���g)
	float fLength;			//����(�X�N�E�F�A���[�g)
	bool bUse;				//�g�p����
	BILLBOARD_TYPE type;	//���
}BILLBOARD;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitBillBoard(void);
void UninitBillBoard(void);
void UpdateBillBoard(void);
void DrawBillBoard(void);
void SetBillBoard(D3DXVECTOR3 pos, D3DXVECTOR3 rot , BILLBOARD_TYPE type, float fAngle,float fLength);

#endif 