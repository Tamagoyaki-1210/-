//==============================================
//
// Player_Ui.cpp
// Author : Saito Shian
//
//==============================================

//==============================================
// �C���N���[�h
//==============================================
#include <assert.h>
#include "player_ui.h"
#include "texture.h"
#include "input.h"  

//==============================================
// �R���X�g���N�^
//==============================================
CPlayerUi::CPlayerUi(const PRIORITY priority) : CObject2D(priority)
{

}

//==============================================
// �f�X�g���N�^
//==============================================
CPlayerUi::~CPlayerUi()
{

}

//==============================================
// ����������
//==============================================
HRESULT CPlayerUi::Init()
{
	// object2D�̏���������
	CObject2D::Init();

	// �e�N�X�`���̐ݒ�
	SetTexture(CTexture::TEXTURE_UI);

	// UI�̎��
	switch (m_Type)
	{
	case CPlayerUi::UITYPE_SUPPORT:
		SetTexPos(0.0f, 1.0f, 0.33f, 0.0f);
		SetData(1150.0f, 75.0f,100.0f,100.0f, CTexture::TEXTURE_BULLET);
		break;
	case CPlayerUi::UITYPE_ATTACK:
		SetTexPos(0.0f, 1.0f, 0.66f, 0.33f);
		SetData(150.0f, 75.0f, 100.0f, 100.0f, CTexture::TEXTURE_FLOOR);
		break;
	case CPlayerUi::UITYPE_WEAPON:
		SetTexPos(0.0f, 1.0f, 1.0f, 0.66f);
		SetData(1150.0f, 60.0f, 100.0f, 100.0f, CTexture::TEXTURE_NONE);
		break;
	case CPlayerUi::UITYPE_MAX:
		break;
	default:
		break;
	}

	// �����o�ϐ��̏�����
	m_AlphaCount = ALPHA_COUNT;
	m_ColorCount = COLOR_COUNT;
	m_UseCount = USE_COUNT;
	m_Alpha = 1.0f;
	m_Color = 1.0f;
	m_bAlphaFlag = false;
	m_bColorFlag = false;
	m_bFlag = false;

	return S_OK;
}

//==============================================
// �I������
//==============================================
void CPlayerUi::Uninit()
{
	// object2D�̏I������
	CObject2D::Uninit();
}

//==============================================
// �X�V����
//==============================================
void CPlayerUi::Update()
{
	// object2D�̍X�V����
	CObject2D::Update();

	// �����x����
	SetAlpha();
}

//==============================================
// �`�揈��
//==============================================
void CPlayerUi::Draw()
{
	// object2D�̕`�揈��
	CObject2D::Draw();
}

//==============================================
// �����x����
//==============================================
void CPlayerUi::SetAlpha()
{
	// �C���v�b�g�̎擾
	CInput* pInput = CInput::GetKey();

	if (pInput->Trigger(DIK_R) && m_Type == CPlayerUi::UITYPE_SUPPORT)
	{
		m_bFlag = true;
		m_bAlphaFlag = true;
	}
	if (pInput->Trigger(DIK_Q) && m_Type == CPlayerUi::UITYPE_ATTACK)
	{
		m_bFlag = true;
		m_bAlphaFlag = true;
	}

	if (m_bFlag == true)
	{
		m_UseCount--;
		if (m_UseCount >= 0)
		{
			SetColor();
		}
	}

	if (m_UseCount <= 0)
	{
		// �������ɂ���
		if (m_bAlphaFlag == true)
		{
			m_Alpha -= 0.03f;
			if (m_Alpha <= 0.5f)
			{
				m_Alpha = 0.5f;
				m_AlphaCount--;
				if (m_AlphaCount <= 0)
				{
					m_bAlphaFlag = false;
				}
			}
		}

		// ���������猳�ɖ߂�
		if (m_AlphaCount <= 0 && m_bAlphaFlag == false)
		{
			m_Alpha += 0.03f;
			if (m_Alpha >= 1.0f)
			{
				// �ϐ��������l�ɖ߂�
				m_Alpha = 1.0f;
				m_bAlphaFlag = false;
				m_AlphaCount = ALPHA_COUNT;
				m_UseCount = USE_COUNT;
				m_bFlag = false;
			}
		}

		// �����x�̐ݒ�
		m_pSkill_UI->SetAlpha(m_Alpha);
		CObject2D::SetAlpha(m_Alpha);
	}
}

//==============================================
// �F����
//==============================================
void CPlayerUi::SetColor()
{
	m_ColorCount--;
	if (m_bColorFlag == false && m_ColorCount <= 0)
	{
		m_Color = 0.0f;
		m_bColorFlag = true;
		m_ColorCount = COLOR_COUNT;
	}
	else if (m_bColorFlag == true && m_ColorCount <= 0)
	{
		m_Color = 1.0f;
		m_bColorFlag = false;
		m_ColorCount = COLOR_COUNT;
	}

	// �F�̐ݒ�
	m_pSkill_UI->SetCol(D3DXCOLOR(1.0f, m_Color, m_Color, 1.0f));
	CObject2D::SetCol(D3DXCOLOR(1.0f, m_Color, m_Color, 1.0f));
}

//==============================================
// �f�[�^�ݒ菈��
//==============================================
void CPlayerUi::SetData(float posX, float posY, float sizeX, float sizeY, CTexture::TEXTURE texture)
{
	m_pSkill_UI = CObject2D::Create(D3DXVECTOR3(posX, posY, 0.0f), D3DXVECTOR2(sizeX, sizeY), CObject::PRIORITY_BACK);
	m_pSkill_UI->SetTexture(texture);
}

//==============================================
// ��������
//==============================================
CPlayerUi* CPlayerUi::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, UITYPE type, const PRIORITY priority)
{
	//�N���X�̐���
	CPlayerUi* pPlayerUI = new CPlayerUi(priority);

	//null�`�F�b�N
	if (pPlayerUI != nullptr)
	{
		pPlayerUI->SetType(type);
		//����������
		pPlayerUI->Init();
		pPlayerUI->SetPos(pos);
		pPlayerUI->SetSize(size);
	}
	else
	{
		assert(false);
	}

	return pPlayerUI;
}
