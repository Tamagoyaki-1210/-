//=========================================
// 
// UI処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "ui.h"
#include "score.h"
#include "time.h"
#include "pop.h"
#include "point.h"

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureScoreAg;			//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffScoreAg = NULL;	//頂点バッファへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffScoreBg = NULL;	//頂点バッファへのポインタ

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void InitScoreDesign(void);
static void UninitScoreDesign(void);
static void UpdateScoreDesign(void);
static void DrawAgScoreDesign(void);
static void DrawBgScoreDesign(void);

//=========================================
// 初期化
//=========================================
void InitUI(void)
{
	InitScore();		// スコア
	InitTime();			// タイム
	InitPop();			// 得点
	InitScoreDesign();	// スコアデザイン
	InitPoint();		// ポイント
}

//=========================================
// 終了
//=========================================
void UninitUI(void)
{
	UninitScore();			// スコア
	UninitTime();			// タイム
	UninitPop();			// 得点
	UninitScoreDesign();	// スコアデザイン
	UninitPoint();			// ポイント
}

//=========================================
// 更新
//=========================================
void UpdateUI(void)
{
	UpdateScore();			// スコア
	UpdateTime();			// タイム
	UpdatePop();			// 得点
	UpdateScoreDesign();	// スコアデザイン
	UpdatePoint();			// ポイント
}

//=========================================
// 描画
//=========================================
void DrawUI()
{
	DrawPop();				// 得点
	DrawBgScoreDesign();	// スコアデザイン
	DrawTime();				// タイム
	DrawScore();			// スコア
	DrawAgScoreDesign();	// スコアデザイン
	DrawPoint();			// ポイント
}

//=========================================
// スコアデザインの初期化
//=========================================
void InitScoreDesign(void)
{
	{
		//テクスチャーの読み込み
		D3DXCreateTextureFromFile(GetDevice(), "data\\TEXTURE\\ScorePaling.png", &s_pTextureScoreAg);

		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&s_pVtxBuffScoreAg,
			NULL);

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffScoreAg->Lock(0, 0, (void**)&pVtx, 0);

		D3DXVECTOR3 pos(SCREEN_WIDTH * 0.5f, 30.0f, 0.0f);
		D3DXVECTOR2 size(512.0f * 0.4f, 256.0f * 0.4f);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - size.x, pos.y - size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - size.x, pos.y + size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + size.x, pos.y + size.y, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuffScoreAg->Unlock();
	}

	{
		// 頂点バッファの生成
		GetDevice()->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&s_pVtxBuffScoreBg,
			NULL);

		VERTEX_2D *pVtx;		// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuffScoreBg->Lock(0, 0, (void**)&pVtx, 0);

		D3DXVECTOR3 pos(SCREEN_WIDTH * 0.5f, 30.0f, 0.0f);
		D3DXVECTOR2 size(512.0f * 0.375f, 256.0f * 0.35f);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x - size.x, pos.y - size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + size.x, pos.y - size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - size.x + 27.0f, pos.y + size.y - 10.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + size.x - 27.0f, pos.y + size.y - 10.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファをアンロックする
		s_pVtxBuffScoreBg->Unlock();
	}
}

//=========================================
// スコアデザインの終了
//=========================================
void UninitScoreDesign(void)
{
	// テクスチャの破棄
	if (s_pTextureScoreAg != NULL)
	{
		s_pTextureScoreAg->Release();
		s_pTextureScoreAg = NULL;
	}

	// 頂点バッファの破棄
	if (s_pVtxBuffScoreAg != NULL)
	{
		s_pVtxBuffScoreAg->Release();
		s_pVtxBuffScoreAg = NULL;
	}
}

//=========================================
// スコアデザインの更新
//=========================================
void UpdateScoreDesign(void)
{

}

//=========================================
// スコアデザインの描画
//=========================================
void DrawAgScoreDesign(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント

												// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuffScoreAg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTextureScoreAg);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=========================================
// スコアデザインの描画
//=========================================
void DrawBgScoreDesign(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント

												// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, s_pVtxBuffScoreBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}
