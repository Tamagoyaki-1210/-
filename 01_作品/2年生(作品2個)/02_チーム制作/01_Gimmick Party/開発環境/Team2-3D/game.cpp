//=============================================================================
//
// game.cpp
// Author : tanimoto kosuke
//
//=============================================================================
#include "game.h"
#include "inputKeyboard.h"
#include "inputPad.h"
#include "application.h"
#include "menu.h"
#include "fade.h"

bool CGame::m_bEndGame = false;		//ゲーム終了判定

//=====================================
// デフォルトコンストラクタ
//=====================================
CGame::CGame()
{

}

//=====================================
// デストラクタ
//=====================================
CGame::~CGame()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CGame::Init(void)
{
	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CGame::Uninit(void)
{
	m_bEndGame = false;	// ゲーム終了判定の初期化
}

//=====================================
// 更新処理
//=====================================
void CGame::Update(void)
{
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		//ゲーム中の処理
		if (m_bEndGame == false)
		{
			//Pでポーズ切り替え
			if (CInputKeyboard::GetKeyboardTrigger(DIK_P)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_START, 0)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_START, 1)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_START, 2)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_START, 3))
			{
				bool bPause = !CApplication::GetPause();	// ポーズの切り替え処理
				CApplication::SetPause(bPause);				// ポーズの設定
				CMenu::PauseChange(bPause);					// ポーズの変更処理
			}
		}
	}
}

//=====================================
// 更新処理
//=====================================
void CGame::SetEndGame(void)
{
	m_bEndGame = true;
}