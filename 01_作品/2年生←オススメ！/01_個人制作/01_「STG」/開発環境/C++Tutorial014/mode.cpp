//=============================================================================
//
// モード
// Author : tanimoto kosuke
//
//=============================================================================
#include "mode.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "menu.h"
#include "stage.h"

#include <assert.h>

CMode::MODE CMode::m_mode = MODE_NONE;
CMode::MODE CMode::m_modeNext = MODE_NONE;
CTitle *CMode::m_pTitle = nullptr;		//タイトルポインタ
CGame *CMode::m_pGame = nullptr;		//ゲームポインタ
CResult *CMode::m_pResult = nullptr;	//リザルトポインタ
CFade *CMode::m_pFade = nullptr;		// フェードポインタ
//=====================================
//デフォルトコンストラクタ
//=====================================
CMode::CMode()
{
	m_pTitle = new CTitle;
	m_pGame = new CGame;
	m_pResult = new CResult;
}

//=====================================
//デストラクタ
//=====================================
CMode::~CMode()
{

}

//=====================================
//初期化処理
//=====================================
void CMode::Init()
{

}

//=====================================
//終了処理
//=====================================
void CMode::Uninit()
{
	//タイトル
	if (m_pTitle != nullptr)
	{
		m_pTitle->Uninit();
		delete m_pTitle;
		m_pTitle = nullptr;
	}
	//ゲーム
	if (m_pGame != nullptr)
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}
	//リザルト
	if (m_pResult != nullptr)
	{
		m_pResult->Uninit();
		delete m_pResult;
		m_pResult = nullptr;
	}
}

//=====================================
//更新処理
//=====================================
void CMode::Update()
{
	switch (m_mode)
	{//どのモードか
	case MODE_TITLE:
		m_pTitle->Update();
		break;
	case MODE_GAME:
		m_pGame->Update();
		break;
	case MODE_RESULT:
		m_pResult->Update();
		break;
	case MODE_NONE:
		/* 処理無し */
		break;
	default:
		assert(false);
		break;
	}
}

//=====================================
//描画処理
//=====================================
void CMode::Draw()
{
	switch (m_mode)
	{//どのモードか
	case MODE_TITLE:
		m_pTitle->Draw();
		break;
	case MODE_GAME:
		m_pGame->Draw();
		break;
	case MODE_RESULT:
		m_pResult->Draw();
		break;
	case MODE_NONE:
		/* 処理無し */
		break;
	default:
		assert(false);
		break;
	}
}

//=====================================
//モードの設定
//=====================================
void CMode::SetMode()
{
	CFade::FADE fade = m_pFade->GetFade();

	if (m_modeNext == MODE_NONE)
	{//次のモードが決まっていない場合
		return;
	}
	
	if (fade == CFade::FADE_NONE)
	{//何もしていない状態なら　
		m_pFade->StartFadeOut();
	}
	if (fade != CFade::FADE_CHANGE)
	{//フェードイン状態じゃない
		return;
	}

	float fAlpha = m_pFade->GetAlpha();

	switch (m_mode)
	{//現在モードの終了
	case MODE_TITLE:
		CSound::StopSound(SOUND_LABEL_BGM_TITLE);
		m_pTitle->Uninit();
		break;
	case MODE_GAME:
		//現在のステージ毎に曲を止める
		switch (CStage::GetStageMode())
		{
		case Stage_1:
			CSound::StopSound(SOUND_LABEL_BGM_STAGE1);
			break;
		case Stage_2:
			CSound::StopSound(SOUND_LABEL_BGM_STAGE2);
			break;
		case Stage_3:
			CSound::StopSound(SOUND_LABEL_BGM_STAGE3);
			break;
		case Stage_4:
			CSound::StopSound(SOUND_LABEL_BGM_STAGE4);
			break;
		case Stage_Boss:
			CSound::StopSound(SOUND_LABEL_BGM_BOSS);
			CSound::StopSound(SOUND_LABEL_BGM_EXPLOSION);
			break;
		default:
			break;
		}
		m_pGame->Uninit();
		break;
	case MODE_RESULT:
		if (CStage::GetClear() == false)
		{
			CSound::StopSound(SOUND_LABEL_BGM_RESULT);
		}
		else
		{
			CSound::StopSound(SOUND_LABEL_BGM_CLEAR);
		}
		m_pResult->Uninit();
		break;
	case MODE_NONE:
		/* 処理無し */
		break;
	default:
		assert(false);
		break;
	}

	switch (m_modeNext)
	{//次のモードの初期化
	case MODE_TITLE:
		CSound::PlaySound(SOUND_LABEL_BGM_TITLE);
		m_pTitle->Init();
		break;
	case MODE_GAME:
		m_pGame->Init();
		break;
	case MODE_RESULT:
		if (CStage::GetClear() == false)
		{
			CSound::PlaySound(SOUND_LABEL_BGM_RESULT);
		}
		else
		{
			CSound::PlaySound(SOUND_LABEL_BGM_CLEAR);
		}
		m_pResult->Init();
		break;
	case MODE_NONE:
		/* 処理無し */
		break;
	default:
		assert(false);
		break;
	}
	m_mode = m_modeNext;	//現在の画面(モード)を切り替える
	m_modeNext = MODE_NONE;

	m_pFade = CFade::Create(fAlpha, fade);
}

//=====================================
//モードの変更
//=====================================
void CMode::ChangeMode(MODE mode)
{
	m_modeNext = mode;
}

//=====================================
//最初のモード設定
//=====================================
void CMode::FirstMode(MODE mode)
{
	m_modeNext = mode;
	m_pFade = CFade::Create(0.0f, CFade::FADE_NONE);
}

//=====================================
//モード取得処理
//=====================================
CMode::MODE CMode::GetMode()
{
	return m_mode;
}