//=========================================
// 
// ゴール処理
// Author Tanimoto_Kosuke
//
// Update 22/03/23
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "goal.h"
#include <stdio.h>
#include "disk.h"
#include "stage.h"
#include "effect.h"
#include "score.h"
#include "game.h"
#include "pop.h"
#include "referee.h"
#include "sound.h"
#include "pipe.h"

//------------------------------------
// スタティック変数
//------------------------------------
static LPDIRECT3DTEXTURE9		s_pTextureGoal[MAX_IMAGE_DRUM] = {};	//テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuffGoal = NULL;	//頂点バッファへのポインタ
static Goal s_aGoal[MAX_GOAL];							//ゴールの情報

//=========================================
// ゴールの初期化処理
//=========================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

										//テクスチャーの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\stripe03.png",	//テクスチャのファイル名
		&s_pTextureGoal[GOAL_TYPE_NORMAL]
	);

	D3DXCreateTextureFromFile
	(
		pDevice,
		"data\\TEXTURE\\goal\\stripe04.png",	//テクスチャのファイル名
		&s_pTextureGoal[GOAL_TYPE_STRIKE]
	);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_GOAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffGoal,
		NULL
	);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	//構造体の初期化処理
	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++, pVtx += 4)
	{
		s_aGoal[nCntGoal].fGoalCounter = 0;
		s_aGoal[nCntGoal].fFlashCounter = 0;
		s_aGoal[nCntGoal].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		s_aGoal[nCntGoal].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		s_aGoal[nCntGoal].bUse = false;
		s_aGoal[nCntGoal].bGoal = false;
		s_aGoal[nCntGoal].bAddFlash = false;

		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[0].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[0].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[1].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[1].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[1].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[2].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[2].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[2].pos.z = s_aGoal[nCntGoal].pos.z;

		pVtx[3].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
		pVtx[3].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
		pVtx[3].pos.z = s_aGoal[nCntGoal].pos.z;

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

	//ゴールの位置
	SetGoal(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, GOAL_TYPE_NORMAL, 0);
	SetGoal(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, SCREEN_HEIGHT / 2 + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, GOAL_TYPE_STRIKE, 1);
	SetGoal(D3DXVECTOR3(GOAL_WIDTH / 2 + 25, SCREEN_HEIGHT - 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false, GOAL_TYPE_NORMAL, 2);
	SetGoal(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, GOAL_TYPE_NORMAL, 3);
	SetGoal(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, GOAL_TYPE_STRIKE, 4);
	SetGoal(D3DXVECTOR3(SCREEN_WIDTH - GOAL_WIDTH / 2 - 25, SCREEN_HEIGHT - 200.0f + (STAGE_HEIGHT_DOWN / 2), 0.1f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), true, GOAL_TYPE_NORMAL, 5);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++, pVtx += 4)
	{
		if (s_aGoal[nCntGoal].bSide == 0)
		{
			if (s_aGoal[nCntGoal].type == POP_TYPE_NORMAL)
			{
				SetPop(D3DXVECTOR3(GOAL_POP_WIDTH / 2, s_aGoal[nCntGoal].pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_NORMAL, nCntGoal);
			}
			else if (s_aGoal[nCntGoal].type == POP_TYPE_STRIKE)
			{
				SetPop(D3DXVECTOR3(GOAL_POP_WIDTH / 2, s_aGoal[nCntGoal].pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_STRIKE, nCntGoal);
			}
		}
		else
		{
			if (s_aGoal[nCntGoal].type == POP_TYPE_NORMAL)
			{
				SetPop(D3DXVECTOR3(SCREEN_WIDTH - GOAL_POP_WIDTH / 2, s_aGoal[nCntGoal].pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_NORMAL, nCntGoal);
			}
			else if (s_aGoal[nCntGoal].type == POP_TYPE_STRIKE)
			{
				SetPop(D3DXVECTOR3(SCREEN_WIDTH - GOAL_POP_WIDTH / 2, s_aGoal[nCntGoal].pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_STRIKE, nCntGoal);
			}
		}
	}
}

//=========================================
// ゴールの終了処理
//=========================================
void UninitGoal(void)
{
	for (int nCntImgGoal = 0; nCntImgGoal < MAX_IMAGE_DRUM; nCntImgGoal++)
	{
		//テクスチャの破棄
		if (s_pTextureGoal[nCntImgGoal] != NULL)
		{
			s_pTextureGoal[nCntImgGoal]->Release();
			s_pTextureGoal[nCntImgGoal] = NULL;
		}
		//頂点バッファの破棄
		if (s_pVtxBuffGoal != NULL)
		{
			s_pVtxBuffGoal->Release();
			s_pVtxBuffGoal = NULL;
		}
	}
}

//=========================================
// ゴールの更新処理
//=========================================
void UpdateGoal(void)
{	
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++, pVtx += 4)
	{
		if (s_aGoal[nCntGoal].bUse == true)
		{
			//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
			pVtx[0].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
			pVtx[0].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
			pVtx[0].pos.z = s_aGoal[nCntGoal].pos.z;

			pVtx[1].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
			pVtx[1].pos.y = s_aGoal[nCntGoal].pos.y - cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
			pVtx[1].pos.z = s_aGoal[nCntGoal].pos.z;

			pVtx[2].pos.x = s_aGoal[nCntGoal].pos.x - sinf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
			pVtx[2].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle - s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
			pVtx[2].pos.z = s_aGoal[nCntGoal].pos.z;

			pVtx[3].pos.x = s_aGoal[nCntGoal].pos.x + sinf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.x) * s_aGoal[nCntGoal].fLength;
			pVtx[3].pos.y = s_aGoal[nCntGoal].pos.y + cosf(s_aGoal[nCntGoal].fAngle + s_aGoal[nCntGoal].rot.y) * s_aGoal[nCntGoal].fLength;
			pVtx[3].pos.z = s_aGoal[nCntGoal].pos.z;

			//テクスチャ座標の設定(0.0f ~ (1 / xパターン数)f)
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//ゴールサイドで向きを変える処理
			if (s_aGoal[nCntGoal].bSide == 0)
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

			if (s_aGoal[nCntGoal].bGoal == true)
			{
				//点滅処理
				if (((int)s_aGoal[nCntGoal].fFlashCounter * GOAL_FLASH) == 255)
				{
					s_aGoal[nCntGoal].bAddFlash = false;
				}
				else if (((int)s_aGoal[nCntGoal].fFlashCounter * GOAL_FLASH) == 0)
				{
					s_aGoal[nCntGoal].bAddFlash = true;
				}

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, ((int)s_aGoal[nCntGoal].fFlashCounter * GOAL_FLASH));
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, ((int)s_aGoal[nCntGoal].fFlashCounter * GOAL_FLASH));
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, ((int)s_aGoal[nCntGoal].fFlashCounter * GOAL_FLASH));
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, ((int)s_aGoal[nCntGoal].fFlashCounter * GOAL_FLASH));

				//点滅の増減
				if (s_aGoal[nCntGoal].bAddFlash == false)
				{
					s_aGoal[nCntGoal].fFlashCounter--;
				}
				else
				{
					s_aGoal[nCntGoal].fFlashCounter++;
				}

				//ゴール秒カウント
				s_aGoal[nCntGoal].fGoalCounter++;

				if (s_aGoal[nCntGoal].fGoalCounter >= 80)
				{
					s_aGoal[nCntGoal].fGoalCounter = 0;
					s_aGoal[nCntGoal].fFlashCounter = 0;
					s_aGoal[nCntGoal].bGoal = false;
				}
			}
			else
			{
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
		}
	}

	//頂点バッファをアンロックする
	s_pVtxBuffGoal->Unlock();
}

