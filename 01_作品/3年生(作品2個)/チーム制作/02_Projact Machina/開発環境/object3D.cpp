//==============================================================================================
//
// オブジェクト　　　object3D.cpp
// Author : tutida ryousei
// Author : tanimoto kosuke
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"object3D.h"
#include"renderer.h"
#include"application.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CObject3D::CObject3D(const CObject::PRIORITY priority) : CObject(priority)
{
	m_pVtxBuff = nullptr;					// 頂点バッファ
	m_texture = CTexture::TEXTURE_NONE;		// テクスチャ
	m_bBillboard = false;					// ビルボードかどうか
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CObject3D::~CObject3D()
{
}

//==============================================================================================
// ポリゴンの初期化
//==============================================================================================
HRESULT CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファを取得
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
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

	// 頂点座標(ローカル座標)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 法線
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================================================
// ポリゴンの終了
//==============================================================================================
void CObject3D::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//==============================================================================================
// ポリゴンの更新
//==============================================================================================
void CObject3D::Update()
{
	UV();
}

//==============================================================================================
// ポリゴンの描画
//==============================================================================================
void CObject3D::Draw()
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.x, m_rot.y, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
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
void CObject3D::UV()
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = atan2f(m_size.x, m_size.y);								// 角度(アークタンジェント)
	float fLength = sqrtf((powf(m_size.x, 2) + powf(m_size.y, 2))) * 0.5f;	// 長さ(スクエアルート)

																			// 頂点情報を設定
	pVtx[0].pos.x = -sinf(fAngle) * fLength;
	pVtx[0].pos.y = cosf(fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = sinf(fAngle) * fLength - m_Subsize.x;
	pVtx[1].pos.y = cosf(fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -sinf(fAngle) * fLength;
	pVtx[2].pos.y = -cosf(fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = sinf(fAngle) * fLength - m_Subsize.x;
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
void CObject3D::SetTexPos(const float top, const  float row, const float right, const float left)
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
// 生成処理
//==============================================================================================
CObject3D* CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const CObject::PRIORITY priority, const D3DXCOLOR col, const bool billboard)
{
	CObject3D* pObject3D = new CObject3D(priority);

	if (FAILED(pObject3D->Init()))
	{
		return nullptr;
	}
	pObject3D->SetPos(pos);
	pObject3D->SetSize(size);
	pObject3D->SetCol(col);
	pObject3D->Setbillboard(billboard);

	return pObject3D;
}