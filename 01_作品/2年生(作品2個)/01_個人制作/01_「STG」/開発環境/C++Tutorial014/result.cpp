//=============================================================================
//
// ���U���g
// Author : tanimoto kosuke
//
//=============================================================================
#include "result.h"
#include "mode.h"
#include "object.h"
#include "input.h"
#include "fade.h"
#include "application.h"
#include "ranking.h"
#include "bg.h"
#include "stage.h"
#include "bullet.h"

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CResult::CResult()
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CResult::~CResult()
{

}

//=====================================
//����������
//=====================================
void CResult::Init()
{
	CRanking::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 45, 0.0f));
	m_nModeCounter = 0;

	if (CStage::GetClear() == true)
	{
		CBG::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), CBG::TYPE_Clear);
	}
}

//=====================================
//�I������
//=====================================
void CResult::Uninit()
{
	CObject::ReleaseAll();
}

//=====================================
//�X�V����
//=====================================
void CResult::Update()
{
	Input();

	if (m_nModeCounter > 3000)
	{
		CMode *pMode = CApplication::GetMode();

		pMode->ChangeMode(CMode::MODE_TITLE);
	}
	else
	{
		m_nModeCounter++;
	}

	if (CStage::GetClear() == true)
	{
		if (rand() % 100 == 0)
		{
			float nRandWidth = (float)(rand() % SCREEN_WIDTH);
			float nRandHeight = (float)(rand() % SCREEN_HEIGHT);
			CBullet::Create(D3DXVECTOR3(nRandWidth, nRandHeight, 0.0f), D3DXVECTOR3((rand() % 200 - 100) * 0.01f, (rand() % 100) * 0.01f, 0.0f), CBullet::ENEMY_SPREAD);	//�e��������
		}
	}
}

//=====================================
//�`�揈��
//=====================================
void CResult::Draw()
{

}

//=====================================
//���͏���
//=====================================
void CResult::Input()
{
	CFade *pFade = CMode::GetFade();

	if (pFade->GetFade() != CFade::FADE_NONE)
	{// �t�F�[�h���Ă���ꍇ
		return;
	}

	CInput *pInput = CApplication::GetInput();

	if (pInput->GetKeyboardTrigger(DIK_RETURN))
	{
		CSound::PlaySound(SOUND_LABEL_SE_YES);
		CMode *pMode = CApplication::GetMode();

		pMode->ChangeMode(CMode::MODE_TITLE);
	}
}