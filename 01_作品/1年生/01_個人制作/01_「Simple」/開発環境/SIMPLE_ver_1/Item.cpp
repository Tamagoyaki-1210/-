//============================================================================
//
//アイテムの処理[item.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "player.h"		//作成したplayer.hをインクルードする
#include "item.h"		//作成したitem.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "score.h"		//作成したscore.hをインクルードする
#include "game.h"		//作成したgame.hをインクルードする
#include "fade.h"		//作成したfade.hをインクルードする
#include "white.h"		//作成したwhite.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureItem[NUM_ITEM] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		//頂点バッファへのポインタ
ITEM g_aItem[MAX_ITEM];								//アイテムの情報
int nDeleteCounter;									//削除したアイテムの個数をカウント

//============================================================================
//アイテムの初期化処理
//============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	//デバイスの取得
	pDevice = GetDevice();

	//通常アイテムのテクスチャ読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\coin000.png",			//テクスチャのファイル名
		&g_pTextureItem[ITEM_TYPE_COIN]
	);

	//通常アイテムのテクスチャ読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\effect001.png",			//テクスチャのファイル名
		&g_pTextureItem[ITEM_TYPE_EFFECT]
	);
	//移動床テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//テクスチャのファイル名
		&g_pTextureItem[ITEM_TYPE_WHITE]
	);

	//アイテムの情報の初期化
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//構造体の初期化処理
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化処理
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期化処理
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きを初期化する
		g_aItem[nCntItem].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//アイテムの初期配色
		g_aItem[nCntItem].setpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//設置位置の初期化処理
		g_aItem[nCntItem].nCounterAnim = 0;
		g_aItem[nCntItem].nCounterAnimBreak = 0;
		g_aItem[nCntItem].nPatternAnim = 0;
		g_aItem[nCntItem].nDeleteItemCounter = 0;					//アイテムが消える秒数
		g_aItem[nCntItem].type = ITEM_TYPE_COIN;
		g_aItem[nCntItem].fWidth = ITEM_WIDTH;						//幅
		g_aItem[nCntItem].fHeight = ITEM_HEIGHT;					//高さ
		g_aItem[nCntItem].fAngle = atan2f(ITEM_WIDTH, ITEM_HEIGHT);
		g_aItem[nCntItem].fLength = sqrtf(ITEM_WIDTH * ITEM_WIDTH + ITEM_HEIGHT * ITEM_HEIGHT) / 2.0f;
		g_aItem[nCntItem].bUse = false;								//アイテムの使用判定
		g_aItem[nCntItem].pBlock = NULL;							//ブロック情報にNULLを代入
		g_aItem[nCntItem].state = ITEM_STATE_NORMAL;
	}
	nDeleteCounter = 0;								//削除したアイテムの個数をカウント

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,	//sizeof(VERTEX_2D) * 必要な頂点数 * MAX_ITEM
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
		pVtx[0].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
		pVtx[0].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

		pVtx[1].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
		pVtx[1].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
		pVtx[1].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

		pVtx[2].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
		pVtx[2].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
		pVtx[2].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

		pVtx[3].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
		pVtx[3].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
		pVtx[3].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

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
		pVtx[0].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 0.0f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 1.0f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 0.0f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 1.0f), 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//============================================================================
//アイテムの終了処理
//============================================================================
void UninitItem(void)
{
	int nCntItemType;	//アイテムの種類をカウントする

	for (nCntItemType = 0; nCntItemType < NUM_ITEM; nCntItemType++)
	{
		//テクスチャの破棄
		if (g_pTextureItem[nCntItemType] != NULL)
		{
			g_pTextureItem[nCntItemType]->Release();
			g_pTextureItem[nCntItemType] = NULL;
		}
		//頂点バッファの破棄
		if (g_pVtxBuffItem != NULL)
		{
			g_pVtxBuffItem->Release();
			g_pVtxBuffItem = NULL;
		}
	}
}

