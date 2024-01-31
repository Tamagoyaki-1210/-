//==============================================================================================
//
// ファイル　　　file.cpp
// tutida ryousei
//
//==============================================================================================
#include"file.h"
#include"renderer.h"
#include"charmanager.h"
#include"player.h"
#include"cpu.h"
#include"objectX.h"
#include"application.h"
#include"character.h"
#include"input.h"
#include"result_char.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CFile::CFile()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CFile::~CFile()
{
}

//==============================================================================================
// キャラクターの情報の読み込み
//==============================================================================================
CCharacter *CFile::LoadCharFile(const CCharManager::Chara_Type type, const int index, int timetype)
{
	// ファイルポインタ
	FILE *pFile = nullptr;

	// ファイル名
	if (timetype == 0)
		pFile = fopen("Data/model/Motion_Player/motion_player.txt", "r");
	if (timetype == 1)
		pFile = fopen("Data/model/Motion_Player/motion_player_past.txt", "r");
	if (timetype == 2)
		pFile = fopen("Data/model/Motion_Player/motion_player_universe.txt", "r");

	// キャラクターセット用
	D3DXVECTOR3 Pos[CCharManager::Max_Character];		// 位置
	D3DXVECTOR3 Rot;				// 角度
	int nIndex = 0;					// 番号
	int nParentIndex;				// 親の番号
	int nPartsIndex = 0;			// パーツの番号
	D3DXVECTOR3 PartsStartPos;		// 初期位置
	D3DXVECTOR3 PartsStartRot;		// 初期角度
	char XFileName[MAX_MODEL][0xff] = {};	// Xファイル名

	int nNum = 0;

	// モーション用
	D3DXVECTOR3 PartsPos;	// 位置
	D3DXVECTOR3 PartsRot;	// 回転
	int nFrame;				// キーのフレーム数
	int nMaxKey;			// キーの数
	int nMaxParts;		// パーツの数
	bool bLoop = false;		// ループするか
	int nKey = 0;			// 現在のキーの数
	int nParts = 0;			// 現在のパーツ
	int nNumMotion = 0;		// 現在のモーション

	CCharacter *pCharacter = nullptr;	// キャラクター

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

					// ファイル名
					if (strcmp(&m_aString[0], "FILE_NAME") == 0)
					{
						// ＝を読み飛ばす
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					// キャラクターの設定
					if (strcmp(&m_aString[0], "CHARACTERSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_CHARACTERSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							while (1)
							{
								if (strcmp(&m_aString[0], "POS") == 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);
									fscanf(pFile, "%f", &Pos[nNum].x);
									fscanf(pFile, "%f", &Pos[nNum].y);
									fscanf(pFile, "%f", &Pos[nNum].z);
								}
								if (nNum == CCharManager::Max_Character)
								{
									break;
								}
								else
								{
									nNum++;
									fscanf(pFile, "%s", &m_aString[0]);
								}
							}
						}

						// キャラクターの種類毎の生成
						switch (type)
						{
						case CCharManager::TYPE_PLAYER:
							pCharacter = CPlayer::Create(Pos[index], index);
							break;
						case CCharManager::TYPE_CPU:
							pCharacter = CCpu::Create(Pos[index], index);
							break;
						case CCharManager::TYPE_RESULT_CHAR:
							pCharacter = CResult_Char::Create(Pos[index], index);
						default:
							break;
						}
					}

					// パーツの数
					if (strcmp(&m_aString[0], "NUMPARTS") == 0)
					{
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%d", &nMaxParts);
					}

					if (strcmp(&m_aString[0], "ALL_PARTSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_ALL_PARTSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							if (strcmp(&m_aString[0], "PARTSSET") == 0)
							{
								while (strcmp(&m_aString[0], "END_PARTSSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// 番号
									if (strcmp(&m_aString[0], "INDEX") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nPartsIndex);
									}

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
								pCharacter->SetModel(nPartsIndex, nParentIndex, PartsStartPos, PartsStartRot, &XFileName[nPartsIndex][0]);
							}
						}
					}

					if (strcmp(&m_aString[0], "MOTIONSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_MOTIONSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// ループするか
							if (strcmp(&m_aString[0], "LOOP") == 0)
							{
								int nLoop;
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nLoop);

								if (nLoop == 1)
								{
									// ループする
									bLoop = true;
								}
								else
								{
									// ループしない
									bLoop = false;
								}
							}

							// キーの数
							if (strcmp(&m_aString[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nMaxKey);
							}

							if (strcmp(&m_aString[0], "KEYSET") == 0)
							{
								while (strcmp(&m_aString[0], "END_KEYSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// キーフレーム
									if (strcmp(&m_aString[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nFrame);
									}

									if (strcmp(&m_aString[0], "KEY") == 0)
									{
										while (strcmp(&m_aString[0], "END_KEY") != 0)
										{
											fscanf(pFile, "%s", &m_aString[0]);

											// 位置
											if (strcmp(&m_aString[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &PartsPos.x);
												fscanf(pFile, "%f", &PartsPos.y);
												fscanf(pFile, "%f", &PartsPos.z);
											}

											// 角度
											if (strcmp(&m_aString[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &PartsRot.x);
												fscanf(pFile, "%f", &PartsRot.y);
												fscanf(pFile, "%f", &PartsRot.z);

											}

											if (strcmp(&m_aString[0], "END_KEY") == 0)
											{
												// モーション値の設定
												pCharacter->SetMotionData(nMaxKey, nKey, nParts, nNumMotion, nFrame, PartsPos, PartsRot, bLoop);

												nParts++;

												if (nParts == nMaxParts)
												{
													nParts = 0;
												}
											}
										}
									}
								}

								nKey++;

								if (nKey == nMaxKey)
								{
									nKey = 0;
								}
							}
						}

						nNumMotion++;
					}
				}
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	// キャラクターの読み込みに成功した場合
	if (pCharacter != nullptr)
	{
		return pCharacter;
	}

	// 読み込みに失敗した場合
	return nullptr;
}