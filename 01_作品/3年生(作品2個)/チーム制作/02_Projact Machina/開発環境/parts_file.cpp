//=============================================================================
//
// parts_file.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "parts_file.h"
#include "application.h"
#include "model.h"

const char* CParts_File::m_cPartsFileName[] =
{
	"Data\\text\\PartsList\\Player\\SG01\\parts_Player_Body_SG01.txt",
	"Data\\text\\PartsList\\Player\\SG03\\parts_Player_Body_SG03.txt",
	"Data\\text\\PartsList\\Player\\SG02\\parts_Player_Body_SG02.txt",
	"Data\\text\\PartsList\\Player\\SG05\\parts_Player_Body_SG05.txt",
	//"Data\\text\\PartsList\\Player\\SG06\\parts_Player_Body_SG06.txt",

	"Data\\text\\PartsList\\Player\\SGStandard\\parts_Player_Arms_S.txt",
	"Data\\text\\PartsList\\Player\\SG01\\parts_Player_Arms_SG01.txt",
	"Data\\text\\PartsList\\Player\\SG03\\parts_Player_Arms_SG03.txt",
	"Data\\text\\PartsList\\Player\\SG02\\parts_Player_Arms_SG02.txt",
	"Data\\text\\PartsList\\Player\\SG05\\parts_Player_Arms_SG05.txt",
	//"Data\\text\\PartsList\\Player\\SG06\\parts_Player_Arms_SG06.txt",

	"Data\\text\\PartsList\\Player\\SGStandard\\parts_Player_Legs_S.txt",
	"Data\\text\\PartsList\\Player\\SG01\\parts_Player_Leg_SG01.txt",
	"Data\\text\\PartsList\\Player\\SG03\\parts_Player_Leg_SG03.txt",
	"Data\\text\\PartsList\\Player\\SG02\\parts_Player_Leg_SG02.txt",
	"Data\\text\\PartsList\\Player\\SG05\\parts_Player_Leg_SG05.txt",
	//"Data\\text\\PartsList\\Player\\SG06\\parts_Player_Leg_SG06.txt",

	"Data\\text\\PartsList\\Mob\\parts_Mob.txt",

	"Data\\text\\PartsList\\Boss\\parts_Boss02.txt",
};

//=====================================
// デフォルトコンストラクタ
//=====================================
CParts_File::CParts_File()
{

}

//=====================================
// デストラクタ
//=====================================
CParts_File::~CParts_File()
{
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CParts_File::Init()
{
	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CParts_File::Uninit()
{
	// 全てのモデルセットの終了
	ReleaseAllFile();
}

//============================================================================
// 終了処理
//============================================================================
void CParts_File::ReleaseAllFile()
{
	// 全てのモデルセットの終了
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++) { m_PartsSet[nCnt] = {}; }
}

//==============================================================================================
// モデルパーツの設定(パーツ.モデル.情報)
//==============================================================================================
void CParts_File::SetModel(const int partsIndex, const int modelIndex, const int parent, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* Xfilename)
{
	// パーツのモデルセット領域を生成する
	m_PartsSet[partsIndex].ModelSet.emplace_back();

	m_PartsSet[partsIndex].ModelSet[modelIndex].nParentIndex = parent;
	m_PartsSet[partsIndex].ModelSet[modelIndex].InitPos = pos;
	m_PartsSet[partsIndex].ModelSet[modelIndex].InitRot = rot;
	m_PartsSet[partsIndex].ModelSet[modelIndex].nModelIndex = CApplication::GetModel()->ReadObject(Xfilename);
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
void CParts_File::LoadFile(const char* Xfilename, const int partsIndex)
{
	// ファイル名
	FILE* pFile = fopen(Xfilename, "r");

	// キャラクターセット用
	D3DXVECTOR3 Rot;				// 角度
	int nIndex = 0;					// 番号
	int nParentIndex = 0;				// 親の番号
	int nPartsIndex = 0;			// パーツの番号
	D3DXVECTOR3 PartsStartPos;		// 初期位置
	D3DXVECTOR3 PartsStartRot;		// 初期角度
	char XFileName[0x20][0xff] = {};	// Xファイル名

	char m_aString[0xff];
	memset(m_aString, 0, sizeof(m_aString));

	// ファイルが開いた場合
	if (pFile != NULL)
	{
		// 名前を設定する
		m_PartsSet[partsIndex].Name = Xfilename;

		while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &m_aString[0]);

			// SCRIPTが一致
			if (strcmp(&m_aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &m_aString[0]);

					// ファイル名
					if (strcmp(&m_aString[0], "MODEL_FILENAME") == 0)
					{
						// ＝を読み飛ばす
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					if (strcmp(&m_aString[0], "ALL_PARTSSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_ALL_PARTSSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							if (strcmp(&m_aString[0], "PARTSSET") == 0)
							{
								while (strcmp(&m_aString[0], "END_PARTSSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// 親の番号
									if (strcmp(&m_aString[0], "PARENT") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nParentIndex);
									}

									// パーツの位置
									if (strcmp(&m_aString[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &PartsStartPos.x);
										fscanf(pFile, "%f", &PartsStartPos.y);
										fscanf(pFile, "%f", &PartsStartPos.z);
									}

									// パーツの向き
									if (strcmp(&m_aString[0], "ROT") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &PartsStartRot.x);
										fscanf(pFile, "%f", &PartsStartRot.y);
										fscanf(pFile, "%f", &PartsStartRot.z);
									}
								}
								// モデルパーツの設定
								SetModel(partsIndex, nPartsIndex, nParentIndex, PartsStartPos, PartsStartRot, &XFileName[nPartsIndex][0]);
								nPartsIndex++;
							}
						}
					}
				}
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
}

//==============================================================================================
// 全てのモデルパーツの読み込み
//==============================================================================================
void CParts_File::LoadAllFile()
{
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{ 
		LoadFile(m_cPartsFileName[nCnt], nCnt);
	}
}