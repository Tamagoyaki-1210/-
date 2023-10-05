//============================================================================
//
//ハイスコアの処理[high_score.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "main.h"		//作成したmain.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "score.h"		//作成したscore.hをインクルードする
#include "high_score.h"	//作成したhigh_score.hをインクルードする
#include <stdio.h>		//stdio.hをインクルードする(FILEに必要)

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureHighScore = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHighScore = NULL;		//頂点バッファへのポインタ
HIGHSCORE g_aHighScore[MAX_DIGIT];						//点数桁毎の情報
int g_nHighScore;										//スコアの値

//============================================================================
//ハイスコアの初期化処理
//============================================================================
void InitHighScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntHighScore;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\number000.png",			//テクスチャのファイル名
		&g_pTextureHighScore
	);

	for (nCntHighScore = 0; nCntHighScore < MAX_DIGIT; nCntHighScore++)
	{
		//構造体の初期化処理
		g_aHighScore[nCntHighScore].pos = D3DXVECTOR3((nCntHighScore + 1.0f) * HIGH_SCORE_WIDTH, HIGH_SCORE_HEIGHT, 0.0f);		//位置の初期化処理
		g_aHighScore[nCntHighScore].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//移動量の初期化処理
		g_aHighScore[nCntHighScore].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//向きを初期化する
		g_aHighScore[nCntHighScore].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					//スコアの初期配色
		g_aHighScore[nCntHighScore].fAngle = atan2f(HIGH_SCORE_WIDTH, HIGH_SCORE_HEIGHT);
		g_aHighScore[nCntHighScore].fLength = sqrtf(HIGH_SCORE_WIDTH * HIGH_SCORE_WIDTH + HIGH_SCORE_HEIGHT * HIGH_SCORE_HEIGHT) / 2.0f;
	}
	//初期スコア
	g_nHighScore = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_DIGIT,	//sizeof(VERTEX_2D) * 必要な頂点数 * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffHighScore,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHighScore = 0; nCntHighScore < MAX_DIGIT; nCntHighScore++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aHighScore[nCntHighScore].pos.x - sinf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[0].pos.y = g_aHighScore[nCntHighScore].pos.y - cosf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[0].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[1].pos.x = g_aHighScore[nCntHighScore].pos.x + sinf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[1].pos.y = g_aHighScore[nCntHighScore].pos.y - cosf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[1].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[2].pos.x = g_aHighScore[nCntHighScore].pos.x - sinf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[2].pos.y = g_aHighScore[nCntHighScore].pos.y + cosf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[2].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[3].pos.x = g_aHighScore[nCntHighScore].pos.x + sinf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[3].pos.y = g_aHighScore[nCntHighScore].pos.y + cosf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[3].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f / BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / BASE_NUMBER, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffHighScore->Unlock();
}

//============================================================================
//ハイスコアの終了処理
//============================================================================
void UninitHighScore(void)
{
	//テクスチャの破棄
	if (g_pTextureHighScore != NULL)
	{
		g_pTextureHighScore->Release();
		g_pTextureHighScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHighScore != NULL)
	{
		g_pVtxBuffHighScore->Release();
		g_pVtxBuffHighScore = NULL;
	}
}

//============================================================================
//ハイスコアの更新処理
//============================================================================
void UpdateHighScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntHighScore;

	//デバイスの取得
	pDevice = GetDevice();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffHighScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntHighScore = 0; nCntHighScore < MAX_DIGIT; nCntHighScore++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aHighScore[nCntHighScore].pos.x - sinf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[0].pos.y = g_aHighScore[nCntHighScore].pos.y - cosf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[0].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[1].pos.x = g_aHighScore[nCntHighScore].pos.x + sinf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[1].pos.y = g_aHighScore[nCntHighScore].pos.y - cosf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[1].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[2].pos.x = g_aHighScore[nCntHighScore].pos.x - sinf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[2].pos.y = g_aHighScore[nCntHighScore].pos.y + cosf(g_aHighScore[nCntHighScore].fAngle - g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[2].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx[3].pos.x = g_aHighScore[nCntHighScore].pos.x + sinf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.x) * g_aHighScore[nCntHighScore].fLength;
		pVtx[3].pos.y = g_aHighScore[nCntHighScore].pos.y + cosf(g_aHighScore[nCntHighScore].fAngle + g_aHighScore[nCntHighScore].rot.y) * g_aHighScore[nCntHighScore].fLength;
		pVtx[3].pos.z = g_aHighScore[nCntHighScore].pos.z + 0.0f;

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffHighScore->Unlock();
}

//============================================================================
//ハイスコアの描画処理
//============================================================================
void DrawHighScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntHighScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffHighScore,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntHighScore = 0; nCntHighScore < MAX_DIGIT; nCntHighScore++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureHighScore);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntHighScore * 4,		//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}
//============================================================================
//ハイスコア読み込み処理
//============================================================================
void LoadHighScore(void)
{
	//FILE *pFile;	//ファイルポインタを宣言

	//				//ファイルを読み解く
	//pFile = fopen(FILENAME_DATA, "rb");

	//if (pFile != NULL)
	//{//ファイルが開けた場合

	// //人数をテキストファイルから読み解く処理
	//	fread(&nMaxPlayer, sizeof(int), 1, pFile);

	//	//人数分配列からテキストファイルから読み解く処理
	//	fread(&pPlayer[0], sizeof(Player), nMaxPlayer, pFile);

	//	//ファイルを閉じる
	//	fclose(pFile);
	//}
	//else
	//{//ファイルが開けなかった場合
	//	printf("***ファイルが開けませんでした***\n");
	//}
}

//============================================================================
//ハイスコア保存処理
//============================================================================
void SaveHighScore(int nScore)
{
	//FILE *pFile;	//ファイルポインタを宣言

	//				//ファイルを開く
	//pFile = fopen(FILENAME_DATA, "wb");

	//if (pFile != NULL)
	//{//ファイルが開けた場合

	// //ファイルに書き出す
	//	printf("\n");
	//	printf("\t+-----------------------------------------------+\n");
	//	printf("\t|           テキストファイルに保存中...         |\n");
	//	printf("\t+-----------------------------------------------+\n");

	//	//人数をテキストファイルに書き込む処理
	//	fwrite(&nMaxPlayer, sizeof(int), 1, pFile);

	//	//人数分配列からテキストファイルに書き込む処理
	//	fwrite(&pPlayer[0], sizeof(Player), nMaxPlayer, pFile);

	//	printf("\t+*****************************+\n");
	//	printf("\t+**         保存完了!       **+\n");
	//	printf("\t+*****************************+\n");

	//	//ファイルを閉じる
	//	fclose(pFile);
	//}
	//else
	//{//ファイルが開けなかった場合
	//	printf("***ファイルが開けませんでした***\n");
	//}
}
