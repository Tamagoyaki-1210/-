//============================================================================
//
// chardecision_window.cpp (キャラ決定ウィンドウ.cpp)
// Author : Saito Shian
//
//============================================================================

//============================================================================
// インクルード
//============================================================================
#include "chardecision_window.h"
#include "application.h"
#include "renderer.h"
#include "object2D.h"
#include "menu.h"
#include "fade.h"
#include "input.h"
#include "game.h"
#include "title.h"
#include "menu_window.h"
#include "title_menu.h"
#include "texture.h"
#include "confirmation_window.h"
#include "char_select.h"
#include "connect.h"
#include "number.h"
#include "player.h"
#include "weapon.h"
#include "player_parameter.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CConfirmation_Window* CCharDecision_Window::m_pConfirmation = nullptr;
CPlayer_Parameter* CCharDecision_Window::m_pPlayer_Parameter = nullptr;
const float CCharDecision_Window::BLINK_SPEED = 0.02f;	// 選択肢の点滅速度
const float CCharDecision_Window::MINI_ALPHA = 0.5f;	// 選択肢の最低透明値

//============================================================================
// コンストラクタ
//============================================================================
CCharDecision_Window::CCharDecision_Window()
{
}

//============================================================================
// デストラクタ
//============================================================================
CCharDecision_Window::~CCharDecision_Window()
{
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CCharDecision_Window::Init()
{
	//==================================================
	// メンバ変数の初期化
	//================================================== 
	m_nSelectIndex = 0;
	m_fMoveX = 175.0f;
	m_bMaxSize = false;
	m_bSlideFlag = false;
	m_bFontFlag = false;
	m_bTextureFlag = false;
	m_bLeftRight = false;
	m_bStopFlag = true;
	m_bPosDest = false;
	//m_bExplanationUninit = false;
	m_bScale = false;
	m_bDicision = false;
	m_bUninitFlag = false;
	m_bScaleReduce = false;
	m_bScaleExpansion = false;
	m_bSpawnWindow = true;
	m_pWindow = CObject2D::Create(D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f), D3DXVECTOR2(0.0f, 0.0f), CObject::PRIORITY_SCREEN);
	m_pWindow->SetCol(D3DXCOLOR(m_Color.r, m_Color.g, m_Color.b, m_Color.a));

	m_pPlayer_Parameter = new CPlayer_Parameter;
	m_pPlayer_Parameter->Init();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CCharDecision_Window::Uninit()
{
	//メンバ変数の初期化
	if (m_pWindow != nullptr)
	{
		m_pWindow->Uninit();
		m_pWindow = nullptr;
	}

	// 選択番号の破棄
	if (m_pSelect_Number != nullptr)
	{
		m_pSelect_Number->Uninit();
		m_pSelect_Number = nullptr;
	}

	if (m_pPlayer_Parameter != nullptr)
	{
		m_pPlayer_Parameter->Uninit();
		delete m_pPlayer_Parameter;
		m_pPlayer_Parameter = nullptr;
	}
}

//============================================================================
// 更新処理
//============================================================================
void CCharDecision_Window::Update()
{
	// 入力デバイスの情報
	CInput* pInput = CInput::GetKey();

	if (m_bScale == false)
	{// ウィンドウの拡大処理
		CharDecisionMenuScale();
	}

	if (m_bScaleReduce == true)
	{
		// フォントとテクスチャの削除
		UninitExplanation();
		CharDecisionMenuScaleReduce();
	}

	if (m_pWindow != nullptr && m_bScaleReduce == false)
	{
		// キャラの変更処理
		CharSelectChoice();
	}

	if (m_bScaleExpansion == true)
	{
		m_pConfirmation->ConfirmatiomnMenuScale();
		m_pConfirmation->VariableInit();
	}

	if (m_bMaxSize == true && m_bScale == true)
	{
		// プレイヤー番号処理
		PlayerIndex();
	}

	// ゲーム画面への遷移
	if ((pInput->Trigger(DIK_RETURN) || pInput->Trigger(JOYPAD_A) || pInput->Trigger(JOYPAD_B))
		&& m_bMaxSize == true)
	{
		m_bScaleReduce = true;

		// プレイヤー番号の設定
		CApplication::SetPlayerJobIndex(m_nSelectIndex, CPlayer::PARTS_BODY);
		CApplication::SetPlayerJobIndex(-1, CPlayer::PARTS_ARMS);
		CApplication::SetPlayerJobIndex(-1, CPlayer::PARTS_LEG);

		// 素手の設定
		CApplication::SetPlayerWeaponIndex(CWeapon::WEAPON_KNUCKLE);
	}
}

//============================================================================
// ウィンドウの拡大処理
//============================================================================
void CCharDecision_Window::CharDecisionMenuScale()
{
	if (m_pWindow != nullptr)
	{// nullチェック

		// サイズの拡大
		m_size.x += SizeXScaleSpeed;
		m_size.y += SizeYScaleSpeed;

		if (m_size.x >= m_MaxSize.x)
		{// Xサイズの最大
			m_size.x = m_MaxSize.x;
		}
		if (m_size.y >= m_MaxSize.y)
		{// Yサイズの最大
			m_size.y = m_MaxSize.y;
		}

		// ウィンドウが最大値まで行ったら
		if (m_size.x >= m_MaxSize.x && m_size.y >= m_MaxSize.y && m_bMaxSize == false)
		{
			m_bMaxSize = true;		// 最大サイズに到達
			m_bScale = true;		// 拡大された

			// 選択番号の生成
			m_pSelect_Number = CNumber::Create({SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f, 0.0f}, 80.0f, 100.0f, CObject::PRIORITY_SCREEN);
			m_pSelect_Number->AnimTexture(m_nSelectIndex + 1, 10);
			return;
		}

		// サイズの設定
		m_pWindow->SetSize(D3DXVECTOR2(m_size.x, m_size.y));
	}
}

//=============================================================================
// メニューのサイズ縮小処理
//=============================================================================
bool CCharDecision_Window::CharDecisionMenuScaleReduce()
{
	if (m_pWindow != nullptr)
	{// nullチェック

		//m_bExplanationUninit = true;		// テクスチャとフォントの削除

		// サイズの縮小
		m_size.x -= SizeXScaleSpeed;
		m_size.y -= SizeYScaleSpeed;

		if (m_size.x <= 0.0f)
		{// Xサイズの最小
			m_size.x = 0.0f;
		}
		if (m_size.y <= 0.0f)
		{// Yサイズの最小
			m_size.y = 0.0f;
		}

		// ウィンドウが最小値まで行ったら
		if (m_size.x <= 0.0f && m_size.y <= 0.0f)
		{
			// 画面遷移
			CFade::SetFade(CApplication::MODE_GAME, 0.1f);
			m_bSpawnWindow = false;		// ウィンドウを生成されていない状態にする
			m_bUninitFlag = true;		// 削除フラグをtrue
			m_bScaleReduce = false;

			// 終了処理
			Uninit();					

			return true;
		}

		// サイズの設定
		m_pWindow->SetSize(D3DXVECTOR2(m_size.x, m_size.y));
	}
	return false;
}

//=============================================================================
// 選択肢の処理
//=============================================================================w
void CCharDecision_Window::CharSelectChoice()
{
	// 入力デバイスの情報
	CInput* pInput = CInput::GetKey();

	// フェード中では無い場合 && 表示中の場合
	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
		// 選択されたかどうか
		bool bSelect = false;

		// 左に移動する
		if ((pInput->Trigger(DIK_A) || pInput->Trigger(JOYPAD_LEFT)) && m_bStopFlag == true
			|| (pInput->Trigger(JOYPAD_UP)) && m_bStopFlag == false)
		{
			// 選択した番号の取得
			m_nSelectIndex--;		// 番号を1つ戻す
			bSelect = true;
			m_bLeftRight = false;	// 左に移動
		}
		// 右に移動する
		else if ((pInput->Trigger(DIK_D) || pInput->Trigger(JOYPAD_RIGHT) )&& m_bStopFlag == true
			|| (pInput->Trigger(JOYPAD_DOWN)) && m_bStopFlag == false)
		{
			m_nSelectIndex++;		// 番号を1つ進める
			bSelect = true;
			m_bLeftRight = true;	// 右に移動
		}

		// 選択された場合
		if (bSelect == true)
		{
			m_bSlideFlag = true;	// スライドさせる
			m_bPosDest = false;		// 目的の位置に到達していない
			m_bStopFlag = false;	// 動いている状態
									// 選択SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
		}
	}
	
	// ウィンドウスライド処理
	SlideWindow();
}

