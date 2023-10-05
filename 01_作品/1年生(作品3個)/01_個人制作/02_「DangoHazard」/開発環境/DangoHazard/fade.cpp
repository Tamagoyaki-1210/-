//============================================================================
//
//フェード処理[fade.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "fade.h"		//作成したfade.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	//頂点バッファへのポインタ

FADE g_fade;									//フェードの状態
MODE g_modeNext;								//次の画面(モード)
D3DXCOLOR g_colorFade;							//ポリゴン(フェード)の色
//============================================================================
//フェードの初期化処理
//============================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//フェードイン状態に
	g_fade = FADE_IN;

	//次の画面(モード)を設定
	g_modeNext = modeNext;	

	//黒いポリゴン(不透明)にしておく
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4,	//sizeof(VERTEX_2D) * 必要な頂点数
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	//モード設定(次の画面に移行)
	SetMode(g_modeNext);
}

//============================================================================
//フェードの終了処理
//============================================================================
void UninitFade(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//============================================================================
//フェードの更新処理
//============================================================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{//何もしていない状態以外の場合の処理
		if (g_fade == FADE_IN)
		{//フェードイン状態(透明化処理)
			if (g_colorFade.a <= 0.0f)
			{//不透明度が0.0fになった場合の処理

				//何もしていない状態にする
				g_fade = FADE_NONE;
			}
			else
			{//不透明度が0.0fより上の場合
				g_colorFade.a -= (1.0f / 60.0f);	//ポリゴンを透明にしていく
			}
		}
		else if (g_fade == FADE_OUT)
		{//フェードアウト状態(不透明化処理)
			if (g_colorFade.a >= 1.0f)
			{//不透明度が1.0fになった場合の処理

				//フェードイン状態(透明化)にする
				g_fade = FADE_IN;			

				//モード設定(次の画面に移行)
				SetMode(g_modeNext);			
			}
			else
			{//不透明度が1.0f未満の場合
				g_colorFade.a += (1.0f / 60.0f);	//ポリゴンを不透明にしていく
			}
		}
		VERTEX_2D *pVtx;

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		//頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
}

//============================================================================
//フェードの描画処理
//============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffFade,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)	//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定(未使用の場合NULLを指定)
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,						//描画する最初の頂点インデックス
		2						//プリミティブアイコンの個数
	);
}

//============================================================================
//フェードの設定処理
//============================================================================
void SetFade(MODE modeNext)
{
	//フェードアウト状態にする
	g_fade = FADE_OUT;

	//次の画面(モード)を設定
	g_modeNext = modeNext;	

	//白いポリゴン(透明)にしておく
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}

//============================================================================
//フェードの取得処理
//============================================================================
FADE GetFade(void)
{
	return g_fade;
}