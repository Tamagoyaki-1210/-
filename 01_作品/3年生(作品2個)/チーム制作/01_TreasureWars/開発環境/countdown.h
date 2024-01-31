//==============================================================================================
//
// �J�E���g�_�E���@�@�@countdown.h
// tutida ryousei
//
//==============================================================================================
#ifndef _COUNTDOWN_H_
#define	_COUNTDOWN_H_

class CNumber;
class CObject2D;

#include"object.h"

class CCountdown : public CObject
{
public:
	CCountdown();
	~CCountdown();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CCountdown *Create();

	void SetCountdown(int countTime);

private:
	CNumber *m_pNumber;			// �i���o�[
	int m_nCountTime;			// �n�܂�܂ł̎���
	int m_nCountFrame;			// ���Ԃ̃J�E���g
	int m_nStart_Count;			// �X�^�[�g��\�����鎞��
	CObject2D *m_pObject2D;		// �X�^�[�g
	D3DXVECTOR3 m_Start_Size;	// �X�^�[�g�̃T�C�Y
	bool m_bDisplay_Start;		// �X�^�[�g��\��������
	bool m_bStartScale;			// �X�^�[�g�̊g�唻��
};

#endif // !_COUNTDOWN_H_
