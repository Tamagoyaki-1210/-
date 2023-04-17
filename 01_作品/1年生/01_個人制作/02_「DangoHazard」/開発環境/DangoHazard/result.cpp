//============================================================================
//
//���U���g��ʂ̏���[result.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "input.h"		//�쐬����input.h���C���N���[�h����
#include "result.h"		//�쐬����result.h���C���N���[�h����
#include "fade.h"		//�쐬����fade.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT_IMAGE];	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Result g_aResult[MAX_RESULT_IMAGE];						//���U���g��ʂ̏��

int g_nCounterResultState = 0;						//��ԊǗ��J�E���^�[

//============================================================================
//���U���g��ʂ̏���������
//============================================================================
void InitResult(void)
{
	int nCntResult;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//�e�N�X�`���̃t�@�C����
		&g_pTextureResult[0]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\press_enter.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureResult[1]
	);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\GAMECLEAR.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureResult[2]
	);

	//�\���̂̏���������
	g_aResult[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);	//�ʒu�̏���������
	g_aResult[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//����������������
	g_aResult[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aResult[0].fAngle = atan2f(RESULT_BG_WIDTH, RESULT_BG_HEIGHT);
	g_aResult[0].fLength = sqrtf(RESULT_BG_WIDTH * RESULT_BG_WIDTH + RESULT_BG_HEIGHT * RESULT_BG_HEIGHT) / 2.0f;

	g_aResult[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 1.25f, 0.0f);	//�ʒu�̏���������
	g_aResult[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//����������������
	g_aResult[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aResult[1].fAngle = atan2f(RESULT_ENTER_WIDTH, RESULT_ENTER_HEIGHT);
	g_aResult[1].fLength = sqrtf(RESULT_ENTER_WIDTH *RESULT_ENTER_WIDTH + RESULT_ENTER_HEIGHT * RESULT_ENTER_HEIGHT) / 2.0f;

	g_aResult[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);	//�ʒu�̏���������
	g_aResult[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//����������������
	g_aResult[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aResult[2].fAngle = atan2f(RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT);
	g_aResult[2].fLength = sqrtf(RESULT_LOGO_WIDTH * RESULT_LOGO_WIDTH + RESULT_LOGO_HEIGHT * RESULT_LOGO_HEIGHT) / 2.0f;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_RESULT_IMAGE,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResult = 0; nCntResult < MAX_RESULT_IMAGE; nCntResult++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aResult[nCntResult].pos.x - sinf(g_aResult[nCntResult].fAngle + g_aResult[nCntResult].rot.x) * g_aResult[nCntResult].fLength;
		pVtx[0].pos.y = g_aResult[nCntResult].pos.y - cosf(g_aResult[nCntResult].fAngle + g_aResult[nCntResult].rot.y) * g_aResult[nCntResult].fLength;
		pVtx[0].pos.z = g_aResult[nCntResult].pos.z + 0.0f;

		pVtx[1].pos.x = g_aResult[nCntResult].pos.x + sinf(g_aResult[nCntResult].fAngle - g_aResult[nCntResult].rot.x) * g_aResult[nCntResult].fLength;
		pVtx[1].pos.y = g_aResult[nCntResult].pos.y - cosf(g_aResult[nCntResult].fAngle - g_aResult[nCntResult].rot.y) * g_aResult[nCntResult].fLength;
		pVtx[1].pos.z = g_aResult[nCntResult].pos.z + 0.0f;

		pVtx[2].pos.x = g_aResult[nCntResult].pos.x - sinf(g_aResult[nCntResult].fAngle - g_aResult[nCntResult].rot.x) * g_aResult[nCntResult].fLength;
		pVtx[2].pos.y = g_aResult[nCntResult].pos.y + cosf(g_aResult[nCntResult].fAngle - g_aResult[nCntResult].rot.y) * g_aResult[nCntResult].fLength;
		pVtx[2].pos.z = g_aResult[nCntResult].pos.z + 0.0f;

		pVtx[3].pos.x = g_aResult[nCntResult].pos.x + sinf(g_aResult[nCntResult].fAngle + g_aResult[nCntResult].rot.x) * g_aResult[nCntResult].fLength;
		pVtx[3].pos.y = g_aResult[nCntResult].pos.y + cosf(g_aResult[nCntResult].fAngle + g_aResult[nCntResult].rot.y) * g_aResult[nCntResult].fLength;
		pVtx[3].pos.z = g_aResult[nCntResult].pos.z + 0.0f;

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
	g_pVtxBuffResult->Unlock();
}

//============================================================================
//���U���g��ʂ̏I������
//============================================================================
void UninitResult(void)
{
	int nCntResult;

	for (nCntResult = 0; nCntResult < MAX_RESULT_IMAGE; nCntResult++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//============================================================================
//���U���g��ʂ̍X�V����
//============================================================================
void UpdateResult(void)
{
	FADE fade;			
	fade = GetFade();

	if (fade == FADE_NONE)
	{//�t�F�[�h���s���ĂȂ��ꍇ�̏���
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//Enter�L�[���̓X�y�[�X�L�[�������ꂽ�ꍇ
		 //���[�h�ݒ�
			SetFade(MODE_TITLE);
		}
	}
}

//============================================================================
//���U���g��ʂ̕`�揈��
//============================================================================
void DrawResult(void)
{
	int nCntResult;
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffResult,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntResult = 0; nCntResult < MAX_RESULT_IMAGE; nCntResult++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResult[nCntResult]);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			4 * nCntResult,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
}