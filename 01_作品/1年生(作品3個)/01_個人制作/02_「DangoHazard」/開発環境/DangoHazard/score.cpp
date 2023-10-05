//============================================================================
//
//点数の処理[score.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "main.h"		//作成したmain.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "score.h"		//作成したscore.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//頂点バッファへのポインタ
Score g_aScore[MAX_DIGIT];							//点数桁毎の情報
int g_nScore;										//スコアの値

//============================================================================
//点数の初期化処理
//============================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\number000.png",			//テクスチャのファイル名
		&g_pTextureScore
	);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//構造体の初期化処理
		g_aScore[nCntScore].pos = D3DXVECTOR3((nCntScore + 1.0f) * SCORE_WIDTH, SCORE_HEIGHT, 0.0f);		//位置の初期化処理
		g_aScore[nCntScore].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//移動量の初期化処理
		g_aScore[nCntScore].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//向きを初期化する
		g_aScore[nCntScore].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					//スコアの初期配色
		g_aScore[nCntScore].fAngle = atan2f(SCORE_WIDTH, SCORE_HEIGHT);
		g_aScore[nCntScore].fLength = sqrtf(SCORE_WIDTH * SCORE_WIDTH + SCORE_HEIGHT * SCORE_HEIGHT) / 2.0f;
	}
	//初期スコア
	g_nScore = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_DIGIT ,	//sizeof(VERTEX_2D) * 必要な頂点数 * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aScore[nCntScore].pos.x - sinf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[0].pos.y = g_aScore[nCntScore].pos.y - cosf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[0].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[1].pos.x = g_aScore[nCntScore].pos.x + sinf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[1].pos.y = g_aScore[nCntScore].pos.y - cosf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[1].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[2].pos.x = g_aScore[nCntScore].pos.x - sinf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[2].pos.y = g_aScore[nCntScore].pos.y + cosf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[2].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[3].pos.x = g_aScore[nCntScore].pos.x + sinf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[3].pos.y = g_aScore[nCntScore].pos.y + cosf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[3].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2(0.0f,					0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / BASE_NUMBER,	0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,					1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / BASE_NUMBER,	1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//============================================================================
//点数の終了処理
//============================================================================
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//============================================================================
//点数の更新処理
//============================================================================
void UpdateScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aScore[nCntScore].pos.x - sinf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[0].pos.y = g_aScore[nCntScore].pos.y - cosf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[0].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[1].pos.x = g_aScore[nCntScore].pos.x + sinf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[1].pos.y = g_aScore[nCntScore].pos.y - cosf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[1].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[2].pos.x = g_aScore[nCntScore].pos.x - sinf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[2].pos.y = g_aScore[nCntScore].pos.y + cosf(g_aScore[nCntScore].fAngle - g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[2].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx[3].pos.x = g_aScore[nCntScore].pos.x + sinf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.x) * g_aScore[nCntScore].fLength;
		pVtx[3].pos.y = g_aScore[nCntScore].pos.y + cosf(g_aScore[nCntScore].fAngle + g_aScore[nCntScore].rot.y) * g_aScore[nCntScore].fLength;
		pVtx[3].pos.z = g_aScore[nCntScore].pos.z + 0.0f;

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//============================================================================
//点数の描画処理
//============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffScore,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntScore * 4,			//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
	//テクスチャの初期化(バグ修正_2023/7/12)
	pDevice->SetTexture(0, NULL);
}

//============================================================================
////点数の設定処理
//============================================================================
void SetScore(int nScore)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntScore;
	int aPosTexU[MAX_DIGIT];	//各桁の数字を格納

	//デバイスの取得
	pDevice = GetDevice();

	g_nScore += nScore;

	if (g_nScore >= 99999999)
	{
		g_nScore = 99999999;
	}

	aPosTexU[0] = g_nScore % 100000000 / 10000000;
	aPosTexU[1] = g_nScore % 10000000 / 1000000;
	aPosTexU[2] = g_nScore % 1000000 / 100000;
	aPosTexU[3] = g_nScore % 100000 / 10000;
	aPosTexU[4] = g_nScore % 10000 / 1000;
	aPosTexU[5] = g_nScore % 1000 / 100;
	aPosTexU[6] = g_nScore % 100 / 10;
	aPosTexU[7] = g_nScore % 10 / 1;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 0.0f,				0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 1.0f / BASE_NUMBER,	0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 0.0f,				1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 1.0f / BASE_NUMBER,	1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//============================================================================
////点数の加算処理
//============================================================================
void AddScore(int nValue)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntScore;
	int aPosTexU[MAX_DIGIT];	//各桁の数字を格納

	//デバイスの取得
	pDevice = GetDevice();

	g_nScore += nValue;

	if (g_nScore >= 99999999)
	{
		g_nScore = 99999999;
	}

	//指定の桁の数値を抽出する処理
	aPosTexU[0] = g_nScore % 100000000 / 10000000;
	aPosTexU[1] = g_nScore % 10000000 / 1000000;
	aPosTexU[2] = g_nScore % 1000000 / 100000;
	aPosTexU[3] = g_nScore % 100000 / 10000;
	aPosTexU[4] = g_nScore % 10000 / 1000;
	aPosTexU[5] = g_nScore % 1000 / 100;
	aPosTexU[6] = g_nScore % 100 / 10;
	aPosTexU[7] = g_nScore % 10 / 1;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_DIGIT; nCntScore++)
	{
		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 1.0f / BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntScore] / BASE_NUMBER) + 1.0f / BASE_NUMBER, 1.0f);
	
		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}