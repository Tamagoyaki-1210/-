//============================================================================
//
//���̏���[white.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "white.h"		//�쐬����white.h���C���N���[�h����
#include "game.h"		//�쐬����game.h���C���N���[�h����
#include "fade.h"		//�쐬����fade.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWhite = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWhite = NULL;		//���_�o�b�t�@�ւ̃|�C���^
White g_aWhite[MAX_WHITE_DIGIT];					//�_�������̏��
int g_nWhite;										//���̒l

//============================================================================
//���̏���������
//============================================================================
void InitWhite(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntWhite;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\number000.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureWhite
	);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//�\���̂̏���������
		g_aWhite[nCntWhite].pos = D3DXVECTOR3(SCREEN_WIDTH * 2 / 3 + (nCntWhite + 1.0f) * WHITE_WIDTH, WHITE_HEIGHT, 0.0f);		//�ʒu�̏���������
		g_aWhite[nCntWhite].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//�ړ��ʂ̏���������
		g_aWhite[nCntWhite].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//����������������
		g_aWhite[nCntWhite].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					//�X�R�A�̏����z�F
		g_aWhite[nCntWhite].fAngle = atan2f(WHITE_WIDTH, WHITE_HEIGHT);
		g_aWhite[nCntWhite].fLength = sqrtf(WHITE_WIDTH * WHITE_WIDTH + WHITE_HEIGHT * WHITE_HEIGHT) / 2.0f;
	}
	//�����X�R�A
	g_nWhite = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_WHITE_DIGIT,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_�� * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffWhite,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWhite->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aWhite[nCntWhite].pos.x - sinf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[0].pos.y = g_aWhite[nCntWhite].pos.y - cosf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[0].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[1].pos.x = g_aWhite[nCntWhite].pos.x + sinf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[1].pos.y = g_aWhite[nCntWhite].pos.y - cosf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[1].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[2].pos.x = g_aWhite[nCntWhite].pos.x - sinf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[2].pos.y = g_aWhite[nCntWhite].pos.y + cosf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[2].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[3].pos.x = g_aWhite[nCntWhite].pos.x + sinf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[3].pos.y = g_aWhite[nCntWhite].pos.y + cosf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[3].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

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
		pVtx[1].tex = D3DXVECTOR2(1.0f / BASE_WHITE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / BASE_WHITE_NUMBER, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWhite->Unlock();
}

//============================================================================
//���̏I������
//============================================================================
void UninitWhite(void)
{

	//�e�N�X�`���̔j��
	if (g_pTextureWhite != NULL)
	{
		g_pTextureWhite->Release();
		g_pTextureWhite = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWhite != NULL)
	{
		g_pVtxBuffWhite->Release();
		g_pVtxBuffWhite = NULL;
	}
}

//============================================================================
//���̍X�V����
//============================================================================
void UpdateWhite(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntWhite;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D *pVtx;

	FADE fade;
	fade = GetFade();

	if (g_nWhite == 0)
	{//�Q�[���N���A
		if (fade == FADE_NONE)
		{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
			SetGameState(GAMESTATE_GAMECLEAR);
		}
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWhite->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aWhite[nCntWhite].pos.x - sinf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[0].pos.y = g_aWhite[nCntWhite].pos.y - cosf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[0].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[1].pos.x = g_aWhite[nCntWhite].pos.x + sinf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[1].pos.y = g_aWhite[nCntWhite].pos.y - cosf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[1].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[2].pos.x = g_aWhite[nCntWhite].pos.x - sinf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[2].pos.y = g_aWhite[nCntWhite].pos.y + cosf(g_aWhite[nCntWhite].fAngle - g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[2].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx[3].pos.x = g_aWhite[nCntWhite].pos.x + sinf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.x) * g_aWhite[nCntWhite].fLength;
		pVtx[3].pos.y = g_aWhite[nCntWhite].pos.y + cosf(g_aWhite[nCntWhite].fAngle + g_aWhite[nCntWhite].rot.y) * g_aWhite[nCntWhite].fLength;
		pVtx[3].pos.z = g_aWhite[nCntWhite].pos.z + 0.0f;

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWhite->Unlock();
}

//============================================================================
//���̕`�揈��
//============================================================================
void DrawWhite(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntWhite;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWhite,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureWhite);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntWhite * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
}

//============================================================================
////���̐ݒ菈��
//============================================================================
void SetWhite(int nWhite)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntWhite;
	int aPosTexU[MAX_WHITE_DIGIT];	//�e���̐������i�[

								//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_nWhite += nWhite;

	if (g_nWhite >= 99999999)
	{
		g_nWhite = 99999999;
	}

	aPosTexU[0] = g_nWhite % 10000 / 1000;
	aPosTexU[1] = g_nWhite % 1000 / 100;
	aPosTexU[2] = g_nWhite % 100 / 10;
	aPosTexU[3] = g_nWhite % 10 / 1;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWhite->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 1.0f / BASE_WHITE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 1.0f / BASE_WHITE_NUMBER, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWhite->Unlock();
}

//============================================================================
////���̉��Z����
//============================================================================
void AddWhite(int nValue)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntWhite;
	int aPosTexU[MAX_WHITE_DIGIT];	//�e���̐������i�[

								//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_nWhite -= nValue;

	if (g_nWhite >= 99999999)
	{
		g_nWhite = 99999999;
	}

	aPosTexU[0] = g_nWhite % 10000 / 1000;
	aPosTexU[1] = g_nWhite % 1000 / 100;
	aPosTexU[2] = g_nWhite % 100 / 10;
	aPosTexU[3] = g_nWhite % 10 / 1;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffWhite->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWhite = 0; nCntWhite < MAX_WHITE_DIGIT; nCntWhite++)
	{
		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 1.0f / BASE_WHITE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntWhite] / BASE_WHITE_NUMBER) + 1.0f / BASE_WHITE_NUMBER, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWhite->Unlock();
}