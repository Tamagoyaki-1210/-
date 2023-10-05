//=============================================================================
//
// menu.cpp
// Author : tanimoto kosuke
//
//=============================================================================
#include "menu.h"
#include "Application.h"
#include "mode.h"
#include "fontString.h"
#include "inputKeyboard.h"
#include "inputPad.h"
#include "game.h"
#include "fade.h"
#include "tutorial.h"

int CMenu::m_nNumAll = 0;
int CMenu::m_nSelectNum = 0;
bool CMenu::m_bResult = false;
CFontString* CMenu::m_pChoice[MaxChoice] = {};
CFontString* CMenu::m_pPause = nullptr;
CObject_2D* CMenu::m_pObj2D = nullptr;

//-----------------------------------------
//プロトタイプ宣言
//-----------------------------------------
void ExitExe(void);		// ウインドウ終了処理

//=====================================
// デフォルトコンストラクタ
//=====================================
CMenu::CMenu()
{

}

//=====================================
// デストラクタ
//=====================================
CMenu::~CMenu()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CMenu::Init(void)
{
	switch (CApplication::GetMode())
	{
	case CApplication::Mode_Title :

		m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 450.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "スタート");
		m_nNumAll++;

		m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "チュートリアル");
		m_nNumAll++;

		m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "おわり");
		m_nNumAll++;

		m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
		break;
	case CApplication::Mode_Game_Race:
		break;
	default:
		break;
	}

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CMenu::Uninit(void)
{
	for (int nCnt = 0; nCnt < MaxChoice; nCnt++)
	{// 選択肢文字列の終了処理
		if (m_pChoice[nCnt] != nullptr)
		{
			m_pChoice[nCnt]->Uninit();
			m_pChoice[nCnt] = nullptr;
		}
	}

	if (m_pPause != nullptr)
	{// ポーズ文字列の終了処理
		m_pPause->Uninit();
		m_pPause = nullptr;
	}

	if (m_pObj2D != nullptr)
	{// オブジェクト2Dの終了処理
		m_pObj2D->Release();
		m_pObj2D = nullptr;
	}

	m_nSelectNum = 0;		// 現在選択されている番号の終了処理
	m_nNumAll = 0;			// 選択肢の全体数の終了処理
	m_bResult = false;		// リザルト判定の終了処理
}

//=====================================
// 更新処理
//=====================================
void CMenu::Update(void)
{
	ModeType();		//モード毎の処理
}

//=====================================
// モード毎の処理
//=====================================
void CMenu::ModeType(void)
{
	switch (CApplication::GetMode())
	{// 取得したモード毎の処理
	case CApplication::Mode_Title:
	{
		for (int nCnt = 0; nCnt < MaxChoice; nCnt++)
		{// 各選択肢の更新処理
			if (m_pChoice[nCnt] != nullptr)
			{
				m_pChoice[nCnt]->Update();
			}
		}
		Input();	// 入力処理
	}
	break;
	case CApplication::Mode_Game_Race:
	{
		if (CGame::GetEndGame() == false)
		{// ゲームが終了していない場合
			if (CApplication::GetPause() == true)
			{// ポーズ中でない場合のみ更新
				for (int nCnt = 0; nCnt < MaxChoice; nCnt++)
				{// 各選択肢の更新処理
					if (m_pChoice[nCnt] != nullptr)
					{
						m_pChoice[nCnt]->Update();
					}

				}
				if (m_pPause != nullptr)
				{// ポーズ文字列の更新処理
					m_pPause->Update();
				}
				Input();	// 入力処理
			}
		}
		else
		{// ゲームが終了している場合
			for (int nCnt = 0; nCnt < MaxChoice; nCnt++)
			{// 各選択肢の更新処理
				if (m_pChoice[nCnt] != nullptr)
				{
					m_pChoice[nCnt]->Update();
				}
			}
			Input();	// 入力処理
		}
	}
	break;
	case CApplication::Mode_Tutorial:
	{
		if (CGame::GetEndGame() == false)
		{// ゲームが終了していない場合
			if (CApplication::GetPause() == true)
			{// ポーズ中でない場合のみ更新
				for (int nCnt = 0; nCnt < MaxChoice; nCnt++)
				{// 各選択肢の更新処理
					if (m_pChoice[nCnt] != nullptr)
					{
						m_pChoice[nCnt]->Update();
					}

				}
				if (m_pPause != nullptr)
				{// ポーズ文字列の更新処理
					m_pPause->Update();
				}
				Input();	// 入力処理
			}
		}
		else
		{// ゲームが終了している場合
			for (int nCnt = 0; nCnt < MaxChoice; nCnt++)
			{// 各選択肢の更新処理
				if (m_pChoice[nCnt] != nullptr)
				{
					m_pChoice[nCnt]->Update();
				}
			}
			Input();	// 入力処理
		}
	}
	break;
	default:
		break;
	}
}

