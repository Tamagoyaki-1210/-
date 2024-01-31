//==============================================================================================
//
// �{�X�̗̑̓Q�[�W	�@�@boss_life_gauge.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"boss_life_gauge.h"
#include"game.h"
#include"character.h"
#include "object2D.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CBoss_Life_Gauge::CBoss_Life_Gauge()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CBoss_Life_Gauge::~CBoss_Life_Gauge()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CBoss_Life_Gauge::Init()
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
	m_FrontGauge->SetCol({ 1.0f,0.0f,0.0f,1.0f });

	// �Q�[�W�̌��̒���
	SetBeaseSize((int)GetGaugeSize().x);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CBoss_Life_Gauge::Uninit()
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
void CBoss_Life_Gauge::Update()
{
	CGauge_Manager::Update();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CBoss_Life_Gauge::Draw()
{
	CGauge_Manager::Draw();
}

//==============================================================================================
// �Q�[�W�̑���
//==============================================================================================
void CBoss_Life_Gauge::Fluctuation()
{
	// ���݂̗̑͂̊���
	float Life_Percent = (float)GetLife() / GetBeaseLife() * 100;

	// �Q�[�W�T�C�Y�𓯂������ɂ���
	float Gauge_Percent = GetGaugeSize().x * Life_Percent / 100;

	// �Q�[�W�̑���
	m_FrontGauge->SetSubSize({ GetBeaseSize() - Gauge_Percent, 0.0f });
}

//==============================================================================================
// �Q�[�W�`��̐ݒ�
//==============================================================================================
void CBoss_Life_Gauge::SetDrawGauge(bool draw)
{
	m_BackGauge->SetDrawFlag(draw);
	m_FrontGauge->SetDrawFlag(draw);
}

//==============================================================================================
// ��������
//==============================================================================================
CBoss_Life_Gauge *CBoss_Life_Gauge::Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size)
{
	CBoss_Life_Gauge *pBoss_Life_Gauge = new CBoss_Life_Gauge;
	
	if (pBoss_Life_Gauge != nullptr)
	{
		pBoss_Life_Gauge->SetGaugePos(pos);
		pBoss_Life_Gauge->SetGaugeSize(size);
		pBoss_Life_Gauge->Init();
	}

	return pBoss_Life_Gauge;
}