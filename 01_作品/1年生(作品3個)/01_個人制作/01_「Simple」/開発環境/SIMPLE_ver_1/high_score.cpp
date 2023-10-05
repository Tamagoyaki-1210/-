//============================================================================
//
//�n�C�X�R�A�̏���[high_score.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "score.h"		//�쐬����score.h���C���N���[�h����
#include "high_score.h"	//�쐬����high_score.h���C���N���[�h����
#include <stdio.h>		//stdio.h���C���N���[�h����(FILE�ɕK�v)

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureHighScore = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScore = NULL;		//���_�o�b�t�@�ւ̃|�C���^
HIGHSCORE g_aHighScore[MAX_DIGIT];						//�_�������̏��
int g_nHighScore;										//�X�R�A�̒l

//============================================================================
//�n�C�X�R�A�̏���������
//============================================================================
void InitHighScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntHighScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\number000.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureHighScore
	);

	for (nCntHighScore = 0; nCntHighScore < MAX_DIGIT; nCntHighScore++)
	{
		//�\���̂̏���������
		g_aHighScore[nCntHighScore].pos = D3DXVECTOR3((nCntHighScore + 1.0f) * HIGH_SCORE_WIDTH, HIGH_SCORE_HEIGHT, 0.0f);		//�ʒu�̏���������
		g_aHighScore[nCntHighScore].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//�ړ��ʂ̏���������
		g_aHighScore[nCntHighScore].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//����������������
		g_aHighScore[nCntHighScore].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					//�X�R�A�̏����z�F
		g_aHighScore[nCntHighScore].fAngle = atan2f(HIGH_SCORE_WIDTH, HIGH_SCORE_HEIGHT);
		g_aHighScore[nCntHighScore].fLength = sqrtf(HIGH_SCORE_WIDTH * HIGH_SCORE_WIDTH + HIGH_SCORE_HEIGHT * HIGH_SCORE_HEIGHT) / 2.0f;
	}
	//�����X�R�A
	g_nHighScore = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_DIGIT,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_�� * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScore,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHighScore = 0; nCntHighScore < MAX_DIGIT; nCntHighScore++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aHighScore[nCntHighScore].pos.x - sinf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[0].pos.y = g_aHighScore[nCntHighScore].pos.y - cosf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[0].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[1].pos.x = g_aHighScore[nCntHighScore].pos.x + sinf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[1].pos.y = g_aHighScore[nCntHighScore].pos.y - cosf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[1].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[2].pos.x = g_aHighScore[nCntHighScore].pos.x - sinf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[2].pos.y = g_aHighScore[nCntHighScore].pos.y + cosf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[2].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[3].pos.x = g_aHighScore[nCntHighScore].pos.x + sinf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[3].pos.y = g_aHighScore[nCntHighScore].pos.y + cosf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[3].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

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
		pVtx[1].tex = D3DXVECTOR2(1.0f / BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / BASE_NUMBER, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHighScore->Unlock();
}

//============================================================================
//�n�C�X�R�A�̏I������
//============================================================================
void UninitHighScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureHighScore != NULL)
	{
		g_pTextureHighScore->Release();
		g_pTextureHighScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHighScore != NULL)
	{
		g_pVtxBuffHighScore->Release();
		g_pVtxBuffHighScore = NULL;
	}
}

//============================================================================
//�n�C�X�R�A�̍X�V����
//============================================================================
void UpdateHighScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntHighScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHighScore = 0; nCntHighScore < MAX_DIGIT; nCntHighScore++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aHighScore[nCntHighScore].pos.x - sinf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[0].pos.y = g_aHighScore[nCntHighScore].pos.y - cosf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[0].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[1].pos.x = g_aHighScore[nCntHighScore].pos.x + sinf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[1].pos.y = g_aHighScore[nCntHighScore].pos.y - cosf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[1].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[2].pos.x = g_aHighScore[nCntHighScore].pos.x - sinf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[2].pos.y = g_aHighScore[nCntHighScore].pos.y + cosf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[2].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[3].pos.x = g_aHighScore[nCntHighScore].pos.x + sinf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[3].pos.y = g_aHighScore[nCntHighScore].pos.y + cosf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[3].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHighScore->Unlock();
}

//============================================================================
//�n�C�X�R�A�̕`�揈��
//============================================================================
void DrawHighScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntHighScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffHighScore,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntHighScore = 0; nCntHighScore < MAX_DIGIT; nCntHighScore++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureHighScore);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntHighScore * 4,		//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
}
//============================================================================
//�n�C�X�R�A�ǂݍ��ݏ���
//============================================================================
void LoadHighScore(void)
{
	//FILE *pFile;	//�t�@�C���|�C���^��錾

	//				//�t�@�C����ǂ݉���
	//pFile = fopen(FILENAME_DATA, "rb");

	//if (pFile != NULL)
	//{//�t�@�C�����J�����ꍇ

	// //�l�����e�L�X�g�t�@�C������ǂ݉�������
	//	fread(&nMaxPlayer, sizeof(int), 1, pFile);

	//	//�l�����z�񂩂�e�L�X�g�t�@�C������ǂ݉�������
	//	fread(&pPlayer[0], sizeof(Player), nMaxPlayer, pFile);

	//	//�t�@�C�������
	//	fclose(pFile);
	//}
	//else
	//{//�t�@�C�����J���Ȃ������ꍇ
	//	printf("***�t�@�C�����J���܂���ł���***\n");
	//}
}

//============================================================================
//�n�C�X�R�A�ۑ�����
//============================================================================
void SaveHighScore(int nScore)
{
	//FILE *pFile;	//�t�@�C���|�C���^��錾

	//				//�t�@�C�����J��
	//pFile = fopen(FILENAME_DATA, "wb");

	//if (pFile != NULL)
	//{//�t�@�C�����J�����ꍇ

	// //�t�@�C���ɏ����o��
	//	printf("\n");
	//	printf("\t+-----------------------------------------------+\n");
	//	printf("\t|           �e�L�X�g�t�@�C���ɕۑ���...         |\n");
	//	printf("\t+-----------------------------------------------+\n");

	//	//�l�����e�L�X�g�t�@�C���ɏ������ޏ���
	//	fwrite(&nMaxPlayer, sizeof(int), 1, pFile);

	//	//�l�����z�񂩂�e�L�X�g�t�@�C���ɏ������ޏ���
	//	fwrite(&pPlayer[0], sizeof(Player), nMaxPlayer, pFile);

	//	printf("\t+*****************************+\n");
	//	printf("\t+**         �ۑ�����!       **+\n");
	//	printf("\t+*****************************+\n");

	//	//�t�@�C�������
	//	fclose(pFile);
	//}
	//else
	//{//�t�@�C�����J���Ȃ������ꍇ
	//	printf("***�t�@�C�����J���܂���ł���***\n");
	//}
}
