//============================================================================
//
//モデルの処理[model.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "model.h"		//作成したmodel.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "camera.h"		//作成したcamera.hをインクルードする
#include "shadow.h"		//作成したshadow.hをインクルードする
#include "effect.h"		//作成したeffect.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする
#include <math.h>		//浮動小数点の計算に必要

//****************************************************************************
//グローバル変数
//****************************************************************************
MODEL g_aModel[MAX_MODEL];			//モデル構造体		
X_MODEL g_aX_Model[IMAGE_MODEL];	//xファイル構造体

//============================================================================
//モデルの初期化処理
//============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

	for (int nCntModel = 0;nCntModel < MAX_MODEL; nCntModel++)
	{
		//位置の初期化処理
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f,0.0f,0.0f);
		g_aModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].Destrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きを初期化する
		g_aModel[nCntModel].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].fAngle = atan2f(MODEL_WIDTH, MODEL_HEIGHT);
		g_aModel[nCntModel].fLength = sqrtf(MODEL_WIDTH * MODEL_WIDTH + MODEL_HEIGHT * MODEL_HEIGHT) / 2.0f;
		g_aModel[nCntModel].bUse = false;
		g_aModel[nCntModel].Worldmtx = D3DXMATRIX();
		g_aModel[nCntModel].type = MODEL_TYPE_ROCK;
	}

	//Xファイルの読み込み
	D3DXLoadMeshFromX("Data\\Model\\rock000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Model[MODEL_TYPE_ROCK].pBuffMat,
		NULL,
		&g_aX_Model[MODEL_TYPE_ROCK].nNumMat,
		&g_aX_Model[MODEL_TYPE_ROCK].pMesh
	);

	//Xファイルの読み込み
	D3DXLoadMeshFromX("Data\\Model\\stone000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Model[MODEL_TYPE_STONE].pBuffMat,
		NULL,
		&g_aX_Model[MODEL_TYPE_STONE].nNumMat,
		&g_aX_Model[MODEL_TYPE_STONE].pMesh
	);

	//Xファイルの読み込み
	D3DXLoadMeshFromX("Data\\Model\\tree000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aX_Model[MODEL_TYPE_TREE].pBuffMat,
		NULL,
		&g_aX_Model[MODEL_TYPE_TREE].nNumMat,
		&g_aX_Model[MODEL_TYPE_TREE].pMesh
	);

	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	for (int nCntImgModel = 0; nCntImgModel < IMAGE_MODEL; nCntImgModel++)
	{
		D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
		pMat = (D3DXMATERIAL*)g_aX_Model[nCntImgModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aX_Model[nCntImgModel].nNumMat; nCntMat++)
		{
			//テクスチャの読み出し
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//テクスチャのファイル名
				&g_aX_Model[nCntImgModel].g_pTexture[MODEL_TYPE_ROCK]
			);

			//テクスチャの読み出し
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//テクスチャのファイル名
				&g_aX_Model[nCntImgModel].g_pTexture[MODEL_TYPE_STONE]
			);

			//テクスチャの読み出し
			D3DXCreateTextureFromFile(
				pDevice,
				"Data\\TEXTURE\\mofu.jpg",	//テクスチャのファイル名
				&g_aX_Model[nCntImgModel].g_pTexture[MODEL_TYPE_TREE]
			);
		}

		g_aX_Model[nCntImgModel].vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aX_Model[nCntImgModel].vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
		//頂点数の取得
		nNumVtx = g_aX_Model[nCntImgModel].pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aX_Model[nCntImgModel].pMesh->GetFVF());
		//頂点バッファのロック
		g_aX_Model[nCntImgModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代表
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標を比較してモデルの最大値を取得
			if (vtx.x > g_aX_Model[nCntImgModel].vtxMax.x)
			{//x座標の最大値
				g_aX_Model[nCntImgModel].vtxMax.x = floorf(vtx.x);
			}
			if (vtx.y > g_aX_Model[nCntImgModel].vtxMax.y)
			{//y座標の最大値
				g_aX_Model[nCntImgModel].vtxMax.y = floorf(vtx.y);
			}
			if (vtx.z > g_aX_Model[nCntImgModel].vtxMax.z)
			{//z座標の最大値
				g_aX_Model[nCntImgModel].vtxMax.z = floorf(vtx.z);
			}

			//頂点座標を比較してモデルの最小値を取得
			if (vtx.x < g_aX_Model[nCntImgModel].vtxMin.x)
			{//x座標の最小値
				g_aX_Model[nCntImgModel].vtxMin.x = floorf(vtx.x);
			}
			if (vtx.y < g_aX_Model[nCntImgModel].vtxMin.y)
			{//y座標の最小値
				g_aX_Model[nCntImgModel].vtxMin.y = floorf(vtx.y);
			}
			if (vtx.z < g_aX_Model[nCntImgModel].vtxMin.z)
			{//z座標の最小値
				g_aX_Model[nCntImgModel].vtxMin.z = floorf(vtx.z);
			}

			//頂点座標フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}
		//頂点バッファのアンロック
		g_aX_Model[nCntImgModel].pMesh->UnlockVertexBuffer();
	}

	//位置、角度、種類
	SetModel(D3DXVECTOR3(100.0f, -10.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_TYPE_ROCK);
	SetModel(D3DXVECTOR3(-100.0f, -10.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_TYPE_STONE);
	SetModel(D3DXVECTOR3(100.0f, -10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), MODEL_TYPE_TREE);
}

//============================================================================
//モデルの初期化処理
//============================================================================
void UninitModel(void)
{
	for (int nCntImgModel = 0; nCntImgModel < IMAGE_MODEL; nCntImgModel++)
	{
		//メッシュの破棄
		if (g_aX_Model[nCntImgModel].pMesh != NULL)
		{
			g_aX_Model[nCntImgModel].pMesh->Release();
			g_aX_Model[nCntImgModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aX_Model[nCntImgModel].pBuffMat != NULL)
		{
			g_aX_Model[nCntImgModel].pBuffMat->Release();
			g_aX_Model[nCntImgModel].pBuffMat = NULL;
		}

		// xモデルの使用テクスチャを全て解放する(メモリリーク修正_2023/7/12)
		for (int nMatTex = 0; nMatTex < MODEL_TYPE_MAX; nMatTex++)
		{
			//テクスチャの破棄
			if (g_aX_Model[nCntImgModel].g_pTexture[nMatTex] != NULL)
			{
				g_aX_Model[nCntImgModel].g_pTexture[nMatTex]->Release();
				g_aX_Model[nCntImgModel].g_pTexture[nMatTex] = NULL;
			}
		}
	}
}

//============================================================================
//モデルの更新処理
//============================================================================
void UpdateModel(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{

		}
	}
}

//============================================================================
//モデルの描画処理
//============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aModel[nCntModel].Worldmtx);
			//現在のマテリアルを取得する
			pDevice->GetMaterial(&matDef);

			//向きを反映(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].Worldmtx, &g_aModel[nCntModel].Worldmtx, &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].Worldmtx, &g_aModel[nCntModel].Worldmtx, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].Worldmtx);

			//現在のマテリアルのポインタを取得
			pMat = (D3DXMATERIAL*)g_aX_Model[g_aModel[nCntModel].type].pBuffMat->GetBufferPointer();

			//テクスチャの読み出し
			for (int nCntMat = 0; nCntMat < (int)g_aX_Model[g_aModel[nCntModel].type].nNumMat; nCntMat++)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_aX_Model[g_aModel[nCntModel].type].g_pTexture[g_aModel[nCntModel].type]);
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				//モデルパーツの描画
				g_aX_Model[g_aModel[nCntModel].type].pMesh->DrawSubset(nCntMat);
			}
			//テクスチャの初期化(バグ修正_2023/7/12)
			pDevice->SetTexture(0, NULL);

			//保持してたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//============================================================================
