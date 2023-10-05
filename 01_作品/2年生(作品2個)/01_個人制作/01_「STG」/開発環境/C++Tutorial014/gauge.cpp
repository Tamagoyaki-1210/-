//=============================================================================
//
// �Q�[�W
// Author : tanimoto kosuke
//
//=============================================================================
#include "gauge.h"
#include "application.h"
#include "texture.h"
#include "life.h"

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CGauge::CGauge() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CGauge::~CGauge()
{

}

//=====================================
//����������
//=====================================
void CGauge::Init()
{
	CObject2D::Init();

	m_fWidth = GAUGE_WIDTH;
	m_fHeight = GAUGE_HEIGHT;

	//�I�u�W�F�N�g�ݒ菈��
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);

	CObject2D::SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.8f));
}

//=====================================
//�I������
//=====================================
void CGauge::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CGauge::Update()
{
	CObject2D::Update();

	//UV�ݒ�
	CObject2D::SetUV();

	//�ʒu�擾
	m_pos = CObject2D::GetPos();

	SetPos(m_pos);
}

//=====================================
//�`�揈��
//=====================================
void CGauge::Draw()
{
	//�摜�擾����
	CApplication::GetTexture()->TextureType(CTexture::TYPE_GAUGE);
	CObject2D::Draw();
}

//=====================================
//�Q�[�W�ݒ菈��
//=====================================
void CGauge::SetGauge(int life, int maxlife, int fastmaxlife)
{
	int nDeffMaxLife = maxlife - fastmaxlife;	//�ő�̗͕��Q�[�W�����ɐL�΂�
	float fGauge = (float)life / (float)maxlife;
	float fWidth = (m_fWidth + (float)nDeffMaxLife) * fGauge;

	m_move.x = m_fWidth - fWidth;
	float fLength = sqrtf((fWidth * fWidth) + (m_fHeight * m_fHeight)) * 0.5f;
	CObject2D::SetLength(fLength);

	float fAngle = atan2f(fWidth, m_fHeight);
	CObject2D::SetAngle(fAngle);

	m_pos = m_basepos - m_move / 2.0f;
	SetPos(m_pos);

	if (nDeffMaxLife > 0)
	{
		CObject2D::SetColor(D3DXCOLOR(0.5f, 1.0f, 0.5f, 0.8f));
	}
}

//=====================================
//��������
//=====================================
CGauge *CGauge::Create(D3DXVECTOR3 pos)
{
	CGauge* pGauge = new CGauge;	//�G�L�����|�C���^

	if (pGauge != nullptr)
	{
		pGauge->Init();
		pGauge->SetPos(pos);
		pGauge->m_basepos = pos;
	}
	return pGauge;
}