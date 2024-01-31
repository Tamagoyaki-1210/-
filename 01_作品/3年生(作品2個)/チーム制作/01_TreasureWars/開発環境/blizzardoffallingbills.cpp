//==============================================================================================
//
// �D����@�@�@blizzardoffallingbills.cpp
// koduna hirohito
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include"blizzardoffallingbills.h"
#include "texture.h"

//==============================================================================================
//�ÓI�Œ�ϐ�
//==============================================================================================
const float CBlizzardOfFallingBills::WIDTH_PER_PATTERN_ONE = 1.0f / EFFECT_PATTERN_MAX;
const float CBlizzardOfFallingBills::BASIC_ROT_MOVE = D3DXToRadian(0.5f);
const float CBlizzardOfFallingBills::BASIC_POS_MOVE = 1.0f;
const float CBlizzardOfFallingBills::RANGE_WITH_PARENT = 200.0f;
const float CBlizzardOfFallingBills::RIGHT_LEFT_WIDTH = D3DXToRadian(80);

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CBlizzardOfFallingBills::CBlizzardOfFallingBills(int nPriority) : CObject2D(nPriority)
{
	m_fParentRot = 0.0f;								//���z�e��Rot
	m_ParentPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���z�e��Pos
	m_fParentRotMove = 0.0f;							//���z�e��Rot�̈ړ���
	m_ParentPosMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���z�e��Pos�̈ړ���
	m_bRightLeft = false;								//�E��
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CBlizzardOfFallingBills::~CBlizzardOfFallingBills()
{
}

//==============================================================================================
// ��������
//==============================================================================================
CBlizzardOfFallingBills *CBlizzardOfFallingBills::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority)
{
	CBlizzardOfFallingBills *pObject = nullptr;

	pObject = new CBlizzardOfFallingBills(nPriority);

	if (pObject != nullptr)
	{
		pObject->SetSize(size.x, size.y);

		//�o�^����q�̈ʒu
		D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�f�[�^�̎Z�o
		Pos.x = sinf(0.0f) * RANGE_WITH_PARENT;
		Pos.y = cosf(0.0f) * RANGE_WITH_PARENT;

		pObject->SetParentPos(pos);
		pObject->Setposition(Pos + pos);
		pObject->Init();
	}

	return pObject;
}

//==============================================================================================
// �D����̏�����
//==============================================================================================
HRESULT CBlizzardOfFallingBills::Init()
{
	//�e�̏�����
	if (FAILED(CObject2D::Init()))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̐ݒ�
	SetTexture(CTexture::TEXTURE_BILL_SNOWSTORM);

	//�e�N�X�`���̃����_������
	int nPattern = rand() % EFFECT_PATTERN_MAX;

	//�e�N�X�`���̈ʒu
	float fTexPos = WIDTH_PER_PATTERN_ONE;

	//�e�N�X�`���̈ʒu�ݒ�
	SetTexPos(0.0f, 1.0f, fTexPos * (nPattern + 1), fTexPos * nPattern);

	//�E���̃����_����
	if (rand() % 2 == 0)
	{
		m_bRightLeft = true;
	}
	else
	{
		m_bRightLeft = false;
	}

	return S_OK;
}

//==============================================================================================
// �D����̍X�V
//==============================================================================================
void CBlizzardOfFallingBills::Update()
{
	//���z�e�̈ʒu�X�V
	m_ParentPos.y += BASIC_POS_MOVE;

	//���z�e�̌����X�V
	if (m_bRightLeft)
	{
		m_fParentRot += BASIC_ROT_MOVE;

		if (RIGHT_LEFT_WIDTH < m_fParentRot)
		{
			m_bRightLeft = false;
		}
	}
	else
	{
		m_fParentRot -= BASIC_ROT_MOVE;

		if (-RIGHT_LEFT_WIDTH> m_fParentRot)
		{
			m_bRightLeft = true;
		}
	}

	//�o�^����q�̈ʒu
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�f�[�^�̎Z�o
	pos.x = sinf(m_fParentRot) * RANGE_WITH_PARENT;
	pos.y = cosf(m_fParentRot) * RANGE_WITH_PARENT;

	//�ʒu��ύX
	Setposition(pos + m_ParentPos);

	//�����̕ύX
	Setrot(m_fParentRot);

	//�e�̍X�V
	CObject2D::Update();
}