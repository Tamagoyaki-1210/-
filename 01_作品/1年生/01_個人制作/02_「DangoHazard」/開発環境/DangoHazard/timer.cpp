//============================================================================
//
//タイマーの処理[timer.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "main.h"		//作成したmain.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "timer.h"		//作成したtimer.hをインクルードする
#include "game.h"		//作成したgame.hをインクルードする
#include "fade.h"		//作成したfade.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		//頂点バッファへのポインタ
TIMER g_aTimer[MAX_TIMER_DIGIT];					//点数桁毎の情報
int g_nTimer;									//タイマーの値
int g_nCounterBreak;

//============================================================================
//タイマーの初期化処理
//============================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTimer;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\number000.png",			//テクスチャのファイル名
		&g_pTextureTimer
	);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		if (nCntTimer < MINIUT_TIMER_DIGIT)
		{
			//構造体の初期化処理
			g_aTimer[nCntTimer].pos = D3DXVECTOR3(-TIMER_WIDTH * MINIUT_TIMER_DIGIT + (nCntTimer) * TIMER_WIDTH + SCREEN_WIDTH / 2,
				TIMER_HEIGHT, 0.0f);		//位置の初期化処理
		}
		else
		{
			g_aTimer[nCntTimer].pos = D3DXVECTOR3(-TIMER_WIDTH * MINIUT_TIMER_DIGIT + TIMER_WIDTH + (nCntTimer) * TIMER_WIDTH + SCREEN_WIDTH / 2,
				TIMER_HEIGHT, 0.0f);		//位置の初期化処理
		}
			g_aTimer[nCntTimer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//移動量の初期化処理
			g_aTimer[nCntTimer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						//向きを初期化する
			g_aTimer[nCntTimer].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);					//タイマーの初期配色
			g_aTimer[nCntTimer].fAngle = atan2f(TIMER_WIDTH, TIMER_HEIGHT);
			g_aTimer[nCntTimer].fLength = sqrtf(TIMER_WIDTH * TIMER_WIDTH + TIMER_HEIGHT * TIMER_HEIGHT) / 2.0f;
	}
	g_nTimer = COUNT_TIMER;
	g_nCounterBreak = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_TIMER_DIGIT,	//sizeof(VERTEX_2D) * 必要な頂点数 * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aTimer[nCntTimer].pos.x - sinf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[0].pos.y = g_aTimer[nCntTimer].pos.y - cosf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[0].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[1].pos.x = g_aTimer[nCntTimer].pos.x + sinf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[1].pos.y = g_aTimer[nCntTimer].pos.y - cosf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[1].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[2].pos.x = g_aTimer[nCntTimer].pos.x - sinf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[2].pos.y = g_aTimer[nCntTimer].pos.y + cosf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[2].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[3].pos.x = g_aTimer[nCntTimer].pos.x + sinf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[3].pos.y = g_aTimer[nCntTimer].pos.y + cosf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[3].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

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
		pVtx[1].tex = D3DXVECTOR2(1.0f / TIMER_BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / TIMER_BASE_NUMBER, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();

	CounterTimer(1);
}

//============================================================================
//タイマーの終了処理
//============================================================================
void UninitTimer(void)
{
	//テクスチャの破棄
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//============================================================================
//タイマーの更新処理
//============================================================================
void UpdateTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntTimer;

	//デバイスの取得
	pDevice = GetDevice();
	VERTEX_2D *pVtx;

	FADE fade;
	fade = GetFade();

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aTimer[nCntTimer].pos.x - sinf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[0].pos.y = g_aTimer[nCntTimer].pos.y - cosf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[0].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[1].pos.x = g_aTimer[nCntTimer].pos.x + sinf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[1].pos.y = g_aTimer[nCntTimer].pos.y - cosf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[1].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[2].pos.x = g_aTimer[nCntTimer].pos.x - sinf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[2].pos.y = g_aTimer[nCntTimer].pos.y + cosf(g_aTimer[nCntTimer].fAngle - g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[2].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		pVtx[3].pos.x = g_aTimer[nCntTimer].pos.x + sinf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.x) * g_aTimer[nCntTimer].fLength;
		pVtx[3].pos.y = g_aTimer[nCntTimer].pos.y + cosf(g_aTimer[nCntTimer].fAngle + g_aTimer[nCntTimer].rot.y) * g_aTimer[nCntTimer].fLength;
		pVtx[3].pos.z = g_aTimer[nCntTimer].pos.z + 0.0f;

		//if(g_nTimer < 10)
		//{//0秒未満の場合
		//	//頂点カラーの設定(赤)
		//	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		//	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		//	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		//	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		//}
		//else
		//{//10秒以上の場合
		//	//頂点カラーの設定
		//	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//}

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	if (g_nTimer <= 0)
	{//タイマーが0以下の場合
		g_nTimer = 0;
		if (fade == FADE_NONE)
		{//フェードが行われてない場合の処理
			SetGameState(GAMESTATE_GAMECLEAR);
		}
	}

	g_nCounterBreak++;
	if (g_nCounterBreak == 60)
	{
		if (g_nTimer <= 0)
		{//0秒より下の場合
			if (fade == FADE_NONE)
			{//フェードが行われてない場合の処理
				SetGameState(GAMESTATE_GAMECLEAR);
			}
		}
		CounterTimer(1);
		g_nCounterBreak = 0;
	}

	if (g_nTimer <= 0)
	{//0秒より下の場合
		g_nTimer = 0;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//============================================================================
//点数の描画処理
//============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntTimer;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffTimer,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTimer);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntTimer * 4,			//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}

//============================================================================
////タイマーの設定処理
//============================================================================
void SetTimer(int nTimer)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntTimer;
	int aPosTexU[MAX_TIMER_DIGIT];	//各桁の数字を格納
		
	pDevice = GetDevice();			//デバイスの取得

	g_nTimer = nTimer;

	if (g_nTimer >= 99999999)
	{
		g_nTimer = 99999999;
	}

	if (g_nTimer >= 0)
	{
		aPosTexU[0] = g_nTimer % 6000 / 600;
		aPosTexU[1] = g_nTimer % 600 / 60;
		aPosTexU[2] = g_nTimer % 60 / 10;
		aPosTexU[3] = g_nTimer % 10;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 1.0f / TIMER_BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 1.0f / TIMER_BASE_NUMBER, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//============================================================================
////タイマーのカウンター処理
//============================================================================
void CounterTimer(int nValue)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntTimer;
	int aPosTexU[MAX_TIMER_DIGIT];	//各桁の数字を格納

								//デバイスの取得
	pDevice = GetDevice();

		g_nTimer -= nValue;
		if (g_nTimer >= 99999999)
		{
			g_nTimer = 99999999;
		}
	
		if (g_nTimer >= 0)
		{
			aPosTexU[0] = g_nTimer % 6000 / 600;
			aPosTexU[1] = g_nTimer % 600 / 60;
			aPosTexU[2] = g_nTimer % 60 / 10;
			aPosTexU[3] = g_nTimer % 10;
		}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
	
	for (nCntTimer = 0; nCntTimer < MAX_TIMER_DIGIT; nCntTimer++)
	{
		//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
		pVtx[0].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 1.0f / TIMER_BASE_NUMBER, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f * aPosTexU[nCntTimer] / TIMER_BASE_NUMBER) + 1.0f / TIMER_BASE_NUMBER, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}