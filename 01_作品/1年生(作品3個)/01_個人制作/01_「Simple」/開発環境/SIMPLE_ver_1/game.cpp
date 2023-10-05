//============================================================================
//
//ゲーム画面の処理[game.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "player.h"			//作成したplayer.hをインクルードする
#include "input.h"			//作成したinput.hをインクルードする
#include "background.h"		//作成したbackground.hをインクルードする
#include "score.h"			//作成したscore.hをインクルードする
#include "title.h"			//作成したtitle.hをインクルードする
#include "game.h"			//作成したgame.hをインクルードする
#include "result.h"			//作成したresult.hをインクルードする
#include "fade.h"			//作成したfade.hをインクルードする
#include "block.h"		//作成したblock.hをインクルードする
#include "item.h"		//作成したitem.hをインクルードする
#include "bullet.h"		//作成したbullet.hをインクルードする
#include "explosion.h"	//作成したexplosion.hをインクルードする
#include "enemy.h"			//作成したenemy.hをインクルードする
#include "life.h"		//作成したlife.hをインクルードする
#include "timer.h"		//作成したtimer,hをインクルードする
#include "arrow.h"		//作成したlife.hをインクルードする
#include "white.h"		//作成したwhite.hをインクルードする
#include "sound.h"
#include <time.h>		//タイム関数を使用するために必要
#include <stdlib.h>		//ランダム関数やsystem("cls")を使用するために必要

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGame = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;		//頂点バッファへのポインタ
Game g_aGame;										//ゲーム画面の情報

GAMESTATE g_gameState = GAMESTATE_NONE;				//ゲームの状態
int g_nCounterGameState = 0;						//状態管理カウンター
													//ブロック情報のデータ
BLOCKINFO g_aBlockInfo[] =
{
	{ D3DXVECTOR3(0.0f, 0.0f, 0.0f),BLOCK_WIDTH ,BLOCK_HEIGHT }
};

//============================================================================
//ゲーム画面の初期化処理
//============================================================================
void InitGame(void)
{
	g_gameState = GAMESTATE_NORMAL;	//通常状態にする

	InitArrow();				//矢印の初期化処理
	InitItem();					//アイテムの初期化処理
	InitEnemy();				//敵の初期化処理
	InitPlayer();				//プレイヤーの初期化処理
	InitBg();					//背景の初期化処理
	InitBlock();				//ブロックの初期化処理
	InitScore();				//点数の初期化処理
	InitBullet();				//弾の初期化処理
	InitExplosion();			//爆発の初期化処理
	InitLife();					//ライフの初期化処理
	InitTimer();				//タイマーの初期化処理
	InitWhite();
	
	int nCntSetBlock = 0;
	int nSetDataBlock = 0;
	float nHeightBlock = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ブロックの処理(中央)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	nSetDataBlock = 4;		//セットする個数
	nHeightBlock = 9.0f;	//セットするブロック位置の高さ
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_MOVE, BLOCK_SIDE_C, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 8;		//セットする個数
	nHeightBlock = 5.0f;	//セットするブロック位置の高さ
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_EVADE, BLOCK_SIDE_C, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ブロックの処理(左側)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	nSetDataBlock = 10;		//セットする個数
	nHeightBlock = 3.0f;	//セットするブロック位置の高さ
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2) ,0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_L,nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 8;		//セットする個数
	nHeightBlock = 7.0f;	//セットするブロック位置の高さ
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_L, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 10;		//セットする個数
	nHeightBlock = 11.0f;	//セットするブロック位置の高さ
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_L, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ブロックの処理(右側)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	nSetDataBlock = 10;		//セットする個数
	nHeightBlock = 3.0f;	//セットするブロック位置の高さ
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_R, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 8;		//セットする個数
	nHeightBlock = 7.0f;	//セットするブロック位置の高さ
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_R, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

	nSetDataBlock = 10;		//セットする個数
	nHeightBlock = 11.0f;	//セットするブロック位置の高さ
	SetBlock(D3DXVECTOR3(0.0f, SCREEN_HEIGHT - (BLOCK_HEIGHT * nHeightBlock) - (BLOCK_HEIGHT / 2), 0.0f),
		BLOCK_TYPE_NORMAL, BLOCK_SIDE_R, nSetDataBlock, nCntSetBlock);
	nCntSetBlock += nSetDataBlock;

