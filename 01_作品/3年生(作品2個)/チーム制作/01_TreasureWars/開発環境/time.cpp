//==============================================================================
//
// time.cpp
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// インクルード
//==============================================================================
#include "application.h"
#include "renderer.h"
#include "time.h"
#include "countdown.h"
#include "game.h"
#include "fade.h"
#include "fever_time.h"

//==============================================================================
// 静的メンバ変数宣言
//==============================================================================

//==============================================================================
// コンストラクタ
//==============================================================================
CTime::CTime()
{

}

//==============================================================================
// デストラクタ
//==============================================================================
CTime::~CTime()
{
}

//==============================================================================
// 初期化処理
//==============================================================================
HRESULT CTime::Init()
{
	//==================================================
	// メンバ変数の初期化
	//==================================================  
	m_nTime = MaxTime;				// 時間の値の初期化
	m_nCntTime = 0;					// 時間のカウントの初期化
	m_nZeroCount = 0;				// 0の数の初期化
	m_count = 300;					// 状態変化のカウントの初期化
	m_LastCount = LastSpurtTime;	// 移動までの時間の初期化
	m_nSizeX = 50.0f;				// Xサイズの初期化
	m_nSizeY = 50.0f;				// Yサイズの初期化
	m_nAlpha = 1.0f;				// 透明度の初期化
	m_Type = Flash_In;				// 状態の初期化

	m_bTimeUp = false;				// タイムアップしたかどうかの判定
	m_bLast = false;				// 残り時間の判定

	//数字の生成
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(540.0f + (70.0f * nCnt) + m_nSizeX / 2, 50.0f, 0.0f), m_nSizeX, m_nSizeY);
	}

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void CTime::Uninit()
{
	//メンバ変数の初期化
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}

	// オブジェクトが使用中の場合
	if (m_pObject2D != nullptr)
	{
		m_pObject2D->Uninit();
		m_pObject2D = nullptr;
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void CTime::Update()
{
	//タイムの設定
	SetTime();

	//残り時間の設定
	SetRemainingTime(30);

	// 警報の処理
	Warning(30);
}

//==============================================================================
// タイムの設定
//==============================================================================
void CTime::SetTime()
{
	// スタートするまでは時間を進めない
	if (CApplication::GetGameStart())
	{
		m_nCntTime++;

		// 1秒 = 60f && 残り時間が0より大きい場合
		if (m_nCntTime > 60 && m_nTime > 0)
		{
			m_nTime--;
			m_nCntTime = 0;
		}

		// 残り時間が0以下になった
		if(m_nTime <= 0)
		{
			// 笛_終了SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_FINISH);

			// タイムアップを表示
			m_pObject2D = CObject2D::Create({ 1280.0f / 2,720.0f / 2,0.0f }, { 300.0f, 200.0f, 0.0f });
			m_pObject2D->SetTexture(CTexture::TEXTURE_TIMEUP);

			// ゲームを終了させる
			CApplication::SetGameStart(false);

			// タイムアップさせる
			m_bTimeUp = true;
		}

	}

	// タイムアップしている場合
	if (m_bTimeUp)
	{
		m_nTimeUp_Count++;

		// リザルトにフェード
		if (m_nTimeUp_Count == 120)
		{
			CFade::SetFade(CApplication::MODE_RESULT, 0.01f);
		}
	}

	if (m_nTime != 0)
	{
		m_aPosTexU[0] = m_nTime % 1000 / 100;
		m_aPosTexU[1] = m_nTime % 100 / 10;
		m_aPosTexU[2] = m_nTime % 10 / 1;

		for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
		{
			//分割数,何番目か
			m_apNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
		}

		// 先頭の0を消す処理
		ZoroCount();
	}
}

//==============================================================================
// 残り時間の設定処理
//==============================================================================
void CTime::SetRemainingTime(int time)
{
	// 初期化
	m_nZeroCount = 0;

	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		//位置の取得
		D3DXVECTOR3 pos = m_apNumber[nCnt]->Getposition();

		// timeの移動
		if (m_nTime <= time + 2)
		{
			if (pos.y < 360.0f && m_bLast == false)
			{
				m_moveY -= 0.015f;
				m_nSizeX -= 0.05f;
				m_nSizeY -= 0.05f;
			}
			else if (pos.y > 360.0f && m_bLast == false)
			{
				m_moveY = 0.0f;		
				m_nSizeX += 0.5f;
				m_nSizeY += 0.5f;
				m_count--;
				if (m_nTime <= time && m_count <= 0)
				{
					m_bLast = true;
				}
			}

			if (pos.y > 250.0f && m_nTime > 30)
			{
				m_apNumber[1]->SetMove(D3DXVECTOR3(m_moveX += 1.0f, 0.0f, 0.0f));
				m_apNumber[2]->SetMove(D3DXVECTOR3(m_moveX -= 1.0f, 0.0f, 0.0f));
			}

			if (m_bLast == true && pos.y > 50.0f)
			{
				m_moveY += 0.05f;
				m_nSizeX -= 0.45f;
				m_nSizeY -= 0.45f;
				m_apNumber[1]->SetMove(D3DXVECTOR3(m_moveX -= 0.35f, 0.0f, 0.0f));
				m_apNumber[2]->SetMove(D3DXVECTOR3(m_moveX += 0.35f, 0.0f, 0.0f));
			}
			else if (pos.y < 50.0f)
			{
				m_moveY = 0.0f;
				m_moveX = 0.0f;
				m_nSizeX = 50.0f;
				m_nSizeY = 50.0f;
			}
		}

		// 状態管理
		if (m_nTime <= time)
		{
			m_count--;
			if (m_Type == Flash_In && m_nTime % 2 == 1)
			{
				m_nAlpha -= 0.005f;
				if (m_nAlpha <= 0.1f)
				{
					m_Type = Flash_Out;
					m_count = FlashingTime;
				}
			}
			else if (m_Type == Flash_Out && m_nTime % 2 <= 0)
			{
				m_nAlpha += 0.005f;
				if (m_nAlpha >= 1.0f)
				{
					m_Type = Flash_In;
					m_count = FlashingTime;
				}
			}

			//色の設定
			m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_nAlpha));

			// 先頭の0を消す処理
			ZoroCount();	
		}

		//サイズの設定
		m_apNumber[nCnt]->SetSize(m_nSizeX, m_nSizeY);
		//移動の設定
		m_apNumber[nCnt]->SetMove(D3DXVECTOR3(m_moveX, m_moveY, 0.0f));

	}
}