//============================================================================
//アイテムの更新処理
//============================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	int nCntItem;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{//アイテムが使用されている場合

			//前回の位置を保存
			g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;

			//位置を更新
			g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;
			g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

			//アイテムの重力
			g_aItem[nCntItem].move.y += 1.5f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//外面外ループ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth / 2 < -ITEM_WIDTH / 2)
			{//コインが左側の画面外に出た場合
				g_aItem[nCntItem].pos.x = SCREEN_WIDTH + ITEM_WIDTH / 2;	//画面右側に移動
				g_aItem[nCntItem].posOld.x = SCREEN_WIDTH + ITEM_WIDTH;		//画面右側の少し先に前回位置を設定
			}
			else if (g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth / 2 > SCREEN_WIDTH + ITEM_WIDTH / 2)
			{//コインが右側の画面外に出た場合1
				g_aItem[nCntItem].pos.x = -ITEM_WIDTH / 2;					//画面左側に移動
				g_aItem[nCntItem].posOld.x = -ITEM_WIDTH;					//画面左側の少し先に前回位置を設定
			}
			if (g_aItem[nCntItem].pos.y >= SCREEN_HEIGHT)
			{//コインが画面下にある場合
				g_aItem[nCntItem].pos.y = SCREEN_HEIGHT;					//画面下側に固定
				g_aItem[nCntItem].move.y = 0.0f;
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ブロック当たり判定の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			BLOCK_COLLISION blockCollision;
			g_aItem[nCntItem].pBlock = NULL;

			blockCollision = (ColisionBlock(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, &g_aItem[nCntItem].move, 
				g_aItem[nCntItem].fWidth, g_aItem[nCntItem].fHeight, &g_aItem[nCntItem].pBlock));
			{//ブロックに当たった場合の処理
				if (blockCollision == BLOCK_COLLISION_TOP)
				{
					if (g_aItem[nCntItem].pBlock != NULL)
					{//ブロックが動いている場合、プレイヤーに慣性を入れる処理
						g_aItem[nCntItem].pos.y = g_aItem[nCntItem].pBlock->pos.y - g_aItem[nCntItem].pBlock->fHeight / 2;
						g_aItem[nCntItem].move.y = 0;
					}
				}
				else if (blockCollision != BLOCK_COLLISION_TOP)
				{
					g_aItem[nCntItem].pos.y += 1.0f;
				}
				if (blockCollision == BLOCK_COLLISION_LEFT)
				{//アイテムが右に動いている場合
					g_aItem[nCntItem].state = ITEM_STATE_TRUN_L;
				}
				else if(blockCollision == BLOCK_COLLISION_RIGHT)
				{//アイテムが左に動いている場合
					g_aItem[nCntItem].state = ITEM_STATE_TRUN_R;
				}
			}

			//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
			pVtx[0].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[0].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[0].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[1].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[1].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[1].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[2].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[2].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[2].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[3].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[3].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[3].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//アニメーション処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			g_aItem[nCntItem].nCounterAnimBreak++;
			g_aItem[nCntItem].nPatternAnim++;

			if (g_aItem[nCntItem].type == ITEM_TYPE_COIN)
			{//コインの場合
				if (g_aItem[nCntItem].nCounterAnimBreak == COIN_MAX_ANIMA * 2)
				{//アニメーションを遅くする処理
					g_aItem[nCntItem].nCounterAnim++;
					g_aItem[nCntItem].nCounterAnimBreak = 0;
				}
				if (g_aItem[nCntItem].nPatternAnim == COIN_MAX_ANIMA)
				{//アニメパターンが最大まで来た場合
					g_aItem[nCntItem].nPatternAnim = 0;
				}

				//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
				pVtx[0].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 0.0f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 1.0f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 0.0f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((1.0f / COIN_ANIMX) * (g_aItem[nCntItem].nCounterAnim % COIN_MAX_ANIMA + 1.0f), 1.0f);
			}
			else if (g_aItem[nCntItem].type == ITEM_TYPE_EFFECT)
			{//エフェクトの場合
				if (g_aItem[nCntItem].nCounterAnimBreak == EFFECT_MAX_ANIMA / 2)
				{//アニメーションを遅くする処理
					g_aItem[nCntItem].nCounterAnim++;
					g_aItem[nCntItem].nCounterAnimBreak = 0;
				}
				if (g_aItem[nCntItem].nPatternAnim == EFFECT_MAX_ANIMA)
				{//アニメパターンが最大まで来た場合
					g_aItem[nCntItem].nPatternAnim = 0;
				}
				//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
				pVtx[0].tex = D3DXVECTOR2((1.0f / EFFECT_ANIMX) * (g_aItem[nCntItem].nCounterAnim % EFFECT_MAX_ANIMA + 0.0f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((1.0f / EFFECT_ANIMX) * (g_aItem[nCntItem].nCounterAnim % EFFECT_MAX_ANIMA + 1.0f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2((1.0f / EFFECT_ANIMX) * (g_aItem[nCntItem].nCounterAnim % EFFECT_MAX_ANIMA + 0.0f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((1.0f / EFFECT_ANIMX) * (g_aItem[nCntItem].nCounterAnim % EFFECT_MAX_ANIMA + 1.0f), 1.0f);
			}

			if (g_aItem[nCntItem].type == ITEM_TYPE_WHITE)
			{
				g_aItem[nCntItem].move.x = 0.0f;
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//アイテムの状態遷移処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			switch (g_aItem[nCntItem].state)
			{//アイテム状態によるそれぞれの処理
			case ITEM_STATE_NORMAL:
				g_aItem[nCntItem].bUse = true;		//使用状態に真を返す
				break;
			case ITEM_STATE_TRUN_R:		
				g_aItem[nCntItem].move.x = 1.0f;
				break;
			case ITEM_STATE_TRUN_L:
				g_aItem[nCntItem].move.x = -1.0f;
				break;
			case ITEM_STATE_DELETE:
				//アイテムが消える秒数カウント
				g_aItem[nCntItem].nDeleteItemCounter++;
				g_aItem[nCntItem].type = ITEM_TYPE_EFFECT;

				if (g_aItem[nCntItem].nDeleteItemCounter == 20)
				{//指定の秒数になった場合
					DeleteItem(nCntItem);
					AddWhite(1);
				}
				break;
			}
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();

	FADE fade;
	fade = GetFade();

	if (nDeleteCounter == MAX_ITEM)
	{//アイテムを全て取った場合ゲームクリア
		if (fade == FADE_NONE)
		{//フェードが行われてない場合の処理
			//SetGameState(GAMESTATE_END);
		}
	}
}

//============================================================================
//アイテムの描画処理
//============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntItem;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffItem,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{//アイテムが使用されている 
		 //テクスチャの設定
			pDevice->SetTexture(0, g_pTextureItem[g_aItem[nCntItem].type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntItem * 4,			//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}
}

//============================================================================
//アイテムの設定処理
//============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEM_TYPE type, float Width, float Height)
{
	int nCntItem;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{//アイテムが使用されていない場合
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].move = move;
			g_aItem[nCntItem].setpos = g_aItem[nCntItem].pos;
			g_aItem[nCntItem].bUse = true;				//使用状態をtrueにする
			g_aItem[nCntItem].type = type;
			g_aItem[nCntItem].fWidth = Width;
			g_aItem[nCntItem].fHeight = Height;
			g_aItem[nCntItem].fAngle = atan2f(Width, Height);
			g_aItem[nCntItem].fLength = sqrtf(Width * Width + Height * Height) / 2.0f;
			
			//コインの処理
			if (g_aItem[nCntItem].type == ITEM_TYPE_COIN)
			{
			 //テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f / 4, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f / 4, 1.0f);
			}

			//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
			pVtx[0].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[0].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[0].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[1].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[1].pos.y = g_aItem[nCntItem].pos.y - cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[1].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[2].pos.x = g_aItem[nCntItem].pos.x - sinf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[2].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle - g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[2].pos.z = g_aItem[nCntItem].pos.z + 0.0f;

			pVtx[3].pos.x = g_aItem[nCntItem].pos.x + sinf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.x) * g_aItem[nCntItem].fLength;
			pVtx[3].pos.y = g_aItem[nCntItem].pos.y + cosf(g_aItem[nCntItem].fAngle + g_aItem[nCntItem].rot.y) * g_aItem[nCntItem].fLength - g_aItem[nCntItem].fHeight / 2;
			pVtx[3].pos.z = g_aItem[nCntItem].pos.z + 0.0f;
			break;		//for文を抜ける処理
		}
		pVtx += 4;		//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//============================================================================
//アイテムの当たり判定
//============================================================================
void ColisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	ITEM *pItem;
	int nCntItem;
	pItem = GetItem();

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].type != ITEM_TYPE_WHITE)
		{//アイテムが白くなっていない場合
			if (pItem->bUse == true)
			{//プレイヤーとアイテムの当たり判定
				if (pPos->y >= pItem->pos.y - pItem->fHeight
					&& pPos->y - fHeight <= pItem->pos.y
					&& pPos->x + fWidth >= pItem->pos.x + pItem->fWidth / 2
					&& pPos->x - fWidth <= pItem->pos.x - pItem->fWidth / 2)
				{
					pItem->state = ITEM_STATE_DELETE;
				}
			}
		}
		pItem++;
	}
}

//============================================================================
//アイテムの削除処理
//============================================================================
void DeleteItem(int nCntItem)
{
	if (g_aItem[nCntItem].bUse == true)
	{
		nDeleteCounter++;				//アイテム削除数をカウント
		g_aItem[nCntItem].bUse = false;	//アイテムを消す
		AddScore(100);					//スコアを加算する
	}
}
//============================================================================
//アイテムの取得処理
//============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];	//敵情報の先頭アドレスを返す
}

//============================================================================
//アイテムの被弾処理
//============================================================================
void HitItem(int nCntItem)
{
	g_aItem[nCntItem].type = ITEM_TYPE_WHITE;
}