//*******************************************************
//アイテムの処理
//*******************************************************
//コイン
	int nCntSetItem;
	srand((unsigned int)time(0));
	for (nCntSetItem = 0; nCntSetItem < MAX_ITEM; nCntSetItem++)
	{
		SetItem
		(D3DXVECTOR3(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT, 0.0f),
			D3DXVECTOR3(1.0f, 0.0f, 0.0f),
			ITEM_TYPE_COIN, ITEM_WIDTH, ITEM_HEIGHT); //位置,動き,種類,大きさ
	}
	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//敵のリスポーン場所処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	ENEMY *pEnemy;			//敵情報へのポインタ
	pEnemy = GetEnemy();	//敵情報の取得
	int nCntSetEnemy;

	for (nCntSetEnemy = 0; nCntSetEnemy < 4; nCntSetEnemy++)
	{//敵の出現位置設定
		SetEnemy(D3DXVECTOR3(ENEMY_WIDTH + rand() % (SCREEN_WIDTH - ENEMY_WIDTH * 2), ENEMY_HEIGHT + rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT * 2),1.0f), D3DXVECTOR3(ENEMY1_SPEAD, 0.0f, 0.0f), ENEMY_TYPE_1);
		SetEnemy(D3DXVECTOR3(ENEMY_WIDTH + rand() % (SCREEN_WIDTH - ENEMY_WIDTH * 2), ENEMY_HEIGHT + rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT * 2), 1.0f), D3DXVECTOR3(ENEMY2_SPEAD, 0.0f, 0.0f), ENEMY_TYPE_2);
		SetEnemy(D3DXVECTOR3(ENEMY_WIDTH + rand() % (SCREEN_WIDTH - ENEMY_WIDTH * 2), ENEMY_HEIGHT + rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT * 2), 1.0f), D3DXVECTOR3(ENEMY3_SPEAD, 0.0f, 0.0f), ENEMY_TYPE_3);
		SetEnemy(D3DXVECTOR3(ENEMY_WIDTH + rand() % (SCREEN_WIDTH - ENEMY_WIDTH * 2), ENEMY_HEIGHT + rand() % (SCREEN_HEIGHT - ENEMY_HEIGHT * 2), 1.0f), D3DXVECTOR3(ENEMY4_SPEAD, 0.0f, 0.0f), ENEMY_TYPE_4);
	}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//白カウンター設置
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	SetWhite(nCntSetBlock + nCntSetItem + 16);
}

//============================================================================
//ゲーム画面の終了処理
//============================================================================
void UninitGame(void)
{
	UninitArrow();
	UninitItem();		//アイテムの終了処理
	UninitEnemy();		//敵の終了処理
	UninitPlayer();		//プレイヤーオブジェクトの終了処理
	UninitBg();			//背景の終了処理
	UninitBlock();		//ブロックの終了処理
	UninitScore();		//スコアの終了処理
	UninitBullet();		//弾の終了処理
	UninitExplosion();	//爆発の終了処理
	UninitLife();		//ライフの終了処理
	UninitTimer();		//タイマーの終了処理
	UninitWhite();

	//テクスチャの破棄
	if (g_pTextureGame != NULL)
	{
		g_pTextureGame->Release();
		g_pTextureGame = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGame != NULL)
	{
		g_pVtxBuffGame->Release();
		g_pVtxBuffGame = NULL;
	}
}

//============================================================================
//ゲーム画面の更新処理
//============================================================================
void UpdateGame(void)
{
	FADE fade;
	fade = GetFade();

	//UpdateArrow();
	UpdateItem();		//アイテムの更新処理
	UpdateEnemy();		//敵の更新処理
	UpdatePlayer();		//プレイヤーオブジェクトの更新処理
	UpdateBg();			//背景の更新処理
	UpdateBlock();		//ブロックの更新処理
	UpdateScore();		//スコアの更新処理
	UpdateBullet();		//弾の更新処理
	UpdateExplosion();	//爆発の更新処理
	UpdateLife();		//ライフの更新処理
	UpdateTimer();		//タイマーの更新処理
	UpdateWhite();

	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:	//通常状態
		break;
	case GAMESTATE_GAMECLEAR:		//終了状態
  		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;	//何もしていない状態
		}
		SetFade(MODE_RESULT);
		break;
	case GAMESTATE_GAMEOVER:		//終了状態
		g_nCounterGameState++;
		if (g_nCounterGameState >= 60)
		{
			g_gameState = GAMESTATE_NONE;	//何もしていない状態
		}
		SetFade(MODE_GAMEOVER);
		break;
	}
}

//============================================================================
//ゲーム画面の描画処理
//============================================================================
void DrawGame(void)
{
	DrawArrow();
	DrawBg();			//背景の描画処理
	DrawItem();			//アイテムの描画処理
	DrawEnemy();		//敵の描画処理
	DrawPlayer();		//プレイヤーオブジェクトの描画処理
	DrawBlock();		//ブロックの描画処理
	DrawScore();		//スコアの描画処理
	DrawBullet();		//弾の描画処理
	DrawExplosion();	//爆発の描画処理
	DrawLife();			//ライフの描画処理
	DrawTimer();		//タイマーの描画処理
	DrawWhite();
}

//============================================================================
//ゲームの状態設定処理
//============================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterGameState = 0;
}

//============================================================================
//ゲームの状態取得処理
//============================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}