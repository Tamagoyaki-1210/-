//=============================================================================
//
// ナンバー
// Author : tanimoto kosuke
//
//=============================================================================
#include "number.h"
#include <stdio.h>
#include "score.h"
#include "application.h"
#include "texture.h"

//---------------------------
//マクロ関数
//---------------------------
#define NUMBER_FILE		"data/.txt/number.txt"		//読み込みファイル

//=====================================
//デフォルトコンストラクタ
//=====================================
CNumber::CNumber() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//デストラクタ
//=====================================
CNumber::~CNumber()
{

}

//=====================================
//初期化処理
//=====================================
void CNumber::Init()
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
void CNumber::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//更新処理
//=====================================
void CNumber::Update()
{
	CObject2D::Update();

	//UV設定処理
	CObject2D::SetUV();
}

//=====================================
//描画処理
//=====================================
void CNumber::Draw()
{
	CApplication::GetTexture()->TextureType(CTexture::TYPE_NUMBER);

	CObject2D::Draw();
}

//=====================================
//点数の設定処理
//=====================================
void CNumber::SetNumber(int nScore)
{
	SetTex(nScore, BASE_NUMBER);
}

//=====================================
//スコア生成処理
//=====================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, Number_Type type)
{
	CNumber* pNumber = new CNumber;		//スコアポインタ

	if (pNumber != nullptr)
	{
		pNumber->m_type = type;
		pNumber->Init();
		pNumber->SetPos(pos);
	}
	return pNumber;
}

//=====================================
//読み込み処理
//=====================================
void CNumber::Load()
{
	char s_aString[256];		//読み込む用文字列
	int nNumType = 0;			//読み込んだタイプ数

								//ファイルを開く
	FILE* pFile = fopen(NUMBER_FILE, "r");

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