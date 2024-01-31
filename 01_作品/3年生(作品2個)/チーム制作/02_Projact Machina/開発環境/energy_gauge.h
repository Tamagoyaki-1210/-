//==============================================================================================
//
// エネルギーゲージ	　　energy_gauge.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _ENERGY_GAUGE_H_
#define _ENERGY_GAUGE_H_

class CObject2D;

#include"gauge_manager.h"

class CEnergy_Gauge : public CGauge_Manager
{
public:
	CEnergy_Gauge();
	~CEnergy_Gauge() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CEnergy_Gauge* Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size);

	void Fluctuation() override;
	void Consumption_Gauge();			// エネルギーの消費
	void Avoidance_Energy();			// 回避時のエネルギー消費
	void Recovery_Pause(int count);		// ゲージ回復を一時停止

	void SetMaxEnerugy(const int enerugy);														// 最大エネルギーの設定
	void SetRecovery_Speed(const float recovery) { m_fRecovery_Speed = recovery; }				// 回復速度の設定
	void SetConsumption_Speed(const float consumption) { m_fConsumption_Speed = consumption; }	// 消費速度の設定
	void SetAvoidance_amount(const float avoidance) { m_fAvoidance = avoidance; }				// 回避時の消費量
	void SetReuse_Percent(const float reuse) { m_fReuse_Percent = reuse; }						// 全消費からの回復時に再利用できるタイミングの設定
	void SetRecovery_Interval(const float interval) { m_fRecovery_Interval = interval; }
	void SetDrawGauge(bool draw) override;
	void SetEnergyConsumed(const float value);
	void SetConsumption(const bool recovery) { m_bConsumption = recovery; }

	const bool GetConsumption() { return m_bAllConsumption; }		// エネルギーを全て消費しているか
	CObject2D *GetBackGauge() { return m_BackGauge; }

private:
	void Recovery_Gauge();		// エネルギーの回復
	void GaugeColor();			// ゲージの色

	int m_nMax_Enerugy;				// 最大エネルギー
	float m_fRecovery_Speed;		// 回復速度
	float m_fConsumption_Speed;		// 消費速度
	float m_fAvoidance;				// 回避時の消費量
	float m_fRecovery_Interval;		// 回復し始めるまでのインターバル
	float m_fReuse_Percent;			// 全消費からの回復時に再利用できるタイミング
	float m_fFluctuation;			// 増減するゲージの量
	int m_nInterval_Count;			// インターバルのカウント
	bool m_bConsumption;			// 消費中か
	bool m_bAllRecovery;			// 回復が出来る状態か
	bool m_bAllConsumption;			// エネルギーを全て消費した
	bool m_bRecovery_Pause;			// 回復の一時停止
	int m_Col_Count;					// 点滅のカウント
	int m_Pause_Count;				// 回復の一時停止中のカウント
	int m_BasePause_Count;			// 回復の一時停止中のカウントのベース

	CObject2D *m_BackGauge;		// ゲージの背景
	CObject2D *m_FrontGauge;	// ゲージの前景
};

#endif // !_ENERGY_GAUGE_H_
