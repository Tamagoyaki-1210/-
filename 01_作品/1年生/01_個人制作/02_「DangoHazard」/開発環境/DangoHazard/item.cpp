//============================================================================
//
//アイテムの処理[item.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "item.h"		//作成したitem.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "camera.h"		//作成したcamera.hをインクルードする
#include "shadow.h"		//作成したshadow.hをインクルードする
#include "effect.h"		//作成したeffect.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "model.h"		//作成したmodel.hをインクルードする
#include "meshfield.h"	//作成したmeshfield.hをインクルードする
#include "score.h"		//作成したscore.hをインクルードする
#include <math.h>		//浮動小数点の計算に必要

//****************************************************************************
//グローバル変数
//****************************************************************************
ITEM g_aItem[MAX_ITEM];			//敵キャラ構造体		
X_ITEM g_aX_Item[IMAGE_ITEM];	//xファイル構造体

									//============================================================================
									//アイテムの初期化処理
									//============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

	//Xファイルの読み込み
	D3DXLoadMeshFromX("Data\\Model\\Corn.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Item[ITEM_TYPE_CORN].pBuffMat,
		NULL,
		&g_aX_Item[ITEM_TYPE_CORN].nNumMat,
		&g_aX_Item[ITEM_TYPE_CORN].pMesh
	);

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCntImgItem = 0; nCntImgItem < IMAGE_ITEM; nCntImgItem++)
	{//パーツの処理
		D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)g_aX_Item[nCntImgItem].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aX_Item[nCntImgItem].nNumMat; nCntMat++)
		{
			//テクスチャの読み出し
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\Corn.jpg",	//テクスチャのファイル名
				&g_aX_Item[nCntImgItem].g_pTexture[ITEM_TYPE_CORN]
			);
		}

		g_aX_Item[nCntImgItem].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aX_Item[nCntImgItem].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点数の取得
		nNumVtx = g_aX_Item[nCntImgItem].pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aX_Item[nCntImgItem].pMesh->GetFVF());
		//頂点バッファのロック
		g_aX_Item[nCntImgItem].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代表
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標を比較してモデルの最大値を取得
			if (vtx.x > g_aX_Item[nCntImgItem].vtxMax.x)
			{//x座標の最大値
				g_aX_Item[nCntImgItem].vtxMax.x = floorf(vtx.x);
			}
			if (vtx.y > g_aX_Item[nCntImgItem].vtxMax.y)
			{//y座標の最大値
				g_aX_Item[nCntImgItem].vtxMax.y = floorf(vtx.y);
			}
			if (vtx.z > g_aX_Item[nCntImgItem].vtxMax.z)
			{//z座標の最大値
				g_aX_Item[nCntImgItem].vtxMax.z = floorf(vtx.z);
			}

			//頂点座標を比較してモデルの最小値を取得
			if (vtx.x < g_aX_Item[nCntImgItem].vtxMin.x)
			{//x座標の最小値
				g_aX_Item[nCntImgItem].vtxMin.x = floorf(vtx.x);
			}
			if (vtx.y < g_aX_Item[nCntImgItem].vtxMin.y)
			{//y座標の最小値
				g_aX_Item[nCntImgItem].vtxMin.y = floorf(vtx.y);
			}
			if (vtx.z < g_aX_Item[nCntImgItem].vtxMin.z)
			{//z座標の最小値
				g_aX_Item[nCntImgItem].vtxMin.z = floorf(vtx.z);
			}

			//頂点座標フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}
		//頂点バッファのアンロック
		g_aX_Item[nCntImgItem].pMesh->UnlockVertexBuffer();
	}

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//位置の初期化処理
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する
		g_aItem[nCntItem].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].fAngle = atan2f(ITEM_WIDTH, ITEM_HEIGHT);
		g_aItem[nCntItem].fLength = sqrtf(ITEM_WIDTH * ITEM_WIDTH + ITEM_HEIGHT * ITEM_HEIGHT) / 2.0f;
		g_aItem[nCntItem].bUse = false;
		g_aItem[nCntItem].Worldmtx = D3DXMATRIX();
		g_aItem[nCntItem].type = ITEM_TYPE_CORN;
		g_aItem[nCntItem].aParts[ITEM_TYPE_CORN].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_aItem[nCntItem].aParts[ITEM_TYPE_CORN].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//============================================================================
