//=============================================================================
//
// line.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "line.h"
#include "application.h"
#include "rendering.h"

//コンストラクタ
CLine::CLine() : CObject(5)
{
	//メンバー変数をクリアする
	m_pos = Vec3Null;				//位置
	m_rot = Vec3Null;				//向き
	m_col = ColorWhite;				//カーラー
	m_start = Vec3Null;				//開始点
	m_finish = Vec3Null;			//終了点
	m_pVtxBuff = nullptr;			//頂点バッファ
}

//デストラクタ
CLine::~CLine()
{

}

//初期化処理
HRESULT CLine::Init(void)
{
	//メンバー変数の初期化
	m_pos = Vec3Null;			//位置
	m_rot = Vec3Null;			//向き
	m_col = ColorWhite;			//カーラー
	m_start = Vec3Null;			//開始点
	m_finish = Vec3Null;		//終了点
	m_pVtxBuff = nullptr;		//頂点バッファ

	return S_OK;
}

//終了処理
void CLine::Uninit(void)
{
	//頂点バッファの破棄処理
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//更新処理
void CLine::Update(void)
{

}

//描画処理
void CLine::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX	mtxRot, mtxTrans;					//計算用マトリックス

	//ライトを無効化にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);	

	//ワルドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワルドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_LINE));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_LINE);

	//四角形を描画する
	pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//位置の設定処理
void CLine::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//サイズの取得処理
const D3DXVECTOR2 CLine::GetSize(void)
{
	return Vec2Null;
}

//位置の取得処理
const D3DXVECTOR3 CLine::GetPos(void)
{
	return m_pos;
}




//=============================================================================
//
//								静的関数
//
//=============================================================================




//生成処理
CLine* CLine::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 start, const D3DXVECTOR3 end, D3DXCOLOR col)
{
	CLine* pLine = new CLine;			//線の生成

	if (FAILED(pLine->Init()))
	{//初期化処理
		return nullptr;
	}

	pLine->m_pos = pos;				//位置の設定
	pLine->m_rot = rot;				//向きの設定
	pLine->m_start = start;			//開始点の設定
	pLine->m_finish = end;			//終了点の設定
	pLine->m_col = col;				//カーラーの設定

	pLine->SetLine();				//頂点の設定処理

	return pLine;					//生成した線を返す
}

//生成処理
CLine* CLine::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 start, const D3DXVECTOR3 end)
{
	CLine* pLine = new CLine();		//線の生成

	if (FAILED(pLine->Init()))
	{//初期化処理
		return nullptr;
	}

	pLine->m_pos = pos;										//位置の設定
	pLine->m_rot = rot;										//向きの設定
	pLine->m_start = start;									//開始点の設定
	pLine->m_finish = end;									//終了点の設定
	pLine->m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//カーラーの設定

	pLine->SetLine();				//頂点の設定処理

	return pLine;					//生成した線を返す
}


//========================================================================================================================
//
//			プライベート関数
//
//========================================================================================================================


//頂点の設定処理
void CLine::SetLine(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_LINE) * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_LINE,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点情報へのポインタ
	VERTEX_LINE*pVtx = NULL;

	//頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_finish;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;

	//頂点バッファのアンロック
	m_pVtxBuff->Unlock();
}