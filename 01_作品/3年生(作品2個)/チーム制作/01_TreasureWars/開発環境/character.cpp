//==============================================================================================
// 
// �L�����N�^�[�@�@�@character.cpp
// tutida ryousei
// 
//==============================================================================================
#include"character.h"
#include"model.h"
#include"application.h"
#include"file.h"
#include"game.h"
#include"entry.h"
#include"stage_select.h"
#include"result.h"
#include"input.h"
#include"objectX.h"
#include"gauge.h"
#include"score.h"
#include"scoreitem_stack.h"
#include"receipt.h"
#include"charmanager.h"
#include"ore.h"
#include"oremanager.h"
#include"map.h"
#include"particle_emitter.h"
#include"shop.h"
#include"enemymanager.h"
#include"enemyspawner.h"
#include"itemmanager.h"
#include"result_obj.h"
#include"area.h"
#include"trolley_manager.h"
#include"debugProc.h"
#include"piller.h"
#include"shop_direction.h"
#include"map.h"
#include"gimmick_model.h"

const float CCharacter::Move_Rot_Speed = 0.2f;				// �L�����̉�]���x
const float CCharacter::Character_Radius = 20.0f;			// �L�����̓����蔻��̃T�C�Y(���a)
const float CCharacter::Attack_Length = 70.0f;				// �L�����̏����U���˒�
const float CCharacter::Attack_Size = 40.0f;				// �L�����̏����U���͈�(�~�`)
const float CCharacter::Attack_LevelUp = 10.0f;				// �U���͏㏸�ɔ������p���[�A�b�v��

const float CCharacter::Attack_Angle = 120.0f;				// �L�����̍U���͈�(��^)
const float CCharacter::Shop_Blink_Alpha_Speed = 0.02f;		// �����̃A���t�@�l�̓_�ł̑��x
const float CCharacter::Shop_Blink_Max_Alpha = 0.4f;		// �����̃A���t�@�l�̓_�ł̍ő�l
const float CCharacter::Shop_Col_Radius = 90.0f;			// �����̓����蔻��̑傫��

int CCharacter::m_nTimeSlip_CoolTime = (m_nBefor_Frame * 2) * CCharManager::Max_Character;
int CCharacter::m_nTimeSlip_CoolTime_Count = 0;
bool CCharacter::Invocation_TimeSlip = false;
int CCharacter::m_nTimeSlip_Charindex = 0;
//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CCharacter::CCharacter() : CObject(2)
{
	m_fRadius = Character_Radius;
	m_fMoveSpeed = 0.0f;
	m_fRotSpeed = Move_Rot_Speed;
	m_nAttackPower = First_Attack_Power;
	m_fAttackLength = Attack_Length;
	m_fAttackSize = Attack_Size;
	m_nKnockBack_Frame = 0;
	m_nAttack_HitCount = 0;
	m_nSaleCount = 0;
	m_nShopBlinkAlphaCount = 0;
	m_bDraw = true;
	m_nWalkCounter = 0;
	m_bTime_Elapsed = false;
	m_bUseTimeSlip = false;
	m_bZeroOver = false;
	m_bAttack = false;
	m_bKnockBack = false;
	m_bFlashing = false;
	m_bSafe = false;
	m_nNum_StackScoreItem = 0;
	m_Move = { 0.0f, 0.0f, 0.0f };
	m_KnockBack = { 0.0f, 0.0f, 0.0f };
	m_bInTerritory = true;
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CCharacter::~CCharacter()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CCharacter::Init()
{
	m_Motion = MOTION_NEUTRAL;
	m_nCurrentMotion = 0;			// ���݂̃��[�V�����ԍ�

	// �O��̎~�܂������[�V������false�ɂ���
	m_MotionSet[m_nCurrentMotion].bStop = false;
	m_nCurrentKey = 0;				// �L�[��0�ɂ���
	m_nCountMotion = 0;				// ���[�V�����J�E���g��0�ɂ���

	m_bNullModel = false;

	for (int nCnt = 0; nCnt < m_nBefor_Frame; nCnt++)
	{
		m_nBefore_Pos[nCnt] = m_pos;
	}

	// �A�C�e�����h���b�v����m��
	m_nProbability = 3;

	// �L�����N�^�[�^�C�v�̕\��
	m_pCharacter_Type = CObject3D::Create({ m_pos.x,m_pos.y + 120.0f,m_pos.z }, { 40.0f,30.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, 4, true);
	m_pCharacter_Type->SetTexture(CTexture::TEXTURE_CHARA_TYPE);

	m_bEntry = false;

	for (int nCnt = 0; nCnt <= m_nCharacterIndex; nCnt++)
	{
		// �G���g���[�����L����
		m_bEntry = CEntry::GetEntry(nCnt);
	}

	// �v���C���[�ԍ��ACPU�̕\��
	m_bEntry ?
		m_pCharacter_Type->SetTexPos(0.0f, 1.0f, 0.2f + (0.2f * m_nCharacterIndex), 0.0f + (0.2f * m_nCharacterIndex)) :
		m_pCharacter_Type->SetTexPos(0.0f, 1.0f, 1.0f, 0.8f);

	// �p�[�c�̏����ʒu
	InitPos[m_nNumParts] = {};
	InitRot[m_nNumParts] = {};

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		// �L������UX��ݒ�
		CharUx();
	}

	// �A�C�e���̎擾��Ԃ�ǂݍ���
	for (int nCnt = 0; nCnt < CItem::ITEM_MAX; nCnt++)
	{
		Item_Effect(nCnt);
	}

	if (m_pScoreItemStack != nullptr)
	{
		// �ő及�������擾
		m_pScoreItemStack->SetDefault(m_nGemStack);
	}

	// �����̐���
	m_Crown = CObjectX::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 3.0f,3.0f,3.0f }, "Data/model/crown.x");
	m_Crown->SetDrawFlag(false);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CCharacter::Uninit()
{
	// ���f���̏I������
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
			delete m_pModel[nCnt];
			m_pModel[nCnt] = nullptr;
		}
	}

	// �X�R�A�̏I������
	if (m_pScore != nullptr)
	{
		// �X�R�A�����U���g�ɑ���
		CResult::SetScore(m_pScore->GetScore(), m_nCharacterIndex);
		CPiller::SetScore(m_pScore->GetScore(), m_nCharacterIndex);
		m_pScore->Uninit();
		m_pScore = nullptr;
	}

	// ��Ώ������̏I������
	if (m_pScoreItemStack != nullptr)
	{
		m_pScoreItemStack->Uninit();
		m_pScoreItemStack = nullptr;
	}

	for (int nCnt = 0; nCnt < Max_GemStack; nCnt++)
	{
		// ��Ώ������̏I������
		if (m_pOreStock[nCnt] != nullptr)
		{
			m_pOreStock[nCnt]->Uninit();
			delete m_pOreStock[nCnt];
			m_pOreStock[nCnt] = nullptr;
		}
	}

	Release();
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CCharacter::Update()
{
	// �m�b�N�o�b�N
	KnockBack();

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		// �����Ƃ̓����蔻��
		ShopCollision();

		// �ߋ��̈ʒu��ۑ�
		Before_Pos();

		if (Invocation_TimeSlip)
		{
			// �^�C���X���b�v
			TimeSlip();
		}

		// �^�C���X���b�v�𔭓������L����
		TimeSlip_CoolTime();

		// �w�n�����ǂ���
		m_bInTerritory = CGame::GetArea()->InCheckTerritory(m_pos, m_nCharacterIndex);

		// �����̕���
		Shop_Direction();

		// �I�u�W�F�N�g�Ƃ̓����蔻��
		CollObject();

		// �G�L�����Ƃ̓����蔻��
		EnemyCollision();
	}

	// �v���C���[�ԍ��̈ʒu�̍X�V
	m_pCharacter_Type->SetPos({ m_pos.x,m_pos.y + 120.0f,m_pos.z });

	// ���G
	Flash();

	// �ړ����Ă���ꍇ
	if (m_bMove)
	{
		Walk();
	}

	// �U��������
	if (m_bAttack)
	{
		// �U������
		Attack();
	}

	// �p�x�̐��K��
	CCharacter::NormalizeRot();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CCharacter::Draw()
{
	if (m_bDraw)
	{
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
		D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�̃}�g���b�N�X

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

		for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
		{
			if (m_pModel[nCnt] != nullptr)
			{
				// ���f���̕`�揈��
				m_pModel[nCnt]->Draw();
			}
		}

		// ���ݏ������Ă���X�R�A�A�C�e��������`�悷��
		for (int nCnt = 0; nCnt < m_nNum_StackScoreItem; nCnt++)
		{
			m_pOreStock[nCnt]->Draw();
		}
	}
}

