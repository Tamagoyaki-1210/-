//============================================================================
//
//�v���C���[�̏���[player.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _PLAYER_H_		//���̃}�N����`������ĂȂ�������
#define _PLAYER_H_		//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define IMAGE_PLAYER		(1)		//�v���C���[�̉摜��
#define PLAYER_WIDTH		(50)	//�v���C���[�̉���
#define PLAYER_HEIGHT		(70)	//�v���C���[�̏c��
#define MAX_PLAYER_LIFE		(30)	//�v���C���[�̍ő�̗�
#define PLAYER_LIFE			(5)		//�v���C���[�̗̑�
#define PLAYER_ATTACK		(2)		//�v���C���[�̍U�����莝������
#define PLAYER_SPEAD		(3)		//�v���C���[�̈ړ����x�{��
#define MAX_PLAYER_PARTS	(4)		//�v���C���[�̃p�[�c��

//****************************************************************************
//�v���C���[��ނ̒�`
//****************************************************************************
//�v���C���[�̃e�N�X�`�����
typedef enum
{
	PLAYER_TEXTURE_MOFU = 0,	//�H�уe�N�X�`��
	PLAYER_TYPE_MAX
}PLAYER_TEXTURE;

//�v���C���[�̃��[�V�������
typedef enum
{
	PLAYER_MOTION_NUTRAL = 0,	//�j���[�g�������[�V����
	PLAYER_MOTION_WARK,			//�������[�V����
	PLAYER_MOTION_JUMP,			//�W�����v���[�V����
	PLAYER_MOTION_LANDING,		//���n���[�V����
	PLAYER_MOTION_ATTACK,		//�U�����[�V����
	PLAYER_MOTION_DAMEGE,		//��e���[�V����
	PLAYER_MOTION_MAX
}PLAYER_MOTION;

//****************************************************************************
//�\���̒�`
//****************************************************************************
typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_PLAYER];
	D3DXMATRIX Worldmtx;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 rot;		//����
	float fFrame;			//�t���[����
	int nIdxModelParent;	//�e���f���̃C���f�b�N�X
}PLAYER_PARTS;

typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 posOld;		//���_���W�O��ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 nor;		//�@���x�N�g��
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 Destrot;	//�����ړI�l
	D3DXMATRIX Worldmtx;	//���[���h�}�g���b�N�X
	D3DCOLOR col;			//���_�J���[
	int nLife;				//�̗�
	int nPower;				//��
	float fAngle;			//�p�x(�A�[�N�^���W�F���g)
	float fLength;			//����(�X�N�E�F�A���[�g)
	float fFlame;			//�t���[����
	float fAttack;			//�U���ҋ@����
	bool bUse;				//�g�p����
	bool bDamege;			//��e����
	bool bLoop;				//���[�v����
	bool bGround;			//�ڒn����̏ꍇ
	PLAYER_MOTION motion;	//���[�V�������
	PLAYER_TEXTURE tex;		//�e�N�X�`�����
	PLAYER_PARTS aParts[MAX_PLAYER_PARTS];
}PLAYER;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

#endif

