//=============================================================================
//
// font.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _FONT_H
#define _FONT_H

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object2D.h"

//---------------------------
// クラス宣言
//---------------------------
class CFont : public CObject2D
{
public:
	CFont(const PRIORITY priority);
	~CFont() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CFont* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const char* letter, const PRIORITY priority = PRIORITY_FRONT, const D3DXCOLOR col = {1.0f, 1.0f, 1.0f, 1.0f});

	void SetString(const char* letter, const D3DXCOLOR col = { 1.0f, 1.0f, 1.0f, 1.0f });	// 文字を設定

private:
	static const int Japanese_Font_Column = 20;	// 日本語フォント画像の横区分
	static const int Japanese_Font_Line = 10;	// 日本語フォント画像の縦区分

	static const int Alphabet_Font_Column = 10;	// アルファベットフォント画像の横区分
	static const int Alphabet_Font_Line = 10;	// アルファベットフォント画像の縦区分
};

#endif // !_FONT_H