//==============================================================================================
// ���[�V�����l�̓ǂݍ���
//==============================================================================================
void CCharacter::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
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
// ���[�V�����̍Đ�
//==============================================================================================
void CCharacter::Motion()
{
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
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
			D3DXVECTOR3 Pos = InitPos[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// �ʒu
			D3DXVECTOR3 Rot = InitRot[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// ��]

			m_pModel[nCnt]->SetPos(Pos);	// �ʒu�̐ݒ�
			m_pModel[nCnt]->SetRot(Rot);	// ��]�̐ݒ�

			m_bNullModel = false;
		}
		else if (m_pModel[0] == nullptr)
		{
			// null�̃v���C���[�ACPU�𖳎�����
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
				m_Motion = MOTION_NEUTRAL;
			}
		}
	}
}

//==============================================================================================
// ���[�V�����ύX
//==============================================================================================
void CCharacter::ChangeMotion()
{
	// ���݂̃��[�V��������ς�����ꍇ
	if (m_nCurrentMotion != m_Motion)
	{
		// ���݃��[�V�����̏I������
		switch (m_nCurrentMotion)
		{
		case MOTION_WALK:
			m_bMove = false;
			m_nWalkCounter = 0;
			break;
		case MOTION_MINE:
			m_bAttack = false;
			m_nAttack_HitCount = 0;
			break;
		default:
			break;
		}

		// �ړI�̊p�x�����݂̊p�x�ɕύX����
		m_rotDest = m_rot;

		// �O��̎~�܂������[�V������false�ɂ���
		m_MotionSet[m_nCurrentMotion].bStop = false;

		m_nCurrentMotion = m_Motion;	// ���[�V������؂�ւ���
		m_nCurrentKey = 0;				// �L�[��0�ɂ���
		m_nCountMotion = 0;				// ���[�V�����J�E���g��0�ɂ���
	}
}

