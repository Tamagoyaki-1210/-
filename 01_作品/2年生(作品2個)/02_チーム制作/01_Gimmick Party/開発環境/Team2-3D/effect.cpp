//=============================================================================
//
// effect.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "effect.h"
#include "application.h"
#include "rendering.h"


//コンストラクタ
CEffect::CEffect()
{
	//メンバー変数をクリアする
	m_rotSpeed = Vec3Null;				//回転速度
	m_move = Vec3Null;					//移動量
	m_deltaCol = ColorNull;				//色の減数
	m_fDeltaSize = 0.0f;				//サイズの減数
	m_nLife = 0;						//ライフ
}

//デストラクタ
CEffect::~CEffect()
{

}

//初期化処理
HRESULT CEffect::Init(void)
{
	//初期化処理

	if (FAILED(CBillboard::Init()))
	{//基本クラスの初期化処理
		return -1;
	}

	m_rotSpeed = Vec3Null;			//回転速度
	m_move = Vec3Null;				//移動量
	m_deltaCol = ColorNull;			//色の減数
	m_fDeltaSize = 0.0f;			//サイズの減数
	m_nLife = 0;					//ライフ

	return S_OK;
}

//終了処理
void CEffect::Uninit(void)
{
	CBillboard::Uninit();			//基本クラスの終了処理
}

//更新処理
void CEffect::Update(void)
{
	{
		m_nLife--;				//ライフをデクリメントする

		if (m_nLife <= 0)
		{//ライフが0以下になったら、破棄する
			Release();
			return;
		}

		//位置の更新
		D3DXVECTOR3 pos = GetPos();
		pos += m_move;
		SetPos(pos);

		//回転の更新
		D3DXVECTOR3 rot = GetRot();
		rot += m_rotSpeed;
		SetRot(rot);

		//色の更新
		D3DXCOLOR col = GetColor();
		col += m_deltaCol;

		if (col.r <= 0.0f && col.g <= 0.0 && col.b <= 0.0f || col.a <= 0.0f)
		{//真っ黒になったら、又は透明になったら、破棄する
			Release();
			return;
		}

		SetColor(col);				//カーラーの設定

		//サイズの更新
		D3DXVECTOR2 Size = GetSize();
		Size.x += m_fDeltaSize;
		Size.y += m_fDeltaSize;
		
		if (Size.x <= 0.0f || Size.y <= 0.0f)
		{//サイズが0以下になったら、破棄する
			Release();
			return;
		}

		SetSize(Size);				//サイズの設定
	}

	CBillboard::Update();			//基本クラスの更新処理
}

//描画処理
void CEffect::Draw(void)
{
	//デバイスの取得処理
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->CRenderer::GetDevice();

	//加算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CBillboard::Draw();				//基本クラスの描画処理

	// 設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


//=============================================================================
//
//								静的関数
//
//=============================================================================


//生成処理
CEffect* CEffect::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rotSpeed, const D3DXVECTOR3 move, const D3DXCOLOR col, const D3DXCOLOR deltaCol, const float fSize, const float fdeltaSize, const int nLife)
{	
	CEffect* pEffect = new CEffect;				//エフェクトを生成する

	if (FAILED(pEffect->Init()))
	{//初期化処理
		return nullptr;
	}

	pEffect->SetPos(pos);								//位置の設定
	pEffect->m_rotSpeed = rotSpeed;						//回転速度の設定
	pEffect->m_move = move;								//移動量の設定
	pEffect->SetColor(col);								//色の設定
	pEffect->m_deltaCol = deltaCol;						//色の減数の設定
	pEffect->SetSize(D3DXVECTOR2(fSize, fSize));		//サイズの設定
	pEffect->m_fDeltaSize = fdeltaSize;					//サイズの減数の設定
	pEffect->m_nLife = nLife;							//ライフの設定
	pEffect->SetPriority(4);							//プライオリティの設定

	return pEffect;										//生成したインスタンスを返す
}