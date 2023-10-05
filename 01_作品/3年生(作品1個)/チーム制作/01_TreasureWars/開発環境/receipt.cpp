//=============================================================================
//
// receipt.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "receipt.h"
#include "application.h"
#include "renderer.h"
#include "object2D.h"
#include "fontString.h"

const D3DXVECTOR3 CReceipt::Font_Size = { 12.0f, 12.0f, 0.0f };
const float CReceipt::FontString_Blink_Speed = 0.02f;
//=====================================
// デフォルトコンストラクタ
//=====================================
CReceipt::CReceipt()
{

}

//=====================================
// デストラクタ
//=====================================
CReceipt::~CReceipt()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CReceipt::Init()
{
	m_nTotalValue = 0;

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CReceipt::Uninit()
{
	for (int nCnt = 0; nCnt < (int)m_apFontString.size(); nCnt++)
	{
		m_apFontString[nCnt]->Uninit();
		m_apFontString[nCnt] = nullptr;
	}

	m_apFontString.clear();

	Release();
}

//============================================================================
// 更新処理
//============================================================================
void CReceipt::Update()
{
	// タイマーが使用されている場合
	if (m_nStringDesthCounter > 0)
	{
		m_nStringDesthCounter--;

		// 消えそうになったら
		if (m_nStringDesthCounter <= String_Death_Timer)
		{
			// 点滅を終了させる
			m_apFontString[0]->BlinkFinish();

			// 一番下の文字をじわじわと消す処理
			m_apFontString[0]->SetAlpha(0.6f * m_nStringDesthCounter / String_Death_Timer);

			// 消える時間に達した場合
			if (m_nStringDesthCounter <= 0)
			{
				// 消す
				m_nStringDesthCounter = 0;
				DestroyString();
			}
		}
	}
}

//============================================================================
// 描画処理
//============================================================================
void CReceipt::Draw()
{

}

//============================================================================
// レシートの合計値の処理
//============================================================================
void CReceipt::Total()
{
	if (m_pTotal == nullptr)
	{
		m_pTotal = CFontString::Create({ m_pos.x - 15.0f , m_pos.y - 30.0f , m_pos.z }, {15.0f, 15.0f, 0.0f}, "<c=255,215,0,255,>ごうけい</>　$<c=255,215,0,255,>" + std::to_string(m_nTotalValue) + "</>");
	}
	else
	{
		m_pTotal->SetString("<c=255,215,0,255,>ごうけい</>　$<c=255,215,0,255,>" + std::to_string(m_nTotalValue) + "</>");
	}
}

//============================================================================
// 文字列の破棄処理
//============================================================================
void CReceipt::DestroyString()
{
	// 文字列が使用されている場合
	if (!m_apFontString.empty())
	{
		// 一番上の行を削除する
		m_apFontString.front()->Uninit();
		m_apFontString.erase(m_apFontString.begin());

		// 文字列が残っている場合
		if (!m_apFontString.empty())
		{
			// 文字列のアンカーポイントの設定処理
			SetStringAnchor();

			m_nStringDesthCounter = String_Death_Timer;
		}
		// 文字列が全て消えた場合
		else
		{
			// 合計を消す
			m_pTotal->Uninit();
			m_pTotal = nullptr;
			m_nTotalValue = 0;
		}
	}
}

//============================================================================
// 文字列の設定処理
//============================================================================
void CReceipt::SetString(const std::string letter)
{
	// 最大行数に達している場合
	if ((int)m_apFontString.size() >= Max_Row)
	{
		// 一番後ろの行を削除する
		m_apFontString.front()->Uninit();
		m_apFontString.erase(m_apFontString.begin());
	}

	// 初期位置を変える処理
	D3DXVECTOR3 sideFirstPos = { -(Font_Size.x * 2.0f * Max_Len), Font_Size.y * 2.0f * m_apFontString.size(), 0.0f };

	if (m_bRight)
	{
		sideFirstPos.x *= -1.0f;
	}

	// 文字列の生成して格納する
	m_apFontString.push_back(CFontString::Create({ m_pos.x + sideFirstPos.x, m_pos.y + sideFirstPos.y, m_pos.z }, Font_Size, letter));

	// 文字列のアンカーポイントの設定処理
	SetStringAnchor();

	// 点滅を開始する
	m_apFontString[m_apFontString.size() - 1]->SetBlink(FontString_Blink_Speed);

	// 文字列の消えるタイミングを設定する
	m_nStringDesthCounter = Setting_String_Death_Timer;
}

//============================================================================
// 文字列のアンカーポイントの設定処理
//============================================================================
void CReceipt::SetStringAnchor()
{
	// 現在の最大行数
	int stringSize = (int)m_apFontString.size();

	// 全文字列の位置設定
	for (int nCnt = 0; nCnt < stringSize; nCnt++)
	{
		float fAnchorBottom = Font_Size.y * 2.0f * nCnt;

		// アンカー位置を設定する
		m_apFontString[nCnt]->SetAnchor(D3DXVECTOR3( m_pos.x, m_pos.y + fAnchorBottom, m_pos.z ));

		// 点滅を終了させる
		m_apFontString[nCnt]->BlinkFinish();
	}
	// 2個まで配列が生成されている場合
	if (stringSize >= 1)
	{
		if (stringSize >= 2)
		{
			m_apFontString[1]->SetAlpha(0.8f);
			m_apFontString[0]->SetAlpha(0.6f);
		}
		else
		{
			m_apFontString[0]->SetAlpha(0.8f);
		}
	}
	Total();
}

//============================================================================
// 生成処理
//============================================================================
CReceipt* CReceipt::Create(const int index)
{
	CReceipt* pReceipt = new CReceipt;

	if (FAILED(pReceipt->Init()))
	{
		return nullptr;
	}

	switch (index)
	{
	case 0:
		pReceipt->m_pos = { 20.0f, 170.0f, 0.0f };
		break;
	case 1:
		pReceipt->m_pos = { 1240.0f - ((pReceipt->Font_Size.x + 5.0f) * pReceipt->Max_Len), 170.0f, 0.0f };
		break;
	case 2:
		pReceipt->m_pos = { 20.0f, 450.0f, 0.0f };
		break;
	case 3:
		pReceipt->m_pos = { 1240.0f - ((pReceipt->Font_Size.x + 5.0f) * pReceipt->Max_Len), 450.0f, 0.0f };
		break;
	default:
		break;
	}

	// 生成位置が画面半分より右側だった場合
	pReceipt->m_bRight = (pReceipt->m_pos.x > (1280.0f * 0.5f)) ? true : false;

	return pReceipt;
}
