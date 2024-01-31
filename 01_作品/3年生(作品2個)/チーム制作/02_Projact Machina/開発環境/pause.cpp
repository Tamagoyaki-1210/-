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
// コンストラクタ
//=============================================================================
CPause::CPause()
{
	m_bPause = false;
	m_Select_Pause = false;
	m_pPause = nullptr;
	m_StringSize = { 0.0f,0.0f };
}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CPause::Init(void)
{
	// ポーズの背景
	if (m_pPause == nullptr)
		m_pPause = CObject2D::Create({ 950.0f,400.0f,0.0f }, { 0.0f,0.0f }, CObject::PRIORITY_SCREEN);

	// ポーズ
	m_pPause_Menu[PAUSE_HEADER] = CFontString::Create({ 860.0f, 200.0f, 0.0f }, { m_StringPauseSize }, "ポーズ");

	// 選択メニューのサイズ
	m_pPause_Pos[PAUSE_CONTINUE] = { 800.0f, 290.0f, 0.0f };
	//m_pPause_Pos[PAUSE_STAGE_SELECT] = { 770.0f, 430.0f, 0.0f };
	m_pPause_Pos[PAUSE_CHAR_SELECT] = { 790.0f, 570.0f, 0.0f };

	// 選択メニューの生成
	m_pPause_Menu[PAUSE_CONTINUE] = CFontString::Create(m_pPause_Pos[PAUSE_CONTINUE], { m_StringSize }, "ゲームをつづける");
	//m_pPause_Menu[PAUSE_STAGE_SELECT] = CFontString::Create(m_pPause_Pos[PAUSE_STAGE_SELECT], { m_StringSize }, "ステージせんたくへ");
	m_pPause_Menu[PAUSE_CHAR_SELECT] = CFontString::Create(m_pPause_Pos[PAUSE_CHAR_SELECT], { m_StringSize }, "キャラせんたくへ");

	// 選択の初期値
	m_nSelect = PAUSE_CONTINUE;

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CPause::Uninit(void)
{
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CPause::Update(void)
{
	if (!CApplication::GetCamera()->GetOpening())
	// ポーズ
	Pause();

	// ポーズ中
	if (m_bPause)
	{
		// 選択処理
		Select();

		// カーソルとの当たり判定
		StringCollision();
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CPause::Draw(void)
{
}

//==============================================================================================
// ポーズ画面
//==============================================================================================
void CPause::Pause()
{
	CInput *pInput = CInput::GetKey();

	if (pInput != nullptr)
	{
		// ポーズ中
		if ((pInput->Trigger(DIK_P) || pInput->Trigger(JOYPAD_START))
			&& !m_bPause)
		{
			m_pPause->SetSize({ 400.0f,500.0f });
			m_StringSize = { 30.0f,30.0f };
			m_StringPauseSize = { 60.0f,60.0f };
			m_bPause = true;
		}
		// ポーズしてない
		else if ((pInput->Trigger(DIK_P) || pInput->Trigger(JOYPAD_START))
			&& m_bPause || m_Select_Pause)
		{
			m_pPause->SetSize({ 0.0f,0.0f });
			m_StringSize = { 0.0f,0.0f };
			m_StringPauseSize = { 0.0f,0.0f };
			m_bPause = false;
			m_Select_Pause = false;
		}

		// 色の設定
		if (m_pPause != nullptr)
			m_pPause->SetCol({ 0.0f,0.0f,0.0f,0.5f });

		for (int nCnt = 0; nCnt < PAUSE_MAX; nCnt++)
		{
			// サイズの設定
			m_pPause_Menu[nCnt]->SetSize(m_StringSize);
		}
		m_pPause_Menu[PAUSE_HEADER]->SetSize({ m_StringPauseSize });
	}
}

//==============================================================================================
// 選択処理
//==============================================================================================
void CPause::Select()
{
	CInput *pInput = CInput::GetKey();
	
	// 選択肢の変更
	if ((pInput->Trigger(DIK_DOWN) || pInput->Trigger(JOYPAD_DOWN)) 
		&& m_nSelect < PAUSE_CHAR_SELECT)
		m_nSelect++;
	else if ((pInput->Trigger(DIK_UP) || pInput->Trigger(JOYPAD_UP))
		&& m_nSelect > PAUSE_CONTINUE)
		m_nSelect--;

	// 選択している項目を濃くする
	m_pPause_Menu[m_nSelect]->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	for (int nCnt = PAUSE_CONTINUE; nCnt < PAUSE_MAX; nCnt++)
	{
		if (nCnt == m_nSelect)
			continue;

		// 選択されていない項目を半透明にする
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
		//	// ステージ選択に戻る
		//		if(CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
		//		// 画面遷移
		//		CFade::SetFade(CApplication::MODE_STAGE_SELECT, 0.05f);
			break;
		case PAUSE_CHAR_SELECT:
			// ウィンドウの破棄
			//DestroyWindow(CApplication::GetWindow());
			// キャラ選択に戻る
			if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
				// 画面遷移
				CFade::SetFade(CApplication::MODE_CHAR_SELECT, 0.05f);
			break;
		default:
			break;
		}
	}
}

//==============================================================================================
// カーソルとの当たり判定
//==============================================================================================
void CPause::StringCollision()
{
	CInput *pInput = CInput::GetKey();

	// マウスカーソルの位置
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
// 生成処理
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