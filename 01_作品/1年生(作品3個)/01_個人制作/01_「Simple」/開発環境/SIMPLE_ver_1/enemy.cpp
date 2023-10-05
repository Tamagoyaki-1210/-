//============================================================================
//
//敵の処理[enemy.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "enemy.h"			//作成したenemy.hをインクルードする
#include "bullet.h"			//作成したbullet.hをインクルードする
#include "explosion.h"		//作成したexplosion.hをインクルードする
#include "score.h"			//作成したscore.hをインクルードする
#include "player.h"			//作成したplayer.hをインクルードする
#include "game.h"			//作成したgame.hをインクルードする
#include "arrow.h"		//作成したlife.hをインクルードする
#include "sound.h"

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEnemy[NUM_ENEMY] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
ENEMY g_aEnemy[MAX_ENEMY];	//敵の情報
int g_nDown;				//敵の撃破数
int g_nPop;					//敵の出現数
float g_fLengthEnemy;		//対角線の長さ
float g_fAngleEnemy;		//対角線の角度

//============================================================================
//敵の初期化処理
//============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//敵の対角線の長さを算出する
	g_fLengthEnemy = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) / 2.0f;

	//敵の対角線の角度を算出する
	g_fAngleEnemy = atan2f(ENEMY_WIDTH, ENEMY_HEIGHT);

	g_nDown = 0;
	g_nPop = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ敵1の読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\enemy000.png",			//テクスチャのファイル名
		&g_pTextureEnemy[ENEMY_TYPE_1]
	);
	//テクスチャ敵2の読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\enemy001.png",			//テクスチャのファイル名
		&g_pTextureEnemy[ENEMY_TYPE_2]
	);
	//テクスチャ敵3の読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\enemy002.png",			//テクスチャのファイル名
		&g_pTextureEnemy[ENEMY_TYPE_3]
	);
	//テクスチャ敵4の読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\enemy003.png",			//テクスチャのファイル名
		&g_pTextureEnemy[ENEMY_TYPE_4]
	);

	//移動床テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\white_screen.jpg",			//テクスチャのファイル名
		&g_pTextureEnemy[ENEMY_TYPE_WHITE]
	);

	//全敵の情報の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//構造体の初期化処理
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].type = ENEMY_TYPE_1;
		g_aEnemy[nCntEnemy].state = ENEMY_STATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nLife = MAX_ENEMY_LIFE;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bMove = false;
		g_aEnemy[nCntEnemy].bHide = false;
		g_aEnemy[nCntEnemy].pBlock = NULL;							//ブロック情報にNULLを代入
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);
	//頂点情報の設定
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
		pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
		pVtx[0].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

		pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
		pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
		pVtx[1].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

		pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
		pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
		pVtx[2].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

		pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
		pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
		pVtx[3].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

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

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//============================================================================
//敵の終了処理
//============================================================================
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		//全敵のテクスチャ破棄
		if (g_pTextureEnemy[nCntEnemy] != NULL)
		{
			g_pTextureEnemy[nCntEnemy]->Release();
			g_pTextureEnemy[nCntEnemy] = NULL;
		}

		//頂点バッファの破棄
		if (g_pVtxBuffEnemy != NULL)
		{
			g_pVtxBuffEnemy->Release();
			g_pVtxBuffEnemy = NULL;
		}
	}
}

