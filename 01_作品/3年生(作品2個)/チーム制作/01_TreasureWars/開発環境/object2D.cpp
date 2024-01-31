//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object2D.cpp
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"main.h"
#include"object2D.h"
#include"renderer.h"
#include"application.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObject2D::CObject2D(int nPriority) : CObject(nPriority)
{
	m_fAngle = 0.0f;		// �Ίp���̊p�x
	m_fRot = 0.0f;			// �|���S���̊p�x
	m_pVtxBuff = NULL;
	m_Col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texture = CTexture::TEXTURE_NONE;

	m_nAnimColumn = 0;

	// �A�j���[�V�����̋敪�̏�����(1 * 1)
	m_nAnimColumnDiv = 1;
	m_nAnimLineDiv = 1;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CObject2D::~CObject2D()
{
}

//==============================================================================================
// �|���S���̏�����
//==============================================================================================
HRESULT CObject2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@���擾
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	m_texture = CTexture::TEXTURE_NONE;

	// ���_�o�b�t�@
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos.x = m_Pos.x + sinf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fWidth;
	pVtx[0].pos.y = m_Pos.y + cosf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_Pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle)) * m_fWidth2;
	pVtx[1].pos.y = m_Pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle)) * m_fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_Pos.x + sinf(m_fRot + (m_fAngle * -1.0f)) * m_fWidth;
	pVtx[2].pos.y = m_Pos.y + cosf(m_fRot + (m_fAngle * -1.0f)) * m_fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_Pos.x + sinf(m_fRot + (m_fAngle * 1.0f)) * m_fWidth2;
	pVtx[3].pos.y = m_Pos.y + cosf(m_fRot + (m_fAngle * 1.0f)) * m_fHeight;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

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
void CObject2D::Uninit()
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
void CObject2D::Update()
{
	UV();
}

//==============================================================================================
// �|���S���̕`��
//==============================================================================================
void CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@�̐ݒ�
	//Stencil();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	 //���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CApplication::GetTexture()->GetTexture(m_texture));

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,		//���_���̐擪�A�h���X
		2);		//�v���~�e�B�u�̐�

	// �X�e���V���o�b�t�@=>����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, nullptr);
}

//==============================================================================================
// �X�e���V���o�b�t�@�̐ݒ�
//==============================================================================================
void CObject2D::Stencil()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@=>�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�ݒ�=>ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	// �X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ�=>0xff(���ׂĐ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@�ݒ�=>
	// �Q�ƒl >= �X�e���V���o�b�t�@�̎Q�ƒl�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);	// GREATEREQUAL:>=(�ȏ�)	EQUAL:==(������)

	// �X�e���V���e�L�X�g�̌��ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// PASS:Z�e�X�g���X�e���V���e�X�g����		REPLACE:�u������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Z�e�X�g���X�e���V���e�X�g���s		KEEP:�ύX�Ȃ�
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ZFAIL:Z�e�X�g�̂ݎ��s					INCR:+1
}

