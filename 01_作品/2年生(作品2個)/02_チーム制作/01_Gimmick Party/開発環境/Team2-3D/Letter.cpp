//=============================================================================
//
// Letter.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "Letter.h"
#include <string>

//コンストラクタ
CLetter::CLetter()
{			
	m_pPreviousLetter = nullptr;			//前の文字
	m_pNextLetter = nullptr;				//次の文字
}

//コンストラクタ
CLetter::CLetter(const int nPriority) : CObject_2D::CObject_2D(nPriority)
{
	m_pPreviousLetter = nullptr;			//前の文字
	m_pNextLetter = nullptr;				//次の文字
}

//デストラクタ
CLetter::~CLetter()
{

}

//初期化処理
HRESULT CLetter::Init(void)
{
	//基本クラスの初期化処理
	if (FAILED(CObject_2D::Init()))
	{
		return -1;
	}

	m_pPreviousLetter = nullptr;		//前の文字
	m_pNextLetter = nullptr;			//次の文字

	return S_OK;
}

//終了処理
void CLetter::Uninit(void)
{
	//基本クラスの終了処理
	CObject_2D::Uninit();
}

//更新処理
void CLetter::Update(void)
{
	//基本クラスの更新処理
	CObject_2D::Update();
}

//終了処理
void CLetter::Draw(void)
{
	//基本クラスの描画処理
	CObject_2D::Draw();
}

//文字の変換処理
bool CLetter::Convert(const char symbol)
{
	char aLetter = 33;		//Unicodeの!(10進数)

	//アニメーションパターンの設定
	int Cell = 27;

	for (int nCnt = 0; nCnt < 93; nCnt++)
	{//~まで確認する
		if (symbol == aLetter)
		{//同じだったら、番号を保存して、止まる
			Cell = nCnt;
			break;
		}

		aLetter += 1;			
	}

	if (Cell < 0 || Cell > 93)
	{//範囲外の文字だったら、falseを返す
		return false;
	}
	else
	{//変換出来たら、アニメーションパターンを設定して、trueを返す
		SetAnimPattern(Cell);
		return true;
	}
}

//前の文字の設定処理
void CLetter::SetPrevLetter(CLetter* pPrev)
{
	m_pPreviousLetter = pPrev;
}

//次の文字の設定処理
void CLetter::SetNextLetter(CLetter* pNext)
{
	m_pNextLetter = pNext;
}

//前の文字の取得処理
CLetter* CLetter::GetPrevLetter(void)
{
	return m_pPreviousLetter;
}

//次の文字の設定処理
CLetter* CLetter::GetNextLetter(void)
{
	return m_pNextLetter;
}


//==================================================================================================================
//
//											静的関数
//
//==================================================================================================================

//生成処理
CLetter* CLetter::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const char letter)
{
	CLetter* pLetter = new CLetter(3);								//生成

	//初期化処理
	if (FAILED(pLetter->Init()))
	{
		return nullptr;
	}

	pLetter->SetPos(pos);									//位置の設定
	pLetter->SetSize(size);									//サイズの設定
															
	pLetter->SetTexture(CObject::TEXTURE_CHARACTERS);		//テクスチャの設定
	pLetter->SetTextureParameter(1, 10, 10, INT_MAX);		//テクスチャパラメータの設定

	char aLetter = 33;				//Unicodeの!(10進数)

	//アニメーションパターンの設定
	int Cell = 27;

	for (int nCnt = 0; nCnt < 93; nCnt++)
	{//~まで確認する
		if (letter == aLetter)
		{//同じだったら、番号を保存して、止まる
			Cell = nCnt;
			break;
		}

		aLetter += 1;
	}

	if (Cell < 0 || Cell > 93)
	{//範囲外の文字だったら、falseを返す
			pLetter->Release();
			return nullptr;
	}
	else
	{//変換出来たら、アニメーションパターンを設定して、trueを返す
		pLetter->SetAnimPattern(Cell);
	}

	return pLetter;				//生成したインスタンスを返す
}

CLetter* CLetter::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const char letter, const int nPriority)
{
	CLetter* pLetter = new CLetter(nPriority);						//生成

	//初期化処理
	if (FAILED(pLetter->Init()))
	{
		return nullptr;
	}
	
	pLetter->SetPos(pos);									//位置の設定
	pLetter->SetSize(size);									//サイズの設定
	//pLetter->SetStartingRot(D3DX_PI * 0.5f);				//回転角度の設定
	pLetter->SetTexture(CObject::TEXTURE_CHARACTERS);		//テクスチャの設定
	pLetter->SetTextureParameter(1, 10, 10, INT_MAX);		//テクスチャパラメータの設定

	char aLetter = 33;				//Unicodeの!(10進数)

	//アニメーションパターンの設定
	int Cell = 27;

	for (int nCnt = 0; nCnt < 93; nCnt++)
	{//~まで確認する
		if (letter == aLetter)
		{//同じだったら、番号を保存して、止まる
			Cell = nCnt;
			break;
		}

		aLetter += 1;
	}

	if (Cell < 0 || Cell > 93)
	{//範囲外の文字だったら、falseを返す
		pLetter->Release();
		return nullptr;
	}
	else
	{//変換出来たら、アニメーションパターンを設定して、trueを返す
		pLetter->SetAnimPattern(Cell);
	}

	return pLetter;				//生成したインスタンスを返す
} 