//==============================================================================================
// ���f���p�[�c�̐ݒ�
//==============================================================================================
CModel *CCharacter::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename)
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

	// �̃��f���̏ꍇ
	if (index == 0 || index == 2)
	{
		switch (m_nCharacterIndex)
		{
			// 1P
		case 0:
			m_charaCol = { 1.0f, 0.0f, 0.0f, 0.0f };
			break;
			// 2P
		case 1:
			m_charaCol = { 0.0f, 0.0f, 1.0f, 0.0f };
			break;
			// 3P
		case 2:
			m_charaCol = { 0.0f, 1.0f, 0.0f, 0.0f };
			break;
			// 4P
		case 3:
			m_charaCol = { 1.0f, 1.0f, 0.0f, 0.0f };
			break;
		default:
			break;
		}
		m_pModel[index]->AddColor(m_charaCol);
	}
	// �w�����X�R�A�A�C�e���̐�������
	else if(index == 12 && CApplication::GetMode() == CApplication::MODE_GAME)
	{
		Create_ScoreItem();
	}

	return m_pModel[index];
}

//==============================================================================================
// �ړ�����
//==============================================================================================
void CCharacter::Move()
{
	// �ړ��ʂ��ʒu�ɉ��Z����
	m_pos += m_Move * m_fMoveSpeed;
}

//==============================================================================================
// �L�������̐F�̏�����
//==============================================================================================
void CCharacter::ResetCharaColor()
{
	for (int index = 0; index < m_nNumParts; index++)
	{
		// �w�肵�����f�����g�p���̏ꍇ
		if (m_pModel[index] != nullptr)
		{
			// ���f���̐F������������
			m_pModel[index]->ResetColor();

			// ���̃��f���̐F�����Z����
			if (index == 0 || index == 2)
			{
				m_pModel[index]->AddColor(m_charaCol);
			}
		}
	}
}

//==============================================================================================
// �����̐ݒ菈��
//==============================================================================================
void CCharacter::SetCrown(const bool crown)
{
	// �_���a��true�̏ꍇ�`�悷��
	if (crown == true)
	{
		// �����̈ʒu�̍X�V
		m_Crown->SetPos({ m_pos.x,m_pos.y + 150.0f,m_pos.z });

		// �����̕`��ݒ�
		m_Crown->SetDrawFlag(true);
	}
	else
	{
		// �����̕`��ݒ�
		m_Crown->SetDrawFlag(false);
	}
}

//==============================================================================================
// �p�x�̐��K��
//==============================================================================================
void CCharacter::NormalizeRot()
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
}

//==============================================================================================
// ��������
//==============================================================================================
void CCharacter::Walk()
{
	m_nWalkCounter++;

	// �p�[�e�B�N��
	if (m_bMove && m_nWalkCounter >= 20)
	{
		std::move(CParticleEmitter::Create("Smoke", m_pos));
		m_nWalkCounter = 0;
	}
}

//==============================================================================================
// �A�C�e���擾���̌���
//==============================================================================================
void CCharacter::Item_Effect(const int type)
{
	switch ((CItem::Item_Type)type)
	{
		// �ړ����x���x��
	case CItem::ITEM_SPEEDUP:
		switch (m_MoveSpeed_Level)
		{
		case 0:
			m_fMoveSpeed = 5;	// �����l
			break;
		case 1:
			m_fMoveSpeed = 6;
			break;
		case 2:
			m_fMoveSpeed = 7;
			break;
		case 3:
			m_fMoveSpeed = 8;
		default:
			break;
		}
		break;
		// �m�b�N�o�b�N�̃��x��
	case CItem::ITEM_KNOCKBACK:
		// ������x��4
		if (m_nKnockBack_Level < 4)
		{
		// �U����Up�̌��ʂ𔽉f(�˒��A�͈́A�З�)
			m_fAttackLength += Attack_LevelUp;
			m_fAttackSize += Attack_LevelUp * 0.5f;
			m_nAttackPower++;
		}
		switch (m_nKnockBack_Level)
		{
		case 0:
			// �����l
			m_nKnockBack_Power = 20;
			break;
		case 1:
			m_nKnockBack_Power = 25;
			m_pModel[11]->SetSize({ 1.3f, 1.3f, 1.3f });
			break;
		case 2:
			m_nKnockBack_Power = 30;
			m_pModel[11]->SetSize({ 1.6f, 1.6f, 1.6f });
			break;
		case 3:
			m_nKnockBack_Power = 35;
			m_pModel[11]->SetSize({ 1.f, 1.9f, 1.9f });
		default:
			break;
		}
		break;
		// ��x�Ɏ��Ă�z�Ηʂ̃��x��
	case CItem::ITEM_STACK:
		switch (m_nGemStack_Level)
		{
		case 0:
			m_nGemStack = 10;	// �����l
			break;
		case 1:
			m_nGemStack = 14;
			m_pModel[12]->SetSize({ 1.1f, 1.1f, 1.1f });
			break;
		case 2:
			m_nGemStack = 17;
			m_pModel[12]->SetSize({ 1.2f, 1.2f, 1.2f });
			break;
		case 3:
			m_nGemStack = 20;
			m_pModel[12]->SetSize({ 1.3f, 1.3f, 1.3f });
			break;
		default:
			break;
		}

		// �X�R�A�A�C�e���X�^�b�N���g�p���̏ꍇ
		if (m_pScoreItemStack != nullptr)
		{
			// �ő及�������擾
			m_pScoreItemStack->SetDefault(m_nGemStack);
		}
		break;
	default:
		break;
	}
}

