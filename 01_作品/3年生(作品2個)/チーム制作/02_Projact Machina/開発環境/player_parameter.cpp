//=============================================================================
//
// player_parameter.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "player_parameter.h"
#include "application.h"

//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CPlayer_Parameter::CPlayer_Parameter()
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CPlayer_Parameter::~CPlayer_Parameter()
{
}

//============================================================================
// ����������
//============================================================================
HRESULT CPlayer_Parameter::Init()
{
	// �W���u�̓ǂݍ���
	LoadJobFile();

	// �p�[�c�̓ǂݍ���
	LoadPartsFile();

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CPlayer_Parameter::Uninit()
{

}

//==============================================================================================
// �W���u�̐ݒ�
//==============================================================================================
void CPlayer_Parameter::LoadJobFile()
{
	// �t�@�C����
	FILE* pFile = fopen("Data\\text\\Parameter\\Player\\parameter_Player.txt", "r");

	int nJob_Index = 0;					// �W���u�̔ԍ�
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
										while (strcmp(&aString[0], "END_PARAM_SET") != 0)
										{
											fscanf(pFile, "%s", &aString[0]);

											// �̗�
											if (strcmp(&aString[0], "LIFE") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &m_Parameter_Job[nJob_Index].nLife);
											}

											// �X�^�~�i
											if (strcmp(&aString[0], "STAMINA") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &m_Parameter_Job[nJob_Index].nStamina);
											}

											// �X�^�����e�l
											if (strcmp(&aString[0], "STAN_TOLERANCE") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &m_Parameter_Job[nJob_Index].nStan_Tolerance);
											}

											// �X�^�~�i
											if (strcmp(&aString[0], "GRAVITY") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &m_Parameter_Job[nJob_Index].nGravity);
											}
										}
									}
								}
								nJob_Index++;				// �W���u�̃J�E���g
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
// �p�[�c�̐ݒ�
//==============================================================================================
void CPlayer_Parameter::LoadPartsFile()
{
	// �t�@�C����
	FILE* pFile = fopen("Data\\text\\Parameter\\Player\\parameter_Parts.txt", "r");

	int nJob_Index = 0;				// �W���u�̔ԍ�
	int nParts_Index = 0;			// �p�����[�^�̔ԍ�
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
										PARAMETERS Parameter[PARTS_RARITY] = {};	// ���A���e�B���p�[�c��

										while (strcmp(&aString[0], "END_PARAM_SET") != 0)
										{
											fscanf(pFile, "%s", &aString[0]);

											// �̗�
											if (strcmp(&aString[0], "LIFE") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[0].nLife);
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[1].nLife);
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[2].nLife);
											}

											// �X�^�~�i
											if (strcmp(&aString[0], "STAMINA") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[0].nStamina);
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[1].nStamina);
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &Parameter[2].nStamina);
											}

											// �X�^�����e�l
											if (strcmp(&aString[0], "STAN_TOLERANCE") == 0)
											{
												int nStan_Tolerance = 0;
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%d", &nStan_Tolerance);
												Parameter[0].nStan_Tolerance = nStan_Tolerance;
												Parameter[1].nStan_Tolerance = nStan_Tolerance;
												Parameter[2].nStan_Tolerance = nStan_Tolerance;
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
										}
										// ���A���e�B���̃p�[�c�p�����[�^�[�̐ݒ�
										for (int nCnt = 0; nCnt < PARTS_RARITY; nCnt++)
										{
											if (nParts_Index == 0) m_Parameter_Arms[nJob_Index][nCnt] = Parameter[nCnt];
											else m_Parameter_Leg[nJob_Index][nCnt] = Parameter[nCnt];
										}
										nParts_Index++;	// �p�[�c�̃J�E���g
									}
								}
								nParts_Index = 0;		// �p�[�c�����̏�����
								nJob_Index++;			// �W���u�̃J�E���g
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