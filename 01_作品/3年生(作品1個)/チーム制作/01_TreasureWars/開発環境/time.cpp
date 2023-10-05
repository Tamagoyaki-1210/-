//==============================================================================
//
// time.cpp
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "application.h"
#include "renderer.h"
#include "time.h"
#include "countdown.h"
#include "game.h"
#include "fade.h"
#include "fever_time.h"

//==============================================================================
// �ÓI�����o�ϐ��錾
//==============================================================================

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CTime::CTime()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CTime::~CTime()
{
}

//==============================================================================
// ����������
//==============================================================================
HRESULT CTime::Init()
{
	//==================================================
	// �����o�ϐ��̏�����
	//==================================================  
	m_nTime = MaxTime;				// ���Ԃ̒l�̏�����
	m_nCntTime = 0;					// ���Ԃ̃J�E���g�̏�����
	m_nZeroCount = 0;				// 0�̐��̏�����
	m_count = 300;					// ��ԕω��̃J�E���g�̏�����
	m_LastCount = LastSpurtTime;	// �ړ��܂ł̎��Ԃ̏�����
	m_nSizeX = 50.0f;				// X�T�C�Y�̏�����
	m_nSizeY = 50.0f;				// Y�T�C�Y�̏�����
	m_nAlpha = 1.0f;				// �����x�̏�����
	m_Type = Flash_In;				// ��Ԃ̏�����

	m_bTimeUp = false;				// �^�C���A�b�v�������ǂ����̔���
	m_bLast = false;				// �c�莞�Ԃ̔���

	//�����̐���
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(540.0f + (70.0f * nCnt) + m_nSizeX / 2, 50.0f, 0.0f), m_nSizeX, m_nSizeY);
	}

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CTime::Uninit()
{
	//�����o�ϐ��̏�����
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}

	// �I�u�W�F�N�g���g�p���̏ꍇ
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Uninit();
		m_pObject2D = nullptr;
	}
}

//==============================================================================
// �X�V����
//==============================================================================
void CTime::Update()
{
	//�^�C���̐ݒ�
	SetTime();

	//�c�莞�Ԃ̐ݒ�
	SetRemainingTime(30);

	// �x��̏���
	Warning(30);
}

//==============================================================================
// �^�C���̐ݒ�
//==============================================================================
void CTime::SetTime()
{
	// �X�^�[�g����܂ł͎��Ԃ�i�߂Ȃ�
	if (CApplication::GetGameStart())
	{
		m_nCntTime++;

		// 1�b = 60f && �c�莞�Ԃ�0���傫���ꍇ
		if (m_nCntTime > 60 && m_nTime > 0)
		{
			m_nTime--;
			m_nCntTime = 0;
		}

		// �c�莞�Ԃ�0�ȉ��ɂȂ���
		if(m_nTime <= 0)
		{
			// �J_�I��SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_FINISH);

			// �^�C���A�b�v��\��
			m_pObject2D = CObject2D::Create({ 1280.0f / 2,720.0f / 2,0.0f }, { 300.0f, 200.0f, 0.0f });
			m_pObject2D->SetTexture(CTexture::TEXTURE_TIMEUP);

			// �Q�[�����I��������
			CApplication::SetGameStart(false);

			// �^�C���A�b�v������
			m_bTimeUp = true;
		}

	}

	// �^�C���A�b�v���Ă���ꍇ
	if (m_bTimeUp)
	{
		m_nTimeUp_Count++;

		// ���U���g�Ƀt�F�[�h
		if (m_nTimeUp_Count == 120)
		{
			CFade::SetFade(CApplication::MODE_RESULT, 0.01f);
		}
	}

	if (m_nTime != 0)
	{
		m_aPosTexU[0] = m_nTime % 1000 / 100;
		m_aPosTexU[1] = m_nTime % 100 / 10;
		m_aPosTexU[2] = m_nTime % 10 / 1;

		for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
		{
			//������,���Ԗڂ�
			m_apNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
		}

		// �擪��0����������
		ZoroCount();
	}
}

