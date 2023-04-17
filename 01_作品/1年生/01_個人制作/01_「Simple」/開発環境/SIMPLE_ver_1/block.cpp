//============================================================================
//
//ブロックの処理[block.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "player.h"		//作成したplayer.hをインクルードする
#include "block.h"		//作成したblock.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "sound.h"

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBlock[NUM_BLOCK] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;			//頂点バッファへのポインタ
BLOCK g_aBlock[MAX_BLOCK];								//ブロックの情報

//============================================================================
//ブロックの初期化処理
//============================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//デバイスの取得
	pDevice = GetDevice();

	//通常床テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block000.jpg",			//テクスチャのファイル名
		&g_pTextureBlock[BLOCK_TYPE_NORMAL]
	);
	//すり抜け床テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block004.jpg",			//テクスチャのファイル名
		&g_pTextureBlock[BLOCK_TYPE_EVADE]
	);
	//移動床テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\block005.jpg",			//テクスチャのファイル名
		&g_pTextureBlock[BLOCK_TYPE_MOVE]
	);
	//移動床テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//テクスチャのファイル名
		&g_pTextureBlock[BLOCK_TYPE_NONE]
	);
	//移動床テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//テクスチャのファイル名
		&g_pTextureBlock[BLOCK_TYPE_WHITE]
	);

	//ブロックの情報の初期化
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//構造体の初期化処理
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化処理
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期化処理
		g_aBlock[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きを初期化する
		g_aBlock[nCntBlock].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//ブロックの初期配色
		g_aBlock[nCntBlock].setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//設置位置の初期化処理
		g_aBlock[nCntBlock].type = BLOCK_TYPE_NORMAL;					//ブロックの種類
		g_aBlock[nCntBlock].state = BLOCK_STATE_NORMAL;					//ブロックの状態
		g_aBlock[nCntBlock].fWidth = BLOCK_WIDTH;						//幅
		g_aBlock[nCntBlock].fHeight = BLOCK_HEIGHT;						//高さ
		g_aBlock[nCntBlock].fAngle = atan2f(BLOCK_WIDTH, BLOCK_HEIGHT);
		g_aBlock[nCntBlock].fLength = sqrtf(BLOCK_WIDTH * BLOCK_WIDTH + BLOCK_HEIGHT * BLOCK_HEIGHT) / 2.0f;
		g_aBlock[nCntBlock].bUse = false;								//ブロックの使用判定
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK,	//sizeof(VERTEX_2D) * 必要な頂点数 * MAX_BLOCK
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
		pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
		pVtx[0].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

		pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
		pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
		pVtx[1].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

		pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
		pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
		pVtx[2].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

		pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
		pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
		pVtx[3].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//============================================================================
//ブロックの終了処理
//============================================================================
void UninitBlock(void)
{
	int nCntBlockType;	//ブロックの種類をカウントする

	for (nCntBlockType = 0; nCntBlockType < NUM_BLOCK; nCntBlockType++)
	{
		//テクスチャの破棄
		if (g_pTextureBlock[nCntBlockType] != NULL)
		{
			g_pTextureBlock[nCntBlockType]->Release();
			g_pTextureBlock[nCntBlockType] = NULL;
		}
		//頂点バッファの破棄
		if (g_pVtxBuffBlock != NULL)
		{
			g_pVtxBuffBlock->Release();
			g_pVtxBuffBlock = NULL;
		}
	}
}

//============================================================================
//ブロックの更新処理
//============================================================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int nCntBlock;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//ブロックが使用されている 
		//前回の位置を保存
			g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;

			//位置を更新
			g_aBlock[nCntBlock].pos.x += g_aBlock[nCntBlock].move.x;
			g_aBlock[nCntBlock].pos.y += g_aBlock[nCntBlock].move.y;

			if (g_aBlock[nCntBlock].type == BLOCK_TYPE_MOVE)
			{//移動ブロックの処理
				if (g_aBlock[nCntBlock].pos.x >= g_aBlock[nCntBlock].setpos.x + 200.0f)
				{//ブロック現在位置が指定の位置(右側)に来た時の処理
					g_aBlock[nCntBlock].move.x -= 1.0f;
				}
				else if (g_aBlock[nCntBlock].pos.x <= g_aBlock[nCntBlock].setpos.x - 200.0f)
				{//ブロック現在位置が指定の位置(左側)に来た時の処理
					g_aBlock[nCntBlock].move.x += 1.0f;
				}
				if (g_aBlock[nCntBlock].pos.y >= g_aBlock[nCntBlock].setpos.y + 100.0f)
				{//ブロック現在位置が指定の位置(上側)に来た時の処理
					g_aBlock[nCntBlock].move.y -= 1.0f;
				}
				else if (g_aBlock[nCntBlock].pos.y <= g_aBlock[nCntBlock].setpos.y - 100.0f)
				{//ブロック現在位置が指定の位置(下側)に来た時の処理
					g_aBlock[nCntBlock].move.y += 1.0f;
				}
			}
			//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[0].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[1].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[2].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[3].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//ブロックの状態管理処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			switch (g_aBlock[nCntBlock].state)
			{//敵の状態によるそれぞれの処理
			case BLOCK_STATE_NORMAL:
				g_aBlock[nCntBlock].bUse = true;
				break;
			case BLOCK_STATE_WHITE:
				g_aBlock[nCntBlock].type = BLOCK_TYPE_WHITE;
				g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			}
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//============================================================================
//ブロックの描画処理
//============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntBlock;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBlock,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//ブロックが使用されている 
		 //テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].type]);

		 //ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntBlock * 4,			//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}
}
//============================================================================
//ブロックの設定処理(set = 設置数),(cur = ポインタ現在位置)
//============================================================================
void SetBlock(D3DXVECTOR3 pos, BLOCK_TYPE type, BLOCK_SIDE side, int set, int cur)
{
	int nCntBlock;
	float fSide;
	VERTEX_2D *pVtx = 0;			//頂点情報へのポインタ

	pVtx += cur * 4;		//ポインタを動かす処理?

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = cur; nCntBlock < set + cur; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{//ブロックが使用されていない場合
			g_aBlock[nCntBlock].bUse = true;
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].type = type;
			g_aBlock[nCntBlock].side = side;
//****************************************************************************
//ブロックサイドごとの処理
//****************************************************************************
			if (g_aBlock[nCntBlock].side == BLOCK_SIDE_C)
			{//中央側ブロック
				fSide = SCREEN_WIDTH / 2 + (BLOCK_WIDTH * (nCntBlock - cur)) - (BLOCK_WIDTH / 2 * set);
			}
			else if (g_aBlock[nCntBlock].side == BLOCK_SIDE_L)
			{//左側ブロック
				fSide = BLOCK_WIDTH / 2 + (BLOCK_WIDTH * (nCntBlock - cur));
			}
			else if (g_aBlock[nCntBlock].side == BLOCK_SIDE_R)
			{//右側ブロック
				fSide = SCREEN_WIDTH - BLOCK_WIDTH / 2 - (BLOCK_WIDTH * (nCntBlock - cur));
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ブロックタイプごとの処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aBlock[nCntBlock].type == BLOCK_TYPE_MOVE)	//通常ブロック
			{
				g_aBlock[nCntBlock].move.x = 1.0f;
			}
			g_aBlock[nCntBlock].pos.x = fSide;
			g_aBlock[nCntBlock].setpos = g_aBlock[nCntBlock].pos;

			//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[0].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y - cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[1].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x - sinf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle - g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[2].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + sinf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.x) * g_aBlock[nCntBlock].fLength;
			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + cosf(g_aBlock[nCntBlock].fAngle + g_aBlock[nCntBlock].rot.y) * g_aBlock[nCntBlock].fLength;
			pVtx[3].pos.z = g_aBlock[nCntBlock].pos.z + 0.0f;
		}
		pVtx += 4;											//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//============================================================================
