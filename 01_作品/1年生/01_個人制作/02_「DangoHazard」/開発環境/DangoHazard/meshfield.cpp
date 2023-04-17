//============================================================================
//
//メッシュポリゴンの処理[meshfield.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "meshfield.h"	//作成したmeshfield.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMeshField = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;	//インデックスバッファへのポインタ
MESHFIELD g_aMeshField;								//メッシュの情報
int g_nMeshPlimitive = 1024;
int g_nMeshVertex = 1024;
int g_nMeshIndex;

//============================================================================
//メッシュポリゴンの初期化処理
//============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
							
	int g_nMeshIndex = (((MAX_MESHFIELD + 1) * 2) * MAX_MESHFIELD) + ((MAX_MESHFIELD - 1) * 2);

	pDevice->CreateVertexBuffer(				//頂点バッファの生成
		sizeof(VERTEX_3D) * g_nMeshVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	D3DXCreateTextureFromFile(					//テクスチャーの読み込み
		pDevice,
		"data\\TEXTURE\\field000.jpg",			//テクスチャのファイル名
		&g_pTextureMeshField
	);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	//位置の初期化処理
	g_aMeshField.nMeshX = MAX_MESHFIELD;
	g_aMeshField.nMeshZ = MAX_MESHFIELD;
	g_aMeshField.pos = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する
	g_aMeshField.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aMeshField.fAngle = atan2f(MESHFIELD_WIDTH, MESHFIELD_HEIGHT);
	g_aMeshField.fLength = sqrtf(MESHFIELD_WIDTH * MESHFIELD_WIDTH + MESHFIELD_HEIGHT * MESHFIELD_HEIGHT) / 2.0f;
	g_aMeshField.Worldmtx = D3DXMATRIX();

	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffMeshField->Unlock();
}

//============================================================================
//メッシュポリゴンの終了処理
//============================================================================
void UninitMeshField(void)
{
	//テクスチャの破棄
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;

	}
	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	//インデックスバッファの解放
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}	
}

