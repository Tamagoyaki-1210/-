//=============================================================================
//
// parts.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "parts.h"
#include "application.h"
#include "character.h"

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CParts::CParts(const CObject::PRIORITY priority) : CObject(priority)
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CParts::~CParts()
{
}

//============================================================================
// ����������
//============================================================================
HRESULT CParts::Init()
{
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CParts::Uninit()
{
	// �S�Ẵ��f���̏I��
	for (auto pModel : m_vpModel)
	{
		pModel->Uninit();
		pModel = nullptr;
	}
	m_vpModel.clear();

	Release();
}

//============================================================================
// �X�V����
//============================================================================
void CParts::Update()
{
	// ���[�V������؂�ւ���ꍇ
	ChangeMotion();

	// ���[�V����
	Motion();

	// �p�x�̐��K��
	NormalizeRot();
}

//============================================================================
// �`�揈��
//============================================================================
void CParts::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{
		// �e�̃}�g���b�N�X�̎擾
		mtxParent = m_pParent->GetWorldMtx();

		// �e���f���̃}�g���b�N�X�Ƃ̊|���Z
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//==============================================================================================
// ���[�V�����̍Đ�
//==============================================================================================
void CParts::Motion()
{
	// ���[�V�������X�g�b�v���Ă���ꍇ
	if (!m_bMotionStop)
	{
		// ���f���Z�b�g���g�p����Ă���ꍇ
		if (!m_vpModel.empty())
		{
			// ���݃��[�V�����̃��[�V���������擾(���[�V�����ԍ�, ���g�̖��O)
			CMotion::MotionPattern MotionSet = CApplication::GetMotion()->GetMotionPattern(m_nCurrentMotion, m_sMotionName);

			// ���[�v��Ԃ̎擾
			m_bLoop = MotionSet.bLoop;

			// �S�Ẵ��f����ǂݍ���
			for (int nCnt = 0; nCnt < (int)m_vpModel.size(); nCnt++)
			{
				// ���f�����g�p���̏ꍇ
				if (m_vpModel[nCnt] != nullptr)
				{
					// ���Βl
					float fRelative = (float)m_nCountMotion / (float)MotionSet.aKeySet[m_nCurrentKey].nFrame;

					// ����
					D3DXVECTOR3 fPosDifference;		// �ʒu�p
					D3DXVECTOR3 fRotDifference;		// ��]�p

													// �Ō�̃L�[����Ȃ��ꍇ
					if (m_nCurrentKey != MotionSet.nMaxKey - 1)
					{
						// �ʒu
						fPosDifference = MotionSet.aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyPos
							- MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;					// �I���l - �J�n�l

																									// �p�x
						fRotDifference = MotionSet.aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyRot
							- MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;					// �I���l - �J�n�l
					}
					// �Ō�̃L�[�̏ꍇ
					else if (m_nCurrentKey == MotionSet.nMaxKey - 1)
					{
						// �ʒu
						fPosDifference = MotionSet.aKeySet[0].aKey[nCnt].KeyPos
							- MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;	// �I���l - �J�n�l

																					// �p�x
						fRotDifference = MotionSet.aKeySet[0].aKey[nCnt].KeyRot
							- MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;	// �I���l - �J�n�l
					}

					// �p�[�c�t�@�C���������ǂݍ���
					CParts_File::MODEL_SET ModelSet = CApplication::GetPartsFile()->GetPartsSet(m_nPartsIndex).ModelSet[nCnt];

					// ���ݒl
					D3DXVECTOR3 Pos = ModelSet.InitPos + MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// �ʒu
					D3DXVECTOR3 Rot = ModelSet.InitRot + MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// ��]

					m_vpModel[nCnt]->SetPos(Pos);	// �ʒu�̐ݒ�
					m_vpModel[nCnt]->SetRot(Rot);	// ��]�̐ݒ�

					// �e���f���̈ʒu�𒆐S�ʒu�ɐݒ�
					if (ModelSet.nParentIndex < 0 && m_pParent != nullptr)
					{
						// ���S�ʒu�̐ݒ�
						m_pParent->SetCenterPos(Pos);
					}
				}
			}

			// ���[�V�����J�E���^�[
			m_nCountMotion++;

			// �Ō�̃t���[���܂Ői��
			if (m_nCountMotion >= MotionSet.aKeySet[m_nCurrentKey].nFrame)
			{
				// �t���[����0�ɖ߂�
				m_nCountMotion = 0;

				// �L�[��i�߂�
				m_nCurrentKey++;

				if (m_nCurrentKey >= MotionSet.nMaxKey)
				{
					// ���[�v����
					if (MotionSet.bLoop)
					{
						// �L�[��߂��ă��[�v����
						m_nCurrentKey = 0;
					}
					// ���[�v���Ȃ�
					else
					{
						m_bMotionStop = true;
					}
				}
			}
		}
	}
}

//==============================================================================================
// ���[�V�����ύX
//==============================================================================================
void CParts::ChangeMotion()
{
	// ���[�V�������؂�ւ�����ꍇ
	if (m_nCurrentMotion != m_nMotion)
	{
		// �O��̎~�܂������[�V������false�ɂ���
		m_bMotionStop = false;

		m_nCurrentMotion = m_nMotion;	// ���[�V������؂�ւ���
		m_nCurrentKey = 0;				// �L�[��0�ɂ���
		m_nCountMotion = 0;				// ���[�V�����J�E���g��0�ɂ���
	}
}

//==============================================================================================
// ��]�����֐U��������鏈��
//==============================================================================================
void CParts::Rotation()
{
	// �ړI�̊p�x�ɂ���
	m_rot.y += (m_rotDest.y - m_rot.y) * m_fRotSpeed;
}

//==============================================================================================
// �p�x�̐��K��
//==============================================================================================
void CParts::NormalizeRot()
{
	// �p�x�̐��K��
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	// �ړI�̊p�x�̐��K��
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2;
	}
}

//==============================================================================================
// �e�p�[�c�𑼃p�[�c�̎q�ɐݒ肷�鏈��
//==============================================================================================
void CParts::SetModelParent(CObjectX* parts, const bool parent)
{
	// �p�[�c�t�@�C���������ǂݍ���
	CParts_File::MODEL_SET ModelSetBody = CApplication::GetPartsFile()->GetPartsSet(m_nPartsIndex).ModelSet[0];

	ModelSetBody.nParentIndex = parent;
	m_vpModel[0]->SetParent(parts);
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
void CParts::SetParts(const int partsIndex)
{
	// ���g�̃p�[�c�̔ԍ�
	m_nPartsIndex = partsIndex;

	int nCntModel = 0;	// ���f����

	// �w�肳�ꂽ�p�[�c�̑S�Ẵ��f���̏���ǂݍ���
	for (auto ModelSet : CApplication::GetPartsFile()->GetPartsSet(m_nPartsIndex).ModelSet)
	{
		CObjectX* pParent = nullptr;

		// �e�|�C���^�����݂���ꍇ
		if (ModelSet.nParentIndex >= 0)
		{
			pParent = m_vpModel[ModelSet.nParentIndex];
		}

		// ���f�����g�p����Ă��Ȃ��ꍇ
		if ((int)m_vpModel.size() <= nCntModel)
		{
			// ���f���𐶐�����
			m_vpModel.push_back(CObjectX::Create(ModelSet.InitPos, ModelSet.InitRot, pParent, ModelSet.nModelIndex));
		}
		// ���f�����g�p����Ă���ꍇ
		else
		{
			// �e�|�C���^�̐ݒ�
			m_vpModel[nCntModel]->SetParent(pParent);

			// ���f���ԍ��̐ݒ�
			m_vpModel[nCntModel]->SetModel(ModelSet.nModelIndex);
		}

		// �e�|�C���^�����݂��Ȃ��ꍇ
		if (ModelSet.nParentIndex < 0)
		{
			m_vpModel.back()->SetParts(true);
		}
		nCntModel++;
	}
	// �ݒ肵���p�[�c�̃��f�������̈悪�傫�������ꍇ
	if ((int)m_vpModel.size() > nCntModel)
	{
		const int nModelSize = (int)m_vpModel.size();

		// �������̗̈���폜
		for (int nCnt = 0; nCnt < nModelSize - nCntModel; nCnt++)
		{
			m_vpModel.back()->Uninit();
			m_vpModel.back() = nullptr;
			m_vpModel.pop_back();
		}
	}
}

//============================================================================
// ��������
//============================================================================
CParts* CParts::Create(const D3DXVECTOR3 pos, const int partsIndex, CMotion::MOTION motion, CCharacter* parent)
{
	CParts* pParts = new CParts;

	if (FAILED(pParts->Init()))
	{
		return nullptr;
	}

	pParts->SetPos(pos);
	pParts->SetParts(partsIndex);
	pParts->SetMotionName(CMotion::m_cMotionFileName[motion]);
	pParts->SetParent(parent);

	return pParts;
}
