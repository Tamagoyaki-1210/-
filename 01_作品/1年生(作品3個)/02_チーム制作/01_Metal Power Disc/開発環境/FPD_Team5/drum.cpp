//=========================================
// 
// ドラム缶の処理
// Author Tanimoto_Kosuke
//
// Update 22/03/22
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "drum.h"
#include <stdio.h>
#include "disk.h"
#include "stage.h"
#include "score.h"
#include "game.h"
#include "pop.h"
#include "goal.h"

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureDrum[MAX_IMAGE_DRUM] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffDrum = NULL;	//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9		s_pTexturePost = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffPost = NULL;	//頂点バッファへのポインタ
static DRUM s_aDrum[MAX_DRUM];
static D3DXVECTOR3 s_posPost;

//=========================================
// ドラム缶の初期化処理
//=========================================
void InitDrum(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\Drum4.png",	//テクスチャのファイル名
		&s_pTextureDrum[DRUM_TYPE_NORMAL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\Drum5.png",	//テクスチャのファイル名
		&s_pTextureDrum[DRUM_TYPE_STRIKE]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\post.png",	//テクスチャのファイル名
		&s_pTexturePost
	);

	//ゴールの位置
	SetDrum(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, DRUM_TYPE_NORMAL, 0);
	SetDrum(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, SCREEN_HEIGHT / 2 + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, DRUM_TYPE_STRIKE, 1);
	SetDrum(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, SCREEN_HEIGHT - 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, DRUM_TYPE_NORMAL, 2);
	SetDrum(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, DRUM_TYPE_NORMAL, 3);
	SetDrum(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, DRUM_TYPE_STRIKE, 4);
	SetDrum(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, SCREEN_HEIGHT - 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, DRUM_TYPE_NORMAL, 5);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_DRUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffDrum,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffDrum->Lock(0, 0, (void**)&pVtx, 0);

	//構造体の初期化処理
	for (int nCntDrum = 0; nCntDrum < MAX_DRUM; nCntDrum++, pVtx += 4)
	{
		s_aDrum[nCntDrum].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aDrum[nCntDrum].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aDrum[nCntDrum].bUse = true;

		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aDrum[nCntDrum].pos.x - sinf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
		pVtx[0].pos.y = s_aDrum[nCntDrum].pos.y - cosf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
		pVtx[0].pos.z = s_aDrum[nCntDrum].pos.z;

		pVtx[1].pos.x = s_aDrum[nCntDrum].pos.x + sinf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
		pVtx[1].pos.y = s_aDrum[nCntDrum].pos.y - cosf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
		pVtx[1].pos.z = s_aDrum[nCntDrum].pos.z;

		pVtx[2].pos.x = s_aDrum[nCntDrum].pos.x - sinf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
		pVtx[2].pos.y = s_aDrum[nCntDrum].pos.y + cosf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
		pVtx[2].pos.z = s_aDrum[nCntDrum].pos.z;

		pVtx[3].pos.x = s_aDrum[nCntDrum].pos.x + sinf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
		pVtx[3].pos.y = s_aDrum[nCntDrum].pos.y + cosf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
		pVtx[3].pos.z = s_aDrum[nCntDrum].pos.z;

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
	s_pVtxBuffDrum->Unlock();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_POST,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPost,
		NULL
	);

	s_posPost = D3DXVECTOR3(GOAL_WIDTH * 0.5f + 50.0f, SCREEN_HEIGHT * 0.5f - 20.0f, 0.0f);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPost->Lock(0, 0, (void**)&pVtx, 0);

	//構造体の初期化処理
	for (int nCntDrum = 0; nCntDrum < MAX_POST; nCntDrum++, pVtx += 4)
	{
		pVtx[0].pos.x = s_posPost.x + -GOAL_WIDTH;
		pVtx[0].pos.y = s_posPost.y + -GOAL_HEIGHT * 2.0f;
		pVtx[0].pos.z = s_posPost.z + 0.0f;

		pVtx[1].pos.x = s_posPost.x + GOAL_WIDTH * 0.75f;
		pVtx[1].pos.y = s_posPost.y + -GOAL_HEIGHT * 2.0f;
		pVtx[1].pos.z = s_posPost.z + 0.0f;

		pVtx[2].pos.x = s_posPost.x + -GOAL_WIDTH;
		pVtx[2].pos.y = s_posPost.y + GOAL_HEIGHT * 2.0f;
		pVtx[2].pos.z = s_posPost.z + 0.0f;

		pVtx[3].pos.x = s_posPost.x + GOAL_WIDTH * 0.75f;
		pVtx[3].pos.y = s_posPost.y + GOAL_HEIGHT * 2.0f;
		pVtx[3].pos.z = s_posPost.z + 0.0f;

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
		pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	}
	//頂点バッファをアンロックする
	s_pVtxBuffPost->Unlock();
}

//=========================================
// ドラム缶の終了処理
//=========================================
void UninitDrum(void)
{
	for (int nCntImgDrum = 0; nCntImgDrum < MAX_IMAGE_DRUM; nCntImgDrum++)
	{
		//テクスチャの破棄
		if (s_pTextureDrum[nCntImgDrum] != NULL)
		{
			s_pTextureDrum[nCntImgDrum]->Release();
			s_pTextureDrum[nCntImgDrum] = NULL;
		}
		//頂点バッファの破棄
		if (s_pVtxBuffDrum != NULL)
		{
			s_pVtxBuffDrum->Release();
			s_pVtxBuffDrum = NULL;
		}
	}

	//テクスチャの破棄
	if (s_pTexturePost != NULL)
	{
		s_pTexturePost->Release();
		s_pTexturePost = NULL;
	}
	//頂点バッファの破棄
	if (s_pVtxBuffPost != NULL)
	{
		s_pVtxBuffPost->Release();
		s_pVtxBuffPost = NULL;
	}
}

