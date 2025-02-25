//==============================================================================
//
// number.cpp
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// インクルード
//==============================================================================
#include "application.h"
#include "renderer.h"
#include "number.h"
//==============================================================================
// 静的メンバ変数宣言
//==============================================================================

//==============================================================================
// コンストラクタ
//==============================================================================
CNumber::CNumber() : CObject2D(4)
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CNumber::~CNumber()
{
}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CNumber::Init()
{
	//オブジェクト2Dの初期化処理
	CObject2D::Init();

	//==================================================
	// メンバ変数の初期化
	//==================================================

	SetTexture(CTexture::TEXTURE_NUMBER);

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CNumber::Uninit()
{
	//オブジェクト2Dの終了処理
	CObject2D::Uninit();
}

//==============================================================================
// 更新処理
//==============================================================================
void CNumber::Update()
{
	//オブジェクト2Dの更新処理
	CObject2D::Update();
}

//==============================================================================
// 描画処理
//==============================================================================
void CNumber::Draw()
{
	//オブジェクト2Dの描画処理
	CObject2D::Draw();
}

//==============================================================================
// 数字の生成
//==============================================================================
CNumber *CNumber::Create(const D3DXVECTOR3 &pos, float xsize, float ysize)
{
	CNumber *pNumber = nullptr;

	//クラスの生成
	pNumber = new CNumber;				//数字のインスタンス生成

	//nullチェック
	if (pNumber != nullptr)
	{
		//初期化処理
		pNumber->Init();
		//設定処理
		pNumber->Setposition(pos);
		pNumber->SetSize(xsize, ysize);
	}
	else
	{
		assert(false);
	}

	return pNumber;
}