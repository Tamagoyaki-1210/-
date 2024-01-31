//=============================================================================
//
// 3DPolgon.cpp
// Author : koduna hirohito
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "3Dpolygon.h"
#include"renderer.h"
#include"application.h"
#include <assert.h>

const float C3DPolygon::FLASHING_AMOUNT = 0.02f;
//*****************************************************************************
// �R���X�g���N�^
//*****************************************************************************
C3DPolygon::C3DPolygon() : CObject(0)
{
	m_TextIndex = CTexture::TEXTURE_NUMBER;
	m_pVtxBuff = nullptr;//���_�o�b�t�@�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_UV = D3DXVECTOR2(0.5f, 0.5f);
	m_UVMove = D3DXVECTOR2(0.0f, 0.0f);
	m_UVSize = D3DXVECTOR2(0.5f, 0.5f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nFlashingCnt = 0;
}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
C3DPolygon::~C3DPolygon()
{
}

//=============================================================================
// �|���S���̏�����
//=============================================================================
HRESULT C3DPolygon::Init()
{
	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = CApplication::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e���_�̖@���̐ݒ�i�x�N�g���̑傫���͂P�ɂ���K�v������j
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I��
//=============================================================================
void C3DPolygon::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// �|���S���̍X�V
//=============================================================================
void C3DPolygon::Update()
{
	m_Pos += m_Move;
	m_UV += m_UVMove;

	if (m_nFlashingCnt > 0)
	{
		m_nFlashingCnt--;
		if (!m_bFadeInOut)
		{
			m_Color.a += FLASHING_AMOUNT;
			if (m_Color.a > 1.0f)
			{
				m_Color.a = 1.0f;
				m_bFadeInOut = true;
			}
		}
		else if (m_bFadeInOut)
		{
			m_Color.a -= FLASHING_AMOUNT;
			if (m_Color.a < 0.2f)
			{
				m_Color.a = 0.2f;
				m_bFadeInOut = false;
			}
		}
	}


	SetPolygon();
}

//=============================================================================
// �|���S���̕`��
//=============================================================================
void C3DPolygon::Draw()
{
	LPDIRECT3DDEVICE9 pD3DDevice = nullptr;

	pD3DDevice = CApplication::GetRenderer()->GetDevice();


	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

										//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pD3DDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̏��
	CTexture *pTexture = CApplication::GetTexture();

	//�e�N�X�`���̐ݒ�
	pD3DDevice->SetTexture(0, pTexture->GetTexture(m_TextIndex));

	//�|���S���̕`��
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,						//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);		//�v���~�e�B�u�i�|���S���j��

	//�e�N�X�`���̏�����
	pD3DDevice->SetTexture(0, NULL);
}

void C3DPolygon::SetDiagonalLine(float fwidth, float fheight)
{
	m_fWidth = fwidth;
	m_fHeight = fheight;
}

void C3DPolygon::AddDiagonalLine(float faddwidth, float faddheight)
{
	m_fWidth += faddwidth;
	m_fHeight += faddheight;
}

void C3DPolygon::SetPolygon()
{
	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�//���[�J�����W��
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 0.0f, m_fHeight);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fHeight);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeight);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeight);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_UV.x - m_UVSize.x, m_UV.y - m_UVSize.y);
	pVtx[1].tex = D3DXVECTOR2(m_UV.x + m_UVSize.x, m_UV.y - m_UVSize.y);
	pVtx[2].tex = D3DXVECTOR2(m_UV.x - m_UVSize.x, m_UV.y + m_UVSize.y);
	pVtx[3].tex = D3DXVECTOR2(m_UV.x + m_UVSize.x, m_UV.y + m_UVSize.y);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

void C3DPolygon::SetUp(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move)
{
	m_Pos = pos;
	m_Rot = rot;
	m_Move = move;
}

//==============================================================================================
// ��������
//==============================================================================================
C3DPolygon *C3DPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXCOLOR col)
{
	C3DPolygon *p3DPolygon = nullptr;

	p3DPolygon = new C3DPolygon;

	if (p3DPolygon != nullptr)
	{

		if (FAILED(p3DPolygon->Init()))
		{
			assert(false);
		}

		p3DPolygon->SetPos(pos);
		p3DPolygon->SetRot(rot);
		p3DPolygon->SetDiagonalLine(size.x, size.y);
		p3DPolygon->SetColor(col);

		p3DPolygon->SetPolygon();
	}

	return p3DPolygon;
}