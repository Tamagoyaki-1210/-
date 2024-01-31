//=============================================================================
//
// shadow.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "shadow.h"
#include "application.h"
#include "renderer.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CShadow::CShadow() : CObject3D(1)
{

}

//=====================================
// デストラクタ
//=====================================
CShadow::~CShadow()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CShadow::Init()
{
	CObject3D::Init();

	SetCol({1.0f, 1.0f, 1.0f, 1.0f});
	Setbillboard(true);

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CShadow::Uninit()
{
	CObject3D::Uninit();
}

//=====================================
// 更新処理
//=====================================
void CShadow::Update()
{
	CObject3D::Update();
}

//=====================================
// 描画処理
//=====================================
void CShadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject3D::Draw();

	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================================
// 影の処理
//============================================================================
void CShadow::SetShadow(const D3DXVECTOR3 pos)
{
	SetPos({ pos.x, 0.0f, pos.z });

	// 高さで色を変更
	SetCol({ 1.0f, 1.0f, 1.0f, 1.0f - (pos.y * 0.005f) });
}

//=====================================
// 生成処理
//=====================================
CShadow* CShadow::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size)
{
	CShadow* pShadow = new CShadow;

	if (FAILED(pShadow->Init()))
	{
		return nullptr;
	}

	pShadow->SetPos(pos);
	pShadow->SetSize(size);
	pShadow->SetTexture(CTexture::TEXTURE_SHADOW);

	return pShadow;
}