//================================//
//								  //
//       ディスクのファイル		  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//インクルードファイル
//====================================
#include "disk.h"
#include "wall.h"
#include "goal.h"
#include <time.h>
#include "shadow.h"
#include "stage.h"
#include "player.h"
#include "input.h"
#include "landingPoint.h"
#include "effect.h"

//====================================
//グローバル変数
//====================================
static Disk g_aDisk[MAX_DISK];									//ディスク型のグローバル変数を宣言する
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDisk = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_apTexDisk = NULL;					//テクスチャへのポインタ

//====================================
//ディスクの初期化処理
//====================================
void InitDisk(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイス情報の取得
	VERTEX_2D*pVtx;									//頂点情報へのポインタ
	srand((unsigned)time(NULL));

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SilverGear.png",
		&g_apTexDisk);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DISK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDisk,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		ZeroMemory(&g_aDisk[nCntDisk], sizeof(Disk));

		//頂点座標の設定
		pVtx[(nCntDisk * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[(nCntDisk * 4) + 0].rhw = 1.0f;
		pVtx[(nCntDisk * 4) + 1].rhw = 1.0f;
		pVtx[(nCntDisk * 4) + 2].rhw = 1.0f;
		pVtx[(nCntDisk * 4) + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[(nCntDisk * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntDisk * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntDisk * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntDisk * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[(nCntDisk * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntDisk * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntDisk * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffDisk->Unlock();
}

//====================================
//ディスクの終了処理
//====================================
void UninitDisk(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffDisk != NULL)
	{
		g_pVtxBuffDisk->Release();
		g_pVtxBuffDisk = NULL;
	}

	//テクスチャポインタの破棄
	if (g_apTexDisk != NULL)
	{
		g_apTexDisk->Release();
		g_apTexDisk = NULL;
	}
}

//====================================
//ディスクの更新処理
//====================================
void UpdateDisk(void)
{
	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		if (g_aDisk[nCntDisk].bUse == true)
		{
			g_aDisk[nCntDisk].pos += g_aDisk[nCntDisk].move;				//現在位置の更新
			g_aDisk[nCntDisk].move += g_aDisk[nCntDisk].acc;				//移動量の更新

			switch (g_aDisk[nCntDisk].type)
			{
			default:
				if (g_aDisk[nCntDisk].fHeight <= 15.0f)
				{
					SetEffect((g_aDisk[nCntDisk].pos - g_aDisk[nCntDisk].move), 0.0f, EFFECT_TYPE_TRAIL);
					SetEffect((g_aDisk[nCntDisk].pos - g_aDisk[nCntDisk].move), 0.0f, EFFECT_TYPE_TRAIL);
				}
				break;

			case DISK_TYPE_LOB:

				if (g_aDisk[nCntDisk].fHeight > 0.0f)
				{
					g_aDisk[nCntDisk].fHeight += g_aDisk[nCntDisk].fVerticalSpeed;
					g_aDisk[nCntDisk].fVerticalSpeed += GRAVITY_ACCELERATION;

					if (g_aDisk[nCntDisk].fHeight <= 0.0f)
					{
						g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				break;

			case DISK_TYPE_JUMP:

				if (g_aDisk[nCntDisk].fHeight > 10.0f)
				{
					SetEffect(D3DXVECTOR3(g_aDisk[nCntDisk].pos.x, g_aDisk[nCntDisk].pos.y - (g_aDisk[nCntDisk].fHeight * 0.2f), 0.0f), g_aDisk[nCntDisk].rot, EFFECT_TYPE_TRAIL_JUMP);
				}

				if (g_aDisk[nCntDisk].fHeight > 0.0f)
				{
					g_aDisk[nCntDisk].fHeight += g_aDisk[nCntDisk].fVerticalSpeed;
					g_aDisk[nCntDisk].fVerticalSpeed += GRAVITY_ACCELERATION;

					if (g_aDisk[nCntDisk].fHeight <= 0.0f)
					{
						g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				break;

			case DISK_TYPE_BLOCKED:

				g_aDisk[nCntDisk].fHeight += g_aDisk[nCntDisk].fVerticalSpeed;
				g_aDisk[nCntDisk].fVerticalSpeed += GRAVITY_ACCELERATION;

				break;

			case DISK_TYPE_SPECIAL_0:

				UpdateSpecialDisk(nCntDisk);

				

				break;

			case DISK_TYPE_SPECIAL_1:

				UpdateSpecialDisk(nCntDisk);

				break;

			case DISK_TYPE_SPECIAL_2:

				UpdateSpecialDisk(nCntDisk);

				SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

				break;

			case DISK_TYPE_SPECIAL_3:

				UpdateSpecialDisk(nCntDisk);

				break;

			case DISK_TYPE_SPECIAL_4:

				UpdateSpecialDisk(nCntDisk);

				for (int nCntEffect = 0; nCntEffect < 10; nCntEffect++)
				{
					SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_TRAIL4);
				}

				break;
			}

			if (g_aDisk[nCntDisk].bBounce == true)
			{
				//壁との当たり判定
				WallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, &g_aDisk[nCntDisk].acc, (g_aDisk[nCntDisk].fSize * 0.5f));
			}

			//ゴールとの当たり判定(pos, lastPos, fWidth, fHeight)
			ColisionGoal(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos);

			g_aDisk[nCntDisk].lastPos = g_aDisk[nCntDisk].pos;							//前回の位置の更新

			g_aDisk[nCntDisk].rot += g_aDisk[nCntDisk].fRotSpeed;
	
			VERTEX_2D *pVtx = NULL;					//頂点情報へのポインタ

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

			g_aDisk[nCntDisk].fVertexLenght = (g_aDisk[nCntDisk].fSize * 0.5f * (1 + (g_aDisk[nCntDisk].fHeight * 0.005f)));

			pVtx[(nCntDisk * 4) + 0].pos.x = g_aDisk[nCntDisk].pos.x + sinf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (D3DX_PI * 0.75f)) * g_aDisk[nCntDisk].fVertexLenght;
			pVtx[(nCntDisk * 4) + 0].pos.y = g_aDisk[nCntDisk].pos.y + cosf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (D3DX_PI * 0.75f)) * g_aDisk[nCntDisk].fVertexLenght - (g_aDisk[nCntDisk].fHeight * 0.2f);
			pVtx[(nCntDisk * 4) + 0].pos.z = 0.0f;

			pVtx[(nCntDisk * 4) + 1].pos.x = g_aDisk[nCntDisk].pos.x + sinf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (D3DX_PI * 0.25f)) * g_aDisk[nCntDisk].fVertexLenght;
			pVtx[(nCntDisk * 4) + 1].pos.y = g_aDisk[nCntDisk].pos.y + cosf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (D3DX_PI * 0.25f)) * g_aDisk[nCntDisk].fVertexLenght - (g_aDisk[nCntDisk].fHeight * 0.2f);
			pVtx[(nCntDisk * 4) + 1].pos.z = 0.0f;

			pVtx[(nCntDisk * 4) + 2].pos.x = g_aDisk[nCntDisk].pos.x + sinf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (-D3DX_PI * 0.75f)) * g_aDisk[nCntDisk].fVertexLenght;
			pVtx[(nCntDisk * 4) + 2].pos.y = g_aDisk[nCntDisk].pos.y + cosf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (-D3DX_PI * 0.75f)) * g_aDisk[nCntDisk].fVertexLenght - (g_aDisk[nCntDisk].fHeight * 0.2f);
			pVtx[(nCntDisk * 4) + 2].pos.z = 0.0f;

			pVtx[(nCntDisk * 4) + 3].pos.x = g_aDisk[nCntDisk].pos.x + sinf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (-D3DX_PI * 0.25f)) * g_aDisk[nCntDisk].fVertexLenght;
			pVtx[(nCntDisk * 4) + 3].pos.y = g_aDisk[nCntDisk].pos.y + cosf(g_aDisk[nCntDisk].rot + g_aDisk[nCntDisk].fAngle + (-D3DX_PI * 0.25f)) * g_aDisk[nCntDisk].fVertexLenght - (g_aDisk[nCntDisk].fHeight * 0.2f);
			pVtx[(nCntDisk * 4) + 3].pos.z = 0.0f;

			//頂点バッファをアンロックする
			g_pVtxBuffDisk->Unlock();

			SetPositionShadow(g_aDisk[nCntDisk].nIdxShadow, g_aDisk[nCntDisk].pos);		//影の位置の更新
		}
	}
}