//アイテムの初期化処理
//============================================================================
void UninitItem(void)
{
	for (int nCntImgItem = 0; nCntImgItem < IMAGE_ITEM; nCntImgItem++)
	{
		//アイテムの破棄
		if (g_aX_Item[nCntImgItem].pMesh != NULL)
		{
			g_aX_Item[nCntImgItem].pMesh->Release();
			g_aX_Item[nCntImgItem].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aX_Item[nCntImgItem].pBuffMat != NULL)
		{
			g_aX_Item[nCntImgItem].pBuffMat->Release();
			g_aX_Item[nCntImgItem].pBuffMat = NULL;
		}

		// xモデルの使用テクスチャを全て解放する(メモリリーク修正_2023/7/12)
		for (int nMatTex = 0; nMatTex < ITEM_TYPE_MAX; nMatTex++)
		{
			//テクスチャの破棄
			if (g_aX_Item[nCntImgItem].g_pTexture[nMatTex] != NULL)
			{
				g_aX_Item[nCntImgItem].g_pTexture[nMatTex]->Release();
				g_aX_Item[nCntImgItem].g_pTexture[nMatTex] = NULL;
			}
		}
	}
}

//============================================================================
//アイテムの更新処理
//============================================================================
void UpdateItem(void)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//角度の正規化処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aItem[nCntItem].rot.y > D3DX_PI)
			{//現在の角度が(PI)より大きい場合
				g_aItem[nCntItem].rot.y -= D3DX_PI * 2;
			}
			else if (g_aItem[nCntItem].rot.y < -D3DX_PI)
			{//現在の角度が(-PI)より小さい場合
				g_aItem[nCntItem].rot.y += D3DX_PI * 2;
			}

			if (g_aItem[nCntItem].Destrot.y - g_aItem[nCntItem].rot.y > D3DX_PI)
			{//現在の目的値が(PI)より大きい場合
				g_aItem[nCntItem].Destrot.y -= 2 * D3DX_PI;
			}
			else if (g_aItem[nCntItem].Destrot.y - g_aItem[nCntItem].rot.y < -D3DX_PI)
			{//現在の目的地が(-PI)より小さい場合
				g_aItem[nCntItem].Destrot.y += 2 * D3DX_PI;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//落ちたら復帰する処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aItem[nCntItem].pos.y <= -100)
			{
				g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//その他の処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//アイテムを回転させる処理
			g_aItem[nCntItem].rot.y += 0.015f;

			//アイテムの前回位置を反映
			g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;

			//アイテムの位置更新
			g_aItem[nCntItem].pos += g_aItem[nCntItem].move * 0.1f;

			//重力を更新(増幅させる)
			g_aItem[nCntItem].move.y -= 4.0f;

			//移動量を更新(減衰させる)
			g_aItem[nCntItem].move -= g_aItem[nCntItem].move * 0.1f;

			//アイテムのサイズの半径を求める処理
			D3DXVECTOR3 r_size = (g_aX_Item[g_aItem[nCntItem].type].vtxMax - g_aX_Item[g_aItem[nCntItem].type].vtxMin) / 2;

			//モデル当たり判定の処理
			ModelCollision(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, &g_aItem[nCntItem].move, &r_size);

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//床の判定処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//メッシュフィールド当たり判定の処理
			FieldCollision(&g_aItem[nCntItem].pos, &g_aItem[nCntItem].posOld, &g_aItem[nCntItem].move);
		}
	}
}