//==============================================================================
// �c�莞�Ԃ̐ݒ菈��
//==============================================================================
void CTime::SetRemainingTime(int time)
{
	// ������
	m_nZeroCount = 0;

	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		//�ʒu�̎擾
		D3DXVECTOR3 pos = m_apNumber[nCnt]->Getposition();

		// time�̈ړ�
		if (m_nTime <= time + 2)
		{
			if (pos.y < 360.0f && m_bLast == false)
			{
				m_moveY -= 0.015f;
				m_nSizeX -= 0.05f;
				m_nSizeY -= 0.05f;
			}
			else if (pos.y > 360.0f && m_bLast == false)
			{
				m_moveY = 0.0f;		
				m_nSizeX += 0.5f;
				m_nSizeY += 0.5f;
				m_count--;
				if (m_nTime <= time && m_count <= 0)
				{
					m_bLast = true;
				}
			}

			if (pos.y > 250.0f && m_nTime > 30)
			{
				m_apNumber[1]->SetMove(D3DXVECTOR3(m_moveX += 1.0f, 0.0f, 0.0f));
				m_apNumber[2]->SetMove(D3DXVECTOR3(m_moveX -= 1.0f, 0.0f, 0.0f));
			}

			if (m_bLast == true && pos.y > 50.0f)
			{
				m_moveY += 0.05f;
				m_nSizeX -= 0.45f;
				m_nSizeY -= 0.45f;
				m_apNumber[1]->SetMove(D3DXVECTOR3(m_moveX -= 0.35f, 0.0f, 0.0f));
				m_apNumber[2]->SetMove(D3DXVECTOR3(m_moveX += 0.35f, 0.0f, 0.0f));
			}
			else if (pos.y < 50.0f)
			{
				m_moveY = 0.0f;
				m_moveX = 0.0f;
				m_nSizeX = 50.0f;
				m_nSizeY = 50.0f;
			}
		}

		// ��ԊǗ�
		if (m_nTime <= time)
		{
			m_count--;
			if (m_Type == Flash_In && m_nTime % 2 == 1)
			{
				m_nAlpha -= 0.005f;
				if (m_nAlpha <= 0.1f)
				{
					m_Type = Flash_Out;
					m_count = FlashingTime;
				}
			}
			else if (m_Type == Flash_Out && m_nTime % 2 <= 0)
			{
				m_nAlpha += 0.005f;
				if (m_nAlpha >= 1.0f)
				{
					m_Type = Flash_In;
					m_count = FlashingTime;
				}
			}

			//�F�̐ݒ�
			m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_nAlpha));

			// �擪��0����������
			ZoroCount();	
		}

		//�T�C�Y�̐ݒ�
		m_apNumber[nCnt]->SetSize(m_nSizeX, m_nSizeY);
		//�ړ��̐ݒ�
		m_apNumber[nCnt]->SetMove(D3DXVECTOR3(m_moveX, m_moveY, 0.0f));

	}
}

//==============================================================================
// �x��̏���
//==============================================================================
void CTime::Warning(const int time)
{
	// �t�B�[�o�[�^�C����2�b�O�ɂȂ��
	if ((m_nTime == time + 2) && m_nCntTime == 0)
	{
		m_pWarning = CObject2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(910.0f, 510.0f, 0.0f), 5);
		m_pWarning->SetCol({ 1.0f, 0.0f, 0.0f, 0.0f });

		// �x��SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WARNING);
	}
	// �t�B�[�o�[�^�C�����n�܂�����
	else if ((m_nTime == time) && m_nCntTime == 0)
	{
		m_pWarning->Uninit();
		m_pWarning = nullptr;
		return;
	}

	// �x�񂪎g�p���̏ꍇ
	if (m_pWarning != nullptr)
	{
		const float flpha = 0.2f / Warning_Blink_Interval;
		m_pWarning->AddAlpha((m_nCntTime % (Warning_Blink_Interval * 2)) < Warning_Blink_Interval ? flpha : -flpha);
	}
}

//==============================================================================
// �擪��0����������
//==============================================================================
void CTime::ZoroCount()
{
	if (m_apNumber[m_nZeroCount] != nullptr)
	{
		if (m_aPosTexU[m_nZeroCount] == 0 && m_nTime != 0)
		{
			m_apNumber[m_nZeroCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCount++;
		}
		else if (m_aPosTexU[m_nZeroCount] == 0 && m_nTime == 0)
		{
			m_apNumber[2]->SetTexPos(0.0f, 1.0f, 0.1f, 0.0f);
			m_apNumber[m_nZeroCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCount++;
		}
	}
}

//==============================================================================
// �^�C���̐���
//==============================================================================
CTime *CTime::Create()
{
	CTime *pTime = nullptr;

	//�N���X�̐���
	pTime = new CTime;				//�^�C���̃C���X�^���X����

	//null�`�F�b�N
	if (pTime != nullptr)
	{
		//����������
		pTime->Init();
	}
	else
	{
		assert(false);
	}

	return pTime;
}