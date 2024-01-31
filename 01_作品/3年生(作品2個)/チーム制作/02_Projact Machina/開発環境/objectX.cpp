//==============================================================================================
//
// �I�u�W�F�N�gX�@�@�@objectX.cpp
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#include "objectX.h"
#include "application.h"
#include "model.h"
#include "light.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObjectX::CObjectX(const PRIORITY priority) : CObject(priority)
{
	// �f�t�H���g
	m_bShadow = false;
	m_bParts = false;
	m_bWireFrame = false;

	m_size = { 1.0f, 1.0f, 1.0f };
	m_col = { 0.0f, 0.0f, 0.0f, 0.0f };
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CObjectX::~CObjectX()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CObjectX::Init()
{
	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CObjectX::Uninit()
{
	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CObjectX::Update()
{

}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CObjectX::Draw()
{
	if (GetDrawFlag())
	{
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
		D3DXMATRIX mtxRot, mtxTrans, mtxScaling;	//�v�Z�p�̃}�g���b�N�X
		D3DMATERIAL9 matDef;				//���݂̃}�e���A���̕ۑ��p
		D3DXMATERIAL *pMat;					//�}�e���A���f�[�^�ւ̃|�C���^

		if (m_bWireFrame)
		{
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		// �T�C�Y�𔽉f
		D3DXMatrixScaling(&mtxScaling, m_size.x, m_size.y, m_size.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// �e�̃}�g���b�N�X
		D3DXMATRIX mtxParent;

		if (m_pParent != nullptr)
		{// �p�[�c���e�̏ꍇ
			mtxParent = m_pParent->GetMatrix();

			// �e�̃}�g���b�N�X�Ƃ�����
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
		}
		else if (m_bParts == true)
		{// ���݂�Matrix(Player��Matrix)���擾
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent); 

			// �e�̃}�g���b�N�X�Ƃ�����
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
		}

		// ���g�̔ԍ��̃��f�������擾
		CModel::ModelPattern modelPattern = CApplication::GetModel()->GetModelPattern(m_nIndex);

		if (m_bShadow == true)
		{
			// ���ʓ��e
			DrawShadow(pDevice, modelPattern);
		}

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// ���݂̃}�e���A����ێ�
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)modelPattern.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
		{
			// �}�e���A�����̐ݒ�
			D3DMATERIAL9 matD3D = pMat[nCntMat].MatD3D;

			D3DXCOLOR matCol = m_Original_col[nCntMat] + m_col;

			// �}�e���A���̐F�ύX
			matD3D.Diffuse = matD3D.Specular = matD3D.Emissive = matCol;

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&matD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, modelPattern.pTexture[nCntMat]);

			// ���f���p�[�c�̕`��
			modelPattern.pMesh->DrawSubset(nCntMat);
		}

		// �ێ����Ă����}�e���A�������ɖ߂�
		pDevice->SetMaterial(&matDef);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		// �X�e���V���o�b�t�@=>����
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	}
}

//==============================================================================================
// ���ʓ��e
//==============================================================================================
void CObjectX::DrawShadow(const LPDIRECT3DDEVICE9 device, CModel::ModelPattern model)
{
	D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;			// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxShadow;		// �V���h�E�}�g���b�N�X
	D3DXPLANE planeField;		// ����
	D3DXVECTOR3 pos, normal;

	// �A���r�G���g�𖳌��ɂ���
	device->SetRenderState(D3DRS_AMBIENT, 0);

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	// ���C�g�̎擾
	CLight *pLight = CApplication::GetLight();

	// ���C�g�̕���
	D3DXVECTOR3 vecDir = pLight->GetLight().Direction;

	D3DXVECTOR4 vecLight = D3DXVECTOR4(-vecDir.x, -vecDir.y, -vecDir.z, 0.0f);

	// �@���̐ݒ�
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���ʂ̍쐬
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// ���C�g�ƕ��ʂ���e�s����쐬
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// �V���h�E�}�g���b�N�X�̐ݒ�
	device->SetTransform(D3DTS_WORLD, &mtxShadow);

	// ���݂̃}�e���A���ێ�
	device->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)model.pBuffMat->GetBufferPointer();

	// �e�N�X�`���̐ݒ�
	device->SetTexture(0, NULL);

	for (int nCnt = 0; nCnt < m_nNumMat; nCnt++)
	{
		// �}�e���A�����̐ݒ�
		D3DMATERIAL9 matD3D = pMat[nCnt].MatD3D;

		// �F�̐ݒ�
		matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �}�e���A���̐ݒ�
		device->SetMaterial(&matD3D);

		// ���f���p�[�c�̕`��
		model.pMesh->DrawSubset(nCnt);
	}

	// �ۑ����Ă����}�e���A����߂�
	device->SetMaterial(&matDef);

	// �A���r�G���g��߂�
	device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
}

