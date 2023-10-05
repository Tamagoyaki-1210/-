//=============================================================================
//
// enemy.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "enemy.h"
#include "application.h"
#include "enemymanager.h"
#include "game.h"
#include "particle_emitter.h"

const float CEnemy::Move_Inertia = 0.2f;
const float CEnemy::Enemy_Rot_Speed = 0.1f;
//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CEnemy::CEnemy() : CObject(2)
{
	// �������Ɏ��g�̃|�C���^���A�C�e���}�l�[�W���[�ɐݒ�
	CGame::GetEnemyManager()->SetEnemy(this);
	m_fRotSpeed = Enemy_Rot_Speed;
}

//=====================================
// �f�X�g���N�^
//=====================================
CEnemy::~CEnemy()
{
	CEnemyManager* pManager = CGame::GetEnemyManager();

	// �}�l�[�W���[���g�p���̏ꍇ�A������Ɏ��g�̃|�C���^��z�񂩂珜�O����
	if (pManager != nullptr) pManager->DestroyEnemy(this);
}

//============================================================================
// ����������
//============================================================================
HRESULT CEnemy::Init()
{
	m_nMotion = 0;
	m_nCurrentMotion = 0;			// ���݂̃��[�V�����ԍ�

	m_nCurrentKey = 0;				// �L�[��0�ɂ���
	m_nCountMotion = 0;				// ���[�V�����J�E���g��0�ɂ���

	m_bNullModel = false;

	m_move = { 0.0f, 0.0f, 0.0f };

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CEnemy::Uninit()
{
	// �S�Ẵ��f���̏I��
	while (!m_pModel.empty())
	{
		m_pModel.back()->Uninit();
		delete m_pModel.back();
		m_pModel.pop_back();
	}

	Release();
}

//============================================================================
// �X�V����
//============================================================================
void CEnemy::Update()
{
	Move();
}

//============================================================================
// �`�揈��
//============================================================================
void CEnemy::Draw()
{
	// �`�撆�̏ꍇ
	if (GetDrawFlag())
	{
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

		//�v�Z�p�̃}�g���b�N�X
		D3DXMATRIX mtxRot, mtxTrans;

		//���[���h�}�g���b�N�X
		D3DXMatrixIdentity(&m_mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// �S�Ẵ��f����ǂݍ���
		for (auto pModel : m_pModel)
		{
			if (pModel != nullptr)
			{
				// ���f���̕`�揈��
				pModel->Draw();
			}
		}
	}
}

//==============================================================================================
// ���[�V�����l�̓ǂݍ���
//==============================================================================================
void CEnemy::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	// �L�[�t���[���̐ݒ�
	m_MotionSet[motion].aKeySet[key].nFrame = frame;

	// �ʒu�Ɗp�x�̐ݒ�
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyPos = pos;
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyRot = rot;

	// �L�[�̍ő吔
	m_MotionSet[motion].nMaxKey = maxkey;

	// ���[�v���邩
	m_MotionSet[motion].bLoop = loop;
}

//==============================================================================================
// �ړ�����
//==============================================================================================
void CEnemy::Move()
{
	// �ړ��ʂ��X�V(����������)
	m_move -= m_move * Move_Inertia;

	// �ʒu�X�V
	m_pos += m_move * m_fSpeed;
}

//==============================================================================================
// ���[�V�����̍Đ�
//==============================================================================================
void CEnemy::Motion()
{
	if (!m_pModel.empty())
	{
		for (int nCnt = 0; nCnt < (int)m_pModel.size(); nCnt++)
		{
			if (m_pModel[nCnt] != nullptr && !m_MotionSet[m_nCurrentMotion].bStop)
			{
				// ���Βl
				float fRelative = (float)m_nCountMotion / (float)m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame;

				// ����
				D3DXVECTOR3 fPosDifference;		// �ʒu�p
				D3DXVECTOR3 fRotDifference;		// ��]�p

												// ����
												// �Ō�̃L�[����Ȃ��ꍇ
				if (m_nCurrentKey != m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
				{
					// �ʒu
					fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyPos
						- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;					// �I���l - �J�n�l

																													// �p�x
					fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyRot
						- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;					// �I���l - �J�n�l
				}
				// �Ō�̃L�[�̏ꍇ
				else if (m_nCurrentKey == m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
				{
					// �ʒu
					fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyPos
						- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;	// �I���l - �J�n�l

																									// �p�x
					fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyRot
						- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;	// �I���l - �J�n�l
				}

				// ���ݒl
				D3DXVECTOR3 Pos = m_InitPos[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// �ʒu
				D3DXVECTOR3 Rot = m_InitRot[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// ��]

				m_pModel[nCnt]->SetPos(Pos);	// �ʒu�̐ݒ�
				m_pModel[nCnt]->SetRot(Rot);	// ��]�̐ݒ�

				m_bNullModel = false;
			}
			else if (m_pModel[0] == nullptr)
			{
				// null�ɂȂ����瑶�݂𖳎�����
				m_bNullModel = true;
			}
		}

		// ���[�V�����J�E���^�[
		m_nCountMotion++;

		// �Ō�̃t���[���܂Ői��
		if (m_nCountMotion >= m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame
			&& !m_bNullModel)
		{
			// �t���[����0�ɖ߂�
			m_nCountMotion = 0;

			// �L�[��i�߂�
			m_nCurrentKey++;

			if (m_nCurrentKey >= m_MotionSet[m_nCurrentMotion].nMaxKey)
			{
				// ���[�v����
				if (m_MotionSet[m_nCurrentMotion].bLoop)
				{
					// �L�[��߂��ă��[�v����
					m_nCurrentKey = 0;
				}
				// ���[�v���Ȃ�
				else
				{
					m_MotionSet[m_nCurrentMotion].bStop = true;
				}
			}
		}
	}
}

//==============================================================================================
// ���[�V�����ύX
//==============================================================================================
void CEnemy::ChangeMotion()
{
	// �O��̎~�܂������[�V������false�ɂ���
	m_MotionSet[m_nCurrentMotion].bStop = false;

	m_nCurrentMotion = m_nMotion;	// ���[�V������؂�ւ���
	m_nCurrentKey = 0;				// �L�[��0�ɂ���
	m_nCountMotion = 0;				// ���[�V�����J�E���g��0�ɂ���
}

//==============================================================================================
// �p�x�̐��K��
//==============================================================================================
const float CEnemy::NormalizeRot()
{
	// �ړI�̊p�x�̐��K��
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2;
	}

	// �ړI�̊p�x�ɂ���
	m_rot.y += (m_rotDest.y - m_rot.y) * m_fRotSpeed;

	// �p�x�̐��K��
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	return m_rot.y;
}

//============================================================================
// ���g�̏I������
//============================================================================
void CEnemy::Death()
{
	// �H��������p�[�e�B�N��
	CParticleEmitter::Create("EraseGhost", GetPos());

	// �I������
	Uninit();
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
void CEnemy::LoadFileEnemy(const char *Xfilename)
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
					if (strcmp(&m_aString[0], "MODEL_FILENAME") == 0)
					{
						// ����ǂݔ�΂�
						fscanf(pFile, "%s", &m_aString[0]);
						fscanf(pFile, "%s", &XFileName[nIndex][0]);
						nIndex++;
					}

					// �p�[�c�̍ő吔
					if (strcmp(&m_aString[0], "NUM_MODEL") == 0)
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
								m_pModel.push_back(nullptr);
								m_InitPos.push_back({ 0.0f, 0.0f, 0.0f });
								m_InitRot.push_back({ 0.0f, 0.0f, 0.0f });
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
						m_MotionSet.emplace_back();
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
								bLoop = nLoop == 1;
							}

							// �L�[�̐�
							if (strcmp(&m_aString[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &m_aString[0]);
								fscanf(pFile, "%d", &nMaxKey);
							}

							if (strcmp(&m_aString[0], "KEYSET") == 0)
							{
								m_MotionSet[nNumMotion].aKeySet.emplace_back();
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
										m_MotionSet[nNumMotion].aKeySet[nKey].aKey.emplace_back();
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

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
CModel* CEnemy::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename)
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
		m_InitPos[index] = m_pModel[index]->GetPos();
		m_InitRot[index] = m_pModel[index]->GetRot();
	}
	return m_pModel[index];
}