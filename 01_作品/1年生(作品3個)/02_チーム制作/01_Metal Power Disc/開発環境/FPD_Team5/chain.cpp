//================================//
//								  //
//      chain.cpp			 	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//インクルードファイル
//====================================
#include "chain.h"

//====================================
//グローバル変数
//====================================
static Chain g_Chain;											//配管型のグローバル変数を宣言する
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffChain = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_pTexChain = NULL;					//テクスチャへのポインタ

//====================================
//鎖の初期化処理
//====================================
void InitChain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイス情報の取得
	VERTEX_2D*pVtx;									//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Chain.png",
		&g_pTexChain);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffChain,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChain->Lock(0, 0, (void**)&pVtx, 0);

	ZeroMemory(&g_Chain, sizeof(Chain));

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
	pVtx[2].tex = D3DXVECTOR2(0.0f, 2.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 2.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffChain->Unlock();
}

//====================================
//鎖の終了処理
//====================================
void UninitChain(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffChain != NULL)
	{
		g_pVtxBuffChain->Release();
		g_pVtxBuffChain = NULL;
	}

	//テクスチャポインタの破棄
	if (g_pTexChain != NULL)
	{
		g_pTexChain->Release();
		g_pTexChain = NULL;
	}
}

//====================================
//鎖の更新処理
//====================================
void UpdateChain(void)
{

}

//====================================
//鎖の描画処理
//====================================
void DrawChain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffChain, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Chain.bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexChain);

		//歯車を描画する
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//====================================
//鎖の設定処理
//====================================
void SetChain(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	VERTEX_2D *pVtx = NULL;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffChain->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Chain.bUse == false)
	{
		g_Chain.pos = pos;
		g_Chain.size = size;
		g_Chain.bUse = true;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - size.x, pos.y - size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - size.x, pos.y + size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 2.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 2.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffChain->Unlock();
	}
}