//============================================================================
// ウィンドウを戻す処理
//============================================================================
void CCharDecision_Window::BackWindow()
{
	// 入力デバイスの情報
	CInput* pInput = CInput::GetKey();
	
	// 前のウィンドウに戻る
	if (pInput->Trigger(DIK_BACK))
	{
		m_bDicision = true;
		m_bScale = true;
	}
	if (m_bScale == true && m_bDicision == true)
	{
		CharDecisionMenuScaleReduce();
	}
}

//============================================================================
// ウィンドウをスライドさせる処理
//============================================================================
void CCharDecision_Window::SlideWindow()
{
	// 位置の取得
	D3DXVECTOR3 pos = m_pWindow->GetPos();

	if (m_bSlideFlag == true)
	{// スライドされている時

	 // 左移動
		if (m_bLeftRight == false)
		{
			//m_bExplanationUninit = true;		// テクスチャとフォントの削除
			UninitExplanation();				// フォントの削除
 			pos.x += m_fMoveX;					// 移動量の減算

			if (pos.x >= 1920.0f && m_bPosDest == false)
			{// 位置が1920.0f以上 && 目的の位置まで到達していなかったら

				pos.x = -SCREEN_WIDTH / 2;		// 位置の設定
				m_bPosDest = true;				// 目的の位置まで到達した
			}
			if (pos.x >= SCREEN_WIDTH / 2 && m_bPosDest == true)
			{// 位置が640以下 && 目的の位置まで到達した時

				pos.x = SCREEN_WIDTH / 2;		// 位置の設定
				m_bFontFlag = false;			// フォントを使用していない状態にする
				m_bTextureFlag = false;
				m_bStopFlag = true;				// スライドを停止させる
				//m_bExplanationUninit = false;   // テクスチャとフォントの表示
			}
		}
		// 右移動
		else
		{
			//m_bExplanationUninit = true;		// テクスチャとフォントの削除
			UninitExplanation();				// フォントの削除
			pos.x -= m_fMoveX;					// 移動量の加算
			if (pos.x <= -SCREEN_WIDTH / 2 && m_bPosDest == false)
			{// 位置が-640以下 && 目的の位置まで到達していなかったら

				pos.x = 1920.0f;				// 位置の設定
				m_bPosDest = true;				// 目的の位置まで到達した
			}
			if (pos.x <= SCREEN_WIDTH / 2 && m_bPosDest == true)
			{// 位置が640以下 && 目的の位置まで到達した時

				pos.x = SCREEN_WIDTH / 2;		// 位置の設定
				m_bFontFlag = false;			// フォントを使用していない状態にする
				m_bTextureFlag = false;
				m_bStopFlag = true;				// スライドを停止させる
				//m_bExplanationUninit = false;	// テクスチャとフォントの表示
			}
		}
		// 位置の設定
		m_pWindow->SetPos(pos);
	}
}

