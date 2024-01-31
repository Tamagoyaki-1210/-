//=============================================================================
//
// time.h (�^�C��.h)
// Author : Saito Shian
//
//=============================================================================

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "application.h"
#include "renderer.h"
#include "time.h"
#include "game.h"
#include "fade.h"
#include "number.h"
#include "input.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTime::CTime()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTime::~CTime()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTime::Init()
{
	//==================================================
	// �����o�ϐ��̏�����
	//==================================================  
	m_nTime = 0;				// ���Ԃ̒l�̏�����
	m_nZeroCount = 0;				// 0�̐��̏�����
	m_nCntTime = 0;

	//�����̐���
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(m_pos.x + (70.0f * nCnt) + 50.0f / 2, m_pos.y, 0.0f), 30.0f, 30.0f,CObject::PRIORITY_SCREEN);
		//������,���Ԗڂ�
		m_apNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTime::Uninit()
{
	//�����o�ϐ��̏�����
	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt] != nullptr)
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CTime::Update()
{
	//�^�C���̐ݒ�
	SetTime();
}

//=============================================================================
// �^�C���̐ݒ�
//=============================================================================
void CTime::SetTime()
{
	CApplication::MODE Mode = CApplication::GetModeType();
	if (Mode == CApplication::MODE_GAME)
	{
		m_nCntTime++;
		// 1�b = 60f && �c�莞�Ԃ�0���傫���ꍇ
		if (m_nCntTime > 60 /*&& m_nTime > 0*/)
		{
			m_nTime++;
			m_nCntTime = 0;
		}
		m_nKeepTime = m_nTime;
	}

	//if (m_nTime <= 0)
	//{
	//	CFade::SetFade(CApplication::MODE_RESULT, 0.05f);
	//}

		m_aPosTexU[0] = m_nTime % 1000 / 100;
		m_aPosTexU[1] = m_nTime % 100 / 10;
		m_aPosTexU[2] = m_nTime % 10 / 1;

		for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
		{
			//������,���Ԗڂ�
			m_apNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
		}

		//// �擪��0����������
		//ZoroCount();
}

//=============================================================================
// �^�C���̐ݒ�
//=============================================================================
void CTime::SetTime(int time)
{
	m_aPosTexU[0] = time % 1000 / 100;
	m_aPosTexU[1] = time % 100 / 10;
	m_aPosTexU[2] = time % 10 / 1;

	for (int nCnt = 0; nCnt < TIME_DIGIT; nCnt++)
	{
		//������,���Ԗڂ�
		m_apNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_aPosTexU[nCnt] + 0.1f, m_aPosTexU[nCnt] * 0.1f);
		// �擪��0����������
		ZoroCount();
	}
}

//=============================================================================
// �擪��0����������
//=============================================================================
void CTime::ZoroCount()
{
	if (m_apNumber[m_nZeroCount] != nullptr)
	{
		if (m_aPosTexU[m_nZeroCount] == 0 && m_nTime != 0 && m_nZeroCount <= 2)
		{
			m_apNumber[m_nZeroCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCount++;
		}
		else if (m_aPosTexU[m_nZeroCount] == 0 && m_nTime == 0 && m_nZeroCount <= 2)
		{
			m_apNumber[1]->SetTexPos(0.0f, 1.0f, 0.1f, 0.0f);
			m_apNumber[m_nZeroCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_nZeroCount++;
		}
		if (m_nTime == 0)
		{
			m_apNumber[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//=============================================================================
// �^�C���̎擾
//=============================================================================
int CTime::GetTime()
{
	return m_nKeepTime;
}

//=============================================================================
// �^�C���̐���
//=============================================================================
CTime* CTime::Create(D3DXVECTOR3 pos)
{
	//�N���X�̐���
	CTime* pTime = new CTime;

	//null�`�F�b�N
	if (pTime != nullptr)
	{
		pTime->SetPos(pos);
		//����������
		pTime->Init();
	}
	else
	{
		assert(false);
	}

	return pTime;
}