//============================================================================
//敵の更新処理
//============================================================================
void UpdateEnemy(void) 
{
	int nCntEnemy;
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//プレイヤーと衝突した時の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Player *player;	//プレイヤー情報へのポインタ
	//プレイヤー情報の取得
	player = GetPlayer();

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵キャラが使用されている場合の処理

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//敵の隠れる処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (ARROW_WIDTH * 1.5>= g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2
				|| SCREEN_WIDTH - ARROW_WIDTH * 1.5 <= g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH)
			{
				if (ARROW_HEIGHT * 1.5 >= g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT / 2
					|| SCREEN_HEIGHT - ARROW_HEIGHT * 1.5 <= g_aEnemy[nCntEnemy].pos.y)
				{
					g_aEnemy[nCntEnemy].bHide = true;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//敵のワープ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
					if (g_aEnemy[nCntEnemy].pos.x <= -ENEMY_WIDTH)
					{
						g_aEnemy[nCntEnemy].pos.y = ARROW_HEIGHT * 2 - 1;
						g_aEnemy[nCntEnemy].posOld.y = ARROW_HEIGHT * 2 - 3;
					}
					else if (g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH + ENEMY_WIDTH)
					{
						g_aEnemy[nCntEnemy].pos.y = ARROW_HEIGHT * 2 - 1;
						g_aEnemy[nCntEnemy].posOld.y = ARROW_HEIGHT * 2 - 3;
					}
				}
			}
			else
			{
				g_aEnemy[nCntEnemy].bHide = false;
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//敵のプレイヤー接触時処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].state != ENEMY_STATE_WHITE && g_aEnemy[nCntEnemy].bHide == false)
			{
				if (player->pos.x + PLAYER_WIDTH / 2 >= g_aEnemy[nCntEnemy].pos.x - ENEMY_WIDTH / 2
					&& player->pos.x - PLAYER_WIDTH / 2 <= g_aEnemy[nCntEnemy].pos.x + ENEMY_WIDTH / 2
					&& player->pos.y - PLAYER_HEIGHT <= g_aEnemy[nCntEnemy].pos.y
					&& player->pos.y >= g_aEnemy[nCntEnemy].pos.y - ENEMY_HEIGHT)
				{
					if (player->state != PLAYERSTATE_DAMAGE)
					{
						HitPlayer(1);
					}
				}
			}
			if (g_aEnemy[nCntEnemy].bMove == false)
			{//敵が動けない場合の処理
				g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ブロック当たり判定の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			BLOCK_COLLISION blockCollision;

			blockCollision = (ColisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move,
				ENEMY_WIDTH, ENEMY_HEIGHT, &g_aEnemy[nCntEnemy].pBlock));
			{//ブロックに当たった場合の処理
				if (blockCollision == BLOCK_COLLISION_TOP)
				{
					if (g_aEnemy[nCntEnemy].pBlock != NULL)
					{//ブロックが動いている場合、プレイヤーに慣性を入れる処理
						g_aEnemy[nCntEnemy].pos.y = g_aEnemy[nCntEnemy].pBlock->pos.y - g_aEnemy[nCntEnemy].pBlock->fHeight / 2;
						g_aEnemy[nCntEnemy].move.y = 0;
					}
				}
				if (blockCollision == BLOCK_COLLISION_LEFT)
				{//ブロックの左側に当たった場合
					g_aEnemy[nCntEnemy].state = ENEMY_STATE_TRUN_L;
				}
				else if (blockCollision == BLOCK_COLLISION_RIGHT)
				{//ブロックの右側に当たった場合
					g_aEnemy[nCntEnemy].state = ENEMY_STATE_TRUN_R;
				} 
			}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//敵の移動量反映処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (4 * nCntEnemy);	//頂点データのポインタを4つ分進める

			//前回の位置を保存
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			//移動量反映
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			//敵の重量
			g_aEnemy[nCntEnemy].move.y += 0.1f;

			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[0].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[1].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[2].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[3].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//敵が画面外に出た時の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].pos.x <= -ENEMY_WIDTH)
			{
				g_aEnemy[nCntEnemy].pos.x = SCREEN_WIDTH + ENEMY_WIDTH / 2;
				g_aEnemy[nCntEnemy].posOld.x = SCREEN_WIDTH + ENEMY_WIDTH;
			}
			else if(g_aEnemy[nCntEnemy].pos.x >= SCREEN_WIDTH + ENEMY_WIDTH)
			{
				g_aEnemy[nCntEnemy].pos.x = -ENEMY_WIDTH / 2;
				g_aEnemy[nCntEnemy].posOld.x = -ENEMY_WIDTH;
			}
			if (g_aEnemy[nCntEnemy].pos.y >= SCREEN_HEIGHT)
			{
				g_aEnemy[nCntEnemy].pos.y = SCREEN_HEIGHT;
				g_aEnemy[nCntEnemy].move.y = 0.0f;
			}
			
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//敵の状態管理処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			switch (g_aEnemy[nCntEnemy].state)
			{//敵の状態によるそれぞれの処理
			case ENEMY_STATE_NORMAL:
				break;
			case ENEMY_STATE_DAMEGE:
				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].state = ENEMY_STATE_NORMAL;

					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				break;
			case ENEMY_STATE_WHITE:
				g_nDown++;		//敵機終了数を増やす
				g_aEnemy[nCntEnemy].type = ENEMY_TYPE_WHITE;
				g_aEnemy[nCntEnemy].bMove = false;
				break;
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//敵の種類別行動処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			case ENEMY_STATE_TRUN_R:
				if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_1)
				{//敵1の処理()
					g_aEnemy[nCntEnemy].pos.x += ENEMY1_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_2)
				{//敵2の処理()
					g_aEnemy[nCntEnemy].pos.x += ENEMY2_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_3)
				{//敵3の処理()
					g_aEnemy[nCntEnemy].pos.x += ENEMY3_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_4)
				{//敵4の処理()
					g_aEnemy[nCntEnemy].pos.x += ENEMY4_SPEAD;
				}
				break;
			case ENEMY_STATE_TRUN_L:
				if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_1)
				{//敵1の処理()
					g_aEnemy[nCntEnemy].pos.x -= ENEMY1_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_2)
				{//敵2の処理()
					g_aEnemy[nCntEnemy].pos.x -= ENEMY2_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_3)
				{//敵3の処理()
					g_aEnemy[nCntEnemy].pos.x -= ENEMY3_SPEAD;
				}
				else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_4)
				{//敵4の処理()
					g_aEnemy[nCntEnemy].pos.x -= ENEMY4_SPEAD;
				}
				break;
			}
			//頂点バッファをアンロックする
			g_pVtxBuffEnemy->Unlock();
		}
	}
	if (g_nDown == MAX_ENEMY)
	{//敵が全員消えた時の処理
		//SetGameState(GAMESTATE_END);
		g_nDown = 0;
	}
}

