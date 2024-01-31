//============================================================================
//
// モーション	motion.cpp
// Author : Tanimoto Kosuke
//
//============================================================================
#include "motion.h"

const char* CMotion::m_cMotionFileName[] =
{
	"Data\\text\\Motion\\Player\\motion_Player_Body.txt",
	"Data\\text\\Motion\\Player\\motion_Player_Arms.txt",
	"Data\\text\\Motion\\Player\\motion_Player_Leg.txt",

	"Data\\text\\Motion\\Mob\\motion_mob.txt",

	"Data\\text\\Motion\\Boss\\Boss002.txt",
};

//=====================================
// デフォルトコンストラクタ
//=====================================
CMotion::CMotion()
{

}

//=====================================
// デストラクタ
//=====================================
CMotion::~CMotion()
{

}

//=====================================
// 全てのモーションの破棄処理
//=====================================
void CMotion::ReleaseAll()
{
	// モーションを全て終了させる
	m_MotionPatternData.clear();
	m_UseFileName.clear();
}

//==============================================================================================
// モーション値の読み込み
//==============================================================================================
void CMotion::SetMotionData(MotionPattern motion, std::string name)
{
	// 名前が使用されていない場合
	if (std::count(m_UseFileName.begin(), m_UseFileName.end(), name) == false)
	{
		// モーション情報を設定
		m_MotionPatternData[name].push_back(motion);
	}
}

//==============================================================================================
// モーションデータの読み込み
//==============================================================================================
void CMotion::LoadFile(const char* Xfilename)
{
	// ファイル名
	FILE* pFile = fopen(Xfilename, "r");

	int nNumMotion = 0;		// 現在のモーション

	char m_aString[0xff];
	memset(m_aString, 0, sizeof(m_aString));

	// ファイルが開いた場合
	if (pFile != NULL)
	{
		while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &m_aString[0]);

			// SCRIPTが一致
			if (strcmp(&m_aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &m_aString[0]);

					// モーションセットの読み込み
					if (strcmp(&m_aString[0], "MOTIONSET") == 0)
					{
						int nKey = 0;			// 現在のキーの数

												// モーション情報
						CMotion::MotionPattern vMotionPattern = {};	// キーセットの情報

						while (strcmp(&m_aString[0], "END_MOTIONSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// ループするか
							if (strcmp(&m_aString[0], "LOOP") == 0)
							{
								int nLoop;
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nLoop);

								// ループ判定の代入(true = 1, false = 0)
								vMotionPattern.bLoop = nLoop == 1;
							}

							// キーの数
							if (strcmp(&m_aString[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								//fscanf(pFile, "%d", &vMotionPattern.nMaxKey);
							}

							if (strcmp(&m_aString[0], "KEYSET") == 0)
							{
								// キーセットのメモリ領域を確保
								vMotionPattern.aKeySet.emplace_back();

								int nParts = 0;			// 現在のパーツ

								while (strcmp(&m_aString[0], "END_KEYSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// キーフレーム
									if (strcmp(&m_aString[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &vMotionPattern.aKeySet[nKey].nFrame);
									}

									if (strcmp(&m_aString[0], "KEY") == 0)
									{
										// キーのメモリ領域を確保
										vMotionPattern.aKeySet[nKey].aKey.emplace_back();

										while (strcmp(&m_aString[0], "END_KEY") != 0)
										{
											fscanf(pFile, "%s", &m_aString[0]);

											// 位置
											if (strcmp(&m_aString[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyPos.x);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyPos.y);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyPos.z);
											}

											// 角度
											if (strcmp(&m_aString[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyRot.x);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyRot.y);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyRot.z);
											}

											if (strcmp(&m_aString[0], "END_KEY") == 0)
											{
												// 次のパーツ
												nParts++;
											}
										}
									}
								}
								nKey++;
							}
						}
						vMotionPattern.nMaxKey = nKey;

						// モーションに設定
						SetMotionData(vMotionPattern, Xfilename);

						nNumMotion++;
					}
				}
			}
		}
		//ファイルを閉じる
		fclose(pFile);

		// モーションに名前を設定する
		SetUseFileName(Xfilename);
	}
}

//==============================================================================================
// 全てのモーションデータの読み込み
//==============================================================================================
void CMotion::LoadAllFile()
{
	for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
	{
		LoadFile(m_cMotionFileName[nCnt]);
	}
}