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
// �f�t�H���g�R���X�g���N�^
//=====================================
CParts_File::CParts_File()
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CParts_File::~CParts_File()
{
}

//============================================================================
// ����������
//============================================================================
HRESULT CParts_File::Init()
{
	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CParts_File::Uninit()
{
	// �S�Ẵ��f���Z�b�g�̏I��
	ReleaseAllFile();
}

//============================================================================
// �I������
//============================================================================
void CParts_File::ReleaseAllFile()
{
	// �S�Ẵ��f���Z�b�g�̏I��
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++) { m_PartsSet[nCnt] = {}; }
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�(�p�[�c.���f��.���)
//==============================================================================================
void CParts_File::SetModel(const int partsIndex, const int modelIndex, const int parent, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* Xfilename)
{
	// �p�[�c�̃��f���Z�b�g�̈�𐶐�����
	m_PartsSet[partsIndex].ModelSet.emplace_back();

	m_PartsSet[partsIndex].ModelSet[modelIndex].nParentIndex = parent;
	m_PartsSet[partsIndex].ModelSet[modelIndex].InitPos = pos;
	m_PartsSet[partsIndex].ModelSet[modelIndex].InitRot = rot;
	m_PartsSet[partsIndex].ModelSet[modelIndex].nModelIndex = CApplication::GetModel()->ReadObject(Xfilename);
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
void CParts_File::LoadFile(const char* Xfilename, const int partsIndex)
{
	// �t�@�C����
	FILE* pFile = fopen(Xfilename, "r");

	// �L�����N�^�[�Z�b�g�p
	D3DXVECTOR3 Rot;				// �p�x
	int nIndex = 0;					// �ԍ�
	int nParentIndex = 0;				// �e�̔ԍ�
	int nPartsIndex = 0;			// �p�[�c�̔ԍ�
	D3DXVECTOR3 PartsStartPos;		// �����ʒu
	D3DXVECTOR3 PartsStartRot;		// �����p�x
	char XFileName[0x20][0xff] = {};	// X�t�@�C����

	char m_aString[0xff];
	memset(m_aString, 0, sizeof(m_aString));

	// �t�@�C�����J�����ꍇ
	if (pFile != NULL)
	{
		// ���O��ݒ肷��
		m_PartsSet[partsIndex].Name = Xfilename;

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
					if (strcmp(&m_aString[0], "MODEL_FILENAME") == 0)
					{
						// ����ǂݔ�΂�
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
								SetModel(partsIndex, nPartsIndex, nParentIndex, PartsStartPos, PartsStartRot, &XFileName[nPartsIndex][0]);
								nPartsIndex++;
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
// �S�Ẵ��f���p�[�c�̓ǂݍ���
//==============================================================================================
void CParts_File::LoadAllFile()
{
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{ 
		LoadFile(m_cPartsFileName[nCnt], nCnt);
	}
}