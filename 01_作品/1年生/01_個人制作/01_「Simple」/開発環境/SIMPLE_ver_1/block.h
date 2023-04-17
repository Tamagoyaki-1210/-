//============================================================================
//
//�u���b�N�̏���[block.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define NUM_BLOCK			(5)		//�u���b�N�̎��
#define MAX_BLOCK			(128)	//�u���b�N�̍ő吔
#define BLOCK_WIDTH			(40)	//�ʏ�u���b�N�̉��T�C�Y
#define BLOCK_HEIGHT		(40)	//�ʏ�u���b�N�̏c�T�C�Y
#define BLOCK_NONE_SET		(2)		//��ʊO�u���b�N�̌�

//****************************************************************************
//�u���b�N�̎��
//****************************************************************************
typedef enum
{
	BLOCK_TYPE_NORMAL = 0,	//�ʏ�u���b�N
	BLOCK_TYPE_EVADE,		//���蔲���u���b�N
	BLOCK_TYPE_MOVE,		//�ړ��u���b�N
	BLOCK_TYPE_NONE,		//��ʊO�u���b�N
	BLOCK_TYPE_WHITE,		//�����u���b�N
	BLOCK_TYPE_MAX
}BLOCK_TYPE;

typedef enum
{
	BLOCK_STATE_NORMAL = 0,
	BLOCK_STATE_WHITE,
	BLOCK_STATE_MAX
}BLOCK_STATE;

typedef enum
{
	BLOCK_SIDE_C = 0,	//�^�񒆃u���b�N
	BLOCK_SIDE_L,		//�����u���b�N
	BLOCK_SIDE_R,		//�E���u���b�N
	BLOCK_SIDE_MAX
}BLOCK_SIDE;

typedef enum
{
	BLOCK_COLLISION_NONE = 0,
	BLOCK_COLLISION_TOP,
	BLOCK_COLLISION_BOTTOM,
	BLOCK_COLLISION_RIGHT,
	BLOCK_COLLISION_LEFT,
	BLOCK_COLLISION_MAX
}BLOCK_COLLISION;

typedef struct
{
	D3DXVECTOR3 pos;//�z�u�ʒu
	float fWidth;	//��
	float fHeight;	//����
}BLOCKINFO;
//****************************************************************************
//�u���b�N�\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 rot;	//��]
	D3DXCOLOR	col;	//�F
	D3DXVECTOR3 posOld;	//�O��̈ʒu
	D3DXVECTOR3 setpos;	//�ݒu�ʒu
	BLOCK_TYPE type;	//���
	BLOCK_STATE state;	//��ԊǗ�
	BLOCK_SIDE side;	//��
	float fWidth;		//��
	float fHeight;		//����
	float fAngle;		//�p�x(�A�[�N�^���W�F���g)
	float fLength;		//����(�X�N�E�F�A���[�g)
	bool bUse;			//�g�p����
}BLOCK;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos,BLOCK_TYPE type, BLOCK_SIDE side,int set,int cur);
BLOCK_COLLISION ColisionBlock(D3DXVECTOR3 *pPos,D3DXVECTOR3 *pPosOld,D3DXVECTOR3 *pMove,float fWidth,float fHeight,BLOCK **pBlock);
BLOCK *GetBlock(void);
void HitBlock(int nCntBlock);

#endif
