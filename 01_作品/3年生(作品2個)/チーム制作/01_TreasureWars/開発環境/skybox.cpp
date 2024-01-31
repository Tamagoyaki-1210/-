//==============================================================================================
//
// skybox.cpp
// Author : saito shian
//
//==============================================================================================

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"main.h"
#include"skybox.h"
#include"renderer.h"
#include"application.h"
#include <assert.h>

//==============================================================================================
// コンストラクタ
//==============================================================================================
CSkyBox::CSkyBox(int nPriority) : CObject(nPriority)
{

}

//==============================================================================================
// デストラクタ
//==============================================================================================
CSkyBox::~CSkyBox()
{
}

//==============================================================================================
// ポリゴンの初期化
//==============================================================================================
HRESULT CSkyBox::Init()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	m_pos = D3DXVECTOR3(0.0f, -250.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld;
	m_nHorizontal = 0;
	m_nVertical = 0;
	m_nVtx = 0;
	m_nIdx = 0;
	m_nPolygon = 0;

	// 横・縦の初期化
	m_nHorizontal = START_SIDE;
	m_nVertical = START_VERTICAL;

	int nXLine = m_nHorizontal + 1;
	int nYLine = m_nVertical + 1;

	// 頂点数を計算
	m_nVtx = nXLine * nYLine;

	// インデックス数を計算
	m_nIdx = ((nXLine * 2) * m_nVertical) + ((m_nVertical - 1) * 2);

	// ポリゴン数を計算
	m_nPolygon = (m_nHorizontal * m_nVertical * 2) + ((m_nVertical - 1) * 4);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < nYLine; y++)
	{
		float fYRot = (((D3DX_PI * 0.25f) / m_nVertical) * y) + (D3DX_PI * 0.25f);

		float fYPos = cosf(fYRot) * MAX_SIZE;

		for (int x = 0; x < nXLine; x++)
		{
			float fRot = ((D3DX_PI * 2.0f) / m_nHorizontal) * x;

			//正規化
			if (fRot > D3DX_PI)
			{
				fRot += D3DX_PI * 2;
			}
			if (fRot < -D3DX_PI)
			{
				fRot += -D3DX_PI * 2;
			}

			float fXPos = sinf(fRot) * sinf(fYRot) * MAX_SIZE;
			float fZPos = cosf(fRot) * sinf(fYRot) * MAX_SIZE;
			D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

			// 頂点座標の設定
			pVtx[x + (y * nXLine)].pos = pos;

			D3DXVECTOR3 vec;

			// 正規化する ( 大きさ 1 のベクトルにする )
			D3DXVec3Normalize(&vec, &pos);

			// 各頂点の法線の設定
			pVtx[x + (y * nXLine)].nor = vec;

			// 頂点カラーの設定
			pVtx[x + (y * nXLine)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			float fUTex = (1.0f / m_nHorizontal) * x;
			float fVTex = (1.0f / m_nVertical) * y;

			// テクスチャ座標の設定
			pVtx[x + (y * nXLine)].tex = D3DXVECTOR2(fUTex, fVTex);
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * m_nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx = NULL;		// インデックス情報へのポインタ

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int x = 0, y = 0; y < m_nVertical; x++, y++)
	{
		for (; x < (nXLine * (y + 1)) + y; x++)
		{
			pIdx[x * 2] = (WORD)(x - y + nXLine);
			pIdx[(x * 2) + 1] = (WORD)(x - y);
			x = x;
		}

		if (y < m_nVertical - 1)
		{// これで終わりじゃないなら
			pIdx[x * 2] = (WORD)(x - (y + 1));
			pIdx[(x * 2) + 1] = (WORD)((x * 2) - (y * (m_nHorizontal + 3)));
			x = x;
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	// 円錐の頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (m_nHorizontal + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffCone,
		NULL);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuffCone->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < nXLine; i++)
	{
		float fYRot = 100.0f;
		float fRot = ((D3DX_PI * 2.0f) / m_nHorizontal) * i;

		//正規化
		if (fRot > D3DX_PI)
		{
			fRot += D3DX_PI * 2;
		}
		if (fRot < -D3DX_PI)
		{
			fRot += -D3DX_PI * 2;
		}

		float fXPos = sinf(-fRot) * sinf(fYRot) * MAX_SIZE;
		float fYPos = cosf(fYRot) * MAX_SIZE;
		float fZPos = cosf(-fRot) * sinf(fYRot) * MAX_SIZE;
		D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

		// 頂点座標の設定
		pVtx[i + 1].pos = pos;

		D3DXVECTOR3 vec;

		// 正規化する ( 大きさ 1 のベクトルにする )
		D3DXVec3Normalize(&vec, &pos);

		// 各頂点の法線の設定
		pVtx[i + 1].nor = vec;

		// 頂点カラーの設定
		pVtx[i + 1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[i + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	float fYRot = ((D3DX_PI * 0.25f) / m_nVertical);

	float fYPos = cosf(fYRot) * MAX_SIZE;

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, fYPos, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = pos;

	D3DXVECTOR3 vec;

	// 正規化する ( 大きさ 1 のベクトルにする )
	D3DXVec3Normalize(&vec, &pos);

	// 各頂点の法線の設定
	pVtx[0].nor = vec;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuffCone->Unlock();

	return S_OK;
}

//==============================================================================================
// ポリゴンの終了
//==============================================================================================
void CSkyBox::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

//==============================================================================================
// ポリゴンの更新
//==============================================================================================
void CSkyBox::Update()
{

}

//==============================================================================================
// ポリゴンの描画
//==============================================================================================
void CSkyBox::Draw()
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture *pTexture = CApplication::GetTexture();

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// ポリゴン描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点バッファ
		0,							// インデックスの最小値
		m_nVtx,						// 頂点数
		0,							// 描画する最初の頂点インデックス
		m_nPolygon);				// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffCone, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLEFAN,			// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		m_nHorizontal);				// プリミティブ(ポリゴン)数

									// ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//==============================================================================================
// 位置の設定
//==============================================================================================
void CSkyBox::Setposition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==============================================================================================
// サイズの設定
//==============================================================================================
void CSkyBox::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//==============================================================================================
//ステージごとに使用する背景を変える
//==============================================================================================
void CSkyBox::SetStageTexture(int nStageNum)
{
	switch (nStageNum)
	{
	case -1:
	case 0:
		m_texture = CTexture::TEXTURE_FLOOR;
		break;
	case 1:
		m_texture = CTexture::TEXTURE_WINDOW_OUTSIDE;
		break;
	case 2:
		m_texture = CTexture::TEXTURE_GALAXY;
		break;
	default:
		assert(false);
		break;
	}





}

//==============================================================================================
// 生成処理
//==============================================================================================
CSkyBox *CSkyBox::Create(D3DXVECTOR3 pos, int nStageNum)
{
	CSkyBox *pSkyBox = nullptr;

	pSkyBox = new CSkyBox(2);

	if (pSkyBox != nullptr)
	{
		pSkyBox->Init();
		pSkyBox->Setposition(pos);
		pSkyBox->SetStageTexture(nStageNum);
	}

	return pSkyBox;
}