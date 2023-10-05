//============================================================================
//
//モデルの処理[model.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "player.h"		//作成したplayer.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "camera.h"		//作成したcamera.hをインクルードする
#include "shadow.h"		//作成したshadow.hをインクルードする
#include "effect.h"		//作成したeffect.hをインクルードする
#include "model.h"		//作成したmodel.hをインクルードする
#include "meshfield.h"	//作成したmeshfield.hをインクルードする
#include "enemy.h"		//作成したenemy.hをインクルードする
#include "item.h"		//作成したitem.hをインクルードする
#include "attack.h"		//作成したattack.hをインクルードする
#include "fade.h"		//作成したfade.hをインクルードする
#include "game.h"		//作成したgame.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
PLAYER g_aPlayer;
D3DXVECTOR3 g_rotDestPlayer;
int g_nIndxShadowPlayer;				//影のインデックス

										//============================================================================
										//プレイヤーの初期化処理
										//============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, -300.0f);
	g_aPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_aPlayer.fAngle = atan2f(PLAYER_WIDTH, PLAYER_HEIGHT);
	g_aPlayer.fLength = sqrtf(PLAYER_WIDTH * PLAYER_WIDTH + PLAYER_HEIGHT * PLAYER_HEIGHT) / 2.0f;
	g_aPlayer.nLife = PLAYER_LIFE;
	g_aPlayer.nPower = 0;
	g_aPlayer.fFlame = 0;
	g_aPlayer.fAttack = 0;
	g_aPlayer.bUse = true;
	g_aPlayer.bDamege = false;
	g_aPlayer.bLoop = false;
	g_aPlayer.bGround = false;
	g_aPlayer.tex = PLAYER_TEXTURE_MOFU;
	g_aPlayer.motion = PLAYER_MOTION_NUTRAL;
	g_aPlayer.Worldmtx = D3DXMATRIX();

	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYER_PARTS; nCntPlayerParts++)
	{
		g_aPlayer.aParts[nCntPlayerParts].Worldmtx = D3DXMATRIX();	//ワールドマトリックス
	}

	//体(親)のパーツ
	g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.aParts[0].nIdxModelParent = -1;

	//頭(子)のパーツ
	g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
	g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.aParts[1].nIdxModelParent = 0;

	//左足(子)のパーツ
	g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
	g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.aParts[2].nIdxModelParent = 0;

	//右足(子)のパーツ
	g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
	g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.aParts[3].nIdxModelParent = 0;

	//Xファイルの読み込み(体)
	D3DXLoadMeshFromX("Data\\Model\\00_niwamaru_body.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPlayer.aParts[0].pBuffMat,
		NULL,
		&g_aPlayer.aParts[0].nNumMat,
		&g_aPlayer.aParts[0].pMesh
	);

	//Xファイルの読み込み(頭)
	D3DXLoadMeshFromX("Data\\Model\\01_niwamaru_head.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPlayer.aParts[1].pBuffMat,
		NULL,
		&g_aPlayer.aParts[1].nNumMat,
		&g_aPlayer.aParts[1].pMesh
	);

	//Xファイルの読み込み(左足)
	D3DXLoadMeshFromX("Data\\Model\\02_niwamaru_legL.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPlayer.aParts[2].pBuffMat,
		NULL,
		&g_aPlayer.aParts[2].nNumMat,
		&g_aPlayer.aParts[2].pMesh
	);

	//Xファイルの読み込み(右足)
	D3DXLoadMeshFromX("Data\\Model\\03_niwamaru_legR.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPlayer.aParts[3].pBuffMat,
		NULL,
		&g_aPlayer.aParts[3].nNumMat,
		&g_aPlayer.aParts[3].pMesh
	);

	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYER_PARTS; nCntPlayerParts++)
	{
		//現在のマテリアルのポインタを取得
		D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)g_aPlayer.aParts[nCntPlayerParts].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aParts[nCntPlayerParts].nNumMat; nCntMat++)
		{
			//テクスチャの読み出し
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//テクスチャのファイル名
				&g_aPlayer.aParts[nCntPlayerParts].g_pTexture[PLAYER_TEXTURE_MOFU]
			);
		}
	}
	g_nIndxShadowPlayer = SetShadow(g_aPlayer.pos, g_aPlayer.rot);
}

