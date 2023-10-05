//============================================================================
//
//�Q�[���I�[�o�[�̏���[gameover.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "gameover.h"	//�쐬����gameover.h���C���N���[�h����
#include "fade.h"		//�쐬����fade.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGameOver[MAX_GAMEOVER_IMAGE];	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameOver = NULL;			//���_�o�b�t�@�ւ̃|�C���^
GAMEOVER g_aGameOver[MAX_GAMEOVER_IMAGE];					//�^�C�g���̏��

int g_nCounterGameOverState = 0;							//��ԊǗ��J�E���^�[
bool GameOverCounterUse = false;

//============================================================================
//�Q�[���I�[�o�[�̏���������
//============================================================================
void InitGameOver(void)
{
	int nCntGameOver;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\wood.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureGameOver[0]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\press_enter.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureGameOver[1]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\gameover_logo.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureGameOver[2]
	);

	//�\���̂̏���������
	g_aGameOver[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);	//�ʒu�̏���������
	g_aGameOver[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGameOver[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//��]������������
	g_aGameOver[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aGameOver[0].fAngle = atan2f(GAMEOVER_BG_WIDTH, GAMEOVER_BG_HEIGHT);
	g_aGameOver[0].fLength = sqrtf(GAMEOVER_BG_WIDTH * GAMEOVER_BG_WIDTH + GAMEOVER_BG_HEIGHT * GAMEOVER_BG_HEIGHT) / 2.0f;

	g_aGameOver[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.25f, 0.0f);	//�ʒu�̏���������
	g_aGameOver[1].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGameOver[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//��]������������
	g_aGameOver[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aGameOver[1].fAngle = atan2f(GAMEOVER_ENTER_WIDTH, GAMEOVER_ENTER_HEIGHT);
	g_aGameOver[1].fLength = sqrtf(GAMEOVER_ENTER_WIDTH * GAMEOVER_ENTER_WIDTH + GAMEOVER_ENTER_HEIGHT * GAMEOVER_ENTER_HEIGHT) / 2.0f;

	g_aGameOver[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f,SCREEN_HEIGHT / 2.0f, 0.0f);	//�ʒu�̏���������
	g_aGameOver[2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aGameOver[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//��]������������
	g_aGameOver[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aGameOver[2].fAngle = atan2f(GAMEOVER_LOGO_WIDTH, GAMEOVER_LOGO_HEIGHT);
	g_aGameOver[2].fLength = sqrtf(GAMEOVER_LOGO_WIDTH * GAMEOVER_LOGO_WIDTH + GAMEOVER_LOGO_HEIGHT * GAMEOVER_LOGO_HEIGHT) / 2.0f;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_GAMEOVER_IMAGE,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameOver,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffGameOver->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGameOver = 0; nCntGameOver < MAX_GAMEOVER_IMAGE; nCntGameOver++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aGameOver[nCntGameOver].pos.x - sinf(g_aGameOver[nCntGameOver].fAngle + g_aGameOver[nCntGameOver].rot.x) * g_aGameOver[nCntGameOver].fLength;
		pVtx[0].pos.y = g_aGameOver[nCntGameOver].pos.y - cosf(g_aGameOver[nCntGameOver].fAngle + g_aGameOver[nCntGameOver].rot.y) * g_aGameOver[nCntGameOver].fLength;
		pVtx[0].pos.z = g_aGameOver[nCntGameOver].pos.z + 0.0f;

		pVtx[1].pos.x = g_aGameOver[nCntGameOver].pos.x + sinf(g_aGameOver[nCntGameOver].fAngle - g_aGameOver[nCntGameOver].rot.x) * g_aGameOver[nCntGameOver].fLength;
		pVtx[1].pos.y = g_aGameOver[nCntGameOver].pos.y - cosf(g_aGameOver[nCntGameOver].fAngle - g_aGameOver[nCntGameOver].rot.y) * g_aGameOver[nCntGameOver].fLength;
		pVtx[1].pos.z = g_aGameOver[nCntGameOver].pos.z + 0.0f;

		pVtx[2].pos.x = g_aGameOver[nCntGameOver].pos.x - sinf(g_aGameOver[nCntGameOver].fAngle - g_aGameOver[nCntGameOver].rot.x) * g_aGameOver[nCntGameOver].fLength;
		pVtx[2].pos.y = g_aGameOver[nCntGameOver].pos.y + cosf(g_aGameOver[nCntGameOver].fAngle - g_aGameOver[nCntGameOver].rot.y) * g_aGameOver[nCntGameOver].fLength;
		pVtx[2].pos.z = g_aGameOver[nCntGameOver].pos.z + 0.0f;

		pVtx[3].pos.x = g_aGameOver[nCntGameOver].pos.x + sinf(g_aGameOver[nCntGameOver].fAngle + g_aGameOver[nCntGameOver].rot.x) * g_aGameOver[nCntGameOver].fLength;
		pVtx[3].pos.y = g_aGameOver[nCntGameOver].pos.y + cosf(g_aGameOver[nCntGameOver].fAngle + g_aGameOver[nCntGameOver].rot.y) * g_aGameOver[nCntGameOver].fLength;
		pVtx[3].pos.z = g_aGameOver[nCntGameOver].pos.z + 0.0f;

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGameOver->Unlock();

}

//============================================================================
//�Q�[���I�[�o�[�̏I������
//============================================================================
void UninitGameOver(void)
{
	int nCntGameOver;

	for (nCntGameOver = 0; nCntGameOver < MAX_GAMEOVER_IMAGE; nCntGameOver++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureGameOver[nCntGameOver] != NULL)
		{
			g_pTextureGameOver[nCntGameOver]->Release();
			g_pTextureGameOver[nCntGameOver] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGameOver != NULL)
	{
		g_pVtxBuffGameOver->Release();
		g_pVtxBuffGameOver = NULL;
	}
}

//============================================================================
//�Q�[���I�[�o�[�̍X�V����
//============================================================================
void UpdateGameOver(void)
{
	FADE fade;
	fade = GetFade();

	if (fade == FADE_NONE)
	{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
	 //Enter�L�[���̓X�y�[�X�L�[�������ꂽ�ꍇ
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//���[�h�ݒ�
			SetFade(MODE_TITLE);

			GameOverCounterUse = true;
		}
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Enter�������ꂽ��̕b���𐔂��鏈��
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (g_nCounterGameOverState >= 60)
	{//60f(�t���[��)��̏���
		GameOverCounterUse = false;
		g_nCounterGameOverState = 0;
	}
	else if (GameOverCounterUse == true)
	{//Enter��������Ă���̎��Ԃ𐔂��鏈��
		g_nCounterGameOverState++;
	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�^�C�g�����S�𓮂�������
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//VERTEX_2D *pVtx;

	////���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	//g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//pVtx += 8;	//��Ƀ|�C���^��8��(�摜2��)����������

	//if (g_aTitle[2].pos.y < TITLE_HEIGHT / 4)
	//{
	//	g_aTitle[2].move.y += (1.0f / 60.0f);

	//	if (fade == FADE_NONE)
	//	{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
	//		if (GetKeyboardTrigger(DIK_RETURN) == true)
	//		{//�^�C�g�����S�����ɗ��Ƃ�����
	//			g_aTitle[2].pos.y = TITLE_HEIGHT / 4;
	//		}
	//	}
	//}
	//else
	//{
	//	g_aTitle[2].move.y = 0.0f;

	//	////Enter�L�[���̓X�y�[�X�L�[�������ꂽ�ꍇ
	//	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//	//{
	//	//	//���[�h�ݒ�
	//	//	SetMode(MODE_GAME);
	//	//}
	//}

	//g_aTitle[2].pos.y += g_aTitle[2].move.y;

	////���_���W�̐ݒ�
	//pVtx[0].pos.x = g_aTitle[2].pos.x - sinf(0.0f + g_fAngleTitle + g_aTitle[2].rot.x) * g_fLengthTitle / 2;
	//pVtx[0].pos.y = g_aTitle[2].pos.y - cosf(0.0f + g_fAngleTitle + g_aTitle[2].rot.y) * g_fLengthTitle / 6;
	//pVtx[0].pos.z = g_aTitle[2].pos.z + 0.0f;

	//pVtx[1].pos.x = g_aTitle[2].pos.x + sinf(0.0f + g_fAngleTitle - g_aTitle[2].rot.x) * g_fLengthTitle / 2;
	//pVtx[1].pos.y = g_aTitle[2].pos.y - cosf(0.0f + g_fAngleTitle - g_aTitle[2].rot.y) * g_fLengthTitle / 6;
	//pVtx[1].pos.z = g_aTitle[2].pos.z + 0.0f;

	//pVtx[2].pos.x = g_aTitle[2].pos.x - sinf(0.0f + g_fAngleTitle - g_aTitle[2].rot.x) * g_fLengthTitle / 2;
	//pVtx[2].pos.y = g_aTitle[2].pos.y + cosf(0.0f + g_fAngleTitle - g_aTitle[2].rot.y) * g_fLengthTitle / 6;
	//pVtx[2].pos.z = g_aTitle[2].pos.z + 0.0f;

	//pVtx[3].pos.x = g_aTitle[2].pos.x + sinf(0.0f + g_fAngleTitle + g_aTitle[2].rot.x) * g_fLengthTitle / 2;
	//pVtx[3].pos.y = g_aTitle[2].pos.y + cosf(0.0f + g_fAngleTitle + g_aTitle[2].rot.y) * g_fLengthTitle / 6;
	//pVtx[3].pos.z = g_aTitle[2].pos.z + 0.0f;

	////rhw�̐ݒ�
	//pVtx[0].rhw = 1.0f;
	//pVtx[1].rhw = 1.0f;
	//pVtx[2].rhw = 1.0f;
	//pVtx[3].rhw = 1.0f;

	////���_�J���[�̐ݒ�
	//pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	////�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
	//pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	//pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	////���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffTitle->Unlock();
}

//============================================================================
//�Q�[���I�[�o�[�̕`�揈��
//============================================================================
void DrawGameOver(void)
{
	int nCntGameOver;
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

								//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffGameOver,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntGameOver = 0; nCntGameOver < MAX_GAMEOVER_IMAGE; nCntGameOver++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureGameOver[nCntGameOver]);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			4 * nCntGameOver,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
}