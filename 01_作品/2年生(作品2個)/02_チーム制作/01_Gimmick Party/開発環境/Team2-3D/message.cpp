//=============================================================================
//
// message.cpp
// Author : tanimoto kosuke
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "message.h"
#include "game.h"
#include "application.h"
#include "menu.h"

bool CMessage::m_bStart = false;

//=====================================
// デフォルトコンストラクタ
//=====================================
CMessage::CMessage()
{

}

//=====================================
// デストラクタ
//=====================================
CMessage::~CMessage()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CMessage::Init(void)
{
	m_nMessageIdx = 0;				// 勝者番号の初期化
	m_nMessageCounter = 0;			// メッセージ表示時間の初期化
	m_nNum = 0;						// カウントダウン現在位置の初期化
	m_bStart = false;				// ゲームスタート判定の初期化
	m_type = MESSAGE_COUNTDOWN;		// メッセージ種類の初期化

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CMessage::Uninit(void)
{
	if (m_pObj2D != nullptr)
	{// オブジェクト2Dの終了
		m_pObj2D->Release();
		m_pObj2D = nullptr;
	}
	if (m_pPlayer2D != nullptr)
	{// 勝者画像の終了
		m_pPlayer2D->Release();
		m_pPlayer2D = nullptr;
	}
	m_nMessageIdx = 0;				// メッセージ番号の終了
	m_nMessageCounter = 0;			// メッセージ表示時間の終了
	m_nNum = 0;						// カウントダウン現在位置の終了
	m_bStart = false;				// ゲームスタート判定の終了
	m_type = MESSAGE_COUNTDOWN;		// メッセージ種類の終了
}

//=====================================
// 更新処理
//=====================================
void CMessage::Update(void)
{
	if (CApplication::GetPause() == false)
	{	// ポーズ中でない場合のみ更新
		if (m_nMessageCounter > 0)
		{// カウントが0より上の場合
			m_nMessageCounter--;

			if (m_nMessageCounter <= 0)
			{// カウントが0以下になった場合
				if (m_type == MESSAGE_COUNTDOWN)
				{// カウントダウン処理の場合
					m_nNum--;
					if (m_nNum > 0)
					{// カウントダウンが0より大きい場合
						m_nMessageCounter = CountDownLife;	// カウントダウン用の寿命を代入
						m_pObj2D->SetAnimPattern(m_nNum);	// 現在のカウントダウンを設定
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
						CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_CHEERS01);
					}
					else
					{// カウントダウンが0以下の場合
						Destroy();			// 破棄処理
						StartMessage();		// スタートメッセージ
					}
				}
				else if (m_type == MESSAGE_GOAL)
				{// ゴール処理の場合
					Destroy();		// 破棄処理
					WinMessage();	// 勝者メッセージ
				}
				else if (m_type == MESSAGE_WIN)
				{// 勝者処理の場合
					CMenu::SetResult();	// リザルト用メニュー設定処理
				}
				else
				{// それ以外の場合
					Destroy();		// 破棄処理
				}
			}
		}
	}
}

//=====================================
// カウントダウンメッセージ表示処理
//=====================================
void CMessage::SetCountDown(int nNum)
{
	if (m_pObj2D == nullptr)
	{// オブジェクト2Dが未使用の場合生成
		m_pObj2D = CObject_2D::Create();
		m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f));
		m_pObj2D->SetSize(D3DXVECTOR2(200.0f, 150.0f));
		m_pObj2D->SetTexture(CObject::TEXTURE_NUMBERS);
		m_pObj2D->SetTextureParameter(1, 5, 3, INT_MAX);
		m_pObj2D->SetPriority(5);
		m_nNum = nNum;
		m_pObj2D->SetAnimPattern(nNum);
		m_nMessageCounter = CountDownLife;
		m_type = MESSAGE_COUNTDOWN;

		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_COUNTDOWN);
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_CHEERS01);
	}
}

