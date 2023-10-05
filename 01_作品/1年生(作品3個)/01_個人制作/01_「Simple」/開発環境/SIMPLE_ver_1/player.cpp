//============================================================================
//
//プレイヤー描画の処理player.cpp] (Update:10/11)
//Author:谷本康輔
//============================================================================
#include "player.h"		//作成したplayer.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "block.h"		//作成したblock.hをインクルードする
#include "item.h"		//作成したitem.hをインクルードする
#include "enemy.h"			//作成したenemy.hをインクルードする
#include "score.h"		//作成したscore.hをインクルードする
#include "fade.h"		//作成したfade.hをインクルードする
#include "game.h"		//作成したgame.hをインクルードする
#include "bullet.h"		//作成したbullet.hをインクルードする
#include "explosion.h"	//作成したexplosion.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffPlayer = NULL;	//頂点バッファへのポインタ
Player g_aPlayer;									//プレイヤーの情報

int	g_nJumpCounter;								//ジャンプ受け付け時間
int g_nJumpStock;								//プレイヤーのジャンプ回数
int g_nBulletCollTime;							//弾のクールタイム
int g_nCounterAnimPlayer;
int g_nPatternAnimPlayer;
bool g_bExplosionUse;

//============================================================================
//ポリゴンの初期化処理
//============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ

	pDevice = GetDevice();				//デバイスの取得

	//テクスチャーの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\player000.png",	//テクスチャのファイル名
		&g_pTexturePlayer
	);

	int	g_nJumpCounter = 0;								//ジャンプ受け付け時間
	int g_nJumpStock = 0;								//プレイヤーのジャンプ回数
	int g_nBulletCollTime = 0;							//弾のクールタイム
	bool g_bExplosionUse = true;

	//位置の初期化処理
	g_aPlayer.pos = D3DXVECTOR3(PLAYER_WIDTH / 2, SCREEN_HEIGHT,0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量の初期化処理
	g_aPlayer.moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//弾の移動量の初期化処理
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する
	g_aPlayer.col = D3DXCOLOR(0.0f,0.0f, 0.0f, 0.0f);
	g_aPlayer.state = PLAYERSTATE_NORMAL;					//プレイヤーの状態
	g_aPlayer.Astate = APPEARSTATE_IN;						//プレイヤーの出現状態
	g_aPlayer.nCounterState = 0;
	g_aPlayer.nCounterAppear = 0;
	g_aPlayer.nLife = MAX_PLAYER_LIFE;						//プレイヤーの初期体力
	g_aPlayer.nDirectionMove = 0;							//プレイヤーの方向
	g_aPlayer.fWidth = PLAYER_WIDTH;
	g_aPlayer.fHeight = PLAYER_HEIGHT;
	g_aPlayer.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);
	g_aPlayer.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;
	g_aPlayer.bUse = true;									//プレイヤー使用判定
	g_aPlayer.bAppearUse = false;							//プレイヤーの出現判定
	g_aPlayer.bDisp = true;	
	g_aPlayer.bIsJumping = false;							//ジャンプ使用判定
	g_aPlayer.pBlock = NULL;								//ブロック情報にNULLを代入

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;			

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer -> Lock(0, 0,(void**)&pVtx, 0);

	//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
	pVtx[0].pos.x = g_aPlayer.pos.x - sinf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.x) * g_aPlayer.fLength;
	pVtx[0].pos.y = g_aPlayer.pos.y - cosf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
	pVtx[0].pos.z = g_aPlayer.pos.z + 0.0f;

	pVtx[1].pos.x = g_aPlayer.pos.x + sinf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.x) * g_aPlayer.fLength;
	pVtx[1].pos.y = g_aPlayer.pos.y - cosf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
	pVtx[1].pos.z = g_aPlayer.pos.z + 0.0f;

	pVtx[2].pos.x = g_aPlayer.pos.x - sinf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.x) * g_aPlayer.fLength;
	pVtx[2].pos.y = g_aPlayer.pos.y + cosf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
	pVtx[2].pos.z = g_aPlayer.pos.z + 0.0f;

	pVtx[3].pos.x = g_aPlayer.pos.x + sinf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.x) * g_aPlayer.fLength;
	pVtx[3].pos.y = g_aPlayer.pos.y + cosf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
	pVtx[3].pos.z = g_aPlayer.pos.z + 0.0f;
	
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
	pVtx[0].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
	pVtx[1].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
	pVtx[2].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
	pVtx[3].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer -> Unlock();
}

