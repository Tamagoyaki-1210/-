//==============================================================================================
//
// エントリー　　　entry.cpp
// tutida ryousei
//
//==============================================================================================
#include"entry.h"
#include"camera.h"
#include"input.h"
#include"fade.h"
#include"file.h"
#include"character.h"
#include "stage_manager.h"
#include "object2D.h"
#include "fontString.h"
#include "charmanager.h"
#include "objectX.h"
#include "skybox.h"
#include "debugproc.h"
#include "stage_select.h"

bool CEntry::m_bEntry[] = {};
bool CEntry::m_bIn[] = {};
bool CEntry::m_bTutorial = false;

const D3DXVECTOR3 CEntry::TVSizeMax = { 920.0f, 520.0f, 0.0f };		// TV画面の最大サイズ
const float CEntry::TVDisplaySpeed = 0.5f;							// TV画面の表示速度
const float CEntry::TVStartAnchorAttenuation = 0.1f;				// TV開始アンカー減衰
const float CEntry::TVEndAnchorAttenuation = 0.05f;					// TV終了アンカー減衰
//==============================================================================================
// コンストラクタ
//==============================================================================================
CEntry::CEntry()
{

}

//==============================================================================================
// デストラクタ
//==============================================================================================
CEntry::~CEntry()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CEntry::Init()
{
	// エントリー画面BGM
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_ENTRY);

	// カメラのポインタ
	CCamera* pCamera = CApplication::GetCamera();

	// 視点、注視点の設定
	pCamera->SetPosV({ 75.0f, 145.0f, -500.0f });
	pCamera->SetPosR({ 75.0f, 40.0f, 0.0f });

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		m_bEntry[nCnt] = false;
		m_bIn[nCnt] = false;
		m_CharPos[nCnt] = { 0.0f,0.0f,500.0f };
		m_pChara[nCnt] = nullptr;
		m_CharPosDest[nCnt] = { 175.0f + (nCnt * 20.0f), 0.0f, 180.0f };
	}

	// スカイボックスの生成
	CSkyBox *pSkyBox = CSkyBox::Create(D3DXVECTOR3(0.0f, -250.0f, 0.0f));
	pSkyBox->SetTexture(CTexture::TEXTURE_WINDOW_OUTSIDE);

	// ステージの生成
	CApplication::GetStageManager()->Create("TEST");

	// ゲームスタート文
	m_pGameStartFont = CFontString::Create({ 720.0f , 50.0f, 0.0f }, { 20.0f, 20.0f, 0.0f }, "STARTボタンでゲームスタート!");

	// チュートリアル文
	m_apTutorialFont[0] = CFontString::Create({ 900.0f , 280.0f, 0.0f }, { 25.0f, 25.0f, 0.0f }, "チュートリアルを");
	m_apTutorialFont[1] = CFontString::Create({ 960.0f , 320.0f, 0.0f }, { 25.0f, 25.0f, 0.0f }, "はじめる");
	m_pABotton = CObject2D::Create({ 960.0f, 380.0f, 0.0f }, { 70.0f, 70.0f, 0.0f });
	m_pABotton->SetTexture(CInput::GetKey()->IsConnected(0) ? CTexture::TEXTURE_BUTTON_A : CTexture::TEXTURE_BUTTON_SPACE);

	// プレイヤー番号の表示
	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		m_apPlayerNumber[nCnt] = CObject2D::Create({ 400.0f + (160.0f * nCnt), 600.0f, 0.0f }, { 80.0f, 80.0f, 0.0f });
		m_apPlayerNumber[nCnt]->SetTexture(CTexture::TEXTURE_CHARA_TYPE);
		m_apPlayerNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.2f + (0.2f * nCnt), 0.0f + (0.2f * nCnt));

		m_apABotton[nCnt] = CObject2D::Create({ 400.0f + (160.0f * nCnt), 680.0f, 0.0f }, { 60.0f, 60.0f, 0.0f });
		m_apABotton[nCnt]->SetTexture(nCnt == 0 ?
			CInput::GetKey()->IsConnected(0) ? CTexture::TEXTURE_BUTTON_A : CTexture::TEXTURE_BUTTON_SPACE
			: CTexture::TEXTURE_BUTTON_A);
	}
	m_nSellect = 0;
	m_nDisplayEndCounter = 0;

	m_bTutorialStart = false;		// チュートリアルの開始判定
	m_bTutorialEnd = false;			// テレビ画面の表示の終了判定
	m_bTVDisplayStart = false;		// テレビ画面の表示の開始判定
	m_bTVDisplayEnd = false;		// テレビ画面の表示の終了判定

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CEntry::Uninit()
{
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CEntry::Update()
{
	// チュートリアル中ではない場合
	if (m_bTutorial == false)
	{
		// 入力処理の取得
		CInput* pInput = CInput::GetKey();

		// コントローラー4つ分の処理
		for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
		{
			// ボタンが押されたらプレイヤーを増やす
			if ((pInput->Trigger(JOYPAD_B, nCnt) || pInput->Trigger(JOYPAD_A, nCnt)
				|| ((pInput->Trigger(DIK_P) || pInput->Trigger(DIK_SPACE)) && nCnt == 0)
				&& CFade::GetFade() == CFade::FADE_NONE)
				&& !m_bEntry[nCnt])
			{
				// エントリーしたか
				m_bEntry[nCnt] = true;

				// 対応するAボタンを終了させる
				m_apABotton[nCnt]->Uninit();
				m_apABotton[nCnt] = nullptr;

				// OK文章
				m_apOKString[nCnt] = CFontString::Create(D3DXVECTOR3(400.0f + (160.0f * nCnt) - 25.0f, 680.0f, 0.0f), { 50.0f, 50.0f, 0.0f }, "OK");
				m_apOKString[nCnt]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
				m_apOKString[nCnt]->SetScale(true);
			}

			// エントリーされている場合
			if (m_bEntry[nCnt])
			{
				// プレイヤーの入場
				PlayerIn(nCnt);
			}

			// 画面推移
			if ((pInput->Trigger(JOYPAD_START, nCnt)
				|| pInput->Trigger(DIK_RETURN))
				&& CFade::GetFade() == CFade::FADE_NONE)
			{
				// 決定SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

				// ステージのオブジェクトの削除
				CApplication::GetStageManager()->Clear();

				// 画面遷移
				CFade::SetFade(CApplication::MODE_STAGE_SELECT, 0.05f);

				CStage_Select::SetStage_Initiative(nCnt);
				return;
			}
		}

		// 行動制限
		Act_Limit();

		// チュートリアルの当たり判定
		TutorialCol();

		// チュートリアルの終了処理
		TutorialEnd();
	}
	else
	{
		// チュートリアルの処理
		Tutorial();
	}

}

//==============================================================================================
// 描画処理
//==============================================================================================
void CEntry::Draw()
{
	
}

//==============================================================================================
// 生成処理
//==============================================================================================
CEntry *CEntry::Create()
{
	CEntry *pEntry = nullptr;

	pEntry = new CEntry;

	if (pEntry != nullptr)
	{
		pEntry->Init();
	}

	return pEntry;
}

//==============================================================================================
// チュートリアルの当たり判定
//==============================================================================================
void CEntry::TutorialCol()
{
	bool bColl = false;	// 当たり判定

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		// エントリーしている場合
		if (m_bEntry[nCnt] == true)
		{
			CCharacter* pChara = CApplication::GetCharManager()->GetChara(nCnt);

			// 当たり判定
			if(pChara->CollisionCircle(pChara->GetPos(), pChara->GetRadius(), { 400.0f, 0.0f, -100.0f }, 200))
			{
				// 当たり判定を真にする
				bColl = true;

				// 文字を拡大させる
				for (int nFont = 0; nFont < 2; nFont++)
				{
					m_apTutorialFont[nFont]->SetScale(true);
				}

				// 入力処理の取得
				CInput* pInput = CInput::GetKey();

				if (pInput->Trigger(JOYPAD_B, nCnt) || pInput->Trigger(JOYPAD_A, nCnt) || pInput->Trigger(DIK_SPACE))
				{
					// チュートリアルを開始する
					m_bTutorial = true;
					m_bTutorialStart = true;
					SetEntryUX(false);

					// テレビ_開始SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_TV_START);

					// チュートリアルの主導権を設定
					m_nTutorialInitiative = nCnt;

					return;
				}
			}
		}
	}

	// 誰も当たっていない場合
	if (bColl == false)
	{
		// 文字を元に戻す
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			m_apTutorialFont[nCnt]->SetScale(false);
		}
	}
}

