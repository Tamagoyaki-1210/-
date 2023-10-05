//=============================================================================
//
// 背景
// Author : tanimoto kosuke
//
//=============================================================================
#include "bg.h"
#include "application.h"
#include "texture.h"

//---------------------------
//マクロ関数
//---------------------------
#define BG_TUTORIAL		(800)

//=====================================
//デフォルトコンストラクタ
//=====================================
CBG::CBG()
{

}

//=====================================
//デストラクタ
//=====================================
CBG::~CBG()
{

}

//=====================================
//初期化処理
//=====================================
void CBG::Init()
{
	CObject2D::Init();

	//外部ファイルで変数に値を代入したい枠
	m_fWidth = BG_WIDTH;
	m_fHeight = BG_HEIGHT;

	//オブジェクト設定処理
	SetObject2D(CObject::TYPE_BG, m_fWidth, m_fHeight);
}

//=====================================
//終了処理
//=====================================
void CBG::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CBG::Update()
{
	CObject2D::Update();


	//UV設定処理
	CObject2D::SetUV();

	//位置取得
	D3DXVECTOR3 pos = CObject2D::GetPos();

	//角度取得処理
	D3DXVECTOR3 rot = CObject2D::GetRot();

	//減衰
	m_move = m_move - (m_move * 0.05f);

	//位置更新
	pos += m_move;

	//位置設定処理
	SetPos(pos);

	//角度取得処理
	CObject2D::SetRot(rot);

	UpdateType();
}

//=====================================
//描画処理
//=====================================
void CBG::Draw()
{
	switch (m_bgType)
	{
	case TYPE_BG01 :
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG);
		break;
	case TYPE_Tutorial01:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG_Tutorial01);
		break;
	case TYPE_Tutorial02:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG_Tutorial02);
		break;
	case TYPE_Tutorial03:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG_Tutorial03);
		break;
	case TYPE_Clear:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_BG);
		break;
	}
	CObject2D::Draw();
}

//=====================================
//種別更新処理
//=====================================
void CBG::UpdateType()
{
	switch (m_bgType)
	{
	case TYPE_BG01:
		break;
	case TYPE_Tutorial01:
		if (m_nDestroyCounter <= 0)
		{
			Uninit();
		}
		else
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)m_nDestroyCounter /  BG_TUTORIAL)));
			m_nDestroyCounter--;
		}
		break;
	case TYPE_Tutorial02:
		if (m_nDestroyCounter <= 0)
		{
			Uninit();
		}
		else
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)m_nDestroyCounter / BG_TUTORIAL)));
			m_nDestroyCounter--;
		}
		break;
	case TYPE_Tutorial03:
		if (m_nDestroyCounter <= 0)
		{
			Uninit();
		}
		else
		{
			CObject2D::SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, ((float)m_nDestroyCounter / BG_TUTORIAL)));
			m_nDestroyCounter--;
		}
		break;
	case TYPE_Clear:
		//(青)
		CObject2D::SetColor(D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.4f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.2f));
		break;
	default:
		break;
	}
}

//=====================================
//背景生成処理
//=====================================
CBG *CBG::Create(D3DXVECTOR3 pos, BG_Type bgType)
{
	CBG* pBG = new CBG;		//プレイヤーポインタ

	if (pBG != nullptr)
	{
		pBG->Init();
		pBG->SetPos(pos);
		pBG->m_bgType = bgType;
		if (bgType == TYPE_Tutorial01 || bgType == TYPE_Tutorial02 || bgType == TYPE_Tutorial03)
		{
			pBG->m_nDestroyCounter = BG_TUTORIAL;
		}
	}

	return pBG;
}
