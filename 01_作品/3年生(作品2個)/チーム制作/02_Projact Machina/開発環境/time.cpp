//=============================================================================
//
// time.h (タイム.h)
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "time.h"
#include "game.h"
#include "fade.h"
#include "number.h"
#include "input.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CTime::CTime()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTime::~CTime()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTime::Init()
{
	//==================================================
	// メンバ変数の初期化
	//==================================================  
	m_nTime = 0;				// 時間の値の初期化
	m_nZeroCount = 0;				// 0の数の初期化
	m_nCntTime = 0;

	//数字の生成
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + (70.0f * nCnt) + 50.0f / 2, m_pos.y, 0.0f), 30.0f, 30.0f,CObject::PRIORITY_SCREEN);
		//分割数,何番目か
		m_apNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
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
}

//=============================================================================
// 更新処理
//=============================================================================
void CTime::Update()
{
	//タイムの設定
	SetTime();
}

//=============================================================================
// タイムの設定
//=============================================================================
void CTime::SetTime()
{
	CApplication::MODE Mode = CApplication::GetModeType();
	if (Mode == CApplication::MODE_GAME)
	{
		m_nCntTime++;
		// 1秒 = 60f && 残り時間が0より大きい場合
		if (m_nCntTime > 60 /*&& m_nTime > 0*/)
		{
			m_nTime++;
			m_nCntTime = 0;
		}
		m_nKeepTime = m_nTime;
	}

	//if (m_nTime <= 0)
	//{
	//	CFade::SetFade(CApplication::MODE_RESULT, 0.05f);
	//}

		m_aPosTexU[0] = m_nTime % 1000 / 100;
		m_aPosTexU[1] = m_nTime % 100 / 10;
		m_aPosTexU[2] = m_nTime % 10 / 1;

		for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
		{
			//分割数,何番目か
			m_apNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
		}

		//// 先頭の0を消す処理
		//ZoroCount();
}

//=============================================================================
// タイムの設定
//=============================================================================
void CTime::SetTime(int time)
{
	m_aPosTexU[0] = time % 1000 / 100;
	m_aPosTexU[1] = time % 100 / 10;
	m_aPosTexU[2] = time % 10 / 1;

	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		//分割数,何番目か
		m_apNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
		// 先頭の0を消す処理
		ZoroCount();
	}
}

//=============================================================================
// 先頭の0を消す処理
//=============================================================================
void CTime::ZoroCount()
{
	if (m_apNumber[m_nZeroCount] != nullptr)
	{
		if (m_aPosTexU[m_nZeroCount] == 0 && m_nTime != 0 && m_nZeroCount <= 2)
		{
			m_apNumber[m_nZeroCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCount++;
		}
		else if (m_aPosTexU[m_nZeroCount] == 0 && m_nTime == 0 && m_nZeroCount <= 2)
		{
			m_apNumber[1]->SetTexPos(0.0f, 1.0f, 0.1f, 0.0f);
			m_apNumber[m_nZeroCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCount++;
		}
		if (m_nTime == 0)
		{
			m_apNumber[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//=============================================================================
// タイムの取得
//=============================================================================
int CTime::GetTime()
{
	return m_nKeepTime;
}

//=============================================================================
// タイムの生成
//=============================================================================
CTime* CTime::Create(D3DXVECTOR3 pos)
{
	//クラスの生成
	CTime* pTime = new CTime;

	//nullチェック
	if (pTime != nullptr)
	{
		pTime->SetPos(pos);
		//初期化処理
		pTime->Init();
	}
	else
	{
		assert(false);
	}

	return pTime;
}