//==============================================================================================
// �U������
//==============================================================================================
void CCharacter::Attack()
{
	if (!m_bKnockBack)
	{
		// �U����U�艺�낷�^�C�~���O
		if (m_nAttack_HitCount == Attack_Swing)
		{
			// �̌@���[�V����_�U�艺�낷SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_SWING);
		}
		// �U���̔������Ԃ܂Ōo�߂���ƍU���������s��
		else if (m_nAttack_HitCount == Attack_Hit)
		{
			// �U��(�~�`)�ݒu�ʒu
			const D3DXVECTOR3 attackPos = m_pos - D3DXVECTOR3(sinf(m_rot.y) * m_fAttackLength, 0.0f, cosf(m_rot.y) * m_fAttackLength);

			// �U���G�t�F�N�g
			CParticleEmitter::Create("Attack", attackPos);

			// ����L�����N�^�[���U�����鏈��
			for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
			{
				// �L�����N�^�[�̃|�C���^���擾
				CCharacter *pCharacter = CApplication::GetCharManager()->GetChara(nCnt);

				// �|�C���^�����g�p�A���� ���g�ł͂Ȃ��ꍇ
				if (pCharacter != nullptr && pCharacter != this)
				{
					// ���肪���S�n�тɂ��Ȃ��ꍇ ���� ���G��Ԃł͂Ȃ��ꍇ
					if (!pCharacter->GetSafe() && !pCharacter->GetFlashing())
					{
						// �ʒu�̎擾
						D3DXVECTOR3 atherPos = pCharacter->GetPos();

						// �U�����������Ă���ꍇ(�̂̑傫�� & �s�b�P���̎˒�)
						if (CollisionCircle(attackPos, m_fAttackSize, atherPos, pCharacter->GetRadius())
							|| AttackCollision(m_pos, GetRot().y, CCharacter::m_fAttackLength, CCharacter::Attack_Angle, atherPos, pCharacter->GetRadius()))
						{
							// ����L�����̃m�b�N�o�b�N�̐ݒ�
							pCharacter->KnockBack_Set(atherPos, m_pos, m_nKnockBack_Power, m_nAttackPower);
						}
					}
				}
			}

			// �Q�[�����ł���ꍇ
			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				// �g���b�R�̓����蔻��̏���
				CTrolley_Manager *pTroManager = CMap::GetTrolleyManager();

				if (pTroManager != nullptr)
				{
					// �g���b�R���ړ����̏ꍇ
					if (pTroManager->GetMove() == true)
					{
						CGimmick_Model *pTrolley = pTroManager->GetGimmick_Model();

						// �g���b�R���g�p���̏ꍇ
						if (pTrolley != nullptr)
						{
							// �ʒu�̎擾
							D3DXVECTOR3 TrolleyPos = pTrolley->GetPos();

							// �U�����������Ă��邩(�̂̑傫�� & �s�b�P���̎˒�)
							bool bHit = CollisionCircle(attackPos, m_fAttackSize, TrolleyPos, pTrolley->GetRadius())
								|| AttackCollision(m_pos, m_rot.y, m_fAttackLength, Attack_Angle, TrolleyPos, pTrolley->GetRadius());

							// �U������������
							if (bHit)
							{
								// �@��p�[�e�B�N��
								std::move(CParticleEmitter::Create("Mine", TrolleyPos));

								// �̌@���[�V����_�̌@SE
								CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_MINE);

								// �����_���ɃX�R�A�A�C�e����(3 + ���x��)�΂�T������
								for (int nCnt = 0; nCnt < 3 + pTrolley->GetLevel(); nCnt++) CScoreItem::RandomCreate(TrolleyPos);

								// �g���b�R�̃��x�����グ��
								pTrolley->Level(true);
							}
						}
					}
				}

				// �G�L�����̓����蔻��
				for (auto pEnemy : CGame::GetEnemyManager()->GetAllEnemy())
				{
					// �ʒu�̎擾
					const D3DXVECTOR3 atherPos = pEnemy->GetPos();
					const float atherradius = pEnemy->GetRadius();

					// �U�����������Ă���ꍇ(�̂̑傫�� & �s�b�P���̎˒�)
					if (CollisionCircle(attackPos, m_fAttackSize, atherPos, atherradius)
						|| AttackCollision(m_pos, GetRot().y, CCharacter::m_fAttackLength, CCharacter::Attack_Angle, atherPos, atherradius))
					{
						// �G�L����������
						pEnemy->Death();
					}
				}

				// �G�L�����X�|�i�[�̃|�C���^���擾
				CEnemySpawner *pEnemySpawner = CGame::GetEnemySpawner();

				// �|�C���^���g�p���̏ꍇ
				if (pEnemySpawner != nullptr)
				{
					// �������I���Ă���ꍇ
					if (pEnemySpawner->GetAppearance())
					{
						// �ʒu�̎擾
						const D3DXVECTOR3 atherPos = pEnemySpawner->GetPos();
						const float atherradius = pEnemySpawner->GetRadius();

						// �U�����������Ă���ꍇ(�̂̑傫�� & �s�b�P���̎˒�)
						if (CollisionCircle(attackPos, m_fAttackSize, atherPos, atherradius)
							|| AttackCollision(m_pos, GetRot().y, CCharacter::m_fAttackLength, CCharacter::Attack_Angle, atherPos, atherradius))
						{
							// �G�L�����X�|�i�[�Ƀ_���[�W��^����
							pEnemySpawner->Damage();
						}
					}
				}
			}

			// �z�΂��̌@���鏈��
			CMine();
		}

		// �U�����[�V�������I�������ꍇ
		if (m_nAttack_HitCount >= Attack_MaxFrame)
		{
			SetMotion(MOTION_NEUTRAL);
		}
		// �U�����[�V�������I�����Ă��Ȃ��ꍇ�J�E���g����
		else
		{
			// �U�����I���܂ł̃J�E���g
			m_nAttack_HitCount++;
		}
	}
}

