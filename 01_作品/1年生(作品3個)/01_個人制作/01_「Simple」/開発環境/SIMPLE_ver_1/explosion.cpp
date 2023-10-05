//============================================================================
//
//爆発処理[explosion.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "main.h"		//作成したmain.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "bullet.h"		//作成したbullet.hをインクルードする
#include "explosion.h"	//作成したexplosion.hをインクルードする
#include "block.h"		//作成したblock.hをインクルードする
#include "item.h"		//作成したitem.hをインクルードする
#include "score.h"		//作成したscore.hをインクルードする
#include "enemy.h"			//作成したenemy.hをインクルードする
#include "white.h"		//作成したwhite.hをインクルードする
#include "sound.h"

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffExplosion = NULL;		//頂点バッファへのポインタ
Explosion	g_aExplosion[MAX_EXPLOSION];	//爆発の情報

float g_fLengthExplosion;								//対角線の長さ
float g_fAngleExplosion;								//対角線の角度

//============================================================================
//爆発の初期化処理
//============================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\explosion000.png",		//テクスチャのファイル名
		&g_pTextureExplosion
	);

	//弾の情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//構造体の初期化処理
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
		g_aExplosion[nCntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].bUse = false;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;				//パターンNo.を初期化する
	}

	//弾の対角線の長さを算出する
	g_fLengthExplosion = sqrtf(EXPLOSION_WIDTH * EXPLOSION_WIDTH + EXPLOSION_HEIGHT * EXPLOSION_HEIGHT) / 2.0f;

	//弾の対角線の角度を算出する
	g_fAngleExplosion = atan2f(EXPLOSION_WIDTH, EXPLOSION_HEIGHT);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,	//sizeof(VERTEX_2D) * 必要な頂点数 * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,							//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
		pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y - cosf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
		pVtx[0].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

		pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
		pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
		pVtx[1].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

		pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x - sinf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
		pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y - cosf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
		pVtx[2].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

		pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x - sinf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
		pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
		pVtx[3].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

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

		//テクスチャ座標の設定:X座標 = (1.0f / xパターン数) * (パターンNo. % xパターン数), Y座標 = (1.0f / yパターン数) * (パターンNo. / xパターン数) : int型なので少数点は反映されない
		pVtx[0].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 0.0f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 1.0f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 0.0f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 1.0f), 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//============================================================================
