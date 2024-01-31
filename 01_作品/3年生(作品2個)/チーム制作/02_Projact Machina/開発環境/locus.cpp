//==============================================================================================
//
// 軌跡　　　locus.cpp
// tutida ryousei
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"locus.h"
#include"renderer.h"
#include"application.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CLocus::CLocus(const CObject::PRIORITY priority) : CObject(priority)
{
	m_pVtxBuff = nullptr;					// 頂点バッファ
	m_texture = CTexture::TEXTURE_NONE;		// テクスチャ
	m_bBillboard = false;					// ビルボードかどうか
	m_nAnchorPoint = 0;
	m_pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CLocus::~CLocus()
{
}

//==============================================================================================
// ポリゴンの初期化
//==============================================================================================
HRESULT CLocus::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファを取得
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((m_nAnchorPoint+1) * 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	m_texture = CTexture::TEXTURE_NONE;
	m_col = { 1.0f,1.0f,1.0f,1.0f };

	// 頂点バッファ
	VERTEX_3D *pVtx;

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 ParentPos = m_pos;
	D3DXVECTOR3 PrevPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pAnchorPoints[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点座標(ローカル座標)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	for (int i = 1; i <= m_nAnchorPoint; i++)
	{
		m_pAnchorPoints[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		m_pAnchorPoints[i].x += (500 - rand() % 10000) / 10.0f;
		m_pAnchorPoints[i].y += (500 + rand() % 15000) / 10.0f;
		m_pAnchorPoints[i].z += (500 - rand() % 10000) / 10.0f;
		
		D3DXVECTOR3 Pos = PrevPos + m_pAnchorPoints[i];
		PrevPos = Pos;

		//pos
		pVtx[2 * i].pos = D3DXVECTOR3(Pos.x - 50.0f, Pos.y, Pos.z);
		pVtx[2 * i + 1].pos = D3DXVECTOR3(Pos.x + 50.0f, Pos.y, Pos.z);

		//法線
		pVtx[2 * i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2 * i + 1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラー
		pVtx[2 * i].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2 * i + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャの座標
		pVtx[2 * i].tex = D3DXVECTOR2(0.0f, 1.0f / m_nAnchorPoint * i);
		pVtx[2 * i + 1].tex = D3DXVECTOR2(1.0f, 1.0f / m_nAnchorPoint * i);
	}
	
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================================
// ポリゴンの終了
//==============================================================================================
void CLocus::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pAnchorPoints != nullptr)
	{
		delete m_pAnchorPoints;
		m_pAnchorPoints = nullptr;
	}

	Release();
}

//==============================================================================================
// ポリゴンの更新
//==============================================================================================
void CLocus::Update()
{
	//UV();
}

//==============================================================================================
// ポリゴンの描画
//==============================================================================================
void CLocus::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// Zテストの設定
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	// ライトを無効化
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// カメラの逆列を設定(ビルボード)
	if (m_bBillboard)
	{
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._21 = mtxView._12;
		m_mtxWorld._22 = mtxView._22;
		m_mtxWorld._23 = mtxView._32;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;
	}

	//角度を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.x, -m_rot.y, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットに設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの情報
	CTexture *pTexture = CApplication::GetTexture();

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	//ビルボードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//元に戻す
	pDevice->SetTexture(0, NULL);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zテストの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

//=====================================
// UV設定処理
//=====================================
void CLocus::UV()
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = atan2f(m_fsize, m_fsize);								// 角度(アークタンジェント)
	float fLength = sqrtf((powf(m_fsize, 2) + powf(m_fsize, 2))) * 0.5f;	// 長さ(スクエアルート)

																			// 頂点情報を設定
	pVtx[0].pos.x = -sinf(fAngle) * fLength;
	pVtx[0].pos.y = cosf(fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(fAngle) * fLength;
	pVtx[1].pos.y = cosf(fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -sinf(fAngle) * fLength;
	pVtx[2].pos.y = -cosf(fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(fAngle) * fLength;
	pVtx[3].pos.y = -cosf(fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================================================
// テクスチャの分割
//==============================================================================================
void CLocus::SetTexPos(const float top, const  float row, const float right, const float left)
{
	// 頂点バッファ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, row);
	pVtx[3].tex = D3DXVECTOR2(right, row);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================================================
// アンカーポイントの生成と設定
//==============================================================================================
void CLocus::SetAnchorPoints(int nanchorpoints)
{
	m_nAnchorPoint = nanchorpoints;
	m_pAnchorPoints = new D3DXVECTOR3[m_nAnchorPoint * 2];
}

//==============================================================================================
// 生成処理
//==============================================================================================
CLocus* CLocus::Create(const D3DXVECTOR3 pos, const float fsize, int nAnchorPoints, const CObject::PRIORITY priority, const D3DXCOLOR col, const bool billboard)
{
	CLocus* pLocus = new CLocus(priority);

	pLocus->SetAnchorPoints(nAnchorPoints);
	if (FAILED(pLocus->Init()))
	{
		return nullptr;
	}
	pLocus->SetPos(pos);
	pLocus->SetSize(fsize);
	pLocus->SetCol(col);
	pLocus->Setbillboard(billboard);

	return pLocus;
}