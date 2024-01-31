//==============================================================================================
//
// エネルギーゲージ	　　energy_gauge.cpp
// Author : tutida ryousei
//
//==============================================================================================
#include"energy_gauge.h"
#include"debugProc.h"
#include "object2D.h"
#include"player_manager.h"
#include"application.h"
#include "tutorial.h"
#include "game.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CEnergy_Gauge::CEnergy_Gauge()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CEnergy_Gauge::~CEnergy_Gauge()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CEnergy_Gauge::Init()
{
	CGauge_Manager::Init();

	const D3DXVECTOR3 pos = GetGaugePos();
	const D3DXVECTOR2 size = GetGaugeSize();

	// 後ろのゲージ
	m_BackGauge = CObject2D::Create(pos, size, PRIORITY_FRONT);
	// 後ろのゲージの色
	m_BackGauge->SetCol({ 0.0f,0.0f,0.0f,1.0f });

	// 前方のゲージ
	m_FrontGauge = CObject2D::Create(pos, size, PRIORITY_FRONT);
	// 前方のゲージの色
	m_FrontGauge->SetCol({ 1.0f, 1.0f, 1.0f,1.0f });

	m_fRecovery_Interval = 180.0f;	// 回復し始めるまでのインターバル
	m_fReuse_Percent = 30.0f;		// 全消費からの回復時に再利用できるタイミング
	m_bConsumption = false;			// 消費中か
	m_bAllRecovery = true;			// 回復が出来る状態か
	m_bAllConsumption = false;		// エネルギーを全て消費した
	m_bRecovery_Pause = false;		// 回復が一時停止中

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CEnergy_Gauge::Uninit()
{
	CGauge_Manager::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CEnergy_Gauge::Update()
{
	// ゲージを回復する
	Recovery_Gauge();

	// 回復するか
	m_bConsumption = false;

	// ゲージの増減
	Fluctuation();

	// ゲージの色の設定
	GaugeColor();

	CGauge_Manager::Update();


	//CDebugProc::Print("プレイヤースタミナ：%f / %d\n", m_fFluctuation, m_nMax_Enerugy);
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CEnergy_Gauge::Draw()
{
	CGauge_Manager::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CEnergy_Gauge* CEnergy_Gauge::Create(const D3DXVECTOR3 &pos, D3DXVECTOR2 size)
{
	CEnergy_Gauge *pEnergy_Gauge = new CEnergy_Gauge;

	if (pEnergy_Gauge != nullptr)
	{
		pEnergy_Gauge->SetGaugePos(pos);
		pEnergy_Gauge->SetGaugeSize(size);
		pEnergy_Gauge->Init();
	}

	return pEnergy_Gauge;
}

//==============================================================================================
// ゲージの増減
//==============================================================================================
void CEnergy_Gauge::Fluctuation()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	CPlayer* pPlayer = nullptr;

	// モード毎にプレイヤーを読み込む
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
	}

	if (m_bRecovery_Pause)
	{
		m_Pause_Count++;

		if (m_Pause_Count >= m_BasePause_Count)
		{
			m_bRecovery_Pause = false;
			m_bAllRecovery = true;		// 回復できない状態にする
			m_bAllConsumption = false;	// エネルギーを全て消費した
			m_Pause_Count = 0;
		}
	}
	else if (m_bAllConsumption && !m_bAllRecovery && !m_bRecovery_Pause)
	{
		// 最後まで減らす
		m_fFluctuation = (float)m_nMax_Enerugy;
	}

	// 現在のエネルギー残量
	float fEnergy = (float)m_nMax_Enerugy - m_fFluctuation;

	// 現在のエネルギー残量の割合
	float fEnergy_Percent = fEnergy / m_nMax_Enerugy * 100;

	// ゲージサイズをエネルギーの割合に合わせる
	float fGeuge_Size = GetGaugeSize().y * fEnergy_Percent / 100.0f;

	// ゲージの増減
	//m_FrontGauge->SetSubSize({ GetGaugeSize().x - fGeuge_Size ,0.0f });
	m_FrontGauge->SetSubSize({ 0.0 ,-GetGaugeSize().y + fGeuge_Size });
}

//==============================================================================================
// エネルギーの回復
//==============================================================================================
void CEnergy_Gauge::Recovery_Gauge()
{
	if (m_bAllRecovery)
	{
		if (!m_bConsumption && m_fFluctuation > 0.0f)
			// 回復するエネルギー量
			m_fFluctuation -= m_fRecovery_Speed;

		// 現在のゲージ残量の割合
		float Gauge_Percent = (m_nMax_Enerugy - m_fFluctuation) / m_nMax_Enerugy * 100;

		// 回復途中エネルギーを使えるようにするタイミング
		if (Gauge_Percent >= m_fReuse_Percent)
			m_bAllConsumption = false;

		// 全回復した
		if (m_fFluctuation <= 0)
			m_fFluctuation = 0.0f;
	}
	else if (!m_bAllRecovery && !m_bRecovery_Pause)
	{
		// エネルギーが切れた時
		if (m_nInterval_Count == 0)
		{
			// 警報SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WARNING);
		}

		// インターバルのカウント
		m_nInterval_Count++;

		if (m_nInterval_Count >= m_fRecovery_Interval)
		{
			// 回復できる状態にする
			m_bAllRecovery = true;
			m_nInterval_Count = 0;
		}
	}
}

//==============================================================================================
// エネルギーの消費
//==============================================================================================
void CEnergy_Gauge::Consumption_Gauge()
{
	if (!m_bAllConsumption)
	{
		// 消費するエネルギー量
		if (m_fFluctuation <= m_nMax_Enerugy)
		{
			// 消費量の加算
			m_fFluctuation += m_fConsumption_Speed;

			// エネルギー消費中
			m_bConsumption = true;
		}
		else
		{
			m_bAllRecovery = false;		// 回復できない状態にする
			m_bAllConsumption = true;	// エネルギーを全て消費した
		}
	}
}

//==============================================================================================
// 回避時のエネルギー消費
//==============================================================================================
void CEnergy_Gauge::Avoidance_Energy()
{
	if (!m_bAllConsumption)
	{
		// 消費するエネルギー量
		m_fFluctuation += m_fAvoidance;

		if (m_fFluctuation < m_nMax_Enerugy)
			// エネルギー消費中
			m_bConsumption = true;
		else
		{
			m_bAllRecovery = false;		// 回復できない状態にする
			m_bAllConsumption = true;	// エネルギーを全て消費した
		}
	}
}

//==============================================================================================
// ゲージの色
//==============================================================================================
void CEnergy_Gauge::GaugeColor()
{
	// 現在のゲージ残量の割合
	float Gauge_Percent = (m_nMax_Enerugy - m_fFluctuation) / m_nMax_Enerugy * 100;

	// 色の設定
	if (Gauge_Percent <= 25.0f || m_bAllConsumption && !m_bRecovery_Pause)
		// 残量25%以下or全て消費した後、全回復するまで(赤)
		m_FrontGauge->SetCol({ 1.0f,0.0f,0.0f,1.0f });
	else if (Gauge_Percent > 20.0f || !m_bAllConsumption)
		// 残量26%以上(橙)
		m_FrontGauge->SetCol({ 1.0f,0.5f,0.0f,1.0f });

	// 後ろのゲージの色
	m_BackGauge->SetCol({ 0.0f,0.0f,0.0f,1.0f });

	// エネルギーを全て消費した
	if (!m_bAllRecovery && !m_bRecovery_Pause)
	{
		// 赤く点滅させる
		m_Col_Count++;
		float fRed = sinf(m_Col_Count * 0.07f);
		m_BackGauge->SetCol({ fRed,0.0f,0.0f,1.0f });
	}
	else
		// 点滅のリセット
		m_Col_Count = 0;
}

//==============================================================================================
// ゲージ回復を一時停止
//==============================================================================================
void CEnergy_Gauge::Recovery_Pause(int count)
{
	if (m_bAllRecovery && !m_bAllConsumption)
	{
		m_bAllRecovery = false;		// 回復できない状態にする
		m_bAllConsumption = true;	// エネルギーを全て消費した
		m_bRecovery_Pause = true;	// 停止中

		// 停止する時間
		m_BasePause_Count = count;
	}
}

//==============================================================================================
// 最大エネルギーの設定
//==============================================================================================
void CEnergy_Gauge::SetMaxEnerugy(const int enerugy)
{
	// 現在のエネルギーが最大エネルギーより多い場合
	if (m_fFluctuation > enerugy)
	{
		m_fFluctuation = (float)enerugy;
	}
	m_nMax_Enerugy = enerugy;
}

//==============================================================================================
// ゲージ描画の設定
//==============================================================================================
void CEnergy_Gauge::SetDrawGauge(bool draw)
{
	m_BackGauge->SetDrawFlag(draw);
	m_FrontGauge->SetDrawFlag(draw);
}

//==============================================================================================
// エネルギー消費量設定処理
//==============================================================================================
void CEnergy_Gauge::SetEnergyConsumed(const float value)
{
	// オーバーヒートしていない場合
	if (!m_bAllConsumption)
	{
		// 減少量を加算する
		m_fFluctuation += value;

		// 現在のエネルギー残量
		float fEnergy = (float)m_nMax_Enerugy - m_fFluctuation;

		// 全て消費してしまった場合
		if (fEnergy <= 0.0f)
		{
			m_fFluctuation = (float)m_nMax_Enerugy;
			m_bAllConsumption = true;
			m_bAllRecovery = false;		// 回復できない状態にする
		}


		// 現在のエネルギー残量の割合
		float fEnergy_Percent = fEnergy / m_nMax_Enerugy * 100;

		// ゲージサイズをエネルギーの割合に合わせる
		float fGeuge_Size = GetGaugeSize().y * fEnergy_Percent / 100.0f;

		// ゲージの増減
		//m_FrontGauge->SetSubSize({ GetGaugeSize().x - fGeuge_Size ,0.0f });
		m_FrontGauge->SetSubSize({ 0.0 ,-GetGaugeSize().y + fGeuge_Size });

		// エネルギー消費中
		m_bConsumption = true;
	}
}