//==============================================================================
// 警報の処理
//==============================================================================
void CTime::Warning(const int time)
{
	// フィーバータイムの2秒前になると
	if ((m_nTime == time + 2) && m_nCntTime == 0)
	{
		m_pWarning = CObject2D::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), D3DXVECTOR3(910.0f, 510.0f, 0.0f), 5);
		m_pWarning->SetCol({ 1.0f, 0.0f, 0.0f, 0.0f });

		// 警報SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WARNING);
	}
	// フィーバータイムが始まった時
	else if ((m_nTime == time) && m_nCntTime == 0)
	{
		m_pWarning->Uninit();
		m_pWarning = nullptr;
		return;
	}

	// 警報が使用中の場合
	if (m_pWarning != nullptr)
	{
		const float flpha = 0.2f / Warning_Blink_Interval;
		m_pWarning->AddAlpha((m_nCntTime % (Warning_Blink_Interval * 2)) < Warning_Blink_Interval ? flpha : -flpha);
	}
}

//==============================================================================
// 先頭の0を消す処理
//==============================================================================
void CTime::ZoroCount()
{
	if (m_apNumber[m_nZeroCount] != nullptr)
	{
		if (m_aPosTexU[m_nZeroCount] == 0 && m_nTime != 0)
		{
			m_apNumber[m_nZeroCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCount++;
		}
		else if (m_aPosTexU[m_nZeroCount] == 0 && m_nTime == 0)
		{
			m_apNumber[2]->SetTexPos(0.0f, 1.0f, 0.1f, 0.0f);
			m_apNumber[m_nZeroCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCount++;
		}
	}
}

//==============================================================================
// タイムの生成
//==============================================================================
CTime *CTime::Create()
{
	CTime *pTime = nullptr;

	//クラスの生成
	pTime = new CTime;				//タイムのインスタンス生成

	//nullチェック
	if (pTime != nullptr)
	{
		//初期化処理
		pTime->Init();
	}
	else
	{
		assert(false);
	}

	return pTime;
}