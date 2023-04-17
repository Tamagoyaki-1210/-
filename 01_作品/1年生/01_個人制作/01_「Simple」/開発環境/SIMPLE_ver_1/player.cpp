//============================================================================
//
//�v���C���[�`��̏���player.cpp] (Update:10/11)
//Author:�J�{�N��
//============================================================================
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "block.h"		//�쐬����block.h���C���N���[�h����
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "enemy.h"			//�쐬����enemy.h���C���N���[�h����
#include "score.h"		//�쐬����score.h���C���N���[�h����
#include "fade.h"		//�쐬����fade.h���C���N���[�h����
#include "game.h"		//�쐬����game.h���C���N���[�h����
#include "bullet.h"		//�쐬����bullet.h���C���N���[�h����
#include "explosion.h"	//�쐬����explosion.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffPlayer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Player g_aPlayer;									//�v���C���[�̏��

int	g_nJumpCounter;								//�W�����v�󂯕t������
int g_nJumpStock;								//�v���C���[�̃W�����v��
int g_nBulletCollTime;							//�e�̃N�[���^�C��
int g_nCounterAnimPlayer;
int g_nPatternAnimPlayer;
bool g_bExplosionUse;

//============================================================================
//�|���S���̏���������
//============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();				//�f�o�C�X�̎擾

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\player000.png",	//�e�N�X�`���̃t�@�C����
		&g_pTexturePlayer
	);

	int	g_nJumpCounter = 0;								//�W�����v�󂯕t������
	int g_nJumpStock = 0;								//�v���C���[�̃W�����v��
	int g_nBulletCollTime = 0;							//�e�̃N�[���^�C��
	bool g_bExplosionUse = true;

	//�ʒu�̏���������
	g_aPlayer.pos = D3DXVECTOR3(PLAYER_WIDTH / 2, SCREEN_HEIGHT,0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��ʂ̏���������
	g_aPlayer.moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�e�̈ړ��ʂ̏���������
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����������������
	g_aPlayer.col = D3DXCOLOR(0.0f,0.0f, 0.0f, 0.0f);
	g_aPlayer.state = PLAYERSTATE_NORMAL;					//�v���C���[�̏��
	g_aPlayer.Astate = APPEARSTATE_IN;						//�v���C���[�̏o�����
	g_aPlayer.nCounterState = 0;
	g_aPlayer.nCounterAppear = 0;
	g_aPlayer.nLife = MAX_PLAYER_LIFE;						//�v���C���[�̏����̗�
	g_aPlayer.nDirectionMove = 0;							//�v���C���[�̕���
	g_aPlayer.fWidth = PLAYER_WIDTH;
	g_aPlayer.fHeight = PLAYER_HEIGHT;
	g_aPlayer.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);
	g_aPlayer.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;
	g_aPlayer.bUse = true;									//�v���C���[�g�p����
	g_aPlayer.bAppearUse = false;							//�v���C���[�̏o������
	g_aPlayer.bDisp = true;	
	g_aPlayer.bIsJumping = false;							//�W�����v�g�p����
	g_aPlayer.pBlock = NULL;								//�u���b�N����NULL����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;			

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer -> Lock(0, 0,(void**)&pVtx, 0);

	//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
	pVtx[0].pos.x = g_aPlayer.pos.x - sinf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.x) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y - cosf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
	pVtx[0].pos.z = g_aPlayer.pos.z + 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.x) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y - cosf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
	pVtx[1].pos.z = g_aPlayer.pos.z + 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x - sinf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.x) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
	pVtx[2].pos.z = g_aPlayer.pos.z + 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.x) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
	pVtx[3].pos.z = g_aPlayer.pos.z + 0.0f;
	
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
	pVtx[0].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
	pVtx[1].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
	pVtx[2].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
	pVtx[3].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer -> Unlock();
}

