//=========================================
// 
// ポイントの表示
// Author YudaKaito
// Author Tanimoto
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "round.h"
#include "score.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define NUM_ROUND		(4)
#define DIGIT_ROUND		(2)
#define ROUND_WIDTH		(128)
#define ROUND_HEIGHT	(128)

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff;				//頂点バッファへのポインタ
static Round s_aRound[NUM_ROUND];
static bool s_bRoundUse;
static int s_nUseTime;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void DestroyRound(void);

//=========================================
// 初期化
//=========================================
void InitRound(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/SetPoint.png", &s_pTexture);

	Round* pRound = s_aRound;

	for (int nCntScore = 0; nCntScore < NUM_ROUND; nCntScore++, pRound++)
	{
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(
			(sizeof(VERTEX_2D) * 4 * 2),		//4つで画像一個分
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pRound->pVtxBuff,
			NULL);

		//pos.x = ((画面半分) - (長さ横 * (桁数 + 2個分))) + ((長さ横 * (表示数 + 桁数 + 1.5個分)) * 何個目)
		pRound->pos = D3DXVECTOR3(((SCREEN_WIDTH / 2) - (ROUND_WIDTH * (DIGIT_ROUND + 1.5))) + ((ROUND_WIDTH * (NUM_ROUND + DIGIT_ROUND + 1)) * nCntScore),
			SCREEN_HEIGHT * 0.5f, 0.0f);
		pRound->bUse = false;
		s_bRoundUse = false;
		s_nUseTime = 0;

		VERTEX_2D*pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点情報へのポインタを取得
		pRound->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDigit = 0; nCntDigit < DIGIT_ROUND; nCntDigit++, pVtx += 4)
		{
			//pVtx.pos.x = (位置.x - 長さ横 + (何個目 * 長さ横 * 桁数))
			pVtx[0].pos = D3DXVECTOR3(pRound->pos.x - ROUND_WIDTH + (nCntDigit * ROUND_WIDTH * DIGIT_ROUND), pRound->pos.y - ROUND_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pRound->pos.x + ROUND_WIDTH + (nCntDigit * ROUND_WIDTH * DIGIT_ROUND), pRound->pos.y - ROUND_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pRound->pos.x - ROUND_WIDTH + (nCntDigit * ROUND_WIDTH * DIGIT_ROUND), pRound->pos.y + ROUND_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pRound->pos.x + ROUND_WIDTH + (nCntDigit * ROUND_WIDTH * DIGIT_ROUND), pRound->pos.y + ROUND_HEIGHT, 0.0f);

			//RHWの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);

			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		//頂点バッファをアンロックする
		pRound->pVtxBuff->Unlock();
	}
}

//=========================================
// 終了
//=========================================
void UninitRound(void)
{
}

//=========================================
// 更新
//=========================================
void UpdateRound(void)
{
	if (s_bRoundUse)
	{
		s_nUseTime++;

		if (s_nUseTime >= 120)
		{
			s_nUseTime = 0;
			s_bRoundUse = false;
		}
	}
}

//=========================================
// 描画
//=========================================
void DrawRound(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	Round* pRound = s_aRound;

	for (int nCntRound = 0; nCntRound < NUM_ROUND; nCntRound++, pRound++)
	{
		if (!s_bRoundUse)
		{
			continue;
		}

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pRound->pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		for (int nCntDigit = 0; nCntDigit < DIGIT_ROUND; nCntDigit++)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntDigit * 4,				//描画する最初の頂点インデックス
				2);							//プリミティブ（ポリゴン）の数
		}
	}
}

//=========================================
// 設定
//=========================================
void SetRound(void)
{
	SCORE* pScore = GetScore();
	Round* pRound = s_aRound;
	s_bRoundUse = true;

	for (int i = 0; i < NUM_ROUND; i++, pScore++, pRound++)
	{
	}
}

//=========================================
// 破壊
//=========================================
void DestroyRound(void)
{
	Round* pRound = s_aRound;
}
