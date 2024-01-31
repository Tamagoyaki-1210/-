//==============================================================================================
//
// �v���C���[�̗̑̓Q�[�W	�@�@player_life_gauge.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"player_life_gauge.h"
#include"player_manager.h"
#include"application.h"
#include "tutorial.h"
#include "game.h"
#include "object2D.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CPlayer_Life_Gauge::CPlayer_Life_Gauge()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CPlayer_Life_Gauge::~CPlayer_Life_Gauge()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CPlayer_Life_Gauge::Init()
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
	m_FrontGauge->SetCol({ 0.0f,1.0f,1.0f,1.0f });

	// �Q�[�W�̌��̒���
	SetBeaseSize((int)GetGaugeSize().y);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CPlayer_Life_Gauge::Uninit()
{
	// ���̃Q�[�W���g�p���̏ꍇ
	if (m_BackGauge != nullptr)
	{
		m_BackGauge->Uninit();
		m_BackGauge = nullptr;
	}

	// �O���̃Q�[�W���g�p���̏ꍇ
	if (m_FrontGauge != nullptr)
	{
		m_FrontGauge->Uninit();
		m_FrontGauge = nullptr;
	}

	CGauge_Manager::Uninit();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CPlayer_Life_Gauge::Update()
{
	CGauge_Manager::Update();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CPlayer_Life_Gauge::Draw()
{
	CGauge_Manager::Draw();
}

//==============================================================================================
// ��������
//==============================================================================================
CPlayer_Life_Gauge *CPlayer_Life_Gauge::Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size)
{
	CPlayer_Life_Gauge *pPlayer_Life_Gauge = new CPlayer_Life_Gauge;

	if (pPlayer_Life_Gauge != nullptr)
	{
		pPlayer_Life_Gauge->SetGaugePos(pos);
		pPlayer_Life_Gauge->SetGaugeSize(size);
		pPlayer_Life_Gauge->Init();
	}

	return pPlayer_Life_Gauge;
}

//==============================================================================================
// �Q�[�W�̑���
//==============================================================================================
void CPlayer_Life_Gauge::Fluctuation()
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

	if (pPlayer != nullptr)
	{
		// ���݂̗̑͂̊���
		m_fLife_Percent = (float)pPlayer->GetLife() / GetBeaseLife() * 100;

		// �Q�[�W�T�C�Y�𓯂������ɂ���
		float Gauge_Percent = GetGaugeSize().y * m_fLife_Percent / 100;

		// �Q�[�W�̑���
		m_FrontGauge->SetSubSize({ 0.0f, -GetBeaseSize() + Gauge_Percent });
	}

	// �F�̐ݒ�
	Col();
}

//==============================================================================================
// �F�̐ݒ�
//==============================================================================================
void CPlayer_Life_Gauge::Col()
{
	// ���݂̗̑͂̊���
	if (m_fLife_Percent <= 25.0f)
		m_FrontGauge->SetCol({ 1.0f,0.0f,0.0f,1.0f });
	else
		m_FrontGauge->SetCol({ 0.0f,1.0f,1.0f,1.0f });
}

//==============================================================================================
// �Q�[�W�`��̐ݒ�
//==============================================================================================
void CPlayer_Life_Gauge::SetDrawGauge(bool draw)
{
	m_BackGauge->SetDrawFlag(draw);
	m_FrontGauge->SetDrawFlag(draw);
}