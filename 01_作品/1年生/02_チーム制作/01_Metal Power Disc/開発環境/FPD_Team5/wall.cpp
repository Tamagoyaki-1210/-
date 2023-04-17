//================================//
//								  //
//       壁のファイル	    	  //
//       Author: Ricci Alex		  //
//								  //
//================================//

//====================================
//インクルードファイル
//====================================
#include "wall.h"
#include "effect.h"
#include "sound.h"

//====================================
//グローバル変数
//====================================
static Wall g_aWall[MAX_WALL];									//壁型のグローバル変数を宣言する
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;			//頂点バッファへのポインタ
static LPDIRECT3DTEXTURE9 g_apTexWall = NULL;					//テクスチャへのポインタ

//====================================
//壁の初期化処理
//====================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイス情報の取得
	VERTEX_2D*pVtx;									//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\stage\\wall.png",
		&g_apTexWall);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		ZeroMemory(&g_aWall[nCntWall], sizeof(Wall));

		//頂点座標の設定
		pVtx[(nCntWall * 4) + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定
		pVtx[(nCntWall * 4) + 0].rhw = 1.0f;
		pVtx[(nCntWall * 4) + 1].rhw = 1.0f;
		pVtx[(nCntWall * 4) + 2].rhw = 1.0f;
		pVtx[(nCntWall * 4) + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[(nCntWall * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntWall * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntWall * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntWall * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[(nCntWall * 4) + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[(nCntWall * 4) + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[(nCntWall * 4) + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[(nCntWall * 4) + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();

}

//====================================
//壁の終了処理
//====================================
void UninitWall(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}

	//テクスチャポインタの破棄
	{
		if (g_apTexWall != NULL)
		{
			g_apTexWall->Release();
			g_apTexWall = NULL;
		}
	}
}

//====================================
//壁の更新処理
//====================================
void UpdateWall(void)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{

		}
	}
}

//====================================
//壁の描画処理
//====================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_apTexWall);

		//ディスクを描画する
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
	}
}

//====================================
//壁の設定処理
//====================================
void SetWall(D3DXVECTOR3 pos, float width, float height, float angle)
{
	VERTEX_2D *pVtx = NULL;							//頂点情報へのポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxOut;			//計算用のマトリックス

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)
		{
			g_aWall[nCntWall].pos = pos;										//壁の位置の設定
			g_aWall[nCntWall].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				//壁の法線の設定
			g_aWall[nCntWall].fWidth = width;									//壁の幅の設定
			g_aWall[nCntWall].fHeight = height;									//壁の高さの設定
			g_aWall[nCntWall].fAlpha = angle;									//壁の回転角度の設定

			//向きを反映
			D3DXMatrixIdentity(&mtxRot);
			D3DXMatrixRotationZ(&mtxRot, angle);

			//壁の法線を計算する
			D3DXVec3TransformCoord(&g_aWall[nCntWall].nor, &g_aWall[nCntWall].nor, &mtxRot);

			//位置を反映
			D3DXMatrixIdentity(&mtxTrans);
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
			D3DXMatrixMultiply(&mtxOut, &mtxRot, &mtxTrans);

			//頂点座標の設定
			pVtx[(nCntWall * 4) + 0].pos = D3DXVECTOR3(- (width * 0.5f), - (height * 0.5f), 0.0f);
			pVtx[(nCntWall * 4) + 1].pos = D3DXVECTOR3(+ (width * 0.5f), - (height * 0.5f), 0.0f);
			pVtx[(nCntWall * 4) + 2].pos = D3DXVECTOR3(- (width * 0.5f), + (height * 0.5f), 0.0f);
			pVtx[(nCntWall * 4) + 3].pos = D3DXVECTOR3(+ (width * 0.5f), + (height * 0.5f), 0.0f);

			//回転後の頂点座標を計算する
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 0].pos, &pVtx[(nCntWall * 4) + 0].pos, &mtxOut);
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 1].pos, &pVtx[(nCntWall * 4) + 1].pos, &mtxOut);
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 2].pos, &pVtx[(nCntWall * 4) + 2].pos, &mtxOut);
			D3DXVec3TransformCoord(&pVtx[(nCntWall * 4) + 3].pos, &pVtx[(nCntWall * 4) + 3].pos, &mtxOut);

			//頂点カラーの設定
			pVtx[(nCntWall * 4) + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntWall * 4) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntWall * 4) + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[(nCntWall * 4) + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_aWall[nCntWall].bUse = true;			//使用されている状態にする

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//壁に当たったら、反射する処理
void WallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pAcc, float fRadius)
{
		VERTEX_2D *pVtx = NULL;													//頂点情報へのポインタ
		D3DXVECTOR3 edge, position, lastPosition, result1, result2, point;		//計算用のベクトル

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

		for (int nIdx = 0; nIdx < MAX_WALL; nIdx++)
		{
			if (g_aWall[nIdx].bUse == true)
			{
				D3DXVECTOR3 Vtx1, Vtx2;

				//弾の一番近い点を求める
				point.x = pPos->x + (-g_aWall[nIdx].nor.x * fRadius);
				point.y = pPos->y + (-g_aWall[nIdx].nor.y * fRadius);
				point.z = 0.0f;

				edge = pVtx[(nIdx * 4) + 3].pos - pVtx[(nIdx * 4) + 2].pos;		//壁の辺のベクトル
				position = pVtx[(nIdx * 4) + 2].pos - point;					//現在の位置から壁の頂点までのベクトル
				lastPosition = pVtx[(nIdx * 4) + 2].pos - *(pLastPos);			//前回の位置から壁の頂点までのベクトル

				//外積を計算する。結果の符号が違う場合、弾は壁に当たった
				D3DXVec3Cross(&result1, &edge, &position);						
				D3DXVec3Cross(&result2, &edge, &lastPosition);					

				if (result1.z * result2.z <= 0)
				{//当たった場合

					//衝突点の座標の計算=======================================================================================

					D3DXVECTOR3 impact, newPosition;

					position = point - *(pLastPos);						//前回の位置から現在の位置までのベクトル

					//前回の位置から衝突点までの距離を計算する
					D3DXVec3Cross(&result1, &lastPosition, &edge);
					D3DXVec3Cross(&result2, &position, &edge);			

					float fLenght = (result1.z / result2.z) - 0.1f;		//前回の位置から衝突点までの距離

					D3DXVec3Normalize(&position, &position);			//前回の位置から現在の位置までのベクトルの向き

					impact = *(pLastPos)+(fLenght * position);			//衝突点

					lastPosition = *(pLastPos)-impact;					//前回の位置から衝突点までのベクトル


					////=========================================================================================================

					////反射処理=================================================================================================

					float fCostrLenght, fDot, fAlpha, fEdgeLenght, fheight, fBounceLenght, fmove;
					D3DXVECTOR3 constrPoint, dP, pN, bouncePoint, bounceDir, finalPos;

					fmove = sqrtf((pMove->x * pMove->x) + (pMove->y * pMove->y));																				//移動量のベクトルの大きさ
					fBounceLenght = sqrtf((((pPos->x - impact.x) * (pPos->x - impact.x)) + ((pPos->y - impact.y) * (pPos->y - impact.y))));						//衝突点から現在の位置までのベクトルの大きさ
					fCostrLenght = sqrtf((((impact.x - pLastPos->x) * (impact.x - pLastPos->x)) + ((impact.y - pLastPos->y) * (impact.y - pLastPos->y))));		//前回の位置から衝突点までのベクトルの大きさ
					fEdgeLenght = sqrtf((edge.x * edge.x) + (edge.y * edge.y));																					//壁の辺の長さ

					dP = D3DXVECTOR3(*(pPos)-*(pLastPos));		
					D3DXVec3Normalize(&pN, &dP);				//前回の位置から現在の位置までのベクトルの向き

					//反射後の点の座標の計算============================================
					constrPoint = impact + (pN * fCostrLenght);	
					dP = constrPoint - impact;					
					
					fDot = D3DXVec3Dot(&dP, &edge);
					fAlpha = acosf(fDot / (fEdgeLenght * fCostrLenght));

					fheight = fLenght * sinf(fAlpha);

					bouncePoint = constrPoint + (g_aWall[nIdx].nor * 2.0f * fheight);
					bounceDir = bouncePoint - impact;
					D3DXVec3Normalize(&bounceDir, &bounceDir);

					finalPos = impact + (bounceDir * fBounceLenght);
					//==================================================================

					*(pPos) = finalPos;							//弾の新しい位置の設定
					*(pMove) = fmove * bounceDir;				//弾の新しい移動量の設定
					*(pAcc) = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					float fY;
					float fBeta;

					if (impact.y < SCREEN_HEIGHT * 0.5f)
					{
						fY = 160.0f;
						fBeta = -D3DX_PI * 0.45f;
					}
					else
					{
						fY = SCREEN_HEIGHT - 110.0f;
						fBeta = D3DX_PI * 0.55f;
					}
					
					for (int nCntAngle = 0; nCntAngle < 20; nCntAngle++, fBeta += D3DX_PI * 0.05f)
					{
						SetEffect(D3DXVECTOR3(impact.x, fY, 0.0f), fBeta, EFFECT_TYPE_WALL_IMPACT);
					}
					PlaySound(SOUND_LABEL_SE_BOUNCE_NORMAL);
				}
			}
		}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//==============================================
//必殺技用の当たり判定
//==============================================
bool SpecialWallBounce(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pLastPos, D3DXVECTOR3 *pMove, float fRadius)
{
	VERTEX_2D *pVtx = NULL;													//頂点情報へのポインタ
	D3DXVECTOR3 edge, position, lastPosition, result1, result2, point;		//計算用のベクトル
	bool bImpact = false;													//当たったかどうか

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nIdx = 0; nIdx < MAX_WALL; nIdx++)
	{
		if (g_aWall[nIdx].bUse == true)
		{
			D3DXVECTOR3 Vtx1, Vtx2;

			//弾の一番近い点を求める
			point.x = pPos->x + (-g_aWall[nIdx].nor.x * fRadius);
			point.y = pPos->y + (-g_aWall[nIdx].nor.y * fRadius);
			point.z = 0.0f;

			edge = pVtx[(nIdx * 4) + 3].pos - pVtx[(nIdx * 4) + 2].pos;		//壁の辺のベクトル
			position = pVtx[(nIdx * 4) + 2].pos - point;					//現在の位置から壁の頂点までのベクトル
			lastPosition = pVtx[(nIdx * 4) + 2].pos - *(pLastPos);			//前回の位置から壁の頂点までのベクトル

			//外積を計算する。結果の符号が違う場合、弾は壁に当たった
			D3DXVec3Cross(&result1, &edge, &position);
			D3DXVec3Cross(&result2, &edge, &lastPosition);

			if (result1.z * result2.z <= 0)
			{//当たった場合

			 //衝突点の座標の計算=======================================================================================

				D3DXVECTOR3 impact, newPosition;

				position = point - *(pLastPos);						//前回の位置から現在の位置までのベクトル

																	//前回の位置から衝突点までの距離を計算する
				D3DXVec3Cross(&result1, &lastPosition, &edge);
				D3DXVec3Cross(&result2, &position, &edge);

				float fLenght = (result1.z / result2.z) - 0.1f;		//前回の位置から衝突点までの距離

				D3DXVec3Normalize(&position, &position);			//前回の位置から現在の位置までのベクトルの向き

				impact = *(pLastPos)+(fLenght * position);			//衝突点

				lastPosition = *(pLastPos)-impact;					//前回の位置から衝突点までのベクトル
				////=========================================================================================================

				*(pPos) = impact /*+ (g_aWall[nIdx].nor * fRadius)*/;	//
				bImpact = true;	

				PlaySound(SOUND_LABEL_SE_BOUNCE_SPECIAL);
			}
		}
	}

	return bImpact;

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}