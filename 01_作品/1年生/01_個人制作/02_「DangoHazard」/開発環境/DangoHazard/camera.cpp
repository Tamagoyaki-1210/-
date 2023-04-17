//============================================================================
//
//カメラの処理[camera.cpp] (ファイルヘッダコメント)
//Author:谷本康輔
//============================================================================
#include "camera.h"		//作成したcamera.hをインクルードする
#include "input.h"		//作成したinput.hをインクルードする
#include "player.h"		//作成したplayer.hをインクルードする

//****************************************************************************
//グローバル変数
//****************************************************************************
CAMERA g_aCamera;									

//============================================================================
//カメラの初期化処理
//============================================================================
void InitCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ

	pDevice = GetDevice();				//デバイスの取得

	//位置の初期化処理
	g_aCamera.posV = D3DXVECTOR3(0.0f, 60.0f, -400.0f);
	g_aCamera.posR = D3DXVECTOR3(0.0f,20.0f,-300.0f);
	g_aCamera.posVDest = D3DXVECTOR3(0.0f, 60.0f, -100.0f);
	g_aCamera.posRDest = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	g_aCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_aCamera.rot = D3DXVECTOR3(0.0f,0.0f, 0.0f);
	g_aCamera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aCamera.fDistance = sqrtf((g_aCamera.posV.z - g_aCamera.posR.z) * (g_aCamera.posV.z - g_aCamera.posR.z)
		+ (g_aCamera.posV.x - g_aCamera.posR.x) * (g_aCamera.posV.x - g_aCamera.posR.x)) / 2;		//視点から注視点の距離
	g_aCamera.bFollowUse = false;
}

//============================================================================
//カメラの終了処理
//============================================================================
void UninitCamera(void)
{

}

