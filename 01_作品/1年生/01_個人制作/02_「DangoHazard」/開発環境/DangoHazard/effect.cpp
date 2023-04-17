//============================================================================
//
//エフェクトの処理[effect.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "effect.h"		//作成したeffect.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	//頂点バッファへのポインタ
EFFECT g_aEffect[MAX_EFFECT];

//============================================================================
//エフェクトの初期化処理
//============================================================================
void InitEffect(void)
{
	int nCntEffect;
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ
	pDevice = GetDevice();				//デバイスの取得

										//テクスチャーの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\TEXTURE\\effect\\Flash01.png",	//テクスチャのファイル名
		&g_pTextureEffect
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//位置の初期化処理
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].bUse = false;
		g_aEffect[nCntEffect].life = 0;
		g_aEffect[nCntEffect].fAngle = atan2f(EFFECT_WIDTH, EFFECT_HEIGHT);
		g_aEffect[nCntEffect].fLength = sqrtf(EFFECT_WIDTH * EFFECT_WIDTH + EFFECT_HEIGHT * EFFECT_HEIGHT) / 2.0f;
		g_aEffect[nCntEffect].Worldmtx = D3DXMATRIX();
		g_aEffect[nCntEffect].type = EFFECT_TYPE_BULLET;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	//頂点情報へのポインタ
	VERTEX_3D *pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
		pVtx[0].pos.x = -sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = cosf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

		pVtx[1].pos.x = sinf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = cosf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

		pVtx[2].pos.x = -sinf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = -cosf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

		pVtx[3].pos.x = sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
		pVtx[3].pos.y = 0.0f;
		pVtx[3].pos.z = -cosf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

		//各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
	g_pVtxBuffEffect->Unlock();

}

//============================================================================
//エフェクトの終了処理
//============================================================================
void UninitEffect(void)
{
	//テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;

	}
	//頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//============================================================================
//エフェクトの更新処理
//============================================================================
void UpdateEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//移動量反映
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			g_aEffect[nCntEffect].life--;
			if (g_aEffect[nCntEffect].life <= 0)
			{
				g_aEffect[nCntEffect].life = 0;
				g_aEffect[nCntEffect].bUse = false;
			}

			//頂点情報へのポインタ
			VERTEX_3D *pVtx = NULL;

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定 = (配置位置 ± 正弦(対角線の角度 ± 向き) * 対角線の長さ)
			pVtx[0].pos.x = -sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = cosf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

			pVtx[1].pos.x = sinf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
			pVtx[1].pos.y = 0.0f;
			pVtx[1].pos.z = cosf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

			pVtx[2].pos.x = -sinf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
			pVtx[2].pos.y = 0.0f;
			pVtx[2].pos.z = -cosf(g_aEffect[nCntEffect].fAngle - g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;

			pVtx[3].pos.x = sinf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.x) * g_aEffect[nCntEffect].fLength;
			pVtx[3].pos.y = 0.0f;
			pVtx[3].pos.z = -cosf(g_aEffect[nCntEffect].fAngle + g_aEffect[nCntEffect].rot.z) * g_aEffect[nCntEffect].fLength;
			pVtx += 4;

			//頂点バッファをアンロックする
			g_pVtxBuffEffect->Unlock();
		}
	}
}

//============================================================================
//エフェクトの描画処理
//============================================================================
void DrawEffect(void)
{
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			//aブレンディングを加算合成の設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//減算合成の設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].Worldmtx);

			//カメラの逆行列を設定
			g_aEffect[nCntEffect].Worldmtx._11 = mtxView._11;
			g_aEffect[nCntEffect].Worldmtx._12 = mtxView._21;
			g_aEffect[nCntEffect].Worldmtx._13 = mtxView._31;

			if (g_aEffect[nCntEffect].type == EFFECT_TYPE_BULLET)
			{
				g_aEffect[nCntEffect].Worldmtx._21 = mtxView._12;
				g_aEffect[nCntEffect].Worldmtx._22 = mtxView._22;
				g_aEffect[nCntEffect].Worldmtx._23 = mtxView._32;
			}

			g_aEffect[nCntEffect].Worldmtx._31 = mtxView._13;
			g_aEffect[nCntEffect].Worldmtx._32 = mtxView._23;
			g_aEffect[nCntEffect].Worldmtx._33 = mtxView._33;

			//向きを反映(YaW : y,Pitch : x,Roll : z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffect[nCntEffect].rot.y, g_aEffect[nCntEffect].rot.x, g_aEffect[nCntEffect].rot.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].Worldmtx, &g_aEffect[nCntEffect].Worldmtx, &mtxRot);
			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCntEffect].Worldmtx, &g_aEffect[nCntEffect].Worldmtx, &mtxTrans);

			//アルファテストを有効
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].Worldmtx);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//Zバッファを描画する
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//Zバッファの値が手前のZバッファの値より小さい場合
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//アルファテストを無効に戻す
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}
	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//============================================================================
//エフェクトの設定処理
//============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int life, EFFECT_TYPE type)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].rot = rot;
			g_aEffect[nCntEffect].life = life;
			g_aEffect[nCntEffect].type = type;

			g_aEffect[nCntEffect].bUse = true;
			break;
		}
	}
}