//============================================================================
//
//影描画の処理[shadow.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "shadow.h"		//作成したshadow.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
SHADOW g_aShadow[MAX_SHADOW];									

//============================================================================
//影の初期化処理
//============================================================================
void InitShadow(void)
{
	int nCntShadow;
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

										//テクスチャーの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\shadow000.jpg",	//テクスチャのファイル名
		&g_pTextureShadow
	);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//位置の初期化処理
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].bUse = false;
		g_aShadow[nCntShadow].fAngle = atan2f(SHADOW_WIDTH, SHADOW_HEIGHT);
		g_aShadow[nCntShadow].fLength = sqrtf(SHADOW_WIDTH * SHADOW_WIDTH + SHADOW_HEIGHT * SHADOW_HEIGHT) / 2.0f;
		g_aShadow[nCntShadow].Worldmtx = D3DXMATRIX();
	}

		//頂点バッファの生成
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffShadow,
			NULL);

		//頂点情報へのポインタ
		VERTEX_3D *pVtx = NULL;

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
		{
			//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
			pVtx[0].pos.x = -sinf(g_aShadow[nCntShadow].fAngle + g_aShadow[nCntShadow].rot.x) * g_aShadow[nCntShadow].fLength;
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = cosf(g_aShadow[nCntShadow].fAngle + g_aShadow[nCntShadow].rot.z) * g_aShadow[nCntShadow].fLength;

			pVtx[1].pos.x = sinf(g_aShadow[nCntShadow].fAngle - g_aShadow[nCntShadow].rot.x) * g_aShadow[nCntShadow].fLength;
			pVtx[1].pos.y = 0.0f;
			pVtx[1].pos.z = cosf(g_aShadow[nCntShadow].fAngle - g_aShadow[nCntShadow].rot.z) * g_aShadow[nCntShadow].fLength;

			pVtx[2].pos.x = -sinf(g_aShadow[nCntShadow].fAngle - g_aShadow[nCntShadow].rot.x) * g_aShadow[nCntShadow].fLength;
			pVtx[2].pos.y = 0.0f;
			pVtx[2].pos.z = -cosf(g_aShadow[nCntShadow].fAngle - g_aShadow[nCntShadow].rot.z) * g_aShadow[nCntShadow].fLength;

			pVtx[3].pos.x = sinf(g_aShadow[nCntShadow].fAngle + g_aShadow[nCntShadow].rot.x) * g_aShadow[nCntShadow].fLength;
			pVtx[3].pos.y = 0.0f;
			pVtx[3].pos.z = -cosf(g_aShadow[nCntShadow].fAngle + g_aShadow[nCntShadow].rot.z) * g_aShadow[nCntShadow].fLength;

			//各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;
		}
		//頂点バッファをアンロックする
		g_pVtxBuffShadow->Unlock();
	
}

//============================================================================
//影の終了処理
//============================================================================
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;

	}
	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//============================================================================
//影の更新処理
//============================================================================
void UpdateShadow(void)
{}

//============================================================================
//影の描画処理
//============================================================================
void DrawShadow(void)
{
	int nCntShadow;
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//g_ColorShadow = 1.0f - g_ColorShadow * g_ColorShadow.a;
	 
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureShadow);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].Worldmtx);
			//向きを反映(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].Worldmtx, &g_aShadow[nCntShadow].Worldmtx, &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].Worldmtx, &g_aShadow[nCntShadow].Worldmtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].Worldmtx);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}
	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//テクスチャの初期化(バグ修正_2023/7/12)
	pDevice->SetTexture(0, NULL);
}

//============================================================================
//影の設定処理
//============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nCntShadow;
	
	for(nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)
		{
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].bUse = true;
			break;
		}
	}
	return nCntShadow;	//影の番号(index)を返す
}
																																																											
//============================================================================
//影の位置更新処理
//============================================================================
void SetpositionShadow(int nIdxShadow, D3DXVECTOR3 pos , float fLength, D3DXCOLOR col)
{
	g_aShadow[nIdxShadow].pos = pos;		//引数で指定された番号のposを設定

	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	g_aShadow[nIdxShadow].fLength = fLength;

	if (fLength <= 0)
	{//影の大きさが0以下になった場合
		g_aShadow[nIdxShadow].fLength = 0;
	}

	pVtx += 4 * nIdxShadow;

	//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
	pVtx[0].pos.x = -sinf(g_aShadow[nIdxShadow].fAngle + g_aShadow[nIdxShadow].rot.x) * g_aShadow[nIdxShadow].fLength;
	pVtx[0].pos.y = 0.0f;
	pVtx[0].pos.z = cosf(g_aShadow[nIdxShadow].fAngle + g_aShadow[nIdxShadow].rot.z) * g_aShadow[nIdxShadow].fLength;

	pVtx[1].pos.x = sinf(g_aShadow[nIdxShadow].fAngle - g_aShadow[nIdxShadow].rot.x) * g_aShadow[nIdxShadow].fLength;
	pVtx[1].pos.y = 0.0f;
	pVtx[1].pos.z = cosf(g_aShadow[nIdxShadow].fAngle - g_aShadow[nIdxShadow].rot.z) * g_aShadow[nIdxShadow].fLength;

	pVtx[2].pos.x = -sinf(g_aShadow[nIdxShadow].fAngle - g_aShadow[nIdxShadow].rot.x) * g_aShadow[nIdxShadow].fLength;
	pVtx[2].pos.y = 0.0f;
	pVtx[2].pos.z = -cosf(g_aShadow[nIdxShadow].fAngle - g_aShadow[nIdxShadow].rot.z) * g_aShadow[nIdxShadow].fLength;

	pVtx[3].pos.x = sinf(g_aShadow[nIdxShadow].fAngle + g_aShadow[nIdxShadow].rot.x) * g_aShadow[nIdxShadow].fLength;
	pVtx[3].pos.y = 0.0f;
	pVtx[3].pos.z = -cosf(g_aShadow[nIdxShadow].fAngle + g_aShadow[nIdxShadow].rot.z) * g_aShadow[nIdxShadow].fLength;

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}