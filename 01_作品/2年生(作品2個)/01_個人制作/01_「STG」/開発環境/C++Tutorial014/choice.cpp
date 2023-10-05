//=============================================================================
//
// 選択肢
// Author : tanimoto kosuke
//
//=============================================================================
#include "choice.h"
#include <assert.h>
#include <stdio.h>
#include "application.h"
#include "texture.h"
#include "fade.h"
#include "input.h"
#include "menu.h"
#include "mode.h"
#include "game.h"

int CChoice::m_nNumAll = 0;
int CChoice::m_nSelect = 0;
int CChoice::m_nSkipSelect = 0;
//---------------------------
//マクロ関数
//---------------------------
#define CHOICE_FILE		"data/.txt/choice.txt"		//読み込みファイル

//-----------------------------------------
//プロトタイプ宣言
//-----------------------------------------
void ExitExe(void);

//=====================================
//デフォルトコンストラクタ
//=====================================
CChoice::CChoice() : CObject2D(UI_PRIORITY_UI), m_nId(0)
{

}

//=====================================
//デストラクタ
//=====================================
CChoice::~CChoice()
{

}

//=====================================
//初期化処理
//=====================================
void CChoice::Init()
{
	//外部ファイルで変数に値を代入する
	Load();

	CObject2D::Init();

	//オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);
}

//=====================================
//終了処理
//=====================================
void CChoice::Uninit()
{
	CObject2D::Uninit();
	ResetId();
}

//=====================================
//更新処理
//=====================================
void CChoice::Update()
{
	CObject2D::Update();

	//UV設定
	CObject2D::SetUV();

	//位置取得
	D3DXVECTOR3 pos = CObject2D::GetPos();

	//位置設定処理
	SetPos(pos);

	if (CApplication::GetMode()->GetMode() == CMode::MODE_GAME)
	{
		if (CGame::GetPause() == true)
		{
			InputChoice();
		}
	}
	else
	{
		InputChoice();
	}
}

//=====================================
//描画処理
//=====================================
void CChoice::Draw()
{
	//画像取得処理
	switch (m_type)
	{
	case TITLE_GAMESTART:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_CHOICE_01);
		break;
	case TITLE_EXIT:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_CHOICE_03);
		break;
	case GAME_CONTINUE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_CHOICE_04);
		break;
	case GAME_EXIT:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_CHOICE_05);
		break;
	default:
		break;
	}
	if (CGame::GetPause() == true)
	{
		CObject2D::Draw();
	}
}

//=====================================
//選択肢の入力処理
//=====================================
void CChoice::InputChoice(void)
{
	CFade *pFade = CMode::GetFade();

	if (pFade->GetFade() != CFade::FADE_NONE)
	{// フェードしている場合
		return;
	}

	CInput *pInput = CApplication::GetInput();

	//現在のID
	if (m_nId == m_nSelect)
	{
		//現在選択されたIDのみ色変更
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		if (pInput->GetKeyboardTrigger(DIK_W))
		{// Wキーが押されたかどうか
			if (m_nSkipSelect <= 0)
			{
				//色初期化
				SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				m_nSelect--;	//一回だけにしたい
				m_nSelect = (m_nSelect + m_nNumAll) % m_nNumAll;

				if (m_nSelect == m_nNumAll - 1)
				{
					m_nSkipSelect = 1;
				}
			}
			else
			{
				m_nSkipSelect--;
			}
			CSound::PlaySound(SOUND_LABEL_SE_SELECT);
		}
		else if (pInput->GetKeyboardTrigger(DIK_S))
		{// Sキーが押されたかどうか
		 //現在のID
			if (m_nSkipSelect <= 0)
			{
				//色初期化
				SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				m_nSelect++;
				m_nSelect = (m_nSelect + m_nNumAll) % m_nNumAll;

				if (m_nSelect != 0)
				{
					m_nSkipSelect = 1;
				}
			}
			else
			{
				m_nSkipSelect--;
			}
			CSound::PlaySound(SOUND_LABEL_SE_SELECT);
		}
		if (pInput->GetKeyboardTrigger(DIK_RETURN) || pInput->GetKeyboardTrigger(DIK_SPACE))
		{//決定キー(ENTERキー)が押されたかどうか
			CMode *pMode = CApplication::GetMode();

			switch (m_type)
			{
			case TITLE_GAMESTART:	//タイトル_ゲーム開始
				pMode->ChangeMode(CMode::MODE_GAME);
				CSound::PlaySound(SOUND_LABEL_SE_YES);
				break;
			case TITLE_EXIT:			//タイトル_終了
				CSound::PlaySound(SOUND_LABEL_SE_YES);
				ExitExe();
				break;
			case GAME_CONTINUE:		//ゲーム_再開
				CGame::SetPause(false);
				CSound::PlaySound(SOUND_LABEL_SE_WHISTLE_START);
				break;
			case GAME_EXIT:			//ゲーム_終了
				pMode->ChangeMode(CMode::MODE_TITLE);
				CSound::PlaySound(SOUND_LABEL_SE_WHISTLE_FINISH);
				break;
			default:
				assert(false);
				break;
			}
		}
	}
}

//=====================================
//生成処理
//=====================================
void CChoice::ResetId()
{
	m_nNumAll = 0;
	m_nSelect = 0;
}

//=====================================
//生成処理
//=====================================
CChoice *CChoice::Create(D3DXVECTOR3 pos, Choice_Type type)
{
	CChoice* pChoice = new CChoice;	//敵キャラポインタ

	if (pChoice != nullptr)
	{
		pChoice->Init();
		pChoice->SetPos(pos);
		pChoice->m_type = type;
		pChoice->m_nId = m_nNumAll;
		if (pChoice->m_nNumAll != 0)
		{
			pChoice->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
		pChoice->m_nNumAll++;
	}
	return pChoice;
}

//=====================================
//読み込み処理
//=====================================
void CChoice::Load()
{
	char s_aString[256];		//読み込む用文字列
	int nNumType = 0;			//読み込んだタイプ数

								//ファイルを開く
	FILE* pFile = fopen(CHOICE_FILE, "r");

	if (pFile != NULL)
	{//ファイルが開いた場合
		fscanf(pFile, "%s", s_aString);

		while (strncmp(s_aString, "SCRIPT", 6) != 0)
		{//スタートが来るまで空白を読み込む
			s_aString[0] = {};
			fscanf(pFile, "%s", s_aString);
		}

		while (strncmp(s_aString, "END_SCRIPT", 10) != 0)
		{//文字列の初期化と読み込み
			fscanf(pFile, "%s", s_aString);

			if (strncmp(s_aString, "#", 1) == 0)
			{//この後にコメント
				fgets(s_aString, sizeof(s_aString), pFile);
				continue;
			}

			if (strcmp(s_aString, "TYPESTATE") == 0)
			{//文字列が一致した場合
				while (strncmp(s_aString, "ENDSTATE", 8) != 0)
				{
					fscanf(pFile, "%s", s_aString);		//=の読み込み
					
					if (strcmp(s_aString, "WIDTH") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fWidth);
					}
					else if (strcmp(s_aString, "HEIGHT") == 0)
					{
						fscanf(pFile, "%s", s_aString);
						fscanf(pFile, "%f", &m_fHeight);
					}
				}
				if (m_type == nNumType)
				{//指定した種別の場合
					break;
				}
				nNumType++;
			}
		}
	}
	//ファイルを閉じる
	fclose(pFile);
}