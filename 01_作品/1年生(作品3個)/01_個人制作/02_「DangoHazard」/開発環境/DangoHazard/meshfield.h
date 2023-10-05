//============================================================================
//
//���b�V���|���S���̏���[meshfield.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _MESHFIELD_H_						//���̃}�N����`������ĂȂ�������
#define _MESHFIELD_H_						//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MESHFIELD_WIDTH			(50)	//���b�V���|���S���̉���
#define MESHFIELD_HEIGHT		(50)	//���b�V���|���S���̏c��
#define MAX_MESHFIELD			(15)	//���b�V���̃|���S���ő吔
//****************************************************************************
//�\���̒�`
//****************************************************************************
//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DXVECTOR3 rot;	//����
	D3DXMATRIX Worldmtx;//���[���h�}�g���b�N�X
	D3DCOLOR col;		//���_�J���[
	int nMeshX;			//x���̌�
	int nMeshZ;			//z���̌�
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
}MESHFIELD;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
bool FieldCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move);

#endif 