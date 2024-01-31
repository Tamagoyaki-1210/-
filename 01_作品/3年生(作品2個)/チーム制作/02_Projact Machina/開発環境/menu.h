//=============================================================================
//
// menu.h
// Author : Tanimoto Kosuke
// Author : Saito Shian
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "d3dx9.h"
#include "fontString.h"
#include <vector>

//=============================================================================
// �N���X�錾
//=============================================================================
class CMenu
{
	static const float CHOICE_BLINK_SPEED;		// �I�����̓_�ő��x
	static const float CHOICE_MINI_ALPHA;		// �I�����̍Œᓧ���l

public:
	CMenu();									// �R���X�g���N�^
	virtual ~CMenu();							// �f�X�g���N�^

	virtual HRESULT Init();						// ����������
	virtual void Uninit();						// �I������
	virtual void Update();						// �X�V����

	virtual void Choice();						// �I�����̏���
	void ChangeChoice(const int nextChoice);	// �I������ύX�������̏���

	void SetMenuInitiative(const int index) { m_nMenuInitiative = index; }		// ���j���[�̎哱������
	void SetDecition(const bool decition) { m_bDecition = decition; }			// �I�����̔ԍ��̐ݒ菈��
	void SetChoice(CFontString* choice) { m_vpListChoice.push_back(choice); }	// �I��������
	void SetDisplay(const bool display);										// �I�����̕\�������ݒ肷��

	const int GetMenuInitiative() { return m_nMenuInitiative; }					// ���j���[�̎哱���̎擾����
	const int GetSelectChoice() { return m_nSelectChoice; }						// �I�����̎擾����
	const bool GetDecition() { return m_bDecition; }							// �I�񂾑I�����̎擾����
	const bool GetDisplay() { return m_bDisplay; }								// �\�����̔���擾����
	CFontString* GetChoice(const int index) { return m_vpListChoice[index]; }	// �I�����|�C���^�ԍ��̎擾����
	std::vector<CFontString*> GetChoiceAll() { return m_vpListChoice; }			// �I�����|�C���^�̎擾����

private:

	int		m_nMenuInitiative;					// ���j���[�̎哱��
	int		m_nSelectChoice;					// �I�����̔ԍ�

	float	m_fBlinkSpeed;						// �_�ő��x

	bool	m_bDecition;						// ����̔���
	bool	m_bDisplay;							// �\�����̔���

	std::vector<CFontString*> m_vpListChoice;	// �I�����|�C���^�̔z��
};

#endif // !_MENU_H_