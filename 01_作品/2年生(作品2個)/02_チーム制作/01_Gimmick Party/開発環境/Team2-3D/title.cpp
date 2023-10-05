//=============================================================================
//
// title.cpp
// Author : tanimoto kosuke
//
//=============================================================================
#include "title.h"
#include "Application.h"
#include "meshfield.h"
#include "camera.h"
#include "playerModel.h"
#include "AnimateUI.h"
#include "halfsphere.h"
#include "coin.h"
#include "SpikeBall.h"
#include "bouncePole.h"
#include "lavaFloor.h"

//=====================================
// デフォルトコンストラクタ
//=====================================
CTitle::CTitle()
{
	//メンバー変数をクリアする

	//モデルへのポインタ
	for (int nCnt = 0; nCnt < MAX_TITLE_MODEL; nCnt++)
	{
		m_pModel[nCnt] = nullptr;
	}

	//UIへのポインタ
	m_pTitle = nullptr;
}

//=====================================
// デストラクタ
//=====================================
CTitle::~CTitle()
{

}

//=====================================
// 初期化処理
//=====================================
HRESULT CTitle::Init(void)
{
	//初期化処理

	//メッシュフィールドの生成
	CMeshfield* pField = CMeshfield::Create(D3DXVECTOR3(-200.0f, 0.0f, 2500.0f), Vec3Null, D3DXVECTOR2(100.0f, 1000.0f), 30, 5);
	pField->SetTexture(CObject::TEXTURE_BLOCK);						//テクスチャの設定
	pField->SetTextureTiling(D3DXVECTOR2(1.0f, 10.0f));				//テクスチャの大きさの設定

	if (CApplication::GetCamera() != nullptr)
	{//カメラがnullではなかったら、視点と注視点の位置を設定する
		CApplication::GetCamera()->SetPos(D3DXVECTOR3(0.0f, 200.0f, -600.0f), D3DXVECTOR3(0.0f, -0.0f, 0.0f));
	}

	//背景の生成
	CHalfSphere* pSphere = CHalfSphere::Create(D3DXVECTOR3(0.0f, -8000.0f, 1000.0f), D3DXVECTOR3(30000.0f, 0.0f, 30000.0f), D3DXVECTOR3(D3DX_PI * -0.15f, D3DX_PI, 0.0f), CHalfSphere::SPHERE_UP);

	LPDIRECT3DTEXTURE9 pTex = CObject_2D::GetTexturePointer(CObject::TEXTURE_SKY);		//テクスチャの取得
	pSphere->BindTexture(pTex);															//テクスチャの設定

	//プレイヤーモデルの設定
	m_pModel[0] = CPlayerModel::Create(D3DXVECTOR3(-125.0f, 50.0f, -280.0f), 0, false);
	m_pModel[1] = CPlayerModel::Create(D3DXVECTOR3(-37.5f, 50.0f, -280.0f), 1, false);
	m_pModel[2] = CPlayerModel::Create(D3DXVECTOR3(37.5f, 50.0f, -280.0f), 2, false);
	m_pModel[3] = CPlayerModel::Create(D3DXVECTOR3(125.0f, 50.0f, -280.0f), 3, false);

	//UIの生成
	CAnimateUI::UIAnimation animInfo;
	animInfo.deltaSize = D3DXVECTOR2(1.0f, 0.5f);
	animInfo.nChangeTime = 60;

	//タイトルUIの生成
	m_pTitle = CAnimateUI::Create(CObject::TEXTURE_TITLE_UI, D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.25f, 0.0f), D3DXVECTOR2(320.0f, 160.0f),
		ColorWhite, animInfo);

	//モデルの生成
	CModel* pModel = CModel::Create(CModel::MODEL_WINDMILL, D3DXVECTOR3(0.0f, -100.0f, 2000.0f));
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));			//向きの設定
	pModel->SetShadowDraw(false);										//影を描画しないように設定する

	pModel = CModel::Create(CModel::MODEL_BALLOON, D3DXVECTOR3(700.0f, -250.0f, 600.0f));
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.25f, 0.0f));			//向きの設定
	pModel->SetShadowDraw(false);										//影を描画しないように設定する

	pModel = CModel::Create(CModel::MODEL_BALLOON, D3DXVECTOR3(-500.0f, -50.0f, 800.0f));
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.15f, 0.0f));			//向きの設定
	pModel->SetModelColor(1, D3DXCOLOR(0.75f, 0.2f, 0.75f, 1.0f));		//モデルの色の設定
	pModel->SetShadowDraw(false);										//影を描画しないように設定する

	pModel = CModel::Create(CModel::MODEL_BALLOON, D3DXVECTOR3(-300.0f, -90.0f, -250.0f));
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.15f, 0.0f));			//向きの設定
	pModel->SetModelColor(1, D3DXCOLOR(0.10f, 1.0f, 0.8f, 1.0f));		//モデルの色の設定
	pModel->SetShadowDraw(false);										//影を描画しないように設定する

	pModel = CModel::Create(CModel::MODEL_BALLOON, D3DXVECTOR3(1300.0f, 100.0f, 1300.0f));
	pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.85f, 0.0f));			//向きの設定
	pModel->SetModelColor(1, D3DXCOLOR(0.10f, 1.0f, 0.2f, 1.0f));		//モデルの色の設定
	pModel->SetShadowDraw(false);										//影を描画しないように設定する

	//コインの生成
	CCoin::Create(D3DXVECTOR3(-125.0f, 75.0f, -200.0f), CCoin::COIN_0);
	CCoin::Create(D3DXVECTOR3(-125.0f, 75.0f, -225.0f), CCoin::COIN_0);
	CCoin::Create(D3DXVECTOR3(-100.0f, 75.0f, -225.0f), CCoin::COIN_0);
	CCoin::Create(D3DXVECTOR3(-100.0f, 75.0f, -200.0f), CCoin::COIN_0);

	CCoin::Create(D3DXVECTOR3(- 50.0f, 75.0f, -150.0f), CCoin::COIN_1);
	CCoin::Create(D3DXVECTOR3(- 50.0f, 75.0f, -175.0f), CCoin::COIN_1);
	CCoin::Create(D3DXVECTOR3(- 25.0f, 75.0f, -175.0f), CCoin::COIN_1);
	CCoin::Create(D3DXVECTOR3(- 25.0f, 75.0f, -150.0f), CCoin::COIN_1);

	CCoin::Create(D3DXVECTOR3(50.0f, 75.0f, -150.0f), CCoin::COIN_2);
	CCoin::Create(D3DXVECTOR3(50.0f, 75.0f, -175.0f), CCoin::COIN_2);
	CCoin::Create(D3DXVECTOR3(25.0f, 75.0f, -175.0f), CCoin::COIN_2);
	CCoin::Create(D3DXVECTOR3(25.0f, 75.0f, -150.0f), CCoin::COIN_2);

	CCoin::Create(D3DXVECTOR3(125.0f, 75.0f, -200.0f), CCoin::COIN_3);
	CCoin::Create(D3DXVECTOR3(125.0f, 75.0f, -225.0f), CCoin::COIN_3);
	CCoin::Create(D3DXVECTOR3(100.0f, 75.0f, -225.0f), CCoin::COIN_3);
	CCoin::Create(D3DXVECTOR3(100.0f, 75.0f, -200.0f), CCoin::COIN_3);

	//障害物の生成
	CSpikeBall::Create(D3DXVECTOR3(-75.0f, 75.0f, -400.0f));
	CSpikeBall::Create(D3DXVECTOR3( 75.0f, 75.0f, -400.0f));

	CBouncePole::Create(D3DXVECTOR3(-90.0f, 75.0f, 300.0f));

	pField = CMeshfield::Create(D3DXVECTOR3(-175.0f, 1.0f, -401.0f), Vec3Null, D3DXVECTOR2(400.0f, 100.0f), 2, 2);
	pField->SetTexture(CObject::TEXTURE_LAVA);
	pField->SetTextureTiling(D3DXVECTOR2(5.0f, 0.5f));
	pField->SetTextureAnim(0.001f, 0.0f);	

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CTitle::Uninit(void)
{

}

//=====================================
// 更新処理
//=====================================
void CTitle::Update(void)
{

}

//=====================================
// 生成処理
//=====================================
CTitle* CTitle::Create(void)
{
	CTitle* pTitle = new CTitle;				//インスタンスを生成する

	if (FAILED(pTitle->Init()))
	{//初期化処理
		return nullptr;
	}

	return pTitle;								//生成したインスタンスを返す
}