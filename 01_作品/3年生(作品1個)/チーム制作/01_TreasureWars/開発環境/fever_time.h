//==============================================================================================
//
// フィーバータイム　　　fever_time.h
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

	void Fever_Time();		// フィーバータイム

	void SetFeverStart(int feverstart) { m_nFeverStart_Time = feverstart; }
	void SetCreateCoolTimeBase(int cooltime) { m_nCreate_CoolTime_Base = cooltime; }

private:
	bool m_bFever;					// フィーバー中かどうか
	int m_nFeverStart_Time;			// フィーバーが始まる時間
	int m_nCreate_CoolTime_Base;	// 宝石を生成する間隔の初期値
	int m_nCreate_CoolTime;			// 宝石を生成する間隔
};

#endif // !_FEVER_TIME_H_