//============================================================================
//プレイヤーの初期化処理
//============================================================================
void UninitPlayer(void)
{
	for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYER_PARTS; nCntPlayerParts++)
	{
		//メッシュの破棄
		if (g_aPlayer.aParts[nCntPlayerParts].pMesh != NULL)
		{
			g_aPlayer.aParts[nCntPlayerParts].pMesh->Release();
			g_aPlayer.aParts[nCntPlayerParts].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aPlayer.aParts[nCntPlayerParts].pBuffMat != NULL)
		{
			g_aPlayer.aParts[nCntPlayerParts].pBuffMat->Release();
			g_aPlayer.aParts[nCntPlayerParts].pBuffMat = NULL;
		}

		// xモデルの使用テクスチャを全て解放する(メモリリーク修正_2023/7/12)
		for (int nMatTex = 0; nMatTex < PLAYER_TYPE_MAX; nMatTex++)
		{
			//テクスチャの破棄
			if (g_aPlayer.aParts[nCntPlayerParts].g_pTexture[nMatTex] != NULL)
			{
				g_aPlayer.aParts[nCntPlayerParts].g_pTexture[nMatTex]->Release();
				g_aPlayer.aParts[nCntPlayerParts].g_pTexture[nMatTex] = NULL;
			}
		}
	}
}

