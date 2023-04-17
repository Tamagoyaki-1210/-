//============================================================================
//
//メッシュポリゴンの処理[meshfield.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "meshcylinder.h"	//作成したmeshcylinder.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMeshCylinder = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;	//インデックスバッファへのポインタ
MESHCYLINDER g_aMeshCylinder;							//メッシュの情報
int g_nMeshCyPlimitive = 1000;
int g_nMeshCyVertex = 1000;
int g_nMeshCyIndex;

//============================================================================
//メッシュ円柱の初期化処理
//============================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	g_nMeshCyIndex = (((MAX_MESHCYLINDER + 1) * 2) * MAX_MESHCYLINDER) + ((MAX_MESHCYLINDER - 1) * 2);

	pDevice->CreateVertexBuffer(				//頂点バッファの生成
		sizeof(VERTEX_3D) * g_nMeshCyIndex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	D3DXCreateTextureFromFile(					//テクスチャーの読み込み
		pDevice,
		"data\\TEXTURE\\Skarmory.jpg",			//テクスチャのファイル名
		&g_pTextureMeshCylinder
	);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshCyIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	//位置の初期化処理
	g_aMeshCylinder.nMeshX = 3;
	g_aMeshCylinder.nMeshZ = 3;
	g_aMeshCylinder.pos = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	g_aMeshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する
	g_aMeshCylinder.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.fAngle = atan2f(MESHCYLINDER_WIDTH, MESHCYLINDER_HEIGHT);
	g_aMeshCylinder.fLength = sqrtf(MESHCYLINDER_WIDTH * MESHCYLINDER_WIDTH + MESHCYLINDER_HEIGHT * MESHCYLINDER_HEIGHT) / 2.0f;
	g_aMeshCylinder.Worldmtx = D3DXMATRIX();

	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

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

	//頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();
}

