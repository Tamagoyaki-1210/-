//==============================================================================================
//
// �O�Ձ@�@�@locus.cpp
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"locus.h"
#include"renderer.h"
#include"application.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CLocus::CLocus(const CObject::PRIORITY priority) : CObject(priority)
{
	m_pVtxBuff = nullptr;					// ���_�o�b�t�@
	m_texture = CTexture::TEXTURE_NONE;		// �e�N�X�`��
	m_bBillboard = false;					// �r���{�[�h���ǂ���
	m_nAnchorPoint = 0;
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CLocus::~CLocus()
{
}

//==============================================================================================
// �|���S���̏�����
//==============================================================================================
HRESULT CLocus::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@���擾
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nAnchorPoint+1) * 2),
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

	D3DXVECTOR3 ParentPos = m_pos;
	D3DXVECTOR3 PrevPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pAnchorPoints[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_���W(���[�J�����W)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	for (int i = 1; i <= m_nAnchorPoint; i++)
	{
		m_pAnchorPoints[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_pAnchorPoints[i].x += (500 - rand() % 10000) / 10.0f;
		m_pAnchorPoints[i].y += (500 + rand() % 15000) / 10.0f;
		m_pAnchorPoints[i].z += (500 - rand() % 10000) / 10.0f;
		
		D3DXVECTOR3 Pos = PrevPos + m_pAnchorPoints[i];
		PrevPos = Pos;

		//pos
		pVtx[2 * i].pos = D3DXVECTOR3(Pos.x - 50.0f, Pos.y, Pos.z);
		pVtx[2 * i + 1].pos = D3DXVECTOR3(Pos.x + 50.0f, Pos.y, Pos.z);

		//�@��
		pVtx[2 * i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2 * i + 1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[
		pVtx[2 * i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2 * i + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`���̍��W
		pVtx[2 * i].tex = D3DXVECTOR2(0.0f, 1.0f / m_nAnchorPoint * i);
		pVtx[2 * i + 1].tex = D3DXVECTOR2(1.0f, 1.0f / m_nAnchorPoint * i);
	}
	
	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================================
// �|���S���̏I��
//==============================================================================================
void CLocus::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pAnchorPoints != nullptr)
	{
		delete m_pAnchorPoints;
		m_pAnchorPoints = nullptr;
	}

	Release();
}

//==============================================================================================
// �|���S���̍X�V
//==============================================================================================
void CLocus::Update()
{
	//UV();
}

//==============================================================================================
// �|���S���̕`��
//==============================================================================================
void CLocus::Draw()
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.x, -m_rot.y, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
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
void CLocus::UV()
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = atan2f(m_fsize, m_fsize);								// �p�x(�A�[�N�^���W�F���g)
	float fLength = sqrtf((powf(m_fsize, 2) + powf(m_fsize, 2))) * 0.5f;	// ����(�X�N�G�A���[�g)

																			// ���_����ݒ�
	pVtx[0].pos.x = -sinf(fAngle) * fLength;
	pVtx[0].pos.y = cosf(fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(fAngle) * fLength;
	pVtx[1].pos.y = cosf(fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -sinf(fAngle) * fLength;
	pVtx[2].pos.y = -cosf(fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(fAngle) * fLength;
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
void CLocus::SetTexPos(const float top, const  float row, const float right, const float left)
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
// �A���J�[�|�C���g�̐����Ɛݒ�
//==============================================================================================
void CLocus::SetAnchorPoints(int nanchorpoints)
{
	m_nAnchorPoint = nanchorpoints;
	m_pAnchorPoints = new D3DXVECTOR3[m_nAnchorPoint * 2];
}

//==============================================================================================
// ��������
//==============================================================================================
CLocus* CLocus::Create(const D3DXVECTOR3 pos, const float fsize, int nAnchorPoints, const CObject::PRIORITY priority, const D3DXCOLOR col, const bool billboard)
{
	CLocus* pLocus = new CLocus(priority);

	pLocus->SetAnchorPoints(nAnchorPoints);
	if (FAILED(pLocus->Init()))
	{
		return nullptr;
	}
	pLocus->SetPos(pos);
	pLocus->SetSize(fsize);
	pLocus->SetCol(col);
	pLocus->Setbillboard(billboard);

	return pLocus;
}