//==============================================================================================
//
// �g���b�R�@�@�@trolley.cpp
// tutida ryousei
//
//==============================================================================================
#include"application.h"
#include"game.h"
#include"charmanager.h"
#include"map.h"
#include"gimmick_model.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CTrolley::CTrolley()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CTrolley::~CTrolley()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CTrolley::Init()
{
	m_nLevelDownCounter = 0;	// ���x���_�E���J�E���^�[

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CTrolley::Uninit()
{
	// ���f���̏I������
	for (int nCnt = 0; nCnt < m_nNum_GimmickParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
			delete m_pModel[nCnt];
			m_pModel[nCnt] = nullptr;
		}
	}

	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CTrolley::Update()
{
	// �g���b�R�𓮂����Ă���ꍇ
	if (CMap::GetTrolleyManager()->GetMove() == true)
	{
		// ���[�V�����̐ݒ�
		Motion();

		// �����蔻��
		Trolley_Collision();

		// ����n�_�����
		Turn();

		// �p�x�̐��K��
		NormalizeRot();

		// �g���b�R�̈ʒu�̐ݒ�
		SetPos(m_Pos);
	}

	// ���x���_�E���̃J�E���^�[
	if (m_nLevelDownCounter++ >= Level_Down_Timer)
	{
		// ���x���_�E��
		Level(false);
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CTrolley::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCnt = 0; nCnt < m_nNum_GimmickParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
			m_pModel[nCnt]->Draw();
	}
}

//==============================================================================================
// �g���b�R�̏�������ݒ�
//==============================================================================================
void CTrolley::SetTrolleyInfomation()
{
	CTrolley_Manager *pTrolley_Manager = CMap::GetTrolleyManager();

	// �g���b�R�}�l�[�W���[�������ǂݍ���
	if (pTrolley_Manager != nullptr)
	{
		m_nStartNum = pTrolley_Manager->GetStartNum();

		for (int nCnt = 0; nCnt < m_nStartNum; nCnt++)
		{
			m_Start_Pos[nCnt] = pTrolley_Manager->GetStart_Pos(nCnt);
		}

		// ����n�_�̐�
		m_nTurnNum = pTrolley_Manager->GetTurnNum();

		// ����n�_
		for (int nCnt = 0; nCnt < m_nTurnNum; nCnt++)
		{
			m_TurnPos[nCnt] = pTrolley_Manager->GetTurn_Pos(nCnt);
		}

		// �ړ����x
		for (int nCnt = 0; nCnt < CTrolley_Manager::MaxLevel; nCnt++)
		{
			m_fSpeed[nCnt] = pTrolley_Manager->GetSpeed(nCnt);
		}

		// ���a
		m_fRadius = pTrolley_Manager->GetRadius();

		// �ŏ��̖ړI�n
		SetStart(pTrolley_Manager->GetStartTurn_Index());
	}
}

//==============================================================================================
// �����ʒu�̐ݒ�
//==============================================================================================
void CTrolley::SetStart(const int index)
{
	// �g���b�R��`�悷��
	SetDrawFlag(true);

	m_fCp = 0.0f;				// �O��

	// �ŏ��̖ړI�n
	m_nNextTurn_Index = index;

	// �ŏ��̖ړI�n�̑��
	m_nNextTurn_IndexInit = m_nNextTurn_Index;

	// 0...����]	1...�t��]
	m_nTurn = rand() % 2;

	// �ŏ��̃x�N�g��
	m_Next_Vec = m_TurnPos[m_nNextTurn_Index] - m_Pos;

	// �ŏ��̊p�x
	m_Rot_Dest.y = (float)atan2(m_Pos.x - m_TurnPos[m_nNextTurn_Index].x, m_Pos.z - m_TurnPos[m_nNextTurn_Index].z);
	m_Rot.y = m_Rot_Dest.y;

	m_bReturn = false;		// �����ʒu�ɖ߂�
	m_bOneTurn = false;		// ���������
	m_bBack = false;		// ��ނ��Ă邩

	m_Motion = TROLLEY_MOTION_MOVE;
}

//==============================================================================================
// ���[�V�����l�̓ǂݍ���
//==============================================================================================
void CTrolley::SetMotionData(int maxkey, int key, int parts, int motion,
	int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	// �L�[�t���[���̐ݒ�
	m_MotionSet.aKeySet[key].nFrame = frame;

	// �ʒu�Ɗp�x�̐ݒ�
	m_MotionSet.aKeySet[key].aKey[parts].KeyPos = pos;
	m_MotionSet.aKeySet[key].aKey[parts].KeyRot = rot;

	// �L�[�̍ő吔
	m_MotionSet.nMaxKey = maxkey;

	// ���[�v���邩
	m_MotionSet.bLoop = loop;
}

//==============================================================================================
// ���[�V�����̐ݒ�
//==============================================================================================
void CTrolley::Motion()
{
	for (int nCnt = 0; nCnt < m_nNum_GimmickParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr && !m_MotionSet.bStop)
		{
			// ���Βl
			float fRelative = (float)m_nCountMotion / (float)m_MotionSet.aKeySet[m_nCurrentKey].nFrame;

			// ����
			D3DXVECTOR3 fPosDifference;		// �ʒu�p
			D3DXVECTOR3 fRotDifference;		// ��]�p

			// ����
			// �Ō�̃L�[����Ȃ��ꍇ
			if (m_nCurrentKey != m_MotionSet.nMaxKey - 1)
			{
				// �ʒu
				fPosDifference = m_MotionSet.aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyPos
					- m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;					// �I���l - �J�n�l

																												// �p�x
				fRotDifference = m_MotionSet.aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyRot
					- m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;					// �I���l - �J�n�l
			}
			// �Ō�̃L�[�̏ꍇ
			else if (m_nCurrentKey == m_MotionSet.nMaxKey - 1)
			{
				// �ʒu
				fPosDifference = m_MotionSet.aKeySet[0].aKey[nCnt].KeyPos
					- m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;	// �I���l - �J�n�l

																								// �p�x
				fRotDifference = m_MotionSet.aKeySet[0].aKey[nCnt].KeyRot
					- m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;	// �I���l - �J�n�l
			}

			// ���ݒl
			D3DXVECTOR3 Pos = InitPos[nCnt] + m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// �ʒu
			D3DXVECTOR3 Rot = InitRot[nCnt] + m_MotionSet.aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// ��]

			m_pModel[nCnt]->SetPos(Pos);	// �ʒu�̐ݒ�
			m_pModel[nCnt]->SetRot(Rot);	// ��]�̐ݒ�
		}
	}

	// ���[�V�����J�E���^�[
	m_nCountMotion++;

	// �Ō�̃t���[���܂Ői��
	if (m_nCountMotion >= m_MotionSet.aKeySet[m_nCurrentKey].nFrame)
	{
		// �t���[����0�ɖ߂�
		m_nCountMotion = 0;

		// �L�[��i�߂�
		m_nCurrentKey++;

		if (m_nCurrentKey >= m_MotionSet.nMaxKey)
		{
			// ���[�v����
			if (m_MotionSet.bLoop)
			{
				// �L�[��߂��ă��[�v����
				m_nCurrentKey = 0;
			}
			// ���[�v���Ȃ�
			else
			{
				m_MotionSet.bStop = true;
				m_Motion = TROLLEY_MOTION_MOVE;
			}
		}
	}
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
void CTrolley::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename)
{
	if (m_pModel[index] == nullptr)
	{
		if (parent >= 0)
		{// �e������ꍇ
			m_pModel[index] = CModel::Create(pos, rot, m_pModel[parent], Xfilename);
		}
		else
		{// �e�����Ȃ��ꍇ
			m_pModel[index] = CModel::Create(pos, rot, nullptr, Xfilename);
		}

		// �p�[�c�̏����ʒu
		InitPos[index] = m_pModel[index]->GetPos();
		InitRot[index] = m_pModel[index]->GetRot();
	}
}