//==============================================================================================
// チュートリアルの処理
//==============================================================================================
void CEntry::Tutorial()
{
	// チュートリアル開始処理が作動している場合m_pChara
	if (m_bTutorialStart == true)
	{
		// チュートリアルの開始処理
		TutorialStart();
	}
	// チュートリアル開始処理が作動していない場合
	else
	{
		// テレビ画面の表示の開始処理が作動している場合
		if (m_bTVDisplayStart == true)
		{
			// TV画面の表示の開始処理
			TVDisplayStart();
		}
		else
		{
			// テレビ画面の表示の終了処理が作動している場合
			if (m_bTVDisplayEnd == true)
			{
				// TV画面の表示の終了処理
				TVDisplayEnd();
			}
			else
			{
				// チュートリアルの選択処理
				TutorialSelect();
			}
		}

	}
}

//==============================================================================================
// チュートリアルの開始処理
//==============================================================================================
void CEntry::TutorialStart()
{
	// 目的移動位置
	D3DXVECTOR3 anchorPosV = { 236.0f, 120.0f, 150.0f };
	D3DXVECTOR3 anchorPosR = { 672.0f, 120.0f, 650.0f };

	// カメラのポインタ
	CCamera* pCamera = CApplication::GetCamera();

	// 移動しきった場合
	if (pCamera->GetPosV() == anchorPosV)
	{
		// チュートリアル画面が使用されていない場合
		if (m_pTutorial == nullptr)
		{
			m_pTutorial = CObject2D::Create({ 640.0f, 360.0f, 0.0f }, { TVSizeMax.x, 0.0f, 0.0f});
			m_pTutorial->SetTexture(CTexture::TEXTURE_NONE);
		}
		// 非表示にされてた場合
		else if (m_pTutorial->GetDrawFlag() == false)
		{
			m_pTutorial->SetDrawFlag(true);
		}
		// チュートリアル画像の初期サイズ
		m_pTutorial->SetSize(TVSizeMax.x, 180.0f);
		// チュートリアル開始処理の終了
		m_bTutorialStart = false;
		m_bTVDisplayStart = true;
	}
	// 移動しきっていない場合
	else
	{
		// 一定の位置まで来たら値を固定させる
		if (((anchorPosV.x - pCamera->GetPosV().x) < 0.1f))
		{
			pCamera->SetPosV(anchorPosV);
			pCamera->SetPosR(anchorPosR);
		}
		else
		{
			D3DXVECTOR3 moveV = (anchorPosV - pCamera->GetPosV()) * TVStartAnchorAttenuation;
			D3DXVECTOR3 moveR = (anchorPosR - pCamera->GetPosR()) * TVStartAnchorAttenuation;

			// 視点、注視点の設定
			pCamera->AddPosV(moveV);
			pCamera->AddPosR(moveR);
		}
	}
}

