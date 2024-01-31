//=============================================================================
//
// font.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
//=============================================================================
//インクルードファイル
//=============================================================================
#include "font.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CFont::CFont(const PRIORITY priority) : CObject2D(priority)
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
HRESULT CFont::Init()
{
	// 基本クラスの初期化処理
	if (FAILED(CObject2D::Init()))
	{
		return -1;
	}

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CFont::Uninit()
{
	// 基本クラスの終了処理
	CObject2D::Uninit();
}

//=====================================
// 更新処理
//=====================================
void CFont::Update()
{
	// 基本クラスの更新処理
	CObject2D::Update();
}

//=====================================
// 描画処理
//=====================================
void CFont::Draw()
{
	// 基本クラスの描画処理
	CObject2D::Draw();
}

//=====================================
// 文字を設定
//=====================================
void CFont::SetString(const char* letter, const D3DXCOLOR col)
{
	// 日本語の場合
	if (letter[0] <= -125 && letter[0] >= -127)
	{
		SetTexture(CTexture::TEXTURE_FONT_JAPANESE_VER2);		//テクスチャの設定
		SetAnimDiv(Japanese_Font_Column, Japanese_Font_Line);

		// アニメーションパターンの設定
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
				Release();
				return;
			}
			else
			{
				SetAnimControl(Cell);	// 画像の指定された文字の部分のみを切り取り
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
					Release();
					return;
				}
				else
				{
					SetAnimControl(Cell);	// 画像の指定された文字の部分のみを切り取り
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
					Release();
					return;
				}
				else
				{
					SetAnimControl(Cell);	// 画像の指定された文字の部分のみを切り取り
				}
			}
		}
		// 記号(-127)と配列[0]が一致する場合
		else if (letter[0] == -127)
		{
			if (letter[1] == -127)
			{
				SetAnimControl(95);	// ＝
			}
			else if (letter[1] == 64)
			{
				SetAnimControl(199);	// 全角スペース
			}
			else if (letter[1] == 69)
			{
				SetAnimControl(186);	// ・
			}
			else if (letter[1] == 91)
			{
				SetAnimControl(187);	// ー
			}
			else if (letter[1] == 117)
			{
				SetAnimControl(191);	// 「
			}
			else if (letter[1] == 118)
			{
				SetAnimControl(192);	// 」
			}
		}
	}
	else
	{
		char aLetter = 33;				//Unicodeの!(10進数)

		SetTexture(CTexture::TEXTURE_FONT_ALPHABET);		//テクスチャの設定
		SetAnimDiv(Alphabet_Font_Column, Alphabet_Font_Line);

		//アニメーションパターンの設定
		int Cell = 0;

		for (int nCnt = 0; nCnt < 93; nCnt++)
		{//~まで確認する
			if (letter[0] == aLetter)
			{//同じだったら、番号を保存して、止まる
				Cell = nCnt;
				break;
			}

			aLetter += 1;
		}

		if (Cell < 0 || Cell > 93)
		{//範囲外の文字だったら、falseを返す
			Release();
			return;
		}
		else
		{//変換出来たら、アニメーションパターンを設定して、trueを返す
			SetAnimControl(Cell);
		}
	}

	// 色の設定
	SetCol(col);
}

//=====================================
// 生成処理
//=====================================
CFont* CFont::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const char* letter, const PRIORITY priority, const D3DXCOLOR col)
{
	CFont* pFont = new CFont(priority);

	// 初期化処理
	if (FAILED(pFont->Init()))
	{
		return nullptr;
	}

	pFont->SetPos(pos);
	pFont->SetSize(size);

	// 文字の設定
	pFont->SetString(letter, col);

	return pFont;
}