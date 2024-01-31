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
	static const float SCALE_SPEED;		// �g�呬�x
	static const float SCALE_SIZE;		// �g��T�C�Y
	static const float ANCHOR_DAMPING;	// �A���J�[�̌������x
	static const float MIMI_BLINK;		// �_�ł̍ő�k���T�C�Y

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

	float m_fScale;					// �����̑傫��
	bool m_bScale;					// �����̊g�唻��
	bool m_bScaleChange;			// �����̊g��k���̕ύX
	bool m_bBlink;					// �_�Ŕ���
	bool m_bBlinkScale;				// �_�ł̑�������(false:����, true:����)
	float m_fBlinkSpeed;			// �_�ő��x
	float m_fBlinkAlpha;			// �����S�̂̃A���t�@�l
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_anchor;			// �A���J�[�ʒu
	D3DXVECTOR2 m_size;
	D3DXVECTOR2 m_sizeYuan;			// �����̌��̑傫��
	std::vector <CFont*> m_apFont;	// �����̃|�C���^
};

#endif // !_FONTSTRING_H_