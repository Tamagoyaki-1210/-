//=============================================================================
//
// フェード
// Author : tanimoto kosuke
//
//=============================================================================
#include "fade.h"
#include <assert.h>
#include <stdio.h>
#include "application.h"
#include "texture.h"

//---------------------------------------------------------
// マクロ定義
//---------------------------------------------------------
#define ALPHA_CHANGE	(0.05f)	// フェードのa値の変化量

//=====================================
// デフォルトコンストラクタ
//=====================================
CFade::CFade() : CObject2D(UI_PRIORITY_FADE), m_fAlpha(0.0f)
{

}

//=====================================
// デストラクタ
//=====================================
CFade::~CFade()
{

}

//=====================================
// 初期化処理
//=====================================
void CFade::Init()
{
	CObject2D::Init();

	// オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_UI, F_SCREEN_WIDTH, F_SCREEN_HEIGHT);
}

//=====================================
// 終了処理
//=====================================
void CFade::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
// 更新処理
//=====================================
void CFade::Update()
{
	CObject2D::Update();

	// UV設定
	CObject2D::SetUV();

	if (m_fade == FADE_NONE)
	{// 何もしていない状態の場合
		return;
	}

	switch (m_fade)
	{
	// フェードアウト状態の場合
	case FADE_OUT:
		m_fAlpha += ALPHA_CHANGE;	// ポリゴンを不透明にしていく

		if (m_fAlpha >= 1.0f)
		{// 完全に不透明になった場合
			m_fAlpha = 1.0f;
			m_fade = FADE_CHANGE;  // フェード切り替え状態
		}
		break;
	// フェードイン状態の場合
	case FADE_IN:		
		m_fAlpha -= ALPHA_CHANGE;	// ポリゴンを徐々に透明にする

		if (m_fAlpha <= 0.0f)
		{// 完全に透明になった場合
			m_fAlpha = 0.0f;
			m_fade = FADE_CHANGE;	// フェード切り替え状態
		}
		break;
	// フェード切り替え状態の場合
	case FADE_CHANGE:
		//ポリゴンが完全に不透明な場合
		if (m_fAlpha == 1.0f)
		{
			m_fade = FADE_IN;		// フェードイン状態
		}
		//ポリゴンが完全に透明な場合
		else if (m_fAlpha == 0.0f)
		{
			m_fade = FADE_NONE;		// 何もしていない状態
		}
		break;
	default:
		assert(false);
		break;
	}

	SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,m_fAlpha));
}

//=====================================
// 描画処理
//=====================================
void CFade::Draw()
{
	//画像取得処理
	CApplication::GetTexture()->TextureType(CTexture::TYPE_NONE);
	CObject2D::Draw();
}

//=====================================
// フェードアウト開始処理
//=====================================
void CFade::StartFadeOut()
{
	m_fade = FADE_OUT;	// フェードアウト状態
	m_fAlpha = 0.0f;	// ポリゴンを透明にする
}

//=====================================
// フェード生成処理
//=====================================
CFade *CFade::Create(float fAlpha, FADE fade)
{
	CFade* pFade = new CFade;

	if (pFade != nullptr)
	{
		pFade->Init();
		pFade->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f));
		pFade->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		pFade->m_fAlpha = fAlpha;
		pFade->m_fade = fade;
	}
	return pFade;
}