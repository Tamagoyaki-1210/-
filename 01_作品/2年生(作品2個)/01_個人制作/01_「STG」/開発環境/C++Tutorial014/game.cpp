//=============================================================================
//
// ゲーム
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

bool CGame::m_bPause = true;		//ポーズ使用判定
bool CGame::m_bEndGame = false;		//ゲーム終了判定
CScore *CGame::m_pScore = nullptr;
CStage *CGame::m_pStage = nullptr;

//=====================================
//デフォルトコンストラクタ
//=====================================
CGame::CGame()
{

}

//=====================================
//デストラクタ
//=====================================
CGame::~CGame()
{

}

//=====================================
//初期化処理
//=====================================
void CGame::Init()
{
	m_pStage = new CStage;

	//UI
	m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 50.0f, 0.0f));
	CMenu::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CMenu::TYPE_NONE, CMenu::MODE_GAME);
	m_pStage->Init();

	m_bPause = false;	//ポーズ未使用
}

//=====================================
//終了処理
//=====================================
void CGame::Uninit()
{
	m_pStage->Uninit();
	CObject::ReleaseAll();
	m_bPause = true;	//ポーズ使用
}

//=====================================
//更新処理
//=====================================
void CGame::Update()
{
	m_pStage->Update();

	CFade *pFade = CMode::GetFade();

	//フェードしていない場合
	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		CInput *pInput = CApplication::GetInput();

		//Pでポーズ切り替え
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

		//ゲーム終了時の処理
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
//描画処理
//=====================================
void CGame::Draw()
{
	m_pStage->Draw();
}