//==============================================================================================
// UV�̏���
//==============================================================================================
void CObject2D::UV()
{
	// �Ίp���̊p�x���Z�o����
	m_fAngle = (float)atan2(1.0f, 1.0f);

	// ���_�o�b�t�@
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�|���S���̍X�V
	pVtx[0].pos.x = m_Pos.x + sinf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fWidth;
	pVtx[0].pos.y = m_Pos.y + cosf(m_fRot + (-D3DX_PI + m_fAngle)) * m_fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_Pos.x + sinf(m_fRot + (D3DX_PI - m_fAngle)) * m_fWidth2;
	pVtx[1].pos.y = m_Pos.y + cosf(m_fRot + (D3DX_PI - m_fAngle)) * m_fHeight2;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_Pos.x + sinf(m_fRot + (m_fAngle * -1.0f)) * m_fWidth;
	pVtx[2].pos.y = m_Pos.y + cosf(m_fRot + (m_fAngle * -1.0f)) * m_fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_Pos.x + sinf(m_fRot + (m_fAngle * 1.0f)) * m_fWidth2;
	pVtx[3].pos.y = m_Pos.y + cosf(m_fRot + (m_fAngle * 1.0f)) * m_fHeight2;
	pVtx[3].pos.z = 0.0f;

	// �F�̍X�V
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//==============================================================================================
// �ʒu�̐ݒ�
//==============================================================================================
void CObject2D::Setposition(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//==============================================================================================
// �ړ��ʂ̐ݒ�
//==============================================================================================
void CObject2D::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
	m_Pos -= move;
}

//==============================================================================================
// �T�C�Y�̐ݒ�
//==============================================================================================
void CObject2D::SetSize(float width, float height)
{
	m_fWidth = width;
	m_fWidth2 = width;
	m_fHeight = height;
	m_fHeight2 = height;
}
//==============================================================================================
// �Q�[�W�̕�
//==============================================================================================
void CObject2D::SetGaugeSize(float width, float width2)
{
	m_fWidth = width;
	m_fWidth2 = width2;
}

//==============================================================================================
// �p�x�̐ݒ�
//==============================================================================================
void CObject2D::Setrot(float rot)
{
	m_fRot = rot;
}

//==============================================================================================
// �e�N�X�`�����W�̐ݒ�
//==============================================================================================
void CObject2D::SetTexPos(float top, float row, float right, float left)
{
	// ���_�o�b�t�@
	VERTEX_2D *pVtx;

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
// �A�j���[�V�����̐ݒ�
//==============================================================================================
void CObject2D::AnimTexture(int nPattern, int nPatternMax)
{
	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	float nDivisionRate = 1.0f / nPatternMax;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nPattern * nDivisionRate, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((nPattern + 1) * nDivisionRate, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattern * nDivisionRate, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((nPattern + 1) * nDivisionRate, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=========================================================
// �蓮�p�̒��_�e�N�X�`���ݒ菈���@(�摜��, �摜�s)
//=========================================================
void CObject2D::SetAnimControl(const int nAnimColumn, const int nAnimLine)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((1.0f / m_nAnimColumnDiv) * nAnimColumn, (1.0f / m_nAnimLineDiv) * nAnimLine);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_nAnimColumnDiv) * (nAnimColumn + 1), (1.0f / m_nAnimLineDiv) * nAnimLine);
	pVtx[2].tex = D3DXVECTOR2((1.0f / m_nAnimColumnDiv) * nAnimColumn, (1.0f / m_nAnimLineDiv) * (nAnimLine + 1));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_nAnimColumnDiv) * (nAnimColumn + 1), (1.0f / m_nAnimLineDiv) * (nAnimLine + 1));

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=========================================================
// �蓮�p�̒��_�e�N�X�`���ݒ菈���@(�摜�ԍ�)
//=========================================================
void CObject2D::SetAnimControl(const int nAnimCounter)
{
	int nAnimLine = 0;	// �摜�A�j���[�V�����̍s��
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �w�肳�ꂽ�A�j���ԍ���ϐ��Ɋi�[����
	int nCounter = nAnimCounter;

	// ���̍ő吔���w�肳�ꂽ�l���傫���ꍇ���[�v����
	while (m_nAnimColumnDiv <= nCounter)
	{
		// �l���敪���������炷
		nCounter -= m_nAnimColumnDiv;

		// �P�s�c�ɃA�j���[�V��������
		nAnimLine++;

		// �c�̍ő吔���w�肳�ꂽ�l���傫���ꍇ
		if (nAnimLine >= m_nAnimLineDiv)
		{
			// �c�A�j���[�V�����̏�����
			nAnimLine = 0;
		}
	}

	// ��A�j���[�V�����̑��
	m_nAnimColumn = nCounter;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((1.0f / m_nAnimColumnDiv) * m_nAnimColumn, (1.0f / m_nAnimLineDiv) * nAnimLine);
	pVtx[1].tex = D3DXVECTOR2((1.0f / m_nAnimColumnDiv) * (m_nAnimColumn + 1), (1.0f / m_nAnimLineDiv) * nAnimLine);
	pVtx[2].tex = D3DXVECTOR2((1.0f / m_nAnimColumnDiv) * m_nAnimColumn, (1.0f / m_nAnimLineDiv) * (nAnimLine + 1));
	pVtx[3].tex = D3DXVECTOR2((1.0f / m_nAnimColumnDiv) * (m_nAnimColumn + 1), (1.0f / m_nAnimLineDiv) * (nAnimLine + 1));

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//==============================================================================================
// �e�N�X�`���̐ݒ�
//==============================================================================================
void CObject2D::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//==============================================================================================
// ������ݒ�
//==============================================================================================
bool CObject2D::SetDisappear(float size, float alpha, float sizespeed, float alphaspeed)
{
	float fSize, fSize1, fSize2;
	fSize = m_fWidth;
	fSize1 = m_fWidth2;
	fSize2 = m_fHeight;

	if (fSize + size >= m_fWidth)
	{
		m_fWidth += sizespeed;		// �T�C�Y�̉��Z
		m_fWidth2 += sizespeed;
		m_fHeight += sizespeed;
	}
	if (m_Col.a >= alpha)
	{
		m_Col.a -= alphaspeed;		// ���l�̌��Z
	}
	if (m_Col.a <= 0)
	{
		return true;
	}

	return false;
}

//==============================================================================================
// ��������
//==============================================================================================
CObject2D *CObject2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority)
{
	CObject2D *pObject2D = nullptr;

	pObject2D = new CObject2D(nPriority);

	if (pObject2D != nullptr)
	{
		pObject2D->SetSize(size.x, size.y);
		pObject2D->Setposition(pos);
		pObject2D->Init();
	}

	return pObject2D;
}