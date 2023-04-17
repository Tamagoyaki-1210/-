//================================//
//								  //
//       歯車のファイル	    	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//インクルードファイル
//====================================
#include "gear.h"

//====================================
//グローバル変数
//====================================
static Gear g_aGear[MAX_GEAR];									//歯車型のグローバル変数を宣言する
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGear = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_apTexGear[5] = {};					//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_apTexLogo = NULL;					//テクスチャへのポインタ

//====================================
//歯車の初期化処理
//====================================
void InitGear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイス情報の取得
	VERTEX_2D*pVtx;									//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear.png",
		&g_apTexGear[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear2.png",
		&g_apTexGear[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear3.png",
		&g_apTexGear[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear4.png",
		&g_apTexGear[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Gear5.png",
		&g_apTexGear[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\TemporaryLogo.png",
		&g_apTexLogo);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GEAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGear,
		NULL);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogo,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGear = 0; nCntGear < MAX_GEAR; nCntGear++)
	{
		ZeroMemory(&g_aGear[nCntGear], sizeof(Gear));

		//頂点座標の設定
		pVtx[(nCntGear * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntGear * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntGear * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntGear * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[(nCntGear * 4) + 0].rhw = 1.0f;
		pVtx[(nCntGear * 4) + 1].rhw = 1.0f;
		pVtx[(nCntGear * 4) + 2].rhw = 1.0f;
		pVtx[(nCntGear * 4) + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[(nCntGear * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntGear * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntGear * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntGear * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[(nCntGear * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntGear * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntGear * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntGear * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffGear->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	g_pVtxBuffLogo->Unlock();
}

//====================================
//はぐるまの終了処理
//====================================
void UninitGear(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffGear != NULL)
	{			  
		g_pVtxBuffGear->Release();
		g_pVtxBuffGear = NULL;
	}
	if (g_pVtxBuffLogo != NULL)
	{			 
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}

	//テクスチャポインタの破棄
	for (int nCntTex = 0; nCntTex < 5; nCntTex++)
	{
		if (g_apTexGear[nCntTex] != NULL)
		{			   
			g_apTexGear[nCntTex]->Release();
			g_apTexGear[nCntTex] = NULL;
		}
	}
	if (g_apTexLogo != NULL)
	{		  
		g_apTexLogo->Release();
		g_apTexLogo = NULL;
	}
}

//====================================
//歯車の更新処理
//====================================
void UpdateGear(void)
{
	VERTEX_2D *pVtx = NULL;						//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGear = 0; nCntGear < MAX_GEAR; nCntGear++)
	{
		if (g_aGear[nCntGear].bUse == true)
		{
			g_aGear[nCntGear].fAngle += g_aGear[nCntGear].fFrameRot;			//回転角度の更新処理

			//===========================================================
			//正規化処理
			if (g_aGear[nCntGear].fAngle > D3DX_PI * 2.0f)
			{
				g_aGear[nCntGear].fAngle += -D3DX_PI * 2.0f;
			}
			else if (g_aGear[nCntGear].fAngle < -D3DX_PI * 2.0f)
			{
				g_aGear[nCntGear].fAngle += D3DX_PI * 2.0f;
			}
			//===========================================================

			//==================================================================================================================================================================
			//頂点座標の更新処理
			pVtx[(nCntGear * 4) + 0].pos.x = g_aGear[nCntGear].pos.x + sinf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (D3DX_PI * 0.75f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 0].pos.y = g_aGear[nCntGear].pos.y + cosf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (D3DX_PI * 0.75f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 0].pos.z = 0.0f;																													   
																																									   
			pVtx[(nCntGear * 4) + 1].pos.x = g_aGear[nCntGear].pos.x + sinf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (D3DX_PI * 0.25f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 1].pos.y = g_aGear[nCntGear].pos.y + cosf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (D3DX_PI * 0.25f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 1].pos.z = 0.0f;																													   
																																									   
			pVtx[(nCntGear * 4) + 2].pos.x = g_aGear[nCntGear].pos.x + sinf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (-D3DX_PI * 0.75f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 2].pos.y = g_aGear[nCntGear].pos.y + cosf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (-D3DX_PI * 0.75f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 2].pos.z = 0.0f;																													   
																																									   
			pVtx[(nCntGear * 4) + 3].pos.x = g_aGear[nCntGear].pos.x + sinf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (-D3DX_PI * 0.25f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 3].pos.y = g_aGear[nCntGear].pos.y + cosf(g_aGear[nCntGear].rot + g_aGear[nCntGear].fAngle + (-D3DX_PI * 0.25f)) * g_aGear[nCntGear].fLenght;
			pVtx[(nCntGear * 4) + 3].pos.z = 0.0f;
			//==================================================================================================================================================================
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffGear->Unlock();
}

//====================================
//歯車の描画処理
//====================================
void DrawGear(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGear, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCntGear = 0; nCntGear < MAX_GEAR; nCntGear++)
	{
		if (g_aGear[nCntGear].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexGear[g_aGear[nCntGear].nType]);

			//歯車を描画する
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGear * 4, 2);
		}
	}

	//元の設定に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//====================================
//はぐるまの設定処理
//====================================
void SetGear(D3DXVECTOR3 pos, float size, float frameRot, int type)
{
	VERTEX_2D *pVtx = NULL;							//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGear = 0; nCntGear < MAX_GEAR; nCntGear++)
	{
		if (g_aGear[nCntGear].bUse == false)
		{	   
			g_aGear[nCntGear].pos = pos;										//歯車の位置の設定
			g_aGear[nCntGear].fSize = size;										//歯車のサイズの設定

			//回転用の変数の設定
			g_aGear[nCntGear].fLenght = sqrtf((g_aGear[nCntGear].fSize * g_aGear[nCntGear].fSize) + (g_aGear[nCntGear].fSize * g_aGear[nCntGear].fSize)) * 0.5f;
			g_aGear[nCntGear].fAngle = atan2f(g_aGear[nCntGear].fSize * 0.5f, g_aGear[nCntGear].fSize * 0.5f);

			g_aGear[nCntGear].fFrameRot = frameRot;								//一フレームの回転角度の設定
			g_aGear[nCntGear].nType = type;										//歯車の種類の設定
			
			//頂点座標の設定
			pVtx[(nCntGear * 4) + 0].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y - (size * 0.5f), pos.z);
			pVtx[(nCntGear * 4) + 1].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y - (size * 0.5f), pos.z);
			pVtx[(nCntGear * 4) + 2].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y + (size * 0.5f), pos.z);
			pVtx[(nCntGear * 4) + 3].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y + (size * 0.5f), pos.z);

			//頂点カラーの設定
			pVtx[(nCntGear * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntGear * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntGear * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntGear * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_aGear[nCntGear].bUse = true;			//使用されている状態にする

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffGear->Unlock();
}

void SetDarkerGear(D3DXVECTOR3 pos, D3DXCOLOR col, float size, float frameRot, int type)
{
	VERTEX_2D *pVtx = NULL;							//頂点情報へのポインタ

													//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGear->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGear = 0; nCntGear < MAX_GEAR; nCntGear++)
	{
		if (g_aGear[nCntGear].bUse == false)
		{
			g_aGear[nCntGear].pos = pos;										//歯車の位置の設定
			g_aGear[nCntGear].fSize = size;										//歯車のサイズの設定

																				//回転用の変数の設定
			g_aGear[nCntGear].fLenght = sqrtf((g_aGear[nCntGear].fSize * g_aGear[nCntGear].fSize) + (g_aGear[nCntGear].fSize * g_aGear[nCntGear].fSize)) * 0.5f;
			g_aGear[nCntGear].fAngle = atan2f(g_aGear[nCntGear].fSize * 0.5f, g_aGear[nCntGear].fSize * 0.5f);

			g_aGear[nCntGear].fFrameRot = frameRot;								//一フレームの回転角度の設定
			g_aGear[nCntGear].nType = type;										//歯車の種類の設定

																				//頂点座標の設定
			pVtx[(nCntGear * 4) + 0].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y - (size * 0.5f), pos.z);
			pVtx[(nCntGear * 4) + 1].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y - (size * 0.5f), pos.z);
			pVtx[(nCntGear * 4) + 2].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y + (size * 0.5f), pos.z);
			pVtx[(nCntGear * 4) + 3].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y + (size * 0.5f), pos.z);

			//頂点カラーの設定
			pVtx[(nCntGear * 4) + 0].col = col;
			pVtx[(nCntGear * 4) + 1].col = col;
			pVtx[(nCntGear * 4) + 2].col = col;
			pVtx[(nCntGear * 4) + 3].col = col;

			g_aGear[nCntGear].bUse = true;			//使用されている状態にする

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffGear->Unlock();
}