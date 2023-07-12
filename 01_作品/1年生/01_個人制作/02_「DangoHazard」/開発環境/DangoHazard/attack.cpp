//============================================================================
//
//攻撃判定の処理[attack.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "attack.h"		//作成したattack.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "camera.h"		//作成したcamera.hをインクルードする
#include "shadow.h"		//作成したshadow.hをインクルードする
#include "effect.h"		//作成したeffect.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include "model.h"		//作成したmodel.hをインクルードする
#include "meshfield.h"	//作成したmeshfield.hをインクルードする
#include <math.h>		//浮動小数点の計算に必要

//****************************************************************************
//グローバル変数
//****************************************************************************
ATTACK g_aAttack[MAX_ATTACK];			//敵キャラ構造体		
X_ATTACK g_aX_Attack[IMAGE_ATTACK];	//xファイル構造体

								//============================================================================
								//攻撃判定の初期化処理
								//============================================================================
void InitAttack(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

										//Xファイルの読み込み
	D3DXLoadMeshFromX("Data\\Model\\Player_collision.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Attack[ATTACK_TYPE_PLAYER].pBuffMat,
		NULL,
		&g_aX_Attack[ATTACK_TYPE_PLAYER].nNumMat,
		&g_aX_Attack[ATTACK_TYPE_PLAYER].pMesh
	);

	//Xファイルの読み込み
	D3DXLoadMeshFromX("Data\\Model\\Player_collision.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Attack[ATTACK_TYPE_ENEMY].pBuffMat,
		NULL,
		&g_aX_Attack[ATTACK_TYPE_ENEMY].nNumMat,
		&g_aX_Attack[ATTACK_TYPE_ENEMY].pMesh
	);

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCntImgAttack = 0; nCntImgAttack < IMAGE_ATTACK; nCntImgAttack++)
	{//パーツの処理
		D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)g_aX_Attack[nCntImgAttack].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aX_Attack[nCntImgAttack].nNumMat; nCntMat++)
		{
			//テクスチャの読み出し
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//テクスチャのファイル名
				&g_aX_Attack[nCntImgAttack].g_pTexture[ATTACK_TYPE_PLAYER]
			);
			//テクスチャの読み出し
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//テクスチャのファイル名
				&g_aX_Attack[nCntImgAttack].g_pTexture[ATTACK_TYPE_ENEMY]
			);
		}

		g_aX_Attack[nCntImgAttack].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aX_Attack[nCntImgAttack].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点数の取得
		nNumVtx = g_aX_Attack[nCntImgAttack].pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aX_Attack[nCntImgAttack].pMesh->GetFVF());
		//頂点バッファのロック
		g_aX_Attack[nCntImgAttack].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代表
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標を比較してモデルの最大値を取得
			if (vtx.x > g_aX_Attack[nCntImgAttack].vtxMax.x)
			{//x座標の最大値
				g_aX_Attack[nCntImgAttack].vtxMax.x = floorf(vtx.x);
			}
			if (vtx.y > g_aX_Attack[nCntImgAttack].vtxMax.y)
			{//y座標の最大値
				g_aX_Attack[nCntImgAttack].vtxMax.y = floorf(vtx.y);
			}
			if (vtx.z > g_aX_Attack[nCntImgAttack].vtxMax.z)
			{//z座標の最大値
				g_aX_Attack[nCntImgAttack].vtxMax.z = floorf(vtx.z);
			}

			//頂点座標を比較してモデルの最小値を取得
			if (vtx.x < g_aX_Attack[nCntImgAttack].vtxMin.x)
			{//x座標の最小値
				g_aX_Attack[nCntImgAttack].vtxMin.x = floorf(vtx.x);
			}
			if (vtx.y < g_aX_Attack[nCntImgAttack].vtxMin.y)
			{//y座標の最小値
				g_aX_Attack[nCntImgAttack].vtxMin.y = floorf(vtx.y);
			}
			if (vtx.z < g_aX_Attack[nCntImgAttack].vtxMin.z)
			{//z座標の最小値
				g_aX_Attack[nCntImgAttack].vtxMin.z = floorf(vtx.z);
			}

			//頂点座標フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}
		//頂点バッファのアンロック
		g_aX_Attack[nCntImgAttack].pMesh->UnlockVertexBuffer();
	}

	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		//位置の初期化処理
		g_aAttack[nCntAttack].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する
		g_aAttack[nCntAttack].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].nLife = 0;
		g_aAttack[nCntAttack].bUse = false;
		g_aAttack[nCntAttack].Worldmtx = D3DXMATRIX();
		g_aAttack[nCntAttack].type = ATTACK_TYPE_PLAYER;
		g_aAttack[nCntAttack].aParts[ATTACK_TYPE_PLAYER].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_aAttack[nCntAttack].aParts[ATTACK_TYPE_PLAYER].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].aParts[ATTACK_TYPE_ENEMY].pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
		g_aAttack[nCntAttack].aParts[ATTACK_TYPE_ENEMY].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

