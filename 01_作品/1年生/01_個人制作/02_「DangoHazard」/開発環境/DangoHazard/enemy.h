//============================================================================
//
//�G�L�����̏���[enemy.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _ENEMY_H_						//���̃}�N����`������ĂȂ�������
#define _ENEMY_H_						//2�A�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define MAX_ENEMY			(400)	//�G�L�����̍ő吔
#define MAX_ENEMY_PARTS		(1)		//�G�L�����̃p�[�c��
#define IMAGE_ENEMY			(2)		//�G�L������
#define ENEMY_ATTACK		(6)		//�G�L�����̍U�����莝������
#define ENEMY_SPEAD			(1.5)		//�G�L�����̈ړ����x�{��
#define ENEMY_BATTLE_RANGE	(100)	//�G�L�����̐퓬�˒�
#define ENEMY_ATTACK_RANGE	(13)	//�G�L�����̍U���J�n�˒�
#define ENEMY_WIDTH			(50)	//�G�L�����̉���
#define ENEMY_HEIGHT		(70)	//�G�L�����̏c��

//****************************************************************************
//�G�L�����̎�ނ̒�`
//****************************************************************************
//�G�L�����̎��
typedef enum
{
	ENEMY_TYPE_POTATE = 0,	//���f��(��c�q)
	ENEMY_TYPE_PUNPKIN,		//���f��(��Z�c�q)
	ENEMY_TYPE_MAX
}ENEMY_TYPE;

//�G�L�����̃��[�V�������
typedef enum
{
	ENEMY_MOTION_NUTRAL = 0,	//�j���[�g�������[�V����
	ENEMY_MOTION_WORK,			//�������[�V����
	ENEMY_MOTION_ATTACK,		//�U�����[�V����
	ENEMY_MOTION_DAMEGE,		//��e���[�V����
	ENEMY_MOTION_MAX
}ENEMY_MOTION;

//****************************************************************************
//�\���̒�`
//****************************************************************************
typedef struct
{
	LPD3DXMESH pMesh;		//�G�L�����f�[�^�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;	//���_�o�b�t�@�ւ̃|�C���^
	DWORD nNumMat;			//�G�L�����}�e���A�����̌�
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	LPDIRECT3DTEXTURE9 g_pTexture[IMAGE_ENEMY];
}X_ENEMY;

typedef struct
{
	D3DXMATRIX Worldmtx;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 rot;		//����
}ENEMY_PARTS;

typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 posOld;		//���_���W
	D3DXVECTOR3 move;		//����
	D3DXVECTOR3 nor;		//�@���x�N�g��
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 Destrot;	//�����ړI�l
	D3DXVECTOR3 vec;		//�x�N�g���ϐ�
	D3DXVECTOR3 vec1;		//�x�N�g���ϐ�(1�Œ�)
	D3DXMATRIX Worldmtx;	//���[���h�}�g���b�N�X
	D3DCOLOR col;			//���_�J���[
	int nIndxShadowModel;	//�e�̃C���f�b�N�X�ԍ�
	int nLife;
	float fAngle;			//�p�x(�A�[�N�^���W�F���g)
	float fLength;			//����(�X�N�E�F�A���[�g)
	float fFlame;			//�t���[����
	float fVecCnt;		//�G�L�����Ǐ]�t���[����
	bool bLoop;				//���[�v����
	bool bUse;				//����
	bool bDamege;			//��e����
	bool bBattle;			//�퓬����
	ENEMY_MOTION motion;	//���[�V�������
	ENEMY_TYPE type;		//���
	ENEMY_PARTS aParts[MAX_ENEMY_PARTS];	//�p�[�c
}ENEMY;


//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
bool EnemyCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type);
ENEMY *GetEnemy(void);

#endif

