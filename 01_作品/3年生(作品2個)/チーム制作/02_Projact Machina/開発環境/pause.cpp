//=============================================================================
//
// pause.cpp
// Author : tutida ryousei
//
//=============================================================================
#include"pause.h"
#include"input.h"
#include"object2D.h"
#include"fontString.h"
#include"application.h"
#include"fade.h"
#include"camera.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPause::CPause()
{
	m_bPause = false;
	m_Select_Pause = false;
	m_pPause = nullptr;
	m_StringSize = { 0.0f,0.0f };
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CPause::Init(void)
{
	// �|�[�Y�̔w�i
	if (m_pPause == nullptr)
		m_pPause = CObject2D::Create({ 950.0f,400.0f,0.0f }, { 0.0f,0.0f }, CObject::PRIORITY_SCREEN);

	// �|�[�Y
	m_pPause_Menu[PAUSE_HEADER] = CFontString::Create({ 860.0f, 200.0f, 0.0f }, { m_StringPauseSize }, "�|�[�Y");

	// �I�����j���[�̃T�C�Y
	m_pPause_Pos[PAUSE_CONTINUE] = { 800.0f, 290.0f, 0.0f };
	//m_pPause_Pos[PAUSE_STAGE_SELECT] = { 770.0f, 430.0f, 0.0f };
	m_pPause_Pos[PAUSE_CHAR_SELECT] = { 790.0f, 570.0f, 0.0f };

	// �I�����j���[�̐���
	m_pPause_Menu[PAUSE_CONTINUE] = CFontString::Create(m_pPause_Pos[PAUSE_CONTINUE], { m_StringSize }, "�Q�[�����Â���");
	//m_pPause_Menu[PAUSE_STAGE_SELECT] = CFontString::Create(m_pPause_Pos[PAUSE_STAGE_SELECT], { m_StringSize }, "�X�e�[�W���񂽂���");
	m_pPause_Menu[PAUSE_CHAR_SELECT] = CFontString::Create(m_pPause_Pos[PAUSE_CHAR_SELECT], { m_StringSize }, "�L�������񂽂���");

	// �I���̏����l
	m_nSelect = PAUSE_CONTINUE;

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CPause::Uninit(void)
{
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CPause::Update(void)
{
	if (!CApplication::GetCamera()->GetOpening())
	// �|�[�Y
	Pause();

	// �|�[�Y��
	if (m_bPause)
	{
		// �I������
		Select();

		// �J�[�\���Ƃ̓����蔻��
		StringCollision();
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CPause::Draw(void)
{
}

//==============================================================================================
// �|�[�Y���
//==============================================================================================
void CPause::Pause()
{
	CInput *pInput = CInput::GetKey();

	if (pInput != nullptr)
	{
		// �|�[�Y��
		if ((pInput->Trigger(DIK_P) || pInput->Trigger(JOYPAD_START))
			&& !m_bPause)
		{
			m_pPause->SetSize({ 400.0f,500.0f });
			m_StringSize = { 30.0f,30.0f };
			m_StringPauseSize = { 60.0f,60.0f };
			m_bPause = true;
		}
		// �|�[�Y���ĂȂ�
		else if ((pInput->Trigger(DIK_P) || pInput->Trigger(JOYPAD_START))
			&& m_bPause || m_Select_Pause)
		{
			m_pPause->SetSize({ 0.0f,0.0f });
			m_StringSize = { 0.0f,0.0f };
			m_StringPauseSize = { 0.0f,0.0f };
			m_bPause = false;
			m_Select_Pause = false;
		}

		// �F�̐ݒ�
		if (m_pPause != nullptr)
			m_pPause->SetCol({ 0.0f,0.0f,0.0f,0.5f });

		for (int nCnt = 0; nCnt < PAUSE_MAX; nCnt++)
		{
			// �T�C�Y�̐ݒ�
			m_pPause_Menu[nCnt]->SetSize(m_StringSize);
		}
		m_pPause_Menu[PAUSE_HEADER]->SetSize({ m_StringPauseSize });
	}
}

//==============================================================================================
// �I������
//==============================================================================================
void CPause::Select()
{
	CInput *pInput = CInput::GetKey();
	
	// �I�����̕ύX
	if ((pInput->Trigger(DIK_DOWN) || pInput->Trigger(JOYPAD_DOWN)) 
		&& m_nSelect < PAUSE_CHAR_SELECT)
		m_nSelect++;
	else if ((pInput->Trigger(DIK_UP) || pInput->Trigger(JOYPAD_UP))
		&& m_nSelect > PAUSE_CONTINUE)
		m_nSelect--;

	// �I�����Ă��鍀�ڂ�Z������
	m_pPause_Menu[m_nSelect]->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	for (int nCnt = PAUSE_CONTINUE; nCnt < PAUSE_MAX; nCnt++)
	{
		if (nCnt == m_nSelect)
			continue;

		// �I������Ă��Ȃ����ڂ𔼓����ɂ���
		m_pPause_Menu[nCnt]->SetColor({ 1.0f,1.0f,1.0f,0.5f });
	}

	if ((pInput->Trigger(DIK_RETURN) || pInput->Trigger(JOYPAD_A) || (pInput->Trigger(MOUSE_INPUT_LEFT)) && m_Select_Cursor))
	{
		switch (m_nSelect)
		{
		case PAUSE_CONTINUE:
			m_Select_Pause = true;
			break;
		//case PAUSE_STAGE_SELECT:
		//	// �X�e�[�W�I���ɖ߂�
		//		if(CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
		//		// ��ʑJ��
		//		CFade::SetFade(CApplication::MODE_STAGE_SELECT, 0.05f);
			break;
		case PAUSE_CHAR_SELECT:
			// �E�B���h�E�̔j��
			//DestroyWindow(CApplication::GetWindow());
			// �L�����I���ɖ߂�
			if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
				// ��ʑJ��
				CFade::SetFade(CApplication::MODE_CHAR_SELECT, 0.05f);
			break;
		default:
			break;
		}
	}
}

//==============================================================================================
// �J�[�\���Ƃ̓����蔻��
//==============================================================================================
void CPause::StringCollision()
{
	CInput *pInput = CInput::GetKey();

	// �}�E�X�J�[�\���̈ʒu
	D3DXVECTOR3 MousePos = pInput->GetMouseCursor();

	m_Select_Cursor = false;

	for (int nCnt = PAUSE_CONTINUE; nCnt < PAUSE_MAX; nCnt++)
	{
		if (m_pPause_Pos[nCnt].x + 350.0f > MousePos.x
			&& m_pPause_Pos[nCnt].x - 20.0f< MousePos.x
			&& m_pPause_Pos[nCnt].y + 50.0f > MousePos.y
			&& m_pPause_Pos[nCnt].y - 50.0f < MousePos.y)
		{
			m_nSelect = nCnt;
			m_Select_Cursor = true;
		}
	}
}

//==============================================================================================
// ��������
//==============================================================================================
CPause *CPause::Create()
{
	CPause *pPause = new CPause;

	if (pPause != nullptr)
	{
		pPause->Init();
	}

	return pPause;
}