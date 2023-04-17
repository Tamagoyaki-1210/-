//============================================================================
//
//ビルボードの処理[billboard.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "billboard.h"	//作成したbillboard.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "shadow.h"		//作成したshadow.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBillBoard[IMAGE_BILLBOARD] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillBoard = NULL;	//頂点バッファへのポインタ
BILLBOARD g_aBillBoard[MAX_BILLBOARD];
float g_fAngleBullet = atan2f(BILLBOARD_BULLET_WIDTH, BILLBOARD_BULLET_HEIGHT);
float g_fLengthBullet = sqrtf(BILLBOARD_BULLET_WIDTH * BILLBOARD_BULLET_WIDTH + BILLBOARD_BULLET_HEIGHT * BILLBOARD_BULLET_HEIGHT) / 2.0f;
float g_fAngleTree = atan2f(BILLBOARD_TREE_WIDTH, BILLBOARD_TREE_HEIGHT);
float g_fLengthTree = sqrtf(BILLBOARD_TREE_WIDTH * BILLBOARD_TREE_WIDTH + BILLBOARD_TREE_HEIGHT * BILLBOARD_TREE_HEIGHT) / 2.0f;
bool g_ZBillBoardDebug = false;						//Zバッファのデバッグ

//============================================================================
//ビルボードの初期化処理
//============================================================================
void InitBillBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

										//テクスチャーの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\bullet000.png",	//テクスチャのファイル名
		&g_pTextureBillBoard[BILLBOARD_TYPE_BULLET]
	);

	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\tree000.png",	//テクスチャのファイル名
		&g_pTextureBillBoard[BILLBOARD_TYPE_TREE]
	);

	for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	{
		//位置の初期化処理
		g_aBillBoard[nCntBillBoard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillBoard[nCntBillBoard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillBoard[nCntBillBoard].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillBoard[nCntBillBoard].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aBillBoard[nCntBillBoard].IndxShadow = SetShadow(g_aBillBoard[nCntBillBoard].pos, g_aBillBoard[nCntBillBoard].rot);
		g_aBillBoard[nCntBillBoard].fAngle = 0.0f;
		g_aBillBoard[nCntBillBoard].fLength = 0.0f;
		g_aBillBoard[nCntBillBoard].Worldmtx = D3DXMATRIX();
		g_aBillBoard[nCntBillBoard].bUse = false;
		g_aBillBoard[nCntBillBoard].type = BILLBOARD_TYPE_BULLET;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillBoard,
		NULL);


	SetBillBoard(D3DXVECTOR3(20.0f, 2.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARD_TYPE_BULLET, g_fAngleBullet, g_fLengthBullet);
	SetBillBoard(D3DXVECTOR3(-48.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARD_TYPE_TREE, g_fAngleTree, g_fLengthTree);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillBoard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = -sinf(g_aBillBoard[nCntBillBoard].fAngle + g_aBillBoard[nCntBillBoard].rot.x) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[0].pos.y = +cosf(g_aBillBoard[nCntBillBoard].fAngle + g_aBillBoard[nCntBillBoard].rot.z) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.x) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[1].pos.y = +cosf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.z) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = -sinf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.x) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[2].pos.y = -cosf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.z) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(g_aBillBoard[nCntBillBoard].fAngle + g_aBillBoard[nCntBillBoard].rot.x) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[3].pos.y = -cosf(g_aBillBoard[nCntBillBoard].fAngle - g_aBillBoard[nCntBillBoard].rot.z) * g_aBillBoard[nCntBillBoard].fLength;
		pVtx[3].pos.z = 0.0f;

		//各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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
	g_pVtxBuffBillBoard->Unlock();
}

//============================================================================
//ビルボードの終了処理
//============================================================================
void UninitBillBoard(void)
{
	for (int nCntBillBoard = 0; nCntBillBoard < IMAGE_BILLBOARD; nCntBillBoard++)
	{
		//テクスチャの破棄
		if (g_pTextureBillBoard[nCntBillBoard] != NULL)
		{
			g_pTextureBillBoard[nCntBillBoard]->Release();
			g_pTextureBillBoard[nCntBillBoard] = NULL;

		}
		//頂点バッファの破棄
		if (g_pVtxBuffBillBoard != NULL)
		{
			g_pVtxBuffBillBoard->Release();
			g_pVtxBuffBillBoard = NULL;
		}
	}
}

