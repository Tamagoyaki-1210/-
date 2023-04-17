//============================================================================
//
//�A�C�e���̏���[item.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"

//****************************************************************************
//�}�N����`
//****************************************************************************
#define NUM_ITEM			(3)		//�A�C�e���̎��
#define MAX_ITEM			(4)		//�A�C�e���̍ő吔
#define ITEM_WIDTH			(30)	//�ʏ�A�C�e���̉��T�C�Y
#define ITEM_HEIGHT			(60)	//�ʏ�A�C�e���̏c�T�C�Y
#define COIN_MAX_ANIMA		(4)		//�R�C���̃A�j���p�^�[����
#define COIN_ANIMX			(4)		//�R�C����X���A�j����
#define COIN_ANIMY			(1)		//�R�C����Y���A�j����
#define EFFECT_MAX_ANIMA	(8)		//�R�C���̃A�j���p�^�[����
#define EFFECT_ANIMX		(8)		//�R�C����X���A�j����
#define EFFECT_ANIMY		(1)		//�R�C����Y���A�j����

//****************************************************************************
//�A�C�e���̎��
//****************************************************************************
typedef enum
{
	ITEM_TYPE_COIN = 0,	//�R�C��
	ITEM_TYPE_EFFECT,
	ITEM_TYPE_WHITE,
	ITEM_TYPE_MAX
}ITEM_TYPE;

typedef enum
{
	ITEM_STATE_NORMAL = 0,
	ITEM_STATE_TRUN_R,
	ITEM_STATE_TRUN_L,
	ITEM_STATE_DELETE,
	ITEM_STATE_MAX
}ITEM_STATE;
//****************************************************************************
//�A�C�e���\���̂̒�`
//****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//��]
	D3DXCOLOR	col;		//�F
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 setpos;		//�ݒu�ʒu
	ITEM_STATE state;		//�o��
	ITEM_TYPE type;			//���
	int nCounterAnim;		//�A�j���[�V�����J�E���^�[
	int nCounterAnimBreak;	//�A�j���[�V�����J�E���^�[�u���[�L
	int nPatternAnim;		//�A�j���[�V�����p�^�[���ԍ�
	int nDeleteItemCounter;	//������b��
	float fWidth;			//��
	float fHeight;			//����
	float fAngle;			//�p�x(�A�[�N�^���W�F���g)
	float fLength;			//����(�X�N�E�F�A���[�g)
	bool bUse;				//�g�p����
	BLOCK *pBlock;			//�u���b�N�̏��
}ITEM;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEM_TYPE type, float Width, float Height);
void ColisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void DeleteItem(int nCntItem);
ITEM *GetItem(void);
void HitItem(int nCntItem);

#endif

