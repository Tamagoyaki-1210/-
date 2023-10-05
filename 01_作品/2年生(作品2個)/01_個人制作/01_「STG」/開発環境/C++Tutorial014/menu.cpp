//=============================================================================
//
// ���j���[
// Author : tanimoto kosuke
//
//=============================================================================
#include "menu.h"
#include <stdio.h>
#include "application.h"
#include "texture.h"
#include "choice.h"
#include "game.h"
#include "message.h"

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CMenu::CMenu() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CMenu::~CMenu()
{

}

//=====================================
// ����������
//=====================================
void CMenu::Init()
{
	CObject2D::Init();

	m_menu = TYPE_NONE;

	m_menuMode = MODE_NONE;

	// �I�u�W�F�N�g�ݒ菈��
	CObject2D::SetObject2D(TYPE_UI, F_SCREEN_WIDTH, F_SCREEN_HEIGHT);
}

//=====================================
// �I������
//=====================================
void CMenu::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
// �X�V����
//=====================================
void CMenu::Update()
{
	CObject2D::Update();

	// UV�ݒ�
	CObject2D::SetUV();
}

//=====================================
// �`�揈��
//=====================================
void CMenu::Draw()
{
	//�摜�擾����
	switch (m_menu)
	{
	case TYPE_NONE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_NONE);
		break;
	case TYPE_FRAME:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_NONE);
		break;
	default:
		break;
	}
	if (CGame::GetPause() == true)
	{
		CObject2D::Draw();
	}
}

//=====================================
// ���j���[���[�h���̐����ݒ菈��
//=====================================
void CMenu::SetCreate(D3DXVECTOR3 pos)
{
	switch (m_menuMode)
	{
	case MODE_TITLE:
		CChoice::Create(pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), CChoice::TITLE_GAMESTART);		//�^�C�g��_�Q�[���J�n
		CChoice::Create(pos + D3DXVECTOR3(0.0f, 150.0f, 0.0f), CChoice::TITLE_EXIT);		//�^�C�g��_�I��
		break;
	case MODE_GAME:
		CMessage::Create(pos + D3DXVECTOR3(0.0f, -200.0f, 0.0f), CMessage::TYPE_PAUSE);	//�Q�[��_�|�[�Y���
		CChoice::Create(pos + D3DXVECTOR3(0.0f, 0.0f, 0.0f), CChoice::GAME_CONTINUE);	//�Q�[��_�ĊJ
		CChoice::Create(pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f), CChoice::GAME_EXIT);		//�Q�[��_�I��
		break;
	default:
		break;
	}
}

//=====================================
// ���j���[��������
//=====================================
CMenu *CMenu::Create(D3DXVECTOR3 pos, Menu_type menu, Menu_Mode menuMode)
{
	CMenu* pMenu = new CMenu;

	if (pMenu != nullptr)
	{
		pMenu->Init();
		pMenu->SetPos(pos);
		pMenu->m_menu = menu;
		pMenu->m_menuMode = menuMode;
		pMenu->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.1f));
		pMenu->SetCreate(pos);
	}
	return pMenu;
}
