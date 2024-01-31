//===================================================
//
// 軌跡の処理
// Author: Teruto Sato
// Author: Kota Tanaka
//
//===================================================

//-----------------------
// インクルード
//-----------------------
//
#include <assert.h>
#include "trail.h"
#include "application.h"
#include "renderer.h"
#include "object3D.h"

//==========================
// コンストラクタ
//==========================
CTrail::CTrail() :
m_offsetPos(0.0f, 0.0f, 0.0f),		// オフセット座標
m_worldPos(0.0f, 0.0f, 0.0f),		// ワールド座標
m_beginCol(0.0f, 0.0f, 0.0f, 0.0f),	// 色の開始値
m_endCol(0.0f, 0.0f, 0.0f, 0.0f),	// 色の終了値
m_nNumVtx(0),						// 頂点数
m_pVtxBuff(nullptr)					// 頂点バッファへのポインタ
{
	D3DXMatrixIdentity(&m_mtxWorld);	//ワールドマトリックス
	D3DXMatrixIdentity(&m_pMtxParent);	//親のマトリックス
}

//==========================
// デストラクタ
//==========================
CTrail::~CTrail()
{
}

//==========================
// 初期化
//==========================
HRESULT CTrail::Init(D3DXVECTOR3 pos)
{
	//初期値の設定
	m_offsetPos = pos;	//位置
	m_beginCol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f);	//色の初期値
	m_endCol = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.0f);

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nMaxVtx,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//----------------------------------
	// 頂点情報の設定
	//----------------------------------
	for (int i = 0; i < nMaxVtx; i++)
	{
		// 頂点座標の設定
		pVtx[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線の設定
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[i].col = m_beginCol;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==========================
// 終了
//==========================
void CTrail::Uninit()
{
	//頂点バッファの破壊
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//==========================
// 更新
//==========================
void CTrail::Update()
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//--------------------------------------
	// 頂点座標を後ろからコピーする
	//--------------------------------------
	for (int i = m_nNumVtx - 1; i > 0; i--)
	{//頂点数分降順に回す
		if (i - 2 >= 0)
		{
			pVtx[i].pos = pVtx[i - 2].pos;
			pVtx[i].col = pVtx[i - 2].col;
		}
	}

	// 色の更新
	float rate = 1.0f / m_nNumVtx;
	for (int i = 0; i < m_nNumVtx; i++)
	{
		float progress = i * rate;
		float attenuation = 3.0f;
		D3DXCOLOR col = pVtx[i].col;

		col.a -= 0.005f;

		pVtx[i].col = D3DXCOLOR(m_beginCol.r + progress * (m_endCol.r - m_beginCol.r) * attenuation,
			m_beginCol.g + progress * (m_endCol.g - m_beginCol.g) * attenuation,
			m_beginCol.b + progress * (m_endCol.b - m_beginCol.b) * attenuation,
			col.a);
	}

	//--------------------------------------
	// 空いた0と1に軌跡の新しい座標を代入
	//--------------------------------------
	//親モデルの原点からオフセット分移動した座標を代入
	D3DXVec3TransformCoord(&m_worldPos, &m_offsetPos, &m_pMtxParent);
	pVtx[1].pos = m_worldPos;	//座標の設定
	pVtx[1].col = m_beginCol;	//色の設定

	//親モデルの原点の座標を代入
	D3DXVec3TransformCoord(&m_worldPos, &D3DXVECTOR3(0.0f, -20.0f, 0.0f), &m_pMtxParent);
	pVtx[0].pos = m_worldPos;	//座標の設定

	//色の設定
	{
		D3DXCOLOR newCol = m_beginCol;
		newCol.a = 0.1f;
		pVtx[0].col = newCol;
	}

	if (m_nNumVtx < nMaxVtx - 2)
	{//頂点の数が最大値を超えないなら
		m_nNumVtx += 2;	//頂点数を加算
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================
// 描画
//==========================
void CTrail::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスの取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// レンダリングステートの設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテスト
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 170);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//裏面から見えるようにする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,											// 描画する最初の頂点インデックス
		m_nNumVtx - 4);								// 描画するプリミティブ数

	//裏面から見えなくする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Zバッファの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// 設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CTrail* CTrail::Create(D3DXVECTOR3 offset, D3DXMATRIX mtx)
{
	CTrail* pOrbit = nullptr;

	//----------------------------------
	// ポリゴンの生成と初期化
	//----------------------------------
	pOrbit = new CTrail;	//生成

	if (pOrbit != nullptr)
	{//NULLチェック
	 //引数の代入
		pOrbit->m_pMtxParent = mtx;	//親のマトリックス
		pOrbit->Init(offset);
	}

	return pOrbit;
}

void CTrail::SetColor(D3DXCOLOR col, D3DXCOLOR destcol)
{
	VERTEX_3D*pVtx;		//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	m_beginCol = col;
	m_endCol = destcol;

	//-------------------------------
	// 色の変更
	//-------------------------------
	pVtx[1].col = col;

	D3DXCOLOR newCol = col;
	newCol.a = 0.1f;

	pVtx[0].col = newCol;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

void CTrail::SetTrail(D3DXMATRIX mtx, D3DXCOLOR col)
{
	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// マトリクスの設定
	m_pMtxParent = mtx;

	//-------------------------------
	// 色の変更
	//-------------------------------
	pVtx[1].col = col;

	D3DXCOLOR newCol = col;
	newCol.a = 0.1f;

	pVtx[0].col = newCol;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}