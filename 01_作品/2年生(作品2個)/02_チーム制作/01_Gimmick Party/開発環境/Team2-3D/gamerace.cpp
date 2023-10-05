//=============================================================================
//
// gamerace.cpp
// Author : tanimoto kosuke
//
//=============================================================================
#include "gamerace.h"
#include "input.h"
#include "application.h"
#include "inputKeyboard.h"
#include "debugProc.h"
#include "fade.h"
#include "stage.h"
#include "message.h"

CStage* CGameRace::m_pStage = nullptr;

//=====================================
// デフォルトコンストラクタ
//=====================================
CGameRace::CGameRace()
{

}

//=====================================
// デストラクタ
//=====================================
CGameRace::~CGameRace()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CGameRace::Init(void)
{
    CGame::Init();

	// カウントダウンメッセージ表示
	CApplication::GetMsg()->SetCountDown(3);

	m_pStage = CStage::Create();	// ステージ生成処理

    return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CGameRace::Uninit(void)
{
    CGame::Uninit();

	if (m_pStage != nullptr)
	{// ステージの終了処理
		m_pStage->Uninit();
		delete m_pStage;
		m_pStage = nullptr;
	}
}

//=====================================
// 更新処理
//=====================================
void CGameRace::Update(void)
{
    CGame::Update();

	if (m_pStage != nullptr)
	{// ステージの更新処理
		m_pStage->Update();
	}

#ifdef _DEBUG

    CDebugProc::Print("\n[F2] : デバッグモードへ移動\n");

	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{// フェード中ではない場合
		if (CInputKeyboard::GetKeyboardTrigger(DIK_BACK))
		{// Enterキーを押したら
			SetEndGame();
		}
	}

#endif // _DEBUG
}

//=====================================
// 生成処理
//=====================================
CGameRace* CGameRace::Create(void)
{
    CGameRace* pGameRace = new CGameRace;

    if (FAILED(pGameRace->Init()))
    {
        return nullptr;
    }

    return pGameRace;
}