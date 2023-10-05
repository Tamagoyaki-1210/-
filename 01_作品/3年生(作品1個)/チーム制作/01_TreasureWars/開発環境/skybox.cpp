//==============================================================================================
//
// skybox.cpp
// Author : saito shian
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"main.h"
#include"skybox.h"
#include"renderer.h"
#include"application.h"
#include <assert.h>

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CSkyBox::CSkyBox(int nPriority) : CObject(nPriority)
{

}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CSkyBox::~CSkyBox()
{
}

//==============================================================================================
// �|���S���̏�����
//==============================================================================================
HRESULT CSkyBox::Init()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	m_pos = D3DXVECTOR3(0.0f, -250.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld;
	m_nHorizontal = 0;
	m_nVertical = 0;
	m_nVtx = 0;
	m_nIdx = 0;
	m_nPolygon = 0;

	// ���E�c�̏�����
	m_nHorizontal = START_SIDE;
	m_nVertical = START_VERTICAL;

	int nXLine = m_nHorizontal + 1;
	int nYLine = m_nVertical + 1;

	// ���_�����v�Z
	m_nVtx = nXLine * nYLine;

	// �C���f�b�N�X�����v�Z
	m_nIdx = ((nXLine * 2) * m_nVertical) + ((m_nVertical - 1) * 2);

	// �|���S�������v�Z
	m_nPolygon = (m_nHorizontal * m_nVertical * 2) + ((m_nVertical - 1) * 4);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx = NULL;		// ���_���ւ̃|�C���^

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < nYLine; y++)
	{
		float fYRot = (((D3DX_PI * 0.25f) / m_nVertical) * y) + (D3DX_PI * 0.25f);

		float fYPos = cosf(fYRot) * MAX_SIZE;

		for (int x = 0; x < nXLine; x++)
		{
			float fRot = ((D3DX_PI * 2.0f) / m_nHorizontal) * x;

			//���K��
			if (fRot > D3DX_PI)
			{
				fRot += D3DX_PI * 2;
			}
			if (fRot < -D3DX_PI)
			{
				fRot += -D3DX_PI * 2;
			}

			float fXPos = sinf(fRot) * sinf(fYRot) * MAX_SIZE;
			float fZPos = cosf(fRot) * sinf(fYRot) * MAX_SIZE;
			D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

			// ���_���W�̐ݒ�
			pVtx[x + (y * nXLine)].pos = pos;

			D3DXVECTOR3 vec;

			// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
			D3DXVec3Normalize(&vec, &pos);

			// �e���_�̖@���̐ݒ�
			pVtx[x + (y * nXLine)].nor = vec;

			// ���_�J���[�̐ݒ�
			pVtx[x + (y * nXLine)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			float fUTex = (1.0f / m_nHorizontal) * x;
			float fVTex = (1.0f / m_nVertical) * y;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[x + (y * nXLine)].tex = D3DXVECTOR2(fUTex, fVTex);
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * m_nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx = NULL;		// �C���f�b�N�X���ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (int x = 0, y = 0; y < m_nVertical; x++, y++)
	{
		for (; x < (nXLine * (y + 1)) + y; x++)
		{
			pIdx[x * 2] = (WORD)(x - y + nXLine);
			pIdx[(x * 2) + 1] = (WORD)(x - y);
			x = x;
		}

		if (y < m_nVertical - 1)
		{// ����ŏI��肶��Ȃ��Ȃ�
			pIdx[x * 2] = (WORD)(x - (y + 1));
			pIdx[(x * 2) + 1] = (WORD)((x * 2) - (y * (m_nHorizontal + 3)));
			x = x;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	// �~���̒��_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (m_nHorizontal + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffCone,
		NULL);

	// ���_�������b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffCone->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < nXLine; i++)
	{
		float fYRot = 100.0f;
		float fRot = ((D3DX_PI * 2.0f) / m_nHorizontal) * i;

		//���K��
		if (fRot > D3DX_PI)
		{
			fRot += D3DX_PI * 2;
		}
		if (fRot < -D3DX_PI)
		{
			fRot += -D3DX_PI * 2;
		}

		float fXPos = sinf(-fRot) * sinf(fYRot) * MAX_SIZE;
		float fYPos = cosf(fYRot) * MAX_SIZE;
		float fZPos = cosf(-fRot) * sinf(fYRot) * MAX_SIZE;
		D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

		// ���_���W�̐ݒ�
		pVtx[i + 1].pos = pos;

		D3DXVECTOR3 vec;

		// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
		D3DXVec3Normalize(&vec, &pos);

		// �e���_�̖@���̐ݒ�
		pVtx[i + 1].nor = vec;

		// ���_�J���[�̐ݒ�
		pVtx[i + 1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[i + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	float fYRot = ((D3DX_PI * 0.25f) / m_nVertical);

	float fYPos = cosf(fYRot) * MAX_SIZE;

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, fYPos, 0.0f);

	// ���_���W�̐ݒ�
	pVtx[0].pos = pos;

	D3DXVECTOR3 vec;

	// ���K������ ( �傫�� 1 �̃x�N�g���ɂ��� )
	D3DXVec3Normalize(&vec, &pos);

	// �e���_�̖@���̐ݒ�
	pVtx[0].nor = vec;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuffCone->Unlock();

	return S_OK;
}

//==============================================================================================
// �|���S���̏I��
//==============================================================================================
void CSkyBox::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//==============================================================================================
// �|���S���̍X�V
//==============================================================================================
void CSkyBox::Update()
{

}

//==============================================================================================
// �|���S���̕`��
//==============================================================================================
void CSkyBox::Draw()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture *pTexture = CApplication::GetTexture();

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// �|���S���`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�o�b�t�@
		0,							// �C���f�b�N�X�̍ŏ��l
		m_nVtx,						// ���_��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		m_nPolygon);				// �v���~�e�B�u(�|���S��)��

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffCone, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLEFAN,			// �v���~�e�B�u�̎��
		0,							// �`�悷��ŏ��̒��_�C���f�b�N�X
		m_nHorizontal);				// �v���~�e�B�u(�|���S��)��

									// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}

//==============================================================================================
// �ʒu�̐ݒ�
//==============================================================================================
void CSkyBox::Setposition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==============================================================================================
// �T�C�Y�̐ݒ�
//==============================================================================================
void CSkyBox::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//==============================================================================================
//�X�e�[�W���ƂɎg�p����w�i��ς���
//==============================================================================================
void CSkyBox::SetStageTexture(int nStageNum)
{
	switch (nStageNum)
	{
	case -1:
	case 0:
		m_texture = CTexture::TEXTURE_FLOOR;
		break;
	case 1:
		m_texture = CTexture::TEXTURE_WINDOW_OUTSIDE;
		break;
	case 2:
		m_texture = CTexture::TEXTURE_GALAXY;
		break;
	default:
		assert(false);
		break;
	}





}

//==============================================================================================
// ��������
//==============================================================================================
CSkyBox *CSkyBox::Create(D3DXVECTOR3 pos, int nStageNum)
{
	CSkyBox *pSkyBox = nullptr;

	pSkyBox = new CSkyBox(2);

	if (pSkyBox != nullptr)
	{
		pSkyBox->Init();
		pSkyBox->Setposition(pos);
		pSkyBox->SetStageTexture(nStageNum);
	}

	return pSkyBox;
}