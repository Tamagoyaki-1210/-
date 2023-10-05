//============================================================================
//
//弾の処理[bullet.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "main.h"		//作成したmain.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "bullet.h"		//作成したbullet.hをインクルードする
#include "explosion.h"	//作成したexplosion.hをインクルードする
#include "block.h"		//作成したblock.hをインクルードする
#include "item.h"		//作成したitem.hをインクルードする
#include "enemy.h"		//作成したenemy.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];						//弾の情報
float g_fLengthBullet;								//対角線の長さ
float g_fAngleBullet;								//対角線の角度

//============================================================================
//弾の初期化処理
//============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\bullet000.png",			//テクスチャのファイル名
		&g_pTextureBullet
	);

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//構造体の初期化処理
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.5f);		//位置の初期化処理
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期化処理
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きを初期化する
		g_aBullet[nCntBullet].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);	//弾の初期配色
		g_aBullet[nCntBullet].nLife = MAX_BULLET_LIFE;					//弾の初期体力
		g_aBullet[nCntBullet].bUse = false;								//弾の使用判定
	}

	//弾の対角線の長さを算出する
	g_fLengthBullet = sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HEIGHT * BULLET_HEIGHT) / 2.0f;

	//弾の対角線の角度を算出する
	g_fAngleBullet = atan2f(BULLET_WIDTH, BULLET_HEIGHT);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BULLET,	//sizeof(VERTEX_2D) * 必要な頂点数 * MAX_BULLET
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,						//頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
		pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
		pVtx[0].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

		pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
		pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
		pVtx[1].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

		pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
		pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
		pVtx[2].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

		pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
		pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
		pVtx[3].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

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
	g_pVtxBuffBullet->Unlock();
}

//============================================================================
//弾の終了処理
//============================================================================
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//============================================================================
//弾の更新処理
//============================================================================
void UpdateBullet(void)
{
	int nCntBullet;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	BLOCK *pBlock;			//ブロック情報へのポインタ
	pBlock = GetBlock();	//ブロック情報の取得
	int nCntBlock;

	ENEMY *pEnemy;			//敵情報へのポインタ
	pEnemy = GetEnemy();	//敵情報の取得
	int nCntEnemy;

	ITEM *pItem;			//敵情報へのポインタ
	pItem = GetItem();		//情報の取得
	int nCntItem;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

			//弾の位置の更新処理
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[0].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[1].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[2].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[3].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//弾が画面外に出た時の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aBullet[nCntBullet].pos.x <= -BULLET_WIDTH / 2)
			{//画面左端まで来た時の処理
				g_aBullet[nCntBullet].pos.x = SCREEN_WIDTH + BULLET_WIDTH / 2;
			}
			else if (g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH + BULLET_WIDTH / 2)
			{//画面右端まで来た時の処理
				g_aBullet[nCntBullet].pos.x = -BULLET_WIDTH / 2;
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//弾の寿命が尽きた時の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//弾の寿命カウントダウン処理
			g_aBullet[nCntBullet].nLife--;

			if(g_aBullet[nCntBullet].nLife <= 0)
			{//弾の寿命が尽きた時の処理
				g_aBullet[nCntBullet].bUse = false;
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//弾がブロックに当たった時の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
			{
				if (pBlock->type != BLOCK_TYPE_WHITE )
				{
					if (pBlock->bUse == true)
					{//ブロックが使用されている場合の処理
						if (g_aBullet[nCntBullet].pos.x - BULLET_WIDTH <= pBlock->pos.x + pBlock->fWidth / 2
							&& g_aBullet[nCntBullet].pos.x + BULLET_WIDTH >= pBlock->pos.x - pBlock->fWidth / 2
							&& g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT < pBlock->pos.y + pBlock->fHeight / 2
							&& g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT > pBlock->pos.y - pBlock->fHeight / 2)
						{//ブロックと爆発が当たった場合の処理
							g_aBullet[nCntBullet].nLife = 0;
						}
					}
				}
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//弾が敵に当たった時の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->type != ENEMY_TYPE_WHITE && pEnemy->bHide == false)
				{
					if (pEnemy->bUse == true)
					{//ブロックが使用されている場合の処理
						if (g_aBullet[nCntBullet].pos.x - BULLET_WIDTH <= pEnemy->pos.x + ENEMY_WIDTH / 2
							&& g_aBullet[nCntBullet].pos.x + BULLET_WIDTH >= pEnemy->pos.x - ENEMY_WIDTH / 2
							&& g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT < pEnemy->pos.y + ENEMY_HEIGHT / 2
							&& g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT > pEnemy->pos.y - ENEMY_HEIGHT / 2)
						{//ブロックと爆発が当たった場合の処理
							g_aBullet[nCntBullet].nLife = 0;
						}
					}
				}
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//弾がアイテムに当たった時の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
			{
				if (pItem->type != ITEM_TYPE_WHITE)
				{
					if (pItem->bUse == true)
					{//ブロックが使用されている場合の処理
						if (g_aBullet[nCntBullet].pos.x - BULLET_WIDTH <= pItem->pos.x + ITEM_WIDTH / 2
							&& g_aBullet[nCntBullet].pos.x + BULLET_WIDTH >= pItem->pos.x - ITEM_WIDTH / 2
							&& g_aBullet[nCntBullet].pos.y - BULLET_HEIGHT < pItem->pos.y + ITEM_HEIGHT / 2
							&& g_aBullet[nCntBullet].pos.y + BULLET_HEIGHT > pItem->pos.y - ITEM_HEIGHT / 2)
						{//ブロックと爆発が当たった場合の処理
							g_aBullet[nCntBullet].nLife = 0;
						}
					}
				}
			}
			
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//============================================================================
//弾の描画処理
//============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	int nCntBullet;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBullet,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている 
			 //ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntBullet * 4,			//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}
}
//============================================================================
////弾の設定処理
//============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	int nCntBullet;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
 	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない場合
			g_aBullet[nCntBullet].pos = pos;

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[0].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[1].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - cosf(0.0f + g_fAngleBullet - g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[2].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - sinf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.x) * g_fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(0.0f + g_fAngleBullet + g_aBullet[nCntBullet].rot.y) * g_fLengthBullet;
			pVtx[3].pos.z = g_aBullet[nCntBullet].pos.z + 0.0f;

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nLife = MAX_BULLET_LIFE;
			g_aBullet[nCntBullet].bUse = true;	//使用状態をtrueにする
			break;								//for文を抜ける処理
		}
		pVtx += 4;								//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}