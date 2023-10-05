//==============================================================================================
//
// �t�@�C���@�@�@file.cpp
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
// �R���X�g���N�^
//==============================================================================================
CFile::CFile()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CFile::~CFile()
{
}

//==============================================================================================
// �L�����N�^�[�̏��̓ǂݍ���
//==============================================================================================
CCharacter *CFile::LoadCharFile(const CCharManager::Chara_Type type, const int index, int timetype)
{
	// �t�@�C���|�C���^
	FILE *pFile = nullptr;

	// �t�@�C����
	if (timetype == 0)
		pFile = fopen("Data/model/Motion_Player/motion_player.txt", "r");
	if (timetype == 1)
		pFile = fopen("Data/model/Motion_Player/motion_player_past.txt", "r");
	if (timetype == 2)
		pFile = fopen("Data/model/Motion_Player/motion_player_universe.txt", "r");

	// �L�����N�^�[�Z�b�g�p
	D3DXVECTOR3 Pos[CCharManager::Max_Character];		// �ʒu
	D3DXVECTOR3 Rot;				// �p�x
	int nIndex = 0;					// �ԍ�
	int nParentIndex;				// �e�̔ԍ�
	int nPartsIndex = 0;			// �p�[�c�̔ԍ�
	D3DXVECTOR3 PartsStartPos;		// �����ʒu
	D3DXVECTOR3 PartsStartRot;		// �����p�x
	char XFileName[MAX_MODEL][0xff] = {};	// X�t�@�C����

	int nNum = 0;

	// ���[�V�����p
	D3DXVECTOR3 PartsPos;	// �ʒu
	D3DXVECTOR3 PartsRot;	// ��]
	int nFrame;				// �L�[�̃t���[����
	int nMaxKey;			// �L�[�̐�
	int nMaxParts;		// �p�[�c�̐�
	bool bLoop = false;		// ���[�v���邩
	int nKey = 0;			// ���݂̃L�[�̐�
	int nParts = 0;			// ���݂̃p�[�c
	int nNumMotion = 0;		// ���݂̃��[�V����

	CCharacter *pCharacter = nullptr;	// �L�����N�^�[

	char m_aString[0xff];
	memset(m_aString, 0, sizeof(m_aString));

	// �t�@�C�����J�����ꍇ
	if (pFile != NULL)
	{
		while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
		{
			fscanf(pFile, "%s", &m_aString[0]);

			// SCRIPT����v
			if (strcmp(&m_aString[0], "SCRIPT") == 0)
			{
				while (strcmp(&m_aString[0], "END_SCRIPT") != 0)
				{
					fscanf(pFile, "%s", &m_aString[0]);

					// �t�@�C����
					if (strcmp(&m_aString[0], "FILE_NAME") == 0)
					{
						// ����ǂݔ�΂�
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					// �L�����N�^�[�̐ݒ�
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

						// �L�����N�^�[�̎�ޖ��̐���
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

					// �p�[�c�̐�
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

									// �ԍ�
									if (strcmp(&m_aString[0], "INDEX") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nPartsIndex);
									}

									// �e�̔ԍ�
									if (strcmp(&m_aString[0], "PARENT") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &nParentIndex);
									}

									// �p�[�c�̈ʒu
									if (strcmp(&m_aString[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &PartsStartPos.x);
										fscanf(pFile, "%f", &PartsStartPos.y);
										fscanf(pFile, "%f", &PartsStartPos.z);
									}

									// �p�[�c�̌���
									if (strcmp(&m_aString[0], "ROT") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%f", &PartsStartRot.x);
										fscanf(pFile, "%f", &PartsStartRot.y);
										fscanf(pFile, "%f", &PartsStartRot.z);
									}
								}
								// ���f���p�[�c�̐ݒ�
								pCharacter->SetModel(nPartsIndex, nParentIndex, PartsStartPos, PartsStartRot, &XFileName[nPartsIndex][0]);
							}
						}
					}

					if (strcmp(&m_aString[0], "MOTIONSET") == 0)
					{
						while (strcmp(&m_aString[0], "END_MOTIONSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// ���[�v���邩
							if (strcmp(&m_aString[0], "LOOP") == 0)
							{
								int nLoop;
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nLoop);

								if (nLoop == 1)
								{
									// ���[�v����
									bLoop = true;
								}
								else
								{
									// ���[�v���Ȃ�
									bLoop = false;
								}
							}

							// �L�[�̐�
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

									// �L�[�t���[��
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

											// �ʒu
											if (strcmp(&m_aString[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &PartsPos.x);
												fscanf(pFile, "%f", &PartsPos.y);
												fscanf(pFile, "%f", &PartsPos.z);
											}

											// �p�x
											if (strcmp(&m_aString[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &PartsRot.x);
												fscanf(pFile, "%f", &PartsRot.y);
												fscanf(pFile, "%f", &PartsRot.z);

											}

											if (strcmp(&m_aString[0], "END_KEY") == 0)
											{
												// ���[�V�����l�̐ݒ�
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

		//�t�@�C�������
		fclose(pFile);
	}

	// �L�����N�^�[�̓ǂݍ��݂ɐ��������ꍇ
	if (pCharacter != nullptr)
	{
		return pCharacter;
	}

	// �ǂݍ��݂Ɏ��s�����ꍇ
	return nullptr;
}