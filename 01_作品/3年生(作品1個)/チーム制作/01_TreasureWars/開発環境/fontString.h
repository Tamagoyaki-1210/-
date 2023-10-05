//=============================================================================
//
// fontString.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _FONTSTRING_H_
#define _FONTSTRING_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "font.h"
#include <vector>
#include <string>

//---------------------------
// �N���X�錾
//---------------------------
class CFontString : public CObject
{
public:
	CFontString();
	~CFontString() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SizeScale();				// �����̊g�又��
	void BlinkColor();
	void BlinkFinish();

	void SetFont();

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; SetFont(); }
	void SetAnchor(const D3DXVECTOR3 anchor) { m_anchor = anchor; }
	void SetSize(const D3DXVECTOR3 size) { m_size = size; SetFont(); }
	void SetColor(const D3DXCOLOR col) { for (auto pFont : m_apFont) pFont->SetCol(col); }
	void SetAlpha(const float alpha) { for (auto pFont : m_apFont) pFont->SetAlpha(alpha); }
	void SetFontDraw(const bool draw) { for (auto pFont : m_apFont) pFont->SetDrawFlag(draw); }
	void SetScale(const bool scale);
	void SetBlink(const float speed) { m_bBlink = true; m_fBlinkSpeed = speed; }
	void SetString(const std::string letter, const int priority = 4);

	std::vector <CFont*> GetFontAll() { return m_apFont; }

	static CFontString* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const std::string letter, const int priority = 4);
private:

	const float ScaleSpeed = 0.4f;	// �g�呬�x
	const float ScaleSize = 5.0f;		// �g��T�C�Y
	const float Anchor_Damping = 0.1f;	// �A���J�[�̌������x
	const float Mimi_Blink = 0.5f;		// �_�ł̍ő�k���T�C�Y

	float m_fScale;					// �����̑傫��
	bool m_bScale;					// �����̊g�唻��
	bool m_bScaleChange;			// �����̊g��k���̕ύX
	bool m_bBlink;					// �_�Ŕ���
	bool m_bBlinkScale;				// �_�ł̑�������(false:����, true:����)
	float m_fBlinkSpeed;			// �_�ő��x
	float m_fBlinkAlpha;			// �����S�̂̃A���t�@�l
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_anchor;			// �A���J�[�ʒu
	D3DXVECTOR3 m_size;
	D3DXVECTOR3 m_sizeYuan;			// �����̌��̑傫��
	std::vector <CFont*> m_apFont;	// �����̃|�C���^
};

#endif // !_FONTSTRING_H_
