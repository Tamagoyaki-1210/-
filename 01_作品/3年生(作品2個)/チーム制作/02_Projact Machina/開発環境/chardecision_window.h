//=============================================================================
//
// chardecision_window.h (�L��������E�B���h�E.h)
// Author : Saito Shian
//
//=============================================================================
#ifndef  _CHARDECISISION_WINDOW_H_				//���̃}�N����`������ĂȂ�������
#define  _CHARDECISISION_WINDOW_H_				//��d�C���N���[�h�h�~�̃}�N����`

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
class CConfirmation_Window;
class CNumber;
class CPlayer_Parameter;

//=============================================================================
// �L��������E�B���h�E�N���X
//=============================================================================
class CCharDecision_Window
{
	static const int MAX_TEXTURE = 2;							// �e�N�X�`���̍ő吔
	static const float BLINK_SPEED;								// �I�����̓_�ő��x
	static const float MINI_ALPHA;								// �I�����̍Œᓧ���l
	const float SizeXScaleSpeed = 31.8f;						// x�T�C�Y�g��k���X�s�[�h
	const float SizeYScaleSpeed = 27.5f;						// y�T�C�Y�g��k���X�s�[�h

public:
	CCharDecision_Window();										// �L��������E�B���h�E�̃R���X�g���N�^
	~CCharDecision_Window();									// �L��������E�B���h�E�̃f�X�g���N�^

	HRESULT Init();												// ����������
	void Uninit();												// �I������
	void Update();												// �X�V����

	void UninitExplanation();									// �����p�e�N�X�`���A�t�H���g�̔j������
	void CharDecisionMenuScale();								// �L��������̃T�C�Y����
	void CharSelectChoice();									// �I�����̏���
	void BackWindow();											// �E�B���h�E��߂�����
	void SlideWindow();											// �E�B���h�E���X���C�h�����鏈��
	void PlayerIndex();											// �v���C���[�ԍ�����
	bool CharDecisionMenuScaleReduce();							// �L��������̃T�C�Y�k������

	void SetFont(const std::string lette);									// �t�H���g�̐ݒ菈��
	void SetTextue(CTexture::TEXTURE texture);		// �e�N�X�`���̐ݒ�
	void SetParameterText();						// �p�����[�^�[�̃e�L�X�g�ݒ�

	int GetSelectIndex() { return m_nSelectIndex; }
	bool GetSapawnWindow() { return m_bSpawnWindow; }							// �E�B���h�E���������ꂽ���̎擾����
	bool GetMaxScale() { return m_bMaxSize; }									// �E�B���h�E���ő�T�C�Y�܂ōs�����̂����擾���鏈��
	bool GetExplanationUninit() { return m_bExplanationUninit; }				// �t�H���g�A�e�N�X�`�����폜���ꂽ���̎擾����		
	bool GetUninit() { return m_bUninitFlag; }									// �폜�t���O�̎擾����
	bool GetScaleReduce() { return m_bScaleReduce; }

	std::vector<CFontString*> GetChoiceAll() { return m_vpListChoice; }										// �I�����|�C���^�̎擾����
	static CCharDecision_Window* Create(D3DXVECTOR3 pos, float xsize, float ysize, D3DXCOLOR col);			// ��������

private:
	static CConfirmation_Window* m_pConfirmation;
	static CPlayer_Parameter *m_pPlayer_Parameter;	// �v���C���[�p�����[�^�[
	CObject2D* m_pWindow;						// �E�B���h�E�p�I�u�W�F�N�g2D�̃C���X�^���X
	CObject2D* m_pObject2D;		// �����p�I�u�W�F�N�g2D�̃C���X�^���X
	CFontString* m_pFont;						// �t�H���g
	CFontString* m_pParameter_Font[2];			// �p�����[�^�[�p�̃t�H���g
	CNumber* m_pSelect_Number;					// �I��ԍ�
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR2 m_size;							// �T�C�Y
	D3DXVECTOR2 m_MaxSize;						// �ő�T�C�Y
	D3DXCOLOR m_Color;							// �F

	std::vector<CFontString*> m_vpListChoice;	// �I�����|�C���^�̔z��

	int		m_nSelectIndex;						// ���Ԗڂ̃X�L�����I�����ꂽ���̕ۑ��ϐ�

	float	m_fBlinkSpeed;						// �_�ő��x
	float	m_fMoveX;							// �X���C�h�̈ړ���

	bool	m_bMaxSize;							// X��Y�̃T�C�Y���ő�l�܂ōs������
	bool    m_bSlideFlag;						// �X���C�h�t���O
	bool    m_bFontFlag;						// �t�H���g���g�p����Ă��邩
	bool    m_bLeftRight;						// ���E�ǂ����Ɉړ����邩
	bool    m_bSpawnWindow;						// �E�B���h�E���������ꂽ��
	bool    m_bStopFlag;						// �X���C�h�������ɒ�~����t���O
	bool    m_bExplanationUninit;				// �e�N�X�`���ƃt�H���g�̍폜�t���O
	bool    m_bPosDest;							// �ړI�̈ʒu�܂ōs�����̂�
	bool    m_bScale;							// �g��k���t���O
	bool	m_bDicision;						// ���肳�ꂽ��
	bool    m_bUninitFlag;						// �폜�t���O
	bool    m_bScaleReduce;						// �k��
	bool	m_bScaleExpansion;					// �g��
	bool    m_bSelect;
	bool    m_bTextureFlag;
};
#endif