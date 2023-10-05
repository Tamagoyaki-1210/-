//============================================================================
//
//�w�i�̏���[background.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "background.h"	//�쐬����background.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBg[MAX_BG] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;
Background g_aBg[MAX_BG];	//�w�i�̏��

//============================================================================
//�w�i�̏���������
//============================================================================
void InitBg(void)
{
	int nCntBg;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

		//�e�N�X�`���̓ǂݍ���1
		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\sky000.jpg",	//�e�N�X�`���̃t�@�C����
			&g_pTextureBg[0]
		);

		//�e�N�X�`���̓ǂݍ���2
		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\wood.png",	//�e�N�X�`���̃t�@�C����
			&g_pTextureBg[1]
		);

		//�e�N�X�`���̓ǂݍ���1
		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\frame_score.png",	//�e�N�X�`���̃t�@�C����
			&g_pTextureBg[2]
		);

		//�e�N�X�`���̓ǂݍ���2
		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\frame_timer001.png",	//�e�N�X�`���̃t�@�C����
			&g_pTextureBg[3]
		);

		//�\���̂̏���������
		g_aBg[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 , 2.0f);
		g_aBg[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[0].nPatternAnim = 0;
		g_aBg[0].fAngle = atan2f(SCREEN_WIDTH, SCREEN_HEIGHT);
		g_aBg[0].fLength = sqrtf(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT) / 2.0f;

		g_aBg[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT /2, 1.0f);
		g_aBg[1].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[1].nPatternAnim = 0;
		g_aBg[1].fAngle = atan2f(SCREEN_WIDTH, SCREEN_HEIGHT);
		g_aBg[1].fLength = sqrtf(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT) / 2.0f;

		g_aBg[2].pos = D3DXVECTOR3(BG_SCORE_WIDTH / 2, BG_SCORE_HEIGHT / 2, 2.0f);
		g_aBg[2].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[2].nPatternAnim = 0;
		g_aBg[2].fAngle = atan2f(BG_SCORE_WIDTH, BG_SCORE_HEIGHT);
		g_aBg[2].fLength = sqrtf(BG_SCORE_WIDTH * BG_SCORE_WIDTH + BG_SCORE_HEIGHT * BG_SCORE_HEIGHT) / 2.0f;

		g_aBg[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, BG_TIMER_HEIGHT / 2, 1.0f);
		g_aBg[3].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBg[3].nPatternAnim = 0;
		g_aBg[3].fAngle = atan2f(BG_TIMER_WIDTH, BG_TIMER_HEIGHT);
		g_aBg[3].fLength = sqrtf(BG_TIMER_WIDTH * BG_TIMER_WIDTH + BG_TIMER_HEIGHT * BG_TIMER_HEIGHT) / 2.0f;
	
	//���_���̐ݒ�
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aBg[nCntBg].pos.x - sinf(g_aBg[nCntBg].fAngle + g_aBg[nCntBg].rot.x) * g_aBg[nCntBg].fLength;
		pVtx[0].pos.y = g_aBg[nCntBg].pos.y - cosf(g_aBg[nCntBg].fAngle + g_aBg[nCntBg].rot.y) * g_aBg[nCntBg].fLength;
		pVtx[0].pos.z = g_aBg[nCntBg].pos.z + 0.0f;

		pVtx[1].pos.x = g_aBg[nCntBg].pos.x + sinf(g_aBg[nCntBg].fAngle - g_aBg[nCntBg].rot.x) * g_aBg[nCntBg].fLength;
		pVtx[1].pos.y = g_aBg[nCntBg].pos.y - cosf(g_aBg[nCntBg].fAngle - g_aBg[nCntBg].rot.y) * g_aBg[nCntBg].fLength;
		pVtx[1].pos.z = g_aBg[nCntBg].pos.z + 0.0f;

		pVtx[2].pos.x = g_aBg[nCntBg].pos.x - sinf(g_aBg[nCntBg].fAngle - g_aBg[nCntBg].rot.x) * g_aBg[nCntBg].fLength;
		pVtx[2].pos.y = g_aBg[nCntBg].pos.y + cosf(g_aBg[nCntBg].fAngle - g_aBg[nCntBg].rot.y) * g_aBg[nCntBg].fLength;
		pVtx[2].pos.z = g_aBg[nCntBg].pos.z + 0.0f;

		pVtx[3].pos.x = g_aBg[nCntBg].pos.x + sinf(g_aBg[nCntBg].fAngle + g_aBg[nCntBg].rot.x) * g_aBg[nCntBg].fLength;
		pVtx[3].pos.y = g_aBg[nCntBg].pos.y + cosf(g_aBg[nCntBg].fAngle + g_aBg[nCntBg].rot.y) * g_aBg[nCntBg].fLength;
		pVtx[3].pos.z = g_aBg[nCntBg].pos.z + 0.0f;

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}

//============================================================================
//�w�i�̏I������
//============================================================================
void UninitBg(void)
{
	int nCntBg;

	for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBg[nCntBg] != NULL)
		{
			g_pTextureBg[nCntBg]->Release();
			g_pTextureBg[nCntBg] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//============================================================================
//�w�i�̍X�V����
//============================================================================
void UpdateBg(void)
{
	//���_���̐ݒ�
	VERTEX_2D *pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//�A�j���[�V�����̏���
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	g_aBg[0].nPatternAnim++;			//�p�^�[��No.���X�V����

	if (g_aBg[0].nPatternAnim >= BG_ANIM_CNT)
	{//���p�^�[�����𒴂������̏���
		g_aBg[0].nPatternAnim = 0;	//�p�^�[��No.�������l�ɖ߂�
	}

	//�e�N�X�`�����W�̐ݒ�:X���W = (1.0f / x�p�^�[����) * (�p�^�[��No. % x�p�^�[����), Y���W = (1.0f / y�p�^�[����) * (�p�^�[��No. / x�p�^�[����) : int�^�Ȃ̂ŏ����_�͔��f����Ȃ�
	pVtx[0].tex = D3DXVECTOR2((1.0f / BG_ANIM_CNT) * (g_aBg[0].nPatternAnim % BG_ANIM_CNT + 0.0f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((1.0f / BG_ANIM_CNT) * (g_aBg[0].nPatternAnim % BG_ANIM_CNT + 1.0f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((1.0f / BG_ANIM_CNT) * (g_aBg[0].nPatternAnim % BG_ANIM_CNT + 0.0f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / BG_ANIM_CNT) * (g_aBg[0].nPatternAnim % BG_ANIM_CNT + 1.0f), 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}

//============================================================================
//�w�i�̕`�揈��
//============================================================================
void DrawBg(void)
{
	int nCntBg;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBg,			//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBg = 0; nCntBg < MAX_BG; nCntBg++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBg[nCntBg]);

		//�|���S���̕`��
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntBg * 4,				//�`�悷��ŏ��̒��_�C���f�b�N�X
			2						//�v���~�e�B�u�A�C�R���̌�
		);
	}
}