//============================================================================
// プレイヤー番号処理
//============================================================================
void CCharDecision_Window::PlayerIndex()
{
	if (m_nSelectIndex <= -1)
	{
		m_nSelectIndex = 3;
	}
	else if (m_nSelectIndex >= 4)
	{
		m_nSelectIndex = 0;
	}

	if (m_nSelectIndex == 0)
	{
		SetFont("ラッシュ");
		SetTextue(CTexture::TEXTURE_SKILL_RUSH);
	}
	else if (m_nSelectIndex == 1)
	{
		SetFont("ヴァンガード");
		SetTextue(CTexture::TEXTURE_SKILL_VANGUARD);
	}
	else if (m_nSelectIndex == 2)
	{
		SetFont("イーグルアイ");
		SetTextue(CTexture::TEXTURE_SKILL_EAGLEEYE);
	}
	else if (m_nSelectIndex == 3)
	{
		SetFont("マーシャル");
		SetTextue(CTexture::TEXTURE_SKILL_MARSHALL);
	}

	// 選択番号の更新
	if (m_pSelect_Number != nullptr)
	{
		m_pSelect_Number->AnimTexture(m_nSelectIndex + 1, 10);
	}
}

//============================================================================
// フォントの設定処理
//============================================================================
void CCharDecision_Window::SetFont(const std::string lette)
{
	if (m_pFont == nullptr && m_bFontFlag == false)
	{
		m_pFont = CFontString::Create(D3DXVECTOR3(650.0f, 250.0f, 0.0f), { 35.0f, 35.0f }, lette);
	}
	m_bFontFlag = true;
}

