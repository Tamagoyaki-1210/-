//==============================================================================================
//
// �G�l���M�[�Q�[�W	�@�@energy_gauge.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"energy_gauge.h"
#include"debugProc.h"
#include "object2D.h"
#include"player_manager.h"
#include"application.h"
#include "tutorial.h"
#include "game.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CEnergy_Gauge::CEnergy_Gauge()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CEnergy_Gauge::~CEnergy_Gauge()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CEnergy_Gauge::Init()
{
	CGauge_Manager::Init();

	const D3DXVECTOR3 pos = GetGaugePos();
	const D3DXVECTOR2 size = GetGaugeSize();

	// ���̃Q�[�W
	m_BackGauge = CObject2D::Create(pos, size, PRIORITY_FRONT);
	// ���̃Q�[�W�̐F
	m_BackGauge->SetCol({ 0.0f,0.0f,0.0f,1.0f });

	// �O���̃Q�[�W
	m_FrontGauge = CObject2D::Create(pos, size, PRIORITY_FRONT);
	// �O���̃Q�[�W�̐F
	m_FrontGauge->SetCol({ 1.0f, 1.0f, 1.0f,1.0f });

	m_fRecovery_Interval = 180.0f;	// �񕜂��n�߂�܂ł̃C���^�[�o��
	m_fReuse_Percent = 30.0f;		// �S�����̉񕜎��ɍė��p�ł���^�C�~���O
	m_bConsumption = false;			// �����
	m_bAllRecovery = true;			// �񕜂��o�����Ԃ�
	m_bAllConsumption = false;		// �G�l���M�[��S�ď����
	m_bRecovery_Pause = false;		// �񕜂��ꎞ��~��

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CEnergy_Gauge::Uninit()
{
	CGauge_Manager::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CEnergy_Gauge::Update()
{
	// �Q�[�W���񕜂���
	Recovery_Gauge();

	// �񕜂��邩
	m_bConsumption = false;

	// �Q�[�W�̑���
	Fluctuation();

	// �Q�[�W�̐F�̐ݒ�
	GaugeColor();

	CGauge_Manager::Update();


	//CDebugProc::Print("�v���C���[�X�^�~�i�F%f / %d\n", m_fFluctuation, m_nMax_Enerugy);
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CEnergy_Gauge::Draw()
{
	CGauge_Manager::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CEnergy_Gauge* CEnergy_Gauge::Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size)
{
	CEnergy_Gauge *pEnergy_Gauge = new CEnergy_Gauge;

	if (pEnergy_Gauge != nullptr)
	{
		pEnergy_Gauge->SetGaugePos(pos);
		pEnergy_Gauge->SetGaugeSize(size);
		pEnergy_Gauge->Init();
	}

	return pEnergy_Gauge;
}

//==============================================================================================
// �Q�[�W�̑���
//==============================================================================================
void CEnergy_Gauge::Fluctuation()
{
	// ���݂̃��[�h
	CApplication::MODE Mode = CApplication::GetModeType();

	CPlayer* pPlayer = nullptr;

	// ���[�h���Ƀv���C���[��ǂݍ���
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
	}

	if (m_bRecovery_Pause)
	{
		m_Pause_Count++;

		if (m_Pause_Count >= m_BasePause_Count)
		{
			m_bRecovery_Pause = false;
			m_bAllRecovery = true;		// �񕜂ł��Ȃ���Ԃɂ���
			m_bAllConsumption = false;	// �G�l���M�[��S�ď����
			m_Pause_Count = 0;
		}
	}
	else if (m_bAllConsumption && !m_bAllRecovery && !m_bRecovery_Pause)
	{
		// �Ō�܂Ō��炷
		m_fFluctuation = (float)m_nMax_Enerugy;
	}

	// ���݂̃G�l���M�[�c��
	float fEnergy = (float)m_nMax_Enerugy - m_fFluctuation;

	// ���݂̃G�l���M�[�c�ʂ̊���
	float fEnergy_Percent = fEnergy / m_nMax_Enerugy * 100;

	// �Q�[�W�T�C�Y���G�l���M�[�̊����ɍ��킹��
	float fGeuge_Size = GetGaugeSize().y * fEnergy_Percent / 100.0f;

	// �Q�[�W�̑���
	//m_FrontGauge->SetSubSize({ GetGaugeSize().x - fGeuge_Size ,0.0f });
	m_FrontGauge->SetSubSize({ 0.0 ,-GetGaugeSize().y + fGeuge_Size });
}

//==============================================================================================
// �G�l���M�[�̉�
//==============================================================================================
void CEnergy_Gauge::Recovery_Gauge()
{
	if (m_bAllRecovery)
	{
		if (!m_bConsumption && m_fFluctuation > 0.0f)
			// �񕜂���G�l���M�[��
			m_fFluctuation -= m_fRecovery_Speed;

		// ���݂̃Q�[�W�c�ʂ̊���
		float Gauge_Percent = (m_nMax_Enerugy - m_fFluctuation) / m_nMax_Enerugy * 100;

		// �񕜓r���G�l���M�[���g����悤�ɂ���^�C�~���O
		if (Gauge_Percent >= m_fReuse_Percent)
			m_bAllConsumption = false;

		// �S�񕜂���
		if (m_fFluctuation <= 0)
			m_fFluctuation = 0.0f;
	}
	else if (!m_bAllRecovery && !m_bRecovery_Pause)
	{
		// �G�l���M�[���؂ꂽ��
		if (m_nInterval_Count == 0)
		{
			// �x��SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WARNING);
		}

		// �C���^�[�o���̃J�E���g
		m_nInterval_Count++;

		if (m_nInterval_Count >= m_fRecovery_Interval)
		{
			// �񕜂ł����Ԃɂ���
			m_bAllRecovery = true;
			m_nInterval_Count = 0;
		}
	}
}

//==============================================================================================
// �G�l���M�[�̏���
//==============================================================================================
void CEnergy_Gauge::Consumption_Gauge()
{
	if (!m_bAllConsumption)
	{
		// �����G�l���M�[��
		if (m_fFluctuation <= m_nMax_Enerugy)
		{
			// ����ʂ̉��Z
			m_fFluctuation += m_fConsumption_Speed;

			// �G�l���M�[���
			m_bConsumption = true;
		}
		else
		{
			m_bAllRecovery = false;		// �񕜂ł��Ȃ���Ԃɂ���
			m_bAllConsumption = true;	// �G�l���M�[��S�ď����
		}
	}
}

//==============================================================================================
// ������̃G�l���M�[����
//==============================================================================================
void CEnergy_Gauge::Avoidance_Energy()
{
	if (!m_bAllConsumption)
	{
		// �����G�l���M�[��
		m_fFluctuation += m_fAvoidance;

		if (m_fFluctuation < m_nMax_Enerugy)
			// �G�l���M�[���
			m_bConsumption = true;
		else
		{
			m_bAllRecovery = false;		// �񕜂ł��Ȃ���Ԃɂ���
			m_bAllConsumption = true;	// �G�l���M�[��S�ď����
		}
	}
}

//==============================================================================================
// �Q�[�W�̐F
//==============================================================================================
void CEnergy_Gauge::GaugeColor()
{
	// ���݂̃Q�[�W�c�ʂ̊���
	float Gauge_Percent = (m_nMax_Enerugy - m_fFluctuation) / m_nMax_Enerugy * 100;

	// �F�̐ݒ�
	if (Gauge_Percent <= 25.0f || m_bAllConsumption && !m_bRecovery_Pause)
		// �c��25%�ȉ�or�S�ď������A�S�񕜂���܂�(��)
		m_FrontGauge->SetCol({ 1.0f,0.0f,0.0f,1.0f });
	else if (Gauge_Percent > 20.0f || !m_bAllConsumption)
		// �c��26%�ȏ�(��)
		m_FrontGauge->SetCol({ 1.0f,0.5f,0.0f,1.0f });

	// ���̃Q�[�W�̐F
	m_BackGauge->SetCol({ 0.0f,0.0f,0.0f,1.0f });

	// �G�l���M�[��S�ď����
	if (!m_bAllRecovery && !m_bRecovery_Pause)
	{
		// �Ԃ��_�ł�����
		m_Col_Count++;
		float fRed = sinf(m_Col_Count * 0.07f);
		m_BackGauge->SetCol({ fRed,0.0f,0.0f,1.0f });
	}
	else
		// �_�ł̃��Z�b�g
		m_Col_Count = 0;
}

//==============================================================================================
// �Q�[�W�񕜂��ꎞ��~
//==============================================================================================
void CEnergy_Gauge::Recovery_Pause(int count)
{
	if (m_bAllRecovery && !m_bAllConsumption)
	{
		m_bAllRecovery = false;		// �񕜂ł��Ȃ���Ԃɂ���
		m_bAllConsumption = true;	// �G�l���M�[��S�ď����
		m_bRecovery_Pause = true;	// ��~��

		// ��~���鎞��
		m_BasePause_Count = count;
	}
}

//==============================================================================================
// �ő�G�l���M�[�̐ݒ�
//==============================================================================================
void CEnergy_Gauge::SetMaxEnerugy(const int enerugy)
{
	// ���݂̃G�l���M�[���ő�G�l���M�[��葽���ꍇ
	if (m_fFluctuation > enerugy)
	{
		m_fFluctuation = (float)enerugy;
	}
	m_nMax_Enerugy = enerugy;
}

//==============================================================================================
// �Q�[�W�`��̐ݒ�
//==============================================================================================
void CEnergy_Gauge::SetDrawGauge(bool draw)
{
	m_BackGauge->SetDrawFlag(draw);
	m_FrontGauge->SetDrawFlag(draw);
}

//==============================================================================================
// �G�l���M�[����ʐݒ菈��
//==============================================================================================
void CEnergy_Gauge::SetEnergyConsumed(const float value)
{
	// �I�[�o�[�q�[�g���Ă��Ȃ��ꍇ
	if (!m_bAllConsumption)
	{
		// �����ʂ����Z����
		m_fFluctuation += value;

		// ���݂̃G�l���M�[�c��
		float fEnergy = (float)m_nMax_Enerugy - m_fFluctuation;

		// �S�ď���Ă��܂����ꍇ
		if (fEnergy <= 0.0f)
		{
			m_fFluctuation = (float)m_nMax_Enerugy;
			m_bAllConsumption = true;
			m_bAllRecovery = false;		// �񕜂ł��Ȃ���Ԃɂ���
		}


		// ���݂̃G�l���M�[�c�ʂ̊���
		float fEnergy_Percent = fEnergy / m_nMax_Enerugy * 100;

		// �Q�[�W�T�C�Y���G�l���M�[�̊����ɍ��킹��
		float fGeuge_Size = GetGaugeSize().y * fEnergy_Percent / 100.0f;

		// �Q�[�W�̑���
		//m_FrontGauge->SetSubSize({ GetGaugeSize().x - fGeuge_Size ,0.0f });
		m_FrontGauge->SetSubSize({ 0.0 ,-GetGaugeSize().y + fGeuge_Size });

		// �G�l���M�[���
		m_bConsumption = true;
	}
}