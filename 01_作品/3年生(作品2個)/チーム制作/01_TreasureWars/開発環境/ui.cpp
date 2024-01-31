//=============================================================================
//
// ui.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "ui.h"
#include "Application.h"

std::vector<CUi*> CUi::m_AllUi = {};
//=====================================
// �R���X�g���N�^
//=====================================
CUi::CUi() : m_bNoneDispray(false)
{
	m_AllUi.push_back(this);
}

//=====================================
// �f�X�g���N�^
//=====================================
CUi::~CUi()
{
	// �f�X�g���N�^�̃^�C�~���O�Ŏ��g��z�񂩂珜�O����
	m_AllUi.erase(std::find(m_AllUi.begin(), m_AllUi.end(), this));
}

//============================================================================
// �I������
//============================================================================
void CUi::Uninit()
{
	// ��ɂȂ�܂ŏI������
	while (!m_listObj2D.empty())
	{
		m_listObj2D.back()->Uninit();
		m_listObj2D.pop_back();
	}

	Release();
}

//============================================================================
// �X�V����
//============================================================================
void CUi::Update()
{
	//// 2D�I�u�W�F�N�g���g�p���̏ꍇ
	//if (!m_listObj2D.empty() && m_bNoneDispray)
	//{
	//	Dispray();	// ��\���ɂ������ꍇ�̏���
	//}
}

//============================================================================
// UI�p�I�u�W�F�N�g2D�̍폜����
//============================================================================
void CUi::DeathObj2D(CObject2D* obj2D)
{
	// �S�Ă�UI�p�I�u�W�F�N�g2D�̒��̑I�񂾃|�C���^�̏���������
	m_listObj2D.erase(std::find(m_listObj2D.begin(), m_listObj2D.end(), obj2D));

	// �I�u�W�F�N�g2D�̏I������
	obj2D->Uninit();
}

////============================================================================
//// UI�̕\������
////============================================================================
//void CUi::Dispray()
//{
//	// UI�̍X�V������ꍇ
//	if (m_bUiUpdate)
//	{
//		SetAlpha(1.0f);
//		SetUiDraw(true);
//		if (m_nNoneDispray_Counter != 0)
//		{
//			m_nNoneDispray_Counter = 0;
//		}
//	}
//	else if (m_nNoneDispray_Counter < None_Dispray_Max)
//	{
//		m_nNoneDispray_Counter++;	// ��\���J�E���^�[��i�߂�
//
//		// ���߂̃J�E���^�[�܂Ői�ނ�
//		if (m_nNoneDispray_Counter >= Invinsible_Dispray_Max)
//		{
//			// ���ߏ���(����)
//			float fInvinsible = (float)(None_Dispray_Max - m_nNoneDispray_Counter) / (float)(None_Dispray_Max - Invinsible_Dispray_Max);
//			SetAlpha(fInvinsible);
//
//			// ��\���J�E���^�[���ő吔�𒴂����ꍇ
//			if (m_nNoneDispray_Counter >= None_Dispray_Max)
//			{
//				SetUiDraw(false);
//			}
//		}
//	}
//
//	m_bUiUpdate = false;	// �ǂݍ��ݏI����ɍX�V������
//}

//============================================================================
// �����I��UI�\���ݒ菈��
//============================================================================
void CUi::SetCompulsDispray(const bool display)
{
	// // �S�Ă�2D�I�u�W�F�N�g
	for (auto pObj2D : m_listObj2D)
	{
		// 2D�I�u�W�F�N�g�̂ǂꂩ���`�悳��Ă���ꍇ�̓��[�v�𔲂���
		if (pObj2D->GetDrawFlag() == true)
		{
			break;
		}
		else
		{
			// �S�Ă�2D�I�u�W�F�N�g���`�悳��Ă��Ȃ��ꍇ
			if (pObj2D == m_listObj2D.back())
			{
				// �S�ĕ`�悷��
				SetUiDraw(true);
			}
		}
	}

	// �\��������ꍇ
	if (display == true)
	{
		// UI��\��������
		SetAlpha(1.0f);
		//m_nNoneDispray_Counter = 0;
	}
	// �\�������Ȃ��ꍇ
	else
	{
		// UI�������n�߂�
		//m_nNoneDispray_Counter = None_Dispray_Max;
		SetAlpha(0);
		SetUiDraw(false);
	}

	// UI��UV��ݒ肷��
	SetUiUV();
}