//爆発の終了処理
//============================================================================
void UninitExplosion(void)
{
	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//============================================================================
//爆発の更新処理
//============================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	BLOCK *pBlock;		//ブロック情報へのポインタ
	pBlock = GetBlock();//ブロック情報の取得
	int nCntBlock;

	ENEMY *pEnemy;			//敵情報へのポインタ
	pEnemy = GetEnemy();	//敵情報の取得
	int nCntEnemy;

	ITEM *pItem;			//敵情報へのポインタ
	pItem = GetItem();		//情報の取得
	int nCntItem;

	Player *pPlayer;
	pPlayer = GetPlayer();
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//アニメーションの処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//爆発が使用されている
			g_aExplosion[nCntExplosion].nPatternAnim += 2;			//パターンNo.を更新する

			if (g_aExplosion[nCntExplosion].nPatternAnim >= EXPLOSION_ANIM_CNT * 6)
			{//総パターン数を超えた時の処理
				g_aExplosion[nCntExplosion].nPatternAnim = 0;	//パターンNo.を初期値に戻す

				//使用されていない状態にする
				g_aExplosion[nCntExplosion].bUse = false;
			}

			//テクスチャ座標の設定:X座標 = (1.0f / xパターン数) * (パターンNo. % xパターン数), Y座標 = (1.0f / yパターン数) * (パターンNo. / xパターン数) : int型なので少数点は反映されない
			pVtx[0].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 0.0f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 1.0f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 0.0f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((1.0f / EXPLOSION_ANIM_CNT) * (g_aExplosion[nCntExplosion].nPatternAnim % EXPLOSION_ANIM_CNT + 1.0f), 1.0f);

			if (pPlayer->state != PLAYERSTATE_DEATH)
			{
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//爆発がブロックに当たった時の処理
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
				{
					if (pBlock->bUse == true)
					{//ブロックが使用されている場合の処理
						if (g_aExplosion[nCntExplosion].pos.x - EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 <= pBlock->pos.x + pBlock->fWidth / 2
							&& g_aExplosion[nCntExplosion].pos.x + EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 >= pBlock->pos.x - pBlock->fWidth / 2
							&& g_aExplosion[nCntExplosion].pos.y - EXPLOSION_HEIGHT / 2 < pBlock->pos.y + pBlock->fHeight / 2
							&& g_aExplosion[nCntExplosion].pos.y + EXPLOSION_HEIGHT / 2 > pBlock->pos.y - pBlock->fHeight / 2)
						{//ブロックと爆発が当たった場合の処理
							if (pBlock->type != BLOCK_TYPE_WHITE && pBlock->type != BLOCK_TYPE_NONE)
							{//ブロックが白くなってない場合
								HitBlock(nCntBlock);	//被弾したブロックを返す処理
								AddScore(100);			//スコアを返す処理
								AddWhite(1);
							}
						}
					}
				}
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//爆発が敵に当たった時の処理
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵が使用されている場合の処理
						if (g_aExplosion[nCntExplosion].pos.x - EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 <= pEnemy->pos.x + ENEMY_WIDTH / 2
							&& g_aExplosion[nCntExplosion].pos.x + EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 >= pEnemy->pos.x - ENEMY_WIDTH / 2
							&& g_aExplosion[nCntExplosion].pos.y - EXPLOSION_HEIGHT / 2 < pEnemy->pos.y + ENEMY_HEIGHT / 2
							&& g_aExplosion[nCntExplosion].pos.y + EXPLOSION_HEIGHT / 2 > pEnemy->pos.y - ENEMY_HEIGHT / 2)
						{//敵と爆発が当たった場合の処理
							if (pEnemy->state != ENEMY_STATE_WHITE && pEnemy->bHide == false)
							{//敵が白くなってない場合
								HitEnemy(nCntEnemy, 100);	//被弾したブロックを返す処理
								AddWhite(1);
							}
						}
					}
				}
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				//爆発がアイテムに当たった時の処理
				//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
				{
					if (pItem->bUse == true)
					{//アイテムが使用されている場合の処理
						if (g_aExplosion[nCntExplosion].pos.x - EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 <= pItem->pos.x + pItem->fWidth / 2
							&& g_aExplosion[nCntExplosion].pos.x + EXPLOSION_WIDTH / EXPLOSION_ANIM_CNT * 2 >= pItem->pos.x - pItem->fWidth / 2
							&& g_aExplosion[nCntExplosion].pos.y - EXPLOSION_HEIGHT / 2 < pItem->pos.y + pItem->fHeight / 2
							&& g_aExplosion[nCntExplosion].pos.y + EXPLOSION_HEIGHT / 2 > pItem->pos.y - pItem->fHeight / 2)
						{//アイテムと爆発が当たった場合の処理
							if (pItem->type != ITEM_TYPE_WHITE && pItem->state != ITEM_STATE_DELETE)
							{//アイテムが白くなってない場合
								HitItem(nCntItem);	//被弾したブロックを返す処理
								AddScore(500);		//スコアを返す処理
								AddWhite(1);
							}
						}
					}
				}
			}
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//============================================================================
//爆発の描画処理
//============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntExplosion;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffExplosion,	//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{//弾が使用されている
		 //ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntExplosion * 4,		//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}
}

//============================================================================
////爆発の設定処理
//============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
 	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{//弾が使用されていない場合
			g_aExplosion[nCntExplosion].pos = pos;

			//頂点座標の設定
			pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
			pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y - cosf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
			pVtx[0].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

			pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
			pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
			pVtx[1].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

			pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x - sinf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
			pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y - cosf(0.0f + g_fAngleExplosion - g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
			pVtx[2].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;

			pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x - sinf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.x) * g_fLengthExplosion;
			pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(0.0f + g_fAngleExplosion + g_aExplosion[nCntExplosion].rot.y) * g_fLengthExplosion;
			pVtx[3].pos.z = g_aExplosion[nCntExplosion].pos.z + 0.0f;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;	//パターンNo.を初期化する

			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].bUse = true;		//使用状態をtrueにする
			break;											//for文を抜ける処理
		}
		pVtx += 4;		//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}