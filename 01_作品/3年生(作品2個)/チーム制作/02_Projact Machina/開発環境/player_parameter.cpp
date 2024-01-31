//=============================================================================
//
// player_parameter.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "player_parameter.h"
#include "application.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CPlayer_Parameter::CPlayer_Parameter()
{

}

//=====================================
// デストラクタ
//=====================================
CPlayer_Parameter::~CPlayer_Parameter()
{
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CPlayer_Parameter::Init()
{
	// ジョブの読み込み
	LoadJobFile();

	// パーツの読み込み
	LoadPartsFile();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CPlayer_Parameter::Uninit()
{

}

//==============================================================================================
// ジョブの設定
//==============================================================================================
void CPlayer_Parameter::LoadJobFile()
{
	// ファイル名
	FILE* pFile = fopen("Data\\text\\Parameter\\Player\\parameter_Player.txt", "r");

	int nJob_Index = 0;					// ジョブの番号
	char aString[0xff];
	memset(aString, 0, sizeof(aString));

	// ファイルが開いた場合
	if (pFile != NULL)
	{
		while (strcmp(&aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &aString[0]);

			// SCRIPTが一致
			if (strcmp(&aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					// 全てのパラメータセット
					if (strcmp(&aString[0], "ALL_PARAM_SET") == 0)
					{
						while (strcmp(&aString[0], "END_ALL_PARAM_SET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							// ジョブ毎のパラメーターセット
							if (strcmp(&aString[0], "JOB_PARAM_SET") == 0)
							{
								while (strcmp(&aString[0], "END_JOB_PARAM_SET") != 0)
								{
									fscanf(pFile, "%s", &aString[0]);

									// パラメーターセット
									if (strcmp(&aString[0], "PARAM_SET") == 0)
									{
										while (strcmp(&aString[0], "END_PARAM_SET") != 0)
										{
											fscanf(pFile, "%s", &aString[0]);

											// 体力
											if (strcmp(&aString[0], "LIFE") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &m_Parameter_Job[nJob_Index].nLife);
											}

											// スタミナ
											if (strcmp(&aString[0], "STAMINA") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &m_Parameter_Job[nJob_Index].nStamina);
											}

											// スタン許容値
											if (strcmp(&aString[0], "STAN_TOLERANCE") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &m_Parameter_Job[nJob_Index].nStan_Tolerance);
											}

											// スタミナ
											if (strcmp(&aString[0], "GRAVITY") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &m_Parameter_Job[nJob_Index].nGravity);
											}
										}
									}
								}
								nJob_Index++;				// ジョブのカウント
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
// パーツの設定
//==============================================================================================
void CPlayer_Parameter::LoadPartsFile()
{
	// ファイル名
	FILE* pFile = fopen("Data\\text\\Parameter\\Player\\parameter_Parts.txt", "r");

	int nJob_Index = 0;				// ジョブの番号
	int nParts_Index = 0;			// パラメータの番号
	char aString[0xff];
	memset(aString, 0, sizeof(aString));

	// ファイルが開いた場合
	if (pFile != NULL)
	{
		while (strcmp(&aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &aString[0]);

			// SCRIPTが一致
			if (strcmp(&aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					// 全てのパラメータセット
					if (strcmp(&aString[0], "ALL_PARAM_SET") == 0)
					{
						while (strcmp(&aString[0], "END_ALL_PARAM_SET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							// ジョブ毎のパラメーターセット
							if (strcmp(&aString[0], "JOB_PARAM_SET") == 0)
							{
								while (strcmp(&aString[0], "END_JOB_PARAM_SET") != 0)
								{
									fscanf(pFile, "%s", &aString[0]);

									// パラメーターセット
									if (strcmp(&aString[0], "PARAM_SET") == 0)
									{
										PARAMETERS Parameter[PARTS_RARITY] = {};	// レアリティ分パーツ数

										while (strcmp(&aString[0], "END_PARAM_SET") != 0)
										{
											fscanf(pFile, "%s", &aString[0]);

											// 体力
											if (strcmp(&aString[0], "LIFE") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[0].nLife);
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[1].nLife);
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[2].nLife);
											}

											// スタミナ
											if (strcmp(&aString[0], "STAMINA") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[0].nStamina);
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[1].nStamina);
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[2].nStamina);
											}

											// スタン許容値
											if (strcmp(&aString[0], "STAN_TOLERANCE") == 0)
											{
												int nStan_Tolerance = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nStan_Tolerance);
												Parameter[0].nStan_Tolerance = nStan_Tolerance;
												Parameter[1].nStan_Tolerance = nStan_Tolerance;
												Parameter[2].nStan_Tolerance = nStan_Tolerance;
											}

											// 重さ
											if (strcmp(&aString[0], "GRAVITY") == 0)
											{
												int nGravity = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nGravity);

												Parameter[0].nGravity = nGravity;
												Parameter[1].nGravity = nGravity;
												Parameter[2].nGravity = nGravity;
											}
										}
										// レアリティ分のパーツパラメーターの設定
										for (int nCnt = 0; nCnt < PARTS_RARITY; nCnt++)
										{
											if (nParts_Index == 0) m_Parameter_Arms[nJob_Index][nCnt] = Parameter[nCnt];
											else m_Parameter_Leg[nJob_Index][nCnt] = Parameter[nCnt];
										}
										nParts_Index++;	// パーツのカウント
									}
								}
								nParts_Index = 0;		// パーツ数数の初期化
								nJob_Index++;			// ジョブのカウント
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