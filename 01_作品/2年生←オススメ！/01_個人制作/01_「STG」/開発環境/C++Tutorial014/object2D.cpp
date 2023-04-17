//=============================================================================
//
// オブジェクト2D
// Author : tanimoto kosuke
//
//=============================================================================
#include "object2D.h"
#include "renderer.h"
#include "application.h"

//=====================================
//デフォルトコンストラクタ
//=====================================
CObject2D::CObject2D(int priority) : CObject(priority), m_pVtxBuff(nullptr)
{
	
}

//=====================================
//デストラクタ
//=====================================
CObject2D::~CObject2D()
{

}

//=====================================
//初期化処理
//=====================================
void CObject2D::Init()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos.x = m_pos.x - sinf(m_fAngle + m_rot.x) * m_fLength;
	pVtx[0].pos.y = m_pos.y - cosf(m_fAngle + m_rot.y) * m_fLength;
	pVtx[0].pos.z = m_pos.z;

	pVtx[1].pos.x = m_pos.x + sinf(m_fAngle - m_rot.x) * m_fLength;
	pVtx[1].pos.y = m_pos.y - cosf(m_fAngle - m_rot.y) * m_fLength;
	pVtx[1].pos.z = m_pos.z;

	pVtx[2].pos.x = m_pos.x - sinf(m_fAngle - m_rot.x) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fAngle - m_rot.y) * m_fLength;
	pVtx[2].pos.z = m_pos.z;

	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle + m_rot.x) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fAngle + m_rot.y) * m_fLength;
	pVtx[3].pos.z = m_pos.z;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//m_bUse = true;
}

//=====================================
//終了処理
//=====================================
void CObject2D::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//オブジェクトの破棄
	Release();
}

//=====================================
//更新処理
//=====================================
void CObject2D::Update()
{

}

//=====================================
//描画処理
//=====================================
void CObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================
//UV設定処理
//=====================================
void CObject2D::SetUV()
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos.x = m_pos.x - sinf(m_fAngle + m_rot.x) * m_fLength;
	pVtx[0].pos.y = m_pos.y - cosf(m_fAngle + m_rot.y) * m_fLength;
	pVtx[0].pos.z = m_pos.z;

	pVtx[1].pos.x = m_pos.x + sinf(m_fAngle - m_rot.x) * m_fLength;
	pVtx[1].pos.y = m_pos.y - cosf(m_fAngle - m_rot.y) * m_fLength;
	pVtx[1].pos.z = m_pos.z;

	pVtx[2].pos.x = m_pos.x - sinf(m_fAngle - m_rot.x) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf(m_fAngle - m_rot.y) * m_fLength;
	pVtx[2].pos.z = m_pos.z;

	pVtx[3].pos.x = m_pos.x + sinf(m_fAngle + m_rot.x) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf(m_fAngle + m_rot.y) * m_fLength;
	pVtx[3].pos.z = m_pos.z;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=====================================
//頂点テクスチャ設定処理(アニメーション)
//=====================================
void CObject2D::SetAnim(int nAnimX, int nAnimY, int nControl)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((1.0f / nAnimX) * m_nAnimCounterX, (1.0f / nAnimY) * m_nAnimCounterY);
	pVtx[1].tex = D3DXVECTOR2((1.0f / nAnimX) * (m_nAnimCounterX + 1.0f), (1.0f / nAnimY) * m_nAnimCounterY);
	pVtx[2].tex = D3DXVECTOR2((1.0f / nAnimX) * m_nAnimCounterX, (1.0f / nAnimY) * (m_nAnimCounterY + 1.0f));
	pVtx[3].tex = D3DXVECTOR2((1.0f / nAnimX) * (m_nAnimCounterX + 1.0f), (1.0f / nAnimY) * (m_nAnimCounterY + 1.0f));

	m_nAnimControl++;

	if (nControl <= m_nAnimControl)
	{
		m_nAnimCounterX++;
		m_nAnimControl = 0;
		if (nAnimX <= m_nAnimCounterX)
		{
			m_nAnimCounterX = 0;
			m_nAnimCounterY++;
			if (nAnimY <= m_nAnimCounterY)
			{
				m_nAnimCounterY = 0;
			}
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=====================================
//頂点テクスチャ設定処理(頂点テクスチャ)
//=====================================
void CObject2D::SetTex(int tex, int div)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((tex + 0.0f) / div, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((tex + 1.0f) / div, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((tex + 0.0f) / div, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((tex + 1.0f) / div, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=====================================
//色設定処理(頂点別)
//=====================================
void CObject2D::SetColor(D3DXCOLOR col0, D3DXCOLOR col1, D3DXCOLOR col2, D3DXCOLOR col3)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col0;
	pVtx[1].col = col1;
	pVtx[2].col = col2;
	pVtx[3].col = col3;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=====================================
//色設定処理(全体)
//=====================================
void CObject2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=====================================
//オブジェクト設定処理
//=====================================
void CObject2D::SetObject2D(Object_Type type, float fWidth, float fHeight)
{
	CObject::SetType(type);
	m_fWidth = fWidth;
	m_fHeight = fHeight;
	m_fAngle = atan2f(fWidth, fHeight);
	m_fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight)) / 2;
}

//=====================================
//オブジェクト2D生成処理
//=====================================
CObject2D *CObject2D::Create(D3DXVECTOR3 pos)
{
	CObject2D *pObject2D = new CObject2D;	//オブジェクト2Dポインタ

	if (pObject2D != nullptr)
	{
		pObject2D->Init();
		pObject2D->SetPos(pos);
	}

	return pObject2D;
}