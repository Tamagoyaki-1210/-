//============================================================================
//
// ���[�V����	motion.cpp
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
// �f�t�H���g�R���X�g���N�^
//=====================================
CMotion::CMotion()
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CMotion::~CMotion()
{

}

//=====================================
// �S�Ẵ��[�V�����̔j������
//=====================================
void CMotion::ReleaseAll()
{
	// ���[�V������S�ďI��������
	m_MotionPatternData.clear();
	m_UseFileName.clear();
}

//==============================================================================================
// ���[�V�����l�̓ǂݍ���
//==============================================================================================
void CMotion::SetMotionData(MotionPattern motion, std::string name)
{
	// ���O���g�p����Ă��Ȃ��ꍇ
	if (std::count(m_UseFileName.begin(), m_UseFileName.end(), name) == false)
	{
		// ���[�V��������ݒ�
		m_MotionPatternData[name].push_back(motion);
	}
}

//==============================================================================================
// ���[�V�����f�[�^�̓ǂݍ���
//==============================================================================================
void CMotion::LoadFile(const char* Xfilename)
{
	// �t�@�C����
	FILE* pFile = fopen(Xfilename, "r");

	int nNumMotion = 0;		// ���݂̃��[�V����

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

					// ���[�V�����Z�b�g�̓ǂݍ���
					if (strcmp(&m_aString[0], "MOTIONSET") == 0)
					{
						int nKey = 0;			// ���݂̃L�[�̐�

												// ���[�V�������
						CMotion::MotionPattern vMotionPattern = {};	// �L�[�Z�b�g�̏��

						while (strcmp(&m_aString[0], "END_MOTIONSET") != 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);

							// ���[�v���邩
							if (strcmp(&m_aString[0], "LOOP") == 0)
							{
								int nLoop;
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nLoop);

								// ���[�v����̑��(true = 1, false = 0)
								vMotionPattern.bLoop = nLoop == 1;
							}

							// �L�[�̐�
							if (strcmp(&m_aString[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								//fscanf(pFile, "%d", &vMotionPattern.nMaxKey);
							}

							if (strcmp(&m_aString[0], "KEYSET") == 0)
							{
								// �L�[�Z�b�g�̃������̈���m��
								vMotionPattern.aKeySet.emplace_back();

								int nParts = 0;			// ���݂̃p�[�c

								while (strcmp(&m_aString[0], "END_KEYSET") != 0)
								{
									fscanf(pFile, "%s", &m_aString[0]);

									// �L�[�t���[��
									if (strcmp(&m_aString[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &m_aString[0]);
										fscanf(pFile, "%d", &vMotionPattern.aKeySet[nKey].nFrame);
									}

									if (strcmp(&m_aString[0], "KEY") == 0)
									{
										// �L�[�̃������̈���m��
										vMotionPattern.aKeySet[nKey].aKey.emplace_back();

										while (strcmp(&m_aString[0], "END_KEY") != 0)
										{
											fscanf(pFile, "%s", &m_aString[0]);

											// �ʒu
											if (strcmp(&m_aString[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyPos.x);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyPos.y);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyPos.z);
											}

											// �p�x
											if (strcmp(&m_aString[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &m_aString[0]);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyRot.x);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyRot.y);
												fscanf(pFile, "%f", &vMotionPattern.aKeySet[nKey].aKey[nParts].KeyRot.z);
											}

											if (strcmp(&m_aString[0], "END_KEY") == 0)
											{
												// ���̃p�[�c
												nParts++;
											}
										}
									}
								}
								nKey++;
							}
						}
						vMotionPattern.nMaxKey = nKey;

						// ���[�V�����ɐݒ�
						SetMotionData(vMotionPattern, Xfilename);

						nNumMotion++;
					}
				}
			}
		}
		//�t�@�C�������
		fclose(pFile);

		// ���[�V�����ɖ��O��ݒ肷��
		SetUseFileName(Xfilename);
	}
}

//==============================================================================================
// �S�Ẵ��[�V�����f�[�^�̓ǂݍ���
//==============================================================================================
void CMotion::LoadAllFile()
{
	for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
	{
		LoadFile(m_cMotionFileName[nCnt]);
	}
}