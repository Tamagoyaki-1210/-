//==============================================================================================
//
// �g���b�R�}�l�[�W���[�@�@�@trolley_manager.cpp
// tutida ryousei
//
//==============================================================================================
#include"gimmick_model.h"
#include"trolley_manager.h"
#include"debugProc.h"
#include"file.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CTrolley_Manager::CTrolley_Manager()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CTrolley_Manager::~CTrolley_Manager()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CTrolley_Manager::Init()
{
	// �g���b�R�̐ݒ�
	SetTrolley();

	// �����ʒu�̐ݒ�
	SetStartPos();

	// ����
	m_pGimmick_Model = CGimmick_Model::Create(m_Pos, m_nIndex, m_FileName);

	m_nCoolTime_Count = m_nCoolTime;

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CTrolley_Manager::Uninit()
{
	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CTrolley_Manager::Update()
{
	// �g���b�R�𓮂����Ă��Ȃ��ꍇ
	if (!m_bMove)
	{
		// �N�[���^�C���ɒB�����ꍇ
		if (m_nCoolTime_Count <= 0)
		{
			// �����ʒu�̐ݒ�
			SetStartPos();

			m_nCoolTime_Count = m_nCoolTime;
		}
		// �N�[���^�C�����J�E���g����
		m_nCoolTime_Count--;
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CTrolley_Manager::Draw()
{
}

//==============================================================================================
// ��������
//==============================================================================================
CTrolley_Manager *CTrolley_Manager::Create(int type, int index, const char *Xfilename)
{
	CTrolley_Manager *pTrolley = nullptr;

	pTrolley = new CTrolley_Manager;

	if (pTrolley != nullptr)
	{
		pTrolley->SetType(type);
		pTrolley->SetIndex(index);
		pTrolley->SetFileName(Xfilename);
		pTrolley->Init();
	}

	return pTrolley;
}

//==============================================================================================
// �g���b�R�̍폜
//==============================================================================================
void CTrolley_Manager::TrolleyDeath()
{
	// �g���b�R���~�߂�
	m_bMove = false;

	// �g���b�R���g�p���̏ꍇ
	if (m_pGimmick_Model != nullptr)
	{
		m_pGimmick_Model->Uninit();
		m_pGimmick_Model = nullptr;
	}
}

//==============================================================================================
// �g���b�R�̐ݒ�
//==============================================================================================
void CTrolley_Manager::SetTrolley()
{
	// �t�@�C���|�C���^
	FILE *pFile = nullptr;

	// �t�@�C����
	pFile = fopen("Data/text/trolley.txt", "r");

	m_nStartNum = 0;		// �����ʒu�̐�
	m_nTurnNum = 0;			// ����n�_�̐�
	m_nFirstNum = 0;		// �ŏ��̖ړI�n
	int nSpeedNum = 0;		// �X�s�[�h�̃��x��

	int nType = 0;			// �^�C�v

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

					// �^�C�v
					if (strcmp(&m_aString[0], "TYPE") == 0)
					{
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%d", &nType);
					}

					if (nType == m_nType)
					{
						// �����ʒu
						if (strcmp(&m_aString[0], "START_POS") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%f", &m_Start_Pos[m_nStartNum].x);
							fscanf(pFile, "%f", &m_Start_Pos[m_nStartNum].y);
							fscanf(pFile, "%f", &m_Start_Pos[m_nStartNum].z);

							m_nStartNum++;
						}

						// ����n�_
						if (strcmp(&m_aString[0], "TURN_POS") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%f", &m_Turn_Pos[m_nTurnNum].x);
							fscanf(pFile, "%f", &m_Turn_Pos[m_nTurnNum].y);
							fscanf(pFile, "%f", &m_Turn_Pos[m_nTurnNum].z);

							m_nTurnNum++;
						}

						// �ŏ��̖ړI�n
						if (strcmp(&m_aString[0], "FIRST_INDEX") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%d", &m_nFirst_Index[m_nFirstNum][0]);
							fscanf(pFile, "%d", &m_nFirst_Index[m_nFirstNum][1]);

							m_nFirstNum++;
						}

						// �N�[���^�C��
						if (strcmp(&m_aString[0], "COOL_TIME") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%d", &m_nCoolTime);
						}

						// ���x
						if (strcmp(&m_aString[0], "SPEED") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%f", &m_fSpeed[nSpeedNum]);
							nSpeedNum++;
						}

						// �����蔻��̔��a
						if (strcmp(&m_aString[0], "RADIUS") == 0)
						{
							fscanf(pFile, "%s", &m_aString[0]);
							fscanf(pFile, "%f", &m_fRadius);
						}
					}
				}
			}
		}
	}
}

//==============================================================================================
// �����ʒu�̐ݒ�
//==============================================================================================
void CTrolley_Manager::SetStartPos()
{
	m_Pos = { 0.0f,0.0f,0.0 };		// ��������ʒu
	m_nRand_StartPos = 0;		// �����ʒu

	// �����ʒu�������_���Ō��߂�
	m_nRand_StartPos = rand() % m_nStartNum;
	m_Pos = m_Start_Pos[m_nRand_StartPos];

	for (int nCnt = 0; nCnt < m_nFirstNum; nCnt++)
	{
		if (m_nRand_StartPos == m_nFirst_Index[nCnt][0])
			m_nStartTurn_Index = m_nFirst_Index[nCnt][1];
	}

	// �g���b�R�����łɑ��݂���ꍇ
	if (m_pGimmick_Model != nullptr)
	{
		// �g���b�R�̈ʒu��ݒ肷��
		m_pGimmick_Model->SetPos(m_Pos);
		m_pGimmick_Model->SetTrolleyInfomation();
	}

	// �g���b�R�𓮂���
	m_bMove = true;
}