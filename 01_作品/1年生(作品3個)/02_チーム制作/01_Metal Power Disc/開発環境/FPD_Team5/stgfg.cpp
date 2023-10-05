//=========================================
// 
// ステージ前景処理
// Author Tanimoto_Kosuke
//
// Update 22/03/23
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "stgfg.h"
#include <stdio.h>
#include "goal.h"
#include "wall.h"
#include "game.h"
#include "stage.h"
#include "referee.h"

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureStgFg[MAX_IMAGE_STGFG] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffStgFg = NULL;					//頂点バッファへのポインタ
static STGFG s_aStgFg[MAX_STGFG_OBJECT];

//=========================================
// ステージ前景の初期化処理
//=========================================
void InitStgFg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\stage\\wall3.png",	//テクスチャのファイル名
		&s_pTextureStgFg[STGFG_TYPE_WALL]
	);

	//壁(下側)
	s_aStgFg[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, MAX_HEIGHT + 5, 0.0f);
	s_aStgFg[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aStgFg[0].fAngle = atan2f(STAGE_WALL_WIDTH, STAGE_WALL_HEIGHT);
	s_aStgFg[0].fLength = sqrtf((STAGE_WALL_WIDTH * STAGE_WALL_WIDTH) + (STAGE_WALL_HEIGHT * STAGE_WALL_HEIGHT)) / 2.0f;
	s_aStgFg[0].type = STGFG_TYPE_WALL;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_STGFG_OBJECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffStgFg,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffStgFg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStgFg = 0; nCntStgFg < MAX_STGFG_OBJECT; nCntStgFg++, pVtx += 4)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aStgFg[nCntStgFg].pos.x - sinf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[0].pos.y = s_aStgFg[nCntStgFg].pos.y - cosf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[0].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[1].pos.x = s_aStgFg[nCntStgFg].pos.x + sinf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[1].pos.y = s_aStgFg[nCntStgFg].pos.y - cosf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[1].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[2].pos.x = s_aStgFg[nCntStgFg].pos.x - sinf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[2].pos.y = s_aStgFg[nCntStgFg].pos.y + cosf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[2].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[3].pos.x = s_aStgFg[nCntStgFg].pos.x + sinf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[3].pos.y = s_aStgFg[nCntStgFg].pos.y + cosf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[3].pos.z = s_aStgFg[nCntStgFg].pos.z;

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	s_pVtxBuffStgFg->Unlock();

	InitReferee();		// レフェリー
}

//=========================================
// ステージ前景の終了処理
//=========================================
void UninitStgFg(void)
{
	for (int nCntImgStgFg = 0; nCntImgStgFg < MAX_IMAGE_STGFG; nCntImgStgFg++)
	{
		//テクスチャの破棄
		if (s_pTextureStgFg[nCntImgStgFg] != NULL)
		{
			s_pTextureStgFg[nCntImgStgFg]->Release();
			s_pTextureStgFg[nCntImgStgFg] = NULL;
		}
		//頂点バッファの破棄
		if (s_pVtxBuffStgFg != NULL)
		{
			s_pVtxBuffStgFg->Release();
			s_pVtxBuffStgFg = NULL;
		}
	}
	UninitReferee();		// レフェリー
}

//=========================================
// ステージ前景の更新処理
//=========================================
void UpdateStgFg(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffStgFg->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStgFg = 0; nCntStgFg < MAX_STGFG_OBJECT; nCntStgFg++, pVtx += 4)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aStgFg[nCntStgFg].pos.x - sinf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[0].pos.y = s_aStgFg[nCntStgFg].pos.y - cosf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[0].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[1].pos.x = s_aStgFg[nCntStgFg].pos.x + sinf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[1].pos.y = s_aStgFg[nCntStgFg].pos.y - cosf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[1].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[2].pos.x = s_aStgFg[nCntStgFg].pos.x - sinf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[2].pos.y = s_aStgFg[nCntStgFg].pos.y + cosf(s_aStgFg[nCntStgFg].fAngle - s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[2].pos.z = s_aStgFg[nCntStgFg].pos.z;

		pVtx[3].pos.x = s_aStgFg[nCntStgFg].pos.x + sinf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.x) * s_aStgFg[nCntStgFg].fLength;
		pVtx[3].pos.y = s_aStgFg[nCntStgFg].pos.y + cosf(s_aStgFg[nCntStgFg].fAngle + s_aStgFg[nCntStgFg].rot.y) * s_aStgFg[nCntStgFg].fLength;
		pVtx[3].pos.z = s_aStgFg[nCntStgFg].pos.z;
	}
	//頂点バッファをアンロックする
	s_pVtxBuffStgFg->Unlock();

	UpdateReferee();		// レフェリー
}

//=========================================
// ステージ前景の描画処理
//=========================================
void DrawStgFg()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

												//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffStgFg,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntStgFg = 0; nCntStgFg < MAX_STGFG_OBJECT; nCntStgFg++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, s_pTextureStgFg[s_aStgFg[nCntStgFg].type]);

		//ポリゴンの描画
		pDevice->DrawPrimitive
		(
			D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntStgFg * 4,			//描画する最初の頂点インデックス
			2						//プリミティブアイコンの個数
		);
	}

	DrawReferee();		// レフェリー
}