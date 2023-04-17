//=============================================================================
//
// �摜
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "main.h"

//---------------------------
//�N���X
//---------------------------
class CTexture
{
public:
	enum Texture_Type
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,		//�v���C���[
		TYPE_ENEMY_01,		//�G�L����1
		TYPE_ENEMY_02,		//�G�L����2
		TYPE_ENEMY_03,		//�G�L����3
		TYPE_ENEMY_04,		//�G�L����4
		TYPE_OBSTACLE_01,	//��Q��1
		TYPE_OBSTACLE_02,	//��Q��2
		TYPE_BULLET_01,		//�e1
		TYPE_BULLET_02,		//�e2
		TYPE_RAY_01,		//�����G�t�F�N�g1
		TYPE_EXPLOSION,		//����
		TYPE_ITEM,			//�A�C�e��
		TYPE_BG,			//�w�i
		TYPE_BG_Tutorial01,	//�w�i(�`���[�g���A��01)
		TYPE_BG_Tutorial02,	//�w�i(�`���[�g���A��02)
		TYPE_BG_Tutorial03,	//�w�i(�`���[�g���A��03)
		TYPE_SCORE,			//�X�R�A
		TYPE_RANKING,		//�����L���O
		TYPE_THISNUMBER,	//����̃X�R�A
		TYPE_TITLE,			//�^�C�g�����S
		TYPE_PAUSE,			//�|�[�Y���
		TYPE_COMMA,			//�R���}
		TYPE_NUMBER,		//�i���o�[
		TYPE_LIFE,			//�̗�
		TYPE_GAUGE,			//�Q�[�W
		TYPE_CHOICE_01,		//�I����1
		TYPE_CHOICE_02,		//�I����2
		TYPE_CHOICE_03,		//�I����3
		TYPE_CHOICE_04,		//�I����4
		TYPE_CHOICE_05,		//�I����5
		TYPE_MAX
	};

	CTexture();
	~CTexture();

	void Uninit();
	void Load();
	void TextureType(Texture_Type textype);
private:
	LPDIRECT3DTEXTURE9 m_pTextureType[TYPE_MAX];		//Texture���
};

#endif // !_TEXTURE_H_
