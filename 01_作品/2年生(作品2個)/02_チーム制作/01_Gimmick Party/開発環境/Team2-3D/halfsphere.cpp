//-------------------------------------
//
//ハーフスフィアーの処理[halfsphere.cpp]
//Author:梶田大夢
//
//-------------------------------------
#include "application.h"
#include "halfsphere.h"
#include "rendering.h"

CHalfSphere::CHalfSphere() : CObject::CObject(3)
{
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
}

//コンストラクタ
CHalfSphere::CHalfSphere(const int nPriority) : CObject::CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
}

//デストラクタ
CHalfSphere::~CHalfSphere()
{
}

//メッシュフィールドの初期化処
HRESULT CHalfSphere::Init(void)
{
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * HALFSPHEAR_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * HALFSPHEAR_INDEXNUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	

	//頂点座標へのポインタ
	VERTEX_3D * pVtx = nullptr;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	

	for (int nCntZ = 0; nCntZ < HALFSPHEAR_Z_BLOCK + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < HALFSPHEAR_X_BLOCK + 1; nCntX++)
		{
			float fRot = (D3DX_PI * 2) / HALFSPHEAR_X_BLOCK * nCntX;
			float fHalfRot = (D3DX_PI / 2) / HALFSPHEAR_Z_BLOCK * nCntZ;

			// 半径と、高さの計算
			float Radius = cosf(fHalfRot) * m_size.x;
			float Height = sinf(fHalfRot) * m_size.z;

			//頂点座標の設定
			pVtx[nCntX + (nCntZ * (HALFSPHEAR_X_BLOCK + 1))].pos.x = sinf(-fRot) * Radius;
			pVtx[nCntX + (nCntZ * (HALFSPHEAR_X_BLOCK + 1))].pos.z = cosf(-fRot) * Radius;
			pVtx[nCntX + (nCntZ * (HALFSPHEAR_X_BLOCK + 1))].pos.y = Height/* + 15.0f*/;

			//各頂点の法線の設定(ベクトルの大きさは1にする必要がある)
			pVtx[nCntX + (nCntZ *(HALFSPHEAR_X_BLOCK + 1))].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntX + (nCntZ * (HALFSPHEAR_X_BLOCK + 1))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


			switch (m_tex)
			{
			case CHalfSphere::SPHERE_UP:
				//テクスチャ座標の設定
				pVtx[nCntX + (nCntZ *(HALFSPHEAR_X_BLOCK + 1))].tex = D3DXVECTOR2((1.0f / HALFSPHEAR_X_BLOCK) * -nCntX, (1.0f / HALFSPHEAR_Z_BLOCK) * -nCntZ);
				break;
			case CHalfSphere::SPHERE_DOWN:
				//テクスチャ座標の設定
				pVtx[nCntX + (nCntZ *(HALFSPHEAR_X_BLOCK + 1))].tex = D3DXVECTOR2((1.0f / HALFSPHEAR_X_BLOCK) * nCntX, (1.0f / HALFSPHEAR_Z_BLOCK) * nCntZ);
				break;
			case CHalfSphere::SPHERE_MAX:
				break;
			default:
				break;
			}
		
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//インデックスバッファをロック
	WORD * pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの設定
	for (int nCntZ = 0; nCntZ < HALFSPHEAR_Z_BLOCK; nCntZ++)
	{
		for (int nCntX = 0; nCntX < HALFSPHEAR_X_BLOCK + 1; nCntX++)
		{
			pIdx[0] = (WORD)((HALFSPHEAR_X_BLOCK + 1) + nCntX + nCntZ * (HALFSPHEAR_X_BLOCK + 1));
			pIdx[1] = (WORD)((HALFSPHEAR_X_BLOCK + 1) + nCntX + nCntZ * (HALFSPHEAR_X_BLOCK + 1) - (HALFSPHEAR_X_BLOCK + 1));

			pIdx += 2;
		}

		//縮退ポリゴン
		if (nCntZ < HALFSPHEAR_Z_BLOCK - 1)
		{
			pIdx[0] = (WORD)((HALFSPHEAR_X_BLOCK + 1)* nCntZ + HALFSPHEAR_X_BLOCK);
			pIdx[1] = (WORD)((HALFSPHEAR_X_BLOCK + 1) * (nCntZ + 2));

			pIdx += 2;
		}
	}
	m_pIdxBuff->Unlock();

	return S_OK;
}

void CHalfSphere::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}
}

void CHalfSphere::Update(void)
{
}

void CHalfSphere::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//メッシュフィールドの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, HALFSPHEAR_VERTEX_NUM, 0, HALFSPHEAR_PRIMITIVE_NUM);

	pDevice->SetTexture(0, NULL);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CHalfSphere::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

void CHalfSphere::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;
}

void CHalfSphere::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

void CHalfSphere::SetTex(const SPHERE_TEX tex)
{
	m_tex = tex;
}

const D3DXVECTOR2 CHalfSphere::GetSize(void)
{
	return (D3DXVECTOR2)m_size;
}

const D3DXVECTOR3 CHalfSphere::GetPos(void)
{
	return m_pos;
}

const D3DXVECTOR3 CHalfSphere::GetRot(void)
{
	return m_rot;
}

//生成
CHalfSphere * CHalfSphere::Create(D3DXVECTOR3 pos,D3DXVECTOR3 size, D3DXVECTOR3 rot, SPHERE_TEX tex)
{
	CHalfSphere* pSphere = new CHalfSphere(3);

	if (pSphere != nullptr)
	{
		pSphere->SetPos(pos);
		pSphere->SetSize(size);
		pSphere->SetRot(rot);
		pSphere->SetTex(tex);

		if (FAILED(pSphere->Init()))
		{
			return nullptr;
		}
	}

	return pSphere;
}

void CHalfSphere::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

void CHalfSphere::LoadTexture(const char * aFileName)
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		aFileName,
		&m_pTexture);
}