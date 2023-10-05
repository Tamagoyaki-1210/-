//==============================================================================================
//
// �I�u�W�F�N�gX�@�@�@objectX.cpp
// tutida ryousei
//
//==============================================================================================
#include"ObjectX.h"
#include"application.h"
#include"game.h"
#include"input.h"
#include"fade.h"
#include"light.h"
#include"camera.h"
#include"charmanager.h"
#include"character.h"
#include"score.h"
#include"shop.h"
#include"charmanager.h"

std::vector <CObjectX::ModelPattern> CObjectX::m_ModelPattern = {};

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_Size = { 1.0f, 1.0f, 1.0f };
	m_vtxMinModel = { 0.0f,0.0f,0.0f };
	m_vtxMaxModel = { 0.0f,0.0f,0.0f };
	m_vecDir = { 0.0f,0.0f,0.0f };
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
	// �}�e���A���F������������
	for (int i = 0; i < (int)m_ModelPattern[m_nIndex].nNumMat; i++)
	{
		m_col.emplace_back();
		m_col.back() = m_ModelPattern[m_nIndex].MatColor[i];
	}

	D3DXMatrixIdentity(&m_mtxRot);
	
	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//���_���̎擾
	nNumVtx = m_ModelPattern[m_nIndex].pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_ModelPattern[m_nIndex].pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	m_ModelPattern[m_nIndex].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

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
	m_ModelPattern[m_nIndex].pMesh->UnlockVertexBuffer();

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
// ��ʑJ�ڎ��ɃI�u�W�F�N�g��j��
//==============================================================================================
void CObjectX::UninitObject()
{
	while(!m_ModelPattern.empty())
	{
		//���b�V���̔j��
		if (m_ModelPattern.back().pMesh != nullptr)
		{
			m_ModelPattern.back().pMesh->Release();
			m_ModelPattern.back().pMesh = nullptr;
		}

		//�}�e���A���̔j��
		if (m_ModelPattern.back().pBuffMat != nullptr)
		{
			m_ModelPattern.back().pBuffMat->Release();
			m_ModelPattern.back().pBuffMat = nullptr;
		}

		for (int nCntMat = 0; nCntMat < (int)m_ModelPattern.back().nNumMat; nCntMat++)
		{
			//�e�N�X�`���̔j��
			if (m_ModelPattern.back().pTexture[nCntMat] != nullptr)
			{
				m_ModelPattern.back().pTexture[nCntMat]->Release();
				m_ModelPattern.back().pTexture[nCntMat] = nullptr;
			}
		}
		// ��Ԍ��̗v�f�����X�g����r������
		m_ModelPattern.pop_back();
	}
}

//==============================================================================================
//�X�V����
//==============================================================================================
void CObjectX::Update()
{

}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;		//�v�Z�p�̃}�g���b�N�X
	D3DMATERIAL9 matDef;		//���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat = nullptr;			//�}�e���A���f�[�^�ւ̃|�C���^

	//�e�N�X�`���̐ݒ��߂�
	pDevice->SetTexture(0, NULL);

	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);

	// �T�C�Y�𔽉f
	D3DXMatrixScaling(&mtxScaling, m_Size.x, m_Size.y, m_Size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���ʓ��e
	//DrawShadow();

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_ModelPattern[m_nIndex].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++)
	{
		// �}�e���A���̐F�ύX
		pMat[nCntMat].MatD3D.Diffuse = pMat[nCntMat].MatD3D.Specular = pMat[nCntMat].MatD3D.Emissive = m_col[nCntMat];

		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_ModelPattern[m_nIndex].pTexture[nCntMat]);

		//���f���p�[�c�̕`��
		m_ModelPattern[m_nIndex].pMesh->DrawSubset(nCntMat);
	}

	//�ێ����Ă����}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//==============================================================================================