//==============================================================================================
// �z�΂��̌@���鏈��
//==============================================================================================
void CCharacter::CMine()
{
	// �̌@���[�V����_�@��SE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_HIT);

	// �Q�[�����̏ꍇ
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		// �z�΃}�l�[�W���[�̃|�C���^
		COreManager* pOreManager = CGame::GetOreManager();

		// �z�΃|�C���^�̃��X�g
		std::list <COre*> pListOre = pOreManager->GetOreList();

		//�C�e���[�^�[���[�v
		for (auto itr = pListOre.begin(); itr != pListOre.end();)
		{
			//�C�e���[�^�[����z�΂̃|�C���^�̑��
			COre* pOre = *itr;

			// �U��(�~�`)�ݒu�ʒu
			const D3DXVECTOR3 attackPos = m_pos - D3DXVECTOR3(sinf(m_rot.y) * m_fAttackLength, 0.0f, cosf(m_rot.y) * m_fAttackLength);

			// ���葤�̏��
			const D3DXVECTOR3 atherPos = pOre->GetPos();

			// �U������������
			if (CollisionCircle(attackPos, m_fAttackSize, atherPos, COreManager::Ore_Radius)
				|| AttackCollision(m_pos, GetRot().y, CCharacter::m_fAttackLength, CCharacter::Attack_Angle, atherPos, COreManager::Ore_Radius))
			{
				// �̌@���[�V����_�̌@SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_MINE);
				// ���w�̏ꍇ
				if (m_bInTerritory == true)
				{
					// �@��p�[�e�B�N��
					std::move(CParticleEmitter::Create("Mine", atherPos));
				}
				// ���w�O�̏ꍇ
				else
				{
					// �@��p�[�e�B�N��
					std::move(CParticleEmitter::Create("MineOre", atherPos));
				}

				// �z�΂Ƀ_���[�W��^���A�̗͂�0�ȉ��ɂȂ����ꍇ
				if (pOre->Damage(m_nMinePower) <= 0)
				{
					// �z�Ώ������p�[�e�B�N��
					CParticleEmitter::Create("Disappear", atherPos);

					// �����_���ŃA�C�e���𐶐�
					int nRand_Item = rand() % m_nProbability;

					if (nRand_Item == 0)
					{
						// �m���ŃA�C�e���̐���
						CItem::Create({ atherPos.x, atherPos.y + 10.0f, atherPos.z }, { 2.0f,2.0f,2.0f });
					}

					// �w�n����ŕ�΂��̌@����񐔂�ύX���鏈��(���w:1��, �G�w:2��)
					for (int nCnt = 0; nCnt < (!m_bInTerritory ? 2 : 1); nCnt++)
					{
						// �����_���ɃX�R�A�A�C�e�����΂�T������
						CScoreItem::RandomCreate(atherPos);
					}
					// �z�΂��폜����
					pOreManager->OreDeath(pOre);

					//���̃C�e���[�^�[�̑���A���݂̃C�e���[�^�[��j��
					itr = pListOre.erase(itr);

					// ������ł��؂�ɂ���
					continue;
				}
			}
			itr++;	// �j�󂳂�Ȃ��ꍇ�C�e���[�^��i�߂�
		}
	}
}

//==============================================================================================
// �m�b�N�o�b�N�̐ݒ�
//==============================================================================================
void CCharacter::KnockBack_Set(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, int knockback_power, int attackpower)
{
	// �m�b�N�o�b�N�̊p�x
	float fPlayerAngle = (float)atan2(pos1.x - pos2.x, pos1.z - pos2.z);

	// �m�b�N�o�b�N�̕��� * �m�b�N�o�b�N�̏���
	m_KnockBack.x = sinf(fPlayerAngle) * knockback_power;
	m_KnockBack.z = cosf(fPlayerAngle) * knockback_power;

	// �m�b�N�o�b�N�̈ړ������̐ݒ�
	SetKnockBackMove(m_KnockBack);

	// �U�������������������
	SetRotDest({ 0.0f,fPlayerAngle,0.0f });
	SetRot({ 0.0f,fPlayerAngle,0.0f });

	// �m�b�N�o�b�N���邩
	SetKnockBack(true);

	// �_�ł��邩
	SetFlashing(true);

	// ���ꃂ�[�V�����ɕύX
	SetMotion(MOTION_KNOCKBACK);

	// �W�F�����h���b�v������
	Damage(attackpower);
}

//==============================================================================================
// �m�b�N�o�b�N
//==============================================================================================
void CCharacter::KnockBack()
{
	// �m�b�N�o�b�N����ꍇ
	if (m_bKnockBack)
	{
		// �m�b�N�o�b�N��
		if (m_nKnockBack_Frame <= KnockBack_MaxFrame)
		{
			// ���ꃂ�[�V�����ɕύX
			SetMotion(MOTION_KNOCKBACK);

			// �ړ��ʂ̌���
			m_KnockBack.x += (0.0f - m_KnockBack.x) * 0.1f;
			m_KnockBack.z += (0.0f - m_KnockBack.z) * 0.1f;

			// �m�b�N�o�b�N���̈ʒu�����Z
			m_pos += m_KnockBack;

			// 5f���ɃG�t�F�N�g
			if (m_nKnockBack_Frame % 5 == 0)
			{
				std::move(CParticleEmitter::Create("Smoke", m_pos));
			}
		}
		// �m�b�N�o�b�N�I��
		else
		{
			m_nKnockBack_Frame = 0;
			m_bKnockBack = false;		// �m�b�N�o�b�N���~�߂�
			m_Motion = MOTION_NEUTRAL;

			// �U�����[�V�����̉���
			m_bAttack = false;
			m_nAttack_HitCount = 0;
		}
		m_nKnockBack_Frame++;
	}
}