//ブロックの当たり判定
//============================================================================
BLOCK_COLLISION ColisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight , BLOCK **pBlock)
{
	int nCntBlock;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	BLOCK_COLLISION blockCollision = BLOCK_COLLISION_NONE;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//ブロックが使用されている場合
			//ブロック上側の処理
			if (pPosOld->y <= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2
				&& pPos->y >= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2
				&& g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 < pPos->x + fWidth / 2
				&& g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2 > pPos->x - fWidth / 2)
			{//プレイヤー前回位置の足が、ブロックの頭より高い位置にある場合(P前回位0置.y <= B現在位置.y)
				//プレイヤー現在位置の足がブロック内にめりこんだ場合
				blockCollision = BLOCK_COLLISION_TOP;

				*pBlock = &g_aBlock[nCntBlock];	//ブロックのポインタを入れる処理
			}
			else if (g_aBlock[nCntBlock].type != BLOCK_TYPE_EVADE && g_aBlock[nCntBlock].type != BLOCK_TYPE_WHITE)
			{//すり抜けブロック以外の場合
				//ブロック下側の処理
				if (pPosOld->y - fHeight >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2
					&& pPos->y - fHeight <= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2
					&& g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 < pPos->x + fWidth / 2
					&& g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2 > pPos->x - fWidth / 2)
				{//プレイヤー前回位置の頭が、ブロック現在位置の下より低位置の場合(P前回位置.y >= B現在位置.y)
					//プレイヤーの頭が、ブロック内にめりこんだ場合
					 //プレイヤー現在位置をブロックの下に固定する
					pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2 + fHeight;
					pMove->y = 0;
				}
				//ブロック左側の処理
				else if (pPosOld->x + fWidth / 2 <= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2
					&& pPos->x + fWidth / 2 >= g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2
					&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2
					&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2)
				{//プレイヤー前回位置右側が、ブロック現在位置より左にある場合(P前回位置.x <= B現在位置.x)
				//プレイヤー現在位置右側が、ブロック内にめりこんだ場合
				 //プレイヤー現在位置をブロックの左に固定する
					pPos->x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 - fWidth / 2;
					pMove->x = 0;

					blockCollision = BLOCK_COLLISION_LEFT;
				}
				//ブロック右側の処理
				else if (pPosOld->x - fWidth / 2 >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2
					&& pPos->x - fWidth / 2 <= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2
					&& pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2
					&& pPos->y - fHeight < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2)
				{//プレイヤー前回位置の左側が、ブロックの右側より左にある場合(P前回位置.x >= B現在位置.x)
					//プレイヤー左側が、ブロック内にめりこんだ場合
					//プレイヤー現在位置をブロックの右に固定する
					pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2 + fWidth / 2;
					pMove->x = 0.0f;

					blockCollision = BLOCK_COLLISION_RIGHT;
				}
			}
			if (g_aBlock[nCntBlock].type == BLOCK_TYPE_MOVE)
			{//移動ブロックの処理
			 //縦移動ブロックの処理
				if (g_aBlock[nCntBlock].posOld.y - (g_aBlock[nCntBlock].fHeight / 2) >= pPosOld->y
					&& g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2) <= pPos->y
					&& pPos->x - (fWidth / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2)
					&& pPos->x + (fWidth / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2))
				{//ブロック前回位置の頭が、プレイヤーの足より低位置の場合(B前回位置.y >= P前回位置.y)
				 //ブロックの頭が、プレイヤー内にめりこんだ場合
				 //プレイヤー現在位置をブロックの頭に固定する
					pPos->y = g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2);
					pMove->y = 0.0f;
					blockCollision = BLOCK_COLLISION_TOP;
					*pBlock = &g_aBlock[nCntBlock];	//ブロックのポインタを入れる処理
				}
				if (g_aBlock[nCntBlock].posOld.y + (g_aBlock[nCntBlock].fHeight / 2) <= pPosOld->y - fHeight
					&& g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2) >= pPos->y - fHeight
					&& pPos->x - (fWidth / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2)
					&& pPos->x + (fWidth / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2))
				{//ブロック前回位置の下側が、プレイヤーの頭より高位置の場合(B前回位置.y <= P前回位置.y)
				 //ブロックの下側が、プレイヤー内にめりこんだ場合
				 //プレイヤー現在位置をブロックの下側に固定する
					pPos->y = g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2 + fHeight);
					pMove->y = 0.0f;
				}
				//横移動ブロックの処理
				if (g_aBlock[nCntBlock].posOld.x - (g_aBlock[nCntBlock].fWidth / 2) >= pPosOld->x + fWidth / 2
					&& g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2) <= pPos->x + fWidth / 2
					&& g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2) < pPos->y
					&& g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2) > pPos->y - fHeight)
				{//ブロック前回位置(左側)が、プレイヤー前回位置の右側より左にある場合(B前回位置.x >= P前回位置.x)
				 //ブロックの左側が、プレイヤーにめりこんだ場合
					pPos->x = g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2) - fWidth / 2;
				}
				if (g_aBlock[nCntBlock].posOld.x + (g_aBlock[nCntBlock].fWidth / 2) <= pPosOld->x - fWidth / 2
					&& g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2) >= pPos->x - fWidth / 2
					&& g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2) < pPos->y
					&& g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2) > pPos->y - fHeight)
				{//ブロック前回位置(右側)が、プレイヤー前回位置の左側より右にある場合(B前回位置.x <= P前回位置.x)
				 //ブロックの左側が、プレイヤーにめりこんだ場合
					pPos->x = g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2) + fWidth / 2;
				}
			}
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
	return blockCollision;
}
//============================================================================
//ブロックの取得処理
//============================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];	//ブロック情報の先頭アドレスを返す
}

//============================================================================
//ブロックの被弾処理
//============================================================================
void HitBlock(int nCntBlock)
{
	g_aBlock[nCntBlock].state = BLOCK_STATE_WHITE;
}