//====================================
//ディスクの描画処理
//====================================
void DrawDisk(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDisk, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		if (g_aDisk[nCntDisk].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTexDisk);

			//ディスクを描画する
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDisk * 4, 2);
		}
	}
}

//====================================
//ディスクの設定処理
//====================================
void SetDisk(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 acc, DISK_TYPE type, int nPlayer, float size)
{
	VERTEX_2D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDisk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisk = 0; nCntDisk < MAX_DISK; nCntDisk++)
	{
		if (g_aDisk[nCntDisk].bUse == false)
		{
			g_aDisk[nCntDisk].pos = pos;									//ディスクの現在の位置の設定
			g_aDisk[nCntDisk].lastPos = pos - move;							//ディスクの前回の位置の設定
			g_aDisk[nCntDisk].move = move;									//ディスクの移動量の設定
			g_aDisk[nCntDisk].fSize = size;									//ディスクの大きさの設定
			g_aDisk[nCntDisk].acc = acc;									//ディスクの加速の設定
			g_aDisk[nCntDisk].type = type;									//ディスクの種類の設定
			g_aDisk[nCntDisk].nCntPhase = 0;								//ディスクの必殺技用のカウンターの設定
			g_aDisk[nCntDisk].nPlayer = nPlayer;							//ディスクを投げたプレイヤーのインデックスの設定
			g_aDisk[nCntDisk].nIdxShadow = SetShadow(pos, size * 1.25f);	//ディスクの影のインデックス
			g_aDisk[nCntDisk].bUse = true;									//使用されている状態にする

			//回転用の変数の設定
			g_aDisk[nCntDisk].fLenght = sqrtf((g_aDisk[nCntDisk].fSize * g_aDisk[nCntDisk].fSize) + (g_aDisk[nCntDisk].fSize * g_aDisk[nCntDisk].fSize)) * 0.5f;
			g_aDisk[nCntDisk].fAngle = atan2f(g_aDisk[nCntDisk].fSize * 0.5f, g_aDisk[nCntDisk].fSize * 0.5f);
			g_aDisk[nCntDisk].rot = 0.0f;
			g_aDisk[nCntDisk].fVertexLenght = size * 0.5f;
			g_aDisk[nCntDisk].fRotSpeed = BASE_ROTATION_SPEED * (sqrtf((move.x * move.x) + (move.y * move.y)) * 0.65);

			//頂点座標の設定
			pVtx[(nCntDisk * 4) + 0].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntDisk * 4) + 1].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y - (size * 0.5f), 0.0f);
			pVtx[(nCntDisk * 4) + 2].pos = D3DXVECTOR3(pos.x - (size * 0.5f), pos.y + (size * 0.5f), 0.0f);
			pVtx[(nCntDisk * 4) + 3].pos = D3DXVECTOR3(pos.x + (size * 0.5f), pos.y + (size * 0.5f), 0.0f);

			//頂点カラーの設定
			pVtx[(nCntDisk * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntDisk * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntDisk * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntDisk * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			int a = 0;
			switch (type)
			{
			default:

				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = true;

				break;

			case DISK_TYPE_LOB:

				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = LOB_STARTING_SPEED;
				g_aDisk[nCntDisk].bBounce = true;
				
				g_aDisk[nCntDisk].move = SetLobSpeed(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].move, nCntDisk, g_aDisk[nCntDisk].fHeight, g_aDisk[nCntDisk].fVerticalSpeed);

				SetLandingMark(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].move, g_aDisk[nCntDisk].fHeight, g_aDisk[nCntDisk].fVerticalSpeed, GRAVITY_ACCELERATION, g_aDisk[nCntDisk].fSize);


				break;

			case DISK_TYPE_JUMP:

				g_aDisk[nCntDisk].fHeight = JUMP_MAX_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = -JUMP_MAX_HEIGHT / JUMP_ATTACK_TIME;
				g_aDisk[nCntDisk].bBounce = false;

				g_aDisk[nCntDisk].move = SetJumpAttackSpeed(g_aDisk[nCntDisk].pos);

				SetLandingMark(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].move, g_aDisk[nCntDisk].fHeight, g_aDisk[nCntDisk].fVerticalSpeed, GRAVITY_ACCELERATION, g_aDisk[nCntDisk].fSize);
				break;

			case DISK_TYPE_BLOCKED:

				g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aDisk[nCntDisk].fHeight = JUMP_MAX_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = JUMP_MAX_HEIGHT / JUMP_ATTACK_TIME;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_0:

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 15.5f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -15.5f;
				}

				if (pos.y <= SCREEN_HEIGHT * 0.5f)
				{
					g_aDisk[nCntDisk].move.y = 0.0f;
					g_aDisk[nCntDisk].acc.y = 1.0f;
				}
				else
				{
					g_aDisk[nCntDisk].move.y = 0.0f;
					g_aDisk[nCntDisk].acc.y = -1.0f;
				}

				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_1:

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 15.5f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -15.5f;
				}

				g_aDisk[nCntDisk].move.y = 0.0f;
				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_2:

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 15.5f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -15.5f;
				}

				g_aDisk[nCntDisk].move.y = 0.0f;
				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_3:

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 15.5f;
					g_aDisk[nCntDisk].acc.x = 0.05f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -15.5f;
					g_aDisk[nCntDisk].acc.x = -0.05f;
				}

				g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aDisk[nCntDisk].move.y = 0.0f;
				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT * 3.0f;
				g_aDisk[nCntDisk].fVerticalSpeed = LOB_STARTING_SPEED * 3.0f;
				g_aDisk[nCntDisk].bBounce = false;

				break;

			case DISK_TYPE_SPECIAL_4:

				int nPos;

				if (pos.y <= SCREEN_HEIGHT * 0.5f)
				{
					nPos = 1;
				}
				else
				{
					nPos = -1;
				}

				if (pos.x < SCREEN_WIDTH * 0.5f)
				{
					g_aDisk[nCntDisk].move.x = 25.0f;
				}
				else
				{
					g_aDisk[nCntDisk].move.x = -25.0f;
				}

				g_aDisk[nCntDisk].move.y = 0.0f;
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.25f * nPos, 0.0f);
				g_aDisk[nCntDisk].fHeight = NORMAL_DISK_HEIGHT;
				g_aDisk[nCntDisk].fVerticalSpeed = NORMAL_VERTICAL_SPEED;
				g_aDisk[nCntDisk].bBounce = false;

				break;
			}

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffDisk->Unlock();
}

