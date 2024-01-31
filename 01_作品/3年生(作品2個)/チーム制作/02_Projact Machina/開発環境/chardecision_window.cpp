//============================================================================
//
// chardecision_window.cpp (�L��������E�B���h�E.cpp)
// Author : Saito Shian
//
//============================================================================

//============================================================================
// �C���N���[�h
//============================================================================
#include "chardecision_window.h"
#include "application.h"
#include "renderer.h"
#include "object2D.h"
#include "menu.h"
#include "fade.h"
#include "input.h"
#include "game.h"
#include "title.h"
#include "menu_window.h"
#include "title_menu.h"
#include "texture.h"
#include "confirmation_window.h"
#include "char_select.h"
#include "connect.h"
#include "number.h"
#include "player.h"
#include "weapon.h"
#include "player_parameter.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CConfirmation_Window* CCharDecision_Window::m_pConfirmation = nullptr;
CPlayer_Parameter* CCharDecision_Window::m_pPlayer_Parameter = nullptr;
const float CCharDecision_Window::BLINK_SPEED = 0.02f;	// �I�����̓_�ő��x
const float CCharDecision_Window::MINI_ALPHA = 0.5f;	// �I�����̍Œᓧ���l

//============================================================================
// �R���X�g���N�^
//============================================================================
CCharDecision_Window::CCharDecision_Window()
{
}

//============================================================================
// �f�X�g���N�^
//============================================================================
CCharDecision_Window::~CCharDecision_Window()
{
}

//============================================================================
// ����������
//============================================================================
HRESULT CCharDecision_Window::Init()
{
	//==================================================
	// �����o�ϐ��̏�����
	//================================================== 
	m_nSelectIndex = 0;
	m_fMoveX = 175.0f;
	m_bMaxSize = false;
	m_bSlideFlag = false;
	m_bFontFlag = false;
	m_bTextureFlag = false;
	m_bLeftRight = false;
	m_bStopFlag = true;
	m_bPosDest = false;
	//m_bExplanationUninit = false;
	m_bScale = false;
	m_bDicision = false;
	m_bUninitFlag = false;
	m_bScaleReduce = false;
	m_bScaleExpansion = false;
	m_bSpawnWindow = true;
	m_pWindow = CObject2D::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), D3DXVECTOR2(0.0f, 0.0f), CObject::PRIORITY_SCREEN);
	m_pWindow->SetCol(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_Color.a));

	m_pPlayer_Parameter = new CPlayer_Parameter;
	m_pPlayer_Parameter->Init();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CCharDecision_Window::Uninit()
{
	//�����o�ϐ��̏�����
	if (m_pWindow != nullptr)
	{
		m_pWindow->Uninit();
		m_pWindow = nullptr;
	}

	// �I��ԍ��̔j��
	if (m_pSelect_Number != nullptr)
	{
		m_pSelect_Number->Uninit();
		m_pSelect_Number = nullptr;
	}

	if (m_pPlayer_Parameter != nullptr)
	{
		m_pPlayer_Parameter->Uninit();
		delete m_pPlayer_Parameter;
		m_pPlayer_Parameter = nullptr;
	}
}

//============================================================================
// �X�V����
//============================================================================
void CCharDecision_Window::Update()
{
	// ���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();

	if (m_bScale == false)
	{// �E�B���h�E�̊g�又��
		CharDecisionMenuScale();
	}

	if (m_bScaleReduce == true)
	{
		// �t�H���g�ƃe�N�X�`���̍폜
		UninitExplanation();
		CharDecisionMenuScaleReduce();
	}

	if (m_pWindow != nullptr && m_bScaleReduce == false)
	{
		// �L�����̕ύX����
		CharSelectChoice();
	}

	if (m_bScaleExpansion == true)
	{
		m_pConfirmation->ConfirmatiomnMenuScale();
		m_pConfirmation->VariableInit();
	}

	if (m_bMaxSize == true && m_bScale == true)
	{
		// �v���C���[�ԍ�����
		PlayerIndex();
	}

	// �Q�[����ʂւ̑J��
	if ((pInput->Trigger(DIK_RETURN) || pInput->Trigger(JOYPAD_A) || pInput->Trigger(JOYPAD_B))
		&& m_bMaxSize == true)
	{
		m_bScaleReduce = true;

		// �v���C���[�ԍ��̐ݒ�
		CApplication::SetPlayerJobIndex(m_nSelectIndex, CPlayer::PARTS_BODY);
		CApplication::SetPlayerJobIndex(-1, CPlayer::PARTS_ARMS);
		CApplication::SetPlayerJobIndex(-1, CPlayer::PARTS_LEG);

		// �f��̐ݒ�
		CApplication::SetPlayerWeaponIndex(CWeapon::WEAPON_KNUCKLE);
	}
}

