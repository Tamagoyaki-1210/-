//=============================================================================
//
// �w�i
// Author : tanimoto kosuke
//
//=============================================================================
#include "bg.h"
#include "application.h"
#include "texture.h"

//---------------------------
//�}�N���֐�
//---------------------------
#define BG_TUTORIAL		(800)

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CBG::CBG()
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CBG::~CBG()
{

}

//=====================================
//����������
//=====================================
void CBG::Init()
{
	CObject2D::Init();

	//�O���t�@�C���ŕϐ��ɒl�����������g
	m_fWidth = BG_WIDTH;
	m_fHeight = BG_HEIGHT;

	//�I�u�W�F�N�g�ݒ菈��
	SetObject2D(CObject::TYPE_BG, m_fWidth, m_fHeight);
}

//=====================================
//�I������
//=====================================
void CBG::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CBG::Update()
{
	CObject2D::Update();


	//UV�ݒ菈��
	CObject2D::SetUV();

	//�ʒu�擾
	D3DXVECTOR3 pos = CObject2D::GetPos();

	//�p�x�擾����
	D3DXVECTOR3 rot = CObject2D::GetRot();

	//����
	m_move = m_move - (m_move * 0.05f);

	//�ʒu�X�V
	pos += m_move;

	//�ʒu�ݒ菈��
	SetPos(pos);

	//�p�x�擾����
	CObject2D::SetRot(rot);

	UpdateType();
}

//=====================================
//�`�揈��
//=====================================
void CBG::Draw()
{
	switch (m_bgType)
	{
	case TYPE_BG01 :
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG);
		break;
	case TYPE_Tutorial01:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG_Tutorial01);
		break;
	case TYPE_Tutorial02:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG_Tutorial02);
		break;
	case TYPE_Tutorial03:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG_Tutorial03);
		break;
	case TYPE_Clear:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG);
		break;
	}
	CObject2D::Draw();
}

//=====================================
//��ʍX�V����
//=====================================
void CBG::UpdateType()
{
	switch (m_bgType)
	{
	case TYPE_BG01:
		break;
	case TYPE_Tutorial01:
		if (m_nDestroyCounter <= 0)
		{
			Uninit();
		}
		else
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)m_nDestroyCounter /  BG_TUTORIAL)));
			m_nDestroyCounter--;
		}
		break;
	case TYPE_Tutorial02:
		if (m_nDestroyCounter <= 0)
		{
			Uninit();
		}
		else
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)m_nDestroyCounter / BG_TUTORIAL)));
			m_nDestroyCounter--;
		}
		break;
	case TYPE_Tutorial03:
		if (m_nDestroyCounter <= 0)
		{
			Uninit();
		}
		else
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)m_nDestroyCounter / BG_TUTORIAL)));
			m_nDestroyCounter--;
		}
		break;
	case TYPE_Clear:
		//(��)
		CObject2D::SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f));
		break;
	default:
		break;
	}
}

//=====================================
//�w�i��������
//=====================================
CBG *CBG::Create(D3DXVECTOR3 pos, BG_Type bgType)
{
	CBG* pBG = new CBG;		//�v���C���[�|�C���^

	if (pBG != nullptr)
	{
		pBG->Init();
		pBG->SetPos(pos);
		pBG->m_bgType = bgType;
		if (bgType == TYPE_Tutorial01 || bgType == TYPE_Tutorial02 || bgType == TYPE_Tutorial03)
		{
			pBG->m_nDestroyCounter = BG_TUTORIAL;
		}
	}

	return pBG;
}
