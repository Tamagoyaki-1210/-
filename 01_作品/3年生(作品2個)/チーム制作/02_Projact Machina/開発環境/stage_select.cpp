//==============================================================================================
//
// �X�e�[�W�Z���N�g�@�@�@stage_select.cpp
// tutida ryousei
//
//==============================================================================================
#include"stage_select.h"
#include"input.h"
#include"application.h"
#include"fade.h"
#include"fontString.h"
#include"connect.h"

int CStage_Select::m_nStage_Number = 0;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CStage_Select::CStage_Select()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CStage_Select::~CStage_Select()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CStage_Select::Init()
{
	CFontString::Create({ 150.0f, SCREEN_HEIGHT / 2 - 200, 0.0f }, { 50.0f, 50.0f }, "�����΂�݂����`���[�g���A��");

	// �X�e�[�W�̔ԍ��̏�����
	m_nStage_Number = 0;

	// �X�e�[�W�̈ʒu
	D3DXVECTOR2 Stage_Size = { m_nScreen_Width / 2.5,m_nScreen_Height / 2.5 };
	D3DXVECTOR2 Min_Stage_Size = { m_nScreen_Width / 8,m_nScreen_Height / 8 };

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		// �X�e�[�W�̐ݒ�
		m_StagePos[nCnt] = { (float)m_nScreen_Width / 2 + (m_nScreen_Width * nCnt),m_nScreen_Height / 2 - 100,0.0f };

		m_pStage[0][nCnt] = CObject2D::Create({ m_StagePos[nCnt].x + (m_nScreen_Width * nCnt),m_StagePos[nCnt].y,0.0f }, Stage_Size);
		m_pStage[1][nCnt] = CObject2D::Create({ m_StagePos[nCnt].x + (m_nScreen_Width * nCnt),m_StagePos[nCnt].y,0.0f }, { Stage_Size.x + 30.0f, Stage_Size.y + 30.0f }, CObject::PRIORITY_CENTER);

		//m_pStage[0][nCnt]->SetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STAGE_1 + (CTexture::TEXTURE)nCnt));
		// �X�e�[�W(��)�̐ݒu
		D3DXVECTOR3 StagePos = { m_nScreen_Width / 2 - 450.0f + (300 * nCnt),600.0f,0.0f };
		m_pMin_Stage[0][nCnt] = CObject2D::Create(StagePos, Min_Stage_Size);
		m_pMin_Stage[1][nCnt] = CObject2D::Create(StagePos, { Min_Stage_Size.x + 20.0f, Min_Stage_Size.y + 20.0f }, CObject::PRIORITY_CENTER);

		//m_pMin_Stage[0][nCnt]->SetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STAGE_1 + (CTexture::TEXTURE)nCnt));
	}

	// �w�i
	m_pFloor = CObject2D::Create({ (float)m_nScreen_Width / 2, (float)m_nScreen_Width / 2 ,0.0f }, { (float)m_nScreen_Width, (float)m_nScreen_Width }, CObject::PRIORITY_BACK_GROUND);
	m_pFloor->SetTexture(CTexture::TEXTURE_FLOOR);

	m_bSelect = false;		// �X�e�[�W�����߂���
	m_nStartCount = 30;		// �n�܂�܂ł̃J�E���g
	m_fAlpha = 0.0f;		// �A���t�@�l
	m_Flash = false;		// �_�ł��Ă邩

	// �I���̖��
	Select_Arrow();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CStage_Select::Uninit()
{

}

//==============================================================================================
// �X�V����
//==============================================================================================
void CStage_Select::Update()
{
	// �X�e�[�W�����܂��Ă��Ȃ��ꍇ
	if (m_bSelect == false)
	{
		// �X�e�[�W�I��
		Select();
	}
	else
	{
		// �X�e�[�W�J�n����
		Stage_Start();
	}

	// �X�e�[�W���X���C�h������
	Stage_Slide();

	// �X�e�[�W(��)�̐ݒ�
	Min_Stage();

	// ���̓_��
	Arrow_Flash();

	// ���͏����̎擾
	CInput* pInput = CInput::GetKey();

	if(pInput->Trigger(DIK_BACK) || pInput->Trigger(JOYPAD_B))
		// ��ʑJ��
		CFade::SetFade(CApplication::MODE_CHAR_SELECT, 0.05f);

}

//==============================================================================================
// ��������
//==============================================================================================
CStage_Select* CStage_Select::Create()
{
	CStage_Select* pStage_Select = new CStage_Select;

	if (FAILED(pStage_Select->Init()))
	{
		return nullptr;
	}

	return pStage_Select;
}