//============================================================================
// テクスチャの設定処理
//============================================================================
void CCharDecision_Window::SetTextue(CTexture::TEXTURE texture)
{
	if (m_bTextureFlag == false)
	{
		if (m_pObject2D == nullptr)
		{
			m_pObject2D = CObject2D::Create(D3DXVECTOR3(500.0f, 200.0f, 0.0f), D3DXVECTOR2(175.0f, 175.0f), CObject::PRIORITY_SCREEN);
			m_pObject2D->SetTexture(texture);
		}

		// パラメーターのテキスト設定
		SetParameterText();

		m_bTextureFlag = true;
	}
}

//============================================================================
// パラメーターのテキスト設定
//============================================================================
void CCharDecision_Window::SetParameterText()
{
	std::string str = {};

	str = "<c=0,255,255,255,>ライフ</>:";
	std::stringstream strNum;
	strNum << m_pPlayer_Parameter->GetParameterJob(m_nSelectIndex + 1).nLife;
	str += strNum.str().c_str();

	if (m_pParameter_Font[0] == nullptr)
	{
		m_pParameter_Font[0] = CFontString::Create(D3DXVECTOR3(370.0f, 400.0f, 0.0f), { 50.0f, 50.0f }, str);
	}

	str = "<c=255,124,0,255,>スタミナ</>:";
	std::stringstream strNum2;
	strNum2 << m_pPlayer_Parameter->GetParameterJob(m_nSelectIndex + 1).nStamina;
	str += strNum2.str().c_str();
	if (m_pParameter_Font[1] == nullptr)
	{
		m_pParameter_Font[1] = CFontString::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), { 50.0f, 50.0f }, str);
	}
}

//============================================================================
// 説明用テクスチャ、フォントの破棄処理
//============================================================================
void CCharDecision_Window::UninitExplanation()
{
	// フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Uninit();
		m_pFont = nullptr;
	}

	// フォントの破棄
	if (m_pParameter_Font[0] != nullptr)
	{
		m_pParameter_Font[0]->Uninit();
		m_pParameter_Font[0] = nullptr;
	}

	// フォントの破棄
	if (m_pParameter_Font[1] != nullptr)
	{
		m_pParameter_Font[1]->Uninit();
		m_pParameter_Font[1] = nullptr;
	}

	// テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (m_pObject2D != nullptr)
		{
			m_pObject2D->Uninit();
			m_pObject2D = nullptr;
		}
	}
}

//============================================================================
// 生成処理
//============================================================================
CCharDecision_Window * CCharDecision_Window::Create(D3DXVECTOR3 pos, float xsize, float ysize, D3DXCOLOR col)
{
	//クラスの生成
	CCharDecision_Window* pCharDecision = new CCharDecision_Window; 

	//nullチェック
	if (pCharDecision != nullptr)
	{
		// 設定処理
		pCharDecision->m_pos = pos;
		pCharDecision->m_Color = col;
		pCharDecision->m_MaxSize.x = xsize;
		pCharDecision->m_MaxSize.y = ysize;
		// 初期化処理
		pCharDecision->Init();         
	}
	else
	{
		assert(false);
	}

	return pCharDecision;
}