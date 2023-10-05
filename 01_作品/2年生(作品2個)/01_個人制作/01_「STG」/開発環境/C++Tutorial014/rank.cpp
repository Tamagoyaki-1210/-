//=============================================================================
//
// ����
// Author : tanimoto kosuke
//
//=============================================================================
#include "rank.h"
#include "number.h"
#include <stdio.h>
#include "score.h"
#include "application.h"
#include "texture.h"

//=====================================
//�f�t�H���g�R���X�g���N�^
//=====================================
CRank::CRank() : CObject2D(UI_PRIORITY_UI)
{

}

//=====================================
//�f�X�g���N�^
//=====================================
CRank::~CRank()
{

}

//=====================================
//����������
//=====================================
void CRank::Init()
{
	CObject2D::Init();

	m_fWidth = COMMA_WIDTH;
	m_fHeight = COMMA_HEIGHT;

	//�I�u�W�F�N�g�ݒ菈��
	CObject2D::SetObject2D(TYPE_UI, m_fWidth, m_fHeight);
}

//=====================================
//�I������
//=====================================
void CRank::Uninit()
{
	CObject2D::Uninit();
}

//=====================================
//�X�V����
//=====================================
void CRank::Update()
{
	CObject2D::Update();

	//UV�ݒ菈��
	CObject2D::SetUV();
}

//=====================================
//�`�揈��
//=====================================
void CRank::Draw()
{
	CApplication::GetTexture()->TextureType(CTexture::TYPE_COMMA);

	CObject2D::Draw();
}

//=====================================
//�X�R�A��������
//=====================================
CRank *CRank::Create(D3DXVECTOR3 pos, int nID)
{
	CRank* pRank = new CRank;		//�X�R�A�|�C���^

	if (pRank != nullptr)
	{
		pRank->Init();
		pRank->SetPos(pos);

		CNumber *pNumber2 = CNumber::Create(pos, CNumber::TYPE_RANKING);

		//2��
		pNumber2->SetPos(D3DXVECTOR3(pos.x - pNumber2->GetWidth() * 2, pos.y, 0.0f));
		pNumber2->SetNumber((nID + 1) / 10);

		CNumber *pNumber = CNumber::Create(pos, CNumber::TYPE_RANKING);
		//1��
		pNumber->SetPos(D3DXVECTOR3(pos.x - pNumber->GetWidth(), pos.y, 0.0f));
		pNumber->SetNumber((nID + 1) % 10);
	}
	return pRank;
}