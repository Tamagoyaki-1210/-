//=============================================================================
//
// �X�R�A
// Author : tanimoto kosuke
//
//=============================================================================
#include "score.h"
#include "application.h"
#include "texture.h"
#include "number.h"
#include <stdio.h>

CScore *CScore::m_pScore = nullptr;
//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CScore::CScore() : CObject2D(UI_PRIORITY_UI), m_nScore(0)
{
	
}

//=====================================
//�f�X�g���N�^
//=====================================
CScore::~CScore()
{

}

//=====================================
//����������
//=====================================
void CScore::Init()
{
	CObject2D::Init();

	//�O���t�@�C���ŕϐ��ɒl�����������g
	m_fWidth = SCORE_WIDTH;
	m_fHeight = SCORE_HEIGHT;

	//�I�u�W�F�N�g�ݒ菈��
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);
}

//=====================================
//�I������
//=====================================
void CScore::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CScore::Update()
{
	CObject2D::Update();

	//UV�ݒ菈��
	CObject2D::SetUV();
}

//=====================================
//�`�揈��
//=====================================
void CScore::Draw()
{
	CApplication::GetTexture()->TextureType(CTexture::TYPE_SCORE);

	CObject2D::Draw();
}

//=====================================
//�_���̐ݒ菈��
//=====================================
void CScore::SetScore()
{
	//�w��̌��̐��l�𒊏o
	for (int nInd = 0; nInd < MAX_DIGIT; nInd++)
	{
		//�v�Z(�X�R�A % 10�ׂ̂��� / �ЂƂ���10�ׂ̂���)
		int A = (int)pow(10.0f, (MAX_DIGIT - nInd ));
		int B = (int)pow(10.0f, (MAX_DIGIT - nInd - 1));
		m_apNumber[nInd]->SetNumber(m_nScore % A / B);
	}
}

//=====================================
//�_���̉��Z����
//=====================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	if (m_nScore > MAX_SCORE)
	{
		m_nScore = MAX_SCORE;
	}

	SetScore();
}

//=====================================
//�X�R�A��������
//=====================================
CScore *CScore::Create(D3DXVECTOR3 pos)
{
	m_pScore = new CScore;

	if (m_pScore != nullptr)
	{
		m_pScore->Init();
		m_pScore->SetPos(pos);
		for (int nInd = 0; nInd < MAX_DIGIT; nInd++)
		{
			m_pScore->m_apNumber[nInd] = CNumber::Create(D3DXVECTOR3(pos.x - (MAX_DIGIT / 2 * 20 - 10) + (20 * nInd), pos.y + 10.0f, 0.0f), CNumber::TYPE_SCORE);
		}
		m_pScore->SetScore();
	}
	return m_pScore;
}

//=====================================
//�����L���O�̏������ݏ���
//=====================================
void CScore::Save(void)
{
	int aRankingData[RANKING_NUM] = {};

	//+++++++++++++++++++++++++++++++++++++
	//�����L���O�ǂݍ���
	//+++++++++++++++++++++++++++++++++++++
	FILE* pFileLoad = fopen(RNKING_FILE, "r");	//�t�@�C�����J��

	if (pFileLoad != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFileLoad, "%d", &aRankingData[0]);
	 //�����L���O�t�@�C������ǂݍ���
		for (int nID = 0; nID < RANKING_NUM; nID++)
		{
			fscanf(pFileLoad, "%d", &aRankingData[nID]);
		}
	}

	//�t�@�C�������
	fclose(pFileLoad);

	//*************************************
	//�o�u���\�[�g�̑S�̏���
	//*************************************
	int nMaxID = 0;	//�n�C�X�R�A��ID
	if (m_nScore > aRankingData[RANKING_NUM - 1])
	{
		aRankingData[RANKING_NUM - 1] = m_nScore;

		int nMax = 0;
		//�����L���O�f�[�^���X�V
		for (int nCnt1 = 0; nCnt1 < RANKING_NUM - 1; nCnt1++)
		{//1���z��̈ʒu���グ�鏈��
			for (int nCnt2 = nCnt1 + 1; nCnt2 < RANKING_NUM; nCnt2++)
			{
				if (aRankingData[nCnt1] < aRankingData[nCnt2])
				{
					nMax = aRankingData[nCnt2];
					aRankingData[nCnt2] = aRankingData[nCnt1];
					aRankingData[nCnt1] = nMax;
				}
			}
		}

		for (int nCnt1 = 0; nCnt1 < RANKING_NUM; nCnt1++)
		{
			if (aRankingData[nCnt1] == m_nScore)
			{
				nMaxID = nCnt1 + 1;
				break;
			}
		}
	}

	//+++++++++++++++++++++++++++++++++++++
	//�����L���O��������
	//+++++++++++++++++++++++++++++++++++++
	FILE* pFileSave = fopen(RNKING_FILE, "w");	//�t�@�C�����J��

	if (pFileSave != NULL)
	{//�t�@�C�����J�����ꍇ
		fprintf(pFileSave, "%d\n", nMaxID);
	 //�����L���O�t�@�C���ɏ�������
		for (int nID = 0; nID < RANKING_NUM; nID++)
		{
			fprintf(pFileSave, "%d\n", aRankingData[nID]);
		}
	}
	else
	{
		printf("�G���[ : �t�@�C�����J���܂���ł���\n");
	}

	//�t�@�C�������
	fclose(pFileSave);

	//+++++++++++++++++++++++++++++++++++++
	//�����L���O��������
	//+++++++++++++++++++++++++++++++++++++
	FILE* pFileThisScore = fopen(THIS_SCORE_FILE, "w");	//�t�@�C�����J��

	if (pFileThisScore != NULL)
	{//�t�@�C�����J�����ꍇ
		fprintf(pFileThisScore, "%d\n", m_nScore);
	}
	else
	{
		printf("�G���[ : �t�@�C�����J���܂���ł���\n");
	}

	//�t�@�C�������
	fclose(pFileThisScore);
}