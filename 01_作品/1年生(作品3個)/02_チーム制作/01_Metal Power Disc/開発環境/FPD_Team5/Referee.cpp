//=========================================
// 
// レフェリー処理
// Author Tanimoto_Kosuke
// Author Yuda Kaito
//
// Update 22/03/14
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "referee.h"
#include "main.h"
#include <stdio.h>
#include "disk.h"
#include "player.h"
#include "sound.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define REFEREE_WIDTH		(100)	// レフェリーの横サイズ
#define REFEREE_HEIGHT		(100)	// レフェリーの縦サイズ
#define REFEREE_TEXTURE		"data\\TEXTURE\\referee\\field000.jpg"	// テクスチャファイル
#define THROW_POWER			(10.0f)

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTexture = NULL;	// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	// 頂点バッファへのポインタ
static REFEREE s_aRefree;							// レフェリーの情報
static int s_nThrowRefreeIdx;						// 投げるプレイヤーの方向

//------------------------------------
// プロトタイプ宣言
//------------------------------------
static void ThrowRefree(int nIdxPlayer);

//=========================================
// 初期化
//=========================================
void InitReferee(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// テクスチャーの読み込み
	D3DXCreateTextureFromFile(pDevice,"data/TEXTURE/Player/FPD_Player04.png",&s_pTexture);

	// ゴールの位置
	s_aRefree.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - REFEREE_HEIGHT * 0.75f, 0.0f);
	s_aRefree.rot = D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f);
	s_aRefree.nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_aRefree.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	s_aRefree.fAngle = atan2f(REFEREE_WIDTH, REFEREE_HEIGHT);
	s_aRefree.fLength = sqrtf((REFEREE_WIDTH * REFEREE_WIDTH) + (REFEREE_HEIGHT * REFEREE_HEIGHT)) / 2.0f;
	s_aRefree.bUse = true;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
	pVtx[0].pos.x = s_aRefree.pos.x - sinf(s_aRefree.fAngle + s_aRefree.rot.z) * s_aRefree.fLength;
	pVtx[0].pos.y = s_aRefree.pos.y - cosf(s_aRefree.fAngle + s_aRefree.rot.z) * s_aRefree.fLength;
	pVtx[0].pos.z = s_aRefree.pos.z;

	pVtx[1].pos.x = s_aRefree.pos.x + sinf(s_aRefree.fAngle - s_aRefree.rot.z) * s_aRefree.fLength;
	pVtx[1].pos.y = s_aRefree.pos.y - cosf(s_aRefree.fAngle - s_aRefree.rot.z) * s_aRefree.fLength;
	pVtx[1].pos.z = s_aRefree.pos.z;

	pVtx[2].pos.x = s_aRefree.pos.x - sinf(s_aRefree.fAngle - s_aRefree.rot.z) * s_aRefree.fLength;
	pVtx[2].pos.y = s_aRefree.pos.y + cosf(s_aRefree.fAngle - s_aRefree.rot.z) * s_aRefree.fLength;
	pVtx[2].pos.z = s_aRefree.pos.z;

	pVtx[3].pos.x = s_aRefree.pos.x + sinf(s_aRefree.fAngle + s_aRefree.rot.z) * s_aRefree.fLength;
	pVtx[3].pos.y = s_aRefree.pos.y + cosf(s_aRefree.fAngle + s_aRefree.rot.z) * s_aRefree.fLength;
	pVtx[3].pos.z = s_aRefree.pos.z;

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();

	s_nThrowRefreeIdx = 0;
}

//=========================================
// 終了
//=========================================
void UninitReferee(void)
{
	// テクスチャの破棄
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}
	// 頂点バッファの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//=========================================
// 更新
//=========================================
void UpdateReferee(void)
{
	if (s_aRefree.bUse)
	{
		ThrowRefree(s_nThrowRefreeIdx);	// 引数のプレイヤーを自在に変更

		VERTEX_2D *pVtx;	// 頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aRefree.pos.x - sinf(s_aRefree.fAngle + s_aRefree.rot.z) * s_aRefree.fLength;
		pVtx[0].pos.y = s_aRefree.pos.y - cosf(s_aRefree.fAngle + s_aRefree.rot.z) * s_aRefree.fLength;
		pVtx[0].pos.z = s_aRefree.pos.z;

		pVtx[1].pos.x = s_aRefree.pos.x + sinf(s_aRefree.fAngle - s_aRefree.rot.z) * s_aRefree.fLength;
		pVtx[1].pos.y = s_aRefree.pos.y - cosf(s_aRefree.fAngle - s_aRefree.rot.z) * s_aRefree.fLength;
		pVtx[1].pos.z = s_aRefree.pos.z;

		pVtx[2].pos.x = s_aRefree.pos.x - sinf(s_aRefree.fAngle - s_aRefree.rot.z) * s_aRefree.fLength;
		pVtx[2].pos.y = s_aRefree.pos.y + cosf(s_aRefree.fAngle - s_aRefree.rot.z) * s_aRefree.fLength;
		pVtx[2].pos.z = s_aRefree.pos.z;

		pVtx[3].pos.x = s_aRefree.pos.x + sinf(s_aRefree.fAngle + s_aRefree.rot.z) * s_aRefree.fLength;
		pVtx[3].pos.y = s_aRefree.pos.y + cosf(s_aRefree.fAngle + s_aRefree.rot.z) * s_aRefree.fLength;
		pVtx[3].pos.z = s_aRefree.pos.z;

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();
	}
}

//=========================================
// 投擲処理
//=========================================
void ThrowRefree(int nIdxPlayer)
{
	// 投げる場合
	if (s_aRefree.bThrow)
	{
		s_aRefree.bThrow = false;

		D3DXVECTOR3 posThrow = s_aRefree.pos;
		posThrow.y -= 80.0f;

		D3DXVECTOR3 move = GetPlayer()[nIdxPlayer].pos - posThrow;

		D3DXVec3Normalize(&move, &move);

		// ディスク投げ
		SetDisk(posThrow, move * THROW_POWER, D3DXVECTOR3(0.0f, 0.0f, 0.0f), DISK_TYPE_NORMAL, -1, 90.0f);

		PlaySound(SOUND_LABEL_SE_WHISTLE_START);
	}
}


//=========================================
// 描画
//=========================================
void DrawReferee()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (s_aRefree.bUse)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//============================================================================
// 取得
//============================================================================
REFEREE* GetReferee(void)
{
	return &s_aRefree;	// ゴール情報の先頭アドレスを返す
}

//============================================================================
// 投げる方向の取得
//============================================================================
void SetThoThrowRefreeIdx(int nIdx)
{
	s_nThrowRefreeIdx = nIdx;
}
