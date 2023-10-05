//=============================================================================
//
// fontString.cpp
// Author : tanimoto kosuke
//
//=============================================================================
#include "fontString.h"
#include "Application.h"

static const int ScaleSpeed = 4;		// 拡大速度

//=====================================
// デフォルトコンストラクタ
//=====================================
CFontString::CFontString()
{

}

//=====================================
// デストラクタ
//=====================================
CFontString::~CFontString()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CFontString::Init(void)
{
	m_nNum = 0;				// 文字数の初期化
	m_fScale = 0.0f;		// 文字の大きさ初期化
	m_bScale = false;		// 文字の拡大判定初期化
	m_bSellect = false;		// 文字の選択判定初期化
	
	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CFontString::Uninit(void)
{
	for (int nCnt = 0; nCnt < MaxWordLenght; nCnt++)
	{// 文字列の終了処理
		if (m_pFont[nCnt] != nullptr)
		{
			m_pFont[nCnt]->Release();
			m_pFont[nCnt] = nullptr;
		}
	}
}

//=====================================
// 更新処理
//=====================================
void CFontString::Update(void)
{
	SizeScale();	// 文字の拡大処理
}

//=====================================
// サイズ変更処理
//=====================================
void CFontString::SizeScale(void)
{
	// 現在その選択肢が選択されている場合
	if (m_bSellect == true)
	{
		if (m_bScale == false)
		{// 文字が拡大している場合
			m_fScale += ScaleSpeed / 10.0f;
			if (m_fScale > 5.0f)
			{// 文字が一定の値まで拡大した場合
				m_bScale = true;	// 文字を縮小させる
			}
		}
		else
		{// 文字が縮小している場合
			m_fScale -= ScaleSpeed / 10.0f;
			if (m_fScale < -5.0f)
			{// 文字が一定の値まで縮小した場合
				m_bScale = false;	// 文字を拡大させる
			}
		}

		for (int nCnt = 0; nCnt < m_nNum; nCnt++)
		{// 文字の大きさ設定処理
			m_pFont[nCnt]->SetSize(D3DXVECTOR2(m_sizeYuan.x + m_fScale, m_sizeYuan.y + m_fScale));
		}
	}
}

//=====================================
// サイズリセット処理
//=====================================
void CFontString::SizeReset(void)
{
	m_bSellect = false;	// 文字選択判定の初期化
	m_fScale = 0.0f;	// 文字の大きさの初期化
	m_bScale = false;	// 文字の拡大処理の初期化

	for (int nCnt = 0; nCnt < m_nNum; nCnt++)
	{// 文字の大きさ設定処理
		m_pFont[nCnt]->SetSize(D3DXVECTOR2(m_sizeYuan.x + m_fScale, m_sizeYuan.y + m_fScale));
	}
}

//=====================================
// 色の設定処理
//=====================================
void CFontString::SetColor(D3DXCOLOR col)
{
	for (int nCnt = 0; nCnt < m_nNum; nCnt++)
	{// 文字の色設定処理
		m_pFont[nCnt]->SetColor(col);
	}
}

//=====================================
// 現在番号の設定処理
//=====================================
void CFontString::SetSellect(void)
{
	m_bSellect = true;
}

//=====================================
// 生成処理
//=====================================
CFontString* CFontString::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const std::string letter)
{
	CFontString* pFontString = new CFontString;
	
	if (FAILED(pFontString->Init()))
	{
		return nullptr;
	}

	for (int nCnt = 0; nCnt < MaxWordLenght; nCnt++)
	{// 文字の初期化
		pFontString->m_pFont[nCnt] = nullptr;
	}

	// string型の文字数を取得
	int nTex = strlen(letter.c_str());

	float maxSizeX = size.x * 2;	// サイズ2個分の間隔を空ける
	for (int nLatter = 0; nLatter < nTex; nLatter += 2)
	{
		// 現在位置から1文字進む処理
		std::string sLatter = letter.substr(nLatter, 2);

		// 文字を中央に配置し、string型から*char型を1文字ずつ取り出す
		if (nTex / 2 % 2 == 1)
		{
			pFontString->m_pFont[pFontString->m_nNum] = CFont::Create(D3DXVECTOR3(pos.x - (maxSizeX * (nTex / 4)) + ((maxSizeX * pFontString->m_nNum)), pos.y, pos.z), size, sLatter.c_str());
		}
		else
		{
			pFontString->m_pFont[pFontString->m_nNum] = CFont::Create(D3DXVECTOR3(pos.x - (maxSizeX * ((nTex / 4) - 1)) + (maxSizeX * pFontString->m_nNum) - (maxSizeX / 2), pos.y, pos.z), size, sLatter.c_str());
		}

		// 最初のサイズを登録する
		pFontString->m_sizeYuan = size;

		// 番号を割り振る
		pFontString->m_nNum++;
	}

	return pFontString;
}