//============================================================================
//プレイヤーの更新処理
//============================================================================
void UpdatePlayer(void)
{
	FADE fade;
	fade = GetFade();
	CAMERA *pCamera = GetCamera();
	ENEMY *pEnemy = GetEnemy();
	ATTACK *pAttack = GetAttack();

	if (g_aPlayer.bUse == true)
	{
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//プレイヤー移動処理
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (GetKeyboardPress(DIK_W) == true)
		{//手前移動
			g_aPlayer.rot.y += (g_aPlayer.Destrot.y - g_aPlayer.rot.y) / 10;
			if (g_aPlayer.bLoop == false && g_aPlayer.bGround == true)
			{
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_WARK;
				g_aPlayer.bLoop = true;
			}

			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y - D3DX_PI / 4) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y - D3DX_PI / 4) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y + D3DX_PI * 3 / 4;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y + D3DX_PI / 4) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y + D3DX_PI / 4) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y - D3DX_PI * 3 / 4;
			}
			else
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y) * PLAYER_SPEAD;

				if (g_aPlayer.rot.y >= pCamera->rot.y)
				{
					g_aPlayer.Destrot.y = pCamera->rot.y + D3DX_PI;
				}
				else if (g_aPlayer.rot.y < pCamera->rot.y)
				{
					g_aPlayer.Destrot.y = pCamera->rot.y - D3DX_PI;
				}
			}
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//奥移動
			g_aPlayer.rot.y += (g_aPlayer.Destrot.y - g_aPlayer.rot.y) / 10;
			if (g_aPlayer.bLoop == false && g_aPlayer.bGround == true)
			{
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_WARK;
				g_aPlayer.bLoop = true;
			}

			if (GetKeyboardPress(DIK_A) == true)
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y - D3DX_PI * 3 / 4) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y - D3DX_PI * 3 / 4) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y + D3DX_PI / 4;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y + D3DX_PI * 3 / 4) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y + D3DX_PI * 3 / 4) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y - D3DX_PI / 4;
			}
			else
			{
				g_aPlayer.move.x += sinf(pCamera->rot.y - D3DX_PI) * PLAYER_SPEAD;
				g_aPlayer.move.z += cosf(pCamera->rot.y - D3DX_PI) * PLAYER_SPEAD;
				g_aPlayer.Destrot.y = pCamera->rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_A) == true)
		{//左移動
			g_aPlayer.rot.y += (g_aPlayer.Destrot.y - g_aPlayer.rot.y) / 10;
			if (g_aPlayer.bLoop == false && g_aPlayer.bGround == true)
			{
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_WARK;
				g_aPlayer.bLoop = true;
			}

			g_aPlayer.move.x += sinf(pCamera->rot.y - D3DX_PI / 2.0f) * PLAYER_SPEAD;
			g_aPlayer.move.z += cosf(pCamera->rot.y - D3DX_PI / 2.0f) * PLAYER_SPEAD;
			g_aPlayer.Destrot.y = pCamera->rot.y + D3DX_PI / 2;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//右移動
			g_aPlayer.rot.y += (g_aPlayer.Destrot.y - g_aPlayer.rot.y) / 10;
			if (g_aPlayer.bLoop == false && g_aPlayer.bGround == true)
			{
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_WARK;
				g_aPlayer.bLoop = true;
			}

			g_aPlayer.move.x += sinf(pCamera->rot.y + D3DX_PI / 2.0f) * PLAYER_SPEAD;
			g_aPlayer.move.z += cosf(pCamera->rot.y + D3DX_PI / 2.0f) * PLAYER_SPEAD;
			g_aPlayer.Destrot.y = pCamera->rot.y - D3DX_PI / 2;
		}

		if (g_aPlayer.bGround == true)
		{//接地時の処理
			if (GetKeyboardTrigger(DIK_SPACE) == true)
			{//ジャンプ
				g_aPlayer.motion = PLAYER_MOTION_JUMP;
				g_aPlayer.bLoop = true;
				g_aPlayer.fFlame = 0;
				g_aPlayer.move.y += 100.0f;
				g_aPlayer.bGround = false;		//着地判定を偽にする
			}
		}

		if (g_aPlayer.nPower < 3)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{//攻撃モーション
				g_aPlayer.motion = PLAYER_MOTION_ATTACK;
				g_aPlayer.bLoop = true;
				g_aPlayer.fFlame = 0;
			}
		}
		else if (g_aPlayer.fFlame >= 6.0f)
		{
			if (GetKeyboardPress(DIK_RETURN) == true)
			{//攻撃モーション
				g_aPlayer.motion = PLAYER_MOTION_ATTACK;
				g_aPlayer.bLoop = true;
				g_aPlayer.fFlame = 0;
			}
		}
		else
		{
			g_aPlayer.fFlame++;
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//角度の正規化処理
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_aPlayer.rot.y > D3DX_PI)
		{//現在の角度が(PI)より大きい場合
			g_aPlayer.rot.y -= D3DX_PI * 2;
		}
		else if (g_aPlayer.rot.y < -D3DX_PI)
		{//現在の角度が(-PI)より小さい場合
			g_aPlayer.rot.y += D3DX_PI * 2;
		}

		if (g_aPlayer.Destrot.y - g_aPlayer.rot.y > D3DX_PI)
		{//現在の目的値が(PI)より大きい場合
			g_aPlayer.Destrot.y -= 2 * D3DX_PI;
		}
		else if (g_aPlayer.Destrot.y - g_aPlayer.rot.y < -D3DX_PI)
		{//現在の目的地が(-PI)より小さい場合
			g_aPlayer.Destrot.y += 2 * D3DX_PI;
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//プレイヤーモーション処理
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//通常状態
		if (g_aPlayer.motion == PLAYER_MOTION_NUTRAL)
		{
			g_aPlayer.fFlame++;

			if (g_aPlayer.fFlame <= 40)
			{
				//体(親)のパーツ
				g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.002f, 0.0f, 0.0f);
				//頭(子)のパーツ
				g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.002f, 0.0f, 0.0f);
				//左足(子)のパーツ
				g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.003f, 0.0f, 0.0f);
				//右足(子)のパーツ
				g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.003f, 0.0f, 0.0f);
			}
			else if (g_aPlayer.fFlame <= 80)
			{
				//体(親)のパーツ
				g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.002f, 0.0f, 0.0f);
				//頭(子)のパーツ
				g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.002f, 0.0f, 0.0f);
				//左足(子)のパーツ
				g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.003f, 0.0f, 0.0f);
				//右足(子)のパーツ
				g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.003f, 0.0f, 0.0f);
			}
			else
			{//モーションリセット
				g_aPlayer.fFlame = 0;
				//体(親)のパーツ
				g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
				g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//頭(子)のパーツ
				g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
				g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//左足(子)のパーツ
				g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
				g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				//右足(子)のパーツ
				g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
				g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
		//通常状態以外の処理
		else if (g_aPlayer.bLoop == true)
		{
			//歩行状態
			if (g_aPlayer.motion == PLAYER_MOTION_WARK)
			{
				g_aPlayer.fFlame++;

				if (g_aPlayer.fFlame <= 5)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.01f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.01f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.1f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 10)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.01f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.01f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 15)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.01f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.01f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 20)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.01f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.01f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.1f, 0.0f, 0.0f);
				}
				else
				{//モーションリセット
					g_aPlayer.fFlame = 0;
					g_aPlayer.bLoop = false;
					//体(親)のパーツ
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//頭(子)のパーツ
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			//ジャンプ状態
			if (g_aPlayer.motion == PLAYER_MOTION_JUMP)
			{
				if (g_aPlayer.fFlame == 0)
				{
					g_aPlayer.fFlame++;
					//体(親)のパーツ
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//頭(子)のパーツ
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 2)
				{
					g_aPlayer.fFlame++;
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.5f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.5f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 4)
				{
					g_aPlayer.fFlame++;
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.5f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.5f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 6)
				{
					g_aPlayer.fFlame++;
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(0.1f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(-0.1f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos += D3DXVECTOR3(0.0f, -0.6f, 0.0f);
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(-0.6f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos += D3DXVECTOR3(0.0f, -0.6f, 0.0f);
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(-0.6f, 0.0f, 0.0f);
				}
			}
			//着地状態
			if (g_aPlayer.motion == PLAYER_MOTION_LANDING)
			{
				g_aPlayer.fFlame++;

				if (g_aPlayer.move.x >= 1.0f || g_aPlayer.move.z >= 1.0f)
				{
					//モーションリセット
					//体(親)のパーツ
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//頭(子)のパーツ
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					g_aPlayer.fFlame = 0;
					g_aPlayer.bLoop = false;
					g_aPlayer.motion = PLAYER_MOTION_WARK;
				}
				else if (g_aPlayer.fFlame == 1)
				{//モーションリセット
					//体(親)のパーツ
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//頭(子)のパーツ
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 5)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].pos += D3DXVECTOR3(0.0f, -0.4f, 0.0f);
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.08f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos += D3DXVECTOR3(0.0f, 0.4f, 0.0f);
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.10f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos += D3DXVECTOR3(0.0f, 0.4f, 0.0f);
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.10f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 15)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].pos -= D3DXVECTOR3(0.0f, -0.2f, 0.0f);
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.04f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos -= D3DXVECTOR3(0.0f, 0.2f, 0.0f);
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.05f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos -= D3DXVECTOR3(0.0f, 0.2f, 0.0f);
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.05f, 0.0f, 0.0f);
				}
				else
				{
					g_aPlayer.fFlame = 0;
					g_aPlayer.bLoop = false;
				}
			}
			//攻撃状態
			if (g_aPlayer.motion == PLAYER_MOTION_ATTACK)
			{
					if (g_aPlayer.fFlame == 0)
					{//攻撃判定作成(位置, 移動量, 持続時間, 種類)
						SetAttack
						(
							D3DXVECTOR3(g_aPlayer.pos.x - sinf(g_aPlayer.rot.y) * 12.0f, (g_aPlayer.pos.y + 10.0f), g_aPlayer.pos.z - cosf(g_aPlayer.rot.y) * 12.0f),
							D3DXVECTOR3(0.0f, -10.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							PLAYER_ATTACK,
							ATTACK_TYPE_PLAYER
						);
					}
				g_aPlayer.fFlame++;
				if (g_aPlayer.fFlame <= 4)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.4f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot += D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].rot += D3DXVECTOR3(0.5f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].rot += D3DXVECTOR3(0.5f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 8)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.4f, 0.0f, 0.0f);
					//頭(子)のパーツ
					g_aPlayer.aParts[1].rot -= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					//左足(子)のパーツ
					g_aPlayer.aParts[2].rot -= D3DXVECTOR3(0.5f, 0.0f, 0.0f);
					//右足(子)のパーツ
					g_aPlayer.aParts[3].rot -= D3DXVECTOR3(0.5f, 0.0f, 0.0f);
				}
				else
				{//モーションリセット
					g_aPlayer.fFlame = 0;
					g_aPlayer.bLoop = false;
					//体(親)のパーツ
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//頭(子)のパーツ
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//弾を終了させる
					pAttack->bUse = false;
				}
			}
			//被弾状態
			if (g_aPlayer.motion == PLAYER_MOTION_DAMEGE)
			{
				if (g_aPlayer.fFlame <= 0)
				{//モーションリセット
					g_aPlayer.fFlame = 0;
					//体(親)のパーツ
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//頭(子)のパーツ
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
				g_aPlayer.fFlame++;

				if (g_aPlayer.fFlame <= 5)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.08f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 10)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot -= D3DXVECTOR3(-0.28f, 0.0f, 0.0f);
				}
				else if (g_aPlayer.fFlame <= 20)
				{
					//体(親)のパーツ
					g_aPlayer.aParts[0].rot += D3DXVECTOR3(-0.14f, 0.0f, 0.0f);
				}
				else
				{//モーションリセット
					g_aPlayer.bLoop = false;
					g_aPlayer.fFlame = 0;

					//体(親)のパーツ
					g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
					g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//頭(子)のパーツ
					g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
					g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//左足(子)のパーツ
					g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					//右足(子)のパーツ
					g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
					g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
		}
		else
		{//通常状態以外のモーションが終了した場合
			g_aPlayer.fFlame = 0;
			//体(親)のパーツ
			g_aPlayer.aParts[0].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
			g_aPlayer.aParts[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//頭(子)のパーツ
			g_aPlayer.aParts[1].pos = D3DXVECTOR3(0.0f, 4.0f, -6.0f);
			g_aPlayer.aParts[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//左足(子)のパーツ
			g_aPlayer.aParts[2].pos = D3DXVECTOR3(2.0f, -2.0f, 0.0f);
			g_aPlayer.aParts[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//右足(子)のパーツ
			g_aPlayer.aParts[3].pos = D3DXVECTOR3(-2.0f, -2.0f, 0.0f);
			g_aPlayer.aParts[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPlayer.motion = PLAYER_MOTION_NUTRAL;		//通常状態に戻す
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//落ちたら復帰する処理
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (g_aPlayer.pos.y <= -100)
		{
			g_aPlayer.nLife--;
			g_aPlayer.pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//その他の処理
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//プレイヤーの前回位置を反映
		g_aPlayer.posOld = g_aPlayer.pos;

		//プレイヤーの位置更新
		g_aPlayer.pos += g_aPlayer.move * 0.1f;

		//重力を更新(増幅させる)
		g_aPlayer.move.y -= 4.0f;

		//移動量を更新(減衰させる)
		g_aPlayer.move -= g_aPlayer.move * 0.1f;

		g_aPlayer.nPower = (g_aPlayer.nLife / PLAYER_LIFE) + 1;

		if (g_aPlayer.nLife >= MAX_PLAYER_LIFE)
		{
			g_aPlayer.nLife = MAX_PLAYER_LIFE;
		}
		else if (g_aPlayer.nLife < 0)
		{
			g_aPlayer.nLife = 0;
		}

		//ライフが0になった時
		if (g_aPlayer.nLife == 0)
		{
			if (fade == FADE_NONE)
			{//フェードが行われてない場合の処理
				SetGameState(GAMESTATE_GAMEOVER);
			}
			g_aPlayer.bUse = false;
		}

		//プレイヤーのサイズを求める処理
		D3DXVECTOR3 size = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

		//プレイヤーのサイズの半径を求める処理
		D3DXVECTOR3 r_size = size / 2;

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//アイテム当たり判定の処理
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		bool bItem = ItemCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, &size);
		
		if (bItem == true)
		{
			g_aPlayer.nLife += 1;
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//被弾判定処理
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		if (pAttack->type != ATTACK_TYPE_PLAYER)
		{
			g_aPlayer.bDamege = AttackCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, &r_size);

			if (g_aPlayer.bDamege == true)
			{

				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_DAMEGE;
				g_aPlayer.bLoop = true;
				g_aPlayer.move =
					D3DXVECTOR3(-sinf(pEnemy->rot.y) * 60.0f, 100.0f, -cosf(pEnemy->rot.y) * 60.0f);
				g_aPlayer.nLife -= 1;
			}
		}

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//床の判定処理
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//メッシュフィールド当たり判定の処理
		bool bIsGroundField = FieldCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move);

		//敵キャラ当たり判定の処理
		bool bIsGroundEnemy = EnemyCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, &r_size);

		//モデル当たり判定の処理
		bool bIsGroundModel = ModelCollision(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, &r_size);

		if (bIsGroundField == true || bIsGroundEnemy == true || bIsGroundModel == true)
		{
			if (g_aPlayer.bGround == false)
			{
				g_aPlayer.bGround = true;	//着地判定を真にする
				g_aPlayer.bLoop = true;
				g_aPlayer.fFlame = 0;
				g_aPlayer.motion = PLAYER_MOTION_LANDING;
			}
		}
		//影の大きさを高さで設定
		SetpositionShadow(g_nIndxShadowPlayer, D3DXVECTOR3(g_aPlayer.pos.x, -2.0f, g_aPlayer.pos.z),
			SHADOW_WIDTH + (g_aPlayer.pos.y / 2), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1 - (g_aPlayer.pos.y / 100)));
	}
}

