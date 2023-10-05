//==============================================================================================
//
// カウントダウン　　　countdown.cpp
// tutida ryousei
//
//==============================================================================================
#include"countdown.h"
#include"number.h"
#include "application.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CCountdown::CCountdown() : CObject(3)
{
	m_nCountTime = 0;			// 始まるまでの時間
	m_nCountFrame = 0;			// 時間のカウント
	m_nStart_Count = 0;			// スタートを表示する時間
	m_pObject2D = nullptr;		// スタート
	m_Start_Size = { 0.0f,0.0f,0.0f };	// スタートのサイズ
	m_bDisplay_Start = false;	// スタートを表示したか
	m_bStartScale = true;	// スタートの拡大判定
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CCountdown::~CCountdown()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CCountdown::Init()
{
	m_nCountTime = 3;		// 始まるまでの時間
	m_Start_Size = { 250.0f,0.0f,0.0f };	// スタートのサイズ
	m_pObject2D = nullptr;	// スタート

	// ナンバーの生成
	m_pNumber = CNumber::Create({ 1280.0f / 2,720.0f / 2,0.0f }, 100.0f, 100.0f);

	// ナンバーの色
	m_pNumber->SetCol({ 1.0f,1.0f,1.0f,1.0f });

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CCountdown::Uninit()
{
	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CCountdown::Update()
{
	// カウントダウンが終了するまで
	if (m_bDisplay_Start == false)
	{
		// 1秒経った
		if (m_nCountFrame % 60 == 0)
		{
			// 最初ではない場合
			if (m_nCountFrame != 0)
			{
				// 一秒減らす
				m_nCountTime--;
			}

			// カウントが0になった
			if (m_nCountTime <= 0)
			{
				// ゲーム開始SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_GAMESTART);

				// ナンバーを終了させる
				m_pNumber->Uninit();
				m_pNumber = nullptr;

				// スタート
				CApplication::SetGameStart(true);

				// スタートを表示
				m_pObject2D = CObject2D::Create({ 1280.0f / 2,720.0f / 2,0.0f }, m_Start_Size);
				m_pObject2D->SetTexture(CTexture::TEXTURE_START);

				// スタートを表示したか
				m_bDisplay_Start = true;
			}
			// カウントが0ではない場合
			else
			{
				// カウントダウンの設定
				SetCountdown(m_nCountTime);
			}
		}
		// 60フレームをカウントする
		m_nCountFrame++;
	}
	else
	{
		// 拡大の判定
		if (m_bStartScale == true)
		{
			m_Start_Size.y += 2;

			// スタートのサイズを変える
			if (m_Start_Size.y > 70.0f)
			{
				m_bStartScale = false;
			}
		}
		else
		{
			m_Start_Size.y -= 2;
		}

		// 大きさが0以下になってしまった場合
		if (m_Start_Size.y <= 0.0f)
		{
			// スタートの終了処理
			m_pObject2D->Uninit();
			m_pObject2D = nullptr;

			// 自身の終了処理
			CCountdown::Uninit();
		}
		// 大きさが残っている場合
		else
		{
			// サイズの更新
			m_pObject2D->SetSize(m_Start_Size.x, m_Start_Size.y);
		}
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CCountdown::Draw()
{

}

//==============================================================================================
// 生成処理
//==============================================================================================
CCountdown *CCountdown::Create()
{
	CCountdown *pCountdown = nullptr;

	pCountdown = new CCountdown;

	if (pCountdown != nullptr)
	{
		pCountdown->Init();
	}

	return pCountdown;
}

//==============================================================================================
// カウントダウンの設定
//==============================================================================================
void CCountdown::SetCountdown(int countTime)
{
	// カウントダウンSE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);

	// テクスチャ座標の設定
	m_pNumber->SetTexPos(0.0f, 1.0f, 0.1f * countTime + 0.1f, countTime * 0.1f);
}