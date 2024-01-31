//==============================================================================================
//
// ���U���g�@�@�@result.cpp
// tutida ryousei
//
//==============================================================================================
#include<time.h>
#include"result.h"
#include"input.h"
#include"application.h"
#include"fade.h"
#include"game.h"
#include"camera.h"
#include"3Dpolygon.h"
#include"Player.h"
#include"file.h"
#include"cpu.h"
#include"objectX.h"
#include"ore.h"
#include"result_obj.h"
#include"ranking.h"
#include"score.h"
#include"piller.h"
#include"logo.h"
#include"title_obj.h"
#include"entry.h"
#include"stage_select.h"

const float CResult::Moving_Piller[Ranking_Max] = { 425.0f, 325.0f, 225.0f, 125.0f };

// ���̈ʒu���
const D3DXVECTOR3 CResult::Piller_Pos[Ranking_Max] = {
	D3DXVECTOR3(-510.0f, -700.0f, 0.0f),
	D3DXVECTOR3(-170.0f, -700.0f, 0.0f),
	D3DXVECTOR3(170.0f, -700.0f, 0.0f),
	D3DXVECTOR3(510.0f, -700.0f, 0.0f),
};

int CResult::m_nScore[Ranking_Max] = {};
bool CResult::m_bFlag = false;

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CResult::CResult()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CResult::~CResult()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CResult::Init()
{
	// ���U���g���BGM
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_RESULT);

	// �J�����̃|�C���^
	CCamera* pCamera = CApplication::GetCamera();

	pCamera->SetPosV(D3DXVECTOR3(0.0f, 330.0f, -1000.0f));
	pCamera->SetPosR(D3DXVECTOR3(0.0f, 330.0f, 0.0f));

	//�N�����Ɉ�񂾂��s�����ߏ������ɏ���	
	srand((unsigned int)time(nullptr));

	//==================================================
	// �����o�ϐ��̏���������
	//==================================================
	//�w�i
	C3DPolygon *pPolygon = C3DPolygon::Create(D3DXVECTOR3(0.0f, 0.0f, 900.0f), D3DXVECTOR3(D3DXToRadian(-90), 0.0f, 0.0f), D3DXVECTOR2(2000.0f, 2000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	
	// �z�΂̃��f���ԍ�
	int nIndexOre = 0;

	//�w�i�̃e�N�X�`���ݒ�
	switch (CStage_Select::GetStage_Number())
	{
	case 0:
		pPolygon->SetTextIndex(CTexture::TEXTURE_FLOOR);
		nIndexOre = CResultObj::ReadObject("Data/model/ore.x");
		break;
	case 1:
		pPolygon->SetTextIndex(CTexture::TEXTURE_JUNGLE);
		nIndexOre = CResultObj::ReadObject("Data/model/Collecting_Point/Stage_02/Fossil_01.x");
		break;
	case 2:
		pPolygon->SetTextIndex(CTexture::TEXTURE_MOON);
		nIndexOre = CResultObj::ReadObject("Data/model/Collecting_Point/Stage_03/Artificial_Satellite_01.x");
		break;
	default:
		break;
	}

	//�z�΂̋���
	for (int nCnt = 0; nCnt < RESULTORE_MAX; nCnt++)
	{
		CResultObj::Create(D3DXVECTOR3(0.0f, -495.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), nIndexOre);
	}

	//====================================================
	// ��r
	//====================================================
	Comparison();

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CResult::Uninit()
{
	// ���U���g���BGM�̏I��
	CApplication::GetSound()->StopLabel(CSound::SOUND_LABEL_BGM_RESULT);

	// �I���t���O���U�ɂ���
	m_bFlag = false;
}

//==============================================================================================
//�X�V����
//==============================================================================================
void CResult::Update()
{
	// �I���t���O���ݒ肳��Ă���ꍇ
	if (m_bFlag == true)
	{
		CInput* pInput = CInput::GetKey();

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if ((pInput->Trigger(DIK_RETURN) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
				|| pInput->Trigger(JOYPAD_START, nCnt))
				&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
			{
				CFade::SetFade(CApplication::MODE_TITLE, 0.05f);
			}
		}
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CResult::Draw()
{

}

//==============================================================================
// ��r����
//==============================================================================
void CResult::Comparison()
{
	//===================================
	// �ϐ��錾
	//===================================
	int no_1 = 0;
	int no_2 = 0;
	int no_3 = m_nScore[0];
	int min = m_nScore[0];
	int min2 = m_nScore[1];

	//===================================
	// ��r
	//===================================
	for (int i = 0; i < Ranking_Max; i++)
	{
		//�ő�l
		if (no_1 < m_nScore[i])
		{//�X�R�A��0���傫��������
			no_2 = no_1;				//��Ԗڂɑ傫���l�i�[�ϐ��ɍő�l���i�[
			no_1 = m_nScore[i];			//�ő�l�i�[�ϐ����̃X�R�A���i�[
		}
		//��Ԗڂɑ傫�����l
		else if (no_2 < m_nScore[i])
		{//�X�R�A����Ԗڂɑ傫���l���傫��������
			no_2 = m_nScore[i];			//��Ԗڂɑ傫���l�i�[�ϐ��ɂ��̃X�R�A���i�[
		}

		//�ŏ��l
		if (no_3 > m_nScore[i])
		{
			no_3 = m_nScore[i];
		}
	}

	//��Ԗڂɏ��������l
	for (int i = 1; i < 4; i++)
	{
		if (m_nScore[i] < min)
		{
			min2 = min;
			min = m_nScore[i];
		}
		else if (m_nScore[i] < min2)
		{
			min2 = m_nScore[i];
		}
	}

	//===================================
	// �X�R�A�i�[
	//===================================
	const int nKeepScoreMax = no_1;		//�ő�l
	const int nKeepScoreMax2 = no_2;	//��Ԗڂɑ傫���l
	const int nKeepScoreMin = no_3;		//�ŏ��l
	const int nKeepScoreMin2 = min2;	//��Ԗڂɏ������l

	CRanking* pRanking = CRanking::Create({ -325.0f,650.0f,0.0f }, { 20.0f,35.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });

	int nMovingIndex = 0;	// �����L���O�p�X�R�A
	const int nIndexPiller = CObjectX::ReadObject("Data/model/01_RankingPillar.x");

	//��ʂ���l�ʂ܂ł̃X�R�A
	for (int nCnt = 0; nCnt < Ranking_Max; nCnt++)
	{
		if (m_nScore[nCnt] == nKeepScoreMax)
		{//�ő�l
			nMovingIndex = 0;
		}
		else if (m_nScore[nCnt] == nKeepScoreMax2)
		{//��Ԗڂɑ傫���l
			nMovingIndex = 1;
		}
		else if (m_nScore[nCnt] == nKeepScoreMin2)
		{//��Ԗڂɏ������l
			nMovingIndex = 2;
		}
		else if (m_nScore[nCnt] == nKeepScoreMin)
		{//�ŏ��l
			nMovingIndex = 3;
		}

		// ���̐���
		CPiller::Create(Piller_Pos[nCnt], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), nIndexPiller, Moving_Piller[nMovingIndex], nCnt);
		CLogo::Create(D3DXVECTOR3(200.0f + (nCnt * 295.0f), 1300.0f, 0.0f), 50.0f, 50.0f, Moving_Piller[nMovingIndex]);
		pRanking->SetScore(m_nScore[nCnt], nCnt);
	}
}

//==============================================================================================
// ��������
//==============================================================================================
CResult *CResult::Create()
{
	CResult *pResult = nullptr;

	pResult = new CResult;

	if (pResult != nullptr)
	{
		pResult->Init();
	}

	return pResult;
}
