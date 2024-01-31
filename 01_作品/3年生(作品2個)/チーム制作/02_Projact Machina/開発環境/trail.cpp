//===================================================
//
// �O�Ղ̏���
// Author: Teruto Sato
// Author: Kota Tanaka
//
//===================================================

//-----------------------
// �C���N���[�h
//-----------------------
//
#include <assert.h>
#include "trail.h"
#include "application.h"
#include "renderer.h"
#include "object3D.h"

//==========================
// �R���X�g���N�^
//==========================
CTrail::CTrail() :
m_offsetPos(0.0f, 0.0f, 0.0f),		// �I�t�Z�b�g���W
m_worldPos(0.0f, 0.0f, 0.0f),		// ���[���h���W
m_beginCol(0.0f, 0.0f, 0.0f, 0.0f),	// �F�̊J�n�l
m_endCol(0.0f, 0.0f, 0.0f, 0.0f),	// �F�̏I���l
m_nNumVtx(0),						// ���_��
m_pVtxBuff(nullptr)					// ���_�o�b�t�@�ւ̃|�C���^
{
	D3DXMatrixIdentity(&m_mtxWorld);	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_pMtxParent);	//�e�̃}�g���b�N�X
}

//==========================
// �f�X�g���N�^
//==========================
CTrail::~CTrail()
{
}

//==========================
// ������
//==========================
HRESULT CTrail::Init(D3DXVECTOR3 pos)
{
	//�����l�̐ݒ�
	m_offsetPos = pos;	//�ʒu
	m_beginCol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);	//�F�̏����l
	m_endCol = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f);

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nMaxVtx,	//�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//----------------------------------
	// ���_���̐ݒ�
	//----------------------------------
	for (int i = 0; i < nMaxVtx; i++)
	{
		// ���_���W�̐ݒ�
		pVtx[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[i].col = m_beginCol;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==========================
// �I��
//==========================
void CTrail::Uninit()
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//==========================
// �X�V
//==========================
void CTrail::Update()
{
	VERTEX_3D*pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//--------------------------------------
	// ���_���W����납��R�s�[����
	//--------------------------------------
	for (int i = m_nNumVtx - 1; i > 0; i--)
	{//���_�����~���ɉ�
		if (i - 2 >= 0)
		{
			pVtx[i].pos = pVtx[i - 2].pos;
			pVtx[i].col = pVtx[i - 2].col;
		}
	}

	// �F�̍X�V
	float rate = 1.0f / m_nNumVtx;
	for (int i = 0; i < m_nNumVtx; i++)
	{
		float progress = i * rate;
		float attenuation = 3.0f;
		D3DXCOLOR col = pVtx[i].col;

		col.a -= 0.005f;

		pVtx[i].col = D3DXCOLOR(m_beginCol.r + progress * (m_endCol.r - m_beginCol.r) * attenuation,
			m_beginCol.g + progress * (m_endCol.g - m_beginCol.g) * attenuation,
			m_beginCol.b + progress * (m_endCol.b - m_beginCol.b) * attenuation,
			col.a);
	}

	//--------------------------------------
	// �󂢂�0��1�ɋO�Ղ̐V�������W����
	//--------------------------------------
	//�e���f���̌��_����I�t�Z�b�g���ړ��������W����
	D3DXVec3TransformCoord(&m_worldPos, &m_offsetPos, &m_pMtxParent);
	pVtx[1].pos = m_worldPos;	//���W�̐ݒ�
	pVtx[1].col = m_beginCol;	//�F�̐ݒ�

	//�e���f���̌��_�̍��W����
	D3DXVec3TransformCoord(&m_worldPos, &D3DXVECTOR3(0.0f, -20.0f, 0.0f), &m_pMtxParent);
	pVtx[0].pos = m_worldPos;	//���W�̐ݒ�

	//�F�̐ݒ�
	{
		D3DXCOLOR newCol = m_beginCol;
		newCol.a = 0.1f;
		pVtx[0].col = newCol;
	}

	if (m_nNumVtx < nMaxVtx - 2)
	{//���_�̐����ő�l�𒴂��Ȃ��Ȃ�
		m_nNumVtx += 2;	//���_�������Z
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==========================
// �`��
//==========================
void CTrail::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�̎擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �����_�����O�X�e�[�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z�e�X�g
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 170);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//���ʂ��猩����悤�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		m_nNumVtx - 4);								// �`�悷��v���~�e�B�u��

	//���ʂ��猩���Ȃ�����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Z�o�b�t�@�̐ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTrail* CTrail::Create(D3DXVECTOR3 offset, D3DXMATRIX mtx)
{
	CTrail* pOrbit = nullptr;

	//----------------------------------
	// �|���S���̐����Ə�����
	//----------------------------------
	pOrbit = new CTrail;	//����

	if (pOrbit != nullptr)
	{//NULL�`�F�b�N
	 //�����̑��
		pOrbit->m_pMtxParent = mtx;	//�e�̃}�g���b�N�X
		pOrbit->Init(offset);
	}

	return pOrbit;
}

void CTrail::SetColor(D3DXCOLOR col, D3DXCOLOR destcol)
{
	VERTEX_3D*pVtx;		//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_beginCol = col;
	m_endCol = destcol;

	//-------------------------------
	// �F�̕ύX
	//-------------------------------
	pVtx[1].col = col;

	D3DXCOLOR newCol = col;
	newCol.a = 0.1f;

	pVtx[0].col = newCol;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

void CTrail::SetTrail(D3DXMATRIX mtx, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �}�g���N�X�̐ݒ�
	m_pMtxParent = mtx;

	//-------------------------------
	// �F�̕ύX
	//-------------------------------
	pVtx[1].col = col;

	D3DXCOLOR newCol = col;
	newCol.a = 0.1f;

	pVtx[0].col = newCol;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}