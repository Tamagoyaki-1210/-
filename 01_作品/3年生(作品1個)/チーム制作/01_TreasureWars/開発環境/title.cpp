//==============================================================================================
//
// �^�C�g���@�@�@title.cpp
// tutida ryousei
//
//==============================================================================================
#include<time.h>
#include"title.h"
#include "input.h"
#include"application.h"
#include"fade.h"
#include"camera.h"
#include"skybox.h"
#include"objectX.h"
#include"title_obj.h"
#include "blizzardoffallingbills.h"
#include "object2D.h"
#include "3Dpolygon.h"
#include "particle_emitter.h"

//==============================================================================================
// �R���X�g���N�^
//==============================================================================================
CTitle::CTitle()
{
}

//==============================================================================================
// �f�X�g���N�^
//==============================================================================================
CTitle::~CTitle()
{
}

//==============================================================================================
// ����������
//==============================================================================================
HRESULT CTitle::Init()
{
	// �^�C�g�����BGM
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_TITLE);

	// �J�����̃|�C���^
	CCamera* pCamera = CApplication::GetCamera();

	pCamera->SetPosV(D3DXVECTOR3(0.0f, 220.0f, -1000.0f));
	pCamera->SetPosR(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�N�����Ɉ�񂾂��s�����ߏ������ɏ���	
	srand((unsigned int)time(nullptr));

	//==================================================
	// �����o�ϐ��̏���������
	//==================================================
	m_RandX = 0.0f;
	m_RandY = 0.0f;

	for (int nCnt = 0; nCnt < TITLEORE_MAX; nCnt++)
	{
		CTitleObj::Create(D3DXVECTOR3(0.0f, -495.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f),"Data/model/ore.x", CTitleObj::ObjType_Ore);
	}

	CTitleObj::Create(D3DXVECTOR3(0.0f, -1000.0f, -750.0f), D3DXVECTOR3(0.0f, D3DXToRadian(20), 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), "Data/model/pileofbills.x", CTitleObj::ObjType_Pileofbills);
	
	CObject2D *pCObject2D = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(480.0f, 270.0f, 0.0f));
	pCObject2D->SetTexture(CTexture::TEXTURE_TITLE);

	//�w�i
	C3DPolygon *pPolygon = C3DPolygon::Create(D3DXVECTOR3(0.0f, 0.0f, 900.0f), D3DXVECTOR3(D3DXToRadian(-90), 0.0f, 0.0f), D3DXVECTOR2(2000.0f, 2000.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pPolygon->SetTextIndex(CTexture::TEXTURE_FLOOR);

	return S_OK;
}

//==============================================================================================
// �I������
//==============================================================================================
void CTitle::Uninit()
{
	// �^�C�g�����BGM�̏I��
	CApplication::GetSound()->StopLabel(CSound::SOUND_LABEL_BGM_TITLE);
}

//==============================================================================================
//�X�V����
//==============================================================================================
void CTitle::Update()
{
	CInput* pInput = CInput::GetKey();

	if (pInput->Trigger(DIK_0))
	{
		CBlizzardOfFallingBills::Create(D3DXVECTOR3(1280.0f * 0.5f,-10.0f,0.0f), D3DXVECTOR3(50.0f,50.0f,0.0f));
	}

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if ((pInput->Trigger(DIK_RETURN) || pInput->Trigger(DIK_SPACE) || pInput->Press(JOYPAD_B, nCnt) || pInput->Press(JOYPAD_A, nCnt)
			|| pInput->Trigger(JOYPAD_START, nCnt))
			&& CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
		{
			// ����SE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_YES);

			CFade::SetFade(CApplication::MODE_ENTRY, 0.05f);
		}
	}
}

//==============================================================================================
// �`�揈��
//==============================================================================================
void CTitle::Draw()
{
}

//==============================================================================================
// ��������
//==============================================================================================
CTitle *CTitle::Create()
{
	CTitle *pTitle = nullptr;

	pTitle = new CTitle;

	if (pTitle != nullptr)
	{
		pTitle->Init();
	}

	return pTitle;
}