//==============================================
//
// ハーフスフィアー.cpp (halfsphere.cpp)
// Author : Saito Shian
//
//==============================================

//==============================================
// インクルード
//==============================================
#include "application.h"
#include "halfsphere.h"

//==============================================
// コンストラクタ
//==============================================
CHalfSphere::CHalfSphere() : CObject(PRIORITY_BACK_GROUND)
{
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_bAnimation = false;
	m_bRot = false;
	m_AnimationMove = D3DXVECTOR2(0.0f, 0.0f);
	m_Animation = D3DXVECTOR2(0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRot = 0.0f;
}

//==============================================
// デストラクタ
//==============================================
CHalfSphere::~CHalfSphere()
{
}

//==============================================
// ハーフスフィアの初期化処理
//==============================================
HRESULT CHalfSphere::Init(void)
{
	// メンバ変数の初期化
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

	// インデックスの開放
	m_pIdxBuff->Unlock();

	return S_OK;
}

//==============================================
// ハーフスフィアの終了処理
//==============================================
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
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	Release();
}

//==============================================
// ハーフスフィアの更新処理
//==============================================
void CHalfSphere::Update(void)
{
	if (!m_bAnimation && !m_bRot)
	{
		return;
	}

	//頂点座標へのポインタ
	VERTEX_3D * pVtx = nullptr;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < HALFSPHEAR_Z_BLOCK + 1; nCntZ++)
	{
		for (int nCntX = 0; nCntX < HALFSPHEAR_X_BLOCK + 1; nCntX++)
		{


			float fRot = m_fRot + (D3DX_PI * 2) / HALFSPHEAR_X_BLOCK * nCntX;
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


			switch (m_tex)
			{
			case CHalfSphere::SPHERE_UP:
				//テクスチャ座標の設定
				pVtx[nCntX + (nCntZ *(HALFSPHEAR_X_BLOCK + 1))].tex = D3DXVECTOR2((1.0f / HALFSPHEAR_X_BLOCK) * -nCntX + m_Animation.x,
					(1.0f / HALFSPHEAR_Z_BLOCK) * -nCntZ + m_Animation.y);
				break;
			case CHalfSphere::SPHERE_DOWN:
				//テクスチャ座標の設定
				pVtx[nCntX + (nCntZ *(HALFSPHEAR_X_BLOCK + 1))].tex = D3DXVECTOR2((1.0f / HALFSPHEAR_X_BLOCK) * nCntX + m_Animation.x,
					(1.0f / HALFSPHEAR_Z_BLOCK) * nCntZ + m_Animation.y);
				break;
			case CHalfSphere::SPHERE_MAX:
				break;
			default:
				break;
			}

		}
	}

	// インデックスの開放
	m_pVtxBuff->Unlock();

	m_fRot += m_fRotMove;
	m_Animation += m_AnimationMove;

}

//==============================================
// ハーフスフィアの描画処理
//==============================================
void CHalfSphere::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ライトを無効化
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

	//元に戻す
	pDevice->SetTexture(0, NULL);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==============================================
// 位置の設定
//==============================================
void CHalfSphere::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==============================================
// サイズの設定
//==============================================
void CHalfSphere::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;
}

//==============================================
// 向きの設定
//==============================================
void CHalfSphere::SetRot(const D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//==============================================
// 
//==============================================
void CHalfSphere::SetTex(const SPHERE_TEX tex)
{
	m_tex = tex;
}

//==============================================
// サイズの取得
//==============================================
const D3DXVECTOR2 CHalfSphere::GetSize(void)
{
	return (D3DXVECTOR2)m_size;
}

//==============================================
// 位置の取得
//==============================================
const D3DXVECTOR3 CHalfSphere::GetPos(void)
{
	return m_pos;
}

//==============================================
// 向きの取得
//==============================================
const D3DXVECTOR3 CHalfSphere::GetRot(void)
{
	return m_rot;
}

//==============================================
// 生成
//==============================================
CHalfSphere* CHalfSphere::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, SPHERE_TEX tex)
{
	CHalfSphere* pSphere = new CHalfSphere;

	if (pSphere != nullptr)
	{
		// 設定
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

//==============================================
// テクスチャの反映
//==============================================
void CHalfSphere::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//==============================================
// テクスチャの読み込み
//==============================================
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