//============================================================================
// �E�B���h�E�̊g�又��
//============================================================================
void CCharDecision_Window::CharDecisionMenuScale()
{
	if (m_pWindow != nullptr)
	{// null�`�F�b�N

		// �T�C�Y�̊g��
		m_size.x += SizeXScaleSpeed;
		m_size.y += SizeYScaleSpeed;

		if (m_size.x >= m_MaxSize.x)
		{// X�T�C�Y�̍ő�
			m_size.x = m_MaxSize.x;
		}
		if (m_size.y >= m_MaxSize.y)
		{// Y�T�C�Y�̍ő�
			m_size.y = m_MaxSize.y;
		}

		// �E�B���h�E���ő�l�܂ōs������
		if (m_size.x >= m_MaxSize.x && m_size.y >= m_MaxSize.y && m_bMaxSize == false)
		{
			m_bMaxSize = true;		// �ő�T�C�Y�ɓ��B
			m_bScale = true;		// �g�傳�ꂽ

			// �I��ԍ��̐���
			m_pSelect_Number = CNumber::Create({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f}, 80.0f, 100.0f, CObject::PRIORITY_SCREEN);
			m_pSelect_Number->AnimTexture(m_nSelectIndex + 1, 10);
			return;
		}

		// �T�C�Y�̐ݒ�
		m_pWindow->SetSize(D3DXVECTOR2(m_size.x, m_size.y));
	}
}

//=============================================================================
// ���j���[�̃T�C�Y�k������
//=============================================================================
bool CCharDecision_Window::CharDecisionMenuScaleReduce()
{
	if (m_pWindow != nullptr)
	{// null�`�F�b�N

		//m_bExplanationUninit = true;		// �e�N�X�`���ƃt�H���g�̍폜

		// �T�C�Y�̏k��
		m_size.x -= SizeXScaleSpeed;
		m_size.y -= SizeYScaleSpeed;

		if (m_size.x <= 0.0f)
		{// X�T�C�Y�̍ŏ�
			m_size.x = 0.0f;
		}
		if (m_size.y <= 0.0f)
		{// Y�T�C�Y�̍ŏ�
			m_size.y = 0.0f;
		}

		// �E�B���h�E���ŏ��l�܂ōs������
		if (m_size.x <= 0.0f && m_size.y <= 0.0f)
		{
			// ��ʑJ��
			CFade::SetFade(CApplication::MODE_GAME, 0.1f);
			m_bSpawnWindow = false;		// �E�B���h�E�𐶐�����Ă��Ȃ���Ԃɂ���
			m_bUninitFlag = true;		// �폜�t���O��true
			m_bScaleReduce = false;

			// �I������
			Uninit();					

			return true;
		}

		// �T�C�Y�̐ݒ�
		m_pWindow->SetSize(D3DXVECTOR2(m_size.x, m_size.y));
	}
	return false;
}

//=============================================================================
// �I�����̏���
//=============================================================================w
void CCharDecision_Window::CharSelectChoice()
{
	// ���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();

	// �t�F�[�h���ł͖����ꍇ && �\�����̏ꍇ
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		// �I�����ꂽ���ǂ���
		bool bSelect = false;

		// ���Ɉړ�����
		if ((pInput->Trigger(DIK_A) || pInput->Trigger(JOYPAD_LEFT)) && m_bStopFlag == true
			|| (pInput->Trigger(JOYPAD_UP)) && m_bStopFlag == false)
		{
			// �I�������ԍ��̎擾
			m_nSelectIndex--;		// �ԍ���1�߂�
			bSelect = true;
			m_bLeftRight = false;	// ���Ɉړ�
		}
		// �E�Ɉړ�����
		else if ((pInput->Trigger(DIK_D) || pInput->Trigger(JOYPAD_RIGHT) )&& m_bStopFlag == true
			|| (pInput->Trigger(JOYPAD_DOWN)) && m_bStopFlag == false)
		{
			m_nSelectIndex++;		// �ԍ���1�i�߂�
			bSelect = true;
			m_bLeftRight = true;	// �E�Ɉړ�
		}

		// �I�����ꂽ�ꍇ
		if (bSelect == true)
		{
			m_bSlideFlag = true;	// �X���C�h������
			m_bPosDest = false;		// �ړI�̈ʒu�ɓ��B���Ă��Ȃ�
			m_bStopFlag = false;	// �����Ă�����
									// �I��SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
		}
	}
	
	// �E�B���h�E�X���C�h����
	SlideWindow();
}

