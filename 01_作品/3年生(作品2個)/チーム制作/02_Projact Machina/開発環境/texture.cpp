//**********************************************************************************************
// 
// texture.cpp
// Author  : tanimoto kosuke
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

	"Data/texture/Font/Font_Japanese002.png",		// ���{��t�H���g
	"Data/texture/Font/Font_Japanese003.png",		// ���{��t�H���g
	"Data/texture/Font/Font_Alphabet.png",			// �A���t�@�x�b�g�t�H���g
	"Data/texture/number.png",						// �i���o�[�̃e�N�X�`��
	"Data/texture/particle/effect001.png",			// �e�̃e�N�X�`��
	"Data/texture/Concrete.png",					// �n�ʂ̃e�N�X�`��
	"Data/texture/map.png",							// �}�b�v�̃e�N�X�`��
	"Data/texture/reticle.png",						// ���e�B�N���̃e�N�X�`��
	"Data/texture/circle.png",						// �~�̃e�N�X�`��
	"Data/texture/ItemPointer.png",					// �A�C�e���|�C���^�[�̃e�N�X�`��
	"Data/texture/UI_00.png",						// UI�̃e�N�X�`��
	"Data/texture/Ekey.png",						// E�L�[�̃e�N�X�`��

	/* �^�C�g�����S */
	"Data/texture/Title/logo/title_00.png",
	"Data/texture/Title/logo/title_01.png",
	"Data/texture/Title/logo/title_02.png",
	"Data/texture/Title/logo/title_03.png",
	"Data/texture/Title/logo/title_04.png",
	"Data/texture/Title/logo/title_05.png",
	"Data/texture/Title/logo/title_06.png",
	"Data/texture/Title/logo/title_07.png",
	"Data/texture/Title/logo/title_08.png",
	"Data/texture/Title/logo/title_robot.png",

	/* �^�C�g�����ԁ@*/
	"Data/texture/Title/gear/gear_00.png",
	"Data/texture/Title/gear/gear_01.png",
	"Data/texture/Title/gear/gear_02.png",
	"Data/texture/Title/gear/gear_03.png",
	"Data/texture/Title/gear/gear_04.png",
	"Data/texture/Title/gear/gear_05.png",
	"Data/texture/Title/gear/gear_06.png",
	
	/* �G�t�F�N�g�Q */
	"Data/texture/particle/smoke.png",				// ��
	"Data/texture/particle/flare.png",				// �G�t�F�N�g1
	"Data/texture/particle/star.png",				// �G�t�F�N�g2
	"Data/texture/particle/bright.png",				// �L���L��A
	"Data/texture/particle/bright2.png",			// �L���L��B
	"Data/texture/particle/ray.png",				// ���C
	"Data/texture/particle/effect001.png",			// �R�C��
	"Data/texture/particle/effect003.png",			// player�̉�
	"Data/texture/particle/soul.png",				// ��
	"Data/texture/particle/up.png",					// ����
	"Data/texture/reticle.png",						// ���e�B�N��
	"Data/texture/chain.png",						// �`�F�[��
	"Data/texture/NotEnter.png",					// �ړ�����

	"Data/texture/Weapon/AR_icon.png",				// �A�T���g���C�t���A�C�R��
	"Data/texture/Weapon/SMG_icon.png",				// �T�u�}�V���K���A�C�R��
	"Data/texture/Weapon/SHandgun_icon.png",		// �n���h�K���A�C�R��
	"Data/texture/Weapon/MG_icon.png",				// �}�V���K���A�C�R��
	"Data/texture/Weapon/SG_icon.png",				// �V���b�g�K���A�C�R��
	"Data/texture/Weapon/SR_icon.png",				// �X�i�C�p�[���C�t���A�C�R��
	"Data/texture/Weapon/Knuckle_icon.png",			// �f��A�C�R��
	"Data/texture/Weapon/Slash_icon.png",			// �X���b�V���A�C�R��
	"Data/texture/Weapon/stabbing_icon.png",		// �h�˃A�C�R��

	"Data/texture/WeaponSkill/WeaponSkill_AR.png",		// �A�T���g���C�t���̃A�^�b�N�X�L���A�C�R��
	"Data/texture/WeaponSkill/WeaponSkill_SMG.png",		// �T�u�}�V���K���̃A�^�b�N�X�L���A�C�R��
	"Data/texture/WeaponSkill/WeaponSkill_SHandgun.png",	// �n���h�K���̃A�^�b�N�X�L���A�C�R��
	"Data/texture/WeaponSkill/WeaponSkill_MG.png",		// �}�V���K���̃A�^�b�N�X�L���A�C�R��
	"Data/texture/WeaponSkill/WeaponSkill_SG.png",		// �V���b�g�K���̃A�^�b�N�X�L���A�C�R��
	"Data/texture/WeaponSkill/WeaponSkill_SR.png",		// �X�i�C�p�[���C�t���̃A�^�b�N�X�L���A�C�R��
	"Data/texture/WeaponSkill/WeaponSkill_Knuckle.png",	// �f��̃A�^�b�N�X�L���A�C�R��
	"Data/texture/WeaponSkill/WeaponSkill_Slash.png",	// �X���b�V���̃A�^�b�N�X�L���A�C�R��
	"Data/texture/WeaponSkill/WeaponSkill_stabbing.png",// �h�˂̃A�^�b�N�X�L���A�C�R��

	"Data/texture/job/job_Rush.png",				// ���b�V��
	"Data/texture/job/job_Vanguard.png",			// ���@���K�[�h
	"Data/texture/job/job_Eagleeye.png",			// �C�[�O���A�C
	"Data/texture/job/job_Marshall.png",			// �}�[�V����

	"Data/texture/job/status_rush.png",				// ���b�V��
	"Data/texture/job/status_vanguard.png",			// ���@���K�[�h
	"Data/texture/job/status_eagleeye.png",			// �C�[�O���A�C
	"Data/texture/job/status_standard.png",			// �}�[�V����

	"Data/texture/TEX001.png",						// �@�̑I��
	"Data/texture/TEX002.png",						// �ŒZ���j
	"Data/texture/TEX003.png",						// �ϋv�_���[�W
	"Data/texture/TEX004.png",						// ���j��

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

	return s_pTexture[inTexture];
}
