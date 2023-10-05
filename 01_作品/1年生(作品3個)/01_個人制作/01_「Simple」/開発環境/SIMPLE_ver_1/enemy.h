//============================================================================
//
//�G�̏���[enemy.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "block.h"		//�쐬����block.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define NUM_ENEMY				(5)		//�G�̎�ސ�
#define MAX_ENEMY				(16)	//�G�̍ő吔
#define ENEMY_WIDTH				(40)	//�S�G�̉��T�C�Y
#define ENEMY_HEIGHT			(40)	//�S�G�̏c�T�C�Y
#define MAX_ENEMY_LIFE			(1)	//�G�̍ő�̗�
#define ENEMY1_SPEAD			(2)		//�G1�̑���
#define ENEMY2_SPEAD			(4)		//�G2�̑���
#define ENEMY3_SPEAD			(6)		//�G3�̑���
#define ENEMY4_SPEAD			(8)		//�G4�̑���

//****************************************************************************
//�G�̎�ނ̒�`
//****************************************************************************
//�G�̎��
typedef enum
{
	ENEMY_TYPE_1 = 0,	//�G1
	ENEMY_TYPE_2,		//�G2
	ENEMY_TYPE_3,		//�G3
	ENEMY_TYPE_4,		//�G4
	ENEMY_TYPE_WHITE,	//�G�������
	ENEMY_TYPE_MAX
}ENEMYTYPE;			

//****************************************************************************
//�G�̏�Ԃ̒�`
//****************************************************************************
typedef enum
{
	ENEMY_STATE_NORMAL = 0,			//�ʏ���
	ENEMY_STATE_DAMEGE,				//�_���[�W����
	ENEMY_STATE_WHITE,				//�������
	ENEMY_STATE_TRUN_R,				//�E�ړ�
	ENEMY_STATE_TRUN_L,				//���ړ�
	ENEMY_STATE_MAX
}ENEMYSTATE;

//****************************************************************************
//�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXCOLOR	col;		//�F
	ENEMYSTATE state;		//���
	ENEMYTYPE type;			//���
	int nCounterState;		//��ԊǗ��J�E���^�[
	int nLife;				//�̗�
	bool bUse;				//�g�p���Ă��邩�ǂ���
	bool bMove;				//�����邩�ǂ���
	bool bHide;				//�B�ꔻ��
	BLOCK *pBlock;			//�u���b�N�̏��
}ENEMY;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE nType);
ENEMY *GetEnemy(void);
void HitEnemy(int nCntEnemy,int nLife);

#endif
