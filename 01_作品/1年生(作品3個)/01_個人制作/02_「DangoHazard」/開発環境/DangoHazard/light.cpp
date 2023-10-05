//============================================================================
//
//ライトの処理[light.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "light.h"		//作成したcamera.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
LIGHT g_aLight;
D3DLIGHT9 g_light[MAX_LIGHT];	//ライト情報

//============================================================================
//ライトの初期化処理
//============================================================================
void InitLight(void)
{
	int nCntLight;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;		//ライトの方向ベクトル
	//ライトをクリアする
	ZeroMemory(&g_light[0], sizeof(g_light));

	//ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	//正規化する(大きさ1のベクトルにする)
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_light[0].Direction = vecDir;

	g_light[1].Direction = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
	g_light[2].Direction = D3DXVECTOR3(0.0f, 0.8f, 0.0f);

	for (nCntLight = 0;nCntLight < MAX_LIGHT; nCntLight++)
	{
		//ライトの種類を設定(平行光源)
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;		
		//ライトの拡散光を設定(ライトの色)
		g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//============================================================================
//ライトの終了処理
//============================================================================
void UninitLight(void)
{

}

//============================================================================
//ライトの更新処理
//============================================================================
void UpdateLight(void)
{

}