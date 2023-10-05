//============================================================================
//
//���f���̏���[model.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _MODEL_H_						//���̃}�N����`������ĂȂ�������
#define _MODEL_H_						//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_MODEL			(3)		//���f���̍ő吔
#define IMAGE_MODEL			(3)		//X���f����
#define MODEL_WIDTH			(50)	//���f���̉���
#define MODEL_HEIGHT		(70)	//���f���̏c��
#define BULLET_INTERVAL		(10)	//�e���ˊԊu

//****************************************************************************
//���f���̎�ނ̒�`
//****************************************************************************
//���f���̎��
typedef enum
{
	MODEL_TYPE_ROCK = 0,	//���f��(��)
	MODEL_TYPE_STONE,		//���f��(��)
	MODEL_TYPE_TREE,		//���f��(��)
	MODEL_TYPE_MAX
}MODEL_TYPE;

//****************************************************************************
//�\���̒�`
//****************************************************************************
typedef struct
{
		D3DXVECTOR3 pos;		//���_���W
		D3DXVECTOR3 posOld;		//���_���W
		D3DXVECTOR3 move;		//����
		D3DXVECTOR3 nor;		//�@���x�N�g��
		D3DXVECTOR3 rot;		//����
		D3DXVECTOR3 Destrot;	//�����ړI�l
		D3DXMATRIX Worldmtx;	//���[���h�}�g���b�N�X
		D3DCOLOR col;			//���_�J���[
		int nIndxShadowModel;	//�e�̃C���f�b�N�X�ԍ�
		float fAngle;			//�p�x(�A�[�N�^���W�F���g)
		float fLength;			//����(�X�N�E�F�A���[�g)
		bool bUse;				//����
		MODEL_TYPE type;		//���
}MODEL;

//****************************************************************************
//���f���\���̒�`
//****************************************************************************
typedef struct
{
	LPD3DXMESH pMesh;		//���b�V���f�[�^�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;	//���_�o�b�t�@�ւ̃|�C���^
	DWORD nNumMat;			//���b�V���}�e���A�����̌�
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_MODEL];
}X_MODEL;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
bool ModelCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL_TYPE type);
MODEL *GetModel(void);

#endif
