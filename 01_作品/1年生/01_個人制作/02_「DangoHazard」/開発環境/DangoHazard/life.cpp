//============================================================================
//
//���C�t�̏���[life.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "life.h"		//�쐬����life.h���C���N���[�h����
#include "player.h"		//�쐬����player.h���C���N���[�h����

LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Life g_aLife[MAX_PLAYER_LIFE];						//���C�t�̏��

//============================================================================
//���C�t�̏���������
//============================================================================
void InitLife(void)
{
	int nCntLife;
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^

	pDevice = GetDevice();				//�f�o�C�X�̎擾

	//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\life000.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureLife
	);
	//���C�t�̏��̏�����
	for (nCntLife = 0; nCntLife < MAX_PLAYER_LIFE; nCntLife++)
	{
		//�ʒu�̏���������
		g_aLife[nCntLife].pos = D3DXVECTOR3(SCREEN_WIDTH + (LIFE_WIDTH * nCntLife) - (LIFE_WIDTH * PLAYER_LIFE),
			LIFE_HEIGHT, 0.0f);		//�ʒu
		g_aLife[nCntLife].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//��]
		g_aLife[nCntLife].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//�F
		g_aLife[nCntLife].fAngle = atan2f(LIFE_WIDTH, LIFE_HEIGHT);
		g_aLife[nCntLife].fLength = sqrtf(LIFE_WIDTH * LIFE_WIDTH + LIFE_HEIGHT * LIFE_HEIGHT) / 2.0f;
		g_aLife[nCntLife].bUse = true;								//���C�t�g�p����
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PLAYER_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLife = 0; nCntLife < MAX_PLAYER_LIFE; nCntLife++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aLife[nCntLife].pos.x - sinf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[0].pos.y = g_aLife[nCntLife].pos.y - cosf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[0].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[1].pos.x = g_aLife[nCntLife].pos.x + sinf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[1].pos.y = g_aLife[nCntLife].pos.y - cosf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[1].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[2].pos.x = g_aLife[nCntLife].pos.x - sinf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[2].pos.y = g_aLife[nCntLife].pos.y + cosf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[2].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[3].pos.x = g_aLife[nCntLife].pos.x + sinf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[3].pos.y = g_aLife[nCntLife].pos.y + cosf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[3].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

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
	g_pVtxBuffLife->Unlock();
}

//============================================================================
//���C�t�̏I������
//============================================================================
void UninitLife(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//============================================================================
//���C�t�̍X�V����
//============================================================================
void UpdateLife(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//�ǉ����ꂽ���C�t��\�����鏈��
	for (int nCntAddLife = 0; nCntAddLife < (pPlayer->nLife / PLAYER_LIFE) + 1 ; nCntAddLife++)
	{
		for (int nCntTreeLife = 0; nCntTreeLife < PLAYER_LIFE; nCntTreeLife++)
		{
			g_aLife[(nCntAddLife * PLAYER_LIFE) + nCntTreeLife].pos = D3DXVECTOR3(SCREEN_WIDTH - (LIFE_WIDTH * nCntTreeLife) - (LIFE_WIDTH * PLAYER_LIFE),
				LIFE_HEIGHT * (nCntAddLife + 1), 0.0f);
			g_aLife[(nCntAddLife * PLAYER_LIFE) + nCntTreeLife].bUse = true;
		}
	}
	for (int nCntLife = 0; nCntLife < MAX_PLAYER_LIFE; nCntLife++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = g_aLife[nCntLife].pos.x - sinf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[0].pos.y = g_aLife[nCntLife].pos.y - cosf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[0].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[1].pos.x = g_aLife[nCntLife].pos.x + sinf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[1].pos.y = g_aLife[nCntLife].pos.y - cosf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[1].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[2].pos.x = g_aLife[nCntLife].pos.x - sinf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[2].pos.y = g_aLife[nCntLife].pos.y + cosf(0.0f + g_aLife[nCntLife].fAngle - g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[2].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx[3].pos.x = g_aLife[nCntLife].pos.x + sinf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.x) * g_aLife[nCntLife].fLength;
		pVtx[3].pos.y = g_aLife[nCntLife].pos.y + cosf(0.0f + g_aLife[nCntLife].fAngle + g_aLife[nCntLife].rot.y) * g_aLife[nCntLife].fLength;
		pVtx[3].pos.z = g_aLife[nCntLife].pos.z + 0.0f;

		pVtx += 4;
	}
	//���C�t��������������false�ɂ���
	for (int nCntPLife = pPlayer->nLife; nCntPLife < MAX_PLAYER_LIFE; nCntPLife++)
	{
		g_aLife[nCntPLife].bUse = false;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
	
}
//============================================================================
//���C�t�̕`�揈��
//============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	int nCntLife;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffLife,		//���_�o�b�t�@�ւ̃|�C���^
		0,
		sizeof(VERTEX_2D)		//���_���\���̂̃T�C�Y
	);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	for (nCntLife = 0; nCntLife < MAX_PLAYER_LIFE; nCntLife++)
	{
		if (g_aLife[nCntLife].bUse == true)
		{//�e���g�p����Ă��� 
		 //�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntLife * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
				2						//�v���~�e�B�u�A�C�R���̌�
			);
		}
	}
}