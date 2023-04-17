//================================//
//								  //
//       影処理(shadow.cpp)		  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//インクルードファイル
//====================================
#include "shadow.h"

//====================================
//グローバル変数
//====================================
static Shadow g_aShadow[MAX_SHADOW];							//影型の配列
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_apTexShadow = NULL;					//テクスチャへのポインタ

//====================================
//影の初期化処理
//====================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイス情報の取得
	VERTEX_2D*pVtx;									//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_apTexShadow);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		ZeroMemory(&g_aShadow[nCntShadow], sizeof(Shadow));

		//頂点座標の設定
		pVtx[(nCntShadow * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[(nCntShadow * 4) + 0].rhw = 1.0f;
		pVtx[(nCntShadow * 4) + 1].rhw = 1.0f;
		pVtx[(nCntShadow * 4) + 2].rhw = 1.0f;
		pVtx[(nCntShadow * 4) + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[(nCntShadow * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntShadow * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntShadow * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntShadow * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[(nCntShadow * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntShadow * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntShadow * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

}

//====================================
//影の終了処理
//====================================
void UninitShadow(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}

	//テクスチャポインタの破棄
	if (g_apTexShadow != NULL)
	{
		g_apTexShadow->Release();
		g_apTexShadow = NULL;
	}
}

//====================================
//影の更新処理
//====================================
void UpdateShadow(void)
{
	VERTEX_2D*pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//==============================================================================================================================
			//頂点座標の更新
			pVtx[(nCntShadow * 4) + 0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - (g_aShadow[nCntShadow].fSize * 0.5f),
				g_aShadow[nCntShadow].pos.y - (g_aShadow[nCntShadow].fSize * 0.5f), 0.0f);

			pVtx[(nCntShadow * 4) + 1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + (g_aShadow[nCntShadow].fSize * 0.5f),
				g_aShadow[nCntShadow].pos.y - (g_aShadow[nCntShadow].fSize * 0.5f), 0.0f);

			pVtx[(nCntShadow * 4) + 2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - (g_aShadow[nCntShadow].fSize * 0.5f),
				g_aShadow[nCntShadow].pos.y + (g_aShadow[nCntShadow].fSize * 0.5f), 0.0f);

			pVtx[(nCntShadow * 4) + 3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + (g_aShadow[nCntShadow].fSize * 0.5f),
				g_aShadow[nCntShadow].pos.y + (g_aShadow[nCntShadow].fSize * 0.5f), 0.0f);
			//==============================================================================================================================
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//====================================
//影の描画処理
//====================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスの取得

	//デバイスの取得
	pDevice = GetDevice();

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexShadow);

			//ディスクを描画する
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetTexture(0, NULL);
}

//====================================
//影の設定処理
//====================================
int SetShadow(D3DXVECTOR3 pos, float size)
{
	VERTEX_2D*pVtx = NULL;								//頂点情報へのポインタ
	int nCntShadow;										//影をカウントする変数を宣言する

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{//使用されていない場合

			size *= 0.5f;								//大きさの調整

			g_aShadow[nCntShadow].pos = pos;			//影の位置の設定
			g_aShadow[nCntShadow].pos.x -= 8.0f;
			g_aShadow[nCntShadow].pos.y += 8.0f;		
			g_aShadow[nCntShadow].fSize = size;			//影の大きさの設定
			g_aShadow[nCntShadow].bUse = true;			//使用されている状態にする

			//頂点座標の設定
			pVtx[(nCntShadow * 4) + 0].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - (size * 0.5f), g_aShadow[nCntShadow].pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntShadow * 4) + 1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + (size * 0.5f), g_aShadow[nCntShadow].pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntShadow * 4) + 2].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - (size * 0.5f), g_aShadow[nCntShadow].pos.y + (size * 0.5f), 0.0f);
			pVtx[(nCntShadow * 4) + 3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + (size * 0.5f), g_aShadow[nCntShadow].pos.y + (size * 0.5f), 0.0f);

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;									//nCntShadowを返す
}

//====================================
//影の位置の更新処理
//====================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	//影の位置の更新
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].pos.x -= 8.0f;
	g_aShadow[nIdxShadow].pos.y += 8.0f;		
}

Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}