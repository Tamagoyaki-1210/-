//==============================================================================================
//
// �����L���O�@�@�@ranking.cpp
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// �C���N���[�h
//**********************************************************************************************
#include<stdio.h>
#include"ranking.h"
#include "result.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CRanking::CRanking()
{
	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		m_pScore[nCnt] = nullptr;
		m_Result[nCnt].m_nScore = 0;		// �X�R�A�̏����l
		m_Result[nCnt].m_nRank = nCnt + 1;	// �����N�̏����l
	}
}

//==============================================================================================
// �f�X���N�^
//==============================================================================================
CRanking::~CRanking()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CRanking::Init()
{
	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		// �X�R�A
		m_pScore[nCnt] = CScore::Create({ 330.0f * nCnt, m_Pos.y, m_Pos.z }, m_Size, 25.0f);
		m_pScore[nCnt]->SetRollSpeed(Result_Score_RoolSpeed);
	}

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CRanking::Uninit()
{
	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		// �X�R�A
		m_pScore[nCnt]->Uninit();
		m_pScore[nCnt] = nullptr;
	}
}

//==============================================================================================
// �X�V����
//==============================================================================================
void CRanking::Update()
{
	// �X�R�A�̕\��
	DisplayRanking();
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CRanking::Draw()
{
}

//==============================================================================================
// ��������
//==============================================================================================
CRanking *CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CRanking *pRanking = new CRanking;

	if (pRanking != nullptr)
	{
		pRanking->SetSize(size);
		pRanking->SetCol(col);
		pRanking->SetPos(pos);
		pRanking->Init();
	}

	return pRanking;
}

//==============================================================================================
// �X�R�A�̐ݒ�
//==============================================================================================
void CRanking::SetScore(int score, int cnt)
{
	// �X�R�A�̑��
	m_pScore[cnt]->SetScore(score);

	// �X�R�A�̕��ёւ�
	//CompareScore(m_pScore[cnt]->GetScore(), cnt);

	m_Result[cnt].m_nScore = score;

	
}

//==============================================================================================
// �X�R�A����ѕς���
//==============================================================================================
void CRanking::CompareScore(int score, int cnt)
{
	// �X�R�A�Ɣԍ��̊i�[��
	int nSave_Score = 0;
	int nSave_Index = 0;

	if (score >= m_Result[m_nNum_Ranking - 1].m_nScore)
	{
		if (score != -1)
		{
			// �X�R�A�̑��
			m_Result[m_nNum_Ranking - 1].m_nScore = score;
		}

		// �v���C���[�ԍ��̑��
		m_Result[m_nNum_Ranking - 1].m_nIndex = cnt;
	}

	for (int nCnt = 0; nCnt < (m_nNum_Ranking - 1); nCnt++)
	{
		for (int nCnt2 = (nCnt + 1); nCnt2 < m_nNum_Ranking; nCnt2++)
		{
			if (m_Result[nCnt].m_nScore < m_Result[nCnt2].m_nScore)
			{
				// �X�R�A�̓���ւ�
				nSave_Score = m_Result[nCnt].m_nScore;
				m_Result[nCnt].m_nScore = m_Result[nCnt2].m_nScore;
				m_Result[nCnt2].m_nScore = nSave_Score;

				// �v���C���[�ԍ��̓���ւ�
				nSave_Score = m_Result[nCnt].m_nIndex;
				m_Result[nCnt].m_nIndex = m_Result[nCnt2].m_nIndex;
				m_Result[nCnt2].m_nIndex = nSave_Score;
			}
		}
	}

	// �����̏����p
	int nContinue = -1;

	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		if (nCnt <= nContinue)
		{
			continue;
		}

		for (int nCnt2 = (nCnt + 1); nCnt2 < m_nNum_Ranking; nCnt2++)
		{
			if (m_Result[nCnt].m_nScore == m_Result[nCnt2].m_nScore)
			{
				// �����̏ꍇ�������ʂɂ���
				m_Result[nCnt].m_nRank = nCnt + 1;
				m_Result[nCnt2].m_nRank = nCnt + 1;

				// �ǂ̏��ʂ܂œ����ɂȂ��Ă��邩
				nContinue = nCnt2 - 1;
			}
		}
	}
}

//==============================================================================================
// �X�R�A�̕\��
//==============================================================================================
void CRanking::DisplayRanking()
{
	for (int nCnt = 0; nCnt < m_nNum_Ranking; nCnt++)
	{
		if (m_pScore[nCnt] != nullptr)
		{
			// �X�R�A�̕\��
			m_pScore[nCnt]->SetScore(m_Result[nCnt].m_nScore);
		}
		if (m_pNumber[nCnt] != nullptr)
		{
			// �e�N�X�`�����W�̐ݒ�
			m_pNumber[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * (m_Result[nCnt].m_nIndex + 1) + 0.1f, (m_Result[nCnt].m_nIndex + 1) * 0.1f);
		}
		if (m_pRank[nCnt] != nullptr)
		{
			// �e�N�X�`�����W�̐ݒ�
			m_pRank[nCnt]->SetTexPos(0.0f, 1.0f, 0.1f * m_Result[nCnt].m_nRank + 0.1f, m_Result[nCnt].m_nRank * 0.1f);
		}
		if (m_pScore[nCnt] != nullptr)
		{
			bool  f = CResult::GetFlag();

			// �����L���O��ʂł̃X�R�A�̐F
			if (f == true)
			{
				m_Alpha += 0.01f;

				m_pScore[0]->SetCol(1.0f, 0.0f, 0.0f, m_Alpha);
				m_pScore[1]->SetCol(0.0f, 0.0f, 1.0f, m_Alpha);
				m_pScore[2]->SetCol(0.0f, 1.0f, 0.0f, m_Alpha);
				m_pScore[3]->SetCol(1.0f, 1.0f, 0.0f, m_Alpha);
			}
			else
			{
				for (int i = 0; i < 4; i++)
				{
					m_pScore[i]->SetCol(0.0f, 0.0f, 0.0f, 0.0f);
				}
			}
		}
	}
}
