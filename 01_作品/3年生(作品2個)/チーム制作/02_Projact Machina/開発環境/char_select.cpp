//==============================================================================================
//
// キャラクターセレクト　　　char_select.cpp
// tutida ryousei
//
//==============================================================================================
#include "char_select.h"
#include "input.h"
#include "application.h"
#include "fade.h"
#include "fontString.h"
#include "chardecision_window.h"
#include "confirmation_window.h"
#include "camera.h"
#include "halfsphere.h"
#include "playerdata.h"
#include "model.h"
#include "objectX.h"
#include "sound.h"
#include "texture.h"

//==============================================================================================
// 静的メンバ変数宣言
//==============================================================================================
CCharDecision_Window* CChar_Select::m_pCharDecisionWindow = nullptr;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CChar_Select::CChar_Select()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CChar_Select::~CChar_Select()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CChar_Select::Init()
{
	//======================================
	// メンバ変数の初期化処理
	//======================================
	m_nIndex = 0;
	m_nPlayerIndex = 0;

	m_pFontTexture = CObject2D::Create({ SCREEN_WIDTH / 2,100.0f,0.0f }, { 300.0f,100.0f });
	m_pFontTexture->SetTexture(CTexture::TEXTURE_AIRCRAAFTSELECTION);

	CCamera* pCamera = CApplication::GetCamera();	// カメラのポインタ
	m_pPlayerData = new CPlayerData;				// プレイヤーデータのインスタンス

	pCamera->SetPosV({ 0.0f, 100.0f, -100.0f });
	pCamera->SetPosR({ 0.0f, 0.0f, 100.0f });

	// プレイヤーデータの初期化処理
	m_pPlayerData->Init();

	// プレイヤーデータの生成処理
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_pObjectX[nCnt] = CObjectX::Create(D3DXVECTOR3(-80.0f + 55.0f * nCnt, 600.0f, 0.0f), D3DXVECTOR3(0.0f, -0.3f + 0.25f * nCnt, 0.0f), nullptr, "Data/model/SelectMode/view_Body_00.x");
	}

	// 倉庫モデルの生成
	m_pWareHouse = CObjectX::Create(D3DXVECTOR3(0.0f, 300.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0, 0.0f), nullptr, "Data/model/CharacterSelect/Warehouse.x");

	//BGM
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_CHARACTER);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CChar_Select::Uninit()
{
	// メニューウィンドウの終了処理
	if (m_pCharDecisionWindow != nullptr)
	{
		m_pCharDecisionWindow->Uninit();
		delete m_pCharDecisionWindow;
		m_pCharDecisionWindow = nullptr;
	}

	// プレイヤーデータ
	if (m_pPlayerData != nullptr)
	{
		m_pPlayerData->Uninit();
		delete m_pPlayerData;
		m_pPlayerData = nullptr;
	}
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CChar_Select::Update()
{
	CInput* pInput = CInput::GetKey();

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if ((pInput->Trigger(DIK_RETURN) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
			|| pInput->Trigger(JOYPAD_START, nCnt))
			&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			if (m_pCharDecisionWindow == nullptr)
			{
				// 決定SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);
				m_pCharDecisionWindow = CCharDecision_Window::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f), 900.0f, 500.0f, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			}
		}
	}

	if (m_pCharDecisionWindow != nullptr)
	{
		m_pCharDecisionWindow->Update();
	}

	// プレイヤーデータの更新処理
	if (m_pPlayerData != nullptr)
	{
		m_pPlayerData->Update();
	}

	// キャラ切り替え処理
	CharSwitching(0);
}

//==============================================================================================
//	キャラ切り替え処理
//==============================================================================================
void CChar_Select::CharSwitching(int PlayerIndex)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (nCnt == PlayerIndex)
		{// 指定したプレイヤーとそのプレイやーと同じ番号だった時

			// 現在の番号の取得
			m_nIndex = m_pPlayerData->GetPlayerIndex();

			// nullチェック
			if (m_pCharDecisionWindow != nullptr)
			{
				// 今選んでいる番号の取得
				m_nIndex = m_pCharDecisionWindow->GetSelectIndex();
			}

			// 番号の変更
			if (m_nIndex <= -1)
			{
				m_nIndex = 3;
			}
			if (m_nIndex >= 4)
			{
				m_nIndex = 0;
			}

			// 現在の番号と前の番号が一致していなかったら
			if (m_nIndex != m_nIndexKeep)
			{
				// オブジェクトXの削除
				if (m_pObjectX[nCnt] != nullptr)
				{
					m_pObjectX[nCnt]->Uninit();
					m_pObjectX[nCnt] = nullptr;
				}
			}

			// 番号の保存
			m_nIndexKeep = m_nIndex;

			// モデルの設定
			if (CChar_Select::GetCharDecisionWindow() != nullptr)
			{
				SetModel(0, "Data/model/SelectMode/view_Body_00.x");
				SetModel(1, "Data/model/SelectMode/view_Body_01.x");
				SetModel(2, "Data/model/SelectMode/view_Body_02.x");
				SetModel(3, "Data/model/SelectMode/view_Body_03.x");
			}
		}
	}
}

//==============================================================================================
// モデルの設定処理
//==============================================================================================
void CChar_Select::SetModel(int index, const char * Xfilename)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pObjectX[nCnt] == nullptr)
		{
			if (m_nIndex == index)
			{
				m_pObjectX[nCnt] = CObjectX::Create(D3DXVECTOR3(-80.0f + 55.0f * nCnt, 600.0f, 0.0f), D3DXVECTOR3(0.0f, -0.3f, 0.0f), nullptr, Xfilename);
			}
		}
	}
}

//==============================================================================================
// 生成処理
//==============================================================================================
CChar_Select* CChar_Select::Create()
{
	CChar_Select* pChar_Select = new CChar_Select;

	if (FAILED(pChar_Select->Init()))
	{
		return nullptr;
	}

	return pChar_Select;
}