//============================================================================
//メッシュポリゴンの更新処理
//============================================================================
void UpdateMeshField(void)
{
	int nCntMeshX;
	int nCntMeshZ;
	int nDeg = 0;		//縮退ポリゴン

	//if (GetKeyboardTrigger(DIK_O) == true)
	//{
	//	if (g_aMeshField.nMeshX < MAX_MESHFIELD)
	//	{
	//		g_aMeshField.nMeshX++;
	//		g_aMeshField.nMeshZ++;
	//	}
	//}
	//else if(GetKeyboardTrigger(DIK_P) == true)
	//{
	//	if (g_aMeshField.nMeshX > 1)
	//	{
	//		g_aMeshField.nMeshX--;
	//		g_aMeshField.nMeshZ--;
	//	}
	//}

	//ポリゴン数計算式 = ((ポリゴン数.x * ポリゴン数.z) * 2) + ((ポリゴン数.z - 1) * 4)
	g_nMeshPlimitive = ((g_aMeshField.nMeshX * g_aMeshField.nMeshZ) * 2) + ((g_aMeshField.nMeshZ - 1) * 4);

	//インデックス数計算式 = (((ポリゴン数.x + 1) * 2) * ポリゴン数.z) + ((ポリゴン数.y - 1) * 2)
	g_nMeshIndex = (((g_aMeshField.nMeshX + 1) * 2) * g_aMeshField.nMeshZ) + ((g_aMeshField.nMeshZ - 1) * 2);	

	//頂点数計算式 = (ポリゴン数.x + 1) * (ポリゴン数.z + 1)
	g_nMeshVertex = (g_aMeshField.nMeshX + 1) * (g_aMeshField.nMeshZ + 1);								
																					
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//指定されたポリゴン数分、頂点を配置する処理
	for (nCntMeshZ = 0; nCntMeshZ < g_aMeshField.nMeshZ + 1; nCntMeshZ++)
	{
		for (nCntMeshX = 0; nCntMeshX < g_aMeshField.nMeshX + 1; nCntMeshX++)
		{
			pVtx[nCntMeshX + (nCntMeshZ * (g_aMeshField.nMeshX + 1))].pos = 
				D3DXVECTOR3((nCntMeshX * MESHFIELD_WIDTH) - (g_aMeshField.nMeshX * MESHFIELD_WIDTH / 2),	//(現カウント長さ) - (ポリゴン数長さ)
				0.0f,
				-(nCntMeshZ * MESHFIELD_HEIGHT) + (g_aMeshField.nMeshZ * MESHFIELD_HEIGHT / 2)				//(現カウント長さ) + (ポリゴン数長さ)
				);
			
			//各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
			pVtx[nCntMeshX + (nCntMeshZ * (g_aMeshField.nMeshX + 1))].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCntMeshX + (nCntMeshZ * (g_aMeshField.nMeshX + 1))].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
			pVtx[nCntMeshX + (nCntMeshZ * (g_aMeshField.nMeshX + 1))].tex = D3DXVECTOR2(nCntMeshX , nCntMeshZ );
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();

	//インデックスバッファをロック
	WORD* pIdx;
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの設定
	for (nCntMeshZ = 0; nCntMeshZ < g_aMeshField.nMeshZ; nCntMeshZ++)
	{//z軸カウント
		if(nCntMeshZ != 0)
		{//縮退ポリゴンが必要な場合
			pIdx[(((g_aMeshField.nMeshX + 1) * 2) * nCntMeshZ) + nDeg] = ((g_aMeshField.nMeshX + 1) * nCntMeshZ) - 1;		//pIdx[現最後のインデックス] = x,z軸計算
			pIdx[(((g_aMeshField.nMeshX + 1) * 2) * nCntMeshZ) + nDeg + 1] = (g_aMeshField.nMeshX + 1) * (nCntMeshZ + 1);	//pIdx[現最後のインデックス + 1] = x,z軸計算
			nDeg += 2;
		}
		//x軸カウント
		for (nCntMeshX = 0; nCntMeshX < (g_aMeshField.nMeshX + 1); nCntMeshX++)
		{
			//pIdx[(現カウントx軸 * 2) + ((数える分のx軸 * 2) * 現カウントz軸) + 縮退ポリゴン]
			pIdx[(nCntMeshX * 2) + (((g_aMeshField.nMeshX + 1) * 2) * nCntMeshZ) + nDeg]	
				// = (数える分のx軸) + 現カウントx軸 + ((数える分のx軸) * 現カウントz軸)
				= (g_aMeshField.nMeshX + 1) + nCntMeshX + ((g_aMeshField.nMeshX + 1) * nCntMeshZ);

			//pIdx[上の+1]
			pIdx[(nCntMeshX * 2) + (((g_aMeshField.nMeshX + 1) * 2) * nCntMeshZ) + nDeg + 1]
				// = 現カウントx軸 + ((数える分のx軸) * 現カウントz軸)
				= nCntMeshX + ((g_aMeshField.nMeshX + 1) * nCntMeshZ);
		}
	}
	//インデックスのアンロック
	g_pIdxBuffMeshField->Unlock();
}

//============================================================================
//メッシュポリゴンの描画処理
//============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

											//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aMeshField.Worldmtx);
	//向きを反映(YaW : y,Pitch : x,Roll : z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField.rot.y, g_aMeshField.rot.x, g_aMeshField.rot.z);
	D3DXMatrixMultiply(&g_aMeshField.Worldmtx, &g_aMeshField.Worldmtx, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aMeshField.pos.x, g_aMeshField.pos.y, g_aMeshField.pos.z);
	D3DXMatrixMultiply(&g_aMeshField.Worldmtx, &g_aMeshField.Worldmtx, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField.Worldmtx);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0,0, g_nMeshVertex,0, g_nMeshPlimitive);

	//テクスチャの終了
	pDevice->SetTexture(0, NULL);
}

//============================================================================
//メッシュポリゴンの当たり判定処理
//============================================================================
bool FieldCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move)
{
	bool bIsGround = false;
	PLAYER *pPlayer = GetPlayer();

	if (pos->x < g_aMeshField.pos.x + (g_aMeshField.nMeshX * (MESHFIELD_WIDTH / 2))
		&& pos->x > g_aMeshField.pos.x - (g_aMeshField.nMeshX * (MESHFIELD_WIDTH / 2))
		&& pos->z < g_aMeshField.pos.z + (g_aMeshField.nMeshZ * (MESHFIELD_HEIGHT / 2))
		&& pos->z > g_aMeshField.pos.z - (g_aMeshField.nMeshZ * (MESHFIELD_HEIGHT / 2)))
	{//床に乗っている場合
		if (pos->y <= g_aMeshField.pos.y && posOld->y >= g_aMeshField.pos.y)
		{//高さがメッシュより高い場合
			pos->y = g_aMeshField.pos.y;
			move->y = 0;
			bIsGround = true;
		}
	}
	return bIsGround;
}
