//=============================================================================
//
// font.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _FONT_H
#define _FONT_H

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "object2D.h"

//---------------------------
// �N���X�錾
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

	void SetString(const char* letter, const D3DXCOLOR col = { 1.0f, 1.0f, 1.0f, 1.0f });	// ������ݒ�

private:
	static const int Japanese_Font_Column = 20;	// ���{��t�H���g�摜�̉��敪
	static const int Japanese_Font_Line = 10;	// ���{��t�H���g�摜�̏c�敪

	static const int Alphabet_Font_Column = 10;	// �A���t�@�x�b�g�t�H���g�摜�̉��敪
	static const int Alphabet_Font_Line = 10;	// �A���t�@�x�b�g�t�H���g�摜�̏c�敪
};

#endif // !_FONT_H
