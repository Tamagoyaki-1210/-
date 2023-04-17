//=============================================================================
//
// stageSelect.cpp
// Author : tanimoto kosuke
//
//=============================================================================
#include "stageSelect.h"
#include "Application.h"
#include "inputKeyboard.h"
#include "inputPad.h"
#include "fade.h"
#include <random>
#include <iostream>//用いるヘッダファイルが変わります。

CFontString* CStageSelect::m_pStr = nullptr;
CObject_2D* CStageSelect::m_pFrame = nullptr;
//=====================================
// デフォルトコンストラクタ
//=====================================
CStageSelect::CStageSelect()
{

}

//=====================================
// デストラクタ
//=====================================
CStageSelect::~CStageSelect()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CStageSelect::Init(void)
{
	m_pStr = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f), "ステージセレクト");

	// ステージセレクト背景の生成
	CObject_2D* pObj = CObject_2D::Create();
	pObj->SetPos(D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f));
	pObj->SetSize(D3DXVECTOR2(6400.0, (float)SCREEN_HEIGHT * 0.5f));
	pObj->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pObj->SetTexture(CObject::TEXTURE_STAGE_SELECT_BG);
	pObj->MoveTexCoordinates(D3DXVECTOR2(0.00002f, 0.0f));
	pObj->SetPriority(3);

	if (m_pFrame == nullptr)
	{// 選択枠が未使用の場合生成
		m_pFrame = CObject_2D::Create();
		m_pFrame->SetPos(D3DXVECTOR3((SCREEN_WIDTH / 5), SCREEN_HEIGHT / 2, 0.0f));
		m_pFrame->SetSize(D3DXVECTOR2(125, 125));
		m_pFrame->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFrame->SetTexture(CObject::TEXTURE_SELECT_FRAME);
		m_pFrame->SetPriority(4);
	}

	m_nNumAll = 4;	// 選択肢の全体数

	for(int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		CObject_2D *pObj2D = CObject_2D::Create();
		
		// 文字を中央に配置し、string型から*char型を1文字ずつ取り出す
		pObj2D->SetPos(D3DXVECTOR3(((float)SCREEN_WIDTH * 0.2f) * (float)(nCnt + 1), (float)SCREEN_HEIGHT * 0.5f, 0.0f));
		pObj2D->SetSize(D3DXVECTOR2(120, 120));
		pObj2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		pObj2D->SetPriority(4);

		// 番号毎にテクスチャを貼り付ける
		if (nCnt == 0)
		{
			pObj2D->SetTexture(CObject::TEXTURE_STAGESEL01);
		}
		else if (nCnt == 1)
		{
			pObj2D->SetTexture(CObject::TEXTURE_STAGESEL02);
		}
		else if (nCnt == 2)
		{
			pObj2D->SetTexture(CObject::TEXTURE_STAGESEL03);
		}
		else if (nCnt == 3)
		{
			pObj2D->SetTexture(CObject::TEXTURE_STAGESELR);
		}
	}

	m_nSelectNum = 0;	// 現在選択されている選択肢の初期化

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CStageSelect::Uninit(void)
{
	if (m_pStr != nullptr)
	{// 文字列の終了処理
		m_pStr->Uninit();
		m_pStr = nullptr;
	}

	if (m_pFrame != nullptr)
	{// 選択枠の終了処理
		m_pFrame->Release();
		m_pFrame = nullptr;
	}
	m_nSelectNum = 0;
}

//=====================================
// 更新処理
//=====================================
void CStageSelect::Update(void)
{
	if (m_pStr != nullptr)
	{// 文字列の更新処理
		m_pStr->Update();
	}

	if (m_pFrame != nullptr)
	{// 選択枠の更新処理
		m_pFrame->Update();
	}
	Input();	// 入力処理
}

//=====================================
// 入力処理
//=====================================
void CStageSelect::Input(void)
{
	CSound *pSound = CApplication::GetSound();

	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{// フェード中ではない場合
		if (CInputKeyboard::GetKeyboardTrigger(DIK_A)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_LEFT, 0)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_LEFT, 1)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_LEFT, 2)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_LEFT, 3))
		{// Wキーが押された場合
			m_nSelectNum--;

			if (m_nSelectNum < 0)
			{// 現在位置が0より下の場合
				m_nSelectNum = m_nNumAll - 1;
			}
			m_pFrame->SetPos(D3DXVECTOR3(((float)SCREEN_WIDTH * 0.2f) * (float)(m_nSelectNum + 1), (float)SCREEN_HEIGHT * 0.5f, 0.0f));
			pSound->Play(CSound::SOUND_LABEL_SE_SELECT);
		}
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_D)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_RIGHT, 0)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_RIGHT, 1)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_RIGHT, 2)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_RIGHT, 3))
		{// Sキーが押された場合
			m_nSelectNum++;

			if (m_nSelectNum >= m_nNumAll)
			{// 現在位置が最大数より大きい場合
				m_nSelectNum = 0;
			}
			m_pFrame->SetPos(D3DXVECTOR3(((float)SCREEN_WIDTH * 0.2f) * (float)(m_nSelectNum + 1), (float)SCREEN_HEIGHT * 0.5f, 0.0f));
			pSound->Play(CSound::SOUND_LABEL_SE_SELECT);
		}
		if (CInputKeyboard::GetKeyboardTrigger(DIK_BACK)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_BACK, 0)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_BACK, 1)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_BACK, 2)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_BACK, 3))
		{// BACKSPACEキーを押したら
			CApplication::SetMode(CApplication::Mode_PlayerSelect);
			pSound->Play(CSound::SOUND_LABEL_SE_NO);
		}
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 0)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 1)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 2)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, 3)
			
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_START, 0)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_START, 1)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_START, 2)
			|| CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_START, 3))
		{// Enterキーを押したら

			if (m_nSelectNum == m_nNumAll - 1)
			{// ランダムが選ばれた場合
				std::random_device rnd;     // 非決定的な乱数生成器を生成
				std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
				std::uniform_int_distribution<> rand100(0, m_nNumAll - 2);   // [0, 99] 範囲の一様乱数
				m_nSelectNum = rand100(mt);
			}

			CApplication::SetStageSelect(m_nSelectNum);				// 選択番号を返す処理
			CApplication::SetMode(CApplication::Mode_Game_Race);	// ゲーム_競争モードに変更
			pSound->Play(CSound::SOUND_LABEL_SE_YES_STAGE);
		}
	}
}

//=====================================
// 生成処理
//=====================================
CStageSelect* CStageSelect::Create(void)
{
	CStageSelect* pStaeSelect = new CStageSelect;

	if (FAILED(pStaeSelect->Init()))
	{
		return nullptr;
	}

	return pStaeSelect;
}