//==============================================================================================
// チュートリアルの終了処理
//==============================================================================================
void CEntry::TutorialEnd()
{
	// チュートリアルの終了処理が読み込まれている場合
	if (m_bTutorialEnd == true)
	{
		// 目的移動位置
		D3DXVECTOR3 anchorPosV = { 75.0f, 145.0f, -500.0f };
		D3DXVECTOR3 anchorPosR = { 75.0f, 40.0f, 0.0f };

		// カメラのポインタ
		CCamera* pCamera = CApplication::GetCamera();

		// 移動しきっていない場合
		if (pCamera->GetPosV() != anchorPosV)
		{
			// 一定の位置まで来たら値を固定させる
			if (((anchorPosV.x - pCamera->GetPosV().x) > -0.1f))
			{
				pCamera->SetPosV(anchorPosV);
				pCamera->SetPosR(anchorPosR);
			}
			else
			{
				D3DXVECTOR3 moveV = (anchorPosV - pCamera->GetPosV()) * TVEndAnchorAttenuation;
				D3DXVECTOR3 moveR = (anchorPosR - pCamera->GetPosR()) * TVEndAnchorAttenuation;

				// 視点、注視点の設定
				pCamera->AddPosV(moveV);
				pCamera->AddPosR(moveR);
			}
		}
		// 移動しきった場合
		else
		{
			m_bTutorialEnd = false;	// 終了処理を終了する
		}
	}
}

