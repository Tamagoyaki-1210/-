//**********************************************************************************************
// 
// texture.h
// Author  : tanimoto kosuke
// 
//**********************************************************************************************
#ifndef _TEXTURE_H_	//���̃}�N����`������ĂȂ�������
#define _TEXTURE_H_	//�Q�d�C���N���[�h�h�~�̃}�N����`

//==============================================================================================
// �C���N���[�h
//==============================================================================================
#include <d3dx9.h>

//==============================================================================================
// ��`
//==============================================================================================
class CTexture
{
public: /* ��` */
	enum TEXTURE
	{
		TEXTURE_FONT_JAPANESE,		// ���{��t�H���g
		TEXTURE_FONT_JAPANESE_VER2,	// ���{��t�H���g
		TEXTURE_FONT_ALPHABET,		// �A���t�@�x�b�g�t�H���g
		TEXTURE_NUMBER,				// �i���o�[
		TEXTURE_BULLET,				// �e
		TEXTURE_FLOOR,				// �n��
		TEXTURE_MAP,				// �n��
		TEXTURE_RETICLE,			// ���e�B�N��
		TEXTURE_CIRCLE,				// �~
		TEXTURE_ITEM_POINTER,		// �A�C�e���|�C���^�[
		TEXTURE_UI,					// UI
		TEXTURE_KEY_E,				// E�L�[

		/* �^�C�g�����S */
		TEXTURE_TITLE_00,
		TEXTURE_TITLE_01,
		TEXTURE_TITLE_02,
		TEXTURE_TITLE_03,
		TEXTURE_TITLE_04,
		TEXTURE_TITLE_05,
		TEXTURE_TITLE_06,
		TEXTURE_TITLE_07,
		TEXTURE_TITLE_08,
		TEXTURE_TITLE_ROBOT,

		/* �^�C�g������*/
		TEXTURE_TITLE_GEAR_00,
		TEXTURE_TITLE_GEAR_01,
		TEXTURE_TITLE_GEAR_02,
		TEXTURE_TITLE_GEAR_03,
		TEXTURE_TITLE_GEAR_04,
		TEXTURE_TITLE_GEAR_05,
		TEXTURE_TITLE_GEAR_06,

		/* �G�t�F�N�g�Q */
		TEXTURE_EFFECT_SMOKE,
		TEXTURE_EFFECT_FLARE,
		TEXTURE_EFFECT_STAR,
		TEXTURE_EFFECT_BRIGHTA,
		TEXTURE_EFFECT_BRIGHTB,
		TEXTURE_EFFECT_RAY,
		TEXTURE_EFFECT_SHOCK,
		TEXTURE_EFFECT_BURST,
		TEXTURE_EFFECT_SOUL,
		TEXTURE_EFFECT_UP,
		TEXTURE_EFFECT_RETICLE,
		TEXTURE_CHAIN,
		TEXTURE_DONOT_ENTER,

		/* ����A�C�R�� */
		TEXTURE_WEAPON_AR,				// �A�T���g���C�t���A�C�R��
		TEXTURE_WEAPON_SMG,				// �T�u�}�V���K���A�C�R��
		TEXTURE_WEAPON_HG,				// �n���h�K���A�C�R��
		TEXTURE_WEAPON_MG,				// �}�V���K���A�C�R��
		TEXTURE_WEAPON_SG,				// �V���b�g�K���A�C�R��
		TEXTURE_WEAPON_SR,				// �X�i�C�p�[���C�t���A�C�R��
		TEXTURE_WEAPON_KNUCKLE,			// �f��A�C�R��
		TEXTURE_WEAPON_SLASH,			// �X���b�V���A�C�R��
		TEXTURE_WEAPON_POKE,			// �h�˃A�C�R��

		/* �A�^�b�N�X�L�� */
		TEXTURE_ATTACK_SKILL_AR,		// �A�T���g���C�t���̃A�^�b�N�X�L���A�C�R��
		TEXTURE_ATTACK_SKILL_SMG,		// �T�u�}�V���K���̃A�^�b�N�X�L���A�C�R��
		TEXTURE_ATTACK_SKILL_HG,		// �n���h�K���̃A�^�b�N�X�L���A�C�R��
		TEXTURE_ATTACK_SKILL_MG,		// �}�V���K���̃A�^�b�N�X�L���A�C�R��
		TEXTURE_ATTACK_SKILL_SG,		// �V���b�g�K���̃A�^�b�N�X�L���A�C�R��
		TEXTURE_ATTACK_SKILL_SR,		// �X�i�C�p�[���C�t���̃A�^�b�N�X�L���A�C�R��
		TEXTURE_ATTACK_SKILL_KNUCKLE,	// �f��̃A�^�b�N�X�L���A�C�R��
		TEXTURE_ATTACK_SKILL_SLASH,		// �X���b�V���̃A�^�b�N�X�L���A�C�R��
		TEXTURE_ATTACK_SKILL_POKE,		// �h�˂̃A�^�b�N�X�L���A�C�R��

		/* �T�|�[�g�X�L�� */
		TEXTURE_SKILL_RUSH,				// ���b�V��
		TEXTURE_SKILL_VANGUARD,			// ���@���K�[�h
		TEXTURE_SKILL_EAGLEEYE,			// �C�[�O���A�C
		TEXTURE_SKILL_MARSHALL,			// �}�[�V����

		/* �T�|�[�g�X�L������ */
		TEXTURE_SKILLEXPLANATION_RUSH,				// ���b�V������
		TEXTURE_SKILLEXPLANATION_VANGUARD,			// ���@���K�[�h����
		TEXTURE_SKILLEXPLANATION_EAGLEEYE,			// �C�[�O���A�C����
		TEXTURE_SKILLEXPLANATION_MARSHALL,			// �}�[�V��������

		/* �����e�N�X�`�� */
		TEXTURE_AIRCRAAFTSELECTION,					// �@�̑I��
		TEXTURE_FASTESTDEFEATE,						// �ŒZ���j
		TEXTURE_DURABILITYDAMAGE,					// �ϋv�_���[�W��
		TEXTURE_NUMBEROFDEFEATS,					// ���j��

		TEXTURE_MAX,
		TEXTURE_NONE
	};

	static const char* s_FileName[];	// �t�@�C���p�X

public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public: /* �����o�֐� */
	void LoadAll();										// �S�Ă̓ǂݍ���
	void ReleaseAll();									// �S�Ă̔j��
	void Release(TEXTURE inTexture);					// �w��̔j��
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// ���̎擾

private: /* �����o�ϐ� */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];			// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
