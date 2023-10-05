//=============================================================================
//
// receipt.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "receipt.h"
#include "application.h"
#include "renderer.h"
#include "object2D.h"
#include "fontString.h"

const D3DXVECTOR3 CReceipt::Font_Size = { 12.0f, 12.0f, 0.0f };
const float CReceipt::FontString_Blink_Speed = 0.02f;
//=====================================
// �f�t�H���g�R���X�g���N�^
//=====================================
CReceipt::CReceipt()
{

}

//=====================================
// �f�X�g���N�^
//=====================================
CReceipt::~CReceipt()
{

}

//============================================================================
// ����������
//============================================================================
HRESULT CReceipt::Init()
{
	m_nTotalValue = 0;

	return S_OK;
}

//============================================================================
// �I������
//============================================================================
void CReceipt::Uninit()
{
	for (int nCnt = 0; nCnt < (int)m_apFontString.size(); nCnt++)
	{
		m_apFontString[nCnt]->Uninit();
		m_apFontString[nCnt] = nullptr;
	}

	m_apFontString.clear();

	Release();
}

//============================================================================
// �X�V����
//============================================================================
void CReceipt::Update()
{
	// �^�C�}�[���g�p����Ă���ꍇ
	if (m_nStringDesthCounter > 0)
	{
		m_nStringDesthCounter--;

		// ���������ɂȂ�����
		if (m_nStringDesthCounter <= String_Death_Timer)
		{
			// �_�ł��I��������
			m_apFontString[0]->BlinkFinish();

			// ��ԉ��̕��������킶��Ə�������
			m_apFontString[0]->SetAlpha(0.6f * m_nStringDesthCounter / String_Death_Timer);

			// �����鎞�ԂɒB�����ꍇ
			if (m_nStringDesthCounter <= 0)
			{
				// ����
				m_nStringDesthCounter = 0;
				DestroyString();
			}
		}
	}
}

//============================================================================
// �`�揈��
//============================================================================
void CReceipt::Draw()
{

}

//============================================================================
// ���V�[�g�̍��v�l�̏���
//============================================================================
void CReceipt::Total()
{
	if (m_pTotal == nullptr)
	{
		m_pTotal = CFontString::Create({ m_pos.x - 15.0f , m_pos.y - 30.0f , m_pos.z }, {15.0f, 15.0f, 0.0f}, "<c=255,215,0,255,>��������</>�@$<c=255,215,0,255,>" + std::to_string(m_nTotalValue) + "</>");
	}
	else
	{
		m_pTotal->SetString("<c=255,215,0,255,>��������</>�@$<c=255,215,0,255,>" + std::to_string(m_nTotalValue) + "</>");
	}
}

//============================================================================
// ������̔j������
//============================================================================
void CReceipt::DestroyString()
{
	// �����񂪎g�p����Ă���ꍇ
	if (!m_apFontString.empty())
	{
		// ��ԏ�̍s���폜����
		m_apFontString.front()->Uninit();
		m_apFontString.erase(m_apFontString.begin());

		// �����񂪎c���Ă���ꍇ
		if (!m_apFontString.empty())
		{
			// ������̃A���J�[�|�C���g�̐ݒ菈��
			SetStringAnchor();

			m_nStringDesthCounter = String_Death_Timer;
		}
		// �����񂪑S�ď������ꍇ
		else
		{
			// ���v������
			m_pTotal->Uninit();
			m_pTotal = nullptr;
			m_nTotalValue = 0;
		}
	}
}

//============================================================================
// ������̐ݒ菈��
//============================================================================
void CReceipt::SetString(const std::string letter)
{
	// �ő�s���ɒB���Ă���ꍇ
	if ((int)m_apFontString.size() >= Max_Row)
	{
		// ��Ԍ��̍s���폜����
		m_apFontString.front()->Uninit();
		m_apFontString.erase(m_apFontString.begin());
	}

	// �����ʒu��ς��鏈��
	D3DXVECTOR3 sideFirstPos = { -(Font_Size.x * 2.0f * Max_Len), Font_Size.y * 2.0f * m_apFontString.size(), 0.0f };

	if (m_bRight)
	{
		sideFirstPos.x *= -1.0f;
	}

	// ������̐������Ċi�[����
	m_apFontString.push_back(CFontString::Create({ m_pos.x + sideFirstPos.x, m_pos.y + sideFirstPos.y, m_pos.z }, Font_Size, letter));

	// ������̃A���J�[�|�C���g�̐ݒ菈��
	SetStringAnchor();

	// �_�ł��J�n����
	m_apFontString[m_apFontString.size() - 1]->SetBlink(FontString_Blink_Speed);

	// ������̏�����^�C�~���O��ݒ肷��
	m_nStringDesthCounter = Setting_String_Death_Timer;
}

//============================================================================
// ������̃A���J�[�|�C���g�̐ݒ菈��
//============================================================================
void CReceipt::SetStringAnchor()
{
	// ���݂̍ő�s��
	int stringSize = (int)m_apFontString.size();

	// �S������̈ʒu�ݒ�
	for (int nCnt = 0; nCnt < stringSize; nCnt++)
	{
		float fAnchorBottom = Font_Size.y * 2.0f * nCnt;

		// �A���J�[�ʒu��ݒ肷��
		m_apFontString[nCnt]->SetAnchor(D3DXVECTOR3( m_pos.x, m_pos.y + fAnchorBottom, m_pos.z ));

		// �_�ł��I��������
		m_apFontString[nCnt]->BlinkFinish();
	}
	// 2�܂Ŕz�񂪐�������Ă���ꍇ
	if (stringSize >= 1)
	{
		if (stringSize >= 2)
		{
			m_apFontString[1]->SetAlpha(0.8f);
			m_apFontString[0]->SetAlpha(0.6f);
		}
		else
		{
			m_apFontString[0]->SetAlpha(0.8f);
		}
	}
	Total();
}

//============================================================================
// ��������
//============================================================================
CReceipt* CReceipt::Create(const int index)
{
	CReceipt* pReceipt = new CReceipt;

	if (FAILED(pReceipt->Init()))
	{
		return nullptr;
	}

	switch (index)
	{
	case 0:
		pReceipt->m_pos = { 20.0f, 170.0f, 0.0f };
		break;
	case 1:
		pReceipt->m_pos = { 1240.0f - ((pReceipt->Font_Size.x + 5.0f) * pReceipt->Max_Len), 170.0f, 0.0f };
		break;
	case 2:
		pReceipt->m_pos = { 20.0f, 450.0f, 0.0f };
		break;
	case 3:
		pReceipt->m_pos = { 1240.0f - ((pReceipt->Font_Size.x + 5.0f) * pReceipt->Max_Len), 450.0f, 0.0f };
		break;
	default:
		break;
	}

	// �����ʒu����ʔ������E���������ꍇ
	pReceipt->m_bRight = (pReceipt->m_pos.x > (1280.0f * 0.5f)) ? true : false;

	return pReceipt;
}
