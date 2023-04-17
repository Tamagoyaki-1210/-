//================================
//
// リザルト画面
// Author:Teruto Sato
//Author:Kishimoto Eiji
//
//================================
#include "result.h"
#include "input.h"
#include "mode.h"

//**************************************************
//マクロ定義
//**************************************************
#define MAX_WIN_LOSE		(2)			//「勝利」「敗北」
#define WIN_LOSE_WIDTH		(184.0f)	//勝敗UIの幅
#define WIN_LOSE_HEIGHT		(96.0f)		//勝敗UIの高さ
#define LEFTSIDE_POS_X		(SCREEN_WIDTH * 0.3f)	//左側のUIのX座標
#define LEFTSIDE_POS_Y		(SCREEN_HEIGHT * 0.7f)	//左側のUIのY座標
#define RIGHTSIDE_POS_X		(SCREEN_WIDTH * 0.7f)	//右側のUIのX座標
#define RIGHTSIDE_POS_Y		(SCREEN_HEIGHT * 0.7f)	//右側のUIのY座標

//**************************************************
//スタティック変数
//**************************************************
static LPDIRECT3DTEXTURE9		s_apTexture[MAX_WIN_LOSE];	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffWinLose = NULL;		//頂点バッファへのポインタ
static int s_nWinPlayerIdx;

//============================
// リザルトの初期化処理
//============================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	VERTEX_2D *pVtx;							//頂点情報へのポインタ

	//テクスチャポインタの初期化
	memset(s_apTexture, NULL, sizeof(s_apTexture));
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/FPD_ResultUI_LOSE.png",
								&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
								"data/TEXTURE/WORD/FPD_ResultUI_WIN.png",
								&s_apTexture[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_WIN_LOSE,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&s_pVtxBuffWinLose,
								NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffWinLose->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_WIN_LOSE; i++)
	{
		D3DXVECTOR3 posWinLose;		//勝敗UIの頂点座標設定用

		if (i == 0)
		{//1P側(左)
			posWinLose = D3DXVECTOR3(LEFTSIDE_POS_X, LEFTSIDE_POS_Y, 0.0f);
		}
		else if(i == 1)
		{//2P側(右)
			posWinLose = D3DXVECTOR3(RIGHTSIDE_POS_X, RIGHTSIDE_POS_Y, 0.0f);
		}

		//頂点座標の設定
		float fLeft		= (posWinLose.x - WIN_LOSE_WIDTH);
		float fRight	= (posWinLose.x + WIN_LOSE_WIDTH);
		float fTop		= (posWinLose.y - WIN_LOSE_HEIGHT);
		float fBottom	= (posWinLose.y + WIN_LOSE_HEIGHT);

		pVtx[0].pos = D3DXVECTOR3(fLeft,  fTop,    0.0f);
		pVtx[1].pos = D3DXVECTOR3(fRight, fTop,    0.0f);
		pVtx[2].pos = D3DXVECTOR3(fLeft,  fBottom, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fRight, fBottom, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//ポインタを進める
	}

	// 頂点バッファをアンロックする
	s_pVtxBuffWinLose->Unlock();
}

//============================
// リザルトの終了処理
//============================
void UninitResult(void)
{
	for (int i = 0; i < MAX_WIN_LOSE; i++)
	{
		if (s_apTexture[i] != NULL)
		{//テクスチャの破棄
			s_apTexture[i]->Release();
			s_apTexture[i] = NULL;
		}
	}

	if (s_pVtxBuffWinLose != NULL)
	{//頂点バッファの破棄
		s_pVtxBuffWinLose->Release();
		s_pVtxBuffWinLose = NULL;
	}
}

//============================
// リザルトの更新処理
//============================
void UpdateResult(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
	{
		ChangeMode(MODE_OPTION);
	}
}

//============================
// リザルトの描画処理
//============================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuffWinLose, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int i = 0; i < MAX_WIN_LOSE; i++)
	{
		if (s_nWinPlayerIdx == i)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, s_apTexture[1]);
		}
		else
		{
			//テクスチャの設定
			pDevice->SetTexture(0, s_apTexture[0]);
		}

		//チュートリアルの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								i * 4,					//描画する最初の頂点インデックス
								2);						//描画するプリミティブ数
	}
}

//============================
// リザルトの描画処理
//============================
void SetWInPlayerIdx(int nIdx)
{
	s_nWinPlayerIdx = nIdx;
}
