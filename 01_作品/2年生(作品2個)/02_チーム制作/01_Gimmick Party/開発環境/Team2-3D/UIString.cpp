//=============================================================================
//
// UIString.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "UIString.h"
#include "Letter.h"

//コンストラクタ
CUIString::CUIString()
{
	//メンバー変数をクリアする
	m_pTop = nullptr;				//先頭の文字へのポインタ
}

CUIString::CUIString(const int nPriority) : CObject(nPriority)
{
	//メンバー変数をクリアする
	m_pTop = nullptr;				//先頭の文字へのポインタ
}

//デストラクタ
CUIString::~CUIString()
{
	
}

//初期化処理
HRESULT CUIString::Init(void)
{
	//初期化処理
	m_pTop = nullptr;				//先頭の文字へのポインタ

	return S_OK;
}

//終了処理
void CUIString::Uninit(void)
{
	//全部の文字を破棄する
	ReleaseLetters();
}

//更新処理
void CUIString::Update(void)
{
	
}

//描画処理
void CUIString::Draw(void)
{

}

//文字の変更処理
void CUIString::ChangeString(const char* pString)
{
	CLetter* pLetter = m_pTop;
	int nCnt = 0;

	while (pLetter != nullptr)
	{
		pLetter->Convert(pString[nCnt]);
		pLetter = pLetter->GetNextLetter();
		nCnt++;
	}
}

//文字の色の設定処理
void CUIString::ChangeColor(const D3DXCOLOR col)
{
	CLetter* pLetter = m_pTop;
	int nCnt = 0;

	while (pLetter != nullptr)
	{
		pLetter->SetColor(col);
		pLetter = pLetter->GetNextLetter();
		nCnt++;
	}
}

//位置の設定処理
void CUIString::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//サイズの取得処理
const D3DXVECTOR2 CUIString::GetSize(void)
{
	return m_size;
}

//位置の取得処理
const D3DXVECTOR3 CUIString::GetPos(void)
{
	return m_pos;
}

//破棄処理
void CUIString::ReleaseLetters(void)
{
	if (m_pTop != nullptr)
	{
		CLetter* pCurrent = m_pTop;
		CLetter* pNext = m_pTop->GetNextLetter();

		while (pCurrent != nullptr)
		{
			pNext = pCurrent->GetNextLetter();
			pCurrent->Release();
			pCurrent = pNext;
		}
	}
}



//=============================================================================
//								静的関数
//=============================================================================




//生成処理
CUIString* CUIString::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const D3DXCOLOR col, const char* pString)
{
	CUIString* pStr = new CUIString;					//インスタンスを生成する

	if (FAILED(pStr->Init()))
	{//初期化処理
		return nullptr;
	}

	//文字のサイズを計算する
	D3DXVECTOR2 letterSize = Vec2Null;			
	int nLenght = strlen(pString);				
	letterSize.x = size.x / nLenght;			
	letterSize.y = size.y;						

	//先頭の文字を生成する
	pStr->m_pTop = CLetter::Create(D3DXVECTOR3(pos.x + letterSize.x * 0.5f, pos.y, 0.0f), letterSize, pString[0], 3);
	CLetter* pLetter = nullptr;

	if (pStr->m_pTop != nullptr)
	{//最初の文字が生成できたら

		CLetter* pCurrent = pStr->m_pTop;

		for (int nCnt = 1; nCnt < nLenght; nCnt++)
		{//他の文字を生成する
			if (pString[nCnt] != ' ')
			{
				//文字の生成処理
				pLetter = CLetter::Create(D3DXVECTOR3(pos.x + letterSize.x * 0.5f + letterSize.x * nCnt, pos.y, 0.0f), letterSize, pString[nCnt], 3);

				if (pLetter != nullptr)
				{
					pCurrent->SetNextLetter(pLetter);			//前の文字の設定
					pLetter->SetPrevLetter(pCurrent);			//次の文字の設定
					pCurrent = pLetter;							//現在の文字を更新する
					pLetter = nullptr;							//ポインタをnullにする
				}
			}
		}
	}

	if (pStr->m_pTop != nullptr)
	{//最初の文字が生成出来たら

		pLetter = pStr->m_pTop;

		while (pLetter != nullptr)
		{//全部の文字の色の設定
			pLetter->SetColor(col);
			pLetter = pLetter->GetNextLetter();
		}
	}

	return pStr;					//生成したインスタンスを返す
}

CUIString* CUIString::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const D3DXCOLOR col, const char* pString, const int nPriority)
{
	CUIString* pStr = new CUIString(nPriority);				//インスタンスを生成する

	if (FAILED(pStr->Init()))
	{//初期化処理
		return nullptr;
	}

	//文字のサイズを計算する
	D3DXVECTOR2 letterSize = Vec2Null;
	int nLenght = strlen(pString);
	letterSize.x = size.x / nLenght;
	letterSize.y = size.y;

	//先頭の文字を生成する
	pStr->m_pTop = CLetter::Create(D3DXVECTOR3(pos.x + letterSize.x * 0.5f, pos.y, 0.0f), letterSize, pString[0], nPriority);
	CLetter* pLetter = nullptr;

	if (pStr->m_pTop != nullptr)
	{//最初の文字が生成できたら
		CLetter* pCurrent = pStr->m_pTop;

		for (int nCnt = 1; nCnt < nLenght; nCnt++)
		{//他の文字を生成する
			if (pString[nCnt] != ' ')
			{
				//文字の生成処理
				pLetter = CLetter::Create(D3DXVECTOR3(pos.x + letterSize.x * 0.5f + letterSize.x * nCnt, pos.y, 0.0f), letterSize, pString[nCnt], nPriority);

				if (pLetter != nullptr)
				{
					pCurrent->SetNextLetter(pLetter);				//前の文字の設定
					pLetter->SetPrevLetter(pCurrent);				//次の文字の設定
					pCurrent = pLetter;								//現在の文字を更新する
					pLetter = nullptr;								//ポインタをnullにする
				}
			}
		}
	}

	if (pStr->m_pTop != nullptr)
	{//最初の文字が生成出来たら
		pLetter = pStr->m_pTop;

		while (pLetter != nullptr)
		{//全部の文字の色の設定
			pLetter->SetColor(col);
			pLetter = pLetter->GetNextLetter();
		}
	}

	return pStr;					//生成したインスタンスを返す
}