//==============================================================================================
// チュートリアルの選択処理
//==============================================================================================
void CEntry::TutorialSelect()
{
	// 選択状況の格納先
	int nSellect = m_nSellect;

	// 入力処理の取得
	CInput* pInput = CInput::GetKey();

	if ((pInput->Trigger(DIK_D)
		|| pInput->Trigger(JOYPAD_RIGHT, m_nTutorialInitiative))
		&& nSellect < MaxTutorialPage)
	{
		// ページを進める
		nSellect++;
		m_apArrow[0]->SetDrawFlag(true);
		if (nSellect == MaxTutorialPage) m_apArrow[1]->SetDrawFlag(false);
		// 選択SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
	}
	else if ((pInput->Trigger(DIK_A)
		|| pInput->Trigger(JOYPAD_LEFT, m_nTutorialInitiative))
		&& nSellect > 0)
	{
		// ページを戻す
		nSellect--;
		m_apArrow[1]->SetDrawFlag(true);
		if (nSellect == 0) m_apArrow[0]->SetDrawFlag(false);
		// 選択SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);
	}

	// チュートリアルを終了する処理
	if (pInput->Trigger(JOYPAD_B, m_nTutorialInitiative) || pInput->Trigger(JOYPAD_A, m_nTutorialInitiative) || pInput->Trigger(DIK_SPACE))
	{
		// テレビ画面の表示の終了処理を始める
		m_bTVDisplayEnd = true;
		m_pTutorial->SetTexture(CTexture::TEXTURE_NONE);
		for (int nArrow = 0; nArrow < 2; nArrow++) m_apArrow[nArrow]->SetDrawFlag(false);
		nSellect = 0;
		m_nSellect = nSellect;
	}

	// 選択に更新があった場合
	if (m_nSellect != nSellect)
	{
		switch (m_nSellect = nSellect)
		{
		case 0:
			m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_01);
			break;
		case 1:
			m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_02);
			break;
		case 2:
			m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_03);
			break;
		case 3:
			m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_04);
			break;
		default:
			break;
		}
	}
}

//==============================================================================================
// TV画面の表示の開始処理
//==============================================================================================
void CEntry::TVDisplayStart()
{
	// テレビ画面のサイズ
	D3DXVECTOR3 size = m_pTutorial->GetSize();

	// 最大サイズに達している場合
	if (size.y == TVSizeMax.y)
	{
		// 矢印が未使用の場合
		if (m_apArrow[0] == nullptr)
		{
			m_apArrow[0] = CObject2D::Create({ 100.0f, 360.0f, 0.0f }, { 120.0f, 120.0f, 0.0f });
			m_apArrow[0]->SetTexture(CTexture::TEXTURE_TUTORIAL_ALLOW);
			m_apArrow[0]->SetTexPos(0.0f, 1.0f, 0.5f, 0.0f);
			m_apArrow[0]->SetDrawFlag(false);

			m_apArrow[1] = CObject2D::Create({ 1180.0f, 360.0f, 0.0f }, { 120.0f, 120.0f, 0.0f });
			m_apArrow[1]->SetTexture(CTexture::TEXTURE_TUTORIAL_ALLOW);
			m_apArrow[1]->SetTexPos(0.0f, 1.0f, 1.0f, 0.5f);
		}
		// チュートリアル画像を貼り付ける
		m_pTutorial->SetTexture(CTexture::TEXTURE_TUTORIAL_01);

		// →矢印を描画する
		m_apArrow[1]->SetDrawFlag(true);

		// テレビ_表示SE
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_TV_DISPLAY);

		// テレビ画面の表示の開始の終了処理
		m_bTVDisplayStart = false;
	}
	else
	{
		// サイズを加算
		size.y += size.y * TVDisplaySpeed;

		// 最大サイズを超えた場合
		if (size.y > TVSizeMax.y)
		{
			// サイズピッタリに設定
			size.y = TVSizeMax.y;
		}
		// チュートリアル画像のサイズを設定する
		m_pTutorial->SetSize(size.x, size.y);
	}
}

