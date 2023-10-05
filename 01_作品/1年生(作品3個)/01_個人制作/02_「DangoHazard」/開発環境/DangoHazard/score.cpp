//============================================================================
//
//�_���̏���[score.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "score.h"		//�쐬����score.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Score g_aScore[MAX_DIGIT];							//�_�������̏��
int g_nScore;										//�X�R�A�̒l

//============================================================================
//�_���̏���������
//============================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\number000.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureScore
	);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//�\���̂̏���������
		g_aScore[nCntScore].pos = D3DXVECTOR3((nCntScore + 1.0f) * SCORE_WIDTH, SCORE_HEIGHT, 0.0f);		//�ʒu�̏���������
		g_aScore[nCntScore].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//�ړ��ʂ̏���������
		g_aScore[nCntScore].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//����������������
		g_aScore[nCntScore].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					//�X�R�A�̏����z�F
		g_aScore[nCntScore].fAngle = atan2f(SCORE_WIDTH, SCORE_HEIGHT);
		g_aScore[nCntScore].fLength = sqrtf(SCORE_WIDTH * SCORE_WIDTH + SCORE_HEIGHT * SCORE_HEIGHT) / 2.0f;
	}
	//�����X�R�A
	g_nScore = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_DIGIT ,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_�� * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aScore[nCntScore].pos.x - sinf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[0].pos.y = g_aScore[nCntScore].pos.y - cosf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[0].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[1].pos.x = g_aScore[nCntScore].pos.x + sinf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[1].pos.y = g_aScore[nCntScore].pos.y - cosf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[1].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[2].pos.x = g_aScore[nCntScore].pos.x - sinf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[2].pos.y = g_aScore[nCntScore].pos.y + cosf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[2].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[3].pos.x = g_aScore[nCntScore].pos.x + sinf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[3].pos.y = g_aScore[nCntScore].pos.y + cosf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[3].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

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
		pVtx[0].tex = D3DXVECTOR2(0.0f,					0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / BASE_NUMBER,	0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,					1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / BASE_NUMBER,	1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//============================================================================
//�_���̏I������
//============================================================================
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//============================================================================
//�_���̍X�V����
//============================================================================
void UpdateScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aScore[nCntScore].pos.x - sinf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[0].pos.y = g_aScore[nCntScore].pos.y - cosf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[0].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[1].pos.x = g_aScore[nCntScore].pos.x + sinf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[1].pos.y = g_aScore[nCntScore].pos.y - cosf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[1].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[2].pos.x = g_aScore[nCntScore].pos.x - sinf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[2].pos.y = g_aScore[nCntScore].pos.y + cosf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[2].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[3].pos.x = g_aScore[nCntScore].pos.x + sinf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[3].pos.y = g_aScore[nCntScore].pos.y + cosf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[3].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//============================================================================
//�_���̕`�揈��
//============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffScore,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntScore * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
	//�e�N�X�`���̏�����(�o�O�C��_2023/7/12)
	pDevice->SetTexture(0, NULL);
}

//============================================================================
////�_���̐ݒ菈��
//============================================================================
void SetScore(int nScore)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntScore;
	int aPosTexU[MAX_DIGIT];	//�e���̐������i�[

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_nScore += nScore;

	if (g_nScore >= 99999999)
	{
		g_nScore = 99999999;
	}

	aPosTexU[0] = g_nScore % 100000000 / 10000000;
	aPosTexU[1] = g_nScore % 10000000 / 1000000;
	aPosTexU[2] = g_nScore % 1000000 / 100000;
	aPosTexU[3] = g_nScore % 100000 / 10000;
	aPosTexU[4] = g_nScore % 10000 / 1000;
	aPosTexU[5] = g_nScore % 1000 / 100;
	aPosTexU[6] = g_nScore % 100 / 10;
	aPosTexU[7] = g_nScore % 10 / 1;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 0.0f,				0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 1.0f / BASE_NUMBER,	0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 0.0f,				1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 1.0f / BASE_NUMBER,	1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//============================================================================
////�_���̉��Z����
//============================================================================
void AddScore(int nValue)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntScore;
	int aPosTexU[MAX_DIGIT];	//�e���̐������i�[

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_nScore += nValue;

	if (g_nScore >= 99999999)
	{
		g_nScore = 99999999;
	}

	//�w��̌��̐��l�𒊏o���鏈��
	aPosTexU[0] = g_nScore % 100000000 / 10000000;
	aPosTexU[1] = g_nScore % 10000000 / 1000000;
	aPosTexU[2] = g_nScore % 1000000 / 100000;
	aPosTexU[3] = g_nScore % 100000 / 10000;
	aPosTexU[4] = g_nScore % 10000 / 1000;
	aPosTexU[5] = g_nScore % 1000 / 100;
	aPosTexU[6] = g_nScore % 100 / 10;
	aPosTexU[7] = g_nScore % 10 / 1;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 1.0f / BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 1.0f / BASE_NUMBER, 1.0f);
	
		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}