//==============================================================================================
// �X�e���V���o�b�t�@�̐ݒ�
//==============================================================================================
void CObjectX::Stencil()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@=>�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�ݒ�=>ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// �X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ�=>0xff(���ׂĐ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@�ݒ�=>
	// �Q�ƒl >= �X�e���V���o�b�t�@�̎Q�ƒl�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);	// GREATEREQUAL:>=(�ȏ�)	EQUAL:==(������)   REF >= ���̒l

																		// �X�e���V���e�L�X�g�̌��ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// PASS:Z�e�X�g���X�e���V���e�X�g����		REPLACE:�u������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Z�e�X�g���X�e���V���e�X�g���s		KEEP:�ύX�Ȃ�
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);		// ZFAIL:Z�e�X�g�̂ݎ��s					INCR:+1
}

//==============================================================================================
// ���f���ԍ��̓ǂݍ���
//==============================================================================================
void CObjectX::SetModel(const int index)
{
	// �ԍ��̐ݒ�
	m_nIndex = index;

	// ���g�̔ԍ��̃��f�������擾
	CModel::ModelPattern model = CApplication::GetModel()->GetModelPattern(m_nIndex);

	// ���f���̃}�e���A�����_����ݒ�
	m_nNumMat = model.nNumMat;

	// �g�p���̐F�̏�����
	if (!m_Original_col.empty())
	{
		m_Original_col.clear();
	}

	// �}�e���A���F������������
	for (int i = 0; i < m_nNumMat; i++)
	{
		m_Original_col.emplace_back();
		m_Original_col.back() = model.MatColor[i];
	}

	m_vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_vtxMaxModel = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	int nNumMeshVtx;	// ���f���̃��b�V�����_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

						//���_���̎擾
	nNumMeshVtx = model.pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(model.pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	model.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumMeshVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//�ŏ��l�̎擾
		if (vtx.x < m_vtxMinModel.x)
		{
			m_vtxMinModel.x = vtx.x;
		}
		else if (vtx.y < m_vtxMinModel.y)
		{
			m_vtxMinModel.y = vtx.y;
		}
		else if (vtx.z < m_vtxMinModel.z)
		{
			m_vtxMinModel.z = vtx.z;
		}

		//�ő�l�̎擾
		if (vtx.x > m_vtxMaxModel.x)
		{
			m_vtxMaxModel.x = vtx.x;
		}
		else if (vtx.y > m_vtxMaxModel.y)
		{
			m_vtxMaxModel.y = vtx.y;
		}
		else if (vtx.z > m_vtxMaxModel.z)
		{
			m_vtxMaxModel.z = vtx.z;
		}

		//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	//���_�o�b�t�@�̃A�����b�N
	model.pMesh->UnlockVertexBuffer();
}

//==============================================================================================
// ��������
//==============================================================================================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, const PRIORITY priority)
{
	CObjectX* pObjectX = new CObjectX(priority);

	if (FAILED(pObjectX->Init()))
	{
		return nullptr;
	}
	pObjectX->SetPos(pos);
	pObjectX->SetRot(rot);
	pObjectX->SetParent(parent);
	pObjectX->SetModel(CApplication::GetModel()->ReadObject(Xfilename));

	return pObjectX;
}

//==============================================================================================
// ��������
//==============================================================================================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const int index, const PRIORITY priority)
{
	CObjectX* pObjectX = new CObjectX(priority);

	if (FAILED(pObjectX->Init()))
	{
		return nullptr;
	}
	pObjectX->SetPos(pos);
	pObjectX->SetRot(rot);
	pObjectX->SetParent(parent);
	pObjectX->SetModel(index);

	return pObjectX;
}