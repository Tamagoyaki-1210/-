//==============================================================================================
//
// �t�B�[�o�[�^�C���@�@�@fever_time.h
// tutida ryousei
//
//==============================================================================================
#ifndef _FEVER_TIME_H_
#define	_FEVER_TIME_H_

#include"object.h"

class CFever_Time : public CObject
{
public:
	CFever_Time();
	~CFever_Time();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CFever_Time *Create(const int fevertime, const int cooltime);

	void Fever_Time();		// �t�B�[�o�[�^�C��

	void SetFeverStart(int feverstart) { m_nFeverStart_Time = feverstart; }
	void SetCreateCoolTimeBase(int cooltime) { m_nCreate_CoolTime_Base = cooltime; }

private:
	bool m_bFever;					// �t�B�[�o�[�����ǂ���
	int m_nFeverStart_Time;			// �t�B�[�o�[���n�܂鎞��
	int m_nCreate_CoolTime_Base;	// ��΂𐶐�����Ԋu�̏����l
	int m_nCreate_CoolTime;			// ��΂𐶐�����Ԋu
};

#endif // !_FEVER_TIME_H_
