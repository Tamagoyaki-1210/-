//============================================================================
//
//�^�C�g���̏���[title.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "main.h"		//�쐬����main.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "title.h"		//�쐬����title.h���C���N���[�h����
#include "fade.h"		//�쐬����fade.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[MAX_TITLE_IMAGE];	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			//���_�o�b�t�@�ւ̃|�C���^
Title g_aTitle[MAX_TITLE_IMAGE];						//�^�C�g���̏��

int g_nCounterTitleState = 0;							//��ԊǗ��J�E���^�[
bool TitleCounterUse = false;

//============================================================================
//�^�C�g���̏���������
//============================================================================
void InitTitle(void)
{
	int nCntTitle;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\dangohazard.png",			//�e�N�X�`���̃t�@�C����
		&g_pTextureTitle[0]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\press_enter.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureTitle[1]
	);

	//�^�C�g��_�w�i
	g_aTitle[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1.0f);		//�ʒu�̏���������
	g_aTitle[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//��]������������
	g_aTitle[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aTitle[0].fAngle = atan2f(TITLE_BG_WIDTH, TITLE_BG_HEIGHT);
	g_aTitle[0].fLength = sqrtf(TITLE_BG_WIDTH * TITLE_BG_WIDTH + TITLE_BG_HEIGHT * TITLE_BG_HEIGHT) / 2.0f;

	//�^�C�g��_ENTER
	g_aTitle[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.25f, 0.0f);	//�ʒu�̏���������
	g_aTitle[1].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//��]������������
	g_aTitle[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aTitle[1].fAngle = atan2f(TITLE_ENTER_WIDTH, TITLE_ENTER_HEIGHT);
	g_aTitle[1].fLength = sqrtf(TITLE_ENTER_WIDTH * TITLE_ENTER_WIDTH + TITLE_ENTER_HEIGHT * TITLE_ENTER_HEIGHT) / 2.0f;

	//�^�C�g��_���S
	g_aTitle[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f, 0.0f);	//�ʒu�̏���������
	g_aTitle[2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aTitle[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//��]������������
	g_aTitle[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aTitle[2].fAngle = atan2f(TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT);
	g_aTitle[2].fLength = sqrtf(TITLE_LOGO_WIDTH * TITLE_LOGO_WIDTH + TITLE_LOGO_HEIGHT * TITLE_LOGO_HEIGHT) * 2 / 3;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_TITLE_IMAGE,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_IMAGE; nCntTitle++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aTitle[nCntTitle].pos.x - sinf(g_aTitle[nCntTitle].fAngle + g_aTitle[nCntTitle].rot.x) * g_aTitle[nCntTitle].fLength;
		pVtx[0].pos.y = g_aTitle[nCntTitle].pos.y - cosf(g_aTitle[nCntTitle].fAngle + g_aTitle[nCntTitle].rot.y) * g_aTitle[nCntTitle].fLength;
		pVtx[0].pos.z = g_aTitle[nCntTitle].pos.z + 0.0f;

		pVtx[1].pos.x = g_aTitle[nCntTitle].pos.x + sinf(g_aTitle[nCntTitle].fAngle - g_aTitle[nCntTitle].rot.x) * g_aTitle[nCntTitle].fLength;
		pVtx[1].pos.y = g_aTitle[nCntTitle].pos.y - cosf(g_aTitle[nCntTitle].fAngle - g_aTitle[nCntTitle].rot.y) * g_aTitle[nCntTitle].fLength;
		pVtx[1].pos.z = g_aTitle[nCntTitle].pos.z + 0.0f;

		pVtx[2].pos.x = g_aTitle[nCntTitle].pos.x - sinf(g_aTitle[nCntTitle].fAngle - g_aTitle[nCntTitle].rot.x) * g_aTitle[nCntTitle].fLength;
		pVtx[2].pos.y = g_aTitle[nCntTitle].pos.y + cosf(g_aTitle[nCntTitle].fAngle - g_aTitle[nCntTitle].rot.y) * g_aTitle[nCntTitle].fLength;
		pVtx[2].pos.z = g_aTitle[nCntTitle].pos.z + 0.0f;

		pVtx[3].pos.x = g_aTitle[nCntTitle].pos.x + sinf(g_aTitle[nCntTitle].fAngle + g_aTitle[nCntTitle].rot.x) * g_aTitle[nCntTitle].fLength;
		pVtx[3].pos.y = g_aTitle[nCntTitle].pos.y + cosf(g_aTitle[nCntTitle].fAngle + g_aTitle[nCntTitle].rot.y) * g_aTitle[nCntTitle].fLength;
		pVtx[3].pos.z = g_aTitle[nCntTitle].pos.z + 0.0f;

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
	g_pVtxBuffTitle->Unlock();
}

//============================================================================
//�^�C�g���̏I������
//============================================================================
void UninitTitle(void)
{
	int nCntTitle;

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_IMAGE; nCntTitle++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTitle[nCntTitle] != NULL)
		{
			g_pTextureTitle[nCntTitle]->Release();
			g_pTextureTitle[nCntTitle] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}

//============================================================================
//�^�C�g���̍X�V����
//============================================================================
void UpdateTitle(void)
{
	FADE fade;
	fade = GetFade();

	if (fade == FADE_NONE)
	{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
		 //Enter�L�[���̓X�y�[�X�L�[�������ꂽ�ꍇ
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			//���[�h�ݒ�
			SetFade(MODE_GAME);

			TitleCounterUse = true;
		}
	}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Enter�������ꂽ��̕b���𐔂��鏈��
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (g_nCounterTitleState >= 60)
	{//60f(�t���[��)��̏���
		TitleCounterUse = false;
		g_nCounterTitleState = 0;
	}
	else if (TitleCounterUse == true)
	{//Enter��������Ă���̎��Ԃ𐔂��鏈��
		g_nCounterTitleState++;
	}
}

//============================================================================
//�^�C�g���̕`�揈��
//============================================================================
void DrawTitle(void)
{
	int nCntTitle;
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffTitle,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_IMAGE; nCntTitle++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			4 * nCntTitle,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
}