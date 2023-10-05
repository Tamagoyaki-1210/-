//=============================================================================
//
// ui.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//*****************************************************************************
// インクルード
//*****************************************************************************
#include "ui.h"
#include "Application.h"

std::vector<CUi*> CUi::m_AllUi = {};
//=====================================
// コンストラクタ
//=====================================
CUi::CUi() : m_bNoneDispray(false)
{
	m_AllUi.push_back(this);
}

//=====================================
// デストラクタ
//=====================================
CUi::~CUi()
{
	// デストラクタのタイミングで自身を配列から除外する
	m_AllUi.erase(std::find(m_AllUi.begin(), m_AllUi.end(), this));
}

//============================================================================
// 終了処理
//============================================================================
void CUi::Uninit()
{
	// 空になるまで終了処理
	while (!m_listObj2D.empty())
	{
		m_listObj2D.back()->Uninit();
		m_listObj2D.pop_back();
	}

	Release();
}

//============================================================================
// 更新処理
//============================================================================
void CUi::Update()
{
	//// 2Dオブジェクトを使用中の場合
	//if (!m_listObj2D.empty() && m_bNoneDispray)
	//{
	//	Dispray();	// 非表示にしたい場合の処理
	//}
}

//============================================================================
// UI用オブジェクト2Dの削除処理
//============================================================================
void CUi::DeathObj2D(CObject2D* obj2D)
{
	// 全てのUI用オブジェクト2Dの中の選んだポインタの処理をする
	m_listObj2D.erase(std::find(m_listObj2D.begin(), m_listObj2D.end(), obj2D));

	// オブジェクト2Dの終了処理
	obj2D->Uninit();
}

////============================================================================
//// UIの表示処理
////============================================================================
//void CUi::Dispray()
//{
//	// UIの更新がある場合
//	if (m_bUiUpdate)
//	{
//		SetAlpha(1.0f);
//		SetUiDraw(true);
//		if (m_nNoneDispray_Counter != 0)
//		{
//			m_nNoneDispray_Counter = 0;
//		}
//	}
//	else if (m_nNoneDispray_Counter < None_Dispray_Max)
//	{
//		m_nNoneDispray_Counter++;	// 非表示カウンターを進める
//
//		// 透過のカウンターまで進むと
//		if (m_nNoneDispray_Counter >= Invinsible_Dispray_Max)
//		{
//			// 透過処理(割合)
//			float fInvinsible = (float)(None_Dispray_Max - m_nNoneDispray_Counter) / (float)(None_Dispray_Max - Invinsible_Dispray_Max);
//			SetAlpha(fInvinsible);
//
//			// 非表示カウンターが最大数を超えた場合
//			if (m_nNoneDispray_Counter >= None_Dispray_Max)
//			{
//				SetUiDraw(false);
//			}
//		}
//	}
//
//	m_bUiUpdate = false;	// 読み込み終了後に更新を消す
//}

//============================================================================
// 強制的なUI表示設定処理
//============================================================================
void CUi::SetCompulsDispray(const bool display)
{
	// // 全ての2Dオブジェクト
	for (auto pObj2D : m_listObj2D)
	{
		// 2Dオブジェクトのどれかが描画されている場合はループを抜ける
		if (pObj2D->GetDrawFlag() == true)
		{
			break;
		}
		else
		{
			// 全ての2Dオブジェクトが描画されていない場合
			if (pObj2D == m_listObj2D.back())
			{
				// 全て描画する
				SetUiDraw(true);
			}
		}
	}

	// 表示させる場合
	if (display == true)
	{
		// UIを表示させる
		SetAlpha(1.0f);
		//m_nNoneDispray_Counter = 0;
	}
	// 表示させない場合
	else
	{
		// UIを消し始める
		//m_nNoneDispray_Counter = None_Dispray_Max;
		SetAlpha(0);
		SetUiDraw(false);
	}

	// UIのUVを設定する
	SetUiUV();
}