//=====================================
// 入力処理
//=====================================
void CMenu::Input(void)
{
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{// フェード中ではない場合
		switch (CApplication::GetMode())
		{// 取得したモード毎の処理
		case CApplication::Mode_Title:
		{
			if (CInputKeyboard::GetKeyboardTrigger(DIK_W)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 0)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 1)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 2)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 3))
			{// Wキーが押された場合
				m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
				m_nSelectNum--;								// 現在選択されている番号を下げる

				if (m_nSelectNum < 0)
				{// 現在位置が0より下の場合
					m_nSelectNum = m_nNumAll - 1;
				}
				m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
			}
			else if (CInputKeyboard::GetKeyboardTrigger(DIK_S)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 0)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 1)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 2)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 3))
			{// Sキーが押された場合
				m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
				m_nSelectNum++;								// 現在選択されている番号

				if (m_nSelectNum >= m_nNumAll)
				{// 現在位置が最大数より大きい場合
					m_nSelectNum = 0;
				}
				m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
			}
			if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 0)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 1)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 2)
				|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 3))
			{// Enterキーが押された場合
				if (m_nSelectNum == 0)
				{// 選択肢が0番目の場合
					CApplication::SetMode(CApplication::Mode_PlayerSelect);
				}
				else if (m_nSelectNum == 1)
				{// 選択肢が1番目の場合
					CApplication::SetStageSelect(3);
					CApplication::SetMode(CApplication::Mode_Tutorial);
				}
				else if (m_nSelectNum == 2)
				{// 選択肢が2番目の場合
					ExitExe();
				}
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);
			}
		}
		break;
		case CApplication::Mode_Game_Race:
		{
			if (CGame::GetEndGame() == false)
			{// ゲーム中の場合
				if (CInputKeyboard::GetKeyboardTrigger(DIK_W) || CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 0))
				{// Wキーが押された場合
					m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
					m_nSelectNum--;								// 現在選択されている番号を下げる

					// 現在位置が0より下の場合
					if (m_nSelectNum < 0)
					{
						m_nSelectNum = m_nNumAll - 1;
					}
					m_pChoice[m_nSelectNum]->SetSellect();	// 現在選択肢の設定処理
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
				}
				else if (CInputKeyboard::GetKeyboardTrigger(DIK_S) || CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 0))
				{// Sキーが押された場合
					m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
					m_nSelectNum++;								// 現在選択されている番号を上げる

					// 現在位置が最大数より大きい場合
					if (m_nSelectNum >= m_nNumAll)
					{
						m_nSelectNum = 0;
					}
					m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
				}
				if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) || CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 0))
				{// Enterキーが押された場合
					if (m_nSelectNum == 0)
					{// 選択肢が0番目の場合
						CApplication::SetPause(false);
						PauseChange(false);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_START);
					}
					else if (m_nSelectNum == 1)
					{// 選択肢が1番目の場合
						CApplication::SetMode(CApplication::Mode_Game_Race);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_FINISH);
					}
					else if (m_nSelectNum == 2)
					{// 選択肢が2番目の場合
						CApplication::SetMode(CApplication::Mode_PlayerSelect);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_FINISH);
					}
					else if (m_nSelectNum == 3)
					{// 選択肢が3番目の場合
						CApplication::SetMode(CApplication::Mode_Title);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_FINISH);
					}
				}
			}
			else if (m_bResult)
			{// ゲームが終了している & リザルト中の場合
				if (CInputKeyboard::GetKeyboardTrigger(DIK_W)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 0)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 1)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 2)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 3))
				{// Wキーが押された場合
					m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
					m_nSelectNum--;								// 現在選択されている番号を下げる

					if (m_nSelectNum < 0)
					{// 現在位置が0より下の場合
						m_nSelectNum = m_nNumAll - 1;
					}
					m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
				}
				else if (CInputKeyboard::GetKeyboardTrigger(DIK_S)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 0)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 1)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 2)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 3))
				{// Sキーが押された場合
					m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
					m_nSelectNum++;								// 現在選択されている番号を上げる

					if (m_nSelectNum >= m_nNumAll)
					{// 現在位置が最大数より大きい場合
						m_nSelectNum = 0;
					}
					m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
				}
				if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 0)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 1)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 2)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 3))
				{// Enterキーが押された場合
					if (m_nSelectNum == 0)
					{// 選択肢が0番目の場合
						CApplication::SetMode(CApplication::Mode_Game_Race);
					}
					else if (m_nSelectNum == 1)
					{// 選択肢が1番目の場合
						CApplication::GetSound()->Stop();
						CApplication::SetMode(CApplication::Mode_StageSelect);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_SELECT);
					}
					if (m_nSelectNum == 2)
					{// 選択肢が2番目の場合
						CApplication::SetMode(CApplication::Mode_PlayerSelect);
					}
					else if (m_nSelectNum == 3)
					{// 選択肢が3番目の場合
						CApplication::SetMode(CApplication::Mode_Title);
					}
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);
				}
			}
		}
		break;
		case CApplication::Mode_Tutorial:
		{
			if (CGame::GetEndGame() == false)
			{// ゲーム中の場合
				if (CInputKeyboard::GetKeyboardTrigger(DIK_W) || CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 0))
				{// Wキーが押された場合
					m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
					m_nSelectNum--;								// 現在選択されている番号を下げる

					if (m_nSelectNum < 0)
					{// 現在位置が0より下の場合
						m_nSelectNum = m_nNumAll - 1;
					}
					m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
				}
				else if (CInputKeyboard::GetKeyboardTrigger(DIK_S) || CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 0))
				{// Sキーが押された場合
					m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
					m_nSelectNum++;

					if (m_nSelectNum >= m_nNumAll)
					{// 現在位置が最大数より大きい場合
						m_nSelectNum = 0;
					}
					m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
				}
				if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN) || CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 0))
				{// Enterキーが押された場合
					if (m_nSelectNum == 0)
					{// 選択肢が0番目の場合
						CApplication::SetPause(false);
						PauseChange(false);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_START);
					}
					else if (m_nSelectNum == 1)
					{// 選択肢が1番目の場合
						CApplication::SetMode(CApplication::Mode_Tutorial);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_FINISH);
					}
					else if (m_nSelectNum == 2)
					{// 選択肢が2番目の場合
						CApplication::SetMode(CApplication::Mode_PlayerSelect);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_FINISH);
					}
					else if (m_nSelectNum == 3)
					{// 選択肢が3番目の場合
						CApplication::SetMode(CApplication::Mode_Title);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_FINISH);
					}
				}
			}
			else if (m_bResult)
			{// ゲームが終了している & リザルト中の場合
				if (CInputKeyboard::GetKeyboardTrigger(DIK_W)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 0)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 1)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 2)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_UP, 3))
				{// Wキーが押された場合
					m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
					m_nSelectNum--;								// 現在選択されている番号を下げる

					if (m_nSelectNum < 0)
					{// 現在位置が0より下の場合
						m_nSelectNum = m_nNumAll - 1;
					}
					m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
				}
				else if (CInputKeyboard::GetKeyboardTrigger(DIK_S)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 0)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 1)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 2)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_DOWN, 3))
				{// Sキーが押された場合
					m_pChoice[m_nSelectNum]->SizeReset();		// 文字の大きさ初期化処理
					m_nSelectNum++;								// 現在選択されている番号を上げる

					if (m_nSelectNum >= m_nNumAll)
					{// 現在位置が最大数より大きい場合
						m_nSelectNum = 0;
					}
					m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
				}
				if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 0)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 1)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 2)
					|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 3))
				{// Enterキーが押された場合
					if (m_nSelectNum == 0)
					{// 選択肢が0番目の場合
						CApplication::SetMode(CApplication::Mode_Tutorial);
					}
					else if (m_nSelectNum == 1)
					{// 選択肢が1番目の場合
						CApplication::GetSound()->Stop();
						CApplication::SetMode(CApplication::Mode_StageSelect);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_SELECT);
					}
					else if (m_nSelectNum == 2)
					{// 選択肢が2番目の場合
						CApplication::SetMode(CApplication::Mode_PlayerSelect);
					}
					else if (m_nSelectNum == 3)
					{// 選択肢が3番目の場合
						CApplication::SetMode(CApplication::Mode_Title);
					}
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);
				}
			}
		}
		break;
		default:
			break;
		}
	}
}

