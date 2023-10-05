//=============================================================================
//
// AnimateUI.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "AnimateUI.h"


const D3DXCOLOR CAnimateUI::targetCol[6] =
{
	ColorRed,						//赤
	ColorYellow,					//黄
	ColorGreen,						//緑
	ColorCyan,						//シアン
	ColorBlue,						//青
	ColorMagenta					//マゼンタ
};

//コンストラクタ
CAnimateUI::CAnimateUI()
{
	//メンバー変数をクリアする
	ZeroMemory(&m_AnimInfo, sizeof(m_AnimInfo));		//アニメーション情報
	m_nCntAnim = 0;										//アニメーションカウンター
	m_bAnimateColor = false;							//カーラーアニメーションがあるかどうか
	m_ntargetCol = 0;									//目的のカーラー番号
	m_deltaCol = ColorNull;								//カーラーの減数
}

//デストラクタ
CAnimateUI::~CAnimateUI()
{

}

//初期化処理
HRESULT CAnimateUI::Init(void)
{
	if (FAILED(CObject_2D::Init()))
	{//基本クラスの初期化処理
		return -1;
	}

	ZeroMemory(&m_AnimInfo, sizeof(m_AnimInfo));	//アニメーション情報
	m_nCntAnim = 0;									//アニメーションカウンター
	m_bAnimateColor = false;						//カーラーアニメーションがあるかどうか
	m_deltaCol = ColorNull;							//カーラーの減数
	m_ntargetCol = 0;								//目的のカーラー番号

	return S_OK;
}

//終了処理
void CAnimateUI::Uninit(void)
{
	//基本クラスの終了処理
	CObject_2D::Uninit();
}

//更新処理
void CAnimateUI::Update(void)
{
	if (m_AnimInfo.nChangeTime != 0)
	{//アニメーション時間が0ではない場合

		m_nCntAnim++;			//カウンターをインクリメントする

		if (m_nCntAnim >= m_AnimInfo.nChangeTime)
		{//設定した時間を超えたら

			m_AnimInfo.deltaSize.x *= -1.0f;		//サイズアニメーションを逆にする
			m_AnimInfo.deltaSize.y *= -1.0f;		//サイズアニメーションを逆にする
			m_nCntAnim = 0;							//カウンターを0に戻す

			if (m_bAnimateColor)
			{//カーラーアニメーションの場合

				m_ntargetCol++;				//目的のカーラーの更新

				if (m_ntargetCol >= 6)
				{//最後のカーラーを超えたら
					m_ntargetCol = 0;			//元に戻す
				}

				D3DXCOLOR target = targetCol[m_ntargetCol];			//目的のカーラーを設定する
				D3DXCOLOR col = GetColor();							//現在のカーラーを設定する

				//カーラーの減数を計算する
				m_deltaCol = target - col;							
				m_deltaCol.r /= m_AnimInfo.nChangeTime;				
				m_deltaCol.g /= m_AnimInfo.nChangeTime;				
				m_deltaCol.b /= m_AnimInfo.nChangeTime;				
				m_deltaCol.a = 0.0f;								
			}
		}

		D3DXVECTOR2 size = GetSize();				//現在のサイズの取得

		size -= m_AnimInfo.deltaSize;				//サイズの更新

		//サイズが0以下にならないように
		if (size.x < 0.0f)
		{
			size.x = 0.0f;
		}
		if (size.y < 0.0f)
		{
			size.y = 0.0f;
		}

		SetSize(size);							//サイズの設定処理

		if (m_bAnimateColor)
		{//カーラーアニメーションの場合

			D3DXCOLOR col = GetColor();			//現在のカーラーの取得

			col += m_deltaCol;					//カーラーの更新
												
			SetColor(col);						//カーラーの設定
		}
	}

	//基本クラスの更新処理
	CObject_2D::Update();
}

//カーラーアニメーションの設定処理
void CAnimateUI::AnimateColor(const bool bAnim)
{
	m_bAnimateColor = bAnim;				
}



//=============================================================================
//
//								静的関数
//
//=============================================================================



//生成処理
CAnimateUI* CAnimateUI::Create(CObject::TextType texture, const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const D3DXCOLOR col, UIAnimation animInfo)
{
	CAnimateUI* pUi = new CAnimateUI;				//UIの生成

	if (FAILED(pUi->Init()))
	{//初期化処理
		return nullptr;
	}

	pUi->SetTexture(texture);						//テクスチャの設定
	pUi->SetPos(pos);								//位置の設定
	pUi->SetSize(size);								//サイズの設定
	pUi->SetColor(col);								//カーラーの設定
	pUi->SetPriority(5);							//プライオリティの設定
	pUi->m_AnimInfo = animInfo;						//アニメーション情報の設定

	return pUi;										//生成したインスタンスを返す
}