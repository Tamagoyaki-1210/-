//=============================================================================
//
// bouncePole.cpp
// Author : KAJITA HIROMU
//
//=============================================================================
//=============================================================================
//インクルードファイル
//=============================================================================
#include "bouncePole.h"
#include "CylinderHitbox.h"
#include "application.h"
#include "camera.h"

//コンストラクタ
CBouncePole::CBouncePole()
{
	m_pHitbox = nullptr;
}

//デストラクタ
CBouncePole::~CBouncePole()
{

}

//初期化処理
HRESULT CBouncePole::Init(void)
{
	if (FAILED(CModel::Init()))
	{
		return -1;
	}

	m_Height = 0.0f;
	m_pHitbox = nullptr;

	return S_OK;
}

//終了処理
void CBouncePole::Uninit(void)
{
	if (m_pHitbox != nullptr)
	{
		m_pHitbox->Release();
		m_pHitbox = nullptr;
	}

	CModel::Uninit();
}

//更新処理
void CBouncePole::Update(void)
{
	D3DXVECTOR3 pos = CModel::GetPos();

	if (m_pHitbox != nullptr)
	{
		D3DXVECTOR3 newPos = GetPos() + m_pHitbox->GetRelativePos();
		m_pHitbox->SetPos(newPos);
	}

	m_Height += D3DX_PI * 0.004f;

	m_move.x = sinf((D3DX_PI * 0.25f) + m_Height) * 1.5f;

	pos += m_move;
	SetPos(pos);

	if (m_pHitbox != nullptr)
	{
		m_pHitbox->SetPos(pos);
		//m_pHitbox->Update();
	}

	CModel::Update();

	if (GetPos().z < CApplication::GetCamera()->GetPos().z - 100.0f)
	{
		Release();
	}
}

//描画処理
void CBouncePole::Draw(void)
{
	if (GetPos().z < CApplication::GetCamera()->GetPos().z + 650.0f)
	{
		CModel::Draw();
	}
}






//生成処理
CBouncePole* CBouncePole::Create(const D3DXVECTOR3 pos)
{
	CBouncePole* pObs = new CBouncePole;

	if (FAILED(pObs->Init()))
	{
		return nullptr;
	}

	pObs->SetModel(CModel::MODEL_BOUNCE_POLE);

	//D3DXVECTOR3 rotation = D3DXVECTOR3((float)random(-100, 100), (float)random(-100, 100), (float)random(-100, 100));
	//D3DXVec3Normalize(&rotation, &rotation);
	//rotation.x *= 0.1f;
	//rotation.y *= 0.1f;
	//rotation.z *= 0.1f;
	//pObs->StartRotation(rotation);
	pObs->SetPos(pos);

	pObs->m_pHitbox = CCylinderHitbox::Create(D3DXVECTOR3(pos.x, pos.y - 10.0f, pos.z), Vec3Null, D3DXVECTOR3(8.0f, 20.0f, 8.0f), CHitbox::TYPE_OBSTACLE, 0, pObs, CHitbox::EFFECT_BOUNCE);

	return pObs;
}