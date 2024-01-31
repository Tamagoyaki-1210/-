//==============================================================================================
//
// ���b�V���t�B�[���h		meshfield.cpp
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"meshfield.h"
#include"application.h"
#include"game.h"
#include<time.h>
#include"player_manager.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CMeshField::CMeshField(const CObject::PRIORITY priority) : CObject3D(priority)
{
	m_nVertexNum = 0;		// ���_��
	m_nIndexNum = 0;		// �C���f�b�N�X�o�b�t�@
	m_nPrimitiveNum = 0;	// �v���~�e�B�u��
	m_nXBlock = 0;			// X�����̃u���b�N�̐�
	m_nZBlock = 0;			// Z�����̃u���b�N�̐�
	m_nHeight = 0;			// ���_�̍���
	m_nVecCount = 0;		// �O�ς��v���X�ɂȂ�����
	m_nPolyCount = 0;
	m_fSize = 0.0f;			// �T�C�Y
	m_pVtxBuff = nullptr;	//���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = nullptr;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CMeshField::~CMeshField()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CMeshField::Init()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	m_nVertexNum = ((m_nXBlock + 1) * (m_nZBlock + 1));										// ���_��	
	m_nIndexNum = (m_nXBlock * 2 + 2) * m_nZBlock + 2 * (m_nZBlock - 1);					// �C���f�b�N�X�o�b�t�@
	m_nPrimitiveNum = (m_nXBlock * m_nZBlock * 2 + 4 * (m_nZBlock - 1));					// �v���~�e�B�u��
	m_nHeight = 1000;		// ���_�̍���(�����_���̍ő�l)

	// �e�N�X�`���̐ݒ�
	m_Texture = CTexture::TEXTURE_FLOOR;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nVertexNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nIndexNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	VERTEX_3D *pVtx = NULL;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntZ = 0; nCntZ <= m_nZBlock; nCntZ++)
	{
		for (int nCntX = 0; nCntX <= m_nXBlock; nCntX++)
		{
			int index = nCntX + (m_nXBlock + 1) * nCntZ;

			pVtx[index].pos = D3DXVECTOR3(-(m_fSize * m_nXBlock) / 2 + nCntX * m_fSize, 0.0f + GetPos().y, (m_fSize * m_nZBlock) / 2 - nCntZ * m_fSize);
			pVtx[index].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[index].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[index].tex = D3DXVECTOR2((float)nCntX, (float)nCntZ);
			pVtx[index].broken = false;
		}
	}

	srand((unsigned int)time(NULL));

	// ���_�����J��Ԃ�
	//for (int nCntZ = 0; nCntZ <= m_nZBlock; nCntZ++)
	//{
	//	for (int nCntX = 0; nCntX <= m_nXBlock; nCntX++)
	//	{
	//		// ���ʂ�����
	//		int nHeight = rand() % m_nHeight;
	//		pVtx[nCntX + (m_nXBlock + 1) * nCntZ].pos.y = (float)nHeight;
	//	}
	//}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;		// �|���S������鏇��
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�̐ݒ�
	for (int nCntZ = 0; nCntZ < m_nZBlock; nCntZ++)
	{
		for (int nCntX = 0; nCntX < (m_nXBlock + 1); nCntX++)
		{
			pIdx[(nCntX * 2 + 1) + nCntZ * ((m_nXBlock + 1) * 2 + 2)] = (WORD)(nCntX + (m_nXBlock + 1) * nCntZ);

			pIdx[nCntX * 2 + nCntZ * ((m_nXBlock + 1) * 2 + 2)] = (WORD)(pIdx[(nCntX * 2 + 1) + nCntZ * ((m_nXBlock + 1) * 2 + 2)] + (m_nXBlock + 1));
		}

		if (nCntZ < m_nZBlock)
		{
			pIdx[((m_nXBlock + 1) * 2) + nCntZ * ((m_nXBlock + 1) * 2 + 2)] = (WORD)(m_nXBlock + ((m_nXBlock + 1))* nCntZ);
			pIdx[((m_nXBlock + 1) * 2) + nCntZ * ((m_nXBlock + 1) * 2 + 2) + 1] = (WORD)(2 * ((m_nXBlock + 1)) + ((m_nXBlock + 1))* nCntZ);
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CMeshField::Uninit(void)
{
	//���_�o�b�t�@�̉��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// �I�u�W�F�N�g�̉��
	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CMeshField::Update(void)
{
	// �@���̐ݒ�
	Normal();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CMeshField::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�ɐݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̏��
	CTexture *pTexture = CApplication::GetTexture();

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetTexture(m_Texture));

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nVertexNum, 0, m_nPrimitiveNum);

	pDevice->SetTexture(0, NULL);
}

//==============================================================================================
// �X�e���V���o�b�t�@�̐ݒ�
//==============================================================================================
void CMeshField::Stencil()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@=>�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �X�e���V���o�b�t�@�Ɣ�r����Q�ƒl�ݒ�=>ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// �X�e���V���o�b�t�@�̒l�ɑ΂��Ẵ}�X�N�ݒ�=>0xff(���ׂĐ^)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@�ݒ�=>
	// �Q�ƒl >= �X�e���V���o�b�t�@�̎Q�ƒl�Ȃ獇�i
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);	// GREATEREQUAL:>=(�ȏ�)	EQUAL:==(������)

	// �X�e���V���e�L�X�g�̌��ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);		// PASS:Z�e�X�g���X�e���V���e�X�g����		REPLACE:�u������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// FAIL:Z�e�X�g���X�e���V���e�X�g���s		KEEP:�ύX�Ȃ�
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// ZFAIL:Z�e�X�g�̂ݎ��s					INCR:+1

	// �X�e���V���o�b�t�@=>����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==============================================================================================
