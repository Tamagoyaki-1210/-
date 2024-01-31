//==============================================
//
// Player_Ui.cpp
// Author : Saito Shian
//
//==============================================

//==============================================
// インクルード
//==============================================
#include <assert.h>
#include "player_ui.h"
#include "texture.h"
#include "input.h"  

//==============================================
// コンストラクタ
//==============================================
CPlayerUi::CPlayerUi(const PRIORITY priority) : CObject2D(priority)
{

}

//==============================================
// デストラクタ
//==============================================
CPlayerUi::~CPlayerUi()
{

}

//==============================================
// 初期化処理
//==============================================
HRESULT CPlayerUi::Init()
{
	// object2Dの初期化処理
	CObject2D::Init();

	// テクスチャの設定
	SetTexture(CTexture::TEXTURE_UI);

	// UIの種類
	switch (m_Type)
	{
	case CPlayerUi::UITYPE_SUPPORT:
		SetTexPos(0.0f, 1.0f, 0.33f, 0.0f);
		SetData(1150.0f, 75.0f,100.0f,100.0f, CTexture::TEXTURE_BULLET);
		break;
	case CPlayerUi::UITYPE_ATTACK:
		SetTexPos(0.0f, 1.0f, 0.66f, 0.33f);
		SetData(150.0f, 75.0f, 100.0f, 100.0f, CTexture::TEXTURE_FLOOR);
		break;
	case CPlayerUi::UITYPE_WEAPON:
		SetTexPos(0.0f, 1.0f, 1.0f, 0.66f);
		SetData(1150.0f, 60.0f, 100.0f, 100.0f, CTexture::TEXTURE_NONE);
		break;
	case CPlayerUi::UITYPE_MAX:
		break;
	default:
		break;
	}

	// メンバ変数の初期化
	m_AlphaCount = ALPHA_COUNT;
	m_ColorCount = COLOR_COUNT;
	m_UseCount = USE_COUNT;
	m_Alpha = 1.0f;
	m_Color = 1.0f;
	m_bAlphaFlag = false;
	m_bColorFlag = false;
	m_bFlag = false;

	return S_OK;
}

//==============================================
// 終了処理
//==============================================
void CPlayerUi::Uninit()
{
	// object2Dの終了処理
	CObject2D::Uninit();
}

//==============================================
// 更新処理
//==============================================
void CPlayerUi::Update()
{
	// object2Dの更新処理
	CObject2D::Update();

	// 透明度処理
	SetAlpha();
}

//==============================================
// 描画処理
//==============================================
void CPlayerUi::Draw()
{
	// object2Dの描画処理
	CObject2D::Draw();
}

//==============================================
// 透明度処理
//==============================================
void CPlayerUi::SetAlpha()
{
	// インプットの取得
	CInput* pInput = CInput::GetKey();

	if (pInput->Trigger(DIK_R) && m_Type == CPlayerUi::UITYPE_SUPPORT)
	{
		m_bFlag = true;
		m_bAlphaFlag = true;
	}
	if (pInput->Trigger(DIK_Q) && m_Type == CPlayerUi::UITYPE_ATTACK)
	{
		m_bFlag = true;
		m_bAlphaFlag = true;
	}

	if (m_bFlag == true)
	{
		m_UseCount--;
		if (m_UseCount >= 0)
		{
			SetColor();
		}
	}

	if (m_UseCount <= 0)
	{
		// 半透明にする
		if (m_bAlphaFlag == true)
		{
			m_Alpha -= 0.03f;
			if (m_Alpha <= 0.5f)
			{
				m_Alpha = 0.5f;
				m_AlphaCount--;
				if (m_AlphaCount <= 0)
				{
					m_bAlphaFlag = false;
				}
			}
		}

		// 半透明から元に戻す
		if (m_AlphaCount <= 0 && m_bAlphaFlag == false)
		{
			m_Alpha += 0.03f;
			if (m_Alpha >= 1.0f)
			{
				// 変数を初期値に戻す
				m_Alpha = 1.0f;
				m_bAlphaFlag = false;
				m_AlphaCount = ALPHA_COUNT;
				m_UseCount = USE_COUNT;
				m_bFlag = false;
			}
		}

		// 透明度の設定
		m_pSkill_UI->SetAlpha(m_Alpha);
		CObject2D::SetAlpha(m_Alpha);
	}
}

//==============================================
// 色処理
//==============================================
void CPlayerUi::SetColor()
{
	m_ColorCount--;
	if (m_bColorFlag == false && m_ColorCount <= 0)
	{
		m_Color = 0.0f;
		m_bColorFlag = true;
		m_ColorCount = COLOR_COUNT;
	}
	else if (m_bColorFlag == true && m_ColorCount <= 0)
	{
		m_Color = 1.0f;
		m_bColorFlag = false;
		m_ColorCount = COLOR_COUNT;
	}

	// 色の設定
	m_pSkill_UI->SetCol(D3DXCOLOR(1.0f, m_Color, m_Color, 1.0f));
	CObject2D::SetCol(D3DXCOLOR(1.0f, m_Color, m_Color, 1.0f));
}

//==============================================
// データ設定処理
//==============================================
void CPlayerUi::SetData(float posX, float posY, float sizeX, float sizeY, CTexture::TEXTURE texture)
{
	m_pSkill_UI = CObject2D::Create(D3DXVECTOR3(posX, posY, 0.0f), D3DXVECTOR2(sizeX, sizeY), CObject::PRIORITY_BACK);
	m_pSkill_UI->SetTexture(texture);
}

//==============================================
// 生成処理
//==============================================
CPlayerUi* CPlayerUi::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, UITYPE type, const PRIORITY priority)
{
	//クラスの生成
	CPlayerUi* pPlayerUI = new CPlayerUi(priority);

	//nullチェック
	if (pPlayerUI != nullptr)
	{
		pPlayerUI->SetType(type);
		//初期化処理
		pPlayerUI->Init();
		pPlayerUI->SetPos(pos);
		pPlayerUI->SetSize(size);
	}
	else
	{
		assert(false);
	}

	return pPlayerUI;
}
