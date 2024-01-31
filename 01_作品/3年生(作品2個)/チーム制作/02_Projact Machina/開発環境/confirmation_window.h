//=============================================================================
//
// confirmatiomn_window.h (�m�F�E�B���h�E.h)
// Author : Saito Shian
//
//=============================================================================
#ifndef  _CONFIRMATION_WINDOW_H_				//���̃}�N����`������ĂȂ�������
#define  _CONFIRMATION_WINDOW_H_				//��d�C���N���[�h�h�~�̃}�N����`

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "object.h"
#include "d3dx9.h"
#include "fontString.h"
#include <vector>

//=============================================================================
// �O���錾
//=============================================================================
class CObject2D;
class CFontString;
class CCharSelect_Window;

//=============================================================================
// �m�F�E�B���h�E�N���X
//=============================================================================
class CConfirmation_Window
{
	static const float BLINK_SPEED;				// �I�����̓_�ő��x
	static const float MINI_ALPHA;				// �I�����̍Œᓧ���l
	const float SizeXScaleSpeed = 31.8f;		// x�T�C�Y�g��k���X�s�[�h
	const float SizeYScaleSpeed = 27.5f;		// y�T�C�Y�g��k���X�s�[�h

public:
	CConfirmation_Window();						// �m�F�E�B���h�E�̃R���X�g���N�^
	~CConfirmation_Window();					// �m�F�E�B���h�E�̃f�X�g���N�^

	HRESULT Init();								// ����������
	void Uninit();								// �I������
	void Update();								// �X�V����

	void ConfirmatiomnMenuScale();				// ���j���[�̃T�C�Y����
	bool ConfirmatiomnMenuScaleReduce();		// �m�F�E�B���h�E�̃T�C�Y�k������
	void CreateCharSelect();					// �L�����I����ʂ̐�������
	void ChangeChoice(const bool nextChoice);	// �I������ύX�������̏���
	void VariableInit();						// �ϐ��̏���������
	void UninitCharSelectWindow();				// �L�����I���E�B���h�E�̔j������

	virtual void Choice();						// �I�����̏���

	void SetDisplay(const bool display);										// �I�����̕\�������ݒ肷��
	void SetSapawnWindow(bool flag) { m_bConfimationSpawnWindow = flag; }		// �E�B���h�E�����̐ݒ菈��
	void SetChoice(CFontString* choice) { m_vpListChoice.push_back(choice); }	// �I�����|�C���^�̐ݒ菈��

	bool GetUninit() { return m_bConfimationUninitFlag; }						// �폜�t���O�̎擾����
	bool GetSapawnWindow() { return m_bConfimationSpawnWindow; }				// �E�B���h�E�����t���O�̎擾����
	std::vector<CFontString*> GetChoiceAll() { return m_vpListChoice; }			// �I�����|�C���^�̎擾����
	static bool GetSelectChoice() { return m_bConfimationSelectChoice; }		// �I�����ԍ��̎擾����
	static CCharSelect_Window* GetCharSelect() { return m_pCharSelectWindow; }	// �L�����I��p�E�B���h�E�̎擾����

	static CConfirmation_Window* Create(D3DXVECTOR3 pos,float xsize, float ysize, D3DXCOLOR col);		// ��������

private:
	static CCharSelect_Window* m_pCharSelectWindow;	// �L�����I��p�E�B���h�E�̃C���X�^���X
	static bool	m_bConfimationSelectChoice;			// �I�����̔ԍ�
	CObject2D* m_pObject2D;							// �I�u�W�F�N�g2D�̃C���X�^���X
	CFontString* m_pFont;							// �t�H���g
	D3DXVECTOR3 m_pos;								// �ʒu
	D3DXCOLOR m_Color;								// �F
	D3DXVECTOR2 m_Size;								// �T�C�Y
	D3DXVECTOR2 m_MaxSize;							// �ő�T�C�Y

	float	m_fConfimationBlinkSpeed;				// �_�ő��x

	bool	m_bConfimationMaxSize;					// X��Y�̃T�C�Y���ő�l�܂ōs������
	bool	m_bConfimationDisplay;					// �\�����̔���
	bool	m_bConfimationScale;					// �g��k���̃t���O
	bool    m_bConfimationUninitFlag;				// �폜�t���O
	bool    m_bConfimationDecition;					// ���肳�ꂽ��
	bool    m_bConfimationSpawnWindow;				// �E�B���h�E���������ꂽ��
	bool    m_bConfirmasionWindowUse;				// ���̃E�B���h�E���g���Ă��邩
	bool    m_bConfimationBack;						// �O�̃E�B���h�E����߂��Ă�����
	
	std::vector<CFontString*> m_vpListChoice;		// �I�����|�C���^�̔z��
};
#endif