//=============================================================================
//
// weapon_parameter.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "weapon_parameter.h"
#include "application.h"

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CWeapon_Parameter::CWeapon_Parameter()
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CWeapon_Parameter::~CWeapon_Parameter()
{
}

//============================================================================
// ����������
//============================================================================
HRESULT CWeapon_Parameter::Init()
{
	// ����̓ǂݍ���
	LoadMeleeWeaponFile();
	LoadGunWeaponFile();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CWeapon_Parameter::Uninit()
{

}

//==============================================================================================
// �ߐڕ���t�@�C���̓ǂݍ���
//==============================================================================================
void CWeapon_Parameter::LoadMeleeWeaponFile()
{
	// �ߐڕ���̃t�@�C����
	FILE* pFile = fopen("Data\\text\\Parameter\\Weapon\\parameter_Melee_Weapon.txt", "r");

	int nWeapon_Index = 0;				// ����̔ԍ�
	char aString[0xff];
	memset(aString, 0, sizeof(aString));

	// �t�@�C�����J�����ꍇ
	if (pFile != NULL)
	{
		while (strcmp(&aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &aString[0]);

			// SCRIPT����v
			if (strcmp(&aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					// �S�Ẵp�����[�^�Z�b�g
					if (strcmp(&aString[0], "ALL_PARAM_SET") == 0)
					{
						while (strcmp(&aString[0], "END_ALL_PARAM_SET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							// �W���u���̃p�����[�^�[�Z�b�g
							if (strcmp(&aString[0], "JOB_PARAM_SET") == 0)
							{
								while (strcmp(&aString[0], "END_JOB_PARAM_SET") != 0)
								{
									fscanf(pFile, "%s", &aString[0]);

									// �p�����[�^�[�Z�b�g
									if (strcmp(&aString[0], "PARAM_SET") == 0)
									{
										MELEE_WEAPON_PARAMETERS Parameter[WEAPON_RARITY] = {};	// ���A���e�B���p�[�c��

										while (strcmp(&aString[0], "END_PARAM_SET") != 0)
										{
											fscanf(pFile, "%s", &aString[0]);

											// �З�
											if (strcmp(&aString[0], "POWER") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[0].nPower);
												fscanf(pFile, "%d", &Parameter[1].nPower);
												fscanf(pFile, "%d", &Parameter[2].nPower);
											}

											// �U�����x
											if (strcmp(&aString[0], "ATTACK_RATE") == 0)
											{
												int nAttack_Rate = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nAttack_Rate);

												Parameter[0].nAttack_Rate = nAttack_Rate;
												Parameter[1].nAttack_Rate = nAttack_Rate;
												Parameter[2].nAttack_Rate = nAttack_Rate;
											}

											// �d��
											if (strcmp(&aString[0], "GRAVITY") == 0)
											{
												int nGravity = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nGravity);

												Parameter[0].nGravity = nGravity;
												Parameter[1].nGravity = nGravity;
												Parameter[2].nGravity = nGravity;
											}

											// �X�^�����e�l
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
										// ���A���e�B���̃p�[�c�p�����[�^�[�̐ݒ�
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
		//�t�@�C�������
		fclose(pFile);
	}
}


//==============================================================================================
// �e����t�@�C���̓ǂݍ���
//==============================================================================================
void CWeapon_Parameter::LoadGunWeaponFile()
{
	// �ߐڕ���̃t�@�C����
	FILE* pFile = fopen("Data\\text\\Parameter\\Weapon\\parameter_Gun_Weapon.txt", "r");

	int nWeapon_Index = 0;				// ����̔ԍ�
	char aString[0xff];
	memset(aString, 0, sizeof(aString));

	// �t�@�C�����J�����ꍇ
	if (pFile != NULL)
	{
		while (strcmp(&aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &aString[0]);

			// SCRIPT����v
			if (strcmp(&aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &aString[0]);

					// �S�Ẵp�����[�^�Z�b�g
					if (strcmp(&aString[0], "ALL_PARAM_SET") == 0)
					{
						while (strcmp(&aString[0], "END_ALL_PARAM_SET") != 0)
						{
							fscanf(pFile, "%s", &aString[0]);

							// �W���u���̃p�����[�^�[�Z�b�g
							if (strcmp(&aString[0], "JOB_PARAM_SET") == 0)
							{
								while (strcmp(&aString[0], "END_JOB_PARAM_SET") != 0)
								{
									fscanf(pFile, "%s", &aString[0]);

									// �p�����[�^�[�Z�b�g
									if (strcmp(&aString[0], "PARAM_SET") == 0)
									{
										GUN_WEAPON_PARAMETERS Parameter[WEAPON_RARITY] = {};	// ���A���e�B���p�[�c��

										while (strcmp(&aString[0], "END_PARAM_SET") != 0)
										{
											fscanf(pFile, "%s", &aString[0]);

											// �З�
											if (strcmp(&aString[0], "POWER") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[0].nPower);
												fscanf(pFile, "%d", &Parameter[1].nPower);
												fscanf(pFile, "%d", &Parameter[2].nPower);
											}

											// ���˕p�x
											if (strcmp(&aString[0], "FIRING_SPEED") == 0)
											{
												float fFiring_Speed = 0.0f;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &fFiring_Speed);

												Parameter[0].fFiring_Speed = fFiring_Speed;
												Parameter[1].fFiring_Speed = fFiring_Speed;
												Parameter[2].fFiring_Speed = fFiring_Speed;
											}

											// �d��
											if (strcmp(&aString[0], "GRAVITY") == 0)
											{
												int nGravity = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nGravity);

												Parameter[0].nGravity = nGravity;
												Parameter[1].nGravity = nGravity;
												Parameter[2].nGravity = nGravity;
											}

											// �e��
											if (strcmp(&aString[0], "BULLET_SPEED") == 0)
											{
												int nBullet_Speed = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nBullet_Speed);

												Parameter[0].nBullet_Speed = nBullet_Speed;
												Parameter[1].nBullet_Speed = nBullet_Speed;
												Parameter[2].nBullet_Speed = nBullet_Speed;
											}

											// �e��
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
										// ���A���e�B���̃p�[�c�p�����[�^�[�̐ݒ�
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
		//�t�@�C�������
		fclose(pFile);
	}
}