//============================================================================
//アイテムの描画処理
//============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aItem[nCntItem].Worldmtx);

			//現在のマテリアルを取得する
			pDevice->GetMaterial(&matDef);

			//向きを反映(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].Worldmtx, &g_aItem[nCntItem].Worldmtx, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_aItem[nCntItem].Worldmtx, &g_aItem[nCntItem].Worldmtx, &mtxTrans);

			for (int nCntImgItem = 0; nCntImgItem < IMAGE_ITEM; nCntImgItem++)
			{
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx);

				//向きを反映(YaW : y,Pitch : x,Roll : z)
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].aParts[nCntImgItem].rot.y, g_aItem[nCntItem].aParts[nCntImgItem].rot.x, g_aItem[nCntItem].aParts[nCntImgItem].rot.z);
				D3DXMatrixMultiply(&g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &mtxRot);

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].aParts[nCntImgItem].pos.x, g_aItem[nCntItem].aParts[nCntImgItem].pos.y, g_aItem[nCntItem].aParts[nCntImgItem].pos.z);
				D3DXMatrixMultiply(&g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &mtxTrans);

				//親モデルのマトリックスとの掛け算
				D3DXMatrixMultiply(&g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx, &g_aItem[nCntItem].Worldmtx);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].aParts[nCntImgItem].Worldmtx);

				//現在のマテリアルのポインタを取得
				pMat = (D3DXMATERIAL*)g_aX_Item[g_aItem[nCntItem].type].pBuffMat->GetBufferPointer();

				//テクスチャの読み出し
				for (int nCntMat = 0; nCntMat < (int)g_aX_Item[g_aItem[nCntItem].type].nNumMat; nCntMat++)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_aX_Item[g_aItem[nCntItem].type].g_pTexture[g_aItem[nCntItem].type]);

					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					//モデルパーツの描画
					g_aX_Item[g_aItem[nCntItem].type].pMesh->DrawSubset(nCntMat);
				}
			}
			//テクスチャの初期化(バグ修正_2023/7/12)
			pDevice->SetTexture(0, NULL);

			//保持してたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//============================================================================
//アイテムの当たり判定処理
//============================================================================
bool ItemCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size)
{
	bool bItem = false;
	PLAYER *pPlayer = GetPlayer();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//アイテムの位置を求める処理(アイテムのサイズ + アイテムの位置)
			float ItemL = g_aX_Item[g_aItem[nCntItem].type].vtxMin.x + g_aItem[nCntItem].pos.x;		//アイテムの左側
			float ItemR = g_aX_Item[g_aItem[nCntItem].type].vtxMax.x + g_aItem[nCntItem].pos.x;		//アイテムの右側

			float ItemFront = g_aX_Item[g_aItem[nCntItem].type].vtxMin.z + g_aItem[nCntItem].pos.z;	//アイテムの手前側
			float ItemBack = g_aX_Item[g_aItem[nCntItem].type].vtxMax.z + g_aItem[nCntItem].pos.z;	//アイテムの奥側

			float ItemDown = g_aX_Item[g_aItem[nCntItem].type].vtxMin.y + g_aItem[nCntItem].pos.y;	//アイテムの下側
			float ItemTop = g_aX_Item[g_aItem[nCntItem].type].vtxMax.y + g_aItem[nCntItem].pos.y;	//アイテムの頭側

																									//敵キャラの当たり判定
			if (pos->x + r_size->x > ItemL		&& pos->x - r_size->x < ItemR
				&&	pos->z + r_size->z > ItemFront	&& pos->z - r_size->z < ItemBack
				&&	pos->y + r_size->y > ItemDown	&& pos->y - r_size->y < ItemTop)
			{
				bItem = true;
				g_aItem[nCntItem].bUse = false;	//使用判定を偽にする
				AddScore(100);
			}
		}
	}
	return bItem;
}

//============================================================================
//アイテムの設定処理
//============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ITEM_TYPE type)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].rot = rot;
			g_aItem[nCntItem].type = type;
			g_aItem[nCntItem].bUse = true;
			break;
		}
	}
}

//============================================================================
//アイテムの取得処理
//============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}
