//=============================================================================
//
// menu_window.h (���j���[�E�B���h�E.h)
// Author : Saito Shian
//
//=============================================================================
#ifndef  _MENU_WINDOW_H_				//���̃}�N����`������ĂȂ�������
#define  _MENU_WINDOW_H_				//��d�C���N���[�h�h�~�̃}�N����`

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "object.h"

//=============================================================================
// �O���錾
//=============================================================================
class CObject2D;

//=============================================================================
// ���j���[�E�B���h�E�N���X
//=============================================================================
class CMenuWindow
{
	const float SizeXScaleSpeed = 31.8f;	// X�T�C�Y�g��k���̃X�s�[�h
	const float SizeYScaleSpeed = 27.5f;	// Y�T�C�Y�g��k���̃X�s�[�h
	const float SizeXScaleMax = 500.0f;		// X�T�C�Y�̍ő�T�C�Y
	const float SizeYScaleMax = 220.0f;		// Y�T�C�Y�̍ő�T�C�Y

public:
	CMenuWindow();							// ���j���[�E�B���h�E�̃R���X�g���N�^
	~CMenuWindow();							// ���j���[�E�B���h�E�̃f�X�g���N�^

	HRESULT Init();							// ����������
	void Uninit();							// �I������
	void Update();							// �X�V����

	void MenuWindowScale();					// ���j���[�̃T�C�Y����
	bool MenuWindowScaleReduce();			// ���j���[�̃T�C�Y�k������

	static CMenuWindow* Create();			// ����

private:
	CObject2D* m_pMenuWindow;				// ���j���[�E�B���h�E�pobject2D�̃C���X�^���X
	D3DXVECTOR2 m_MenuWindowSize;			// ���j���[�E�B���h�E�̃T�C�Y

	bool m_bMenuWindowMaxSize;				// �ő�T�C�Y�܂ōs�����̂�
	bool m_bMenuWindowScaleFlag;			// �g��k���t���O
};
#endif

