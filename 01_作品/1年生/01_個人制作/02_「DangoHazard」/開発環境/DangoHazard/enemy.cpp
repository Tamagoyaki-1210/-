//============================================================================
//
//敵キャラの処理[enemy.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "enemy.h"		//作成したenemy.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "camera.h"		//作成したcamera.hをインクルードする
#include "shadow.h"		//作成したshadow.hをインクルードする
#include "effect.h"		//作成したeffect.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "model.h"		//作成したmodel.hをインクルードする
#include "meshfield.h"	//作成したmeshfield.hをインクルードする
#include "attack.h"		//作成したattack.hをインクルードする
#include "score.h"		//作成したscore.hをインクルードする
#include <math.h>		//浮動小数点の計算に必要

//****************************************************************************
//グローバル変数
//****************************************************************************
ENEMY g_aEnemy[MAX_ENEMY];			//敵キャラ構造体		
X_ENEMY g_aX_Enemy[IMAGE_ENEMY];	//xファイル構造体

									//============================================================================
									//敵キャラの初期化処理
									//============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

	//Xファイルの読み込み
	D3DXLoadMeshFromX("Data\\Model\\imodanngo.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Enemy[ENEMY_TYPE_POTATE].pBuffMat,
		NULL,
		&g_aX_Enemy[ENEMY_TYPE_POTATE].nNumMat,
		&g_aX_Enemy[ENEMY_TYPE_POTATE].pMesh
	);

	//Xファイルの読み込み
	D3DXLoadMeshFromX("Data\\Model\\kabotyadanngo.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Enemy[ENEMY_TYPE_PUNPKIN].pBuffMat,
		NULL,
		&g_aX_Enemy[ENEMY_TYPE_PUNPKIN].nNumMat,
		&g_aX_Enemy[ENEMY_TYPE_PUNPKIN].pMesh
	);

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCntImgEnemy = 0; nCntImgEnemy < IMAGE_ENEMY; nCntImgEnemy++)
	{//画像の処理
		D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)g_aX_Enemy[nCntImgEnemy].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aX_Enemy[nCntImgEnemy].nNumMat; nCntMat++)
		{
			//テクスチャの読み出し
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//テクスチャのファイル名
				&g_aX_Enemy[nCntImgEnemy].g_pTexture[ENEMY_TYPE_POTATE]
			);

			//テクスチャの読み出し
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//テクスチャのファイル名
				&g_aX_Enemy[nCntImgEnemy].g_pTexture[ENEMY_TYPE_PUNPKIN]
			);
		}

		g_aX_Enemy[nCntImgEnemy].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aX_Enemy[nCntImgEnemy].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点数の取得
		nNumVtx = g_aX_Enemy[nCntImgEnemy].pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aX_Enemy[nCntImgEnemy].pMesh->GetFVF());
		//頂点バッファのロック
		g_aX_Enemy[nCntImgEnemy].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代表
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標を比較してモデルの最大値を取得
			if (vtx.x > g_aX_Enemy[nCntImgEnemy].vtxMax.x)
			{//x座標の最大値
				g_aX_Enemy[nCntImgEnemy].vtxMax.x = floorf(vtx.x);
			}
			if (vtx.y > g_aX_Enemy[nCntImgEnemy].vtxMax.y)
			{//y座標の最大値
				g_aX_Enemy[nCntImgEnemy].vtxMax.y = floorf(vtx.y);
			}
			if (vtx.z > g_aX_Enemy[nCntImgEnemy].vtxMax.z)
			{//z座標の最大値
				g_aX_Enemy[nCntImgEnemy].vtxMax.z = floorf(vtx.z);
			}

			//頂点座標を比較してモデルの最小値を取得
			if (vtx.x < g_aX_Enemy[nCntImgEnemy].vtxMin.x)
			{//x座標の最小値
				g_aX_Enemy[nCntImgEnemy].vtxMin.x = floorf(vtx.x);
			}
			if (vtx.y < g_aX_Enemy[nCntImgEnemy].vtxMin.y)
			{//y座標の最小値
				g_aX_Enemy[nCntImgEnemy].vtxMin.y = floorf(vtx.y);
			}
			if (vtx.z < g_aX_Enemy[nCntImgEnemy].vtxMin.z)
			{//z座標の最小値
				g_aX_Enemy[nCntImgEnemy].vtxMin.z = floorf(vtx.z);
			}

			//頂点座標フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}
		//頂点バッファのアンロック
		g_aX_Enemy[nCntImgEnemy].pMesh->UnlockVertexBuffer();
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//位置の初期化処理
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	
		g_aEnemy[nCntEnemy].vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].vec1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].fAngle = atan2f(ENEMY_WIDTH, ENEMY_HEIGHT);
		g_aEnemy[nCntEnemy].fLength = sqrtf(ENEMY_WIDTH * ENEMY_WIDTH + ENEMY_HEIGHT * ENEMY_HEIGHT) / 2.0f;
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].fFlame = 0;
		g_aEnemy[nCntEnemy].fVecCnt = 0;
		g_aEnemy[nCntEnemy].bLoop = false;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].bDamege = false;
		g_aEnemy[nCntEnemy].bBattle = false;
		g_aEnemy[nCntEnemy].Worldmtx = D3DXMATRIX();
		g_aEnemy[nCntEnemy].type = ENEMY_TYPE_POTATE;
		g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_NUTRAL;
		g_aEnemy[nCntEnemy].aParts[ENEMY_TYPE_POTATE].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		g_aEnemy[nCntEnemy].aParts[ENEMY_TYPE_POTATE].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].aParts[ENEMY_TYPE_PUNPKIN].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		g_aEnemy[nCntEnemy].aParts[ENEMY_TYPE_PUNPKIN].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//位置、角度、種類
	SetEnemy(D3DXVECTOR3(20.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMY_TYPE_PUNPKIN);
}

