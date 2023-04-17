//=============================================================================
//
// camera.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "camera.h"
#include "application.h"
#include "rendering.h"
#include "message.h"
#include "inputMouse.h"
#include "inputKeyboard.h"
#include "debugProc.h"

//=============================================================================
//
//静的変数の初期化
//
//=============================================================================
const float CCamera::CAMERA_SPEED = 1.0;				//カメラの移動量

//コンストラクタ
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//注視点
	m_DestPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//目的の視点
	m_DestPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//目的の注視点
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//目的の向き
	m_fLenght = 0.0f;									//視点から注視点までの距離			
	m_nDelay = 0;										//ディレイ		
}

//デストラクタ
CCamera::~CCamera()
{

}

//初期化処理
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//視点
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//注視点
	m_DestPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//目的の視点
	m_DestPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//目的の注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				//上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//目的の向き
	m_fLenght = 0.0f;									//視点から注視点までの距離	
	m_nDelay = 0;										//ディレイ		

	return S_OK;
}

//終了処理
void CCamera::Uninit(void)
{

}

//更新処理
void CCamera::Update(void)
{
#ifdef _DEBUG

	//デバッグ用のカメラ操作
	if (CInputKeyboard::GetKeyboardPress(DIK_LEFT))
	{
		D3DXVECTOR3 v = m_posR - m_posV;
		v.y = 0.0f;
		D3DXVec3Normalize(&v, &v);
		D3DXVECTOR3 unit = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		float fDot = D3DXVec3Dot(&v, &unit);
		float fAngle = acosf(fDot);
		if (v.z < 0.0f)
		{
			fAngle *= -1.0f;
		}

		fAngle += D3DX_PI * 0.5f;
		m_posV.x += cosf(fAngle) * 5.0f;
		m_posV.z += sinf(fAngle) * 5.0f;
		m_posR.x += cosf(fAngle) * 5.0f;
		m_posR.z += sinf(fAngle) * 5.0f;

	}
	if (CInputKeyboard::GetKeyboardPress(DIK_UP))
	{
		D3DXVECTOR3 v = m_posR - m_posV;
		v.y = 0.0f;
		D3DXVec3Normalize(&v, &v);
		D3DXVECTOR3 unit = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		float fDot = D3DXVec3Dot(&v, &unit);
		float fAngle = acosf(fDot);
		if (v.z < 0.0f)
		{
			fAngle *= -1.0f;
		}

		m_posV.x += cosf(fAngle) * 5.0f;
		m_posV.z += sinf(fAngle) * 5.0f;
		m_posR.x += cosf(fAngle) * 5.0f;
		m_posR.z += sinf(fAngle) * 5.0f;
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_RIGHT))
	{
		D3DXVECTOR3 v = m_posR - m_posV;
		v.y = 0.0f;
		D3DXVec3Normalize(&v, &v);
		D3DXVECTOR3 unit = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		float fDot = D3DXVec3Dot(&v, &unit);
		float fAngle = acosf(fDot);
		if (v.z < 0.0f)
		{
			fAngle *= -1.0f;
		}

		fAngle += -D3DX_PI * 0.5f;
		m_posV.x += cosf(fAngle) * 5.0f;
		m_posV.z += sinf(fAngle) * 5.0f;
		m_posR.x += cosf(fAngle) * 5.0f;
		m_posR.z += sinf(fAngle) * 5.0f;
	}
	if (CInputKeyboard::GetKeyboardPress(DIK_DOWN))
	{
		D3DXVECTOR3 v = m_posR - m_posV;
		v.y = 0.0f;
		D3DXVec3Normalize(&v, &v);
		D3DXVECTOR3 unit = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		float fDot = D3DXVec3Dot(&v, &unit);
		float fAngle = acosf(fDot);
		if (v.z < 0.0f)
		{
			fAngle *= -1.0f;
		}

		fAngle += D3DX_PI;
		m_posV.x += cosf(fAngle) * 5.0f;
		m_posV.z += sinf(fAngle) * 5.0f;
		m_posR.x += cosf(fAngle) * 5.0f;
		m_posR.z += sinf(fAngle) * 5.0f;
	}

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= 2.0f * D3DX_PI;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += 2.0f * D3DX_PI;
	}

	m_posR.x = m_posV.x + m_fLenght * cosf(m_rot.y);
	m_posR.z = m_posV.z + m_fLenght * sinf(m_rot.y);

	CDebugProc::Print("\nPosV: %f, %f, %f\nPosR: %f, %f, %f", m_posV.x, m_posV.y, m_posV.z, m_posR.x, m_posR.y, m_posR.z);

