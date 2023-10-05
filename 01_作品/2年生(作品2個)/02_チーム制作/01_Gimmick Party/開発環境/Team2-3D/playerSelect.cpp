//=============================================================================
//
// playerSelect.cpp
// Author : tanimoto kosuke
//
//=============================================================================
#include "playerSelect.h"
#include "Application.h"
#include "inputKeyboard.h"
#include "inputPad.h"
#include "fade.h"
#include "playerModel.h"
#include "camera.h"
#include "halfsphere.h"

CFontString* CPlayerSelect::m_pStr = nullptr;
//=====================================
// デフォルトコンストラクタ
//=====================================
CPlayerSelect::CPlayerSelect()
{

}

//=====================================
// デストラクタ
//=====================================
CPlayerSelect::~CPlayerSelect()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CPlayerSelect::Init(void)
{
	CHalfSphere* m_pSphere = CHalfSphere::Create(D3DXVECTOR3(0.0f, -8000.0f, 1000.0f), D3DXVECTOR3(30000.0f, 0.0f, 30000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), CHalfSphere::SPHERE_UP);
	
	m_pSphere->BindTexture(CObject_2D::GetTexturePointer(CObject::TEXTURE_BACKGROUND));

	CObject_2D* pObj2D = CObject_2D::Create();
	pObj2D->SetPos(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	pObj2D->SetSize(D3DXVECTOR2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 5));
	pObj2D->SetTexture(CObject::TEXTURE_CHARASET_FRAM);
	pObj2D->SetPriority(5);

	m_pStr = CFontString::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), D3DXVECTOR2(30.0f, 30.0f), "キャラクターセレクト");

	CCamera* pCamera = CApplication::GetCamera();
	pCamera->SetPos(D3DXVECTOR3(50.0f, 230.0f, -140.0f), D3DXVECTOR3(50.0f, 220.0f, 100.0f));

	pObj2D = CObject_2D::Create();
	pObj2D->SetPos(D3DXVECTOR3(675.0f, 635.0f, 0.0f));
	pObj2D->SetSize(D3DXVECTOR2(200.0f, 66.7f));
	pObj2D->SetTexture(CObject::TEXTURE_BUTTON_SELECT_UI);
	pObj2D->SetTextureParameter(2, 1, 2, 20);
	pObj2D->SetPriority(5);

	m_nPlayerCount = 0;

	pObj2D = CObject_2D::Create();
	pObj2D->SetPos(D3DXVECTOR3(250.0f, 635.0f, 0.0f));
	pObj2D->SetSize(D3DXVECTOR2(210.0f, 54.0f));
	pObj2D->SetTexture(CObject::TEXTURE_PLAYER_SEL_CTRL);
	pObj2D->SetTextureParameter(4, 1, 4, 20);
	pObj2D->SetPriority(5);

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CPlayerSelect::Uninit(void)
{
	if (m_pStr != nullptr)
	{
		m_pStr->Uninit();
		m_pStr = nullptr;
	}
}

//=====================================
// 更新処理
//=====================================
void CPlayerSelect::Update(void)
{
	if (m_pStr != nullptr)
	{
		m_pStr->Update();
	}
	Input();
}

//=====================================
// 入力処理
//=====================================
void CPlayerSelect::Input(void)
{
	CSound *pSound = CApplication::GetSound();

	if (CApplication::GetFade()->GetFade() == CFade::FADE_NONE)
	{
//#ifdef _DEBUG
		if (CInputKeyboard::GetKeyboardTrigger(DIK_BACK) || CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_BACK, 0))
		{// BACKSPACEキーを押したら
			CApplication::SetMode(CApplication::Mode_Title);
			pSound->Play(CSound::SOUND_LABEL_SE_NO);
		}
		else if (CInputKeyboard::GetKeyboardTrigger(DIK_RETURN))
		{
			CApplication::SetMode(CApplication::Mode_StageSelect);
			pSound->Play(CSound::SOUND_LABEL_SE_YES);
		}
//#endif // !DEBUG
		if (m_nPlayerCount >= 4)
		{//全員が準備おｋだったら
			CApplication::SetMode(CApplication::Mode_StageSelect);
			pSound->Play(CSound::SOUND_LABEL_SE_YES);
		}
	}
}

//=====================================
// 生成処理
//=====================================
CPlayerSelect* CPlayerSelect::Create(void)
{
	CPlayerSelect* pPlayerSelect = new CPlayerSelect;

	if (FAILED(pPlayerSelect->Init()))
	{
		return nullptr;
	}

	CPlayerModel* pModel = CPlayerModel::Create(D3DXVECTOR3(-55.0f, 186.0f, 100.0f), 0, true);
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.12f, 0.0f));
	pModel->SetStage(pPlayerSelect);
	pModel = CPlayerModel::Create(D3DXVECTOR3(15.0f, 186.0f, 100.0f), 1, true);
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.03f, 0.0f));
	pModel->SetStage(pPlayerSelect);
	pModel = CPlayerModel::Create(D3DXVECTOR3(82.5f, 186.0f, 100.0f), 2, true);
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.03f, 0.0f));
	pModel->SetStage(pPlayerSelect);
	pModel = CPlayerModel::Create(D3DXVECTOR3(155.0f, 186.0f, 100.0f), 3, true);
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.12f, 0.0f));
	pModel->SetStage(pPlayerSelect);

	return pPlayerSelect;
}

void CPlayerSelect::AddPlayerCounnt(int nCount)
{
	m_nPlayerCount += nCount;
}