//==============================================================================================
// ���G����
//==============================================================================================
void CCharacter::Flash()
{
	// �_�ł��邩
	if (m_bFlashing)
	{
		if (m_nFlashCount < Flashing_MaxFrame)
		{
			// �b��(�_�ł̌���)�œ_�ł�ς���
			m_bDraw = m_nFlashCount++ % (Flashing_SlowDown * 2) > Flashing_SlowDown;
		}
		else
		{
			m_nFlashCount = 0;
			m_bFlashing = false;		// �_�ł��~�߂�
			m_bDraw = true;				// �`�悷��
		}
	}
}

//==============================================================================================
// ��e����
//==============================================================================================
void CCharacter::Damage(const int power)
{
	// �_���[�WSE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);

	// ��΃X�^�b�N���g�p���̏ꍇ
	if (m_pScoreItemStack != nullptr)
	{
		m_pScoreItemStack->LostScoreItem(power, m_pos);

		// �����Ă���W�F�������炷
		if (m_nNum_StackScoreItem - power > 0)
		{
			m_nNum_StackScoreItem -= power;
		}
		else
		{
			m_nNum_StackScoreItem = 0;
		}
	}
}

//==============================================================================================
// �X�R�A�A�C�e���������ɓ����
//==============================================================================================
void CCharacter::SetStackScoreItem()
{
	// �X�R�A�A�C�e�����g�p���̏ꍇ
	if (m_pOreStock[m_nNum_StackScoreItem] != nullptr)
	{
		D3DXVECTOR3 Rand_OrePos, Rand_OreRot;

		// �X�R�A�A�C�e���̃����_���Ȉʒu�Ɗp�x�̐ݒ�
		Rand_OrePos.x = (float)(rand() % (10 + 10 + 1) - 10) * 0.75f;	// X��(10�`-10)
		Rand_OrePos.z = (float)(rand() % (10 + 10 + 1) - 10) * 0.75f;	// Z��(-10�`10)

		Rand_OreRot = { (float)(rand() % 3) ,(float)(rand() % 3) ,(float)(rand() % 3) };	// �p�x

		// �X�R�A�A�C�e���̈ʒu��ݒ�
		m_pOreStock[m_nNum_StackScoreItem]->SetPos({ Rand_OrePos.x, -10.0f + m_nNum_StackScoreItem, Rand_OrePos.z });
		m_pOreStock[m_nNum_StackScoreItem]->SetRot(Rand_OreRot);
	}

	// ���ݐ��𑝂₷
	m_nNum_StackScoreItem++;
}

//==============================================================================================
// �I�u�W�F�N�g�Ƃ̓����蔻��
//==============================================================================================
void CCharacter::CollObject()
{
	CMap *pMap = CGame::GetMap();

	if (pMap != nullptr)
	{
		m_pos += pMap->CollisionMapObject(m_pos, m_fRadius);
	}
}

//==============================================================================================
// �ߋ��̈ʒu��ۑ�
//==============================================================================================
void CCharacter::Before_Pos()
{
	if (!m_bPlayTimeSlip && CApplication::GetGameStart())
	{
		// ���b�O�̈ʒu�̊i�[
		m_nBefore_Pos[m_nTimeSlip_Count] = m_pos;
		m_nBefore_Rot[m_nTimeSlip_Count] = m_rot;

		if (m_nTimeSlip_Count < m_nBefor_Frame - 1)
		{
			m_nTimeSlip_Count++;
		}
		else
		{
			m_nTimeSlip_Count = 0;
		}

		// ���݂̎���
		m_nTimeSlip_Frame = m_nTimeSlip_Count;

		// �N�[���^�C���̐ݒ�
		m_nTimeSlip_CoolTime_Count++;

		if (m_nTimeSlip_CoolTime_Count >= m_nTimeSlip_CoolTime)
			m_bTime_Elapsed = true;
	}
}

//==============================================================================================
// �^�C���X���b�v
//==============================================================================================
void CCharacter::TimeSlip()
{
	if (m_bTime_Elapsed && !m_bUseTimeSlip)
	{
		m_bPlayTimeSlip = true;

		// ���Ԃ�߂�
		m_nTimeSlip_Frame -= m_nTimeSlip_Speed;

		// 0�ȉ��ɂȂ������ԏ�ɖ߂�
		if (m_nTimeSlip_Frame < 0 && !m_bZeroOver)
		{
			// 0�𒴂��Ė߂�����
			int sub = m_nTimeSlip_Frame;

			m_nTimeSlip_Frame = m_nBefor_Frame - 1;
			m_nTimeSlip_Frame += sub;

			m_bZeroOver = true;
		}

		// �Ō�܂Ŗ߂���
		if (m_nTimeSlip_Frame <= m_nTimeSlip_Count && m_bZeroOver)
		{
			m_nTimeSlip_Frame = m_nTimeSlip_Count;

			// �L�����N�^�[�̎擾
			for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
			{
				// �L�����N�^�[�̃|�C���^���擾
				CCharacter *pCharacter = CApplication::GetCharManager()->GetChara(nCnt);

				if (pCharacter != nullptr)
				{
					m_nTimeSlip_CoolTime_Count = 0;			// �N�[���^�C���̃��Z�b�g
					pCharacter->SetTimeSlip_SaveTime(0);	// �i�񂾃t���[���̏�����
					pCharacter->SetPlayTimeSlip(false);		// �^�C���X���b�v���͈ʒu�̊i�[�����Ȃ�
					pCharacter->SetTime_Elapsed(false);		// 300�t���[����������
					pCharacter->SetZeroOver(false);			// 0����������
					pCharacter->SetTimeSlip(false);			// �^�C���X���b�v���󂯂��Ԃɂ���
				}
			}

			if (m_nTimeSlip_Charindex != 3)
			{
				if (m_nCharacterIndex == 3)
					Invocation_TimeSlip = false;
			}
			else
			{
				if (m_nCharacterIndex == 2)
					Invocation_TimeSlip = false;
			}
		}

		// �ʒu�Ɗp�x�̍X�V
		m_pos = m_nBefore_Pos[m_nTimeSlip_Frame];
		m_rot = m_nBefore_Rot[m_nTimeSlip_Frame];
	}
	else if (!m_bTime_Elapsed)
	{
		Invocation_TimeSlip = false;
		m_bUseTimeSlip = false;
	}
}

