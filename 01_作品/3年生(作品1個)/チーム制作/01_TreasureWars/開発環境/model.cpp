//==============================================================================================
//
// ���f���@�@�@madel.cpp
// tutida ryousei
//
//==============================================================================================
#include"model.h"
#include"application.h"
#include"light.h"
#include"game.h"
#include"player.h"

std::vector <CModel::Model> CModel::m_Model = {};
//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CModel::CModel()
{
	m_Size = {1.0f, 1.0f, 1.0f};
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CModel::~CModel()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CModel::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	m_bShadow = true;

	// �}�e���A���F������������
	for (int i = 0; i < (int)m_Model[m_nIndex].nNumMat; i++)
	{
		m_col.emplace_back();
		m_col.back() = m_Model[m_nIndex].MatColor[i];
	}

	m_vtxMinModel = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_vtxMaxModel = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);

	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = m_Model[m_nIndex].pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_Model[m_nIndex].pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_Model[m_nIndex].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
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
	m_Model[m_nIndex].pMesh->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CModel::Uninit()
{

}

//==============================================================================================
// �X�V����
//==============================================================================================
void CModel::Update()
{

}

//==============================================================================================
// �`�揈��
//==============================================================================================
D3DXMATRIX CModel::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;	//�v�Z�p�̃}�g���b�N�X
	D3DMATERIAL9 matDef;				//���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat;					//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);

	// �T�C�Y�𔽉f
	D3DXMatrixScaling(&mtxScaling, m_Size.x, m_Size.y, m_Size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�̃}�g���b�N�X
	D3DXMATRIX mtxParent;

	if (m_pParent != nullptr)
	{// �p�[�c���e�̏ꍇ
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{// ���݂�Matrix(Player��Matrix)���擾
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// �e�̃}�g���b�N�X�Ƃ�����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	if (m_bShadow == true)
	{
		// ���ʓ��e
		DrawShadow(pDevice);
	}

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_Model[m_nIndex].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++)
	{
		// �}�e���A�����̐ݒ�
		D3DMATERIAL9 matD3D = pMat[nCntMat].MatD3D;

		// �}�e���A���̐F�ύX
		matD3D.Diffuse = matD3D.Specular = matD3D.Emissive = m_col[nCntMat];

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_Model[m_nIndex].pTexture[nCntMat]);

		// ���f���p�[�c�̕`��
		m_Model[m_nIndex].pMesh->DrawSubset(nCntMat);
	}

	// �ێ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	// �X�e���V���o�b�t�@=>����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	return m_mtxWorld;
}

//==============================================================================================
// ���ʓ��e
//==============================================================================================
void CModel::DrawShadow(const LPDIRECT3DDEVICE9 device)
{
	if (CApplication::GetMode() != CApplication::MODE_RESULT)
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

		// �ʒu�̐ݒ�
		if (CApplication::GetMode() == CApplication::MODE_ENTRY)
			pos = D3DXVECTOR3(0.0f, 4.0f, 0.0f);
		else if (CApplication::GetMode() != CApplication::MODE_ENTRY)
			pos = D3DXVECTOR3(0.0f, 8.0f, 0.0f);

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
		pMat = (D3DXMATERIAL*)m_Model[m_nIndex].pBuffMat->GetBufferPointer();

		// �e�N�X�`���̐ݒ�
		device->SetTexture(0, NULL);

		for (int nCnt = 0; nCnt < (int)m_Model[m_nIndex].nNumMat; nCnt++)
		{
			// �}�e���A�����̐ݒ�
			D3DMATERIAL9 matD3D = pMat[nCnt].MatD3D;

			// �F�̐ݒ�
			matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			// �}�e���A���̐ݒ�
			device->SetMaterial(&matD3D);

			// ���f���p�[�c�̕`��
			m_Model[m_nIndex].pMesh->DrawSubset(nCnt);
		}

		// �ۑ����Ă����}�e���A����߂�
		device->SetMaterial(&matDef);

		// �A���r�G���g��߂�
		device->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
	}
}

//==============================================================================================
// �X�e���V���o�b�t�@�̐ݒ�
//==============================================================================================
void CModel::Stencil()
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
// �S�Ẵ��f���̏I������
//==============================================================================================
void CModel::ModelReleaseAll()
{
	// ���f����S�ďI��������
	while (!m_Model.empty())
	{
		// ���b�V���̔j��
		if (m_Model.back().pMesh != nullptr)
		{
			m_Model.back().pMesh->Release();
		}

		// �}�e���A���̔j��
		if (m_Model.back().pBuffMat != nullptr)
		{
			m_Model.back().pBuffMat->Release();
		}

		for (int nCntMat = 0; nCntMat < (int)m_Model.back().nNumMat; nCntMat++)
		{
			// �e�N�X�`���̔j��
			if (m_Model.back().pTexture[nCntMat] != nullptr)
			{
				m_Model.back().pTexture[nCntMat]->Release();
			}
		}	
		// ��Ԍ��̗v�f�����X�g����r������
		m_Model.pop_back();
	}
}

//==============================================================================================
// �t�@�C���̓ǂݍ���
//==============================================================================================
int CModel::ReadObject(const char* name)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	int nIndex = 0;

	// �쐬���ꂽ�S�Ẵ��f����ǂݍ���
	for (auto model : m_Model)
	{
		if (model.fileName == name)
		{
			// �g�p���̃��f���ƈ�v����ꍇ�I��
			return nIndex;
		}
		nIndex++;
	}

	// ���f�����̌��̗v�f��ǉ�
	m_Model.emplace_back();

	m_Model.back().fileName = name;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(m_Model.back().fileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_Model.back().pBuffMat,
		NULL,
		&m_Model.back().nNumMat,
		&m_Model.back().pMesh);

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_Model.back().pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int i = 0; i < (int)m_Model.back().nNumMat; i++)
	{
		// �e�N�X�`���̌��̗v�f��ǉ�
		m_Model.back().pTexture.emplace_back();

		if (pMat[i].pTextureFilename != nullptr)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&m_Model.back().pTexture.back());
		}
		else
		{
			m_Model.back().pTexture.back() = nullptr;
		}

		// �}�e���A���̌��̗v�f��ǉ�
		m_Model.back().MatColor.emplace_back();

		// �}�e���A���̐F����ۑ�
		m_Model.back().MatColor.back() = pMat[i].MatD3D.Diffuse;
	}

	return nIndex;
}

//==============================================================================================
// ��������
//==============================================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel *parent, char *Xfilename)
{
	CModel *pModel = nullptr;

	pModel = new CModel;

	if (pModel != nullptr)
	{
		pModel->SetPos(pos);
		pModel->SetRot(rot);
		pModel->SetParent(parent);
		pModel->m_nIndex = pModel->ReadObject(Xfilename);
		pModel->Init();
	}

	return pModel;
}