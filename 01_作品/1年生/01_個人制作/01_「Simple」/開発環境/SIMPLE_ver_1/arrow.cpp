//============================================================================
//
//���̏���[arrow.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "arrow.h"		//�쐬����life.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����
#include "item.h"		//�쐬����item.h���C���N���[�h����
#include "enemy.h"		//�쐬����enemy.h���C���N���[�h����

LPDIRECT3DTEXTURE9 g_pTextureArrow = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffArrow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
ARROW g_aArrow[MAX_ARROW];						//���̏��

//============================================================================
//���̏���������
//============================================================================
void InitArrow(void)
{
	int nCntArrow;
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();				//�f�o�C�X�̎擾

										//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\Arrow.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureArrow
	);
		//�ʒu�̏���������
		g_aArrow[0].pos = D3DXVECTOR3(ARROW_WIDTH / 2, ARROW_HEIGHT / 2 * 3, 0.5f);		//�ʒu
		g_aArrow[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//��]
		g_aArrow[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F
		g_aArrow[0].fAngle = atan2f(ARROW_WIDTH, ARROW_HEIGHT);
		g_aArrow[0].fLength = sqrtf(ARROW_WIDTH * ARROW_WIDTH + ARROW_HEIGHT * ARROW_HEIGHT) / 2.0f;
		g_aArrow[0].bUse = true;								//���C�t�g�p����

		g_aArrow[1].pos = D3DXVECTOR3(SCREEN_WIDTH - ARROW_WIDTH / 2, ARROW_HEIGHT / 2 * 3, 0.5f);		//�ʒu
		g_aArrow[1].rot = D3DXVECTOR3(3.14f, 3.14f, 0.0f);		//��]
		g_aArrow[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F
		g_aArrow[1].fAngle = atan2f(ARROW_WIDTH, ARROW_HEIGHT);
		g_aArrow[1].fLength = sqrtf(ARROW_WIDTH * ARROW_WIDTH + ARROW_HEIGHT * ARROW_HEIGHT) / 2.0f;
		g_aArrow[1].bUse = true;								//���C�t�g�p����

		g_aArrow[2].pos = D3DXVECTOR3(ARROW_WIDTH / 2, SCREEN_HEIGHT - ARROW_HEIGHT / 2, 0.5f);		//�ʒu
		g_aArrow[2].rot = D3DXVECTOR3(3.14f, 3.14f, 0.0f);		//��]
		g_aArrow[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F
		g_aArrow[2].fAngle = atan2f(ARROW_WIDTH, ARROW_HEIGHT);
		g_aArrow[2].fLength = sqrtf(ARROW_WIDTH * ARROW_WIDTH + ARROW_HEIGHT * ARROW_HEIGHT) / 2.0f;
		g_aArrow[2].bUse = true;								//���C�t�g�p����

		g_aArrow[3].pos = D3DXVECTOR3(SCREEN_WIDTH - ARROW_WIDTH / 2, SCREEN_HEIGHT - ARROW_HEIGHT / 2, 0.5f);		//�ʒu
		g_aArrow[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//��]
		g_aArrow[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F
		g_aArrow[3].fAngle = atan2f(ARROW_WIDTH, ARROW_HEIGHT);
		g_aArrow[3].fLength = sqrtf(ARROW_WIDTH * ARROW_WIDTH + ARROW_HEIGHT * ARROW_HEIGHT) / 2.0f;
		g_aArrow[3].bUse = true;								//���C�t�g�p����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ARROW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffArrow,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aArrow[nCntArrow].pos.x - sinf(0.0f + g_aArrow[nCntArrow].fAngle + g_aArrow[nCntArrow].rot.x) * g_aArrow[nCntArrow].fLength;
		pVtx[0].pos.y = g_aArrow[nCntArrow].pos.y - cosf(0.0f + g_aArrow[nCntArrow].fAngle + g_aArrow[nCntArrow].rot.y) * g_aArrow[nCntArrow].fLength;
		pVtx[0].pos.z = g_aArrow[nCntArrow].pos.z + 0.0f;

		pVtx[1].pos.x = g_aArrow[nCntArrow].pos.x + sinf(0.0f + g_aArrow[nCntArrow].fAngle - g_aArrow[nCntArrow].rot.x) * g_aArrow[nCntArrow].fLength;
		pVtx[1].pos.y = g_aArrow[nCntArrow].pos.y - cosf(0.0f + g_aArrow[nCntArrow].fAngle - g_aArrow[nCntArrow].rot.y) * g_aArrow[nCntArrow].fLength;
		pVtx[1].pos.z = g_aArrow[nCntArrow].pos.z + 0.0f;

		pVtx[2].pos.x = g_aArrow[nCntArrow].pos.x - sinf(0.0f + g_aArrow[nCntArrow].fAngle - g_aArrow[nCntArrow].rot.x) * g_aArrow[nCntArrow].fLength;
		pVtx[2].pos.y = g_aArrow[nCntArrow].pos.y + cosf(0.0f + g_aArrow[nCntArrow].fAngle - g_aArrow[nCntArrow].rot.y) * g_aArrow[nCntArrow].fLength;
		pVtx[2].pos.z = g_aArrow[nCntArrow].pos.z + 0.0f;

		pVtx[3].pos.x = g_aArrow[nCntArrow].pos.x + sinf(0.0f + g_aArrow[nCntArrow].fAngle + g_aArrow[nCntArrow].rot.x) * g_aArrow[nCntArrow].fLength;
		pVtx[3].pos.y = g_aArrow[nCntArrow].pos.y + cosf(0.0f + g_aArrow[nCntArrow].fAngle + g_aArrow[nCntArrow].rot.y) * g_aArrow[nCntArrow].fLength;
		pVtx[3].pos.z = g_aArrow[nCntArrow].pos.z + 0.0f;

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
	g_pVtxBuffArrow->Unlock();
}

//============================================================================
//���̏I������
//============================================================================
void UninitArrow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureArrow != NULL)
	{
		g_pTextureArrow->Release();
		g_pTextureArrow = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffArrow != NULL)
	{
		g_pVtxBuffArrow->Release();
		g_pVtxBuffArrow = NULL;
	}
}

//============================================================================
//���̍X�V����
//============================================================================
void UpdateArrow(void)
{}
//============================================================================
//���̕`�揈��
//============================================================================
void DrawArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntArrow;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffArrow,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureArrow);

	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_aArrow[nCntArrow].bUse == true)
		{//�e���g�p����Ă��� 
		 //�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntArrow * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}
}