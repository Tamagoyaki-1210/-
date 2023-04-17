//============================================================================
//
//壁描画の処理[wall.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "wall.h"		//作成したwall.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "polygon.h"	//作成したpolygon.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//頂点バッファへのポインタ
WALL g_aWall[MAX_WALL];									

//============================================================================
//壁の初期化処理
//============================================================================
void InitWall(void)
{
	int nCntWall;
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

										//テクスチャーの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\block000.jpg",	//テクスチャのファイル名
		&g_pTextureWall
	);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//位置の初期化処理
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する
		g_aWall[nCntWall].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].bUse = false;
		g_aWall[nCntWall].fAngle = atan2f(WALL_WIDTH, WALL_HEIGHT);
		g_aWall[nCntWall].fLength = sqrtf(WALL_WIDTH * WALL_WIDTH + WALL_HEIGHT * WALL_HEIGHT) / 2.0f;
		g_aWall[nCntWall].Worldmtx = D3DXMATRIX();
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4  * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = -sinf(g_aWall[nCntWall].fAngle + g_aWall[nCntWall].rot.x) * g_aWall[nCntWall].fLength;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = cosf(g_aWall[nCntWall].fAngle + g_aWall[nCntWall].rot.z) * g_aWall[nCntWall].fLength;

		pVtx[1].pos.x = sinf(g_aWall[nCntWall].fAngle - g_aWall[nCntWall].rot.x) * g_aWall[nCntWall].fLength;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = cosf(g_aWall[nCntWall].fAngle - g_aWall[nCntWall].rot.z) * g_aWall[nCntWall].fLength;

		pVtx[2].pos.x = -sinf(g_aWall[nCntWall].fAngle - g_aWall[nCntWall].rot.x) * g_aWall[nCntWall].fLength;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = -cosf(g_aWall[nCntWall].fAngle - g_aWall[nCntWall].rot.z) * g_aWall[nCntWall].fLength;

		pVtx[3].pos.x = sinf(g_aWall[nCntWall].fAngle + g_aWall[nCntWall].rot.x) * g_aWall[nCntWall].fLength;
		pVtx[3].pos.y = 0.0f;
		pVtx[3].pos.z = -cosf(g_aWall[nCntWall].fAngle + g_aWall[nCntWall].rot.z) * g_aWall[nCntWall].fLength;

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
	g_pVtxBuffWall->Unlock();

	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT / 2, -WALL_WIDTH / 2), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(0.0f, WALL_HEIGHT / 2, WALL_WIDTH / 2), D3DXVECTOR3(D3DX_PI / 2, D3DX_PI, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetWall(D3DXVECTOR3(WALL_WIDTH / 2, WALL_HEIGHT / 2, 0.0f), D3DXVECTOR3(D3DX_PI / 2, -D3DX_PI / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f,1.0f));
	SetWall(D3DXVECTOR3(-WALL_WIDTH / 2, WALL_HEIGHT / 2, 0.0f), D3DXVECTOR3(D3DX_PI / 2, D3DX_PI / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//============================================================================
//壁の終了処理
//============================================================================
void UninitWall(void)
{
	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;

	}
	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//============================================================================
//壁の更新処理
//============================================================================
void UpdateWall(void)
{

}

//============================================================================
//壁の描画処理
//============================================================================
void DrawWall(void)
{
	int nCntWall;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCntWall].Worldmtx);
			//向きを反映(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].Worldmtx, &g_aWall[nCntWall].Worldmtx, &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntWall].Worldmtx, &g_aWall[nCntWall].Worldmtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].Worldmtx);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
	}
}

//============================================================================
//壁の設定処理
//============================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	int nCntWall;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;

			//頂点カラーの設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aWall[nCntWall].bUse = true;
		}
		pVtx += 4;
		nCntWall++;

		//壁の裏側の処理
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = -rot;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(col, col, col, 0.4f);
			pVtx[1].col = D3DXCOLOR(col, col, col, 0.4f);
			pVtx[2].col = D3DXCOLOR(col, col, col, 0.4f);
			pVtx[3].col = D3DXCOLOR(col, col, col, 0.4f);

			g_aWall[nCntWall].bUse = true;
			break;
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}
