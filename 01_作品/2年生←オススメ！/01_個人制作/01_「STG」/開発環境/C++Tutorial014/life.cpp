//=============================================================================
//
// ���C�t
// Author : tanimoto kosuke
//
//=============================================================================
#include "life.h"
#include "application.h"
#include "texture.h"
#include "gauge.h"

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CLife::CLife() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CLife::~CLife()
{

}

//=====================================
//����������
//=====================================
void CLife::Init()
{
	CObject2D::Init();

	m_fWidth = LIFE_WIDTH;
	m_fHeight = LIFE_HEIGHT;

	//�I�u�W�F�N�g�ݒ菈��
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);

	//�Q�[�W���O�ɕ`�悳�ꂽ�ꍇ�A�����ɂ���
	CObject2D::SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
}

//=====================================
//�I������
//=====================================
void CLife::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CLife::Update()
{
	CObject2D::Update();

	//�ʒu�擾
	D3DXVECTOR3 pos = CObject2D::GetPos();

	SetPos(pos);
}

//=====================================
//�`�揈��
//=====================================
void CLife::Draw()
{
	//�摜�擾����
	CApplication::GetTexture()->TextureType(CTexture::TYPE_LIFE);
	CObject2D::Draw();
}


//=====================================
//�ʒu�ݒ菈��
//=====================================
void  CLife::SetLife(int life, int maxlife, int fastmaxlife)
{
	m_pGauge->SetGauge(life, maxlife, fastmaxlife);
	CObject2D::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f));
}

//=====================================
//��������
//=====================================
CLife *CLife::Create(D3DXVECTOR3 pos)
{
	CLife* pLife = new CLife;	//�G�L�����|�C���^

	if (pLife != nullptr)
	{
		pLife->Init();
		pLife->SetPos(pos);
		pLife->m_pGauge = CGauge::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f));
		pLife->CObject2D::SetUV();
	}
	return pLife;
}