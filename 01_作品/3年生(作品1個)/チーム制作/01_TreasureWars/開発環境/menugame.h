//=============================================================================
//
// menugame.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _MENU_GAME_H_
#define _MENU_GAME_H_

#include "menu.h"
#include "object2D.h"

//---------------------------
// �N���X�錾
//---------------------------
class CMenuGame : public CMenu
{
public:
	enum Choice_Type
	{
		Choice_None = 0,	// �����Ă΂�Ă��Ȃ�
		Choice_Continue,	// �Q�[���𑱂���
		Choice_Entry,		// �G���g���[��ʂ�
		Choice_Title,		// �^�C�g����ʂ�
		Choice_Max
	};

	CMenuGame();
	~CMenuGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	void PauceObjUpdate();		// ��~���̃|�[�Y�I�u�W�F�N�g�̍X�V����
	void Pauce();				// �|�[�Y�̏���
	void Display();				// ��ʕ\���̏���
	void Choice();				// �I�����̏���

	static CMenuGame* Create();

private:
	bool m_bDisplay;			// ��ʕ\���̔���
	bool m_bCurrentDisplay;		// �O��̉�ʕ\���̔���

	CFontString* m_pPauceLogo;	// �|�[�Y���S�̃I�u�W�F�N�g2D�|�C���^
	CObject2D* m_pFade;			// �t�F�[�h�[��
};

#endif // !_MENU_GAME_H_