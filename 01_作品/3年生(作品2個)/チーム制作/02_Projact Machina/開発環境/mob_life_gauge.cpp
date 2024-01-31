//==============================================================================================
//
// �G���G�̗̑̓Q�[�W	�@�@mob_life_gauge.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"mob_life_gauge.h"
#include"game.h"
#include"character.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CMob_Life_Gauge::CMob_Life_Gauge()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CMob_Life_Gauge::~CMob_Life_Gauge()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CMob_Life_Gauge::Init()
{
	CGauge_Manager::Init();

	const D3DXVECTOR3 pos = GetGaugePos();
	const D3DXVECTOR2 size = GetGaugeSize();

	// �Q�[�W�̌��̒���
	SetBeaseSize((int)GetGaugeSize().x);

	// ���̃Q�[�W
	m_BackGauge = CObject3D::Create(pos, size, PRIORITY_CENTER, { 0.0f,0.0f,0.0f,1.0f }, true);

	// �O���̃Q�[�W
	m_FrontGauge = CObject3D::Create(pos, size, PRIORITY_FRONT, { 1.0f,1.0f,1.0f,1.0f }, true);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CMob_Life_Gauge::Uninit()
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
void CMob_Life_Gauge::Update()
{
	const D3DXVECTOR3 pos = GetGaugePos();

	m_BackGauge->SetPos(pos);
	m_FrontGauge->SetPos(pos);

	CGauge_Manager::Update();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CMob_Life_Gauge::Draw()
{
	CGauge_Manager::Draw();
}

//==============================================================================================
// �Q�[�W�̑���
//==============================================================================================
void CMob_Life_Gauge::Fluctuation()
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
void CMob_Life_Gauge::SetDrawGauge(bool draw)
{
	m_BackGauge->SetDrawFlag(draw);
	m_FrontGauge->SetDrawFlag(draw);
}

//==============================================================================================
// ��������
//==============================================================================================
CMob_Life_Gauge *CMob_Life_Gauge::Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size)
{
	CMob_Life_Gauge *pMob_Life_Gauge = new CMob_Life_Gauge;

	if (pMob_Life_Gauge != nullptr)
	{
		pMob_Life_Gauge->SetGaugePos(pos);
		pMob_Life_Gauge->SetGaugeSize(size);
		pMob_Life_Gauge->Init();
	}

	return pMob_Life_Gauge;
}