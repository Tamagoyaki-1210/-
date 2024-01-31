//==============================================
//
//カメラの処理[camera.cpp]
//Author：tutida ryousei
//
//==============================================
#include"camera.h"
#include"input.h"
#include"application.h"
#include"game.h"
#include"title.h"

//==============================================
//コンストラクタ
//==============================================
CCamera::CCamera()
{

}

//==============================================
//デストラクタ
//==============================================
CCamera::~CCamera()
{

}

//==============================================
//カメラの初期化処理
//==============================================
void CCamera::Init(void)
{
	//視点・注視点・上方向を設定する
	m_posV = D3DXVECTOR3(800.0f, 1200.0f, -250.0f);	// 視点(オフセット)
	m_posR = D3DXVECTOR3(800.0f, 200.0f, 500.0f);			// 注視点(オフセット)
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 角度
	m_fRotSpeed = 0.03f;							// 回転速度
	m_CPos = { 0.0f,0.0f,0.0f };
}

//==============================================
//カメラの終了処理
//==============================================
void CCamera::Uninit(void)
{

}

//==============================================
//カメラの更新処理
//==============================================
void CCamera::Update(void)
{
	// 行列を使ったカメラ制御
	Matrix();

	// カメラの移動
	//Move();
}

//==============================================
// カメラのセット処理
//==============================================
void CCamera::SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの生成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_WorldCameraPosV,
		&m_WorldCameraPosR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの生成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)1280.0f / (float)720.0f,
		10.0f,
		10000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//==============================================
// 行列を使ったカメラ制御
//==============================================
void CCamera::Matrix()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

	// ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

	// 行列を回転に反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 行列に移動を反映(プレイヤーの位置)
	D3DXMatrixTranslation(&mtxTrans, m_CPos.x, m_CPos.y, m_CPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DXVec3TransformCoord(&m_WorldCameraPosV, &m_posV, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_WorldCameraPosR, &m_posR, &m_mtxWorld);
}

//==============================================
// カメラの移動
//==============================================
void CCamera::Move()
{
	// 入力デバイスの情報
	CInput* pInput = CInput::GetKey();

	// カメラの移動
	if (pInput->Press(DIK_RIGHT))
	{
		//m_rot.y += m_fRotSpeed;
		m_posV.x += 10.0f;
		m_posR.x += 10.0f;
	}
	if (pInput->Press(DIK_LEFT))
	{
		//m_rot.y -= m_fRotSpeed;
		m_posV.x -= 10.0f;
		m_posR.x -= 10.0f;
	}
	if (pInput->Press(DIK_UP))
	{
		//m_rot.x -= m_fRotSpeed;
		m_posV.z += 10.0f;
		m_posR.z += 10.0f;
	}
	if (pInput->Press(DIK_DOWN))
	{
		//m_rot.x += m_fRotSpeed;
		m_posV.z -= 10.0f;
		m_posR.z -= 10.0f;
	}

	if (pInput->Press(DIK_RSHIFT))
	{
		//m_rot.x -= m_fRotSpeed;
		m_posV.y += 10.0f;
		m_posR.y += 10.0f;
	}
	if (pInput->Press(DIK_RCONTROL))
	{
		//m_rot.x += m_fRotSpeed;
		m_posV.y -= 10.0f;
		m_posR.y -= 10.0f;
	}


	// 角度の正規化
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}