//==============================================================================================
//
// �t�F�[�h�@�@�@fade.cpp
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"fade.h"
#include"application.h"

//**********************************************************************************************
// �ÓI�����o�ϐ��̐錾
//**********************************************************************************************
float CFade::m_fFadeSpeed = 0.0f;
CApplication::MODE CFade::NextMode = CApplication::MODE_NONE;
CFade::FADE CFade::FadeMode = FADE_NONE;
CObject2D* CFade::m_pFadeObj = nullptr;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CFade::CFade()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CFade::~CFade()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CFade::Init()
{
	m_fAlpha = 0.0f;		// �A���t�@�l

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CFade::Uninit()
{
	// �t�F�[�h�p�I�u�W�F�N�g�̏I��
	if (m_pFadeObj != nullptr)
	{
		m_pFadeObj->Uninit();
		m_pFadeObj = nullptr;
	}
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CFade::Update()
{
	if (FadeMode != FADE_NONE)
	{
		// �A���t�@�l�̐ݒ�
		m_pFadeObj->SetAlpha(m_fAlpha);

		if (FadeMode == FADE_IN)
		{
			// �|���S���𓧖��ɂ��Ă���
			m_fAlpha -= m_fFadeSpeed;

			if (m_fAlpha <= 0.0f)
			{
				m_fAlpha = 0.0f;

				//�������ĂȂ���Ԃɂ���
				FadeMode = FADE_NONE;

				// �I������
				if (m_pFadeObj != nullptr)
				{
					m_pFadeObj->Uninit();
					m_pFadeObj = nullptr;
				}
			}
		}
		else if (FadeMode == FADE_OUT)
		{
			//�|���S����s�����ɂ��Ă���
			m_fAlpha += m_fFadeSpeed;

			if (m_fAlpha >= 1.0f)
			{
				m_fAlpha = 1.0f;

				// ��ʑJ��
				CApplication::SetMode(NextMode);
				m_pFadeObj = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), { SCREEN_WIDTH, SCREEN_HEIGHT }, CObject::PRIORITY_SCREEN);
				m_pFadeObj->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fAlpha));

				//�t�F�[�h�C����Ԃɂ���
				FadeMode = FADE_IN;
			}
		}
		
		// �|�[�Y���̂�2D�I�u�W�F�N�g���X�V������
		if (CApplication::GetPauce() == true)
		{
			m_pFadeObj->Update();
		}
	}
}

//==============================================================================================
// ��������
//==============================================================================================
CFade *CFade::Create()
{
	CFade *pFade = nullptr;

	pFade = new CFade;

	if (pFade != nullptr)
	{
		pFade->Init();
	}

	return pFade;
}

//==============================================================================================
// �t�F�[�h����
//==============================================================================================
void CFade::SetFade(CApplication::MODE mode, float fadespeed)
{
	// �t�F�[�h�p�|���S���̐���
	if (m_pFadeObj == nullptr)
	{
		m_pFadeObj = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), { SCREEN_WIDTH, SCREEN_HEIGHT }, CObject::PRIORITY_SCREEN);
		m_pFadeObj->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		m_fFadeSpeed = fadespeed;

		// ���̃��[�h�̐ݒ�
		NextMode = mode;

		// �t�F�[�h�A�E�g��Ԃɂ���
		FadeMode = FADE_OUT;
	}
}