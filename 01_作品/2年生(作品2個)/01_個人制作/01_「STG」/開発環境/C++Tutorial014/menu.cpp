//=============================================================================
//
// メニュー
// Author : tanimoto kosuke
//
//=============================================================================
#include "menu.h"
#include <stdio.h>
#include "application.h"
#include "texture.h"
#include "choice.h"
#include "game.h"
#include "message.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CMenu::CMenu() : CObject2D(UI_PRIORITY_UI)
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
void CMenu::Init()
{
	CObject2D::Init();

	m_menu = TYPE_NONE;

	m_menuMode = MODE_NONE;

	// オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_UI, F_SCREEN_WIDTH, F_SCREEN_HEIGHT);
}

//=====================================
// 終了処理
//=====================================
void CMenu::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
// 更新処理
//=====================================
void CMenu::Update()
{
	CObject2D::Update();

	// UV設定
	CObject2D::SetUV();
}

//=====================================
// 描画処理
//=====================================
void CMenu::Draw()
{
	//画像取得処理
	switch (m_menu)
	{
	case TYPE_NONE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_NONE);
		break;
	case TYPE_FRAME:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_NONE);
		break;
	default:
		break;
	}
	if (CGame::GetPause() == true)
	{
		CObject2D::Draw();
	}
}

//=====================================
// メニューモード毎の生成設定処理
//=====================================
void CMenu::SetCreate(D3DXVECTOR3 pos)
{
	switch (m_menuMode)
	{
	case MODE_TITLE:
		CChoice::Create(pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), CChoice::TITLE_GAMESTART);		//タイトル_ゲーム開始
		CChoice::Create(pos + D3DXVECTOR3(0.0f, 150.0f, 0.0f), CChoice::TITLE_EXIT);		//タイトル_終了
		break;
	case MODE_GAME:
		CMessage::Create(pos + D3DXVECTOR3(0.0f, -200.0f, 0.0f), CMessage::TYPE_PAUSE);	//ゲーム_ポーズ画面
		CChoice::Create(pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), CChoice::GAME_CONTINUE);	//ゲーム_再開
		CChoice::Create(pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f), CChoice::GAME_EXIT);		//ゲーム_終了
		break;
	default:
		break;
	}
}

//=====================================
// メニュー生成処理
//=====================================
CMenu *CMenu::Create(D3DXVECTOR3 pos, Menu_type menu, Menu_Mode menuMode)
{
	CMenu* pMenu = new CMenu;

	if (pMenu != nullptr)
	{
		pMenu->Init();
		pMenu->SetPos(pos);
		pMenu->m_menu = menu;
		pMenu->m_menuMode = menuMode;
		pMenu->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.1f));
		pMenu->SetCreate(pos);
	}
	return pMenu;
}
