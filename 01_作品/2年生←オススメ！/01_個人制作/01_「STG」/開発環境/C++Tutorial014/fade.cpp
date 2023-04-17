//=============================================================================
//
// �t�F�[�h
// Author : tanimoto kosuke
//
//=============================================================================
#include "fade.h"
#include <assert.h>
#include <stdio.h>
#include "application.h"
#include "texture.h"

//---------------------------------------------------------
// �}�N����`
//---------------------------------------------------------
#define ALPHA_CHANGE	(0.05f)	// �t�F�[�h��a�l�̕ω���

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CFade::CFade() : CObject2D(UI_PRIORITY_FADE), m_fAlpha(0.0f)
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CFade::~CFade()
{

}

//=====================================
// ����������
//=====================================
void CFade::Init()
{
	CObject2D::Init();

	// �I�u�W�F�N�g�ݒ菈��
	CObject2D::SetObject2D(TYPE_UI, F_SCREEN_WIDTH, F_SCREEN_HEIGHT);
}

//=====================================
// �I������
//=====================================
void CFade::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
// �X�V����
//=====================================
void CFade::Update()
{
	CObject2D::Update();

	// UV�ݒ�
	CObject2D::SetUV();

	if (m_fade == FADE_NONE)
	{// �������Ă��Ȃ���Ԃ̏ꍇ
		return;
	}

	switch (m_fade)
	{
	// �t�F�[�h�A�E�g��Ԃ̏ꍇ
	case FADE_OUT:
		m_fAlpha += ALPHA_CHANGE;	// �|���S����s�����ɂ��Ă���

		if (m_fAlpha >= 1.0f)
		{// ���S�ɕs�����ɂȂ����ꍇ
			m_fAlpha = 1.0f;
			m_fade = FADE_CHANGE;  // �t�F�[�h�؂�ւ����
		}
		break;
	// �t�F�[�h�C����Ԃ̏ꍇ
	case FADE_IN:		
		m_fAlpha -= ALPHA_CHANGE;	// �|���S�������X�ɓ����ɂ���

		if (m_fAlpha <= 0.0f)
		{// ���S�ɓ����ɂȂ����ꍇ
			m_fAlpha = 0.0f;
			m_fade = FADE_CHANGE;	// �t�F�[�h�؂�ւ����
		}
		break;
	// �t�F�[�h�؂�ւ���Ԃ̏ꍇ
	case FADE_CHANGE:
		//�|���S�������S�ɕs�����ȏꍇ
		if (m_fAlpha == 1.0f)
		{
			m_fade = FADE_IN;		// �t�F�[�h�C�����
		}
		//�|���S�������S�ɓ����ȏꍇ
		else if (m_fAlpha == 0.0f)
		{
			m_fade = FADE_NONE;		// �������Ă��Ȃ����
		}
		break;
	default:
		assert(false);
		break;
	}

	SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha));
}

//=====================================
// �`�揈��
//=====================================
void CFade::Draw()
{
	//�摜�擾����
	CApplication::GetTexture()->TextureType(CTexture::TYPE_NONE);
	CObject2D::Draw();
}

//=====================================
// �t�F�[�h�A�E�g�J�n����
//=====================================
void CFade::StartFadeOut()
{
	m_fade = FADE_OUT;	// �t�F�[�h�A�E�g���
	m_fAlpha = 0.0f;	// �|���S���𓧖��ɂ���
}

//=====================================
// �t�F�[�h��������
//=====================================
CFade *CFade::Create(float fAlpha, FADE fade)
{
	CFade* pFade = new CFade;

	if (pFade != nullptr)
	{
		pFade->Init();
		pFade->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
		pFade->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		pFade->m_fAlpha = fAlpha;
		pFade->m_fade = fade;
	}
	return pFade;
}