//=====================================
// スタートメッセージ表示処理
//=====================================
void CMessage::StartMessage(void)
{
	if (m_pObj2D == nullptr)
	{// オブジェクト2Dが未使用の場合生成
		m_pObj2D = CObject_2D::Create();
		m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 300.0f, 0.0f));
		m_pObj2D->SetSize(D3DXVECTOR2(360.0f, 200.0f));
		m_pObj2D->SetTexture(CObject::TEXTURE_MESSAGE_START);
		m_pObj2D->SetPriority(5);
		m_nMessageCounter = 30;
		m_type = MESSAGE_START;
		m_bStart = true;
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_GAMESTART);
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_PISTOL);
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_CHEERS02);
	}
}

//=====================================
// ゴールメッセージ表示処理
//=====================================
void CMessage::GoalMessage(int nMessageIdx)
{
	if (m_pObj2D == nullptr)
	{// オブジェクト2Dが未使用の場合生成
		m_pObj2D = CObject_2D::Create();
		m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f));
		m_pObj2D->SetSize(D3DXVECTOR2(300.0f, 100.0f));
		m_pObj2D->SetTexture(CObject::TEXTURE_MESSAGE_GOAL);
		m_pObj2D->SetPriority(5);
		m_nMessageCounter = 120;
		m_type = MESSAGE_GOAL;
		m_bStart = false;
		// 勝敗番号を代入する
		m_nMessageIdx = nMessageIdx;

		CApplication::GetSound()->Stop();

		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WHISTLE_FINISH);
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_CHEERS02);

		// ゲーム終了判定
		CGame::SetEndGame();
	}
}


//=====================================
// ゴールメッセージ表示処理
//=====================================
void CMessage::WinMessage()
{
	if (m_pObj2D == nullptr)
	{// オブジェクト2Dが未使用の場合生成
		if (m_nMessageIdx != 0)
		{// 同点ではない場合
			// WIN生成
			m_pObj2D = CObject_2D::Create();
			m_pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 8 * 5, 100.0f, 0.0f));
			m_pObj2D->SetSize(D3DXVECTOR2(180.0f, 60.0f));
			m_pObj2D->SetTexture(CObject::TEXTURE_WINNER);
			m_pObj2D->SetPriority(5);
		}

		if (m_pPlayer2D == nullptr)
		{
			// 勝敗メッセージ生成
			m_pPlayer2D = CObject_2D::Create();
			m_pPlayer2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 8 * 3, 100.0f, 0.0f));
			m_pPlayer2D->SetSize(D3DXVECTOR2(100.0f, 60.0f));
			m_pPlayer2D->SetPriority(5);

			switch (m_nMessageIdx)
			{// 番号でプレイヤーを変更する
			case 0:
				m_pPlayer2D->SetTexture(CObject::TEXTURE_DRAW);
				m_pPlayer2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f));
				m_pPlayer2D->SetSize(D3DXVECTOR2(240.0f, 80.0f));
				break;
			case 1:
				m_pPlayer2D->SetTexture(CObject::TEXTURE_1P_WIN);
				break;
			case 2:
				m_pPlayer2D->SetTexture(CObject::TEXTURE_2P_WIN);
				break;
			case 3:
				m_pPlayer2D->SetTexture(CObject::TEXTURE_3P_WIN);
				break;
			case 4:
				m_pPlayer2D->SetTexture(CObject::TEXTURE_4P_WIN);
				break;
			default:
				m_pPlayer2D->SetTexture(CObject::TEXTURE_NULL);
				break;
			}

			m_type = MESSAGE_WIN;
			m_nMessageCounter = 120;
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_WIN);
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_CHEERS02);
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_RESULT);
		}
	}
}

//=====================================
// 終了処理
//=====================================
void CMessage::Destroy(void)
{
	if (m_pObj2D != nullptr)
	{// オブジェクト2Dが未使用の場合生成
		m_pObj2D->Release();
		m_pObj2D = nullptr;
	}
	m_nMessageCounter = 0;	// メッセージ表示時間の初期化
}


//=====================================
// 生成処理
//=====================================
CMessage* CMessage::Create()
{
	CMessage* pMessage = new CMessage;

	//初期化処理
	if (FAILED(pMessage->Init()))
	{
		return nullptr;
	}

	return pMessage;
}