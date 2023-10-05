//=============================================================================
//
// �Q�[��
// Author : tanimoto kosuke
//
//=============================================================================
#include "game.h"
#include "score.h"
#include "menu.h"
#include "input.h"
#include "application.h"
#include "mode.h"
#include "fade.h"
#include "object.h"
#include "stage.h"

bool CGame::m_bPause = true;		//�|�[�Y�g�p����
bool CGame::m_bEndGame = false;		//�Q�[���I������
CScore *CGame::m_pScore = nullptr;
CStage *CGame::m_pStage = nullptr;

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CGame::CGame()
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CGame::~CGame()
{

}

//=====================================
//����������
//=====================================
void CGame::Init()
{
	m_pStage = new CStage;

	//UI
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 50.0f, 0.0f));
	CMenu::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CMenu::TYPE_NONE, CMenu::MODE_GAME);
	m_pStage->Init();

	m_bPause = false;	//�|�[�Y���g�p
}

//=====================================
//�I������
//=====================================
void CGame::Uninit()
{
	m_pStage->Uninit();
	CObject::ReleaseAll();
	m_bPause = true;	//�|�[�Y�g�p
}

//=====================================
//�X�V����
//=====================================
void CGame::Update()
{
	m_pStage->Update();

	CFade *pFade = CMode::GetFade();

	//�t�F�[�h���Ă��Ȃ��ꍇ
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		CInput *pInput = CApplication::GetInput();

		//P�Ń|�[�Y�؂�ւ�
		if (pInput->GetKeyboardTrigger(DIK_P))
		{
			if (!m_bPause)
			{
				CSound::PlaySound(SOUND_LABEL_SE_WHISTLE_STOP);
			}
			else
			{
				CSound::PlaySound(SOUND_LABEL_SE_WHISTLE_START);
			}
			m_bPause = !m_bPause;
		}

		//�Q�[���I�����̏���
		if (m_bEndGame == true)
		{
			CMode *pMode = CApplication::GetMode();

			pMode->ChangeMode(CMode::MODE_RESULT);

			m_bEndGame = false;

			m_pScore->Save();
		}
	}
}

//=====================================
//�`�揈��
//=====================================
void CGame::Draw()
{
	m_pStage->Draw();
}
