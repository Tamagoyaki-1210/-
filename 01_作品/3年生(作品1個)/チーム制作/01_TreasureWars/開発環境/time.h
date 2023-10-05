//==============================================================================
//
// time.h
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2�d�C���N���[�h�K�[�h
//==============================================================================
#ifndef _TIME_H_
#define	_TIME_H_

//==============================================================================
// �C���N���[�h
//==============================================================================
#include "object.h"
#include "number.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define TIME_DIGIT		 (3)		//���Ԃ̌���

//==============================================================================
// ���Ԃ�\������N���X
//==============================================================================
class CTime
{
	static const int FeverStart_Count = 30;	// �t�B�[�o�[�^�C�����n�܂鎞��
	static const int GemCreate_Count = 5;	// ��΂𐶐�����Ԋu
	static const int FlashingTime = 55;
	static const int LastSpurtTime = 500;
	static const int MaxTime = 180;
	static const int Warning_Blink_Interval = 30;	// �x��̓_�ŊԊu
public:
	enum Flash
	{
		Flash_In = 0,
		Flash_Out
	};
	CTime();				// �R���X�g���N�^
	~CTime();				// �f�X�g���N�^

	HRESULT Init();			// ����������
	void Uninit();			// �I������
	void Update();			// �X�V����

	void SetTime();							// ���Ԃ̐ݒ�
	void SetRemainingTime(int time);		// �c�莞�Ԃ̐ݒ菈��
	void ZoroCount();						// �擪��0����������
	void Warning(const int time);			// �x��̏���

	static CTime *Create();	//����

	// �Q�b�^�[
	int GetTime() { return m_nTime; }

private:
	CNumber *m_apNumber[TIME_DIGIT];		// ���Ԃ̐���(����)
	CObject2D *m_pObject2D;					// object2D�̃C���X�^���X
	CObject2D *m_pWarning;					// �x��I�u�W�F�N�g

	Flash m_Type;							// ���

	int m_nTime;							// ���Ԃ̒l
	int m_nCntTime;							// ���Ԃ̃J�E���^�[
	int m_nZeroCount;						// 0�̐�
	int m_count;							// ��ԕω��̃J�E���g
	int m_LastCount;						// �ړ��܂ł̎���
	int m_aPosTexU[TIME_DIGIT];				// ����
	int m_nTimeUp_Count;					// �^�C���A�b�v��\�����鎞��

	float m_nSizeX;							// X�̃T�C�Y
	float m_nSizeY;							// Y�̃T�C�Y
	float m_nAlpha;							// �����x
	float m_moveY;							// Y�̈ړ���
	float m_moveX;							// X�̈ړ���

	bool m_bTimeUp;							// �^�C���A�b�v�������ǂ����̔���
	bool m_bLast;							// �c�莞�Ԃ̔���
};

#endif
