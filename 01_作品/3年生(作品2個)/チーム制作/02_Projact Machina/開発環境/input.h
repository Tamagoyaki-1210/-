//=============================================================================
//
// ���͏��� [input.h]
// Author : KOZUNA HIROHITO
// 
//=============================================================================

#ifndef _INPUT_H_		//���̃}�N����`������Ȃ�������
#define _INPUT_H_		//2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "inputkeydata.h"
#include "DirectInput.h"
#include <vector>

//----------------------------------------------------------------------------
//�O���錾
//----------------------------------------------------------------------------
class CInputKeyboard;
class CInputJoyPad;
class CInputMouse;

//----------------------------------------------------------------------------
//�N���X��`
//----------------------------------------------------------------------------
class CInput
{
public:

	CInput();
	~CInput();

	//*�A�v���P�[�V���������ɏ������
	static CInput *Create();							//���͏����n�̃N���G�C�g�AInit�̑O�ɏ���
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		//���͏����S���̏�����
	void Uninit();										//���͏����S���̏I������
	void Update();										//���͏����S���̍X�V����
	//*

	//*�C���v�b�g���K�v�Ȏ��Ăяo��
	static CInput *GetKey() { return m_pInput; }		//�v���C��g���K�[�Ȃǂ̃A�h���X�̎擾

	/* ���� */
	bool Press(STAN_DART_INPUT_KEY key, int nKeyLock) { return KeyChackAll(key, nKeyLock, 1); }		// �v���X
	bool Trigger(STAN_DART_INPUT_KEY key, int nKeyLock) { return KeyChackAll(key, nKeyLock, 2); }	// �g���K�[
	bool Release(STAN_DART_INPUT_KEY key, int nKeyLock) { return KeyChackAll(key, nKeyLock, 3); }	// �����[�X

	bool Press(STAN_DART_INPUT_KEY key, int nKeyLock, int nNum) { return KeyChackNum(key, nKeyLock, 1, nNum); }		// �v���X
	bool Trigger(STAN_DART_INPUT_KEY key, int nKeyLock, int nNum) { return KeyChackNum(key, nKeyLock, 2, nNum); }	// �g���K�[
	bool Release(STAN_DART_INPUT_KEY key, int nKeyLock, int nNum) { return KeyChackNum(key, nKeyLock, 3, nNum); }	// �����[�X

	//�{�Q�[����p
	bool MovePress(BOOK_GAME_DEDICATED_MOVE_KEY key,int nKeyLock = 0);


	D3DXVECTOR3 VectorMove(); //�ړ��p�x�N�g���̎擾

	// ���͂��Ă���f�o�C�X��Ԃ�
	std::vector<int> PressDevice(STAN_DART_INPUT_KEY key);		// �v���X
	std::vector<int> TriggerDevice(STAN_DART_INPUT_KEY key);	// �g���K�[
	std::vector<int> ReleaseDevice(STAN_DART_INPUT_KEY key);	// �����[�X

	/* Keyboard */
	bool Press(int nKey, int nKeyLock = 0,int num = -1);								//�v���X
	bool Trigger(int nkey, int nKeyLock = 0, int num = -1);								//�g���K�[
	bool Release(int nkey, int nKeyLock = 0, int num = -1);								//�����[�X

	/* JoyPad */
	bool Press(DirectJoypad key, int nKeyLock = 0, int nNum = 0);			//�W���C�p�b�g�v���X
	bool Trigger(DirectJoypad key, int nKeyLock = 0, int nNum = 0);		//�W���C�p�b�g�g���K�[
	bool Release(DirectJoypad key, int nKeyLock = 0, int nNum = 0);		//�W���C�p�b�g�����[�X
	bool StickPress(JOYKEY_DIRECT_CROSS key, int nNum = 0, bool bleftandright = false, float frot = 0.25f,int nKeyLock = 0); //�X�e�B�b�N�̃v���X

	D3DXVECTOR3 VectorMoveKey(int nNum);				//�\���L�[���̃x�N�g���擾
	D3DXVECTOR3 VectorMoveJoyStick(int nNum = 0, bool bleftandright = false); //�W���C�X�e�B�b�N�̃x�N�g���擾
	D3DXVECTOR3 VectorMoveJoyStickAll(bool bleftandright = false); //�W���C�X�e�B�b�N�̃x�N�g���擾

	/* Mouse */
	bool Press(MOUSE_KEY key, int nKeyLock = 0);				//�v���X
	bool Trigger(MOUSE_KEY key, int nKeyLock = 0);			//�g���K�[
	bool Release(MOUSE_KEY key, int nKeyLock = 0);			//�����[�X

	D3DXVECTOR3 VectorMoveKeyAll();			//�\���L�[���̃x�N�g���擾

	D3DXVECTOR3 GetMouseCursor(void);		// �}�E�X�J�[�\���̃X�N���[�����W�̎擾
	int GetMouseWheel(void);				// �}�E�X�̃z�C�[���̓�������
	D3DXVECTOR3 GetMouseMove(void);			// �}�E�X�̈ړ��ʂ��o�͏���
	void SetCursorErase(bool bUse);			// ��ʓ��̃J�[�\�����������ǂ���

	InputType GetOldInputType() { return m_nOldInputType; }		//�Ō�ɐG�����f�o�C�X
																
	void SetJoypadKeyConfig(int nPlayerNum, DirectJoypad OldKey, DirectJoypad NewKey); //�L�[�̓���ւ�

	bool IsConnected(int nNum);	//�W���C�p�b�h���ڑ�����Ă��邩�ǂ���
	int GetIsConnectedNumMax(); //�W���C�p�b�h�̐ڑ���

	void IntermediateReception(bool bIR = false);	//�f�o�C�X�̓r�����m�̃I���I�t
	bool GetIntermediateReception();		//���݃f�o�C�X�̓r�����m���s���Ă��邩�ǂ���

	void SetKeyLock(int nKeyLock); //�L�[�̐ݒ�(�㏑���ݒ�̓G���[��\��)
	void UnlockKey(int nKeyLock); //�L�[�̉���

private:
	bool KeyChackAll(STAN_DART_INPUT_KEY key, int type, int nKeyLock);			// �S�f�o�C�X�̓��͂��m�F
	bool KeyChackNum(STAN_DART_INPUT_KEY key, int type, int nKeyLock, int nNum);	// �w�肵���f�o�C�X�̓��͂��m�F
private:

	CInputKeyboard *m_pKeyboard;	//�L�[�{�[�h�̏��
	CInputJoyPad *m_pJoyPad;		//�W���C�p�b�h�̏��
	CInputMouse *m_pMouse;			//�}�E�X�̏��
	static CInput *m_pInput;		//���̃N���X�̏��
	InputType m_nOldInputType;		//�Ō�ɐG�����f�o�C�X
	int m_nKeyLock;					//�L�[�����Ă���Ă���Ƃ��L�[�������Ă��Ȃ���������̓��͂𖳎�����

};

#endif