// ���b�V���̓����蔻��
//==============================================================================================
float CMeshField::MeshCollision(D3DXVECTOR3 pos)
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	D3DXVECTOR3 Vec[3];
	D3DXVECTOR3 PVec = { 0.0f,0.0f,0.0f };

	memset(&Vec, 0, sizeof(Vec));

	m_fPosY = 0;
	m_bPorigon_Scope = false;
	m_bHit = false;

	for (int nCnt = 0; nCnt < m_nIndexNum; nCnt++)
	{
		// �k�ރ|���S���͔��肵�Ȃ�
		if (pVtx[pIdx[nCnt]].pos == pVtx[pIdx[nCnt + 1]].pos) {
			continue;
		}

		// ���_���璸�_�̃x�N�g��
		if (m_nPolyCount % 2 == 0)
		{
			Vec[0] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt]].pos;
			Vec[1] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt + 1]].pos;
			Vec[2] = pVtx[pIdx[nCnt]].pos - pVtx[pIdx[nCnt + 2]].pos;
		}
		else
		{
			Vec[0] = pVtx[pIdx[nCnt]].pos - pVtx[pIdx[nCnt + 1]].pos;
			Vec[1] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt + 2]].pos;
			Vec[2] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt]].pos;
		}

		m_nPolyCount++;

		// �@���ʃx�N�g��
		m_NorVec = { 0.0f,0.0f,0.f };
		D3DXVECTOR3 Vector[2];

		// �v�Z�p�x�N�g��
		Vector[0] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt]].pos;
		Vector[1] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt]].pos;

		D3DXVec3Cross(&m_NorVec, &Vector[0], &Vector[1]);

		// �@���ʃx�N�g���̐��K��
		D3DXVec3Normalize(&m_NorVec, &m_NorVec);

		float fCrossProduct = 0;	// �O�ς̊i�[��
		m_nVecCount = 0;

		for (int nVCnt = 0; nVCnt < 3; nVCnt++)
		{
			// �v���C���[��3���_�̃x�N�g��
			PVec = pos - pVtx[pIdx[nCnt + nVCnt]].pos;

			// �O�ς̌v�Z
			fCrossProduct = Vec[nVCnt].x * PVec.z - Vec[nVCnt].z * PVec.x;

			if (fCrossProduct <= 0)
				// �O�ς��}�C�i�X�̏ꍇ
				m_nVecCount++;
			else
				break;
		}

		// ���̃|���S���̒��ɂ���ꍇ
		if (m_nVecCount == 3)
		{
			// �v���C���[��Y���W�̎Z�o
			m_fPosY = pVtx[pIdx[nCnt]].pos.y - ((pos.x - pVtx[pIdx[nCnt]].pos.x) * m_NorVec.x + (pos.z - pVtx[pIdx[nCnt]].pos.z) * m_NorVec.z) / m_NorVec.y;
			m_bPorigon_Scope = true;
			m_nCntIndex = nCnt;

			// �ڒn���Ă���ꍇ
			if (m_fPosY >= pos.y)
			{
				m_bHit = true;

				// ������ł��؂�
				break;
			}
		}
	}
	m_nPolyCount = 0;

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuff->Unlock();

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return m_fPosY;
}