#endif // DEBUG

	if (CMessage::GetStart() && m_posV.z <= 750.0f || CApplication::GetMode() == CApplication::Mode_Tutorial)
	{
		D3DXVECTOR3 v = m_posR - m_posV;
		v.y = 0.0f;
		D3DXVec3Normalize(&v, &v);
		D3DXVECTOR3 unit = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		float fDot = D3DXVec3Dot(&v, &unit);
		float fAngle = acosf(fDot);
		if (v.z < 0.0f)
		{
			fAngle *= -1.0f;
		}

		m_posV.x += cosf(fAngle) * CAMERA_SPEED;
		m_posV.z += sinf(fAngle) * CAMERA_SPEED;
		m_posR.x += cosf(fAngle) * CAMERA_SPEED;
		m_posR.z += sinf(fAngle) * CAMERA_SPEED;
	}

	m_posR.x = m_posV.x + m_fLenght * cosf(m_rot.y);
	m_posR.z = m_posV.z + m_fLenght * sinf(m_rot.y);
}

//設定処理
void CCamera::Set(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		50000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//視点の取得処理
const D3DXVECTOR3 CCamera::GetPos(void)
{
	return m_posV;
}

//回転角度の取得処理
const D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

//カメラの向きの取得処理
const D3DXVECTOR3 CCamera::GetDirection(void)
{
	D3DXVECTOR3 dir = m_posR - m_posV;
	D3DXVec3Normalize(&dir, &dir);

	return dir;
}

//注視点の取得処理
const D3DXVECTOR3 CCamera::GetFocalPoint(void)
{
	return m_posR;
}

//注視点の設定処理
void CCamera::SetFocalPoint(const D3DXVECTOR3 pos)
{
	m_posR = pos;
}

//視点注視点の設定
void CCamera::SetPos(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR)
{
	m_posV = posV;
	m_posR = posR;
}



//=============================================================================
//
//								静的関数
//
//=============================================================================


//生成処理
CCamera* CCamera::Create(D3DXVECTOR3 pos, D3DXVECTOR3 focalPoint)
{
	CCamera* pCamera = new CCamera;				//カメラの生成

	if (FAILED(pCamera->Init()))
	{//初期化処理
		return nullptr;
	}

	pCamera->m_posV = pos;									//視点の設定
	pCamera->m_posR = focalPoint;							//注視点の設定
															
	D3DXVECTOR3 v = pCamera->m_posR - pCamera->m_posV;		//視点から注視点までのベクトルを計算する
	D3DXVECTOR3 unit = D3DXVECTOR3(1.0f, 0.0f, 0.0f);		//単位ベクトル
	D3DXVec3Normalize(&v, &v);								//視点から注視点までのベクトルを正規化する

	pCamera->m_rot.y = acosf(D3DXVec3Dot(&v, &unit));		//内積を計算する

	if (pCamera->m_posR.z < pCamera->m_posV.z)
	{//必要だったら、正規化する
		pCamera->m_rot.y *= -1.0f;
	}

	//視点から注視点までの距離を計算する
	pCamera->m_fLenght = sqrtf(((pCamera->m_posR.x - pCamera->m_posV.x) * (pCamera->m_posR.x - pCamera->m_posV.x)) + ((pCamera->m_posR.z - pCamera->m_posV.z) * (pCamera->m_posR.z - pCamera->m_posV.z)));
	
	return pCamera;				//生成したカメラを返す
}