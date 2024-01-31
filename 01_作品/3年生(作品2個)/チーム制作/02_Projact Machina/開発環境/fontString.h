//=============================================================================
//
// fontString.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _FONTSTRING_H_
#define _FONTSTRING_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "font.h"
#include <vector>
#include <string>

//---------------------------
// クラス宣言
//---------------------------
class CFontString : public CObject
{
	static const float SCALE_SPEED;		// 拡大速度
	static const float SCALE_SIZE;		// 拡大サイズ
	static const float ANCHOR_DAMPING;	// アンカーの減衰速度
	static const float MIMI_BLINK;		// 点滅の最大縮小サイズ

public:
	CFontString();
	~CFontString() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SizeScale();				// 文字の拡大処理
	void BlinkColor();
	void BlinkFinish();

	void SetFont();

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; SetFont(); }
	void SetAnchor(const D3DXVECTOR3 anchor) { m_anchor = anchor; }
	void SetSize(const D3DXVECTOR2 size) { m_size = size; SetFont(); }
	void SetColor(const D3DXCOLOR col) { for (auto pFont : m_apFont) pFont->SetCol(col); }
	void SetAlpha(const float alpha) { for (auto pFont : m_apFont) pFont->SetAlpha(alpha); }
	void SetFontDraw(const bool draw) { for (auto pFont : m_apFont) pFont->SetDrawFlag(draw); }
	void SetScale(const bool scale);
	void SetBlink(const float speed) { m_bBlink = true; m_fBlinkSpeed = speed; }
	void SetString(const std::string letter, const PRIORITY priority = PRIORITY_SCREEN);

	std::vector <CFont*> GetFontAll() { return m_apFont; }

	static CFontString* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const std::string letter, const PRIORITY priority = PRIORITY_SCREEN);
private:

	float m_fScale;					// 文字の大きさ
	bool m_bScale;					// 文字の拡大判定
	bool m_bScaleChange;			// 文字の拡大縮小の変更
	bool m_bBlink;					// 点滅判定
	bool m_bBlinkScale;				// 点滅の増減判定(false:増加, true:減衰)
	float m_fBlinkSpeed;			// 点滅速度
	float m_fBlinkAlpha;			// 文字全体のアルファ値
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_anchor;			// アンカー位置
	D3DXVECTOR2 m_size;
	D3DXVECTOR2 m_sizeYuan;			// 文字の元の大きさ
	std::vector <CFont*> m_apFont;	// 文字のポインタ
};

#endif // !_FONTSTRING_H_