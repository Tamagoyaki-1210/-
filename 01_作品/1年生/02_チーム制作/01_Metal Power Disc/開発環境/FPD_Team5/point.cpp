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
#include "point.h"
#include "score.h"
#include "sound.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define NUM_POINT	(2)
#define DIGIT_POINT	(2)

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture = NULL;			//テクスチャへのポインタ
static Point s_aPoint[NUM_POINT];
static bool s_bPointUse;
static int s_nUseTime;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void DestroyPoint(void);

//=========================================
// 初期化
//=========================================
void InitPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number2.png", &s_pTexture);

	Point* pPoint = s_aPoint;

	for (int nCntScore = 0; nCntScore < NUM_POINT; nCntScore++, pPoint++)
	{
		//頂点バッファの生成
		pDevice->CreateVertexBuffer(
			(sizeof(VERTEX_2D) * 4 * 2),		//4つで画像一個分
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&pPoint->pVtxBuff,
			NULL);

		//pos.x = ((画面半分) - (長さ横 * (桁数 + 2個分))) + ((長さ横 * (表示数 + 桁数 + 1.5個分)) * 何個目)
		pPoint->pos = D3DXVECTOR3(((SCREEN_WIDTH / 2) - (POINT_WIDTH * (DIGIT_POINT + 1.5))) + ((POINT_WIDTH * (NUM_POINT + DIGIT_POINT + 1)) * nCntScore),
			SCREEN_HEIGHT * 0.425f, 0.0f);
		pPoint->bUse = false;
		s_bPointUse = false;
		s_nUseTime = 0;

		VERTEX_2D*pVtx;		//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		pPoint->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntDigit = 0; nCntDigit < DIGIT_POINT; nCntDigit++ , pVtx += 4)
		{
			//pVtx.pos.x = (位置.x - 長さ横 + (何個目 * 長さ横 * 桁数))
			pVtx[0].pos = D3DXVECTOR3(pPoint->pos.x - POINT_WIDTH + (nCntDigit * POINT_WIDTH * DIGIT_POINT), pPoint->pos.y - POINT_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pPoint->pos.x + POINT_WIDTH + (nCntDigit * POINT_WIDTH * DIGIT_POINT), pPoint->pos.y - POINT_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pPoint->pos.x - POINT_WIDTH + (nCntDigit * POINT_WIDTH * DIGIT_POINT), pPoint->pos.y + POINT_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pPoint->pos.x + POINT_WIDTH + (nCntDigit * POINT_WIDTH * DIGIT_POINT), pPoint->pos.y + POINT_HEIGHT, 0.0f);

			//RHWの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f);

			//テクスチャの座標設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		}

		//頂点バッファをアンロックする
		pPoint->pVtxBuff->Unlock();
	}
}

//=========================================
// 終了
//=========================================
void UninitPoint(void)
{
}

//=========================================
// 更新
//=========================================
void UpdatePoint(void)
{
	if (s_bPointUse)
	{
		s_nUseTime++;

		if (s_nUseTime >= 120)
		{
			s_nUseTime = 0;
			s_bPointUse = false;
		}
	}
}

//=========================================
// 描画
//=========================================
void DrawPoint(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	Point* pPoint = s_aPoint;

	for (int nCntPoint = 0; nCntPoint < NUM_POINT; nCntPoint++, pPoint++)
	{
		if (!s_bPointUse)
		{
			continue;
		}

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pPoint->pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		for (int nCntDigit = 0; nCntDigit < DIGIT_POINT; nCntDigit++)
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
void SetPoint(void)
{
	SCORE* pScore = GetScore();
	Point* pPoint = s_aPoint;
	s_bPointUse = true;

	PlaySound(SOUND_LABEL_SE_POINT);

	for (int i = 0; i < NUM_POINT; i++, pScore++, pPoint++)
	{
		pPoint->nScore = pScore->nScore;

		int aNumDigit = pPoint->nScore;

		pPoint->col = D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f);

		VERTEX_2D*pVtx;		//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		pPoint->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャの座標設定
		int aData = aNumDigit * 0.1f;
		pVtx[0].tex = D3DXVECTOR2(aData % 10 * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aData % 10 + 1) * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aData % 10 * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aData % 10 + 1) * 0.1f, 1.0f);

		pVtx[4].tex = D3DXVECTOR2(aNumDigit % 10 * 0.1f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2((aNumDigit % 10 + 1) * 0.1f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(aNumDigit % 10 * 0.1f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2((aNumDigit % 10 + 1) * 0.1f, 1.0f);

		//頂点バッファをアンロックする
		pPoint->pVtxBuff->Unlock();
	}
}

//=========================================
// 設定
//=========================================
void SetPointRound(int* nIdx)
{
	Point* pPoint = s_aPoint;
	s_bPointUse = true;

	for (int i = 0; i < NUM_POINT; i++, pPoint++)
	{
		pPoint->nScore = nIdx[i];

		int aNumDigit = nIdx[i];

		VERTEX_2D*pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点情報へのポインタを取得
		pPoint->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャの座標設定
		int aData = aNumDigit * 0.1f;
		pVtx[0].tex = D3DXVECTOR2(aData % 10 * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aData % 10 + 1) * 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aData % 10 * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aData % 10 + 1) * 0.1f, 1.0f);

		pVtx[4].tex = D3DXVECTOR2(aNumDigit % 10 * 0.1f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2((aNumDigit % 10 + 1) * 0.1f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(aNumDigit % 10 * 0.1f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2((aNumDigit % 10 + 1) * 0.1f, 1.0f);

		//頂点バッファをアンロックする
		pPoint->pVtxBuff->Unlock();
	}
}

//=========================================
// 破壊
//=========================================
void DestroyPoint(void)
{
	Point* pPoint = s_aPoint;
}