//==============================================================================================
// TV画面の表示の終了処理
//==============================================================================================
void CEntry::TVDisplayEnd()
{
	// テレビ画面のサイズ
	D3DXVECTOR3 size = m_pTutorial->GetSize();

	// 最低サイズに達している場合
	if (size.y == 0.0f)
	{
		// TV画面の表示の終了時間に達した場合
		if (m_nDisplayEndCounter > TVDisplayEndTimer)
		{
			m_nDisplayEndCounter = 0;
			// チュートリアルを終了する
			m_bTutorial = false;
			m_bTVDisplayEnd = false;
			m_bTutorialEnd = true;
			SetEntryUX(true);
			m_pTutorial->SetDrawFlag(false);
			// テレビ_終了SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_TV_FINISH);
		}
		else
		{
			m_nDisplayEndCounter++;
		}
	}
	else
	{
		// サイズを減算
		size.y -= size.y * TVDisplaySpeed;

		// 最低サイズを超えた場合
		if (size.y < 0.01f)
		{
			// サイズピッタリに設定
			size.y = 0.0f;
		}
		// チュートリアル画像のサイズを設定する
		m_pTutorial->SetSize(size.x, size.y);
	}
}

//==============================================================================================
// プレイヤーの入場
//==============================================================================================
void CEntry::PlayerIn(int index)
{
	if (m_pChara[index] == nullptr)
	{
		// キャラクターの生成
		m_pChara[index] = CApplication::GetCharManager()->EntryChara(m_CharPos[index], CCharManager::TYPE_PLAYER, index, 0);
	}
	else if (!m_bIn[index])
	{
		D3DXVECTOR3 Vec[CCharManager::Max_Character] = {};
		float fCharSpeed = m_pChara[index]->GetMoveSpeed();

		// 現在の位置から目的の位置までのベクトル
		D3DXVECTOR3 DestPos_Vec = m_CharPosDest[index] - m_CharPos[index];
		Vec[index] = DestPos_Vec;

		// ベクトルの正規化
		D3DXVec3Normalize(&DestPos_Vec, &DestPos_Vec);

		// 目的の位置まで自動で移動
		m_CharPos[index] += DestPos_Vec * fCharSpeed;

		// 位置の設定
		m_pChara[index]->SetPos(m_CharPos[index]);

		// 目的の位置までの角度
		float fAngle = (float)atan2(m_CharPos[index].x - m_CharPosDest[index].x, m_CharPos[index].z - m_CharPosDest[index].z);

		// 角度の設定
		m_pChara[index]->SetRotDest({ 0.0f,fAngle,0.0f });

		// モーションの設定
		m_pChara[index]->SetMotion(CCharacter::MOTION_WALK);

		// 目的の位置についたら止まる
		if (Vec[index].x < fCharSpeed && Vec[index].x > -fCharSpeed
			&& Vec[index].z < fCharSpeed && Vec[index].z > -fCharSpeed)
		{
			m_bIn[index] = true;

			// モーションの設定
			m_pChara[index]->SetMotion(CCharacter::MOTION_NEUTRAL);
		}
	}
}

//==============================================================================================
// 行動制限
//==============================================================================================
void CEntry::Act_Limit()
{
	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		if (m_pChara[nCnt] != nullptr
			&& m_bIn[nCnt])
		{
			// 位置の取得
			D3DXVECTOR3 CharPos = m_pChara[nCnt]->GetPos();

			// X軸の制限
			if (CharPos.x > 250)
				CharPos.x = 250;
			else if (CharPos.x < -100)
				CharPos.x = -100;

			// Z軸の制限
			if (CharPos.z >	150)
				CharPos.z = 150;
			else if (CharPos.z < -300)
				CharPos.z = -300;

			// 位置の設定
			m_pChara[nCnt]->SetPos(CharPos);
		}
	}
}

//==============================================================================================
// エントリー画面の全てのUXの設定処理
//==============================================================================================
void CEntry::SetEntryUX(const bool use)
{
	// チュートリアルを開始する時にUXを消したい
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (m_apTutorialFont[nCnt] != nullptr) for(auto apTutorialFont : m_apTutorialFont[nCnt]->GetFontAll()) apTutorialFont->SetDrawFlag(use);
	}

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		if (m_apPlayerNumber[nCnt] != nullptr) m_apPlayerNumber[nCnt]->SetDrawFlag(use);
		if (m_apABotton[nCnt] != nullptr) m_apABotton[nCnt]->SetDrawFlag(use);
		if (m_apOKString[nCnt] != nullptr) for(auto apOKString : m_apOKString[nCnt]->GetFontAll()) apOKString->SetDrawFlag(use);
	}

	if (m_pGameStartFont != nullptr) for (auto pGameStartFont : m_pGameStartFont->GetFontAll()) pGameStartFont->SetDrawFlag(use);
	if (m_pABotton != nullptr) m_pABotton->SetDrawFlag(use);
}
