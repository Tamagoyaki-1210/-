//=============================================================================
//
// trampoline.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "trampoline.h"
#include "CylinderHitbox.h"
#include "application.h"
#include "camera.h"

//コンストラクタ
CTrampoline::CTrampoline()
{
	//メンバー変数をクリアする
	m_pHitbox = nullptr;				//ヒットボックスを生成する
}

//デストラクタ
CTrampoline::~CTrampoline()
{

}

//初期化処理
HRESULT CTrampoline::Init(void)
{
	//初期化処理
	if (FAILED(CModel::Init()))
	{//初期化処理
		return -1;
	}

	m_pHitbox = nullptr;			//ヒットボックス

	return S_OK;
}

//終了処理
void CTrampoline::Uninit(void)
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
void CTrampoline::Update(void)
{
	//基本クラスの更新処理
	CModel::Update();

	if (GetPos().z < CApplication::GetCamera()->GetPos().z - 100.0f)
	{//見えなくなったら、消す
		Release();
	}
}

//描画処理
void CTrampoline::Draw(void)
{
	if (GetPos().z < CApplication::GetCamera()->GetPos().z + 650.0f)
	{//遠すぎたら、描画しない
		CModel::Draw();				//基本クラスの描画処理
	}
}




//=============================================================================
//								静的関数
//=============================================================================



//生成処理
CTrampoline* CTrampoline::Create(const D3DXVECTOR3 pos)
{
	CTrampoline* pModel = new CTrampoline;			//インスタンスを生成する

	if (FAILED(pModel->Init()))
	{//初期化処理
		return nullptr;
	}

	pModel->SetModel(CModel::MODEL_TRAMPOLINE);				//モデルの設定
	pModel->SetPos(pos);									//位置の設定
	pModel->SetShadowDraw(false);							//影を描画しないように設定する

	//ヒットボックスの生成
	pModel->m_pHitbox = CCylinderHitbox::Create(pos, Vec3Null, D3DXVECTOR3(20.0f, 3.0f, 20.0f), CHitbox::TYPE_OBSTACLE, 0, nullptr, CHitbox::EFFECT_JUMP);

	return pModel;											//生成したインスタンスを返す
}