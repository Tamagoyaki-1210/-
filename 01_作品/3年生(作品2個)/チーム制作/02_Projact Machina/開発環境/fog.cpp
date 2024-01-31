//============================================================================
//
// fog.cpp (フォグ.cpp)
// Author : Saito Shian
//
//============================================================================

//============================================================================
// インクルード
//============================================================================
#include "fog.h"
#include "application.h"
#include "renderer.h"
#include "object.h"
#include "input.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
bool CFog::m_bCreate = false;
bool CFog::m_bIntensityFlag = false;
float CFog::m_fIntensity = 0.0f;
float CFog::m_fIntensityDest = 0.5f;

float CFog::m_fFogStartPos = 0.0f;
float CFog::m_fFogEndPos = 0.0f;


//============================================================================
// コンストラクタ
//============================================================================
CFog::CFog()
{
}

//============================================================================
// デストラクタ
//============================================================================
CFog::~CFog()
{
}

//============================================================================
// フォグの設定処理
//============================================================================
void CFog::SetFog(D3DXCOLOR col, const float intensity)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//フォグの有効設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//フォグカラー
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(col.a, col.g, col.b, col.a));

	//フォグのモード(範囲指定:D3DFOG_LINEAR,密度指定:D3DFOG_EXP)
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);

	//密度指定(※EXP時に指定)
	m_fIntensity = intensity;
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_fIntensity));
}

//============================================================================
// フォグの終了処理
//============================================================================
void CFog::DestroyFog()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//フォグの有効設定
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	m_fIntensity = 0.0f;
}