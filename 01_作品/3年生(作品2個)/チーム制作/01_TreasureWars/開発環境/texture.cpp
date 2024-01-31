//**********************************************************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**********************************************************************************************

//==============================================================================================
// �C���N���[�h
//==============================================================================================
#include "application.h"
#include "texture.h"
#include "renderer.h"

#include <assert.h>

//==============================================================================================
// ��`
//==============================================================================================
const char* CTexture::s_FileName[] =
{// �e�N�X�`���̃p�X
	"Data/texture/title.png",						// �^�C�g��(��)
	"Data/texture/game.jpg",						// �Q�[��(��)
	"Data/texture/result.jpg",						// ���U���g(��)
	"Data/texture/number.png",						// ����
	"Data/texture/start.png",						// �X�^�[�g
	"Data/texture/timeup.png",						// �^�C���A�b�v
	"Data/texture/sky000.jpg",						// �X�J�C�{�b�N�X
	"Data/texture/paper_00104.jpg",					// �̎���
	"Data/texture/Font/Japanese2000unicode.png",	// ���{��t�H���g
	"Data/texture/Font/Alphabet.png",				// �A���t�@�x�b�g�t�H���g�t�H���g
	"Data/texture/particle/smoke.png",				// ��
	"Data/texture/particle/flare.png",				// �G�t�F�N�g1
	"Data/texture/particle/star.png",				// �G�t�F�N�g2
	"Data/texture/particle/bright.png",				// �L���L��A
	"Data/texture/particle/bright2.png",			// �L���L��B
	"Data/texture/particle/ray.png",				// ���C
	"Data/texture/particle/coin.png",				// �R�C��
	"Data/texture/particle/jewel.png",				// �p�[�e�B�N���p���
	"Data/texture/particle/soul.png",				// ��
	"Data/texture/particle/up.png",					// ����
	"Data/texture/blizzardoffallingbills.png",		// �D����
	"Data/texture/Area.png",						// �G���A
	"Data/texture/PlayerNumber.png",				// �L�����N�^�[�^�C�v
	"Data/texture/shadow000.jpg",					// �I�u�W�F�N�g3D�p�̉e
	"Data/texture/sale.png",						// ���p��UX
	"Data/texture/stage_1.png",						// �X�e�[�W1
	"Data/texture/stage_2.png",						// �X�e�[�W2
	"Data/texture/stage_3.png",						// �X�e�[�W3
	"Data/texture/stage_random.png",				// �X�e�[�W�����_��
	"Data/texture/1P.png",
	"Data/texture/2P.png",
	"Data/texture/3P.png",
	"Data/texture/4P.png",
	"Data/texture/CPU.png",
	"Data/texture/���̊O.png",
	"Data/texture/select_arrow.png",				// �I�����p�̖��
	"Data/texture/button_A.png",					// A�{�^���������Ă�
	"Data/texture/button_SPACE.png",				// SPACE�L�[�������Ă�
	"Data/texture/tutorial_01.png",					// �`���[�g���A���摜_01
	"Data/texture/tutorial_02.png",					// �`���[�g���A���摜_02
	"Data/texture/tutorial_03.png",					// �`���[�g���A���摜_03
	"Data/texture/tutorial_04.png",					// �`���[�g���A���摜_04
	"Data/texture/shop_direction.png",
	"Data/texture/daller.png",
	"Data/texture/floor.png",						// ��
	"Data/texture/Jungle.png",
	"Data/texture/Moon.png",
	"Data/texture/random_Floar.png",
	"Data/texture/Galaxy.png",
	"Data/texture/����.png",						// ����
	"Data/texture/Drop_Score_Item/scoreitem_none.png",		// �X�R�A�A�C�e���̃f�t�H���g
	"Data/texture/Drop_Score_Item/Stage_01/Diamond.png",	// �_�C�������h
	"Data/texture/Drop_Score_Item/Stage_01/Ruby.png",		// ���r�[
	"Data/texture/Drop_Score_Item/Stage_01/Sapphire.png",	// �T�t�@�C��
	"Data/texture/Drop_Score_Item/Stage_01/Emerald.png",	// �G�������h
	"Data/texture/Drop_Score_Item/Stage_01/Topaz.png",		// �g�p�[�Y
	"Data/texture/Drop_Score_Item/Stage_02/Head.png",		// ���̉���
	"Data/texture/Drop_Score_Item/Stage_02/Amber.png",		// ����
	"Data/texture/Drop_Score_Item/Stage_02/Bone.png",		// ��
	"Data/texture/Drop_Score_Item/Stage_02/Footprints.png",	// ���Ղ̉���
	"Data/texture/Drop_Score_Item/Stage_02/Ammonite.png",	// �A�����i�C�g
	"Data/texture/Drop_Score_Item/Stage_03/Cpu.png",		// CPU
	"Data/texture/Drop_Score_Item/Stage_03/Solarpanel.png",	// �\�[���[�p�l��
	"Data/texture/Drop_Score_Item/Stage_03/Antenna.png",	// �A���e�i
	"Data/texture/Drop_Score_Item/Stage_03/Engine.png",		// �G���W��
	"Data/texture/Drop_Score_Item/Stage_03/OxygenTank.png",	// �_�f�^���N
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//----------------------------------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//----------------------------------------------------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//----------------------------------------------------------------------------------------------
// �f�X�g���N�^
//----------------------------------------------------------------------------------------------
CTexture::~CTexture()
{
}

//----------------------------------------------------------------------------------------------
// �S�Ă̓ǂݍ���
//----------------------------------------------------------------------------------------------
void CTexture::LoadAll()
{
	// �f�o�C�X�ւ̃|�C���^�̎擾
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// �e�N�X�`���̓ǂݍ��݂�����Ă���
			continue;
		}

		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

////----------------------------------------------------------------------------------------------
//// �ǂݍ���
////----------------------------------------------------------------------------------------------
//void CTexture::Load(TEXTURE inTexture)
//{
//	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);
//
//	if (s_pTexture[inTexture] != nullptr)
//	{// �e�N�X�`���̓ǂݍ��݂�����Ă���
//		return;
//	}
//
//	// �f�o�C�X�ւ̃|�C���^�̎擾
//	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
//
//	// �e�N�X�`���̓ǂݍ���
//	D3DXCreateTextureFromFile(pDevice,
//		s_FileName[inTexture],
//		&s_pTexture[inTexture]);
//}

//----------------------------------------------------------------------------------------------
// �S�Ẳ��
//----------------------------------------------------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// �e�N�X�`���̉��
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//----------------------------------------------------------------------------------------------
// ���
//----------------------------------------------------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != NULL)
	{// �e�N�X�`���̉��
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = NULL;
	}
}

//----------------------------------------------------------------------------------------------
// �擾
//----------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// �e�N�X�`�����g�p���Ȃ�
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// �ǂݍ���
	//Load(inTexture);

	return s_pTexture[inTexture];
}
