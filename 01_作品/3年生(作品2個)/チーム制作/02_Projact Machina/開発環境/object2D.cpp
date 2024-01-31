//==============================================================================================
//
// �I�u�W�F�N�g�@�@�@object2D.cpp
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"object2D.h"
#include"renderer.h"
#include"application.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObject2D::CObject2D(const PRIORITY priority) : CObject(priority)
{
	m_rot = { 0.0f, 0.0f };			// �|���S���̊p�x
	m_pVtxBuff = nullptr;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_texture = CTexture::TEXTURE_NONE;
	m_Subsize = { 0.0f,0.0f };

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

	// ���_���W(���[�J�����W)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f; 
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

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
	// ���_�o�b�t�@
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = atan2f(m_size.x, m_size.y);							// �p�x(�A�[�N�^���W�F���g)
	float fLength = sqrtf((m_size.x * m_size.x) + (m_size.y * m_size.y)) * 0.5f;

	// ���_����ݒ�
	pVtx[0].pos.x = m_pos.x - sinf(fAngle + m_rot.x) * fLength;
	pVtx[0].pos.y = m_pos.y - cosf(fAngle + m_rot.y) * fLength - m_Subsize.y;
	pVtx[0].pos.z = m_pos.z;

	pVtx[1].pos.x = m_pos.x + sinf(fAngle - m_rot.x) * fLength - m_Subsize.x;
	pVtx[1].pos.y = m_pos.y - cosf(fAngle - m_rot.y) * fLength - m_Subsize.y;
	pVtx[1].pos.z = m_pos.z;

	pVtx[2].pos.x = m_pos.x - sinf(fAngle - m_rot.x) * fLength;
	pVtx[2].pos.y = m_pos.y + cosf(fAngle - m_rot.y) * fLength;
	pVtx[2].pos.z = m_pos.z;

	pVtx[3].pos.x = m_pos.x + sinf(fAngle + m_rot.x) * fLength - m_Subsize.x;
	pVtx[3].pos.y = m_pos.y + cosf(fAngle + m_rot.y) * fLength;
	pVtx[3].pos.z = m_pos.z;

	// �F�̍X�V
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//==============================================================================================
// �ړ��ʂ̐ݒ�
//==============================================================================================
void CObject2D::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
	m_pos -= move;
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
// ��������
//==============================================================================================
CObject2D* CObject2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const PRIORITY priority)
{
	CObject2D* pObject2D = nullptr;

	pObject2D = new CObject2D(priority);

	if (FAILED(pObject2D->Init()))
	{
		return nullptr;
	}
	pObject2D->SetSize(size);
	pObject2D->SetPos(pos);

	return pObject2D;
}