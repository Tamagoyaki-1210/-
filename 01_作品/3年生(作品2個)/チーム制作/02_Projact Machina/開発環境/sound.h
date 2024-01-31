//=============================================================================
//
// sound.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _SOUND_H
#define _SOUND_H

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
//#include <d3dx9.h>
#include "xaudio2.h"	// ���y�����ɕK�v
#include <vector>

//---------------------------
// �N���X�錾
//---------------------------
class CSound
{
public:
	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM_TITLE = 0,			// �^�C�g�����BGM
		SOUND_LABEL_BGM_GAME,				// �Q�[�����BGM1
		SOUND_LABEL_BGM_RESULT,				// ���U���g���BGM
		SOUND_LABEL_BGM_CHARACTER,			// �L�����I�����BGM
		SOUND_LABEL_BGM_BOSS,				// �{�XBGM

		SOUND_BGM_MAX,						// BGM�ő�l

		SOUND_LABEL_SE_SELECT,				// �I��
		SOUND_LABEL_SE_YES,					// ����
		SOUND_LABEL_SE_ENTER,				// �ŏI����

		SOUND_LABEL_SE_BOSSENTRY,			// �{�X�o��
		SOUND_LABEL_SE_BREAKBILL,			// �r���|��
		SOUND_LABEL_SE_DAMAGE,				// �_���[�W
		SOUND_LABEL_SE_BULLET_DAMAGE,		// �e�_���[�W
		SOUND_LABEL_SE_EXPLOSION,			// ����

		SOUND_LABEL_SE_WARNING,				// �x����
		SOUND_LABEL_SE_GUN_STANDBY,			// �e_�\��
		SOUND_LABEL_SE_OUT_OF_BULLET,		// �e�؂�

		SOUND_LABEL_SE_BULLET_AR,			// �e����(AR)
		SOUND_LABEL_SE_BULLET_MG,			// �e����(MG)
		SOUND_LABEL_SE_BULLET_SG,			// �e����(SG)
		SOUND_LABEL_SE_BULLET_SR,			// �e����(SR)

		SOUND_LABEL_SE_PUNCH,				// �Ō�
		SOUND_LABEL_SE_SLASH,				// ���U��
		SOUND_LABEL_SE_THRUST,				// �h��

		SOUND_LABEL_SE_PLAYERBOOST,			// �u�[�X�g
		SOUND_LABEL_SE_PLAYERWALK,			// ����

		SOUND_SE_MAX,						// SE�ő�l

		SOUND_LABEL_MAX
	};

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit();
	void Update();
	void Destroy();						// �����I�����\�[�X�{�C�X���I�������鏈��
	HRESULT Play(SOUND_LABEL label);	// �I�������Ȃ̍Đ�����
	void StopLabel(SOUND_LABEL label);	// �I�������Ȃ̒�~����
	void StopAll();						// �S�Ă̋Ȃ̒�~����
	void StopAllBGM();					// �S�Ă�BGM�̒�~����

	static CSound* Create(HWND hWnd);

private:

	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDPARAM;

	struct SSourceVoice
	{
		IXAudio2SourceVoice* pSourceVoice;		// �\�[�X�{�C�X�̃|�C���^
		SOUND_LABEL label;						// ���x���̎��
	};

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// �e���f�ނ̃p�����[�^
	static const SOUNDPARAM m_aParam[SOUND_LABEL_MAX];

	IXAudio2* m_pXAudio2 = nullptr;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_pMasteringVoice = nullptr;		// �}�X�^�[�{�C�X
	BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y
	WAVEFORMATEXTENSIBLE m_aWfx[SOUND_LABEL_MAX] = {};			// .wav�t�H�[�}�b�g�̏��

	std::vector<SSourceVoice> m_listSourceVoice = {};			// �\�[�X�{�C�X�̍\����
};

#endif // !_SOUND_H