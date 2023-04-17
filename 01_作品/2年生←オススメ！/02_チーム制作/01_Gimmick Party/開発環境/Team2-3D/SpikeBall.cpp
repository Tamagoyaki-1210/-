//=============================================================================
//
// SpikeBall.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "SpikeBall.h"
#include "CylinderHitbox.h"
#include "application.h"
#include "camera.h"


//コンストラクタ
CSpikeBall::CSpikeBall()
{
	//メンバー変数をクリアする
	m_pHitbox = nullptr;			//ヒットボックス
}

//デストラクタ
CSpikeBall::~CSpikeBall()
{

}

//初期化処理
HRESULT CSpikeBall::Init(void)
{
	//初期化処理
	if (FAILED(CModel::Init()))
	{//基本クラスの初期化処理
		return -1;
	}

	m_pHitbox = nullptr;			//ヒットボックス

	return S_OK;
}

//終了処理
void CSpikeBall::Uninit(void)
{
	//ヒットボックスの破棄
	if (m_pHitbox != nullptr)
	{
		m_pHitbox->Release();
		m_pHitbox = nullptr;
	}

	//基本クラスの終了処理
	CModel::Uninit();
}

//更新処理
void CSpikeBall::Update(void)
{
	if (m_pHitbox != nullptr)
	{//ヒットボックスがnullではなかったら
		//位置を更新する
		D3DXVECTOR3 newPos = GetPos() +  m_pHitbox->GetRelativePos();	
		m_pHitbox->SetPos(newPos);
	}

	//基本クラスの更新処理
	CModel::Update();

	if (GetPos().z < CApplication::GetCamera()->GetPos().z - 100.0f)
	{//見えなくなったら、消す
		Release();
	}
}

//描画処理
void CSpikeBall::Draw(void)
{
	if (GetPos().z < CApplication::GetCamera()->GetPos().z + 650.0f)
	{//遠すぎたら、描画しない
		CModel::Draw();
	}
}




//=============================================================================
//								静的関数
//=============================================================================




//生成処理
CSpikeBall* CSpikeBall::Create(const D3DXVECTOR3 pos)
{
	CSpikeBall* pObs = new CSpikeBall;				//インスタンスを生成する

	if (FAILED(pObs->Init()))
	{//初期化処理
		return nullptr;
	}

	pObs->SetModel(CModel::MODEL_SPIKE_BALL);			//モデルの設定

	pObs->SetPos(pos);		//位置の設定

	//ランダムで回転を設定する
	D3DXVECTOR3 rotation = D3DXVECTOR3((float)random(-100, 100), (float)random(-100, 100), (float)random(-100, 100));
	D3DXVec3Normalize(&rotation, &rotation);
	rotation.x *= 0.1f;
	rotation.y *= 0.1f;
	rotation.z *= 0.1f;
	pObs->StartRotation(rotation);

	//ヒットボックスを生成する
	pObs->m_pHitbox = CCylinderHitbox::Create(D3DXVECTOR3(pos.x, pos.y - 10.0f, pos.z), Vec3Null, D3DXVECTOR3(8.0f, 20.0f, 8.0f), CHitbox::TYPE_OBSTACLE, -30, pObs, CHitbox::EFFECT_LAUNCH);

	return pObs;									//生成したインスタンスを返す
}