//==============================================================================================
//
// ステージセレクト　　　stage_select.cpp
// tutida ryousei
//
//==============================================================================================
#include"stage_select.h"
#include"input.h"
#include"application.h"
#include"fade.h"
#include"fontString.h"
#include"connect.h"

int CStage_Select::m_nStage_Number = 0;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CStage_Select::CStage_Select()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CStage_Select::~CStage_Select()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CStage_Select::Init()
{
	CFontString::Create({ 150.0f, SCREEN_HEIGHT / 2 - 200, 0.0f }, { 50.0f, 50.0f }, "いちばんみぎがチュートリアル");

	// ステージの番号の初期化
	m_nStage_Number = 0;

	// ステージの位置
	D3DXVECTOR2 Stage_Size = { m_nScreen_Width / 2.5,m_nScreen_Height / 2.5 };
	D3DXVECTOR2 Min_Stage_Size = { m_nScreen_Width / 8,m_nScreen_Height / 8 };

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		// ステージの設定
		m_StagePos[nCnt] = { (float)m_nScreen_Width / 2 + (m_nScreen_Width * nCnt),m_nScreen_Height / 2 - 100,0.0f };

		m_pStage[0][nCnt] = CObject2D::Create({ m_StagePos[nCnt].x + (m_nScreen_Width * nCnt),m_StagePos[nCnt].y,0.0f }, Stage_Size);
		m_pStage[1][nCnt] = CObject2D::Create({ m_StagePos[nCnt].x + (m_nScreen_Width * nCnt),m_StagePos[nCnt].y,0.0f }, { Stage_Size.x + 30.0f, Stage_Size.y + 30.0f }, CObject::PRIORITY_CENTER);

		//m_pStage[0][nCnt]->SetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STAGE_1 + (CTexture::TEXTURE)nCnt));
		// ステージ(小)の設置
		D3DXVECTOR3 StagePos = { m_nScreen_Width / 2 - 450.0f + (300 * nCnt),600.0f,0.0f };
		m_pMin_Stage[0][nCnt] = CObject2D::Create(StagePos, Min_Stage_Size);
		m_pMin_Stage[1][nCnt] = CObject2D::Create(StagePos, { Min_Stage_Size.x + 20.0f, Min_Stage_Size.y + 20.0f }, CObject::PRIORITY_CENTER);

		//m_pMin_Stage[0][nCnt]->SetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_STAGE_1 + (CTexture::TEXTURE)nCnt));
	}

	// 背景
	m_pFloor = CObject2D::Create({ (float)m_nScreen_Width / 2, (float)m_nScreen_Width / 2 ,0.0f }, { (float)m_nScreen_Width, (float)m_nScreen_Width }, CObject::PRIORITY_BACK_GROUND);
	m_pFloor->SetTexture(CTexture::TEXTURE_FLOOR);

	m_bSelect = false;		// ステージを決めたか
	m_nStartCount = 30;		// 始まるまでのカウント
	m_fAlpha = 0.0f;		// アルファ値
	m_Flash = false;		// 点滅してるか

	// 選択の矢印
	Select_Arrow();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CStage_Select::Uninit()
{

}

//==============================================================================================
// 更新処理
//==============================================================================================
void CStage_Select::Update()
{
	// ステージが決まっていない場合
	if (m_bSelect == false)
	{
		// ステージ選択
		Select();
	}
	else
	{
		// ステージ開始処理
		Stage_Start();
	}

	// ステージをスライドさせる
	Stage_Slide();

	// ステージ(小)の設定
	Min_Stage();

	// 矢印の点滅
	Arrow_Flash();

	// 入力処理の取得
	CInput* pInput = CInput::GetKey();

	if(pInput->Trigger(DIK_BACK) || pInput->Trigger(JOYPAD_B))
		// 画面遷移
		CFade::SetFade(CApplication::MODE_CHAR_SELECT, 0.05f);

}

//==============================================================================================
// 生成処理
//==============================================================================================
CStage_Select* CStage_Select::Create()
{
	CStage_Select* pStage_Select = new CStage_Select;

	if (FAILED(pStage_Select->Init()))
	{
		return nullptr;
	}

	return pStage_Select;
}