//=========================================
// ゴールの描画処理
//=========================================
void DrawGoal()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		s_pVtxBuffGoal,		//頂点バッファへのポインタ
		0,
		sizeof(VERTEX_2D)		//頂点情報構造体のサイズ
	);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (s_aGoal[nCntGoal].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, s_pTextureGoal[s_aGoal[nCntGoal].type]);

			//ポリゴンの描画
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				nCntGoal * 4,			//描画する最初の頂点インデックス
				2						//プリミティブアイコンの個数
			);
		}
	}
}

//=========================================
// ゴールの設定処理
//=========================================
void SetGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool side, GOAL_TYPE type, int nIdxGoal)
{
	s_aGoal[nIdxGoal].pos = pos;
	s_aGoal[nIdxGoal].rot = rot;
	s_aGoal[nIdxGoal].bSide = side;
	s_aGoal[nIdxGoal].type = type;
	s_aGoal[nIdxGoal].bUse = true;

	if (s_aGoal[nIdxGoal].type == GOAL_TYPE_NORMAL)
	{
		s_aGoal[nIdxGoal].fAngle = atan2f(GOAL_WIDTH, GOAL_HEIGHT);
		s_aGoal[nIdxGoal].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (GOAL_HEIGHT * GOAL_HEIGHT)) / 2.0f;
	}
	else if (s_aGoal[nIdxGoal].type == GOAL_TYPE_STRIKE)
	{
		s_aGoal[nIdxGoal].fAngle = atan2f(GOAL_WIDTH, STRIKE_GOAL_HEIGHT);
		s_aGoal[nIdxGoal].fLength = sqrtf((GOAL_WIDTH * GOAL_WIDTH) + (STRIKE_GOAL_HEIGHT * STRIKE_GOAL_HEIGHT)) / 2.0f;
	}
}