//============================================================================
//攻撃判定の初期化処理
//============================================================================
void UninitAttack(void)
{
	for (int nCntImgAttack = 0; nCntImgAttack < IMAGE_ATTACK; nCntImgAttack++)
	{
		//攻撃判定の破棄
		if (g_aX_Attack[nCntImgAttack].pMesh != NULL)
		{
			g_aX_Attack[nCntImgAttack].pMesh->Release();
			g_aX_Attack[nCntImgAttack].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aX_Attack[nCntImgAttack].pBuffMat != NULL)
		{
			g_aX_Attack[nCntImgAttack].pBuffMat->Release();
			g_aX_Attack[nCntImgAttack].pBuffMat = NULL;
		}

		// xモデルの使用テクスチャを全て解放する(メモリリーク修正_2023/7/12)
		for (int nMatTex = 0; nMatTex < ATTACK_TYPE_MAX; nMatTex++)
		{
			//テクスチャの破棄
			if (g_aX_Attack[nCntImgAttack].g_pTexture[nMatTex] != NULL)
			{
				g_aX_Attack[nCntImgAttack].g_pTexture[nMatTex]->Release();
				g_aX_Attack[nCntImgAttack].g_pTexture[nMatTex] = NULL;
			}
		}
	}
}

//============================================================================
//攻撃判定の更新処理
//============================================================================
void UpdateAttack(void)
{
	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//角度の正規化処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			if (g_aAttack[nCntAttack].rot.y > D3DX_PI)
			{//現在の角度が(PI)より大きい場合
				g_aAttack[nCntAttack].rot.y -= D3DX_PI * 2;
			}
			else if (g_aAttack[nCntAttack].rot.y < -D3DX_PI)
			{//現在の角度が(-PI)より小さい場合
				g_aAttack[nCntAttack].rot.y += D3DX_PI * 2;
			}

			if (g_aAttack[nCntAttack].Destrot.y - g_aAttack[nCntAttack].rot.y > D3DX_PI)
			{//現在の目的値が(PI)より大きい場合
				g_aAttack[nCntAttack].Destrot.y -= 2 * D3DX_PI;
			}
			else if (g_aAttack[nCntAttack].Destrot.y - g_aAttack[nCntAttack].rot.y < -D3DX_PI)
			{//現在の目的地が(-PI)より小さい場合
				g_aAttack[nCntAttack].Destrot.y += 2 * D3DX_PI;
			}

			if (g_aAttack[nCntAttack].nLife <= 0)
			{
				g_aAttack[nCntAttack].bUse = false;
			}

			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//その他の処理
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//攻撃判定の寿命を減らす処理
			g_aAttack[nCntAttack].nLife--;

			//攻撃判定の前回位置を反映
			g_aAttack[nCntAttack].posOld = g_aAttack[nCntAttack].pos;

			//攻撃判定の位置更新
			g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move;
		}
	}
}