//============================================================================
//プレイヤーの終了処理
//============================================================================
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer -> Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer -> Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//============================================================================
//プレイヤーの更新処理
//============================================================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	FADE fade;
	fade = GetFade();

	//頂点バッファをロックし、頂点情報へのポインタを取得
 	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aPlayer.nLife < 0)
	{
		g_aPlayer.nLife = 0;
	}

	if (g_aPlayer.bUse == true)
	{//プレイヤーが使用されている場合
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//左右に動かす処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
		{//Aキーが押された時、左移動
				g_aPlayer.move.x -= sinf(D3DX_PI/ 4.0f) * 1.0f;
				g_aPlayer.move.y -= cosf(D3DX_PI/ 4.0f) * 0.0f;

				if (GetKeyboardPress(DIK_B) == true || GetKeyboardPress(DIK_LSHIFT) == true)
				{//Bダッシュで倍速
					g_aPlayer.move.x -= 0.5f;
					g_aPlayer.move.y += 0.0f;
				}
				g_nCounterAnimPlayer++;				//カウンターを加算
				g_aPlayer.nDirectionMove = 1;
		}
		else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
		{//Dキーが押された時、右移動
			g_aPlayer.move.x += sinf(D3DX_PI / 4.0f) * 1.0f;
			g_aPlayer.move.y += cosf(D3DX_PI / 4.0f) * 0.0f;

			if (GetKeyboardPress(DIK_B) == true || GetKeyboardPress(DIK_LSHIFT) == true)
			{//Bダッシュで倍速
				g_aPlayer.move.x += 0.5f;
				g_aPlayer.move.y += 0.0f;
			}

			g_nCounterAnimPlayer++;				//カウンターを加算
			g_aPlayer.nDirectionMove = 0;
		}
		else if(g_aPlayer.bIsJumping == false)
		{//移動0でもアニメーションさせる処理
			g_nPatternAnimPlayer = 0 ;
			pVtx[0].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
			pVtx[1].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
			pVtx[2].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
			pVtx[3].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
		}
		if (GetKeyboardPress(DIK_Q) == true)
		{//Qキーを押した場合、左向きにする処理
			g_aPlayer.nDirectionMove = 1;
		}
		if (GetKeyboardPress(DIK_E) == true)
		{//Eキーを押した場合、右向きにする処理
			g_aPlayer.nDirectionMove = 0;
		}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//キーを押してなくても働き続ける処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (GetKeyboardTrigger(DIK_Z) == true)
		{//Zでスライド噴射
			g_aPlayer.move.x *= 2.0f;
		}
		if (GetKeyboardTrigger(DIK_P) == true)
		{
			//カンストスコアを渡す処理
			AddScore(100000000);
		}
		else if (GetKeyboardTrigger(DIK_O) == true)
		{
			InitScore();		//点数の初期化処理
		}

		//if (GetKeyboardTrigger(DIK_RETURN) == true)
		//{//※デバック・ENTERキーでリザルト
		//	if (fade == FADE_NONE)
		//	{//フェードが行われてない場合の処理
		//		SetGameState(GAMESTATE_GAMECLEAR);
		//	}
		//}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ジャンプの処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_nJumpStock < PLAYER_MAXJUMP)
		{//指定されたジャンプ回数のみジャンプする
			if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
			{//Wキーが押された時 or スペースキーを押した時、上移動
				g_aPlayer.bIsJumping = true;	//ジャンプ使用を真に
				g_nJumpCounter++;			//ジャンプ受付け秒カウント
				if (g_nJumpCounter == 1)
				{//小ジャンプ(ジャンプ回数ごとに減速)
					g_aPlayer.move.y = 0;
					g_aPlayer.move.y -= cosf(D3DX_PI * 1.0f / 4.0f) * 30.0f;
					g_nJumpStock++;
				}
				else if (g_nJumpCounter > 1 && g_nJumpCounter <= 10)
				{//秒数設定可能のジャンプ
					g_aPlayer.move.x -= sinf(D3DX_PI * 1.0f / 4.0f) * 0.0f;
					g_aPlayer.move.y -= cosf(D3DX_PI * 1.0f / 4.0f) * 1.0f;
				}
				if (g_nPatternAnimPlayer == 0 || g_nPatternAnimPlayer == 2)
				{
					g_nPatternAnimPlayer++;
				}
				if (g_nPatternAnimPlayer == 1 || g_nPatternAnimPlayer == 3)
				{
					g_nPatternAnimPlayer += 2;
				}
				pVtx[0].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
				pVtx[1].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
				pVtx[2].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
				pVtx[3].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
			}
			else
			{//キーを離した時の処理
				g_nJumpCounter = 0;
			}
		}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//アニメーション処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_nCounterAnimPlayer >= PLAYER_ANIMATION)
		{//一定時間経過した時の処理
			if (g_nPatternAnimPlayer >= PLAYER_ANIMATION)
			{//総パターン数を超えた
				g_nPatternAnimPlayer = 0;	//パターンNo.を初期値に戻す
			}
			if (g_aPlayer.bIsJumping == false)
			{//地面に着地している場合、アニメを動かす処理
			 //テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)(テクスチャ左向き)
				pVtx[0].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
				pVtx[1].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 0.0f));
				pVtx[2].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 0.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
				pVtx[3].tex = D3DXVECTOR2((1.0f / PLAYER_ANIMX) * (g_nPatternAnimPlayer + 1.0f), (1.0f / PLAYER_ANIMY) * (g_aPlayer.nDirectionMove + 1.0f));
				g_nCounterAnimPlayer = 0;		//カウンターを初期値に戻す
				g_nPatternAnimPlayer++;			//パターンNo.を更新する
			}
		}

		//プレイヤーに重力を発生させる
		g_aPlayer.move.y += 1.5f;
		
		//前回の位置を保存
		g_aPlayer.posOld = g_aPlayer.pos;

		//位置を更新
		g_aPlayer.pos.x += g_aPlayer.move.x;
		g_aPlayer.pos.y += g_aPlayer.move.y;

		//移動量を更新(減衰させる)
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.1f;

		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = g_aPlayer.pos.x - sinf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.x) * g_aPlayer.fLength;
		pVtx[0].pos.y = g_aPlayer.pos.y - cosf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
		pVtx[0].pos.z = g_aPlayer.pos.z + 0.0f;

		pVtx[1].pos.x = g_aPlayer.pos.x + sinf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.x) * g_aPlayer.fLength;
		pVtx[1].pos.y = g_aPlayer.pos.y - cosf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
		pVtx[1].pos.z = g_aPlayer.pos.z + 0.0f;

		pVtx[2].pos.x = g_aPlayer.pos.x - sinf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.x) * g_aPlayer.fLength;
		pVtx[2].pos.y = g_aPlayer.pos.y + cosf(0.0f + g_aPlayer.fAngle - g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
		pVtx[2].pos.z = g_aPlayer.pos.z + 0.0f;

		pVtx[3].pos.x = g_aPlayer.pos.x + sinf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.x) * g_aPlayer.fLength;
		pVtx[3].pos.y = g_aPlayer.pos.y + cosf(0.0f + g_aPlayer.fAngle + g_aPlayer.rot.y) * g_aPlayer.fLength - PLAYER_HEIGHT / 2;
		pVtx[3].pos.z = g_aPlayer.pos.z + 0.0f;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//自機の弾の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//弾の移動距離と方向
		g_aPlayer.moveBullet.x = sinf(g_aPlayer.rot.x + g_aPlayer.fAngle) * 16.0f;
		g_aPlayer.moveBullet.y = cosf(g_aPlayer.rot.y + g_aPlayer.fAngle) * 0.0f;
		g_aPlayer.moveBullet.z = 0.0f;

		if (g_nBulletCollTime == 0)
		{
			//ノーマルショット
			if (GetKeyboardTrigger(DIK_SPACE) == true)
			{//SPACEキーが押された
				g_nBulletCollTime = 20;
				if (g_aPlayer.nDirectionMove == 0)
				{//右向きの場合
					if (GetKeyboardPress(DIK_E) == true)
					{//Eキーが押されている場合、右上に弾を打つ
						g_aPlayer.moveBullet.y = cosf(g_aPlayer.rot.y + g_aPlayer.fAngle) * -8.0f;
						SetBullet	//位置,移動量
						(D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth / 2, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), g_aPlayer.moveBullet);
					}
					else
					{
						SetBullet	//位置,移動量
						(D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth / 2, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), g_aPlayer.moveBullet);
					}
				}
				else if (g_aPlayer.nDirectionMove == 1)
				{//左向きの場合
					if (GetKeyboardPress(DIK_Q) == true)
					{//Wキーが押されている場合、左上に弾を打つ
						g_aPlayer.moveBullet.y = cosf(g_aPlayer.rot.y + g_aPlayer.fAngle) * 8.0f;
						SetBullet	//位置,移動量
						(D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth / 2, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), -g_aPlayer.moveBullet);
					}
					else
					{
						SetBullet	//位置,移動量
						(D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth / 2, g_aPlayer.pos.y - g_aPlayer.fHeight / 2, 0.0f), -g_aPlayer.moveBullet);
					}
				}
			}
		}
		else
		{
			g_nBulletCollTime--;
		}
		if (GetKeyboardPress(DIK_N) == true && g_aPlayer.fLength > PLAYER_WIDTH)
		{//Nキーで縮小
			g_aPlayer.fLength -= 4.0f;
		}
		if (GetKeyboardPress(DIK_M) == true && g_aPlayer.fLength < PLAYER_WIDTH * 2)
		{//Mキーで拡大
			g_aPlayer.fLength += 4.0f;
		}
		if (GetKeyboardPress(DIK_K) == true)
		{//Kキーで左回り
			g_aPlayer.rot.x += 10.0;
			g_aPlayer.rot.y += 10.0;
		}
		if (GetKeyboardPress(DIK_L) == true)
		{//Lキーで右回り
			g_aPlayer.rot.x = 0.0;
			g_aPlayer.rot.y = 0.0;
		}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//外面外ループ処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_aPlayer.pos.x > SCREEN_WIDTH + PLAYER_WIDTH / 2)
		{//画面右端まで来た時の処理
			g_aPlayer.pos.x = 0.0f - PLAYER_WIDTH / 2 + 1;
			g_aPlayer.posOld.x = 0.0f - PLAYER_WIDTH / 2;
		}
		if (g_aPlayer.pos.x < 0.0f - PLAYER_WIDTH / 2)
		{//画面左端まで来た時の処理
			g_aPlayer.pos.x = SCREEN_WIDTH + PLAYER_WIDTH / 2 - 1;
			g_aPlayer.posOld.x = SCREEN_WIDTH + PLAYER_WIDTH / 2;
		}
		if (g_aPlayer.pos.y > SCREEN_HEIGHT)
		{//画面下端まで来た時の処理
			g_aPlayer.pos.y = SCREEN_HEIGHT;
			g_aPlayer.move.y = 0;
			g_aPlayer.bIsJumping = false;
			g_nJumpStock = 0;
		}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ブロック当たり判定の処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		g_aPlayer.pBlock = NULL;
		if (ColisionBlock(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, 
			g_aPlayer.fWidth, g_aPlayer.fHeight, &g_aPlayer.pBlock) == BLOCK_COLLISION_TOP)
		{//ブロックに乗った場合の処理
			g_nJumpStock = 0;
			g_aPlayer.bIsJumping = false;

			//ブロックの上にプレイヤーを固定
			g_aPlayer.pos.y = g_aPlayer.pBlock->pos.y - g_aPlayer.pBlock->fHeight / 2;
			g_aPlayer.move.y = 0;

			if (g_aPlayer.pBlock->move != NULL)
			{//ブロックが動いている場合、プレイヤーに慣性を入れる処理
				g_aPlayer.move.x += g_aPlayer.pBlock->move.x * 0.1f;

				if (g_aPlayer.pBlock->pos.x >= g_aPlayer.pBlock->setpos.x + 200.0f
					|| g_aPlayer.pBlock->pos.x <= g_aPlayer.pBlock->setpos.x - 200.0f)
				{//ブロックが指定の位置に来た時、プレイヤーを動かす処理
					g_aPlayer.move.x += g_aPlayer.pBlock->move.x * 2;
				}
			}
			if (g_aPlayer.pBlock->type == BLOCK_TYPE_EVADE || g_aPlayer.pBlock->type == BLOCK_TYPE_WHITE)
			{//すり抜けブロックの上にいる時の処理
				if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
				{//下キーですり抜け
					g_aPlayer.pos.y = g_aPlayer.pBlock->pos.y - g_aPlayer.pBlock->fHeight / 2 + 5.0f;
					g_aPlayer.pBlock = NULL;
				}
			}
		}
		ColisionItem(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, g_aPlayer.fWidth, g_aPlayer.fHeight);
	}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//プレイヤーの状態遷移処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	switch (g_aPlayer.state)
	{//プレイヤー状態によるそれぞれの処理
	case PLAYERSTATE_NORMAL:
		g_aPlayer.bUse = true;
		g_aPlayer.bAppearUse = false;
		g_bExplosionUse = false;
		g_aPlayer.col.a = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		break;
	case PLAYERSTATE_DAMAGE:
		g_aPlayer.nCounterState--;
		g_aPlayer.bAppearUse = true;

		if (g_aPlayer.nCounterState <= 0)
		{
			g_aPlayer.state = PLAYERSTATE_APPEAR;
		}
		break;
	case PLAYERSTATE_DEATH:
		g_aPlayer.nCounterState--;
		g_aPlayer.col.a -= 0.01f;		//プレイヤーを徐々に透明にする処理
		g_aPlayer.bUse = false;
		if (g_bExplosionUse == false)
		{
			g_bExplosionUse = true;
			SetExplosion(g_aPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (g_aPlayer.col.a <= 0.5f)
		{
			g_aPlayer.col.a = 0.5f;
		}
		//頂点カラーの設定
		pVtx[0].col = g_aPlayer.col;
		pVtx[1].col = g_aPlayer.col;
		pVtx[2].col = g_aPlayer.col;
		pVtx[3].col = g_aPlayer.col;


		if (fade == FADE_NONE)
		{//フェードが行われてない場合の処理
			SetGameState(GAMESTATE_GAMEOVER);
		}
		break;
	case PLAYERSTATE_APPEAR:
		g_aPlayer.nCounterAppear++;		//ダメージ点滅時間

		if (g_aPlayer.nCounterAppear <= 120)
		{
			if (g_aPlayer.Astate == APPEARSTATE_IN)
			{//AstateがINの状態の処理
				g_aPlayer.col.a -= 0.5f;		//プレイヤーを徐々に透明にする処理

				if (g_aPlayer.col.a <= 0.0f)
				{
					g_aPlayer.col.a = 0.0f;
					g_aPlayer.Astate = APPEARSTATE_OUT;		//プレイヤーの出現状態変更
				}
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 0);
				pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 0);
				pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 0);
				pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 0);
			}
			else if (g_aPlayer.Astate == APPEARSTATE_OUT)
			{//AstateがOUTの状態の処理
				g_aPlayer.col.a += 0.5f;		//プレイヤーを徐々に不透明にする処理

				if (g_aPlayer.col.a >= 1.0f)
				{
					g_aPlayer.col.a = 1.0f;
					g_aPlayer.Astate = APPEARSTATE_IN;	//プレイヤーの出現状態変更
				}
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
		}
		else
		{
			g_aPlayer.nCounterAppear = 0;
			g_aPlayer.state = PLAYERSTATE_NORMAL;
		}
		break;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//============================================================================
//プレイヤーの描画処理
//============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (g_aPlayer.bDisp == true)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(
			0,
			g_pVtxBuffPlayer,		//頂点バッファへのポインタ
			0,
			sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
		);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		//ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			0,						//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}
}

//============================================================================
//プレイヤーの取得処理
//============================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}

//============================================================================
////プレイヤーの被弾処理
//============================================================================
void HitPlayer(int nDamage)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	if (g_aPlayer.bAppearUse == false)
	{//プレイヤーが使用されている場合

		//敵の弾が当たった時、プレイヤーにダメージ
		g_aPlayer.nLife -= nDamage;

		if (g_aPlayer.nLife <= 0)
		{//プレイヤーのライフが0になった時の処理
			g_aPlayer.bUse = false;
			g_aPlayer.state = PLAYERSTATE_DEATH;
		}
		else
		{//プレイヤーのライフが残っている時の処理
			g_aPlayer.state = PLAYERSTATE_DAMAGE;
			g_aPlayer.nCounterState = 1;		//ダメージ点滅時間

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//頂点カラーの設定(赤)
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(252, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

			g_pVtxBuffPlayer->Unlock();
		}
	}
}
