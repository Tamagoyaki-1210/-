//============================================================================
//
//�J�����̏���[camera.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _CAMERA_H_						//���̃}�N����`������ĂȂ�������
#define _CAMERA_H_						//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define CAMERA_NEAR				(10)	//�J�����̃j�A�[
#define CAMERA_FAR				(3000)	//�J�����̃t�@�[
#define CAMERA_SPEAD			(2)		//�J�����̈ړ����x

//****************************************************************************
//�\���̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			//���_
	D3DXVECTOR3 posR;			//�����_
	D3DXVECTOR3 posVDest;		//�ړI�̎��_
	D3DXVECTOR3 posRDest;		//�ړI�̒����_
	D3DXVECTOR3 vecU;			//������x�N�g��
	D3DXVECTOR3 rot;			//����
	D3DXVECTOR3 rotDest;		//�ړI�̕���
	D3DXMATRIX mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X
	float fDistance;			//���_���璍���_�̋���
	bool bFollowUse;			//�Ǐ]�@�\����
}CAMERA;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
CAMERA *GetCamera(void);

#endif #pragma once