//============================================================================
//カメラの更新処理
//============================================================================
void UpdateCamera(void)
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//ポリゴンを動かす処理
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//視点 & 注視点の同時移動
	if (GetKeyboardPress(DIK_LEFT) == true || GetKeyboardPress(DIK_J) == true)
	{//カメラ左移動
		g_aCamera.posV.x -= sinf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posR.x -= sinf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posV.z -= cosf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posR.z -= cosf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true || GetKeyboardPress(DIK_L) == true)
	{//カメラ右移動
		g_aCamera.posV.x += sinf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posR.x += sinf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posV.z += cosf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
		g_aCamera.posR.z += cosf(g_aCamera.rot.y + (D3DX_PI / 2)) * CAMERA_SPEAD;
	}
	if (GetKeyboardPress(DIK_UP) == true || GetKeyboardPress(DIK_I) == true)
	{//カメラ前方移動
		g_aCamera.posV.x += sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.x += sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posV.z += cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.z += cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true || GetKeyboardPress(DIK_K) == true)
	{//カメラ後方移動
		g_aCamera.posV.x -= sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.x -= sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posV.z -= cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.z -= cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
	}
	if (GetKeyboardPress(DIK_U) == true)
	{//カメラ上移動
		g_aCamera.posV.y+= 1;
		g_aCamera.posR.y++;
		g_aCamera.posV.x -= sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.x -= sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posV.z -= cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.z -= cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
	}
	else if (GetKeyboardPress(DIK_O) == true)
	{//カメラ下移動
		g_aCamera.posV.y-= 1;
		g_aCamera.posR.y--;
		g_aCamera.posV.x += sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.x += sinf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posV.z += cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
		g_aCamera.posR.z += cosf(g_aCamera.rot.y) * CAMERA_SPEAD;
	}
	//if (GetKeyboardPress(DIK_M) == true)
	//{//カメラ上移動
	//	g_aCamera.posV.y += 1.0f;
	//	//g_aCamera.posR.y += 1.0f;
	//}
	//else if (GetKeyboardPress(DIK_N) == true)
	//{//カメラ下移動
	//	g_aCamera.posV.y -= 1.0f;
	//	//g_aCamera.posR.y -= 1.0f;
	//}
	//if (GetKeyboardPress(DIK_U) == true)
	//{//カメラ注視点上移動
	//	g_aCamera.posR.y++;
	//}
	//else if (GetKeyboardPress(DIK_I) == true)
	//{//カメラ注視点下移動
	//	g_aCamera.posR.y--;
	//}

	////注視点の旋回
	//if (GetKeyboardPress(DIK_Z) == true)
	//{//左回り
	//	g_aCamera.rot.y -= 0.1f;
	//	g_aCamera.posR.x = g_aCamera.posV.x + sinf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//	g_aCamera.posR.z = g_aCamera.posV.z + cosf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_X) == true)
	//{//右回り
	//	g_aCamera.rot.y += 0.1f;
	//	g_aCamera.posR.x = g_aCamera.posV.x + sinf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//	g_aCamera.posR.z = g_aCamera.posV.z + cosf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//}

	////視点の旋回
	//if (GetKeyboardPress(DIK_C) == true)
	//{//左回り
	//	g_aCamera.rot.y += 0.1f;
	//	g_aCamera.posV.x = g_aCamera.posR.x - sinf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//	g_aCamera.posV.z = g_aCamera.posR.z - cosf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//}
	//else if (GetKeyboardPress(DIK_V) == true)
	//{//右回り
	//	g_aCamera.rot.y -= 0.1f;
	//	g_aCamera.posV.x = g_aCamera.posR.x - sinf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//	g_aCamera.posV.z = g_aCamera.posR.z - cosf(g_aCamera.rot.y) * g_aCamera.fDistance;
	//}

	PLAYER *pPlayer = GetPlayer();

	////カメラの追従処理
	//if (g_aCamera.bFollowUse == true)
	//{
	//	g_aCamera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.x) * pPlayer->fLength;
	//	g_aCamera.posRDest.z = pPlayer->pos.z + sinf(pPlayer->rot.z) * pPlayer->fLength;

	//	g_aCamera.posVDest.x = pPlayer->pos.x + sinf(g_aCamera.rot.x) * g_aCamera.fDistance;
	//	g_aCamera.posVDest.z = pPlayer->pos.z + sinf(g_aCamera.rot.z) * g_aCamera.fDistance;

	//	g_aCamera.posR.x += (g_aCamera.posRDest.x - g_aCamera.posR.x) * 0.1f;
	//	g_aCamera.posV.x += (g_aCamera.posVDest.x - g_aCamera.posV.x) * 0.1f;
	//	g_aCamera.posR.z += (g_aCamera.posRDest.z - g_aCamera.posR.z) * 0.1f;
	//	g_aCamera.posV.z += (g_aCamera.posVDest.z - g_aCamera.posV.z) * 0.1f;
	//	//g_aCamera.posR += (g_aCamera.posRDest - g_aCamera.posR) * 0.1f;
	//	//g_aCamera.posV += (g_aCamera.posVDest - g_aCamera.posV) * 0.1f;
	//}

	if (GetKeyboardTrigger(DIK_F3) == true)
	{//カメラ追従使用処理
		g_aCamera.rot.y = 0.0f;
		g_aCamera.bFollowUse = !g_aCamera.bFollowUse;
	}

	if (g_aCamera.rot.y > D3DX_PI)
	{//カメラ向きの正規化処理
		g_aCamera.rot.y = -D3DX_PI;
	}
	else if(g_aCamera.rot.y < -D3DX_PI)
	{
		g_aCamera.rot.y = D3DX_PI;
	}
}

//============================================================================
//カメラの設定処理
//============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_aCamera.mtxView);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&g_aCamera.mtxView,
		&g_aCamera.posV,
		&g_aCamera.posR,
		&g_aCamera.vecU
	);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_aCamera.mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_aCamera.mtxProjection);
	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(
		&g_aCamera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//アスペクト比
		CAMERA_NEAR,CAMERA_FAR
	);
	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_aCamera.mtxProjection);

	//g_aCamera.bFollowUse = true;
}

//============================================================================
//カメラの取得処理
//============================================================================
CAMERA *GetCamera(void)
{
	return &g_aCamera;
}