//============================================================================
//メッシュポリゴンの終了処理
//============================================================================
void UninitMeshCylinder(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;

	}
	//頂点バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}
	//インデックスバッファの解放
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//============================================================================
//メッシュポリゴンの更新処理
//============================================================================
void UpdateMeshCylinder(void)
{
	int nCntMeshCyX;
	int nCntMeshCyZ;
	int nDeg = 0;		//縮退ポリゴン

	if (GetKeyboardTrigger(DIK_O) == true)
	{
		if (g_aMeshCylinder.nMeshX < MAX_MESHCYLINDER)
		{
			g_aMeshCylinder.nMeshX++;
			g_aMeshCylinder.nMeshZ++;
		}
	}
	else if (GetKeyboardTrigger(DIK_P) == true)
	{
		if (g_aMeshCylinder.nMeshX > 1)
		{
			g_aMeshCylinder.nMeshX--;
			g_aMeshCylinder.nMeshZ--;
		}
	}

	//ポリゴン数計算式 = ((ポリゴン数.x * ポリゴン数.z) * 2) + ((ポリゴン数.z - 1) * 4)
	g_nMeshCyPlimitive = ((g_aMeshCylinder.nMeshX * g_aMeshCylinder.nMeshZ) * 2) + ((g_aMeshCylinder.nMeshZ - 1) * 4);

	//インデックス数計算式 = (((ポリゴン数.x + 1) * 2) * ポリゴン数.z) + ((ポリゴン数.y - 1) * 2)
	g_nMeshCyIndex = (((g_aMeshCylinder.nMeshX + 1) * 2) * g_aMeshCylinder.nMeshZ) + ((g_aMeshCylinder.nMeshZ - 1) * 2);

	//頂点数計算式 = (ポリゴン数.x + 1) * (ポリゴン数.z + 1)
	g_nMeshCyVertex = (g_aMeshCylinder.nMeshX + 1) * (g_aMeshCylinder.nMeshZ + 1);

	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//指定されたポリゴン数分、頂点を配置する処理
	for (nCntMeshCyZ = 0; nCntMeshCyZ < g_aMeshCylinder.nMeshZ + 1; nCntMeshCyZ++)
	{
		for (nCntMeshCyX = 0; nCntMeshCyX < g_aMeshCylinder.nMeshX + 1; nCntMeshCyX++)
		{
			pVtx[nCntMeshCyX + (nCntMeshCyZ * (g_aMeshCylinder.nMeshX + 1))].pos =
				D3DXVECTOR3((nCntMeshCyX * MESHCYLINDER_WIDTH) - (g_aMeshCylinder.nMeshX * MESHCYLINDER_WIDTH / 2),	//(現カウント長さ) - (ポリゴン数長さ)
					-(nCntMeshCyZ * MESHCYLINDER_HEIGHT) + (g_aMeshCylinder.nMeshZ * MESHCYLINDER_HEIGHT / 2),				//(現カウント長さ) + (ポリゴン数長さ)
					0.0f
				);

			//各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
			pVtx[nCntMeshCyX + (nCntMeshCyZ * (g_aMeshCylinder.nMeshX + 1))].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntMeshCyX + (nCntMeshCyZ * (g_aMeshCylinder.nMeshX + 1))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
			pVtx[nCntMeshCyX + (nCntMeshCyZ * (g_aMeshCylinder.nMeshX + 1))].tex = D3DXVECTOR2(nCntMeshCyX, nCntMeshCyZ);
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();

	//インデックスバッファをロック
	WORD* pIdx;
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの設定
	for (nCntMeshCyZ = 0; nCntMeshCyZ < g_aMeshCylinder.nMeshZ; nCntMeshCyZ++)
	{//z軸カウント
		if (nCntMeshCyZ != 0)
		{//縮退ポリゴンが必要な場合
			pIdx[(((g_aMeshCylinder.nMeshX + 1) * 2) * nCntMeshCyZ) + nDeg] = ((g_aMeshCylinder.nMeshX + 1) * nCntMeshCyZ) - 1;		//pIdx[現最後のインデックス] = x,z軸計算
			pIdx[(((g_aMeshCylinder.nMeshX + 1) * 2) * nCntMeshCyZ) + nDeg + 1] = (g_aMeshCylinder.nMeshX + 1) * (nCntMeshCyZ + 1);	//pIdx[現最後のインデックス + 1] = x,z軸計算
			nDeg += 2;
		}
		//x軸カウント
		for (nCntMeshCyX = 0; nCntMeshCyX < (g_aMeshCylinder.nMeshX + 1); nCntMeshCyX++)
		{
			//pIdx[(現カウントx軸 * 2) + ((数える分のx軸 * 2) * 現カウントz軸) + 縮退ポリゴン]
			pIdx[(nCntMeshCyX * 2) + (((g_aMeshCylinder.nMeshX + 1) * 2) * nCntMeshCyZ) + nDeg]
				// = (数える分のx軸) + 現カウントx軸 + ((数える分のx軸) * 現カウントz軸)
				= (g_aMeshCylinder.nMeshX + 1) + nCntMeshCyX + ((g_aMeshCylinder.nMeshX + 1) * nCntMeshCyZ);

			//pIdx[上の+1]
			pIdx[(nCntMeshCyX * 2) + (((g_aMeshCylinder.nMeshX + 1) * 2) * nCntMeshCyZ) + nDeg + 1]
				// = 現カウントx軸 + ((数える分のx軸) * 現カウントz軸)
				= nCntMeshCyX + ((g_aMeshCylinder.nMeshX + 1) * nCntMeshCyZ);
		}
	}
	//インデックスのアンロック
	g_pIdxBuffMeshCylinder->Unlock();
}

//============================================================================
//メッシュポリゴンの描画処理
//============================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

											//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aMeshCylinder.Worldmtx);
	//向きを反映(YaW : y,Pitch : x,Roll : z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder.rot.y, g_aMeshCylinder.rot.x, g_aMeshCylinder.rot.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.Worldmtx, &g_aMeshCylinder.Worldmtx, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder.pos.x, g_aMeshCylinder.pos.y, g_aMeshCylinder.pos.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.Worldmtx, &g_aMeshCylinder.Worldmtx, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder.Worldmtx);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nMeshCyVertex, 0, g_nMeshCyPlimitive);

	//テクスチャの終了
	pDevice->SetTexture(0, NULL);
}