//============================================================================
//プレイヤーの描画処理
//============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DXMATRIX mtxParent;						//親用マトリクス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用

	if (g_aPlayer.bUse)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.Worldmtx);

		//現在のマテリアルを取得する
		pDevice->GetMaterial(&matDef);

		//向きを反映(YaW : y,Pitch : x,Roll : z)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.Worldmtx, &g_aPlayer.Worldmtx, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.Worldmtx, &g_aPlayer.Worldmtx, &mtxTrans);

		//パーツの処理
		for (int nCntPlayerParts = 0; nCntPlayerParts < MAX_PLAYER_PARTS; nCntPlayerParts++)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer.aParts[nCntPlayerParts].Worldmtx);

			//向きを反映(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aParts[nCntPlayerParts].rot.y, g_aPlayer.aParts[nCntPlayerParts].rot.x, g_aPlayer.aParts[nCntPlayerParts].rot.z);
			D3DXMatrixMultiply(&g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aParts[nCntPlayerParts].pos.x, g_aPlayer.aParts[nCntPlayerParts].pos.y, g_aPlayer.aParts[nCntPlayerParts].pos.z);
			D3DXMatrixMultiply(&g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &mtxTrans);

			if (g_aPlayer.aParts[nCntPlayerParts].nIdxModelParent == -1)
			{//親モデルのインデックスが-1の場合
				mtxParent = g_aPlayer.Worldmtx;				//プレイヤーマトリクスを登録
			}
			else
			{
				mtxParent = g_aPlayer.aParts[g_aPlayer.aParts[nCntPlayerParts].nIdxModelParent].Worldmtx;		//親のマトリクスを登録
			}
			//親モデルのマトリックスとの掛け算
			D3DXMatrixMultiply(&g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &g_aPlayer.aParts[nCntPlayerParts].Worldmtx, &mtxParent);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aParts[nCntPlayerParts].Worldmtx);

			//現在のマテリアルのポインタを取得
			D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_aPlayer.aParts[nCntPlayerParts].pBuffMat->GetBufferPointer();

			//テクスチャの読み出し
			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aParts[nCntPlayerParts].nNumMat; nCntMat++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_aPlayer.aParts[nCntPlayerParts].g_pTexture[g_aPlayer.tex]);

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				//モデルパーツの描画
				g_aPlayer.aParts[nCntPlayerParts].pMesh->DrawSubset(nCntMat);
			}
		}
		//テクスチャの初期化(バグ修正_2023/7/12)
		pDevice->SetTexture(0, NULL);

		//保持してたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//============================================================================
//プレイヤーの取得処理
//============================================================================
PLAYER *GetPlayer(void)
{
	return &g_aPlayer;
}

