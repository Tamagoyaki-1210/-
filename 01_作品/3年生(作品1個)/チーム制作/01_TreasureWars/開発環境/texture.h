//**********************************************************************************************
// 
// texture.h
// Author  : katsuki mizuki
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
		TEXTURE_TITLE = 0,			// �^�C�g��
		TEXTURE_GAME,				// �Q�[��(��)
		TEXTURE_RESULT,				// ���U���g(��)
		TEXTURE_NUMBER,				// ����
		TEXTURE_START,				// �X�^�[�g
		TEXTURE_TIMEUP,				// �^�C���A�b�v
		TEXTURE_SKYBOX,
		TEXTURE_PAPER,				// �̎���
		TEXTURE_FONT_JAPANESE,		// ���{��t�H���g
		TEXTURE_FONT_ALPHABET,		// �A���t�@�x�b�g�t�H���g
		TEXTURE_EFFECT_SMOKE,
		TEXTURE_EFFECT_FLARE,
		TEXTURE_EFFECT_STAR,
		TEXTURE_EFFECT_BRIGHTA,
		TEXTURE_EFFECT_BRIGHTB,
		TEXTURE_EFFECT_RAY,
		TEXTURE_EFFECT_COIN,
		TEXTURE_EFFECT_JEWEL,
		TEXTURE_EFFECT_SOUL,
		TEXTURE_EFFECT_UP,
		TEXTURE_BILL_SNOWSTORM,
		TEXTURE_AREA,				//�G���A�p�e�N�X�`��
		TEXTURE_CHARA_TYPE,			// �L�����N�^�[�^�C�v
		TEXTURE_SHADOW,				// �I�u�W�F�N�g3D�p�̉e
		TEXTURE_SALE,				// ���p��UX
		TEXTURE_STAGE_1,			// �X�e�[�W1
		TEXTURE_STAGE_2,			// �X�e�[�W2
		TEXTURE_STAGE_3,			// �X�e�[�W3
		TEXTURE_STAGE_RADOM,		// �X�e�[�W�����_��
		TEXTURE_PLAYER_1P,
		TEXTURE_PLAYER_2P,
		TEXTURE_PLAYER_3P,
		TEXTURE_PLAYER_4P,
		TEXTURE_PLAYER_CPU,
		TEXTURE_WINDOW_OUTSIDE,
		TEXTURE_TUTORIAL_ALLOW,		// �I�����p�̖��
		TEXTURE_BUTTON_A,			// A�{�^���������Ă�
		TEXTURE_BUTTON_SPACE,		// SPACE�L�[�������Ă�
		TEXTURE_TUTORIAL_01,		// �`���[�g���A���摜
		TEXTURE_TUTORIAL_02,		// �`���[�g���A���摜
		TEXTURE_TUTORIAL_03,		// �`���[�g���A���摜
		TEXTURE_TUTORIAL_04,		// �`���[�g���A���摜
		TEXTURE_SHOP_DIRECTION,		// �����̕���
		TEXTURE_DALLER,				// �h���}�[�N
		TEXTURE_FLOOR,
		TEXTURE_JUNGLE,				// �W�����O��
		TEXTURE_MOON,				// ����
		TEXTURE_RANDOM_FLOAR,		// �����_���w�i
		TEXTURE_GALAXY,				// ���
		TEXTURE_RANK,				// ����
		TEXTURE_DROP_SCORE_ITEM_NONE,				// �X�R�A�A�C�e���̃f�t�H���g
		TEXTURE_DROP_SCORE_ITEM_DIAMOND,			// �_�C�������h
		TEXTURE_DROP_SCORE_ITEM_RUBY,				// ���r�[
		TEXTURE_DROP_SCORE_ITEM_SAPPHIRE,			// �T�t�@�C��
		TEXTURE_DROP_SCORE_ITEM_EMERALD,			// �G�������h
		TEXTURE_DROP_SCORE_ITEM_TOPAZ,				// �g�p�[�Y
		TEXTURE_DROP_SCORE_ITEM_HEAD,				// ���̉���
		TEXTURE_DROP_SCORE_ITEM_AMBER,				// ����
		TEXTURE_DROP_SCORE_ITEM_BONE,				// ��
		TEXTURE_DROP_SCORE_ITEM_FOOTPRINTS,			// ���Ղ̉���
		TEXTURE_DROP_SCORE_ITEM_AMMONITE,			// �A�����i�C�g
		TEXTURE_DROP_SCORE_ITEM_CPU,				// CPU
		TEXTURE_DROP_SCORE_ITEM_SOLARPANEL,			// �\�[���[�p�l��
		TEXTURE_DROP_SCORE_ITEM_ANTENNA,			// �A���e�i
		TEXTURE_DROP_SCORE_ITEM_ENGINE,				// �G���W��
		TEXTURE_DROP_SCORE_ITEM_OXYGENTANK,			// �_�f�^���N
		TEXTURE_MAX,
		TEXTURE_NONE
	};

	static const char* s_FileName[];	// �t�@�C���p�X

public:
	CTexture();		// �f�t�H���g�R���X�g���N�^
	~CTexture();	// �f�X�g���N�^

public: /* �����o�֐� */
	void LoadAll();										// �S�Ă̓ǂݍ���
	//void Load(TEXTURE inTexture);						// �w��̓ǂݍ���
	void ReleaseAll();									// �S�Ă̔j��
	void Release(TEXTURE inTexture);					// �w��̔j��
	LPDIRECT3DTEXTURE9 GetTexture(TEXTURE inTexture);	// ���̎擾

private: /* �����o�ϐ� */
	LPDIRECT3DTEXTURE9 s_pTexture[TEXTURE_MAX];			// �e�N�X�`���̏��
};

#endif // !_TEXTURE_H_
