//============================================================================
//
//ポリゴン描画の処理[polygon.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "polygon.h"		//作成したpolygon.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePolygon = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//頂点バッファへのポインタ
POLYGON g_aPolygon;									//ポリゴンの情報

//============================================================================
//ポリゴンの初期化処理
//============================================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

										//テクスチャーの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\mofu.jpg",	//テクスチャのファイル名
		&g_pTexturePolygon
	);

	//位置の初期化処理
	g_aPolygon.pos = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	g_aPolygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPolygon.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する
	g_aPolygon.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aPolygon.fAngle = atan2f(POLYGON_WIDTH, POLYGON_HEIGHT);
	g_aPolygon.fLength = sqrtf(POLYGON_WIDTH * POLYGON_WIDTH + POLYGON_HEIGHT * POLYGON_HEIGHT) / 2.0f;
	g_aPolygon.Worldmtx = D3DXMATRIX();


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);
	
	//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
	pVtx[0].pos.x = g_aPolygon.pos.x - sinf(g_aPolygon.fAngle + g_aPolygon.rot.x) * g_aPolygon.fLength;
	pVtx[0].pos.y = g_aPolygon.pos.y;
	pVtx[0].pos.z = g_aPolygon.pos.z + cosf(g_aPolygon.fAngle + g_aPolygon.rot.z) * g_aPolygon.fLength;

	pVtx[1].pos.x = g_aPolygon.pos.x + sinf(g_aPolygon.fAngle - g_aPolygon.rot.x) * g_aPolygon.fLength;
	pVtx[1].pos.y = g_aPolygon.pos.y;
	pVtx[1].pos.z = g_aPolygon.pos.z + cosf(g_aPolygon.fAngle - g_aPolygon.rot.z) * g_aPolygon.fLength;

	pVtx[2].pos.x = g_aPolygon.pos.x - sinf(g_aPolygon.fAngle - g_aPolygon.rot.x) * g_aPolygon.fLength;
	pVtx[2].pos.y = g_aPolygon.pos.y;
	pVtx[2].pos.z = g_aPolygon.pos.z - cosf(g_aPolygon.fAngle - g_aPolygon.rot.z) * g_aPolygon.fLength;

	pVtx[3].pos.x = g_aPolygon.pos.x + sinf(g_aPolygon.fAngle + g_aPolygon.rot.x) * g_aPolygon.fLength;
	pVtx[3].pos.y = g_aPolygon.pos.y;
	pVtx[3].pos.z = g_aPolygon.pos.z - cosf(g_aPolygon.fAngle + g_aPolygon.rot.z) * g_aPolygon.fLength;

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

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//============================================================================
//ポリゴンの終了処理
//============================================================================
void UninitPolygon(void)
{
	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;

	}
	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//============================================================================
//ポリゴンの更新処理
//============================================================================
void UpdatePolygon(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ポリゴンを動かす処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (GetKeyboardPress(DIK_J) == true)
	{//左移動
		g_aPolygon.pos.x += 1.0f;
	}
	else if (GetKeyboardPress(DIK_L) == true)
	{//右移動
		g_aPolygon.pos.x -= 1.0f;
	}
	if (GetKeyboardPress(DIK_I) == true)
	{//上移動
		g_aPolygon.pos.y += 1.0f;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//下移動
		g_aPolygon.pos.y -= 1.0f;
	}
	if (GetKeyboardPress(DIK_U) == true)
	{//前移動
		g_aPolygon.pos.z += 1.0f;
	}
	else if (GetKeyboardPress(DIK_O) == true)
	{//後ろ移動
		g_aPolygon.pos.z -= 1.0f;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//============================================================================
//ポリゴンの描画処理
//============================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPolygon.Worldmtx);
	//向きを反映(YaW : y,Pitch : x,Roll : z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPolygon.rot.y, g_aPolygon.rot.x, g_aPolygon.rot.z);
	D3DXMatrixMultiply(&g_aPolygon.Worldmtx, &g_aPolygon.Worldmtx, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aPolygon.pos.x, g_aPolygon.pos.y, g_aPolygon.pos.z);
	D3DXMatrixMultiply(&g_aPolygon.Worldmtx, &g_aPolygon.Worldmtx, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aPolygon.Worldmtx);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygon);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