//==============================================================================================
// �n�ʂ����
//==============================================================================================
float CMeshField::Ground_Broken(D3DXVECTOR3 pos, float damage, int scope)
{
	D3DXVECTOR3 Pos = pos;

	float fPosY = MeshCollision(Pos);

	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	if (m_bPorigon_Scope)
	{
		int nIndex = m_nCntIndex;
		int nScope = scope;
		int nPlus = 0;
		bool bEnd = false;
		bool bEdge[8] = {};

		// ����̔ԍ�
		int up_down = ((m_nXBlock + 1) * 2 + 2);

		for (int nCnt = 0; nCnt < m_nXBlock && !bEnd; nCnt++, nPlus += up_down)
		{
			for (int nCnt2 = 0; nCnt2 <= m_nXBlock; nCnt2++)
			{
				// �k�ރ|���S���ȊO�̋����̔ԍ�
				int nEven_Number = 2 * nCnt2 + 1 + nPlus;

				if (nIndex == nEven_Number)
				{
					// ��̈ʒu�����炷
					nIndex--;
					m_nCntIndex--;
					bEnd = true;
					break;
				}
			}
		}

		for (int nCnt = 0; nCnt < nScope; nCnt++)
		{
			// ����͈͂̐ݒ�
			int Rand_Index = rand() % 8;

			for (int nCnt2 = 0; nCnt2 < 4; nCnt2++)
			{
				if (!bEdge[Rand_Index])
				{
					// �n�ʂ�P��
					if (!pVtx[pIdx[nIndex + nCnt2]].broken)
					{
						// ���������܂łւ��܂���͈�
						float fDamage = 0.0f;
						if (nCnt <= 0)
							fDamage = m_fPosY - damage;
						else
							fDamage = damage;

						// ��x�����Ă��Ȃ�
						pVtx[pIdx[nIndex + nCnt2]].pos.y = fDamage;
						pVtx[pIdx[nIndex + nCnt2]].broken = true;
					}
					else
					{
						// ��x���Ă���
						pVtx[pIdx[nIndex + nCnt2]].pos.y -= damage;
					}

					// �k�ރ|���S���̔���
					if (pVtx[pIdx[nIndex + nCnt2]].pos == pVtx[pIdx[nIndex + nCnt2 + 1]].pos)
						break;
				}
			}

			// ���_�̔ԍ��̃J�E���g
			int Porigon_Count = 0;

			switch (Rand_Index)
			{
			case 0:
				// ��
				if (nIndex != 0)
				{
					if (pVtx[pIdx[nIndex]].pos != pVtx[pIdx[nIndex - 1]].pos)
						nIndex -= 2;
					else
						bEdge[Rand_Index] = true;
				}
				break;
			case 1:
				// �E
				if (pVtx[pIdx[nIndex + 1]].pos != pVtx[pIdx[nIndex + 2]].pos)
					nIndex += 2;
				else
					bEdge[Rand_Index] = true;
				break;
			case 2:
				// ��
				for (int nCnt2 = 0; nCnt2 < m_nXBlock; nCnt2++)
				{
					// ��ԏ�̗񂩂ǂ���
					if (nIndex + 1 == nCnt2 * 2 + 1)
						Porigon_Count++;
				}

				if (Porigon_Count == 0)
					nIndex -= up_down;
				else
					bEdge[Rand_Index] = true;
				break;
			case 3:
				// ����
				for (int nCnt2 = 0; nCnt2 < m_nXBlock; nCnt2++)
				{
					// ��ԏ�̗񂩂ǂ���
					if (nIndex + 1 == nCnt2 * 2 + 1)
						Porigon_Count++;
				}

				if (Porigon_Count == 0)
				{
					nIndex -= up_down;

					if (nIndex != 0)
					{
						if (pVtx[pIdx[nIndex]].pos != pVtx[pIdx[nIndex - 1]].pos)
							nIndex -= 2;
						else
							bEdge[Rand_Index] = true;
					}
					else
						bEdge[Rand_Index] = true;
				}
				else
					bEdge[Rand_Index] = true;
				break;
			case 4:
				// �E��
				for (int nCnt2 = 0; nCnt2 < m_nXBlock; nCnt2++)
				{
					// ��ԏ�̗񂩂ǂ���
					if (nIndex + 1 == nCnt2 * 2 + 1)
						Porigon_Count++;
				}

				if (Porigon_Count == 0)
				{
					nIndex -= up_down;

					if (nIndex != 0)
					{
						if (pVtx[pIdx[nIndex + 1]].pos != pVtx[pIdx[nIndex + 2]].pos)
							nIndex += 2;
						else
							bEdge[Rand_Index] = true;
					}
					else
						bEdge[Rand_Index] = true;
				}
				else
					bEdge[Rand_Index] = true;
				break;
			case 5:
				// ��
				for (int nCnt2 = 0; nCnt2 < m_nXBlock; nCnt2++)
				{
					// �񂲂Ƃ̈�ԉ��̔ԍ�
					int Under = up_down * (m_nZBlock - 1) + nCnt2 * 2;

					// ���݂̔ԍ�����ԉ��̔ԍ����ǂ���
					if (nIndex == Under)
						Porigon_Count++;
				}
				if (Porigon_Count == 0)
				{
					// ���̔ԍ��Ɉړ�����
					nIndex += up_down;
				}
				else
					bEdge[Rand_Index] = true;
				break;
			case 6:
				// ����
				for (int nCnt2 = 0; nCnt2 < m_nXBlock; nCnt2++)
				{
					// �񂲂Ƃ̈�ԉ��̔ԍ�
					int Under = up_down * (m_nZBlock - 1) + nCnt2 * 2;

					// ���݂̔ԍ�����ԉ��̔ԍ����ǂ���
					if (nIndex == Under)
						Porigon_Count++;
				}
				if (Porigon_Count == 0)
				{
					// ���̔ԍ��Ɉړ�����
					nIndex += (m_nXBlock + 1) * 2 + 2;

					// ���Ɉړ�����
					if (pVtx[pIdx[nIndex]].pos != pVtx[pIdx[nIndex - 1]].pos)
						nIndex -= 2;
					else
						bEdge[Rand_Index] = true;
				}
				else
					bEdge[Rand_Index] = true;
				break;
			case 7:
				// �E��
				for (int nCnt2 = 0; nCnt2 < m_nXBlock; nCnt2++)
				{
					// �񂲂Ƃ̈�ԉ��̔ԍ�
					int Under = up_down * (m_nZBlock - 1) + nCnt2 * 2;

					// ���݂̔ԍ�����ԉ��̔ԍ����ǂ���
					if (nIndex == Under)
						Porigon_Count++;
				}
				if (Porigon_Count == 0)
				{
					// ���̔ԍ��Ɉړ�����
					nIndex += (m_nXBlock + 1) * 2 + 2;

					// �E�Ɉړ�����
					if (pVtx[pIdx[nIndex + 1]].pos != pVtx[pIdx[nIndex + 2]].pos)
						nIndex += 2;
					else
						bEdge[Rand_Index] = true;
				}
				else
					bEdge[Rand_Index] = true;
				break;
			default:
				break;
			}
		}
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pIdxBuff->Unlock();

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return fPosY;
}

//==============================================================================================
// �@���̐ݒ�
//==============================================================================================
void CMeshField::Normal()
{
	//���_���ւ̃|�C���^
	VERTEX_3D *pVtx = NULL;

	//���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < m_nVertexNum; nCnt++)
	{
		// �v�Z�p�x�N�g��
		D3DXVECTOR3 Vector[2];

		Vector[0] = pVtx[pIdx[nCnt + 1]].pos - pVtx[pIdx[nCnt]].pos;
		Vector[1] = pVtx[pIdx[nCnt + 2]].pos - pVtx[pIdx[nCnt]].pos;

		// �@���ʃx�N�g��
		if ((nCnt % 2) == 0)
			D3DXVec3Cross(&m_NorVec, &Vector[0], &Vector[1]);
		else
			D3DXVec3Cross(&m_NorVec, &Vector[1], &Vector[0]);

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&m_NorVec, &m_NorVec);

		for (int nCntV = 0; nCntV < 3; nCntV++)
		{
			pVtx[pIdx[nCnt]].nor += m_NorVec;
			
			// �x�N�g���̐��K��
			D3DXVec3Normalize(&pVtx[pIdx[nCnt]].nor, &pVtx[pIdx[nCnt]].nor);
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//==============================================================================================
// ��������
//==============================================================================================
CMeshField* CMeshField::Create(D3DXVECTOR3 pos, int Xblock, int Zblock, float size)
{
	CMeshField* pMeshField = nullptr;

	pMeshField = new CMeshField(CObject::PRIORITY_BACK_GROUND);

	if (pMeshField != nullptr)
	{
		pMeshField->SetSize(size);					// �T�C�Y�̐ݒ�
		pMeshField->SetNumBlock(Xblock, Zblock);	// �u���b�N�̐�
		pMeshField->SetPos(pos);					// �ʒu�̐ݒ�
		pMeshField->Init();							// ����������
	}

	return pMeshField;
}