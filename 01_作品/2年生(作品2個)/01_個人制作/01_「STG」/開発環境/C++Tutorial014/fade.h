//=============================================================================
//
// �t�F�[�h.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�N���X
//---------------------------
class CFade : public CObject2D
{
public:
	enum FADE
	{
		FADE_NONE = 0,	// �������Ă��Ȃ����
		FADE_OUT,		// �t�F�[�h�A�E�g(�����Ȃ����)
		FADE_IN,		// �t�F�[�h�C��(��������)
		FADE_CHANGE,	// �t�F�[�h�؂�ւ�
		FADE_MAX
	};

	CFade();
	~CFade() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void StartFadeOut();

	static CFade *Create(float fAlpha, FADE fade);

	float GetAlpha() { return m_fAlpha; }
	FADE GetFade() { return m_fade; }	//�t�F�[�h�擾����
private:
	float m_fAlpha;		// �|���S��(�t�F�[�h)��a�l
	FADE m_fade;		// ���݂̃t�F�[�h
};

#endif // !_PLAYER_H_