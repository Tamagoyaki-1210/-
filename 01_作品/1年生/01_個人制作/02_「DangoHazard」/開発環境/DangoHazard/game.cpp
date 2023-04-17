//============================================================================
//
//ゲーム画面の処理[game.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "input.h"			//作成したinput.hをインクルードする
#include "title.h"			//作成したtitle.hをインクルードする
#include "game.h"			//作成したgame.hをインクルードする
#include "result.h"			//作成したresult.hをインクルードする
#include "fade.h"			//作成したfade.hをインクルードする

#include "timer.h"			//作成したtimer.hをインクルードする
#include "camera.h"			//作成したcamera.hをインクルードする
#include "light.h"			//作成したlight.hをインクルードする
#include "player.h"			//作成したplayer.hをインクルードする
#include "model.h"			//作成したmodel.hをインクルードする
#include "shadow.h"			//作成したshadow.hをインクルードする
#include "wall.h"			//作成したwall.hをインクルードする
#include "billboard.h"		//作成したbillboard.hをインクルードする
#include "meshfield.h"		//作成したmeshfield.hをインクルードする
#include "meshcylinder.h"	//作成したmeshcylinder.hをインクルードする
#include "effect.h"			//作成したeffect.hをインクルードする
#include "enemy.h"			//作成したenemy.hをインクルードする
#include "item.h"			//作成したitem.hをインクルードする
#include "attack.h"			//作成したattack.hをインクルードする
#include "score.h"			//作成したscore.hをインクルードする
#include "life.h"			//作成したlife.hをインクルードする
#include "background.h"	//作成したbackground.hをインクルードする

#include <time.h>			//タイム関数を使用するために必要
#include <stdlib.h>			//ランダム関数やsystem("cls")を使用するために必要
	
//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGame = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;		//頂点バッファへのポインタ
Game g_aGame;										//ゲーム画面の情報

GAMESTATE g_gameState = GAMESTATE_NONE;				//ゲームの状態
int g_nCounterGameState = 0;						//状態管理カウンター
int g_SetItemCounter = 0;
int g_SetEnemyCounter = 0;


//============================================================================
//ゲーム画面の初期化処理
//============================================================================
void InitGame(void)
{	
	//通常状態にする
	g_gameState = GAMESTATE_NORMAL;

	InitMeshField();	//メッシュポリゴンの初期化処理
	InitCamera();		//カメラの初期化処理
	InitLight();		//ライトの初期化処理
	InitShadow();		//影の初期化処理
	InitPlayer();		//プレイヤーの初期化処理
	InitEnemy();		//敵キャラの処理
	InitItem();			//アイテムの処理
	InitModel();		//モデルの初期化処理
	//InitWall();			//壁の初期化処理
	//InitBillBoard();	//ビルボードの初期化処理
	InitAttack();		//攻撃判定の初期化処理
	InitEffect();		//エフェクトの処理
	InitTimer();		//タイマーの初期化処理
	InitScore();		//スコアの初期化処理
	InitLife();			//ライフの初期化処理
	InitBg();			//背景の初期化処理

	srand((unsigned int)time(0));
	
	ENEMY *pEnemy;			//敵情報へのポインタ
	pEnemy = GetEnemy();	//敵情報の取得
}

//============================================================================
//ゲーム画面の終了処理
//============================================================================
void UninitGame(void)
{
	UninitMeshField();		//メッシュポリゴンの終了処理
	UninitCamera();			//カメラの終了処理
	UninitLight();			//ライトの終了処理
	UninitPlayer();			//プレイヤーの終了処理
	UninitEnemy();			//敵キャラの終了処理
	UninitItem();			//アイテムの終了処理
	UninitModel();			//モデルの終了処理
	UninitShadow();			//影の終了処理
	//UninitWall();			//壁の終了処理
	//UninitBillBoard();		//ビルボードの終了処理
	UninitAttack();			//攻撃の終了処理
	UninitEffect();			//エフェクトの終了処理
	UninitTimer();			//タイマーの終了処理
	UninitScore();			//スコアの終了処理
	UninitLife();			//ライフの終了処理
	UninitBg();				//背景の終了処理

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

	UpdateMeshField();		//メッシュポリゴンの更新処理
	UpdateCamera();			//カメラの更新処理
	UpdateLight();			//ライトの更新処理
	UpdatePlayer();			//プレイヤーの更新処理
	UpdateEnemy();			//敵キャラの更新処理
	UpdateItem();			//アイテムの更新処理
	UpdateModel();			//モデルの更新処理
	UpdateShadow();			//影の更新処理
	//UpdateWall();			//壁の更新処理
	//UpdateBillBoard();		//ビルボードの更新処理
	UpdateAttack();			//攻撃判定の更新処理
	UpdateEffect();			//エフェクトの更新処理
	UpdateTimer();			//タイマーの更新処理
	UpdateScore();			//スコアの更新処理
	UpdateLife();			//ライフの更新処理
	UpdateBg();				//背景の更新処理
		
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
//*******************************************************
//アイテムの処理
//*******************************************************
	g_SetItemCounter++;

	if (g_SetItemCounter == 50)
	{
		//位置、角度、種類
		SetItem(D3DXVECTOR3(250.0f - rand() % 500, 50.0f, 250.0f - rand() % 500),
			D3DXVECTOR3(0.5f, -1.0f, 0.0f), ITEM_TYPE_CORN);
		g_SetItemCounter = 0;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//敵のリスポーン場所処理
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	g_SetEnemyCounter++;

	if (g_SetEnemyCounter == 25)
	{
		//位置、角度、種類
		SetEnemy(
			D3DXVECTOR3(250.0f - rand() % 500, 50.0f, 250.0f - rand() % 500),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			//(ENEMY_TYPE)(rand() % IMAGE_ENEMY)
			ENEMY_TYPE_POTATE
		);

		if (rand() % 10 == 0)
		{
			//位置、角度、種類
			SetEnemy(
				D3DXVECTOR3(250.0f - rand() % 500, 50.0f, 250.0f - rand() % 500),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				//(ENEMY_TYPE)(rand() % IMAGE_ENEMY)
				ENEMY_TYPE_PUNPKIN
			);
		}
		g_SetEnemyCounter = 0;
	}
}

//============================================================================
//ゲーム画面の描画処理
//============================================================================
void DrawGame(void)
{
	SetCamera();		//カメラの設定処理
	//DrawWall();			//壁の描画処理
	DrawMeshField();	//メッシュポリゴンの描画処理
	//DrawBillBoard();	//ビルボードの描画処理
	DrawAttack();		//攻撃判定の描画処理
	DrawPlayer();		//プレイヤーの描画処理
	DrawEnemy();		//敵キャラの描画処理
	DrawItem();			//アイテムの描画処理
	DrawModel();		//モデルの描画処理
	DrawShadow();		//影の描画処理
	DrawEffect();		//エフェクトの描画処理
	DrawTimer();		//タイマーの描画処理
	DrawScore();		//スコアの描画処理
	DrawLife();			//ライフの描画処理
	DrawBg();			//背景の描画処理
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