//============================================================================
//ビルボードの更新処理
//============================================================================
void UpdateBillBoard(void)
{
	//for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	//{
	//	if (g_aBillBoard[nCntBillBoard].bUse == true)
	//	{
	//		SetpositionShadow(g_aBillBoard[nCntBillBoard].IndxShadow, D3DXVECTOR3(g_aBillBoard[nCntBillBoard].pos.x, 0.0f, g_aBillBoard[nCntBillBoard].pos.z),
	//			SHADOW_WIDTH + (g_aBillBoard[nCntBillBoard].pos.y / 2), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1 - (g_aBillBoard[nCntBillBoard].pos.y / 100)));
	//	}
	//}

	if (GetKeyboardTrigger(DIK_F7) == true)
	{//Zバッファテスト
		g_ZBillBoardDebug = !g_ZBillBoardDebug;
	}
}

//============================================================================
//ビルボードの描画処理
//============================================================================
void DrawBillBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	{
		if (g_aBillBoard[nCntBillBoard].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBillBoard[nCntBillBoard].Worldmtx);

			D3DXMATRIX mtxView;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//カメラの逆行列を設定
			g_aBillBoard[nCntBillBoard].Worldmtx._11 = mtxView._11;
			g_aBillBoard[nCntBillBoard].Worldmtx._12 = mtxView._21;
			g_aBillBoard[nCntBillBoard].Worldmtx._13 = mtxView._31;

			if (g_aBillBoard[nCntBillBoard].type == BILLBOARD_TYPE_BULLET)
			{
				g_aBillBoard[nCntBillBoard].Worldmtx._21 = mtxView._12;
				g_aBillBoard[nCntBillBoard].Worldmtx._22 = mtxView._22;
				g_aBillBoard[nCntBillBoard].Worldmtx._23 = mtxView._32;
			}

			g_aBillBoard[nCntBillBoard].Worldmtx._31 = mtxView._13;
			g_aBillBoard[nCntBillBoard].Worldmtx._32 = mtxView._23;
			g_aBillBoard[nCntBillBoard].Worldmtx._33 = mtxView._33;

			////向きを反映(YaW : y,Pitch : x,Roll : z)
			//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBillBoard[nCntBillBoard].rot.y, g_aBillBoard[nCntBillBoard].rot.x, g_aBillBoard[nCntBillBoard].rot.z);
			//D3DXMatrixMultiply(&g_aBillBoard[nCntBillBoard].Worldmtx, &g_aBillBoard[nCntBillBoard].Worldmtx, &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBillBoard[nCntBillBoard].pos.x, g_aBillBoard[nCntBillBoard].pos.y, g_aBillBoard[nCntBillBoard].pos.z);
			D3DXMatrixMultiply(&g_aBillBoard[nCntBillBoard].Worldmtx, &g_aBillBoard[nCntBillBoard].Worldmtx, &mtxTrans);

			//ライトを無効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			//Zバッファの値が手前のZバッファの値より小さい場合
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			if (g_ZBillBoardDebug == false)
			{
				//Zバッファを描画する
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}
			else
			{
				//Zバッファを描画する
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBillBoard[nCntBillBoard].Worldmtx);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillBoard, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBillBoard[g_aBillBoard[nCntBillBoard].type]);

			//ポリゴンを描画する
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillBoard * 4, 2);

			//Zバッファを描画する
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//Zバッファの値が手前のZバッファの値より小さい場合
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}
	//アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//ライトを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//============================================================================
//ビルボードの設定処理
//============================================================================
void SetBillBoard(D3DXVECTOR3 pos, D3DXVECTOR3 rot, BILLBOARD_TYPE type, float fAngle, float fLength)
{
	for (int nCntBillBoard = 0; nCntBillBoard < MAX_BILLBOARD; nCntBillBoard++)
	{
		if (g_aBillBoard[nCntBillBoard].bUse == false)
		{
			g_aBillBoard[nCntBillBoard].pos = pos;
			g_aBillBoard[nCntBillBoard].rot = rot;
			g_aBillBoard[nCntBillBoard].type = type;
			g_aBillBoard[nCntBillBoard].fAngle = fAngle;
			g_aBillBoard[nCntBillBoard].fLength = fLength;
			g_aBillBoard[nCntBillBoard].bUse = true;
			break;
		}
	}
}