//必殺技
void UpdateSpecialDisk(int nCntDisk)
{
	float fChangePoint;
	bool bImpact = false;	

	STAGE_LENGTH *pArea1 = GetP1StgLng();					//プレイヤー１のエリア情報へのポインタ
	STAGE_LENGTH *pArea2 = GetP2StgLng();					//プレイヤー２のエリア情報へのポインタ

	float playArea = pArea2->max.x - pArea1->min.x;

	switch (g_aDisk[nCntDisk].type)
	{
	//====================================================================================================================================
	case DISK_TYPE_SPECIAL_0:

		switch (g_aDisk[nCntDisk].nCntPhase)
		{
		default:
			for (int nCntEffect = 0; nCntEffect < 8; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_GENERAL);
			}
			break;

		case 0:

			bImpact = SpecialWallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, 10.0f);

			for (int nCntEffect = 0; nCntEffect < 8; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_GENERAL);
			}

			if (bImpact == true)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f - (30.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;

		case 1:

			for (int nCntEffect = 0; nCntEffect < 15; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_TRAIL0);
			}

			fChangePoint = SCREEN_WIDTH * (0.7f - (0.3f * g_aDisk[nCntDisk].nPlayer));

			if (g_aDisk[nCntDisk].pos.x >= fChangePoint && g_aDisk[nCntDisk].move.x > 0.0f)
			{
				int nDir = (int)(g_aDisk[nCntDisk].pos.y / SCREEN_HEIGHT + 1.0f);

				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(-2.5f + (1.0f * g_aDisk[nCntDisk].nPlayer), 15.0f - (30.0f * nDir), 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.5f - (1.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
			}
			else if (g_aDisk[nCntDisk].pos.x <= fChangePoint && g_aDisk[nCntDisk].move.x < 0.0f)
			{
				int nDir = (int)(g_aDisk[nCntDisk].pos.y / SCREEN_HEIGHT + 1.0f);

				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(-2.5f + (1.0f * g_aDisk[nCntDisk].nPlayer), 15.0f - (30.0f * nDir), 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.5f - (1.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
			}

			break;

		case 2:

			bImpact = SpecialWallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, 10.0f);

			for (int nCntEffect = 0; nCntEffect < 8; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_GENERAL);
			}

			if (bImpact == true)
			{
				int nDir = (int)(g_aDisk[nCntDisk].pos.y / SCREEN_HEIGHT + 1.0f);

				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(15.0f - (30.0f * g_aDisk[nCntDisk].nPlayer), -6.0f + (12.0f * (nDir)), 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;
		}

		break;
		//====================================================================================================================================
		//====================================================================================================================================
	case DISK_TYPE_SPECIAL_1:

		switch (g_aDisk[nCntDisk].nCntPhase)
		{

		default:

			SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

			break;

		case 0:

			fChangePoint = SCREEN_WIDTH * (0.3f + (0.4f * g_aDisk[nCntDisk].nPlayer));

			SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

			if (g_aDisk[nCntDisk].move.x >= 0)
			{
				if (g_aDisk[nCntDisk].pos.x >= fChangePoint)
				{
					int nDir = (int)(g_aDisk[nCntDisk].pos.y / ((SCREEN_HEIGHT * 0.5f) + 1.0f));

					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, -20.0f + (40.0f * nDir), 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else
			{
				if (g_aDisk[nCntDisk].pos.x < fChangePoint)
				{
					int nDir = (int)(g_aDisk[nCntDisk].pos.y / ((SCREEN_HEIGHT * 0.5f) + 1.0f));

					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, -20.0f + (40.0f * nDir), 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}

			break;

		case 1:

			bImpact = SpecialWallBounce(&g_aDisk[nCntDisk].pos, &g_aDisk[nCntDisk].lastPos, &g_aDisk[nCntDisk].move, 10.0f);

			SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

			if (bImpact == true)
			{
				g_aDisk[nCntDisk].nCntPhase++;
				g_aDisk[nCntDisk].move = D3DXVECTOR3(20.0f - (40.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
				g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}

			break;

		case 2:

			fChangePoint = SCREEN_WIDTH * (0.7f - (0.4f * g_aDisk[nCntDisk].nPlayer));

			for (int nCntEffect = 0; nCntEffect < 15; nCntEffect++)
			{
				SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_TRAIL0);
			}

			if (g_aDisk[nCntDisk].move.x < 0)
			{
				if (g_aDisk[nCntDisk].pos.x <= fChangePoint)
				{
					int nDir = (int)(g_aDisk[nCntDisk].pos.y / ((SCREEN_HEIGHT * 0.5f) + 1.0f));
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 15.0f - (30.0f * nDir), 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else
			{
				if (g_aDisk[nCntDisk].pos.x >= fChangePoint)
				{
					int nDir = (int)(g_aDisk[nCntDisk].pos.y / ((SCREEN_HEIGHT * 0.5f) + 1.0f));
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(0.0f, 15.0f - (30.0f * nDir), 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}

			break;

		case 3:

			fChangePoint = (SCREEN_HEIGHT * 0.5f) + ((rand() % 201) - 100);

			SetEffect(g_aDisk[nCntDisk].pos, g_aDisk[nCntDisk].rot, EFFECT_TYPE_SPECIAL_TRAIL2);

			if (g_aDisk[nCntDisk].move.y > 0)
			{
				if (g_aDisk[nCntDisk].pos.y >= fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(20.0f - (40.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			else
			{
				if (g_aDisk[nCntDisk].pos.y <= fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(20.0f - (40.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
					g_aDisk[nCntDisk].acc = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}

			break;
		}

		break;
		//====================================================================================================================================
		//====================================================================================================================================
		case DISK_TYPE_SPECIAL_2:		

			switch (g_aDisk[nCntDisk].nCntPhase)
			{
			case 0:

				fChangePoint = playArea * (0.25f + (0.5f * g_aDisk[nCntDisk].nPlayer));

				if (g_aDisk[nCntDisk].move.x > 0.0f && g_aDisk[nCntDisk].pos.x > fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;

					if (g_aDisk[nCntDisk].pos.y <= SCREEN_HEIGHT * 0.5f)
					{
						g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT - 200.0f;
					}
					else
					{
						g_aDisk[nCntDisk].pos.y = 200.0f;
					}
				}
				else if (g_aDisk[nCntDisk].move.x < 0.0f && g_aDisk[nCntDisk].pos.x < fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;

					if (g_aDisk[nCntDisk].pos.y <= SCREEN_HEIGHT * 0.5f)
					{
						g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT - 200.0f;
					}
					else
					{
						g_aDisk[nCntDisk].pos.y = 200.0f;
					}
				}

				break;

			case 1:

				fChangePoint = playArea * 0.5f;

				if (g_aDisk[nCntDisk].move.x > 0.0f && g_aDisk[nCntDisk].pos.x > fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;

					if (g_aDisk[nCntDisk].pos.y <= SCREEN_HEIGHT * 0.5f)
					{
						g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT - 200.0f;
					}
					else
					{
						g_aDisk[nCntDisk].pos.y = 200.0f;
					}
				}
				else if (g_aDisk[nCntDisk].move.x < 0.0f && g_aDisk[nCntDisk].pos.x < fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;

					if (g_aDisk[nCntDisk].pos.y <= SCREEN_HEIGHT * 0.5f)
					{
						g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT - 200.0f;
					}
					else
					{
						g_aDisk[nCntDisk].pos.y = 200.0f;
					}
				}
				break;

			case 2:

				fChangePoint = playArea * (0.7f - (0.4f * g_aDisk[nCntDisk].nPlayer));

				if (g_aDisk[nCntDisk].move.x > 0.0f && g_aDisk[nCntDisk].pos.x >= fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT * 0.5f + ((rand() % 200) - 100);
				}
				else if (g_aDisk[nCntDisk].move.x < 0.0f && g_aDisk[nCntDisk].pos.x <= fChangePoint)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].pos.y = SCREEN_HEIGHT * 0.5f + ((rand() % 200) - 100);
				}
				
				break;

			default:
				break;
			}
			break;
			//====================================================================================================================================
			//====================================================================================================================================
		case DISK_TYPE_SPECIAL_3:

			switch (g_aDisk[nCntDisk].nCntPhase)
			{
			case 0:

				g_aDisk[nCntDisk].fHeight += g_aDisk[nCntDisk].fVerticalSpeed;
				g_aDisk[nCntDisk].fVerticalSpeed -= 0.75f;

				if (g_aDisk[nCntDisk].fHeight <= 0.0f)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].fHeight = 0.0f;
					g_aDisk[nCntDisk].fVerticalSpeed = 0.0f;
					g_aDisk[nCntDisk].move = D3DXVECTOR3(30.0f + (-60.0f * g_aDisk[nCntDisk].nPlayer), 0.0f, 0.0f);
				}

				break;

			default:

				for (int nCntEffect = 0; nCntEffect < 15; nCntEffect++)
				{
					SetEffect(g_aDisk[nCntDisk].pos, 0.0f, EFFECT_TYPE_SPECIAL_TRAIL3);
				}

				break;
			}
			break;
			//====================================================================================================================================
			//====================================================================================================================================
		case DISK_TYPE_SPECIAL_4:

			switch (g_aDisk[nCntDisk].nCntPhase)
			{
			default:
				break;

			case 0:

				if (g_aDisk[nCntDisk].move.y >= 0 && g_aDisk[nCntDisk].pos.y > SCREEN_HEIGHT * 0.5f)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move.y = 0.0f;
					g_aDisk[nCntDisk].acc.y = 0.0f;
				}
				else if (g_aDisk[nCntDisk].move.y < 0 && g_aDisk[nCntDisk].pos.y < SCREEN_HEIGHT * 0.5f)
				{
					g_aDisk[nCntDisk].nCntPhase++;
					g_aDisk[nCntDisk].move.y = 0.0f;
					g_aDisk[nCntDisk].acc.y = 0.0f;
				}

				break;

			case 1:

				g_aDisk[nCntDisk].pos.y = 150.0f * sinf(g_aDisk[nCntDisk].pos.x / 50.0f) + 360.0f;

				break;
			}

			break;
			//====================================================================================================================================
	}
}

//============================================================================
//ディスクの取得処理
//============================================================================
Disk *GetDisk(void)
{
	return g_aDisk;	//ディスク情報の先頭アドレスを返す
}

//============================================================================
//上投げの移動量の設定処理
//============================================================================
D3DXVECTOR3 SetLobSpeed(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nCntDisk, float fHeight, float fVerticalSpeed)
{
	STAGE_LENGTH *pArea1 = GetP1StgLng();								//プレイヤー１のエリア情報へのポインタ
	STAGE_LENGTH *pArea2 = GetP2StgLng();								//プレイヤー２のエリア情報へのポインタ
	D3DXVECTOR3 newSpeed = move;										//新しい移動量
	D3DXVECTOR3 lastPos = pos;											//最初の位置
	D3DXVECTOR3 endPos = pos;											//落ちた後の位置
	D3DXVECTOR3 newPos;													//新しい位置
	float fTime = 0.0f;													//ディスクが落ちるまでの必要なフレーム
	float height, vSpeed;												//ディスクが落ちるまでの必要なフレームを計算する用の変数
	height = fHeight;
	vSpeed = fVerticalSpeed;

	while (height > 0.0f)
	{//ディスクが落ちた時の位置と必要な時間を計算する
		height += vSpeed;
		vSpeed += GRAVITY_ACCELERATION;
		endPos += move;
		fTime += 1.0f;
	}
	newPos = endPos;						//目的の位置

	//相手のエリアを出ないようにする処理
	if (pos.x < SCREEN_WIDTH * 0.5f)
	{//投げるプレイヤーはプレイヤー１だったら
		if (newPos.y < pArea2->min.y + g_aDisk[nCntDisk].fSize)
		{//エリアの上側を出ないようにする
			newPos.y = pArea2->min.y + g_aDisk[nCntDisk].fSize;
		}
		else if (newPos.y > pArea2->max.y - g_aDisk[nCntDisk].fSize)
		{//エリアの下側を出ないようにする
			newPos.y = pArea2->max.y - g_aDisk[nCntDisk].fSize;
		}

		if (newPos.x < pArea2->min.x + g_aDisk[nCntDisk].fSize)
		{//エリアの左側を出ないようにする
			newPos.x = pArea2->min.x + g_aDisk[nCntDisk].fSize;
		}
		else if (newPos.x > pArea2->max.x - g_aDisk[nCntDisk].fSize)
		{//エリアの右側を出ないようにする
			newPos.x = pArea2->max.x - g_aDisk[nCntDisk].fSize;
		}
	}
	else
	{//投げるプレイヤーはプレイヤー２だったら
		if (newPos.y < pArea1->min.y + g_aDisk[nCntDisk].fSize)
		{//エリアの上側を出ないようにする
			newPos.y = pArea1->min.y + g_aDisk[nCntDisk].fSize;
		}
		else if (newPos.y > pArea1->max.y - g_aDisk[nCntDisk].fSize)
		{//エリアの下側を出ないようにする
			newPos.y = pArea1->max.y - g_aDisk[nCntDisk].fSize;
		}

		if (newPos.x < pArea1->min.x + g_aDisk[nCntDisk].fSize)
		{//エリアの左側を出ないようにする
			newPos.x = pArea1->min.x + g_aDisk[nCntDisk].fSize;
		}
		else if (newPos.x > pArea1->max.x - g_aDisk[nCntDisk].fSize)
		{//エリアの右側を出ないようにする
			newPos.x = pArea1->max.x - g_aDisk[nCntDisk].fSize;
		}
	}

	newSpeed = (newPos - pos) / fTime;						//新しい移動量を計算する

	return newSpeed;										//新しい移動量を返す
}

//============================================================================
//ジャンプ投げの移動量の設定処理
//============================================================================
D3DXVECTOR3 SetJumpAttackSpeed(D3DXVECTOR3 pos)
{
	STAGE_LENGTH *pArea1 = GetP1StgLng();					//プレイヤー１のエリア情報へのポインタ
	STAGE_LENGTH *pArea2 = GetP2StgLng();					//プレイヤー２のエリア情報へのポインタ
	Player *pPlayer = GetPlayer();							//プレイヤー情報へのポインタ

	D3DXVECTOR3 endPos, speed;								//目的の位置と移動量のローカル変数

	int nPlayer;											//ディスクを投げたプレイヤー

	if (pos.x < SCREEN_WIDTH * 0.5f)
	{//プレイヤー１
		nPlayer = 0;
	}
	else
	{//プレイヤー２
		nPlayer = 1;
	}

	if (nPlayer == 0)
	{//プレイヤー１だったら
		endPos.x = pArea2->min.x + 200.0f + (pPlayer->fThrowPower * 5.0f);			//目的の位置のX座標を設定する
		endPos.y = pos.y;															//目的の位置のY座標を設定する
		endPos.z = 0.0f;															//目的の位置のZ座標を0にする
	}
	else
	{//プレイヤー２だったら
		endPos.x = pArea1->max.x - 200.0f - (pPlayer->fThrowPower * 5.0f);			//目的の位置のX座標を設定する
		endPos.y = pos.y;															//目的の位置のY座標を設定する
		endPos.z = 0.0f;															//目的の位置のZ座標を0にする
	}																				
		
	//新しい移動量を計算する
	speed = endPos - pos;															
	speed.x /= JUMP_ATTACK_TIME;

	return speed;																	//新しい移動量を返す
}

//======================================
//ディスクの破棄
//======================================
void DestroyDisk(void)
{
	g_aDisk[0].bUse = false;							//ディスクを使用されていない状態にする
	GetShadow()[g_aDisk[0].nIdxShadow].bUse = false;	//影を使用されていない状態にする
	DestroyLandingMark();								// 空中の到着地点のマークを削除
	g_aDisk[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置の初期化
}