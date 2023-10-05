//==============================================================================================
//
// �t�F�[�h�@�@�@fade.h
// tutida ryousei
//
//==============================================================================================
#ifndef _FADE_H_
#define	_FADE_H_

#include"object2D.h"
#include"application.h"

class CFade
{
public:
	//�t�F�[�h�̏��
	enum FADE
	{
		FADE_NONE = 0,		//�������ĂȂ����
		FADE_IN,			//�t�F�[�h�C��
		FADE_OUT,			//�t�F�[�h�A�E�g
		FADE_MAX
	};

	CFade();
	~CFade();
	 
	HRESULT Init();
	void Uninit();
	void Update();

	static CFade *CFade::Create();
	static void SetFade(CApplication::MODE mode, float fadespeed);

	//EObjType GetObjType() override { return OBJTYPE_MODE; }
	static FADE GetFade() { return FadeMode; }

private:
	D3DXVECTOR3 m_Pos;					// �ʒu
	float m_fAlpha;						// �A���t�@
	static bool m_bCreate;				// ����������
	static float m_fFadeSpeed;			// �t�F�[�h�̑��x
	static FADE FadeMode;				// �t�F�[�h�̏��
	static CApplication::MODE NextMode;	// ���̉��
	static CObject2D* m_pFadeObj;				// �t�F�[�h�p��2D�I�u�W�F�N�g

};

#endif // !_FADE_H_