// ���ʓ��e
//==============================================================================================
void CObjectX::DrawShadow()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat = nullptr;			// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxShadow;		// �V���h�E�}�g���b�N�X
	D3DXPLANE planeField;		// ����
	D3DXVECTOR3 pos, normal;

	// �A���r�G���g�𖳌��ɂ���
	CApplication::GetRenderer()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0);

	// �V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	// ���C�g�̎擾
	CLight *pLight = CApplication::GetLight();

	// ���C�g�̕���
	D3DXVECTOR3 vecDir = pLight->GetLight().Direction;

	D3DXVECTOR4 vecLight = D3DXVECTOR4(-vecDir.x, -vecDir.y, -vecDir.z, 0.0f);

	// �ʒu�̐ݒ�
	pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// �@���̐ݒ�
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// ���ʂ̍쐬
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// ���C�g�ƕ��ʂ���e�s����쐬
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// �V���h�E�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// ���݂̃}�e���A���ێ�
	pDevice->GetMaterial(&matDef);

	if (m_ModelPattern[m_nIndex].pBuffMat != nullptr)
	{
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_ModelPattern[m_nIndex].pBuffMat->GetBufferPointer();
	}

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	for (int nCnt = 0; nCnt < (int)m_ModelPattern[m_nIndex].nNumMat; nCnt++)
	{
		D3DMATERIAL9 matD3D;

		if (pMat != nullptr)
		{
			// �}�e���A�����̐ݒ�
			matD3D = pMat[nCnt].MatD3D;
		}

		// �F�̐ݒ�
		matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matD3D);

		if (m_ModelPattern[m_nIndex].pMesh != nullptr)
		{
			// ���f���p�[�c�̕`��
			m_ModelPattern[m_nIndex].pMesh->DrawSubset(nCnt);
		}
	}

	// �ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	// �A���r�G���g��߂�
	CApplication::GetRenderer()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
}

//==============================================================================================
// �t�@�C���̓ǂݍ���
//==============================================================================================
int CObjectX::ReadObject(const char* name)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	int nIndex = 0;

	// �쐬���ꂽ�S�Ẵ��f����ǂݍ���
	for (auto model : m_ModelPattern)
	{
		if (model.fileName == name)
		{
			// �g�p���̃��f���ƈ�v����ꍇ�I��
			return nIndex;
		}
		nIndex++;
	}

	// ���f�����̌��̗v�f��ǉ�
	m_ModelPattern.emplace_back();

	m_ModelPattern.back().fileName = name;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(m_ModelPattern.back().fileName.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_ModelPattern.back().pBuffMat,
		NULL,
		&m_ModelPattern.back().nNumMat,
		&m_ModelPattern.back().pMesh);

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_ModelPattern.back().pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int i = 0; i < (int)m_ModelPattern.back().nNumMat; i++)
	{
		// �e�N�X�`���̌��̗v�f��ǉ�
		m_ModelPattern.back().pTexture.emplace_back();

		if (pMat[i].pTextureFilename != nullptr)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&m_ModelPattern.back().pTexture.back());
		}
		else
		{
			m_ModelPattern.back().pTexture[i] = nullptr;
		}
		// �}�e���A���̌��̗v�f��ǉ�
		m_ModelPattern.back().MatColor.emplace_back();

		// �}�e���A���̐F����ۑ�
		m_ModelPattern.back().MatColor.back() = pMat[i].MatD3D.Diffuse;
	}

	return nIndex;
}

//==============================================================================================
// ���f���ԍ��̐ݒ�
//==============================================================================================
void CObjectX::SetIndex(const int index)
{
	m_nIndex = index;

	// ���f���̐F���ݒ蒆�̏ꍇ
	if (!m_col.empty())
	{
		// ���݂̃}�e���A���̐F������
		m_col.clear();

		// �}�e���A���F������������
		for (int i = 0; i < (int)m_ModelPattern[m_nIndex].nNumMat; i++)
		{
			m_col.emplace_back();
			m_col.back() = m_ModelPattern[m_nIndex].MatColor[i];
		}
	}
}

//==============================================================================================
// ��������
//==============================================================================================
CObjectX * CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* Xfilename)
{
	CObjectX *pObjectX = new CObjectX(2);

	if (pObjectX != nullptr)
	{
		pObjectX->SetPos(pos);
		pObjectX->SetRot(rot);
		pObjectX->SetSize(size);
		pObjectX->SetIndex(ReadObject(Xfilename));
		pObjectX->Init();
	}

	return pObjectX;
}