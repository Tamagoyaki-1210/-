//==============================================================================================
//
// �G���g���[�@�@�@entry.h
// tutida ryousei
//
//==============================================================================================
#ifndef _ENTRY_H_
#define	_ENTRY_H_

#include"mode.h"
#include "charmanager.h"

class CCharacter;
class CFile;
class CObject2D;
class CFontString;

class CEntry : public CMode
{
public:
	CEntry();
	~CEntry();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CEntry *Create();

	void TutorialCol();			// �`���[�g���A���̓����蔻��
	void Tutorial();			// �`���[�g���A���̏���
	void TutorialStart();		// �`���[�g���A���̊J�n����
	void TutorialEnd();			// �`���[�g���A���̏I������
	void TutorialSelect();		// �`���[�g���A���̑I������
	void TVDisplayStart();		// TV��ʂ̕\���̊J�n����
	void TVDisplayEnd();		// TV��ʂ̕\���̏I������
	void PlayerIn(int index);	// �v���C���[�̓���
	void Act_Limit();			// �s������

	void SetEntryUX(const bool use);	// �G���g���[��ʂ̑S�Ă�UX�̐ݒ菈��

	static bool GetEntry(int index) { return m_bEntry[index]; }
	static bool GetIn(int index) { return m_bIn[index]; }
	static bool GetTutorial() { return m_bTutorial; }

private:
	static const D3DXVECTOR3 TVSizeMax;							// TV��ʂ̍ő�T�C�Y

	static const float TVDisplaySpeed;							// TV��ʂ̕\�����x
	static const float TVStartAnchorAttenuation;				// TV�J�n�A���J�[����
	static const float TVEndAnchorAttenuation;					// TV�I���A���J�[����
	static const int MaxTutorialPage = 3;						// �`���[�g���A��3�y�[�W
	static const int TVDisplayEndTimer = 20;					// TV��ʂ̕\���̏I������

	static bool m_bEntry[CCharManager::Max_Character];			// �G���g���[������
	static bool m_bIn[CCharManager::Max_Character];				// ���ꂵ����
	D3DXVECTOR3 m_CharPos[CCharManager::Max_Character];			// �L�����N�^�[�̈ʒu
	D3DXVECTOR3 m_CharPosDest[CCharManager::Max_Character];		// �L�����N�^�[�̖ړI�̈ʒu
	CCharacter* m_pChara[CCharManager::Max_Character];			// �L�����N�^�[�̏��

	static bool m_bTutorial;									// �`���[�g���A������
	bool m_bTutorialStart;										// �`���[�g���A���̊J�n����
	bool m_bTutorialEnd;										// �e���r��ʂ̕\���̏I������
	bool m_bTVDisplayStart;										// �e���r��ʂ̕\���̊J�n����
	bool m_bTVDisplayEnd;										// �e���r��ʂ̕\���̏I������
	CFontString* m_apTutorialFont[2];							// �`���[�g���A����
	CFontString* m_pGameStartFont;								// �Q�[���X�^�[�g��
	CObject2D* m_apPlayerNumber[CCharManager::Max_Character];	// �v���C���[�ԍ�
	CObject2D* m_apABotton[CCharManager::Max_Character];		// �v���C���[�l������A�{�^��
	CFontString* m_apOKString[CCharManager::Max_Character];		// �v���C���[�l������OK
	CObject2D* m_pABotton;										// A�{�^��
	CObject2D* m_pTutorial;										// �`���[�g���A�����
	CObject2D* m_apArrow[2];									// ���2��

	int m_nSellect;												// ���݂̑I�����
	int m_nDisplayEndCounter;									// �e���r��ʂ̕\���̏I������
	int m_nTutorialInitiative;									// �`���[�g���A���̎哱��
};

#endif // !_ENTRY_H_
