//============================================================================
//
//�A�C�e���̏���[item.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _ITEM_H_						//���̃}�N����`������ĂȂ�������
#define _ITEM_H_						//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_ITEM			(400)		//�A�C�e���̍ő吔
#define IMAGE_ITEM			(1)		//�A�C�e����
#define ITEM_WIDTH			(50)	//�A�C�e���̉���
#define ITEM_HEIGHT			(70)	//�A�C�e���̏c��

//****************************************************************************
//�A�C�e���̎�ނ̒�`
//****************************************************************************
//�A�C�e���̎��
typedef enum
{
	ITEM_TYPE_CORN = 0,	//���f��(��冋o)
	ITEM_TYPE_MAX
}ITEM_TYPE;

//****************************************************************************
//�\���̒�`
//****************************************************************************
typedef struct
{
	LPD3DXMESH pMesh;		//�f�[�^�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;	//���_�o�b�t�@�ւ̃|�C���^
	DWORD nNumMat;			//�}�e���A�����̌�
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_ITEM];
}X_ITEM;

typedef struct
{
	D3DXMATRIX Worldmtx;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 rot;		//����
}ITEM_PARTS;

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
	ITEM_TYPE type;		//���
	ITEM_PARTS aParts[IMAGE_ITEM]; //�p�[�c
}ITEM;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
bool ItemCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ITEM_TYPE type);
ITEM *GetItem(void);

#endif

