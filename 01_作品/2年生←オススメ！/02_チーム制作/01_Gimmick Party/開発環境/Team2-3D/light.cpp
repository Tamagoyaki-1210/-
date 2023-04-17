//=============================================================================
//
// light.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "light.h"
#include "application.h"
#include "rendering.h"

//ライトのアドレス配列
CLight* CLight::m_pLight[CLight::MaxLight] = {};

//コンストラクタ
CLight::CLight()
{
	for (int nCnt = 0; nCnt < CLight::MaxLight; nCnt++)
	{
		if (m_pLight[nCnt] == nullptr)
		{//スペースがあったら、ライトのアドレスを保存する
			m_pLight[nCnt] = this;
			m_nIdx = nCnt;
			break;
		}
	}
}

//デストラクタ
CLight::~CLight()
{

}

//ライトの初期化処理
HRESULT CLight::Init(void)
{
	return S_OK;
}

//ライトの終了処理
void CLight::Uninit(void)
{

}

//ライトの更新処理
void CLight::Update(void)
{

}

//インスタンスの破棄処理
void CLight::Release(void)
{
	int nIdx = m_nIdx;			//インデックスを保存する

	delete m_pLight[nIdx];		//メモリを解放する
	m_pLight[nIdx] = nullptr;	//ポインタをnullにする
}

//インデックスの取得処理
const int CLight::GetLightIdx(void)
{
	return m_nIdx;
}



//全部のライトの破棄処理
void CLight::ReleaseAll(void)
{
	for (int nCnt = 0; nCnt < CLight::MaxLight; nCnt++)
	{
		if (m_pLight[nCnt] != nullptr)
		{
			m_pLight[nCnt]->Release();
		}
	}
}