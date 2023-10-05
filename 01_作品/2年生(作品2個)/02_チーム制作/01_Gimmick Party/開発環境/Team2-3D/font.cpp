//=============================================================================
//
// font.cpp
// Author : tanimoto kosuke
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "font.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CFont::CFont()
{

}

CFont::CFont(const int nPriority) : CObject_2D::CObject_2D(nPriority)
{

}

//=====================================
// デストラクタ
//=====================================
CFont::~CFont()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CFont::Init(void)
{
	//基本クラスの初期化処理
	if (FAILED(CObject_2D::Init()))
	{
		return -1;
	}

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CFont::Uninit(void)
{
	//基本クラスの終了処理
	CObject_2D::Uninit();
}

//=====================================
// 更新処理
//=====================================
void CFont::Update(void)
{
	//基本クラスの更新処理
	CObject_2D::Update();
}

//=====================================
// 描画処理
//=====================================
void CFont::Draw(void)
{
	//基本クラスの描画処理
	CObject_2D::Draw();
}

//=====================================
// 生成処理
//=====================================
CFont* CFont::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const char* letter)
{
	CFont* pFont = new CFont(5);		//生成(Priority = 5)

	//初期化処理
	if (FAILED(pFont->Init()))
	{
		return nullptr;
	}

	pFont->SetPos(pos);									//位置の設定
	pFont->SetSize(size);								//サイズの設定

	pFont->SetTexture(CObject::TEXTURE_JAPANESE);		//テクスチャの設定
	pFont->SetTextureParameter(1, 20, 10, INT_MAX);		//テクスチャパラメータの設定

	//アニメーションパターンの設定
	int Cell = 0;

	// ひらがな(-126)と配列[0]が一致する場合
	if (letter[0] == -126)
	{
		int aLetter = -97;	// 最初の文字が始まる位置(-97)

		for (int nCnt = 0; nCnt < 83; nCnt++)
		{// 画像のひらがな83文字から1つを切り取る
			if (letter[1] == aLetter + nCnt)
			{// 入力された文字と現在の文字が一致した場合
				Cell = nCnt;	// 文字の位置を代入
				break;
			}
		}

		// "ん"まで対応
		if (Cell < 0 || Cell > 83)
		{
			pFont->Release();
			return nullptr;
		}
		else
		{
			pFont->SetAnimPattern(Cell);	// 画像の指定された文字の部分のみを切り取り
		}
	}
	// カタカナ(-125)と配列[0]が一致する場合
	else if (letter[0] == -125)
	{
		// "ミ"までのカタカナ
		if (letter[1] >= 64)
		{
			int aLetter = 64;	// 最初の文字が始まる位置(64)
			int aKatakana = 96;	// カタカナが始まる位置(96)

			for (int nCnt = 0; nCnt < 63; nCnt++)
			{// 画像のカタカナ63文字から1つを切り取る
				if (letter[1] == aLetter + nCnt)
				{// 入力された文字と現在の文字が一致した場合
					Cell = nCnt + aKatakana;	// 文字の位置を代入
					break;
				}
			}

			if (Cell < aKatakana || Cell > aKatakana + 63)
			{// 画像のカタカナ63文字から1つを切り取る
				pFont->Release();
				return nullptr;
			}
			else
			{
				pFont->SetAnimPattern(Cell);	// 画像の指定された文字の部分のみを切り取り
			}
		}
		// "ム"以降のカタカナ
		else
		{
			int aLetter = -128;		// 最初の文字が始まる位置(-128)
			int aKatakana = 159;	// カタカナが始まる位置(159)

			for (int nCnt = 0; nCnt < 23; nCnt++)
			{// 画像のカタカナ23文字から1つを切り取る
				if (letter[1] == aLetter + nCnt)
				{// 入力された文字と現在の文字が一致した場合
					Cell = nCnt + aKatakana;	// 文字の位置を代入
					break;
				}
			}

			if (Cell < aKatakana || Cell > aKatakana + 23)
			{// 画像のカタカナ23文字から1つを切り取る
				pFont->Release();
				return nullptr;
			}
			else
			{
				pFont->SetAnimPattern(Cell);	// 画像の指定された文字の部分のみを切り取り
			}
		}
	}
	// 記号(-127)と配列[0]が一致する場合
	else if (letter[0] == -127)
	{
		if (letter[1] == -127)
		{
			pFont->SetAnimPattern(95);	//＝
		}
		else if (letter[1] == 69)
		{
			pFont->SetAnimPattern(186);	//・
		}
		else if (letter[1] == 91)
		{
			pFont->SetAnimPattern(187);	//ー
		}
	}

	return pFont;
}