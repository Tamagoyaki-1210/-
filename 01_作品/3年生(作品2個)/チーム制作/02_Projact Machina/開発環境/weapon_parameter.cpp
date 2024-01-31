//=============================================================================
//
// weapon_parameter.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "weapon_parameter.h"
#include "application.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CWeapon_Parameter::CWeapon_Parameter()
{

}

//=====================================
// デストラクタ
//=====================================
CWeapon_Parameter::~CWeapon_Parameter()
{
}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CWeapon_Parameter::Init()
{
	// 武器の読み込み
	LoadMeleeWeaponFile();
	LoadGunWeaponFile();

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CWeapon_Parameter::Uninit()
{

}

//==============================================================================================
// 近接武器ファイルの読み込み
//==============================================================================================
void CWeapon_Parameter::LoadMeleeWeaponFile()
{
	// 近接武器のファイル名
	FILE* pFile = fopen("Data\\text\\Parameter\\Weapon\\parameter_Melee_Weapon.txt", "r");

	int nWeapon_Index = 0;				// 武器の番号
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
										MELEE_WEAPON_PARAMETERS Parameter[WEAPON_RARITY] = {};	// レアリティ分パーツ数

										while (strcmp(&aString[0], "END_PARAM_SET") != 0)
										{
											fscanf(pFile, "%s", &aString[0]);

											// 威力
											if (strcmp(&aString[0], "POWER") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[0].nPower);
												fscanf(pFile, "%d", &Parameter[1].nPower);
												fscanf(pFile, "%d", &Parameter[2].nPower);
											}

											// 攻撃速度
											if (strcmp(&aString[0], "ATTACK_RATE") == 0)
											{
												int nAttack_Rate = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nAttack_Rate);

												Parameter[0].nAttack_Rate = nAttack_Rate;
												Parameter[1].nAttack_Rate = nAttack_Rate;
												Parameter[2].nAttack_Rate = nAttack_Rate;
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

											// スタン許容値
											if (strcmp(&aString[0], "LENGTH") == 0)
											{
												int nLength = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nLength);

												Parameter[0].nLength = nLength;
												Parameter[1].nLength = nLength;
												Parameter[2].nLength = nLength;
											}

										}
										// レアリティ分のパーツパラメーターの設定
										for (int nCnt = 0; nCnt < WEAPON_RARITY; nCnt++)
										{
											m_Melee_Weapon[nWeapon_Index][nCnt] = Parameter[nCnt];
										}
										nWeapon_Index++;
									}
								}
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
// 銃武器ファイルの読み込み
//==============================================================================================
void CWeapon_Parameter::LoadGunWeaponFile()
{
	// 近接武器のファイル名
	FILE* pFile = fopen("Data\\text\\Parameter\\Weapon\\parameter_Gun_Weapon.txt", "r");

	int nWeapon_Index = 0;				// 武器の番号
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
										GUN_WEAPON_PARAMETERS Parameter[WEAPON_RARITY] = {};	// レアリティ分パーツ数

										while (strcmp(&aString[0], "END_PARAM_SET") != 0)
										{
											fscanf(pFile, "%s", &aString[0]);

											// 威力
											if (strcmp(&aString[0], "POWER") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[0].nPower);
												fscanf(pFile, "%d", &Parameter[1].nPower);
												fscanf(pFile, "%d", &Parameter[2].nPower);
											}

											// 発射頻度
											if (strcmp(&aString[0], "FIRING_SPEED") == 0)
											{
												float fFiring_Speed = 0.0f;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &fFiring_Speed);

												Parameter[0].fFiring_Speed = fFiring_Speed;
												Parameter[1].fFiring_Speed = fFiring_Speed;
												Parameter[2].fFiring_Speed = fFiring_Speed;
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

											// 弾速
											if (strcmp(&aString[0], "BULLET_SPEED") == 0)
											{
												int nBullet_Speed = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nBullet_Speed);

												Parameter[0].nBullet_Speed = nBullet_Speed;
												Parameter[1].nBullet_Speed = nBullet_Speed;
												Parameter[2].nBullet_Speed = nBullet_Speed;
											}

											// 弾速
											if (strcmp(&aString[0], "LIFE") == 0)
											{
												int nLife = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nLife);

												Parameter[0].nLife = nLife;
												Parameter[1].nLife = nLife;
												Parameter[2].nLife = nLife;
											}
										}
										// レアリティ分のパーツパラメーターの設定
										for (int nCnt = 0; nCnt < WEAPON_RARITY; nCnt++)
										{
											m_Gun_Weapon[nWeapon_Index][nCnt] = Parameter[nCnt];
										}
										nWeapon_Index++;
									}
								}
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