//==============================================================================================
// �^�C���X���b�v�𔭓������L����
//==============================================================================================
void CCharacter::TimeSlip_CoolTime()
{
	if (m_bUseTimeSlip)
	{
		// �^�C���X���b�v�𔭓������L�����̔ԍ�
		m_nTimeSlip_Charindex = m_nCharacterIndex;
	}
}

//==============================================================================================
// �X�R�A�A�C�e���̔��p
//==============================================================================================
void CCharacter::ShopCollision()
{
	// �����̏��
	CShop *pShop = CGame::GetShop(m_nCharacterIndex);

	// �������g�p����Ă���ꍇ
	if (pShop != nullptr)
	{
		// �ʒu�̎擾
		D3DXVECTOR3 Shop_Pos = pShop->GetPos();

		// �����ɐG��Ă�
		if (CollisionCircle(Shop_Pos, Shop_Col_Radius, m_pos, m_fRadius))
		{
			// ��΂��������Ă���ꍇ�ɔ��p��UX��\������
			pShop->SetSaleUx(m_nNum_StackScoreItem > 0 ? true : false);

			// �_�ł̎���(�_�ł̍ő�l / �_�ł̑��x)
			int nBlinkMax = (int)(Shop_Blink_Max_Alpha / Shop_Blink_Alpha_Speed);

			// �����Ɍ��Z�������A���t�@�l(����Ԋu % (�_�ł̎��Ԃ�2�{) < �_�ł̍ő�l) ���ʁ��F�𑝌�����
			pShop->AddAlpha(m_nShopBlinkAlphaCount++ % (nBlinkMax * 2) < nBlinkMax ? -Shop_Blink_Alpha_Speed : Shop_Blink_Alpha_Speed);

			// �_�ł̍ő�l�ɒB����ƃ��Z�b�g
			if (m_nShopBlinkAlphaCount >= nBlinkMax * 2)
			{
				m_nShopBlinkAlphaCount = 0;
			}

			// ����Ԋu
			m_nSaleCount++;

			// ���p�J�n�̃^�C�~���O�ɒB�����ꍇ
			if (m_nSaleCount >= Sale_Start_Timer)
			{
				// ��΂𔄂鏈��(�Ԃ�l : ��΂̒l�i)
				int GemScore = GetGemStack()->SaleScoreItem();

				// �X�R�A�̉��Z
				GetScore()->AddScore(GemScore);

				// �X�R�A�����Z���ꂽ�ꍇ�̂݃p�[�e�B�N���\��
				if (GemScore > 0)
				{
					// ���pSE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SALE);

					// ����p�[�e�B�N��
					std::move(CParticleEmitter::Create("Buy", Shop_Pos));

					// �����Ă���W�F�������炷
					m_nNum_StackScoreItem--;

					// �J�E���g�̃��Z�b�g
					m_nSaleCount = Sale_Start_Timer - Sale_Next_Timer;
				}
			}

			// ���S�n�т̔���
			if (m_bSafe == false)
			{
				m_bSafe = true;
			}
		}
		// �O�񔄋p���s���Ă��ꍇ
		else if (m_bSafe == true)
		{
			// ���p��UX�̕\��
			pShop->SetSaleUx(false);

			// �J�E���g�̃��Z�b�g
			m_nSaleCount = 0;
			m_nShopBlinkAlphaCount = 0;

			// ���S�n�т̔���
			m_bSafe = false;

			// �F�����Z�b�g����
			pShop->ResetColor();
		}
	}
}

//==============================================================================================
// �G�L�����Ƃ̓����蔻��
//==============================================================================================
void CCharacter::EnemyCollision()
{
	// �S�Ă̓G�L�����̏��
	for (auto pEnemy : CGame::GetEnemyManager()->GetAllEnemy())
	{
		D3DXVECTOR3 atherPos = pEnemy->GetPos();

		// �L�����N�^�[�ƓG�L�������ڐG�����ꍇ
		if (CollisionCircle(m_pos, m_fRadius, atherPos, pEnemy->GetRadius()))
		{
			// ���G���Ŗ������
			if (!m_bFlashing)
			{
				// �m�b�N�o�b�N�̊p�x
				float fPlayerAngle = (float)atan2(m_pos.x - atherPos.x, m_pos.z - atherPos.z);
				// �m�b�N�o�b�N
				KnockBack_Set(m_pos, atherPos, pEnemy->GetKnockBack(), 3);

				// �H��������p�[�e�B�N��
				CParticleEmitter::Create("EraseGhost", pEnemy->GetPos());

				// �G�L��������������
				pEnemy->Death();
			}
		}
	}
}

