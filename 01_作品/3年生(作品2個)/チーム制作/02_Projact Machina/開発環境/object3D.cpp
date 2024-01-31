//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object3D.cpp
// Author : tutida ryousei
// Author : tanimoto kosuke
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"object3D.h"
#include"renderer.h"
#include"application.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObject3D::CObject3D(const CObject::PRIORITY priority) : CObject(priority)
{
	m_pVtxBuff = nullptr;					// ���_�o�b�t�@
	m_texture = CTexture::TEXTURE_NONE;		// �e�N�X�`��
	m_bBillboard = false;					// �r���{�[�h���ǂ���
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CObject3D::~CObject3D()
{
}

//==============================================================================================
// �|���S���̏�����
//==============================================================================================
HRESULT CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@���擾
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	m_texture = CTexture::TEXTURE_NONE;
	m_col = { 1.0f,1.0f,1.0f,1.0f };

	// ���_�o�b�t�@
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W(���[�J�����W)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �@��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`���̍��W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================================
// �|���S���̏I��
//==============================================================================================
void CObject3D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//==============================================================================================
// �|���S���̍X�V
//==============================================================================================
void CObject3D::Update()
{
	UV();
}

//==============================================================================================
// �|���S���̕`��
//==============================================================================================
void CObject3D::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	// Z�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// ���C�g�𖳌���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �J�����̋t���ݒ�(�r���{�[�h)
	if (m_bBillboard)
	{
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._21 = mtxView._12;
		m_mtxWorld._22 = mtxView._22;
		m_mtxWorld._23 = mtxView._32;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;
	}

	//�p�x�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.x, m_rot.y, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̏��
	CTexture *pTexture = CApplication::GetTexture();

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	//�r���{�[�h�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���ɖ߂�
	pDevice->SetTexture(0, NULL);

	//���C�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Z�e�X�g�̐ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

//=====================================
// UV�ݒ菈��
//=====================================
void CObject3D::UV()
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = atan2f(m_size.x, m_size.y);								// �p�x(�A�[�N�^���W�F���g)
	float fLength = sqrtf((powf(m_size.x, 2) + powf(m_size.y, 2))) * 0.5f;	// ����(�X�N�G�A���[�g)

																			// ���_����ݒ�
	pVtx[0].pos.x = -sinf(fAngle) * fLength;
	pVtx[0].pos.y = cosf(fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(fAngle) * fLength - m_Subsize.x;
	pVtx[1].pos.y = cosf(fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -sinf(fAngle) * fLength;
	pVtx[2].pos.y = -cosf(fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(fAngle) * fLength - m_Subsize.x;
	pVtx[3].pos.y = -cosf(fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;

	// ���_�J���[
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================================================
// �e�N�X�`���̕���
//==============================================================================================
void CObject3D::SetTexPos(const float top, const  float row, const float right, const float left)
{
	// ���_�o�b�t�@
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, row);
	pVtx[3].tex = D3DXVECTOR2(right, row);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================================================
// ��������
//==============================================================================================
CObject3D* CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const CObject::PRIORITY priority, const D3DXCOLOR col, const bool billboard)
{
	CObject3D* pObject3D = new CObject3D(priority);

	if (FAILED(pObject3D->Init()))
	{
		return nullptr;
	}
	pObject3D->SetPos(pos);
	pObject3D->SetSize(size);
	pObject3D->SetCol(col);
	pObject3D->Setbillboard(billboard);

	return pObject3D;
}