//=========================================
// ドラム缶の更新処理
//=========================================
void UpdateDrum(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

									//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffDrum->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDrum = 0; nCntDrum < MAX_DRUM; nCntDrum++, pVtx += 4)
	{
		if (s_aDrum[nCntDrum].bUse == true)
		{
			//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
			pVtx[0].pos.x = s_aDrum[nCntDrum].pos.x - sinf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
			pVtx[0].pos.y = s_aDrum[nCntDrum].pos.y - cosf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
			pVtx[0].pos.z = s_aDrum[nCntDrum].pos.z;

			pVtx[1].pos.x = s_aDrum[nCntDrum].pos.x + sinf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
			pVtx[1].pos.y = s_aDrum[nCntDrum].pos.y - cosf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
			pVtx[1].pos.z = s_aDrum[nCntDrum].pos.z;

			pVtx[2].pos.x = s_aDrum[nCntDrum].pos.x - sinf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
			pVtx[2].pos.y = s_aDrum[nCntDrum].pos.y + cosf(s_aDrum[nCntDrum].fAngle - s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
			pVtx[2].pos.z = s_aDrum[nCntDrum].pos.z;

			pVtx[3].pos.x = s_aDrum[nCntDrum].pos.x + sinf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.x) * s_aDrum[nCntDrum].fLength;
			pVtx[3].pos.y = s_aDrum[nCntDrum].pos.y + cosf(s_aDrum[nCntDrum].fAngle + s_aDrum[nCntDrum].rot.y) * s_aDrum[nCntDrum].fLength;
			pVtx[3].pos.z = s_aDrum[nCntDrum].pos.z;

			//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//ゴールサイドで向きを変える処理
			if (s_aDrum[nCntDrum].bSide == 0)
			{
				//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else
			{
				//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
				pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			}
		}
	}

	//頂点バッファをアンロックする
	s_pVtxBuffDrum->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffPost->Lock(0, 0, (void**)&pVtx, 0);

	//構造体の初期化処理
	for (int nCntDrum = 0; nCntDrum < MAX_POST; nCntDrum++, pVtx += 4)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_posPost.x + -GOAL_WIDTH;
		pVtx[0].pos.y = s_posPost.y + -GOAL_HEIGHT * 2.0f;
		pVtx[0].pos.z = s_posPost.z + 0.0f;

		pVtx[1].pos.x = s_posPost.x + GOAL_WIDTH * 0.75f;
		pVtx[1].pos.y = s_posPost.y + -GOAL_HEIGHT * 2.0f;
		pVtx[1].pos.z = s_posPost.z + 0.0f;

		pVtx[2].pos.x = s_posPost.x + -GOAL_WIDTH;
		pVtx[2].pos.y = s_posPost.y + GOAL_HEIGHT * 2.0f;
		pVtx[2].pos.z = s_posPost.z + 0.0f;

		pVtx[3].pos.x = s_posPost.x + GOAL_WIDTH * 0.75f;
		pVtx[3].pos.y = s_posPost.y + GOAL_HEIGHT * 2.0f;
		pVtx[3].pos.z = s_posPost.z + 0.0f;
	}
	//頂点バッファをアンロックする
	s_pVtxBuffPost->Unlock();

}

//=========================================
// ドラム缶の描画処理
//=========================================
void DrawDrum()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

												//頂点バッファをデータストリームに設定
	//pDevice->SetStreamSource(0, s_pVtxBuffPost, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	////テクスチャの設定
	//pDevice->SetTexture(0, s_pTexturePost);

	//ポリゴンの描画
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,			//描画する最初の頂点インデックス
		2						//プリミティブアイコンの個数
	);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffDrum,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDrum = 0; nCntDrum < MAX_DRUM; nCntDrum++)
	{
		if (s_aDrum[nCntDrum].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTextureDrum[s_aDrum[nCntDrum].type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntDrum * 4,			//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}

}

//=========================================
// ドラム缶の設定処理
//=========================================
void SetDrum(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, DRUM_TYPE type, int nIdxGoal)
{
	s_aDrum[nIdxGoal].pos = pos;
	s_aDrum[nIdxGoal].rot = rot;
	s_aDrum[nIdxGoal].bSide = side;
	s_aDrum[nIdxGoal].type = type;
	s_aDrum[nIdxGoal].bUse = true;

	if (s_aDrum[nIdxGoal].type == DRUM_TYPE_NORMAL)
	{
		s_aDrum[nIdxGoal].fAngle = atan2f(DRUM_WIDTH, DRUM_HEIGHT);
		s_aDrum[nIdxGoal].fLength = sqrtf((DRUM_WIDTH * DRUM_WIDTH) + (DRUM_HEIGHT * DRUM_HEIGHT)) / 2.0f;
	}
	else if (s_aDrum[nIdxGoal].type == DRUM_TYPE_STRIKE)
	{
		s_aDrum[nIdxGoal].fAngle = atan2f(DRUM_WIDTH, DRUM_HEIGHT);
		s_aDrum[nIdxGoal].fLength = sqrtf((DRUM_WIDTH * DRUM_WIDTH) + (DRUM_HEIGHT * DRUM_HEIGHT)) / 2.0f;
	}
}

//============================================================================
//ドラム缶の取得処理
//============================================================================
DRUM *GetDrum(void)
{
	return s_aDrum;	//ドラム缶情報の先頭アドレスを返す
}