//モデルの当たり判定処理
//============================================================================
bool ModelCollision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *r_size)
{
	bool bIsGround = false;
	PLAYER *pPlayer = GetPlayer();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
			//モデルの位置を求める処理(モデルのサイズ + モデルの位置)
			float ModelL = g_aX_Model[g_aModel[nCntModel].type].vtxMin.x + g_aModel[nCntModel].pos.x;		//モデルの左側
			float ModelR = g_aX_Model[g_aModel[nCntModel].type].vtxMax.x + g_aModel[nCntModel].pos.x;		//モデルの右側

			float ModelFront = g_aX_Model[g_aModel[nCntModel].type].vtxMin.z + g_aModel[nCntModel].pos.z;	//モデルの手前側
			float ModelBack = g_aX_Model[g_aModel[nCntModel].type].vtxMax.z + g_aModel[nCntModel].pos.z;	//モデルの奥側

			float ModelDown = g_aX_Model[g_aModel[nCntModel].type].vtxMin.y + g_aModel[nCntModel].pos.y;	//モデルの下側
			float ModelTop = g_aX_Model[g_aModel[nCntModel].type].vtxMax.y + g_aModel[nCntModel].pos.y;		//モデルの頭側

			//モデルの当たり判定
			if (	pos->x + r_size->x > ModelL		&& pos->x - r_size->x < ModelR
				&&	pos->z + r_size->z > ModelFront	&& pos->z - r_size->z < ModelBack
				&&	pos->y + r_size->y > ModelDown	&& pos->y - r_size->y < ModelTop)
			{
				//x軸当たり判定
				if (pos->x + r_size->x > ModelL && posOld->x + r_size->x <= ModelL)
				{//左側の当たり判定
					pos->x = ModelL - r_size->x;
					move->x = 0.0f;
				}
				else if (pos->x - r_size->x < ModelR && posOld->x - r_size->x >= ModelR)
				{//右側の当たり判定
					pos->x = ModelR + r_size->x;
					move->x = 0.0f;
				}
				//z軸当たり判定
				if (pos->z + r_size->z > ModelFront && posOld->z + r_size->z <= ModelFront)
				{//手前側の当たり判定
					pos->z = ModelFront - r_size->z;
					move->z = 0.0f;
				}
				else if (pos->z - r_size->z < ModelBack && posOld->z - r_size->z >= ModelBack)
				{//奥側の当たり判定
					pos->z = ModelBack + r_size->z;
					move->z = 0.0f;
				}
				//y軸当たり判定
				if (pos->y + r_size->y > ModelDown && posOld->y + r_size->y <= ModelDown)
				{//下側の当たり判定
					pos->y = ModelDown - r_size->y;
					move->y = 0.0f;
				}
				else if (pos->y - r_size->y < ModelTop && posOld->y - r_size->y >= ModelTop)
				{//上側の当たり判定
					pos->y = ModelTop + r_size->y;
					move->y = 0.0f;

					bIsGround = true;
				}
			}
		}
	}
	return bIsGround;
}

//============================================================================
//モデルの設定処理
//============================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODEL_TYPE type)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{
			g_aModel[nCntModel].pos = pos;
			g_aModel[nCntModel].rot = rot;
			g_aModel[nCntModel].type = type;
			g_aModel[nCntModel].bUse = true;
			break;
		}
	}
}

//============================================================================
//モデルの取得処理
//============================================================================
MODEL *GetModel(void)
{
		return &g_aModel[0];
}