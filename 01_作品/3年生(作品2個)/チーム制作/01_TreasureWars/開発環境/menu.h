//=============================================================================
//
// menu.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

#include "d3dx9.h"
#include "fontString.h"
#include <vector>

class CInput;

//---------------------------
// �N���X�錾
//---------------------------
class CMenu
{
public:
	CMenu();
	virtual ~CMenu();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	void Input();								// ���͏���
	void ChangeChoice(const int nextChoice);	// �I������ύX�������̏���

	void SetMenuInitiative(const int index) { m_nMenuInitiative = index; }
	void SetDecition(const bool decition) { m_bDecition = decition; }
	void SetChoice(CFontString* choice) { m_pListChoice.push_back(choice); }

	const int GetMenuInitiative() { return m_nMenuInitiative; }
	const int GetSelectChoice() { return m_nSelectChoice; }
	const bool GetDecition() { return m_bDecition; }
	std::vector<CFontString*> GetChoice() { return m_pListChoice; }

private:
	static const float Choice_Blink_Speed;		// �I�����̓_�ő��x
	static const float Choice_Mini_Alpha;		// �I�����̍Œᓧ���l

	int m_nMenuInitiative;						// ���j���[�̎哱��
	int m_nSelectChoice;						// �I�����̔ԍ�
	float m_fBlinkSpeed;						// �_�ő��x
	bool m_bDecition;							// ����̔���

	std::vector<CFontString*> m_pListChoice;	// �I�����|�C���^�̔z��
};

#endif // !_MENU_H_