//============================================================================
//�v���C���[�̏I������
//============================================================================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer -> Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer -> Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//============================================================================
//�v���C���[�̍X�V����
//============================================================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	FADE fade;
	fade = GetFade();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
 	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aPlayer.nLife < 0)
	{
		g_aPlayer.nLife = 0;
	}

	if (g_aPlayer.bUse == true)
	{//�v���C���[���g�p����Ă���ꍇ
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���E�ɓ���������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
		{//A�L�[�������ꂽ���A���ړ�
				g_aPlayer.move.x -= sinf(D3DX_PI/ 4.0f) * 1.0f;
				g_aPlayer.move.y -= cosf(D3DX_PI/ 4.0f) * 0.0f;

				if (GetKeyboardPress(DIK_B) == true || GetKeyboardPress(DIK_LSHIFT) == true)
				{//B�_�b�V���Ŕ{��
					g_aPlayer.move.x -= 0.5f;
					g_aPlayer.move.y += 0.0f;
				}
				g_nCounterAnimPlayer++;				//�J�E���^�[�����Z
				g_aPlayer.nDirectionMove = 1;
		}
		else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
		{//D�L�[�������ꂽ���A�E�ړ�
			g_aPlayer.move.x += sinf(D3DX_PI / 4.0f) * 1.0f;
			g_aPlayer.move.y += cosf(D3DX_PI / 4.0f) * 0.0f;

			if (GetKeyboardPress(DIK_B) == true || GetKeyboardPress(DIK_LSHIFT) == true)
			{//B�_�b�V���Ŕ{��
				g_aPlayer.move.x += 0.5f;
				g_aPlayer.move.y += 0.0f;
			}

			g_nCounterAnimPlayer++;				//�J�E���^�[�����Z
			g_aPlayer.nDirectionMove = 0;
		}
		else if(g_aPlayer.bIsJumping == false)
		{//�ړ�0�ł��A�j���[�V���������鏈��
			g_nPatternAnimPlayer = 0 ;
			pVtx[0].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
			pVtx[1].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
			pVtx[2].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
			pVtx[3].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
		}
		if (GetKeyboardPress(DIK_Q) == true)
		{//Q�L�[���������ꍇ�A�������ɂ��鏈��
			g_aPlayer.nDirectionMove = 1;
		}
		if (GetKeyboardPress(DIK_E) == true)
		{//E�L�[���������ꍇ�A�E�����ɂ��鏈��
			g_aPlayer.nDirectionMove = 0;
		}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�L�[�������ĂȂ��Ă����������鏈��
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (GetKeyboardTrigger(DIK_Z) == true)
		{//Z�ŃX���C�h����
			g_aPlayer.move.x *= 2.0f;
		}
		if (GetKeyboardTrigger(DIK_P) == true)
		{
			//�J���X�g�X�R�A��n������
			AddScore(100000000);
		}
		else if (GetKeyboardTrigger(DIK_O) == true)
		{
			InitScore();		//�_���̏���������
		}

		//if (GetKeyboardTrigger(DIK_RETURN) == true)
		//{//���f�o�b�N�EENTER�L�[�Ń��U���g
		//	if (fade == FADE_NONE)
		//	{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
		//		SetGameState(GAMESTATE_GAMECLEAR);
		//	}
		//}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�W�����v�̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_nJumpStock < PLAYER_MAXJUMP)
		{//�w�肳�ꂽ�W�����v�񐔂̂݃W�����v����
			if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
			{//W�L�[�������ꂽ�� or �X�y�[�X�L�[�����������A��ړ�
				g_aPlayer.bIsJumping = true;	//�W�����v�g�p��^��
				g_nJumpCounter++;			//�W�����v��t���b�J�E���g
				if (g_nJumpCounter == 1)
				{//���W�����v(�W�����v�񐔂��ƂɌ���)
					g_aPlayer.move.y = 0;
					g_aPlayer.move.y -= cosf(D3DX_PI * 1.0f / 4.0f) * 30.0f;
					g_nJumpStock++;
				}
				else if (g_nJumpCounter > 1 && g_nJumpCounter <= 10)
				{//�b���ݒ�\�̃W�����v
					g_aPlayer.move.x -= sinf(D3DX_PI * 1.0f / 4.0f) * 0.0f;
					g_aPlayer.move.y -= cosf(D3DX_PI * 1.0f / 4.0f) * 1.0f;
				}
				if (g_nPatternAnimPlayer == 0 || g_nPatternAnimPlayer == 2)
				{
					g_nPatternAnimPlayer++;
				}
				if (g_nPatternAnimPlayer == 1 || g_nPatternAnimPlayer == 3)
				{
					g_nPatternAnimPlayer += 2;
				}
				pVtx[0].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
				pVtx[1].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
				pVtx[2].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
				pVtx[3].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
			}
			else
			{//�L�[�𗣂������̏���
				g_nJumpCounter = 0;
			}
		}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�A�j���[�V��������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_nCounterAnimPlayer >= PLAYER_ANIMATION)
		{//��莞�Ԍo�߂������̏���
			if (g_nPatternAnimPlayer >= PLAYER_ANIMATION)
			{//���p�^�[�����𒴂���
				g_nPatternAnimPlayer = 0;	//�p�^�[��No.�������l�ɖ߂�
			}
			if (g_aPlayer.bIsJumping == false)
			{//�n�ʂɒ��n���Ă���ꍇ�A�A�j���𓮂�������
			 //�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)(�e�N�X�`��������)
				pVtx[0].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
				pVtx[1].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
				pVtx[2].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
				pVtx[3].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
				g_nCounterAnimPlayer = 0;		//�J�E���^�[�������l�ɖ߂�
				g_nPatternAnimPlayer++;			//�p�^�[��No.���X�V����
			}
		}

		//�v���C���[�ɏd�͂𔭐�������
		g_aPlayer.move.y += 1.5f;
		
		//�O��̈ʒu��ۑ�
		g_aPlayer.posOld = g_aPlayer.pos;

		//�ʒu���X�V
		g_aPlayer.pos.x += g_aPlayer.move.x;
		g_aPlayer.pos.y += g_aPlayer.move.y;

		//�ړ��ʂ��X�V(����������)
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.1f;

		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aPlayer.pos.x - sinf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.x) * g_aPlayer.fLength;
		pVtx[0].pos.y = g_aPlayer.pos.y - cosf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
		pVtx[0].pos.z = g_aPlayer.pos.z + 0.0f;

		pVtx[1].pos.x = g_aPlayer.pos.x + sinf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.x) * g_aPlayer.fLength;
		pVtx[1].pos.y = g_aPlayer.pos.y - cosf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
		pVtx[1].pos.z = g_aPlayer.pos.z + 0.0f;

		pVtx[2].pos.x = g_aPlayer.pos.x - sinf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.x) * g_aPlayer.fLength;
		pVtx[2].pos.y = g_aPlayer.pos.y + cosf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
		pVtx[2].pos.z = g_aPlayer.pos.z + 0.0f;

		pVtx[3].pos.x = g_aPlayer.pos.x + sinf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.x) * g_aPlayer.fLength;
		pVtx[3].pos.y = g_aPlayer.pos.y + cosf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
		pVtx[3].pos.z = g_aPlayer.pos.z + 0.0f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���@�̒e�̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//�e�̈ړ������ƕ���
		g_aPlayer.moveBullet.x = sinf(g_aPlayer.rot.x + g_aPlayer.fAngle) * 16.0f;
		g_aPlayer.moveBullet.y = cosf(g_aPlayer.rot.y + g_aPlayer.fAngle) * 0.0f;
		g_aPlayer.moveBullet.z = 0.0f;

		if (g_nBulletCollTime == 0)
		{
			//�m�[�}���V���b�g
			if (GetKeyboardTrigger(DIK_SPACE) == true)
			{//SPACE�L�[�������ꂽ
				g_nBulletCollTime = 20;
				if (g_aPlayer.nDirectionMove == 0)
				{//�E�����̏ꍇ
					if (GetKeyboardPress(DIK_E) == true)
					{//E�L�[��������Ă���ꍇ�A�E��ɒe��ł�
						g_aPlayer.moveBullet.y = cosf(g_aPlayer.rot.y + g_aPlayer.fAngle) * -8.0f;
						SetBullet	//�ʒu,�ړ���
						(D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth / 2, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), g_aPlayer.moveBullet);
					}
					else
					{
						SetBullet	//�ʒu,�ړ���
						(D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth / 2, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), g_aPlayer.moveBullet);
					}
				}
				else if (g_aPlayer.nDirectionMove == 1)
				{//�������̏ꍇ
					if (GetKeyboardPress(DIK_Q) == true)
					{//W�L�[��������Ă���ꍇ�A����ɒe��ł�
						g_aPlayer.moveBullet.y = cosf(g_aPlayer.rot.y + g_aPlayer.fAngle) * 8.0f;
						SetBullet	//�ʒu,�ړ���
						(D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth / 2, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), -g_aPlayer.moveBullet);
					}
					else
					{
						SetBullet	//�ʒu,�ړ���
						(D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth / 2, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), -g_aPlayer.moveBullet);
					}
				}
			}
		}
		else
		{
			g_nBulletCollTime--;
		}
		if (GetKeyboardPress(DIK_N) == true && g_aPlayer.fLength > PLAYER_WIDTH)
		{//N�L�[�ŏk��
			g_aPlayer.fLength -= 4.0f;
		}
		if (GetKeyboardPress(DIK_M) == true && g_aPlayer.fLength < PLAYER_WIDTH * 2)
		{//M�L�[�Ŋg��
			g_aPlayer.fLength += 4.0f;
		}
		if (GetKeyboardPress(DIK_K) == true)
		{//K�L�[�ō����
			g_aPlayer.rot.x += 10.0;
			g_aPlayer.rot.y += 10.0;
		}
		if (GetKeyboardPress(DIK_L) == true)
		{//L�L�[�ŉE���
			g_aPlayer.rot.x = 0.0;
			g_aPlayer.rot.y = 0.0;
		}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�O�ʊO���[�v����
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_aPlayer.pos.x > SCREEN_WIDTH + PLAYER_WIDTH / 2)
		{//��ʉE�[�܂ŗ������̏���
			g_aPlayer.pos.x = 0.0f - PLAYER_WIDTH / 2 + 1;
			g_aPlayer.posOld.x = 0.0f - PLAYER_WIDTH / 2;
		}
		if (g_aPlayer.pos.x < 0.0f - PLAYER_WIDTH / 2)
		{//��ʍ��[�܂ŗ������̏���
			g_aPlayer.pos.x = SCREEN_WIDTH + PLAYER_WIDTH / 2 - 1;
			g_aPlayer.posOld.x = SCREEN_WIDTH + PLAYER_WIDTH / 2;
		}
		if (g_aPlayer.pos.y > SCREEN_HEIGHT)
		{//��ʉ��[�܂ŗ������̏���
			g_aPlayer.pos.y = SCREEN_HEIGHT;
			g_aPlayer.move.y = 0;
			g_aPlayer.bIsJumping = false;
			g_nJumpStock = 0;
		}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�u���b�N�����蔻��̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		g_aPlayer.pBlock = NULL;
		if (ColisionBlock(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, 
			g_aPlayer.fWidth, g_aPlayer.fHeight, &g_aPlayer.pBlock) == BLOCK_COLLISION_TOP)
		{//�u���b�N�ɏ�����ꍇ�̏���
			g_nJumpStock = 0;
			g_aPlayer.bIsJumping = false;

			//�u���b�N�̏�Ƀv���C���[���Œ�
			g_aPlayer.pos.y = g_aPlayer.pBlock->pos.y - g_aPlayer.pBlock->fHeight / 2;
			g_aPlayer.move.y = 0;

			if (g_aPlayer.pBlock->move != NULL)
			{//�u���b�N�������Ă���ꍇ�A�v���C���[�Ɋ��������鏈��
				g_aPlayer.move.x += g_aPlayer.pBlock->move.x * 0.1f;

				if (g_aPlayer.pBlock->pos.x >= g_aPlayer.pBlock->setpos.x + 200.0f
					|| g_aPlayer.pBlock->pos.x <= g_aPlayer.pBlock->setpos.x - 200.0f)
				{//�u���b�N���w��̈ʒu�ɗ������A�v���C���[�𓮂�������
					g_aPlayer.move.x += g_aPlayer.pBlock->move.x * 2;
				}
			}
			if (g_aPlayer.pBlock->type == BLOCK_TYPE_EVADE || g_aPlayer.pBlock->type == BLOCK_TYPE_WHITE)
			{//���蔲���u���b�N�̏�ɂ��鎞�̏���
				if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
				{//���L�[�ł��蔲��
					g_aPlayer.pos.y = g_aPlayer.pBlock->pos.y - g_aPlayer.pBlock->fHeight / 2 + 5.0f;
					g_aPlayer.pBlock = NULL;
				}
			}
		}
		ColisionItem(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, g_aPlayer.fWidth, g_aPlayer.fHeight);
	}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�v���C���[�̏�ԑJ�ڏ���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	switch (g_aPlayer.state)
	{//�v���C���[��Ԃɂ�邻�ꂼ��̏���
	case PLAYERSTATE_NORMAL:
		g_aPlayer.bUse = true;
		g_aPlayer.bAppearUse = false;
		g_bExplosionUse = false;
		g_aPlayer.col.a = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
	case PLAYERSTATE_DAMAGE:
		g_aPlayer.nCounterState--;
		g_aPlayer.bAppearUse = true;

		if (g_aPlayer.nCounterState <= 0)
		{
			g_aPlayer.state = PLAYERSTATE_APPEAR;
		}
		break;
	case PLAYERSTATE_DEATH:
		g_aPlayer.nCounterState--;
		g_aPlayer.col.a -= 0.01f;		//�v���C���[�����X�ɓ����ɂ��鏈��
		g_aPlayer.bUse = false;
		if (g_bExplosionUse == false)
		{
			g_bExplosionUse = true;
			SetExplosion(g_aPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (g_aPlayer.col.a <= 0.5f)
		{
			g_aPlayer.col.a = 0.5f;
		}
		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aPlayer.col;
		pVtx[1].col = g_aPlayer.col;
		pVtx[2].col = g_aPlayer.col;
		pVtx[3].col = g_aPlayer.col;


		if (fade == FADE_NONE)
		{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
			SetGameState(GAMESTATE_GAMEOVER);
		}
		break;
	case PLAYERSTATE_APPEAR:
		g_aPlayer.nCounterAppear++;		//�_���[�W�_�Ŏ���

		if (g_aPlayer.nCounterAppear <= 120)
		{
			if (g_aPlayer.Astate == APPEARSTATE_IN)
			{//Astate��IN�̏�Ԃ̏���
				g_aPlayer.col.a -= 0.5f;		//�v���C���[�����X�ɓ����ɂ��鏈��

				if (g_aPlayer.col.a <= 0.0f)
				{
					g_aPlayer.col.a = 0.0f;
					g_aPlayer.Astate = APPEARSTATE_OUT;		//�v���C���[�̏o����ԕύX
				}
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 0);
				pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 0);
				pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 0);
				pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 0);
			}
			else if (g_aPlayer.Astate == APPEARSTATE_OUT)
			{//Astate��OUT�̏�Ԃ̏���
				g_aPlayer.col.a += 0.5f;		//�v���C���[�����X�ɕs�����ɂ��鏈��

				if (g_aPlayer.col.a >= 1.0f)
				{
					g_aPlayer.col.a = 1.0f;
					g_aPlayer.Astate = APPEARSTATE_IN;	//�v���C���[�̏o����ԕύX
				}
				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
		}
		else
		{
			g_aPlayer.nCounterAppear = 0;
			g_aPlayer.state = PLAYERSTATE_NORMAL;
		}
		break;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//============================================================================
//�v���C���[�̕`�揈��
//============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_aPlayer.bDisp == true)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(
			0,
			g_pVtxBuffPlayer,		//���_�o�b�t�@�ւ̃|�C���^
			0,
			sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
		);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
}

//============================================================================
//�v���C���[�̎擾����
//============================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}

//============================================================================
////�v���C���[�̔�e����
//============================================================================
void HitPlayer(int nDamage)
{
	VERTEX_2D*pVtx;		//���_���ւ̃|�C���^

	if (g_aPlayer.bAppearUse == false)
	{//�v���C���[���g�p����Ă���ꍇ

		//�G�̒e�������������A�v���C���[�Ƀ_���[�W
		g_aPlayer.nLife -= nDamage;

		if (g_aPlayer.nLife <= 0)
		{//�v���C���[�̃��C�t��0�ɂȂ������̏���
			g_aPlayer.bUse = false;
			g_aPlayer.state = PLAYERSTATE_DEATH;
		}
		else
		{//�v���C���[�̃��C�t���c���Ă��鎞�̏���
			g_aPlayer.state = PLAYERSTATE_DAMAGE;
			g_aPlayer.nCounterState = 1;		//�_���[�W�_�Ŏ���

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//���_�J���[�̐ݒ�(��)
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(252, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

			g_pVtxBuffPlayer->Unlock();
		}
	}
}
