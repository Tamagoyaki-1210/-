//============================================================================
//
//�e�`��̏���[shadow.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _SHADOW_H_						//���̃}�N����`������ĂȂ�������
#define _SHADOW_H_						//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_SHADOW				(1)		//�e�̍ő吔
#define SHADOW_WIDTH			(10)	//�e�摜�̉���
#define SHADOW_HEIGHT			(10)	//�e�摜�̏c��
//****************************************************************************
//�\���̒�`
//****************************************************************************
//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 rot;	//����
	D3DXMATRIX Worldmtx;//���[���h�}�g���b�N�X
	D3DCOLOR col;		//���_�J���[
	bool bUse;
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
}SHADOW;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void SetpositionShadow(int nIdxShadow, D3DXVECTOR3 pos ,float fLenght, D3DXCOLOR col);

#endif 