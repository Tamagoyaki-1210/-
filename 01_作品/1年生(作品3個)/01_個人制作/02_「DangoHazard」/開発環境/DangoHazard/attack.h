//============================================================================
//
//�U������̏���[item.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _ATTACK_H_						//���̃}�N����`������ĂȂ�������
#define _ATTACK_H_						//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_ATTACK			(255)	//�U������̍ő吔
#define IMAGE_ATTACK		(2)		//�U�����萔
#define ATTACK_WIDTH		(50)	//�U������̉���
#define ATTACK_HEIGHT		(70)	//�U������̏c��

//****************************************************************************
//�U������̎�ނ̒�`
//****************************************************************************
//�U������̎��
typedef enum
{
	ATTACK_TYPE_PLAYER = 0,	//�����蔻��(�v���C���[)
	ATTACK_TYPE_ENEMY,		//�����蔻��(�G�L����)
	ATTACK_TYPE_MAX
}ATTACK_TYPE;

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
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_ATTACK];
}X_ATTACK;

typedef struct
{
	D3DXMATRIX Worldmtx;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 rot;		//����
}ATTACK_PARTS;

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
	int nLife;				//����
	float fAngle;			//�p�x(�A�[�N�^���W�F���g)
	float fLength;			//����(�X�N�E�F�A���[�g)
	bool bUse;				//����
	ATTACK_TYPE type;		//���
	ATTACK_PARTS aParts[IMAGE_ATTACK]; //�p�[�c
}ATTACK;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitAttack(void);
void UninitAttack(void);
void UpdateAttack(void);
void DrawAttack(void);
bool AttackCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size);
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int life, ATTACK_TYPE type);
ATTACK *GetAttack(void);

#endif
