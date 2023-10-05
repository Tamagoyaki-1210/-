//============================================================================
//
//�t�F�[�h����[fade.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "fade.h"		//�쐬����fade.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	//���_�o�b�t�@�ւ̃|�C���^

FADE g_fade;									//�t�F�[�h�̏��
MODE g_modeNext;								//���̉��(���[�h)
D3DXCOLOR g_colorFade;							//�|���S��(�t�F�[�h)�̐F
//============================================================================
//�t�F�[�h�̏���������
//============================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�t�F�[�h�C����Ԃ�
	g_fade = FADE_IN;

	//���̉��(���[�h)��ݒ�
	g_modeNext = modeNext;	

	//�����|���S��(�s����)�ɂ��Ă���
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4,	//sizeof(VERTEX_2D) * �K�v�Ȓ��_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL
	);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//�e�N�X�`�����W�̐ݒ�(0.0f ~ (1 / x�p�^�[����)f)
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();

	//���[�h�ݒ�(���̉�ʂɈڍs)
	SetMode(g_modeNext);
}

//============================================================================
//�t�F�[�h�̏I������
//============================================================================
void UninitFade(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//============================================================================
//�t�F�[�h�̍X�V����
//============================================================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{//�������Ă��Ȃ���ԈȊO�̏ꍇ�̏���
		if (g_fade == FADE_IN)
		{//�t�F�[�h�C�����(����������)
			if (g_colorFade.a <= 0.0f)
			{//�s�����x��0.0f�ɂȂ����ꍇ�̏���

				//�������Ă��Ȃ���Ԃɂ���
				g_fade = FADE_NONE;
			}
			else
			{//�s�����x��0.0f����̏ꍇ
				g_colorFade.a -= (1.0f / 60.0f);	//�|���S���𓧖��ɂ��Ă���
			}
		}
		else if (g_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���(�s����������)
			if (g_colorFade.a >= 1.0f)
			{//�s�����x��1.0f�ɂȂ����ꍇ�̏���

				//�t�F�[�h�C�����(������)�ɂ���
				g_fade = FADE_IN;			

				//���[�h�ݒ�(���̉�ʂɈڍs)
				SetMode(g_modeNext);			
			}
			else
			{//�s�����x��1.0f�����̏ꍇ
				g_colorFade.a += (1.0f / 60.0f);	//�|���S����s�����ɂ��Ă���
			}
		}
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffFade->Unlock();
	}
}

//============================================================================
//�t�F�[�h�̕`�揈��
//============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffFade,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)	//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�(���g�p�̏ꍇNULL���w��)
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
		2						//�v���~�e�B�u�A�C�R���̌�
	);
}

//============================================================================
//�t�F�[�h�̐ݒ菈��
//============================================================================
void SetFade(MODE modeNext)
{
	//�t�F�[�h�A�E�g��Ԃɂ���
	g_fade = FADE_OUT;

	//���̉��(���[�h)��ݒ�
	g_modeNext = modeNext;	

	//�����|���S��(����)�ɂ��Ă���
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//============================================================================
//�t�F�[�h�̎擾����
//============================================================================
FADE GetFade(void)
{
	return g_fade;
}