//=========================================
// ゴールの当たり判定処理
//=========================================
void ColisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos)
{
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	Disk *pDisk = GetDisk();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuffGoal->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (s_aGoal[nCntGoal].bUse == true)
		{
			if (s_aGoal[nCntGoal].type == GOAL_TYPE_NORMAL)
			{//3点ゴールの場合
				if (ColSegmentsGoal(*pPos, *pPos - *pLastPos,
					s_aGoal[nCntGoal].pos - D3DXVECTOR3(0.0f, (GOAL_HEIGHT / 2), 0.0f),
					(s_aGoal[nCntGoal].pos + D3DXVECTOR3(0.0f, (GOAL_HEIGHT / 2), 0.0f)) - (s_aGoal[nCntGoal].pos - D3DXVECTOR3(0.0f, (GOAL_HEIGHT / 2), 0.0f))) == true)
				{
					if (s_aGoal[nCntGoal].bSide == false)
					{
						SetPop(D3DXVECTOR3 (GOAL_POP_WIDTH / 2, pDisk->pos.y,0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_NORMAL, nCntGoal);
						AddScore(3, 1);
						SetThoThrowRefreeIdx(0);	// 投げる方向の選択
	
						PointSmokeAnimation(1);
	
						*GetResetScore() = true;
					}
					else
					{
						PointSmokeAnimation(0);

						SetPop(D3DXVECTOR3(SCREEN_WIDTH - GOAL_POP_WIDTH / 2, pDisk->pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_NORMAL, nCntGoal);
						AddScore(3, 0);
						SetThoThrowRefreeIdx(1);	// 投げる方向の選択
						*GetResetScore() = true;
					}
					PlaySound(SOUND_LABEL_SE_GOAL1);
					s_aGoal[nCntGoal].bGoal = true;
					pDisk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else if(s_aGoal[nCntGoal].type == GOAL_TYPE_STRIKE)
			{//5点ゴールの場合
				if (ColSegmentsGoal(*pPos, *pPos - *pLastPos,
					s_aGoal[nCntGoal].pos - D3DXVECTOR3(0.0f, (STRIKE_GOAL_HEIGHT / 2), 0.0f),
					(s_aGoal[nCntGoal].pos + D3DXVECTOR3(0.0f, (STRIKE_GOAL_HEIGHT / 2), 0.0f)) - (s_aGoal[nCntGoal].pos - D3DXVECTOR3(0.0f, (STRIKE_GOAL_HEIGHT / 2), 0.0f))) == true)
				{
					if (s_aGoal[nCntGoal].bSide == false)
					{		
						SetPop(D3DXVECTOR3(GOAL_POP_WIDTH / 2, pDisk->pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_STRIKE, nCntGoal);
						AddScore(5,1);
						SetThoThrowRefreeIdx(0);	// 投げる方向の選択
						*GetResetScore() = true;

						PointSmokeAnimation(1);

					}
					else
					{
						PointSmokeAnimation(0);
						
						SetPop(D3DXVECTOR3(SCREEN_WIDTH - GOAL_POP_WIDTH / 2, pDisk->pos.y, 0.0f), s_aGoal[nCntGoal].rot, s_aGoal[nCntGoal].bSide, POP_TYPE_STRIKE, nCntGoal);
						AddScore(5, 0);
						SetThoThrowRefreeIdx(1);	// 投げる方向の選択
						*GetResetScore() = true;
					}
					PlaySound(SOUND_LABEL_SE_GOAL2);
					s_aGoal[nCntGoal].bGoal = true;
					pDisk->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}
	//頂点バッファをアンロックする
	s_pVtxBuffGoal->Unlock();
}

//============================================================================
//ゴールの外積処理
//============================================================================
bool ColSegmentsGoal(D3DXVECTOR3 &seg1Start, D3DXVECTOR3 &seg1Vec, D3DXVECTOR3 &seg2Start, D3DXVECTOR3 &seg2Vec)
{
	D3DXVECTOR3 vec = seg2Start - seg1Start;
	float Crs_v1_v2 = Vec3CrossGoal(&seg1Vec, &seg2Vec);

	if (Crs_v1_v2 == 0.0f) {
		// 平行状態
		return false;
	}

	float Crs_v_v1 = Vec3CrossGoal(&vec, &seg1Vec);
	float Crs_v_v2 = Vec3CrossGoal(&vec, &seg2Vec);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	float eps = 0.00001f;

	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) {
		// 交差していない
		return false;
	}

	return true;
}

//============================================================================
//ゴールの内積処理
//============================================================================
float Vec3CrossGoal(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2) 
{
	return (vec1->x * vec2->y) - (vec1->y * vec2->x);
}

//============================================================================
//ゴールの取得処理
//============================================================================
Goal *GetGoal(void)
{
	return s_aGoal;	//ゴール情報の先頭アドレスを返す
}