//==============================================================================================
// �����蔻��
//==============================================================================================
void CTrolley::Trolley_Collision()
{
	// �v���C���[�̏��
	D3DXVECTOR3 Char_Pos = { 0.0f,0.0f,0.0f };		// �ʒu
	float fChar_Radius = 0.0f;						// ���a
	bool bFlash = false;							// �_�ł��Ă邩

	// �v���C���[�Ƃ̋���
	float fDistance = 0.0f;

	for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
	{
		// �v���C���[�̎擾
		CCharacter *pChar = CApplication::GetCharManager()->GetChara(nCnt);

		if (pChar != nullptr)
		{
			Char_Pos = pChar->GetPos();	// �ʒu
			fChar_Radius = pChar->GetRadius();		// ���a
			bFlash = pChar->GetFlashing();			// �_�ł��Ă邩

			// �v���C���[�Ƃ̋���
			fDistance = Distance(Char_Pos, m_Pos);

			// �g���b�R�ɓ�������
			if (fDistance <= fChar_Radius + m_fRadius
				&& !bFlash)
				// �m�b�N�o�b�N
				pChar->KnockBack_Set(Char_Pos, m_Pos, 10, 3);
		}
	}
}

//==============================================================================================
// ����n�_�����
//==============================================================================================
void CTrolley::Turn()
{
	// �ړI�n�܂ł̃x�N�g��
	D3DXVECTOR3 DestPos_Vec = { 0.0f,0.0f,0.0f };
	DestPos_Vec = m_TurnPos[m_nNextTurn_Index] - m_Pos;

	// �ړI�̈ʒu�ɂ�����~�܂�
	if (DestPos_Vec.x < m_fSpeed[m_nLevel] && DestPos_Vec.x > -m_fSpeed[m_nLevel]
		&& DestPos_Vec.z < m_fSpeed[m_nLevel] && DestPos_Vec.z > -m_fSpeed[m_nLevel])
	{
		// �ʒu�̒���
		m_Pos = m_TurnPos[m_nNextTurn_Index];

		// �g���b�R�̏��
		CTrolley_Manager *pTrolley_Manager = CMap::GetTrolleyManager();

		// �g���b�R���g�p���̏ꍇ
		if (pTrolley_Manager != nullptr)
		{
			// ���̖ړI�̈ʒu
			if (m_nTurn && !m_bReturn)
			{
				m_nNextTurn_Index++;

				// �ő吔�𒴂����ꍇ
				if (m_nNextTurn_Index > m_nTurnNum - 1)
					m_nNextTurn_Index = 0;
			}
			else if (!m_nTurn && !m_bReturn)
			{
				m_nNextTurn_Index--;

				// �ŏ����𒴂����ꍇ
				if (m_nNextTurn_Index < 0)
					m_nNextTurn_Index = m_nTurnNum - 1;
			}

			int Rand_Start = 0;

			Rand_Start = pTrolley_Manager->GetRandStart();

			// ���������
			if (m_bOneTurn)
			{
				m_bReturn = true;
				m_TurnPos[m_nNextTurn_Index] = m_Start_Pos[Rand_Start];
			}

			// ���̖ړI�n�܂ł̃x�N�g��
			D3DXVECTOR3 NextDestPos_Vec = { 0.0f,0.0f,0.0f };
			NextDestPos_Vec = m_TurnPos[m_nNextTurn_Index] - m_Pos;

			// �O��
			m_fCp = m_Next_Vec.x * NextDestPos_Vec.z - m_Next_Vec.z * NextDestPos_Vec.x;

			// �^���Ɉړ����Ă邩
			if (m_fCp == 0 && m_Next_Vec * -1 == NextDestPos_Vec)
				m_bBack = true;

			// ���̃x�N�g��
			m_Next_Vec = m_TurnPos[m_nNextTurn_Index] - m_Pos;

			// ���̖ړI�n�܂ł̊p�x
			m_Rot_Dest.y = (float)atan2(m_Pos.x - m_TurnPos[m_nNextTurn_Index].x, m_Pos.z - m_TurnPos[m_nNextTurn_Index].z);

			// �������O�̖ړI�n
			if (m_nNextTurn_IndexInit == m_nNextTurn_Index)
				m_bOneTurn = true;

			// ���̈ʒu�ɖ߂����������
			if (m_bReturn && m_Next_Vec == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			{
				// �g���b�R�̓������~�߂�
				pTrolley_Manager->SetMove(false);

				// �g���b�R��`�悵�Ȃ�
				SetDrawFlag(false);
			}
		}
	}
	// �x�N�g���̐��K��
	D3DXVec3Normalize(&DestPos_Vec, &DestPos_Vec);

	// �ړI�̈ʒu�܂ňړ�
	m_Pos += DestPos_Vec * m_fSpeed[m_nLevel];
}

//==============================================================================================
// ���x���A�b�v�̏���
//==============================================================================================
void CTrolley::Level(const bool up)
{
	// ���x���_�E���J�E���^�[�����Z�b�g
	m_nLevelDownCounter = 0;

	// �ύX��̃��x��
	const int nLevel = m_nLevel + (up ? 1 : -1);

	// ���x������ɒB���Ă��Ȃ��ꍇ
	if (nLevel < CTrolley_Manager::MaxLevel && nLevel >= 0)
	{
		// ���݂̃��x����ύX
		m_nLevel = nLevel;

		// ���f���̐F���Z����
		for (int nCnt = 0; nCnt < m_nNum_GimmickParts; nCnt++)
		{
			if (m_pModel[nCnt] != nullptr)
			{
				m_pModel[nCnt]->AddColor(D3DXCOLOR((up ? 0.1f : -0.1f), 0.0f, 0.0f, 0.0f));
			}
		}
	}
}

//==============================================================================================
// �p�x�̐��K��
//==============================================================================================
void CTrolley::NormalizeRot()
{
	// �ړI�̊p�x�̐��K��
	if (m_Rot_Dest.y - m_Rot.y > D3DX_PI)
		m_Rot_Dest.y -= D3DX_PI * 2;
	else if (m_Rot_Dest.y - m_Rot.y < -D3DX_PI)
		m_Rot_Dest.y += D3DX_PI * 2;

	// �ړI�̊p�x�ɂ���
	if (!m_bBack)
	{
		if (m_fCp < 0 && m_Rot_Dest.y > m_Rot.y)
			m_Rot.y += 0.01f * m_fSpeed[m_nLevel];
		else if (m_fCp > 0 && m_Rot_Dest.y < m_Rot.y)
			m_Rot.y -= 0.01f * m_fSpeed[m_nLevel];
	}
	else
	{
		if (m_fCp < 0 && m_Rot_Dest.y + D3DX_PI > m_Rot.y)
			m_Rot.y += 0.01f * m_fSpeed[m_nLevel];
		else if (m_fCp > 0 && m_Rot_Dest.y - D3DX_PI < m_Rot.y)
			m_Rot.y -= 0.01f * m_fSpeed[m_nLevel];
	}

	// �p�x�̐��K��
	if (m_Rot.y > D3DX_PI)
		m_Rot.y -= D3DX_PI * 2;
	else if (m_Rot.y < -D3DX_PI)
		m_Rot.y += D3DX_PI * 2;
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
void CTrolley::LoadFileGimmick(const char *Xfilename)
{
	// �t�@�C����
	FILE* pFile = fopen(Xfilename, "r");

	// �L�����N�^�[�Z�b�g�p
	D3DXVECTOR3 Rot;				// �p�x
	int nIndex = 0;					// �ԍ�
	int nParentIndex;				// �e�̔ԍ�
	int nPartsIndex = 0;			// �p�[�c�̔ԍ�
	D3DXVECTOR3 PartsStartPos;		// �����ʒu
	D3DXVECTOR3 PartsStartRot;		// �����p�x
	char XFileName[0x20][0xff] = {};	// X�t�@�C����

	int nNum = 0;

	// ���[�V�����p
	D3DXVECTOR3 PartsPos;	// �ʒu
	D3DXVECTOR3 PartsRot;	// ��]
	int nFrame;				// �L�[�̃t���[����
	int nMaxKey;			// �L�[�̐�
	int nMaxParts;			// �p�[�c�̐�
	bool bLoop = false;		// ���[�v���邩
	int nKey = 0;			// ���݂̃L�[�̐�
	int nParts = 0;			// ���݂̃p�[�c
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

					// �t�@�C����
					if (strcmp(&m_aString[0], "FILE_NAME") == 0)
					{
						// ����ǂݔ�΂�
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					// �p�[�c�̍ő吔
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
								SetModel(nPartsIndex, nParentIndex, PartsStartPos, PartsStartRot, &XFileName[nPartsIndex][0]);
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
												SetMotionData(nMaxKey, nKey, nParts, nNumMotion, nFrame, PartsPos, PartsRot, bLoop);

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
							}
						}
						nKey = 0;	// �L�[�̏�����

						nNumMotion++;
					}
				}
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
}