//============================================================================
//敵キャラの初期化処理
//============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		for (int nCntImgEnemy = 0; nCntImgEnemy < IMAGE_ENEMY; nCntImgEnemy++)
		{
			//敵キャラの破棄
			if (g_aX_Enemy[nCntImgEnemy].pMesh != NULL)
			{
				g_aX_Enemy[nCntImgEnemy].pMesh->Release();
				g_aX_Enemy[nCntImgEnemy].pMesh = NULL;
			}

			//マテリアルの破棄
			if (g_aX_Enemy[nCntImgEnemy].pBuffMat != NULL)
			{
				g_aX_Enemy[nCntImgEnemy].pBuffMat->Release();
				g_aX_Enemy[nCntImgEnemy].pBuffMat = NULL;
			}
		}
	}
}

//============================================================================
//敵キャラの更新処理
//============================================================================
void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			CAMERA *pCamera = GetCamera();
			ATTACK *pAttack = GetAttack();
			PLAYER *pPlayer = GetPlayer();

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//ベクトル(敵追従処理)
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			g_aEnemy[nCntEnemy].vec = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

			//正規化する(大きさ1のベクトルにする)
			D3DXVec3Normalize(&g_aEnemy[nCntEnemy].vec1, &g_aEnemy[nCntEnemy].vec);
			g_aEnemy[nCntEnemy].vec.y = 0;
			g_aEnemy[nCntEnemy].vec1.y = 0;

			//敵キャラのサイズを求める処理
			D3DXVECTOR3 size = (g_aX_Enemy[0].vtxMax - g_aX_Enemy[0].vtxMin);
			//敵キャラのサイズの半径を求める処理
			D3DXVECTOR3 r_size = size / 2.0f;
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//デバッグ用敵移動処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		
			//if (GetKeyboardPress(DIK_W) == true)
			//{//手前移動
			//	g_aEnemy[0].rot.y += (g_aEnemy[0].Destrot.y - g_aEnemy[0].rot.y) / 10;
			//	
			//	if (g_aEnemy[0].bLoop == false)
			//	{
			//		g_aEnemy[0].motion = ENEMY_MOTION_WORK;
			//		g_aEnemy[0].bLoop = true;
			//	}

			//	if (GetKeyboardPress(DIK_A) == true)
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y - D3DX_PI / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y - D3DX_PI / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y + D3DX_PI * 3 / 4;
			//	}
			//	else if (GetKeyboardPress(DIK_D) == true)
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y + D3DX_PI / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y + D3DX_PI / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y - D3DX_PI * 3 / 4;
			//	}
			//	else
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y) * ENEMY_SPEAD;

			//		if (g_aEnemy[0].rot.y >= pCamera->rot.y)
			//		{
			//			g_aEnemy[0].Destrot.y = pCamera->rot.y + D3DX_PI;
			//		}
			//		else if (g_aEnemy[0].rot.y < pCamera->rot.y)
			//		{
			//			g_aEnemy[0].Destrot.y = pCamera->rot.y - D3DX_PI;
			//		}
			//	}
			//}
			//else if (GetKeyboardPress(DIK_S) == true)
			//{//奥移動
			//	g_aEnemy[0].rot.y += (g_aEnemy[0].Destrot.y - g_aEnemy[0].rot.y) / 10;

			//	if (g_aEnemy[0].bLoop == false)
			//	{
			//		g_aEnemy[0].motion = ENEMY_MOTION_WORK;
			//		g_aEnemy[0].bLoop = true;
			//	}

			//	if (GetKeyboardPress(DIK_A) == true)
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y - D3DX_PI * 3 / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y - D3DX_PI * 3 / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y + D3DX_PI / 4;
			//	}
			//	else if (GetKeyboardPress(DIK_D) == true)
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y + D3DX_PI * 3 / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y + D3DX_PI * 3 / 4) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y - D3DX_PI / 4;
			//	}
			//	else
			//	{
			//		g_aEnemy[0].move.x += sinf(pCamera->rot.y - D3DX_PI) * ENEMY_SPEAD;
			//		g_aEnemy[0].move.z += cosf(pCamera->rot.y - D3DX_PI) * ENEMY_SPEAD;
			//		g_aEnemy[0].Destrot.y = pCamera->rot.y;
			//	}
			//}
			//else if (GetKeyboardPress(DIK_A) == true)
			//{//左移動
			//	g_aEnemy[0].rot.y += (g_aEnemy[0].Destrot.y - g_aEnemy[0].rot.y) / 10;
			//	if (g_aEnemy[0].bLoop == false)
			//	{
			//		g_aEnemy[0].motion = ENEMY_MOTION_WORK;
			//		g_aEnemy[0].bLoop = true;
			//	}

			//	g_aEnemy[0].move.x += sinf(pCamera->rot.y - D3DX_PI / 2.0f) * ENEMY_SPEAD;
			//	g_aEnemy[0].move.z += cosf(pCamera->rot.y - D3DX_PI / 2.0f) * ENEMY_SPEAD;
			//	g_aEnemy[0].Destrot.y = pCamera->rot.y + D3DX_PI / 2;
			//}
			//else if (GetKeyboardPress(DIK_D) == true)
			//{//右移動
			//	g_aEnemy[0].rot.y += (g_aEnemy[0].Destrot.y - g_aEnemy[0].rot.y) / 10;
			//	if (g_aEnemy[0].bLoop == false)
			//	{
			//		g_aEnemy[0].motion = ENEMY_MOTION_WORK;
			//		g_aEnemy[0].bLoop = true;
			//	}

			//	g_aEnemy[0].move.x += sinf(pCamera->rot.y + D3DX_PI / 2.0f) * ENEMY_SPEAD;
			//	g_aEnemy[0].move.z += cosf(pCamera->rot.y + D3DX_PI / 2.0f) * ENEMY_SPEAD;
			//	g_aEnemy[0].Destrot.y = pCamera->rot.y - D3DX_PI / 2;
			//}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//デバッグ用敵移動処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].vec.z > 2.0f)
			{//手前移動
				g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y) / 10;
				
				if (g_aEnemy[nCntEnemy].bLoop == false)
				{
					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
					g_aEnemy[nCntEnemy].bLoop = true;
				}

				if (g_aEnemy[nCntEnemy].vec.x < 2.0f)
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y + D3DX_PI * 3 / 4;
				}
				else if (g_aEnemy[nCntEnemy].vec.x > 2.0f)
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y - D3DX_PI * 3 / 4;
				}
				else
				{

					if (g_aEnemy[nCntEnemy].rot.y >= pCamera->rot.y)
					{
						g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y + D3DX_PI;
					}
					else if (g_aEnemy[nCntEnemy].rot.y < pCamera->rot.y)
					{
						g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y - D3DX_PI;
					}
				}
			}
			else if (g_aEnemy[nCntEnemy].vec.z < 2.0f)
			{//奥移動
				g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y) / 10;

				if (g_aEnemy[nCntEnemy].bLoop == false)
				{
					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
					g_aEnemy[nCntEnemy].bLoop = true;
				}

				if (g_aEnemy[nCntEnemy].vec.x < 2.0f)
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y + D3DX_PI / 4;
				}
				else if (g_aEnemy[nCntEnemy].vec.x > 2.0f)
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y - D3DX_PI / 4;
				}
				else
				{
					g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y;
				}
			}
			else if (g_aEnemy[nCntEnemy].vec.x < 2.0f)
			{//左移動
				g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y) / 10;
				if (g_aEnemy[nCntEnemy].bLoop == false)
				{
					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
					g_aEnemy[nCntEnemy].bLoop = true;
				}
				g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y + D3DX_PI / 2;
			}
			else if (g_aEnemy[nCntEnemy].vec.x > 2.0f)
			{//右移動
				g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y) / 10;
				if (g_aEnemy[nCntEnemy].bLoop == false)
				{
					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
					g_aEnemy[nCntEnemy].bLoop = true;
				}
				g_aEnemy[nCntEnemy].Destrot.y = pCamera->rot.y - D3DX_PI / 2;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//角度の正規化処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{//現在の角度が(PI)より大きい場合
				g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2;
			}
			else if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{//現在の角度が(-PI)より小さい場合
				g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}

			if (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
			{//現在の目的値が(PI)より大きい場合
				g_aEnemy[nCntEnemy].Destrot.y -= 2 * D3DX_PI;
			}
			else if (g_aEnemy[nCntEnemy].Destrot.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
			{//現在の目的地が(-PI)より小さい場合
				g_aEnemy[nCntEnemy].Destrot.y += 2 * D3DX_PI;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//敵キャラモーション処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			for (int nCntPartsEnemy = 0; nCntPartsEnemy < MAX_ENEMY_PARTS; nCntPartsEnemy++)
			{
				//通常状態
				if (g_aEnemy[nCntEnemy].motion == ENEMY_MOTION_NUTRAL)
				{
					g_aEnemy[nCntEnemy].fFlame++;

					if (g_aEnemy[nCntEnemy].fFlame <= 40)
					{
						//体(親)のパーツ
						g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(-0.002f, 0.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].fFlame <= 80)
					{
						//体(親)のパーツ
						g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(-0.002f, 0.0f, 0.0f);
					}
					else
					{//モーションリセット
						g_aEnemy[nCntEnemy].fFlame = 0;
						//体(親)のパーツ
						g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
						g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				//通常状態以外の処理
				else if (g_aEnemy[nCntEnemy].bLoop == true)
				{
					//歩行状態
					if (g_aEnemy[nCntEnemy].motion == ENEMY_MOTION_WORK)
					{
						if (g_aEnemy[nCntEnemy].fFlame <= 0)
						{//モーションリセット
							g_aEnemy[nCntEnemy].fFlame = 0;
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
						g_aEnemy[nCntEnemy].fFlame++;

						if (g_aEnemy[nCntEnemy].fFlame <= 5)
						{
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(-0.02f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 10)
						{
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(-0.02f, 0.0f, 0.0f);
						}
						else
						{//モーションリセット
							g_aEnemy[nCntEnemy].fFlame = 0;
							g_aEnemy[nCntEnemy].bLoop = false;
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
					//攻撃状態
					else if (g_aEnemy[nCntEnemy].motion == ENEMY_MOTION_ATTACK)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						if (g_aEnemy[nCntEnemy].fFlame <= 0)
						{//モーションリセット
							g_aEnemy[nCntEnemy].fFlame = 0;
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
						g_aEnemy[nCntEnemy].fFlame++;

						if (g_aEnemy[nCntEnemy].fFlame <= 20)
						{
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(-0.03f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 30)
						{
							if (g_aEnemy[nCntEnemy].fFlame == 30)
							{//攻撃判定作成(位置, 移動量, 持続時間, 種類)
								SetAttack
								(
									D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - sinf(g_aEnemy[nCntEnemy].rot.y) * 1.0f, (g_aEnemy[nCntEnemy].pos.y + 1.0f), g_aEnemy[nCntEnemy].pos.z - cosf(g_aEnemy[nCntEnemy].rot.y) * 1.0f),
									D3DXVECTOR3(-sinf(g_aEnemy[nCntEnemy].rot.y) * 2.5f, 1.0f, -cosf(g_aEnemy[nCntEnemy].rot.y) * 2.5f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f),
									ENEMY_ATTACK,
									ATTACK_TYPE_ENEMY
								);
							}

							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(-0.06f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 40)
						{
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos += D3DXVECTOR3(0.0f, 2.0f, -2.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(0.03f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 50)
						{
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos -= D3DXVECTOR3(0.0f, 2.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 60)
						{
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos -= D3DXVECTOR3(0.0f, 0.0f, -2.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(0.03f, 0.0f, 0.0f);
						}
						else
						{//モーションリセット
							g_aEnemy[nCntEnemy].fFlame = 0;
							g_aEnemy[nCntEnemy].bLoop = false;
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
					//被弾状態
					else if (g_aEnemy[nCntEnemy].motion == ENEMY_MOTION_DAMEGE)
					{
						if (g_aEnemy[nCntEnemy].fFlame <= 0)
						{//モーションリセット
							g_aEnemy[nCntEnemy].fFlame = 0;
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
						g_aEnemy[nCntEnemy].fFlame++;

						if (g_aEnemy[nCntEnemy].fFlame <= 5)
						{
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(0.04f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 10)
						{
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot += D3DXVECTOR3(0.14f, 0.0f, 0.0f);
						}
						else if (g_aEnemy[nCntEnemy].fFlame <= 20)
						{
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot -= D3DXVECTOR3(0.07f, 0.0f, 0.0f);
						}
						else
						{//モーションリセット
							g_aEnemy[nCntEnemy].bLoop = false;
							g_aEnemy[nCntEnemy].fFlame = 0;
							//体(親)のパーツ
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
							g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
				else
				{//通常状態以外のモーションが終了した場合
					g_aEnemy[nCntEnemy].fFlame = 0;
					//体(親)のパーツ
					g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
					g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_NUTRAL;		//通常状態に戻す
				}
			}
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//落ちたら復帰する処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].pos.y <= -100)
			{
				g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//敵ごとのAI
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aEnemy[nCntEnemy].vec.x >= -(size.x + ENEMY_BATTLE_RANGE) || g_aEnemy[nCntEnemy].vec.x <= (size.x + ENEMY_BATTLE_RANGE)
				|| g_aEnemy[nCntEnemy].vec.z >= -(size.z + ENEMY_BATTLE_RANGE) || g_aEnemy[nCntEnemy].vec.z <= (size.z + ENEMY_BATTLE_RANGE))
			{
				g_aEnemy[nCntEnemy].bBattle = true;
			}
			else
			{
				g_aEnemy[nCntEnemy].bBattle = false;
			}

			//イモダンゴの処理
			if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_POTATE)
			{
				if (g_aEnemy[nCntEnemy].bBattle == true)
				{
					g_aEnemy[nCntEnemy].fVecCnt++; 
					//敵キャラの追従を制限する処理
					if (g_aEnemy[nCntEnemy].fVecCnt >= 60)
					{
						//ベクトルを移動量に加算して追従させる処理
						g_aEnemy[nCntEnemy].move += ((g_aEnemy[nCntEnemy].vec1 * 0.4f) + (g_aEnemy[nCntEnemy].vec * 0.00001f));

						if (g_aEnemy[nCntEnemy].bLoop == false)
						{
							if (g_aEnemy[nCntEnemy].vec.x >= (size.x + ENEMY_ATTACK_RANGE) || g_aEnemy[nCntEnemy].vec.x <= -(size.x + ENEMY_ATTACK_RANGE)
								|| g_aEnemy[nCntEnemy].vec.z >= (size.z + ENEMY_ATTACK_RANGE) || g_aEnemy[nCntEnemy].vec.z <= -(size.z + ENEMY_ATTACK_RANGE))
							{
								g_aEnemy[nCntEnemy].fFlame = 0;
								g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
								g_aEnemy[nCntEnemy].bLoop = true;
							}
							else
							{
								g_aEnemy[nCntEnemy].fFlame = 0;
								g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_ATTACK;
								g_aEnemy[nCntEnemy].bLoop = true;
							}
						}
					}
					if (g_aEnemy[nCntEnemy].fVecCnt >= 90)
					{
						g_aEnemy[nCntEnemy].fVecCnt = 0;
					}
				}
			}
			//カボチャダンゴの処理
			if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_PUNPKIN)
			{
				if (g_aEnemy[nCntEnemy].bBattle == true)
				{
					g_aEnemy[nCntEnemy].fVecCnt++;
					//敵キャラの追従を制限する処理
					if (g_aEnemy[nCntEnemy].fVecCnt >= 90)
					{
						//ベクトルを移動量に加算して追従させる処理
						g_aEnemy[nCntEnemy].move += ((g_aEnemy[nCntEnemy].vec1 * ENEMY_SPEAD) + (g_aEnemy[nCntEnemy].vec * 0.00001f));

						if (g_aEnemy[nCntEnemy].bLoop == false)
						{
							if (g_aEnemy[nCntEnemy].vec.x >= (size.x + ENEMY_ATTACK_RANGE) || g_aEnemy[nCntEnemy].vec.x <= -(size.x + ENEMY_ATTACK_RANGE)
								|| g_aEnemy[nCntEnemy].vec.z >= (size.z + ENEMY_ATTACK_RANGE) || g_aEnemy[nCntEnemy].vec.z <= -(size.z + ENEMY_ATTACK_RANGE))
							{
								g_aEnemy[nCntEnemy].fFlame = 0;
								g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_WORK;
								g_aEnemy[nCntEnemy].bLoop = true;
							}
							else
							{
								g_aEnemy[nCntEnemy].fFlame = 0;
								g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_ATTACK;
								g_aEnemy[nCntEnemy].bLoop = true;
							}
						}
					}
					if (g_aEnemy[nCntEnemy].fVecCnt >= 120)
					{
						g_aEnemy[nCntEnemy].fVecCnt = 0;
					}
				}
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//その他の処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//プレイヤーの前回位置を反映
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			//敵キャラの位置更新
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.1f;

			//重力を更新(増幅させる)
			g_aEnemy[nCntEnemy].move.y -= 4.0f;

			//移動量を更新(減衰させる)
			g_aEnemy[nCntEnemy].move -= g_aEnemy[nCntEnemy].move * 0.1f;
			
			//ライフが0以下になった場合
			if (g_aEnemy[nCntEnemy].nLife <= 0)
			{
				g_aEnemy[nCntEnemy].bUse = false;
				AddScore(500);
			}

			//モデル当たり判定の処理
			ModelCollision(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, &r_size);

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//被弾判定処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//被弾判定の処理
			if (pAttack->type != ATTACK_TYPE_ENEMY)
			{
				g_aEnemy[nCntEnemy].bDamege = AttackCollision(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, &r_size);
			}

			if (g_aEnemy[nCntEnemy].bDamege == true)
			{
				g_aEnemy[nCntEnemy].fFlame = 0;
				g_aEnemy[nCntEnemy].motion = ENEMY_MOTION_DAMEGE;
				g_aEnemy[nCntEnemy].bLoop = true;

				g_aEnemy[nCntEnemy].move =
					D3DXVECTOR3(-sinf(pPlayer->rot.y) * 20.0f, 100.0f, -cosf(pPlayer->rot.y) * 20.0f);
				g_aEnemy[nCntEnemy].nLife -= pPlayer->nPower;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//床の判定処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//メッシュフィールド当たり判定の処理
			FieldCollision(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move);
		}
	}
}

//============================================================================
//敵キャラの描画処理
//============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].Worldmtx);

			//現在のマテリアルを取得する
			pDevice->GetMaterial(&matDef);

			//向きを反映(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].Worldmtx, &g_aEnemy[nCntEnemy].Worldmtx, &mtxRot);
			
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].Worldmtx, &g_aEnemy[nCntEnemy].Worldmtx, &mtxTrans);
			
			for (int nCntPartsEnemy = 0; nCntPartsEnemy < MAX_ENEMY_PARTS; nCntPartsEnemy++)
			{
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx);

				//向きを反映(YaW : y,Pitch : x,Roll : z)
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot.y, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot.x, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].rot.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &mtxRot);

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos.x, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos.y, g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].pos.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &mtxTrans);

				//親モデルのマトリックスとの掛け算
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx, &g_aEnemy[nCntEnemy].Worldmtx);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aParts[nCntPartsEnemy].Worldmtx);

				//現在のマテリアルのポインタを取得
				pMat = (D3DXMATERIAL*)g_aX_Enemy[g_aEnemy[nCntEnemy].type].pBuffMat->GetBufferPointer();

				//テクスチャの読み出し
				for (int nCntMat = 0; nCntMat < (int)g_aX_Enemy[g_aEnemy[nCntEnemy].type].nNumMat; nCntMat++)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_aX_Enemy[g_aEnemy[nCntEnemy].type].g_pTexture[g_aEnemy[nCntEnemy].type]);
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					//モデルパーツの描画
					g_aX_Enemy[g_aEnemy[nCntEnemy].type].pMesh->DrawSubset(nCntMat);
				}
			}
			//保持してたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//============================================================================
//敵キャラの当たり判定処理
//============================================================================
bool EnemyCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size)
{
	bool bIsGround = false;
	PLAYER *pPlayer = GetPlayer();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			for (int nCntPartsEnemy = 0; nCntPartsEnemy < MAX_ENEMY_PARTS; nCntPartsEnemy++)
			{
				//敵キャラの位置を求める処理(敵キャラのサイズ + 敵キャラの位置)
				float EnemyL = g_aX_Enemy[nCntPartsEnemy].vtxMin.x + g_aEnemy[nCntEnemy].pos.x;		//敵キャラの左側
				float EnemyR = g_aX_Enemy[nCntPartsEnemy].vtxMax.x + g_aEnemy[nCntEnemy].pos.x;		//敵キャラの右側

				float EnemyFront = g_aX_Enemy[nCntPartsEnemy].vtxMin.z + g_aEnemy[nCntEnemy].pos.z;	//敵キャラの手前側
				float EnemyBack = g_aX_Enemy[nCntPartsEnemy].vtxMax.z + g_aEnemy[nCntEnemy].pos.z;	//敵キャラの奥側

				float EnemyDown = g_aX_Enemy[nCntPartsEnemy].vtxMin.y + g_aEnemy[nCntEnemy].pos.y;	//敵キャラの下側
				float EnemyTop = g_aX_Enemy[nCntPartsEnemy].vtxMax.y + g_aEnemy[nCntEnemy].pos.y;	//敵キャラの頭側

				//敵キャラの当たり判定
				if (pos->x + r_size->x > EnemyL		&& pos->x - r_size->x < EnemyR
					&&	pos->z + r_size->z > EnemyFront	&& pos->z - r_size->z < EnemyBack
					&&	pos->y + r_size->y > EnemyDown	&& pos->y - r_size->y < EnemyTop)
				{
					//x軸当たり判定
					if (pos->x + r_size->x > EnemyL && posOld->x + r_size->x <= EnemyL)
					{//左側の当たり判定
						pos->x = EnemyL - r_size->x;
						move->x = 0.0f;
						g_aEnemy[nCntEnemy].move.x = 0.0f;
					}
					else if (pos->x - r_size->x < EnemyR && posOld->x - r_size->x >= EnemyR)
					{//右側の当たり判定
						pos->x = EnemyR + r_size->x;
						move->x = 0.0f;
						g_aEnemy[nCntEnemy].move.x = 0.0f;
					}
					//z軸当たり判定
					if (pos->z + r_size->z > EnemyFront && posOld->z + r_size->z <= EnemyFront)
					{//手前側の当たり判定
						pos->z = EnemyFront - r_size->z;
						move->z = 0.0f;
						g_aEnemy[nCntEnemy].move.z = 0.0f;
					}
					else if (pos->z - r_size->z < EnemyBack && posOld->z - r_size->z >= EnemyBack)
					{//奥側の当たり判定
						pos->z = EnemyBack + r_size->z;
						move->z = 0.0f;
						g_aEnemy[nCntEnemy].move.z = 0.0f;
					}
					//y軸当たり判定
					if (pos->y + r_size->y > EnemyDown && posOld->y + r_size->y <= EnemyDown)
					{//下側の当たり判定
						pos->y = EnemyDown - r_size->y;
						move->y = 0.0f;
						g_aEnemy[nCntEnemy].move.y = 0.0f;
					}
					else if (pos->y - r_size->y < EnemyTop && posOld->y - r_size->y >= EnemyTop)
					{//上側の当たり判定
						pos->y = EnemyTop + r_size->y;
						move->y = 0.0f;
						g_aEnemy[nCntEnemy].move.y = 0.0f;

						bIsGround = true;
					}
				}
			}
		}
	}
	return bIsGround;
}

//============================================================================
//敵キャラの設定処理
//============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;
			g_aEnemy[nCntEnemy].type = type;
			g_aEnemy[nCntEnemy].bUse = true;

			if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_POTATE)
			{
				g_aEnemy[nCntEnemy].nLife = 2;
			}
			else if (g_aEnemy[nCntEnemy].type == ENEMY_TYPE_PUNPKIN)
			{
				g_aEnemy[nCntEnemy].nLife = 3;
			}
			break;
		}
	}
}

//============================================================================
//敵キャラの取得処理
//============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}
