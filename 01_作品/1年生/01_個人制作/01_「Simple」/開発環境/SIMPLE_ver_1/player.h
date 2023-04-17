//============================================================================
//
//�v���C���[�`��(�v���C���[000)�̏���[player.h] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#ifndef _POLYGON_H_		//���̃}�N����`������ĂȂ�������
#define _POLYGON_H_		//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "block.h"		//�쐬����block.h���C���N���[�h����

//****************************************************************************
//�}�N����`
//****************************************************************************
#define PLAYER_WIDTH			(50)	//�v���C���[�摜�̉���
#define PLAYER_HEIGHT			(70)	//�v���C���[�摜�̏c��
#define PLAYER_MAXJUMP			(2)		//�v���C���[�̃W�����v��
#define MAX_PLAYER_LIFE			(3)		//�v���C���[�̍ő僉�C�t
#define PLAYER_ANIMATION		(4)		//�v���C���[�̃A�j���p�^�[����
#define PLAYER_ANIMX			(4)		//�v���C���[��X���A�j����
#define PLAYER_ANIMY			(2)		//�v���C���[��Y���A�j����

//****************************************************************************
//�v���C���[�\���̂̒�`
//****************************************************************************
typedef enum
{	
	PLAYERSTATE_APPEAR = 0,		//�o�����(�_��)
	PLAYERSTATE_NORMAL,	//�ʏ���
	PLAYERSTATE_DAMAGE,		//�_���[�W���
	PLAYERSTATE_DEATH,		//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{
	APPEARSTATE_IN = 0,		//��������
	APPEARSTATE_OUT,		//�����Ȃ����
	APPEARSTATE_MAX
}APPEARSTATE;

typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 moveBullet;	//�e�̈ړ���
	D3DXVECTOR3 rot;		//��]
	D3DXVECTOR3 posOld;		//�O��̈ʒu 
	D3DXCOLOR	col;		//�F
	PLAYERSTATE state;		//���
	APPEARSTATE Astate;		//�o�����
	int nCounterState;		//��ԊǗ��J�E���^�[
	int nCounterAppear;		//�o����ԃJ�E���^�[
	int nPatternAnim;		//�A�j���[�V�����p�^�[��
	int nLife;				//����
	int nDirectionMove;
	float fWidth;			//��
	float fHeight;			//����
	float fAngle;			//�p�x(�A�[�N�^���W�F���g)
	float fLength;			//����(�X�N�E�F�A���[�g)
	bool bUse;				//�g�p����
	bool bAppearUse;		//�o���g�p����
	bool bDisp;
	bool bIsJumping;		//�W�����v�g�p����
	BLOCK *pBlock;			//�u���b�N�̏��
}Player;

//****************************************************************************
//�v���g�^�C�v�錾
//****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void HitPlayer(int nDamage);

#endif