//=============================================================================
//
// ���j���[
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _MENU_H_
#define _MENU_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//�N���X
//---------------------------
class CMenu : public CObject2D
{
public:
	//���j���[�摜���
	enum Menu_type
	{
		TYPE_NONE = 0,	// �������Ă��Ȃ����
		TYPE_FRAME,		// ���j���[�̊z��
		TYPE_MAX
	};

	// ���j���[���[�h���
	enum Menu_Mode
	{
		MODE_NONE = 0,
		MODE_TITLE,	//�^�C�g��
		MODE_GAME,	//�Q�[��
		MODE_MAX
	};

	CMenu();
	~CMenu() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetCreate(D3DXVECTOR3 pos);

	static CMenu *Create(D3DXVECTOR3 pos, Menu_type menu, Menu_Mode menuMode);
private:
	Menu_type m_menu;
	Menu_Mode m_menuMode;
};

#endif // !_MENU_H_
