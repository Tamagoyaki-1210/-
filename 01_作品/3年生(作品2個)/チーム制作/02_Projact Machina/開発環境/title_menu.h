//=============================================================================
//
// �^�C�g�����j���[
// title_menu.h
// Author : Tanimoto Kosuke
// Author : Saito Shian
//
//=============================================================================
#ifndef _TITLE_MENU_H_
#define _TITLE_MENU_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "menu.h"
#include "object2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CConfirmation_Window;

//=============================================================================
// �^�C�g�����j���[�N���X�錾
//=============================================================================
class CTitleMenu : public CMenu
{
public:
	enum Choice_Type
	{
		Choice_GameStart,			// �Q�[���X�^�[�g
		Choice_Window_Exit,			// �E�B���h�E�����
		Choice_Max,					// �ő�I��
		Choice_Invalid = -1,		// �����Ă΂�Ă��Ȃ�
	};

	CTitleMenu();					// �R���X�g���N�^
	~CTitleMenu() override;			// �f�X�g���N�^

	HRESULT Init() override;		// ����������
	void Uninit() override;			// �I������
	void Update() override;			// �X�V����

	void Choice() override;			// �I�����̏���

	static CConfirmation_Window* GetComfirmationWindow() { return m_pConfirmationWindow; }	// ���j���[�E�B���h�E�̎擾����
	static void UninitComfirmationWindow();													// ���j���[�E�B���h�E�̔j������

	static CTitleMenu* Create();	// ����

private:
	static CConfirmation_Window* m_pConfirmationWindow;	// ���j���[�E�B���h�E�̃C���X�^���X
};

#endif // !_TITLE_MENU_H_