//============================================================================
// �E�B���h�E��߂�����
//============================================================================
void CCharDecision_Window::BackWindow()
{
	// ���̓f�o�C�X�̏��
	CInput* pInput = CInput::GetKey();
	
	// �O�̃E�B���h�E�ɖ߂�
	if (pInput->Trigger(DIK_BACK))
	{
		m_bDicision = true;
		m_bScale = true;
	}
	if (m_bScale == true && m_bDicision == true)
	{
		CharDecisionMenuScaleReduce();
	}
}

//============================================================================
// �E�B���h�E���X���C�h�����鏈��
//============================================================================
void CCharDecision_Window::SlideWindow()
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos = m_pWindow->GetPos();

	if (m_bSlideFlag == true)
	{// �X���C�h����Ă��鎞

	 // ���ړ�
		if (m_bLeftRight == false)
		{
			//m_bExplanationUninit = true;		// �e�N�X�`���ƃt�H���g�̍폜
			UninitExplanation();				// �t�H���g�̍폜
 			pos.x += m_fMoveX;					// �ړ��ʂ̌��Z

			if (pos.x >= 1920.0f && m_bPosDest == false)
			{// �ʒu��1920.0f�ȏ� && �ړI�̈ʒu�܂œ��B���Ă��Ȃ�������

				pos.x = -SCREEN_WIDTH / 2;		// �ʒu�̐ݒ�
				m_bPosDest = true;				// �ړI�̈ʒu�܂œ��B����
			}
			if (pos.x >= SCREEN_WIDTH / 2 && m_bPosDest == true)
			{// �ʒu��640�ȉ� && �ړI�̈ʒu�܂œ��B������

				pos.x = SCREEN_WIDTH / 2;		// �ʒu�̐ݒ�
				m_bFontFlag = false;			// �t�H���g���g�p���Ă��Ȃ���Ԃɂ���
				m_bTextureFlag = false;
				m_bStopFlag = true;				// �X���C�h���~������
				//m_bExplanationUninit = false;   // �e�N�X�`���ƃt�H���g�̕\��
			}
		}
		// �E�ړ�
		else
		{
			//m_bExplanationUninit = true;		// �e�N�X�`���ƃt�H���g�̍폜
			UninitExplanation();				// �t�H���g�̍폜
			pos.x -= m_fMoveX;					// �ړ��ʂ̉��Z
			if (pos.x <= -SCREEN_WIDTH / 2 && m_bPosDest == false)
			{// �ʒu��-640�ȉ� && �ړI�̈ʒu�܂œ��B���Ă��Ȃ�������

				pos.x = 1920.0f;				// �ʒu�̐ݒ�
				m_bPosDest = true;				// �ړI�̈ʒu�܂œ��B����
			}
			if (pos.x <= SCREEN_WIDTH / 2 && m_bPosDest == true)
			{// �ʒu��640�ȉ� && �ړI�̈ʒu�܂œ��B������

				pos.x = SCREEN_WIDTH / 2;		// �ʒu�̐ݒ�
				m_bFontFlag = false;			// �t�H���g���g�p���Ă��Ȃ���Ԃɂ���
				m_bTextureFlag = false;
				m_bStopFlag = true;				// �X���C�h���~������
				//m_bExplanationUninit = false;	// �e�N�X�`���ƃt�H���g�̕\��
			}
		}
		// �ʒu�̐ݒ�
		m_pWindow->SetPos(pos);
	}
}

