//=============================================================================
//
// silhouette.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "silhouette.h"
#include "application.h"
#include "rendering.h"


//コンストラクタ
CSilhouette::CSilhouette() : CObject_2D(4)
{

}

//デストラクタ
CSilhouette::~CSilhouette()
{

}

//初期化処理
HRESULT CSilhouette::Init(void)
{
	if (FAILED(CObject_2D::Init()))
	{//基本クラスの初期化処理
		return -1;
	}

	SetPos(D3DXVECTOR3((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f, 0.0f));			//位置の初期化
	SetSize(D3DXVECTOR2((float)SCREEN_WIDTH * 0.5f, (float)SCREEN_HEIGHT * 0.5f));				//サイズの初期化
	SetTexture(CObject::TEXTURE_NULL);															//テクスチャの初期化
	SetTextureParameter(1, 1, 1, INT_MAX);														//テクスチャパラメータの初期化
	SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));												//色を真っ黒にする
																								
	return S_OK;
}

//終了処理
void CSilhouette::Uninit(void)
{
	//基本クラスの終了処理
	CObject_2D::Uninit();
}

//描画処理
void CSilhouette::Draw(void)
{
	//デバイスの取得処理
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//ステンシルバッファを有効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	//ステンシルバッファと比較する参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x02);

	//ステンシルバッファの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	//ステンシルテストの比較方法の設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	//ステンシルテストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	CObject_2D::Draw();

	//ステンシルバッファを無効にする
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}



//=============================================================================
//								静的関数
//=============================================================================



//生成処理
CSilhouette* CSilhouette::Create(void)
{
	CSilhouette* pObj = new CSilhouette;	//インスタンスを生成する

	if (FAILED(pObj->Init()))
	{//初期化処理
		return nullptr;
	}

	return pObj;							//生成したインスタンスを返す
}