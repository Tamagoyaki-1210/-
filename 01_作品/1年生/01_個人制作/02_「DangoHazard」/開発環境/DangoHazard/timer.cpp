//============================================================================
//
//�^�C�}�[�̏���[timer.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "timer.h"		//�쐬����timer.h���C���N���[�h����
#include "game.h"		//�쐬����game.h���C���N���[�h����
#include "fade.h"		//�쐬����fade.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		//���_�o�b�t�@�ւ̃|�C���^
TIMER g_aTimer[MAX_TIMER_DIGIT];					//�_�������̏��
int g_nTimer;									//�^�C�}�[�̒l
int g_nCounterBreak;

//============================================================================
//�^�C�}�[�̏���������
//============================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTimer;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\number000.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureTimer
	);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		if (nCntTimer < MINIUT_TIMER_DIGIT)
		{
			//�\���̂̏���������
			g_aTimer[nCntTimer].pos = D3DXVECTOR3(-TIMER_WIDTH * MINIUT_TIMER_DIGIT + (nCntTimer) * TIMER_WIDTH + SCREEN_WIDTH / 2,
				TIMER_HEIGHT, 0.0f);		//�ʒu�̏���������
		}
		else
		{
			g_aTimer[nCntTimer].pos = D3DXVECTOR3(-TIMER_WIDTH * MINIUT_TIMER_DIGIT + TIMER_WIDTH + (nCntTimer) * TIMER_WIDTH + SCREEN_WIDTH / 2,
				TIMER_HEIGHT, 0.0f);		//�ʒu�̏���������
		}
			g_aTimer[nCntTimer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//�ړ��ʂ̏���������
			g_aTimer[nCntTimer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//����������������
			g_aTimer[nCntTimer].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					//�^�C�}�[�̏����z�F
			g_aTimer[nCntTimer].fAngle = atan2f(TIMER_WIDTH, TIMER_HEIGHT);
			g_aTimer[nCntTimer].fLength = sqrtf(TIMER_WIDTH * TIMER_WIDTH + TIMER_HEIGHT * TIMER_HEIGHT) / 2.0f;
	}
	g_nTimer = COUNT_TIMER;
	g_nCounterBreak = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_TIMER_DIGIT,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_�� * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aTimer[nCntTimer].pos.x - sinf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[0].pos.y = g_aTimer[nCntTimer].pos.y - cosf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[0].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[1].pos.x = g_aTimer[nCntTimer].pos.x + sinf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[1].pos.y = g_aTimer[nCntTimer].pos.y - cosf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[1].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[2].pos.x = g_aTimer[nCntTimer].pos.x - sinf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[2].pos.y = g_aTimer[nCntTimer].pos.y + cosf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[2].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[3].pos.x = g_aTimer[nCntTimer].pos.x + sinf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[3].pos.y = g_aTimer[nCntTimer].pos.y + cosf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[3].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / TIMER_BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TIMER_BASE_NUMBER, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();

	CounterTimer(1);
}

//============================================================================
//�^�C�}�[�̏I������
//============================================================================
void UninitTimer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//============================================================================
//�^�C�}�[�̍X�V����
//============================================================================
void UpdateTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntTimer;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	VERTEX_2D *pVtx;

	FADE fade;
	fade = GetFade();

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aTimer[nCntTimer].pos.x - sinf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[0].pos.y = g_aTimer[nCntTimer].pos.y - cosf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[0].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[1].pos.x = g_aTimer[nCntTimer].pos.x + sinf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[1].pos.y = g_aTimer[nCntTimer].pos.y - cosf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[1].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[2].pos.x = g_aTimer[nCntTimer].pos.x - sinf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[2].pos.y = g_aTimer[nCntTimer].pos.y + cosf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[2].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[3].pos.x = g_aTimer[nCntTimer].pos.x + sinf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[3].pos.y = g_aTimer[nCntTimer].pos.y + cosf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[3].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		//if(g_nTimer < 10)
		//{//0�b�����̏ꍇ
		//	//���_�J���[�̐ݒ�(��)
		//	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		//	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		//	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		//	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		//}
		//else
		//{//10�b�ȏ�̏ꍇ
		//	//���_�J���[�̐ݒ�
		//	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//}

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	if (g_nTimer <= 0)
	{//�^�C�}�[��0�ȉ��̏ꍇ
		g_nTimer = 0;
		if (fade == FADE_NONE)
		{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
			SetGameState(GAMESTATE_GAMECLEAR);
		}
	}

	g_nCounterBreak++;
	if (g_nCounterBreak == 60)
	{
		if (g_nTimer <= 0)
		{//0�b��艺�̏ꍇ
			if (fade == FADE_NONE)
			{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
				SetGameState(GAMESTATE_GAMECLEAR);
			}
		}
		CounterTimer(1);
		g_nCounterBreak = 0;
	}

	if (g_nTimer <= 0)
	{//0�b��艺�̏ꍇ
		g_nTimer = 0;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();
}

//============================================================================
//�_���̕`�揈��
//============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntTimer;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffTimer,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTimer);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntTimer * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
}

//============================================================================
////�^�C�}�[�̐ݒ菈��
//============================================================================
void SetTimer(int nTimer)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	int nCntTimer;
	int aPosTexU[MAX_TIMER_DIGIT];	//�e���̐������i�[
		
	pDevice = GetDevice();			//�f�o�C�X�̎擾

	g_nTimer = nTimer;

	if (g_nTimer >= 99999999)
	{
		g_nTimer = 99999999;
	}

	if (g_nTimer >= 0)
	{
		aPosTexU[0] = g_nTimer % 6000 / 600;
		aPosTexU[1] = g_nTimer % 600 / 60;
		aPosTexU[2] = g_nTimer % 60 / 10;
		aPosTexU[3] = g_nTimer % 10;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 1.0f / TIMER_BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 1.0f / TIMER_BASE_NUMBER, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();
}

//============================================================================
////�^�C�}�[�̃J�E���^�[����
//============================================================================
void CounterTimer(int nValue)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntTimer;
	int aPosTexU[MAX_TIMER_DIGIT];	//�e���̐������i�[

								//�f�o�C�X�̎擾
	pDevice = GetDevice();

		g_nTimer -= nValue;
		if (g_nTimer >= 99999999)
		{
			g_nTimer = 99999999;
		}
	
		if (g_nTimer >= 0)
		{
			aPosTexU[0] = g_nTimer % 6000 / 600;
			aPosTexU[1] = g_nTimer % 600 / 60;
			aPosTexU[2] = g_nTimer % 60 / 10;
			aPosTexU[3] = g_nTimer % 10;
		}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	
	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 1.0f / TIMER_BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 1.0f / TIMER_BASE_NUMBER, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimer->Unlock();
}