//============================================================================
//敵の描画処理
//============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffEnemy,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{//敵が使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemy[g_aEnemy[nCntEnemy].type]);
			//ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntEnemy * 4,			//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}
}

//============================================================================
////敵の設定処理
//============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE type)
{
	int nCntEnemy;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//敵が撃破された場合

			//敵の情報の設定
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].move = move;
			g_aEnemy[nCntEnemy].type = type;
			g_aEnemy[nCntEnemy].state = ENEMY_STATE_NORMAL;
			g_aEnemy[nCntEnemy].nCounterState = 0;
			g_aEnemy[nCntEnemy].bUse = true;
			g_aEnemy[nCntEnemy].bMove = true;
			g_aEnemy[nCntEnemy].nLife = MAX_ENEMY_LIFE;

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (4 * nCntEnemy);	//頂点データのポインタを4つ分進める

			//頂点座標の設定
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[0].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[1].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - sinf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy - g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[2].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.x) * g_fLengthEnemy;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(0.0f + g_fAngleEnemy + g_aEnemy[nCntEnemy].rot.y) * g_fLengthEnemy - ENEMY_HEIGHT / 2;
			pVtx[3].pos.z = g_aEnemy[nCntEnemy].pos.z + 0.0f;
			break;

			//頂点バッファをアンロックする
			g_pVtxBuffEnemy->Unlock();
		}
	}
}

//============================================================================
////敵の取得処理
//============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];	//敵情報の先頭アドレスを返す
}

//============================================================================
////敵の被弾処理
//============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	//弾が当たった時、敵にダメージ
	g_aEnemy[nCntEnemy].nLife -= nDamage;
	
	if (g_aEnemy[nCntEnemy].nLife > 0)
	{//敵のライフが0より上の場合の処理
		g_aEnemy[nCntEnemy].state = ENEMY_STATE_DAMEGE;
	}
	else
	{//体力が0以下の場合
		g_aEnemy[nCntEnemy].state = ENEMY_STATE_WHITE;

		if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_1)
		{
			AddScore(200);
		}
		else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_2)
		{
			AddScore(400);
		}
		else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_3)
		{
			AddScore(600);
		}
		else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_4)
		{
			AddScore(1200);
		}
	}
}