//==============================================================================================
// �X�e�[�W�I��
//==============================================================================================
void CStage_Select::Select()
{
	// ���͏����̎擾
	CInput* pInput = CInput::GetKey();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if ((pInput->Trigger(DIK_D)
				|| pInput->Trigger(JOYPAD_RIGHT, nCnt)))
			{
				// �I��SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

				// �X�e�[�W��i�߂�
				m_nStage_Number++;

				if (m_nStage_Number > STAGE_TUTORIAL)
					m_nStage_Number = STAGE_1;

				// �w�i�̕ύX
				//m_pFloor->SetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_FLOOR + (CTexture::TEXTURE)m_nStage_Number));
				break;
			}
			if ((pInput->Trigger(DIK_A)
				|| pInput->Trigger(JOYPAD_LEFT, nCnt)))
			{
				// �I��SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

				// �X�e�[�W��߂�
				m_nStage_Number--;

				if (m_nStage_Number < STAGE_1)
					m_nStage_Number = STAGE_TUTORIAL;

				// �w�i�̕ύX
				//m_pFloor->SetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_FLOOR + (CTexture::TEXTURE)m_nStage_Number));
				break;
			}

			if ((pInput->Trigger(DIK_RETURN) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
				|| pInput->Trigger(JOYPAD_START, nCnt))
				&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				// ����SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

				//// �����_����I�������ꍇ
				//if (m_nStage_Number == STAGE_RANDOM)
				//	m_nStage_Number = rand() % 3;

				// �X�e�[�W�����肵����
				m_bSelect = true;

				for (int nCnt2 = 0; nCnt2 < STAGE_MAX; nCnt2++)
				{
					// �ړI�̈ʒu�̐ݒ�
					m_StagePos[nCnt2].x = (float)(m_nScreen_Width / 2 + (m_nScreen_Width * nCnt2) - m_nScreen_Width * m_nStage_Number);

					for (int nCnt3 = 0; nCnt3 < 2; nCnt3++)
					{
						// �ʒu�̍X�V
						m_pStage[nCnt3][nCnt2]->SetPos(m_StagePos[nCnt2]);
					}
				}
			}

			// �L�����Z���N�g��ʂɖ߂�
			if ((pInput->Trigger(DIK_BACK, nCnt) || pInput->Trigger(JOYPAD_B))
				&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
				// ��ʑJ��
				CFade::SetFade(CApplication::MODE_CHAR_SELECT, 0.05f);
		}
	}
}

//==============================================================================================
// �X�e�[�W�J�n����
//==============================================================================================
void CStage_Select::Stage_Start()
{
	// �n�܂�܂ł̃J�E���g
	m_nStartCount--;

	if (m_nStage_Number != STAGE_TUTORIAL
		&& m_nStartCount <= 0)
	{
		// ��ʑJ��
		CFade::SetFade(CApplication::MODE_GAME, 0.05f);
	}

	else if (m_nStage_Number == STAGE_TUTORIAL
		&& m_nStartCount <= 0)
	{
		// ��ʑJ��
		CFade::SetFade(CApplication::MODE_TUTORIAL, 0.05f);
	}
}

//==============================================================================================
// �X�e�[�W���X���C�h������
//==============================================================================================
void CStage_Select::Stage_Slide()
{
	// �X�e�[�W����O
	if (!m_bSelect)
	{
		for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
		{
			// �ړI�̈ʒu�̐ݒ�
			m_StagePosDest[nCnt].x = (float)(m_nScreen_Width / 2 + (m_nScreen_Width * nCnt) - m_nScreen_Width * m_nStage_Number);

			// �ړ��̑��x����
			m_StagePos[nCnt].x += (m_StagePosDest[nCnt].x - m_StagePos[nCnt].x) * 0.05f;

			for (int nCnt2 = 0; nCnt2 < 2; nCnt2++)
			{
				// �ʒu�̍X�V
				m_pStage[nCnt2][nCnt]->SetPos(m_StagePos[nCnt]);
			}
		}
	}
}

//==============================================================================================
// �I���̖��
//==============================================================================================
void CStage_Select::Select_Arrow()
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_Select_Arrow[nCnt] = { 150.0f + nCnt * 980,m_nScreen_Height / 2 - 100.0f,0.0f };
		m_pSelect_Arrow[nCnt] = CObject2D::Create({ m_Select_Arrow[nCnt] }, { 150.0f,150.0f }, CObject::PRIORITY_SCREEN);
		//m_pSelect_Arrow[nCnt]->SetTexture(CTexture::TEXTURE_SHOP_DIRECTION);
	}

	m_pSelect_Arrow[0]->SetRot({ D3DX_PI / 2 , D3DX_PI / 2 });
	m_pSelect_Arrow[1]->SetRot({ -D3DX_PI / 2 , -D3DX_PI / 2 });
}

//==============================================================================================
// �X�e�[�W(��)�̐ݒ�
//==============================================================================================
void CStage_Select::Min_Stage()
{
	// �_�ł��邩
	if (m_fRed >= 1)
		m_bMin = false;
	if (m_fRed <= 0)
		m_bMin = true;

	// �A���t�@�l
	if (!m_bMin)
		m_fRed -= 0.03f;
	else
		m_fRed += 0.03f;

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		// �X�e�[�W(��)�̐F
		m_pMin_Stage[1][nCnt]->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// �X�e�[�W(��)�̐F
		m_pMin_Stage[1][m_nStage_Number]->SetCol({ 1.0f, m_fRed, m_fRed, 1.0f });
	}
}

//==============================================================================================
// ���̓_��
//==============================================================================================
void CStage_Select::Arrow_Flash()
{
	// �_�ł��邩
	if (m_fAlpha >= 1)
		m_Flash = false;
	if (m_fAlpha <= 0)
		m_Flash = true;

	// �A���t�@�l
	if (!m_Flash)
		m_fAlpha -= 0.03f;
	else
		m_fAlpha += 0.03f;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// ���̐F
		m_pSelect_Arrow[nCnt]->SetCol({ 1.0f, 1.0f, 1.0f, m_fAlpha });
	}
}