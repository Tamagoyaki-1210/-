//==========================================//
//										    //
//       landingPoint.cpp				    //
//       Author: Ricci Alex				    //
//										    //
//==========================================//

//====================================
//インクルードファイル
//====================================
#include "landingPoint.h"

//====================================
//グローバル変数
//====================================
static LandingMark g_aLandingMark;										
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLandingMark = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_apTexLandingMark = NULL;					//テクスチャへのポインタ

void InitLandingMark(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイス情報の取得
	VERTEX_2D*pVtx;									//頂点情報へのポインタ

													//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SilverGear.png",
		&g_apTexLandingMark);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLandingMark,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLandingMark->Lock(0, 0, (void**)&pVtx, 0);

	ZeroMemory(&g_aLandingMark, sizeof(LandingMark));

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffLandingMark->Unlock();
}

void UninitLandingMark(void)
{
	//テクスチャポインタの破棄
	if (g_apTexLandingMark != NULL)
	{
		g_apTexLandingMark->Release();
		g_apTexLandingMark = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLandingMark != NULL)
	{
		g_pVtxBuffLandingMark->Release();
		g_pVtxBuffLandingMark = NULL;
	}
}

void UpdateLandingMark(void)
{
	VERTEX_2D *pVtx = NULL;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLandingMark->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aLandingMark.bUse == true)
	{
		g_aLandingMark.nCntAnim++;

		if (g_aLandingMark.nCntAnim % 7 == 6)
		{
			g_aLandingMark.nAnimPattern ^= 1;
		}

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f * (1 - g_aLandingMark.nAnimPattern), 1.0f * g_aLandingMark.nAnimPattern, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f * (1 - g_aLandingMark.nAnimPattern), 1.0f * g_aLandingMark.nAnimPattern, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f * (1 - g_aLandingMark.nAnimPattern), 1.0f * g_aLandingMark.nAnimPattern, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f * (1 - g_aLandingMark.nAnimPattern), 1.0f * g_aLandingMark.nAnimPattern, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffLandingMark->Unlock();
}

void DrawLandingMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLandingMark, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aLandingMark.bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexLandingMark);

		//ディスクを描画する
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);
	}
}

void SetLandingMark(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeight, float fSpeed, float fAcc, float fSize)
{
	VERTEX_2D *pVtx = NULL;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLandingMark->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aLandingMark.bUse == false)
	{
		while (fHeight > 0.0f)
		{
			fHeight += fSpeed;
			fSpeed += fAcc;
			pos += move;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - (fSize * 0.25f), pos.y - (fSize * 0.25f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + (fSize * 0.25f), pos.y - (fSize * 0.25f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - (fSize * 0.25f), pos.y + (fSize * 0.25f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + (fSize * 0.25f), pos.y + (fSize * 0.25f), 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_aLandingMark.pos = pos;				//位置の設定
		g_aLandingMark.nCntAnim = 0;			//アニメーションカウンターの設定
		g_aLandingMark.nAnimPattern = 0;		//アニメーション用の変数の設定
		g_aLandingMark.bUse = true;				//使用されている状態にする
	}

	//頂点バッファをアンロックする
	g_pVtxBuffLandingMark->Unlock();
}

void DestroyLandingMark(void)
{
	g_aLandingMark.bUse = false;
}

LandingMark* GetLandingMark(void)
{
	return &g_aLandingMark;
}