//==============================================================================================
// ステージ選択
//==============================================================================================
void CStage_Select::Select()
{
	// 入力処理の取得
	CInput* pInput = CInput::GetKey();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if ((pInput->Trigger(DIK_D)
				|| pInput->Trigger(JOYPAD_RIGHT, nCnt)))
			{
				// 選択SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

				// ステージを進める
				m_nStage_Number++;

				if (m_nStage_Number > STAGE_TUTORIAL)
					m_nStage_Number = STAGE_1;

				// 背景の変更
				//m_pFloor->SetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_FLOOR + (CTexture::TEXTURE)m_nStage_Number));
				break;
			}
			if ((pInput->Trigger(DIK_A)
				|| pInput->Trigger(JOYPAD_LEFT, nCnt)))
			{
				// 選択SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

				// ステージを戻す
				m_nStage_Number--;

				if (m_nStage_Number < STAGE_1)
					m_nStage_Number = STAGE_TUTORIAL;

				// 背景の変更
				//m_pFloor->SetTexture((CTexture::TEXTURE)(CTexture::TEXTURE_FLOOR + (CTexture::TEXTURE)m_nStage_Number));
				break;
			}

			if ((pInput->Trigger(DIK_RETURN) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
				|| pInput->Trigger(JOYPAD_START, nCnt))
				&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				// 決定SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

				//// ランダムを選択した場合
				//if (m_nStage_Number == STAGE_RANDOM)
				//	m_nStage_Number = rand() % 3;

				// ステージを決定したか
				m_bSelect = true;

				for (int nCnt2 = 0; nCnt2 < STAGE_MAX; nCnt2++)
				{
					// 目的の位置の設定
					m_StagePos[nCnt2].x = (float)(m_nScreen_Width / 2 + (m_nScreen_Width * nCnt2) - m_nScreen_Width * m_nStage_Number);

					for (int nCnt3 = 0; nCnt3 < 2; nCnt3++)
					{
						// 位置の更新
						m_pStage[nCnt3][nCnt2]->SetPos(m_StagePos[nCnt2]);
					}
				}
			}

			// キャラセレクト画面に戻る
			if ((pInput->Trigger(DIK_BACK, nCnt) || pInput->Trigger(JOYPAD_B))
				&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
				// 画面遷移
				CFade::SetFade(CApplication::MODE_CHAR_SELECT, 0.05f);
		}
	}
}

//==============================================================================================
// ステージ開始処理
//==============================================================================================
void CStage_Select::Stage_Start()
{
	// 始まるまでのカウント
	m_nStartCount--;

	if (m_nStage_Number != STAGE_TUTORIAL
		&& m_nStartCount <= 0)
	{
		// 画面遷移
		CFade::SetFade(CApplication::MODE_GAME, 0.05f);
	}

	else if (m_nStage_Number == STAGE_TUTORIAL
		&& m_nStartCount <= 0)
	{
		// 画面遷移
		CFade::SetFade(CApplication::MODE_TUTORIAL, 0.05f);
	}
}

//==============================================================================================
// ステージをスライドさせる
//==============================================================================================
void CStage_Select::Stage_Slide()
{
	// ステージ決定前
	if (!m_bSelect)
	{
		for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
		{
			// 目的の位置の設定
			m_StagePosDest[nCnt].x = (float)(m_nScreen_Width / 2 + (m_nScreen_Width * nCnt) - m_nScreen_Width * m_nStage_Number);

			// 移動の速度減衰
			m_StagePos[nCnt].x += (m_StagePosDest[nCnt].x - m_StagePos[nCnt].x) * 0.05f;

			for (int nCnt2 = 0; nCnt2 < 2; nCnt2++)
			{
				// 位置の更新
				m_pStage[nCnt2][nCnt]->SetPos(m_StagePos[nCnt]);
			}
		}
	}
}

//==============================================================================================
// 選択の矢印
//==============================================================================================
void CStage_Select::Select_Arrow()
{
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_Select_Arrow[nCnt] = { 150.0f + nCnt * 980,m_nScreen_Height / 2 - 100.0f,0.0f };
		m_pSelect_Arrow[nCnt] = CObject2D::Create({ m_Select_Arrow[nCnt] }, { 150.0f,150.0f }, CObject::PRIORITY_SCREEN);
		//m_pSelect_Arrow[nCnt]->SetTexture(CTexture::TEXTURE_SHOP_DIRECTION);
	}

	m_pSelect_Arrow[0]->SetRot({ D3DX_PI / 2 , D3DX_PI / 2 });
	m_pSelect_Arrow[1]->SetRot({ -D3DX_PI / 2 , -D3DX_PI / 2 });
}

//==============================================================================================
// ステージ(小)の設定
//==============================================================================================
void CStage_Select::Min_Stage()
{
	// 点滅するか
	if (m_fRed >= 1)
		m_bMin = false;
	if (m_fRed <= 0)
		m_bMin = true;

	// アルファ値
	if (!m_bMin)
		m_fRed -= 0.03f;
	else
		m_fRed += 0.03f;

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		// ステージ(小)の色
		m_pMin_Stage[1][nCnt]->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// ステージ(小)の色
		m_pMin_Stage[1][m_nStage_Number]->SetCol({ 1.0f, m_fRed, m_fRed, 1.0f });
	}
}

//==============================================================================================
// 矢印の点滅
//==============================================================================================
void CStage_Select::Arrow_Flash()
{
	// 点滅するか
	if (m_fAlpha >= 1)
		m_Flash = false;
	if (m_fAlpha <= 0)
		m_Flash = true;

	// アルファ値
	if (!m_Flash)
		m_fAlpha -= 0.03f;
	else
		m_fAlpha += 0.03f;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		// 矢印の色
		m_pSelect_Arrow[nCnt]->SetCol({ 1.0f, 1.0f, 1.0f, m_fAlpha });
	}
}