//============================================================================
// �v���C���[�ԍ�����
//============================================================================
void CCharDecision_Window::PlayerIndex()
{
	if (m_nSelectIndex <= -1)
	{
		m_nSelectIndex = 3;
	}
	else if (m_nSelectIndex >= 4)
	{
		m_nSelectIndex = 0;
	}

	if (m_nSelectIndex == 0)
	{
		SetFont("���b�V��");
		SetTextue(CTexture::TEXTURE_SKILL_RUSH);
	}
	else if (m_nSelectIndex == 1)
	{
		SetFont("���@���K�[�h");
		SetTextue(CTexture::TEXTURE_SKILL_VANGUARD);
	}
	else if (m_nSelectIndex == 2)
	{
		SetFont("�C�[�O���A�C");
		SetTextue(CTexture::TEXTURE_SKILL_EAGLEEYE);
	}
	else if (m_nSelectIndex == 3)
	{
		SetFont("�}�[�V����");
		SetTextue(CTexture::TEXTURE_SKILL_MARSHALL);
	}

	// �I��ԍ��̍X�V
	if (m_pSelect_Number != nullptr)
	{
		m_pSelect_Number->AnimTexture(m_nSelectIndex + 1, 10);
	}
}

//============================================================================
// �t�H���g�̐ݒ菈��
//============================================================================
void CCharDecision_Window::SetFont(const std::string lette)
{
	if (m_pFont == nullptr && m_bFontFlag == false)
	{
		m_pFont = CFontString::Create(D3DXVECTOR3(650.0f, 250.0f, 0.0f), { 35.0f, 35.0f }, lette);
	}
	m_bFontFlag = true;
}

//============================================================================
// �e�N�X�`���̐ݒ菈��
//============================================================================
void CCharDecision_Window::SetTextue(CTexture::TEXTURE texture)
{
	if (m_bTextureFlag == false)
	{
		if (m_pObject2D == nullptr)
		{
			m_pObject2D = CObject2D::Create(D3DXVECTOR3(500.0f, 200.0f, 0.0f), D3DXVECTOR2(175.0f, 175.0f), CObject::PRIORITY_SCREEN);
			m_pObject2D->SetTexture(texture);
		}

		// �p�����[�^�[�̃e�L�X�g�ݒ�
		SetParameterText();

		m_bTextureFlag = true;
	}
}

//============================================================================
// �p�����[�^�[�̃e�L�X�g�ݒ�
//============================================================================
void CCharDecision_Window::SetParameterText()
{
	std::string str = {};

	str = "<c=0,255,255,255,>���C�t</>:";
	std::stringstream strNum;
	strNum << m_pPlayer_Parameter->GetParameterJob(m_nSelectIndex + 1).nLife;
	str += strNum.str().c_str();

	if (m_pParameter_Font[0] == nullptr)
	{
		m_pParameter_Font[0] = CFontString::Create(D3DXVECTOR3(370.0f, 400.0f, 0.0f), { 50.0f, 50.0f }, str);
	}

	str = "<c=255,124,0,255,>�X�^�~�i</>:";
	std::stringstream strNum2;
	strNum2 << m_pPlayer_Parameter->GetParameterJob(m_nSelectIndex + 1).nStamina;
	str += strNum2.str().c_str();
	if (m_pParameter_Font[1] == nullptr)
	{
		m_pParameter_Font[1] = CFontString::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), { 50.0f, 50.0f }, str);
	}
}

//============================================================================
// �����p�e�N�X�`���A�t�H���g�̔j������
//============================================================================
void CCharDecision_Window::UninitExplanation()
{
	// �t�H���g�̔j��
	if (m_pFont != nullptr)
	{
		m_pFont->Uninit();
		m_pFont = nullptr;
	}

	// �t�H���g�̔j��
	if (m_pParameter_Font[0] != nullptr)
	{
		m_pParameter_Font[0]->Uninit();
		m_pParameter_Font[0] = nullptr;
	}

	// �t�H���g�̔j��
	if (m_pParameter_Font[1] != nullptr)
	{
		m_pParameter_Font[1]->Uninit();
		m_pParameter_Font[1] = nullptr;
	}

	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_pObject2D != nullptr)
		{
			m_pObject2D->Uninit();
			m_pObject2D = nullptr;
		}
	}
}

//============================================================================
// ��������
//============================================================================
CCharDecision_Window * CCharDecision_Window::Create(D3DXVECTOR3 pos, float xsize, float ysize, D3DXCOLOR col)
{
	//�N���X�̐���
	CCharDecision_Window* pCharDecision = new CCharDecision_Window; 

	//null�`�F�b�N
	if (pCharDecision != nullptr)
	{
		// �ݒ菈��
		pCharDecision->m_pos = pos;
		pCharDecision->m_Color = col;
		pCharDecision->m_MaxSize.x = xsize;
		pCharDecision->m_MaxSize.y = ysize;
		// ����������
		pCharDecision->Init();         
	}
	else
	{
		assert(false);
	}

	return pCharDecision;
}