//=====================================
// ポーズ選択肢処理
//=====================================
void CMenu::PauseChange(bool bPause)
{
	if (bPause == true)
	{// ポーズ中の場合
		m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "つづける");
		m_nNumAll++;

		m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "リトライ");
		m_nNumAll++;

		m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 500.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "キャラセレクト");
		m_nNumAll++;

		m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "タイトルにもどる");
		m_nNumAll++;

		m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理

		m_pPause = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 150.0f, 0.0f), D3DXVECTOR2(100.0f, 100.0f), "ポーズ");

		// オブジェクト2D生成
		m_pObj2D = CObject_2D::Create();
		m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		m_pObj2D->SetSize(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		m_pObj2D->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.1f));
		m_pObj2D->SetTexture(CObject::TEXTURE_NULL);
		m_pObj2D->SetPriority(4);
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_STOP);
	}
	else
	{// ポーズ中ではない場合
		for (int nCnt = 0; nCnt < MaxChoice; nCnt++)
		{
			if (m_pChoice[nCnt] != nullptr)
			{// 選択肢文字列使用中の場合初期化
				m_pChoice[nCnt]->Uninit();
				m_pChoice[nCnt] = nullptr;
			}
		}

		if (m_pPause != nullptr)
		{// ポーズ文字列使用中の場合初期化
			m_pPause->Uninit();
			m_pPause = nullptr;
		}

		if (m_pObj2D != nullptr)
		{// オブジェクト2D使用中の場合初期化
			m_pObj2D->Release();
			m_pObj2D = nullptr;
		}

		m_nSelectNum = 0;	// 現在選択されている番号の初期化
		m_nNumAll = 0;		// 選択肢の全体数の初期化
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_START);
	}
}

//=====================================
// リザルトの設定処理
//=====================================
void CMenu::SetResult(void)
{
	m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 250.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "リトライ");
	m_nNumAll++;

	m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "ステージセレクト");
	m_nNumAll++;

	m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 450.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "キャラセレクト");
	m_nNumAll++;

	m_pChoice[m_nNumAll] = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f, 0.0f), D3DXVECTOR2(40.0f, 40.0f), "タイトルにもどる");
	m_nNumAll++;

	m_pChoice[m_nSelectNum]->SetSellect();		// 現在選択肢の設定処理

	m_bResult = true;		// リザルト中にする
}

//=====================================
// 生成処理
//=====================================
CMenu* CMenu::Create(void)
{
	CMenu* pMenu = new CMenu;

	if (FAILED(pMenu->Init()))
	{
		return nullptr;
	}

	return pMenu;
}