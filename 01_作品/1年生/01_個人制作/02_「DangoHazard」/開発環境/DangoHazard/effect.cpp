//============================================================================
//
//�G�t�F�N�g�̏���[effect.cpp] (�t�@�C���w�b�_�R�����g)
//Author:�J�{�N��
//============================================================================
#include "effect.h"		//�쐬����effect.h���C���N���[�h����
#include "input.h"		//�쐬����input.h���C���N���[�h����

//****************************************************************************
//�O���[�o���ϐ�
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//���_�o�b�t�@�ւ̃|�C���^
EFFECT g_aEffect[MAX_EFFECT];

//============================================================================
//�G�t�F�N�g�̏���������
//============================================================================
void InitEffect(void)
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X�̎擾

										//�e�N�X�`���[�̓ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\effect\\Flash01.png",	//�e�N�X�`���̃t�@�C����
		&g_pTextureEffect
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//�ʒu�̏���������
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].bUse = false;
		g_aEffect[nCntEffect].life = 0;
		g_aEffect[nCntEffect].fAngle = atan2f(EFFECT_WIDTH, EFFECT_HEIGHT);
		g_aEffect[nCntEffect].fLength = sqrtf(EFFECT_WIDTH * EFFECT_WIDTH + EFFECT_HEIGHT * EFFECT_HEIGHT) / 2.0f;
		g_aEffect[nCntEffect].Worldmtx = D3DXMATRIX();
		g_aEffect[nCntEffect].type = EFFECT_TYPE_BULLET;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
		pVtx[0].pos.x = -sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = cosf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

		pVtx[1].pos.x = sinf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = cosf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

		pVtx[2].pos.x = -sinf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = -cosf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

		pVtx[3].pos.x = sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
		pVtx[3].pos.y = 0.0f;
		pVtx[3].pos.z = -cosf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

		//�e���_�̖@���̐ݒ�(���x�N�g���̑傫����1�ɂ���K�v������)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	g_pVtxBuffEffect->Unlock();

}

//============================================================================
//�G�t�F�N�g�̏I������
//============================================================================
void UninitEffect(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;

	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//============================================================================
//�G�t�F�N�g�̍X�V����
//============================================================================
void UpdateEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//�ړ��ʔ��f
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			g_aEffect[nCntEffect].life--;
			if (g_aEffect[nCntEffect].life <= 0)
			{
				g_aEffect[nCntEffect].life = 0;
				g_aEffect[nCntEffect].bUse = false;
			}

			//���_���ւ̃|�C���^
			VERTEX_3D *pVtx = NULL;

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ� = (�z�u�ʒu �} ����(�Ίp���̊p�x �} ����) * �Ίp���̒���)
			pVtx[0].pos.x = -sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = cosf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

			pVtx[1].pos.x = sinf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
			pVtx[1].pos.y = 0.0f;
			pVtx[1].pos.z = cosf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

			pVtx[2].pos.x = -sinf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
			pVtx[2].pos.y = 0.0f;
			pVtx[2].pos.z = -cosf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

			pVtx[3].pos.x = sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
			pVtx[3].pos.y = 0.0f;
			pVtx[3].pos.z = -cosf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;
			pVtx += 4;

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffEffect->Unlock();
		}
	}
}

//============================================================================
//�G�t�F�N�g�̕`�揈��
//============================================================================
void DrawEffect(void)
{
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect);

			//a�u�����f�B���O�����Z�����̐ݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//���Z�����̐ݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].Worldmtx);

			//�J�����̋t�s���ݒ�
			g_aEffect[nCntEffect].Worldmtx._11 = mtxView._11;
			g_aEffect[nCntEffect].Worldmtx._12 = mtxView._21;
			g_aEffect[nCntEffect].Worldmtx._13 = mtxView._31;

			if (g_aEffect[nCntEffect].type == EFFECT_TYPE_BULLET)
			{
				g_aEffect[nCntEffect].Worldmtx._21 = mtxView._12;
				g_aEffect[nCntEffect].Worldmtx._22 = mtxView._22;
				g_aEffect[nCntEffect].Worldmtx._23 = mtxView._32;
			}

			g_aEffect[nCntEffect].Worldmtx._31 = mtxView._13;
			g_aEffect[nCntEffect].Worldmtx._32 = mtxView._23;
			g_aEffect[nCntEffect].Worldmtx._33 = mtxView._33;

			//�����𔽉f(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffect[nCntEffect].rot.y, g_aEffect[nCntEffect].rot.x, g_aEffect[nCntEffect].rot.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].Worldmtx, &g_aEffect[nCntEffect].Worldmtx, &mtxRot);
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].Worldmtx, &g_aEffect[nCntEffect].Worldmtx, &mtxTrans);

			//�A���t�@�e�X�g��L��
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].Worldmtx);

			//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//Z�o�b�t�@��`�悷��
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//Z�o�b�t�@�̒l����O��Z�o�b�t�@�̒l��菬�����ꍇ
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//�A���t�@�e�X�g�𖳌��ɖ߂�
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}
	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================================
//�G�t�F�N�g�̐ݒ菈��
//============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int life, EFFECT_TYPE type)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].rot = rot;
			g_aEffect[nCntEffect].life = life;
			g_aEffect[nCntEffect].type = type;

			g_aEffect[nCntEffect].bUse = true;
			break;
		}
	}
}