//==============================================================================================
// �����̕���
//==============================================================================================
void CCharacter::Shop_Direction()
{
	if (m_Shop_Direction == nullptr)
		// �����̕���
		m_Shop_Direction = CShop_Direction::Create({ m_pos.x,m_pos.y, m_pos.z }, { 0.0f,0.0f,0.0f }, { 100.0f,0.0f,100.0f });

	if (m_Shop_Direction != nullptr)
	{
		if (m_bEntry)
		{
			switch (m_nCharacterIndex)
			{
			case 0:
				m_Shop_Direction->SetCol({ 1.0f,0.0f,0.0f,1.0f });
				break;
			case 1:
				m_Shop_Direction->SetCol({ 0.0f,0.0f,1.0f,1.0f });
				break;
			case 2:
				m_Shop_Direction->SetCol({ 0.0f,1.0f,0.0f,1.0f });
				break;
			case 3:
				m_Shop_Direction->SetCol({ 1.0f,1.0f,0.0f,1.0f });
				break;
			default:
				break;
			}

			m_Shop_Direction->SetDaller(true);
		}
		else
		{
			m_Shop_Direction->SetCol({ 1.0f,1.0f,1.0f,1.0f });
			m_Shop_Direction->SetDaller(true);
		}

		if(m_bSafe)
		{
			m_Shop_Direction->SetCol({ 1.0f,1.0f,1.0f,0.0f });
			m_Shop_Direction->SetDaller(false);
		}

		// �����̏��
		CShop *pShop = CGame::GetShop(m_nCharacterIndex);
		D3DXVECTOR3 Shop_Pos = { 0.0f,0.0f,0.0f };

		// �������g�p����Ă���ꍇ
		if (pShop != nullptr)
		{
			// �ʒu�̎擾
			Shop_Pos = pShop->GetPos();
		}

		// �����̊p�x
		float fShop_Angle = (float)atan2(Shop_Pos.x - m_pos.x, Shop_Pos.z - m_pos.z);

		// �����܂ł̃x�N�g��
		D3DXVECTOR3 Vec = Shop_Pos - m_pos;

		// �x�N�g���̐��K��
		D3DXVec3Normalize(&Vec, &Vec);

		m_Shop_Direction->SetShopVec(Vec);

		// ���̈ʒu
		m_Shop_Direction->SetPos_Direction(m_pos);
		m_Shop_Direction->SetRot_Direction({ fShop_Angle,0.0f,0.0f });
	}
}

//==============================================================================================
// �L������UX��ݒ肷��
//==============================================================================================
void CCharacter::CharUx()
{
	switch (m_nCharacterIndex)
	{
	case 0:
		m_pScore = CScore::Create({ 0.0f, 40.0f, 0.0f }, { 20.0f, 30.0f, 0.0f }, 25.0f);
		m_pScore->SetCol(1.0f, 0.0f, 0.0f, 1.0f);
		m_pScoreItemStack = CScoreItemStack::Create({ 30.0f, 90.0f, 0.0f }, m_nCharacterIndex);
		break;
	case 1:
		m_pScore = CScore::Create({ 1080.0f, 40.0f, 0.0f }, { 20.0f, 30.0f, 0.0f }, 25.0f);
		m_pScore->SetCol(0.0f, 0.0f, 1.0f, 1.0f);
		m_pScoreItemStack = CScoreItemStack::Create({ 1120.0f, 90.0f, 0.0f }, m_nCharacterIndex);
		break;
	case 2:
		m_pScore = CScore::Create({ 0.0f, 320.0f, 0.0f }, { 20.0f, 30.0f, 0.0f }, 25.0f);
		m_pScore->SetCol(0.0f, 1.0f, 0.0f, 1.0f);
		m_pScoreItemStack = CScoreItemStack::Create({ 30.0f, 370.0f, 0.0f }, m_nCharacterIndex);
		break;
	case 3:
		m_pScore = CScore::Create({ 1080.0f, 320.0f, 0.0f }, { 20.0f, 30.0f, 0.0f }, 25.0f);
		m_pScore->SetCol(1.0f, 1.0f, 0.0f, 1.0f);
		m_pScoreItemStack = CScoreItemStack::Create({ 1120.0f, 370.0f, 0.0f }, m_nCharacterIndex);
		break;
	default:
		break;
	}
}

//==============================================================================================
// �w�����X�R�A�A�C�e���̐�������
//==============================================================================================
void CCharacter::Create_ScoreItem()
{
	char *Gem_Name = "\0";

	D3DXVECTOR3 Size = { 1.0f,1.0f,1.0f };

	// �X�e�[�W���Ƃ̍z��
	switch (CStage_Select::GetStage_Number())
	{
	case 0:
		Gem_Name = "Data/model/BasketDuringObject/Stage01/Geminthebasket.x";
		Size = { 1.3f,1.3f,1.3f };
		break;
	case 1:
		Gem_Name = "Data/model/BasketDuringObject/Stage02/Fossil.x";
		Size = { 2.0f,2.0f,2.0f };
		break;
	case 2:
		Gem_Name = "Data/model/BasketDuringObject/Stage03/Debris.x";
		Size = { 3.0f,3.0f,3.0f, };
		break;
	default:
		break;
	}

	for (int nCnt = 0; nCnt < Max_GemStack; nCnt++)
	{
		m_pOreStock[nCnt] = CModel::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, m_pModel[12], Gem_Name);
		m_pOreStock[nCnt]->SetSize(Size);
		m_pOreStock[nCnt]->SetShadow(false);	// �e��`�悵�Ȃ�
	}
}