//============================================================================
//攻撃判定の描画処理
//============================================================================
void DrawAttack(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aAttack[nCntAttack].Worldmtx);

			//現在のマテリアルを取得する
			pDevice->GetMaterial(&matDef);

			//向きを反映(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAttack[nCntAttack].rot.y, g_aAttack[nCntAttack].rot.x, g_aAttack[nCntAttack].rot.z);
			D3DXMatrixMultiply(&g_aAttack[nCntAttack].Worldmtx, &g_aAttack[nCntAttack].Worldmtx, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aAttack[nCntAttack].pos.x, g_aAttack[nCntAttack].pos.y, g_aAttack[nCntAttack].pos.z);
			D3DXMatrixMultiply(&g_aAttack[nCntAttack].Worldmtx, &g_aAttack[nCntAttack].Worldmtx, &mtxTrans);

			for (int nCntImgAttack = 0; nCntImgAttack < IMAGE_ATTACK; nCntImgAttack++)
			{
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx);

				//向きを反映(YaW : y,Pitch : x,Roll : z)
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAttack[nCntAttack].aParts[nCntImgAttack].rot.y, g_aAttack[nCntAttack].aParts[nCntImgAttack].rot.x, g_aAttack[nCntAttack].aParts[nCntImgAttack].rot.z);
				D3DXMatrixMultiply(&g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &mtxRot);

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aAttack[nCntAttack].aParts[nCntImgAttack].pos.x, g_aAttack[nCntAttack].aParts[nCntImgAttack].pos.y, g_aAttack[nCntAttack].aParts[nCntImgAttack].pos.z);
				D3DXMatrixMultiply(&g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &mtxTrans);

				//親モデルのマトリックスとの掛け算
				D3DXMatrixMultiply(&g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx, &g_aAttack[nCntAttack].Worldmtx);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aAttack[nCntAttack].aParts[nCntImgAttack].Worldmtx);

				//現在のマテリアルのポインタを取得
				pMat = (D3DXMATERIAL*)g_aX_Attack[g_aAttack[nCntAttack].type].pBuffMat->GetBufferPointer();

				//テクスチャの読み出し
				for (int nCntMat = 0; nCntMat < (int)g_aX_Attack[g_aAttack[nCntAttack].type].nNumMat; nCntMat++)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_aX_Attack[g_aAttack[nCntAttack].type].g_pTexture[g_aAttack[nCntAttack].type]);

					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					//モデルパーツの描画
					g_aX_Attack[g_aAttack[nCntAttack].type].pMesh->DrawSubset(nCntMat);
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
//攻撃判定の当たり判定処理
//============================================================================
bool AttackCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size)
{
	PLAYER *pPlayer = GetPlayer();

	bool bDamege = false;

	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//攻撃判定の位置を求める処理(アイテムのサイズ + アイテムの位置)
			float AttackL = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMin.x + g_aAttack[nCntAttack].pos.x;		//アイテムの左側
			float AttackR = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMax.x + g_aAttack[nCntAttack].pos.x;		//アイテムの右側

			float AttackFront = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMin.z + g_aAttack[nCntAttack].pos.z;	//アイテムの手前側
			float AttackBack = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMax.z + g_aAttack[nCntAttack].pos.z;	//アイテムの奥側

			float AttackDown = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMin.y + g_aAttack[nCntAttack].pos.y;	//アイテムの下側
			float AttackTop = g_aX_Attack[g_aAttack[nCntAttack].type].vtxMax.y + g_aAttack[nCntAttack].pos.y;	//アイテムの頭側

																									//アイテムの当たり判定
																									//モデルの当たり判定
			if (pos->x + r_size->x > AttackL		&& pos->x - r_size->x < AttackR
				&&	pos->z + r_size->z > AttackFront	&& pos->z - r_size->z < AttackBack
				&&	pos->y + r_size->y > AttackDown	&& pos->y - r_size->y < AttackTop)
			{
				bDamege = true;
				if (pPlayer->nPower < 3)
				{
					g_aAttack[nCntAttack].bUse = false;	//使用判定を偽にする
				}
				else
				{
					g_aAttack[nCntAttack].bUse = true;	//使用判定を偽にする
				}
				//x軸当たり判定
				if (pos->x + r_size->x > AttackL && posOld->x + r_size->x <= AttackL)
				{//左側の当たり判定
					pos->x = AttackL - r_size->x;
					move->x = 0.0f;
				}
				else if (pos->x - r_size->x < AttackR && posOld->x - r_size->x >= AttackR)
				{//右側の当たり判定
					pos->x = AttackR + r_size->x;
					move->x = 0.0f;
				}
				//z軸当たり判定
				if (pos->z + r_size->z > AttackFront && posOld->z + r_size->z <= AttackFront)
				{//手前側の当たり判定
					pos->z = AttackFront - r_size->z;
					move->z = 0.0f;
				}
				else if (pos->z - r_size->z < AttackBack && posOld->z - r_size->z >= AttackBack)
				{//奥側の当たり判定
					pos->z = AttackBack + r_size->z;
					move->z = 0.0f;
				}
				//y軸当たり判定
				if (pos->y + r_size->y > AttackDown && posOld->y + r_size->y <= AttackDown)
				{//下側の当たり判定
					pos->y = AttackDown - r_size->y;
					move->y = 0.0f;
				}
				else if (pos->y - r_size->y < AttackTop && posOld->y - r_size->y >= AttackTop)
				{//上側の当たり判定
					pos->y = AttackTop + r_size->y;
					move->y = 0.0f;
				}
			}
		}
	}
	return bDamege;
}

//============================================================================
//攻撃判定の設定処理
//============================================================================
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int life, ATTACK_TYPE type)
{
	for (int nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == false)
		{
			g_aAttack[nCntAttack].pos = pos;
			g_aAttack[nCntAttack].move = move;
			g_aAttack[nCntAttack].rot = rot;
			g_aAttack[nCntAttack].nLife = life;
			g_aAttack[nCntAttack].type = type;
			g_aAttack[nCntAttack].bUse = true;
			break;
		}
	}
}

//============================================================================
//攻撃判定の取得処理
//============================================================================
ATTACK *GetAttack(void)
{
	return &g_aAttack[0];
}
