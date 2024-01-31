//==============================================
//
// time.h (�^�C��.h)
// Author : Saito Shian
//
//==============================================
#ifndef  _TIME_H_				//���̃}�N����`������ĂȂ�������
#define  _TIME_H_				//��d�C���N���[�h�h�~�̃}�N����`

//==============================================
// �C���N���[�h
//==============================================
#include "main.h"
#include "object.h"

//==============================================
// �O���錾
//==============================================
class CNumber;

//==============================================
// �X�R�A�N���X
//==============================================
class CTime
{
public:
	static const int TIME_DIGIT = 3;	// ���Ԃ̌���
	static const int MAX_TIME = 180;	// �ő厞��

	//****************
	// public�Ȋ֐�
	//****************
	CTime();							// �X�R�A�̃R���X�g���N�^
	~CTime();							// �X�R�A�̃f�X�g���N�^

	HRESULT Init();						// ����������
	void Uninit();						// �I������
	void Update();						// �X�V����

	void SetTime();						// ���Ԃ̐ݒ�
	void SetTime(int time);
	void ZoroCount();					// �擪��0����������
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	int GetTime();						// ���Ԃ̎擾

	static CTime* Create(D3DXVECTOR3 pos);		// ����

private:
	//****************
	// private�ȕϐ�
	//****************
	CNumber* m_apNumber[TIME_DIGIT];		// �i���o�[�̌���
	D3DXVECTOR3 m_pos;						// �ʒu

	int m_aPosTexU[TIME_DIGIT];				// ����
	int m_nCntTime;							// ���Ԃ̃J�E���g
	int m_nTime;							// ���Ԃ̒l
	int m_nKeepTime;						// ���Ԃ̕ۑ��p�ϐ�
	int m_nZeroCount;						// 0�̐�

};
#endif
