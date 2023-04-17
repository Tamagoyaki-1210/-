//=============================================================================
//
// メッセージ
// Author : tanimoto kosuke
//
//=============================================================================
#include "message.h"
#include <stdio.h>
#include "score.h"
#include "application.h"
#include "texture.h"
#include "game.h"

//---------------------------
//マクロ関数
//---------------------------
#define MESSAGE_FILE		"data/.txt/message.txt"		//読み込みファイル

//=====================================
//デフォルトコンストラクタ
//=====================================
CMessage::CMessage() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//デストラクタ
//=====================================
CMessage::~CMessage()
{

}

//=====================================
//初期化処理
//=====================================
void CMessage::Init()
{
	CObject2D::Init();

	//外部ファイルで変数に値を代入
	Load();

	//オブジェクト設定処理
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);
}

//=====================================
//終了処理
//=====================================
void CMessage::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CMessage::Update()
{
	CObject2D::Update();

	//UV設定処理
	CObject2D::SetUV();
}

//=====================================
//描画処理
//=====================================
void CMessage::Draw()
{
	switch (m_type)
	{
	case CMessage::TYPE_THISNUMBER:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_THISNUMBER);
		break;
	case CMessage::TYPE_TITLE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_TITLE);
		break;
	case CMessage::TYPE_PAUSE:
		CApplication::GetTexture()->TextureType(CTexture::TYPE_PAUSE);
		break;
	default:
		break;
	}
	if (m_type != TYPE_PAUSE || CGame::GetPause() == true)
	{
		CObject2D::Draw();
	}
}

//=====================================
//スコア生成処理
//=====================================
CMessage *CMessage::Create(D3DXVECTOR3 pos, Message_Type type)
{
	CMessage* pMessage = new CMessage;		//スコアポインタ

	if (pMessage != nullptr)
	{
		pMessage->m_type = type;
		pMessage->Init();
		pMessage->SetPos(pos);
	}
	return pMessage;
}

//=====================================
//読み込み処理
//=====================================
void CMessage::Load()
{
	char s_aString[256];		//読み込む用文字列
	int nNumType = 0;			//読み込んだタイプ数

								//ファイルを開く
	FILE* pFile = fopen(MESSAGE_FILE, "r");

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