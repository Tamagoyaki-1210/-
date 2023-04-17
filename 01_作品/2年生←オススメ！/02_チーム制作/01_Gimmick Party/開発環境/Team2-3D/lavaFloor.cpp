//=============================================================================
//
// lavaFloor.h
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "lavaFloor.h"
#include "BoxHitbox.h"
#include "application.h"
#include "camera.h"


//コンストラクタ
CLavaFloor::CLavaFloor()
{
	m_pHitbox = nullptr;
}

//デストラクタ
CLavaFloor::~CLavaFloor()
{

}

//初期化処理
HRESULT CLavaFloor::Init(void)
{
	//初期化処理
	if (FAILED(CModel::Init()))
	{
		return 1;
	}

	m_pHitbox = nullptr;			//ヒットボックス

	return S_OK;
}

//終了処理
void CLavaFloor::Uninit(void)
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
void CLavaFloor::Update(void)
{
	//基本クラスの更新処理
	CModel::Update();

	//見えなくなったら、消す
	if (GetPos().z < CApplication::GetCamera()->GetPos().z - 100.0f)
	{
		Release();
	}
}

//描画処理
void CLavaFloor::Draw(void)
{
	//遠すぎたら、描画しない
	if (GetPos().z < CApplication::GetCamera()->GetPos().z + 650.0f)
	{
		CModel::Draw();
	}
}



//=============================================================================
//
//								静的関数
//
//=============================================================================



//生成処理
CLavaFloor* CLavaFloor::Create(const D3DXVECTOR3 pos)
{
	CLavaFloor* pObj = new CLavaFloor;				//オブジェクトを生成する

	if (FAILED(pObj->Init()))
	{//初期化処理
		return nullptr;
	}

	pObj->SetPos(pos);								//位置の設定
	pObj->SetModel(CModel::MODEL_LAVA_FLOOR);		//モデル情報の取得

	//ヒットボックスの生成
	pObj->m_pHitbox = CBoxHitbox::Create(pos, Vec3Null, D3DXVECTOR3(25.0f, 5.0f, 25.0f), CHitbox::TYPE_OBSTACLE, pObj, -30, CHitbox::EFFECT_LAUNCH);

	return pObj;				//生成したインスタンスを返す
}