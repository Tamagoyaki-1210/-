//==============================================
//
// カメラの処理[camera.cpp]
// Author：tutida ryousei
// Author：tanimoto kosuke
// Author：saito shian
//
//==============================================
#include "camera.h"
#include "input.h"
#include "application.h"
#include "game.h"
#include "tutorial.h"
#include "player_manager.h"
#include "mode.h"
#include "fade.h"
#include"debugProc.h"
#include "utility.h"

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
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 視点(オフセット)
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 注視点(オフセット)
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 上方向ベクトル
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 角度
	m_fRotSpeed = 0.03f;							// 回転速度
	m_CPos = { 0.0f,0.0f,0.0f };					// キャラクターの位置
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の視点
	m_posRDest = D3DXVECTOR3(600.0f, 150.0f, 0.0f);	// 目的の注視点
	m_bPerspective = false;
	m_bValue = false;
	m_bOpening = false;								// オープニングの有無

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_bOpening_step[nCnt] = false;
	}
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
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	if (((Mode == CApplication::MODE_GAME && CGame::GetPlayerManager()->GetPlayer(0) != nullptr)
		|| (Mode == CApplication::MODE_TUTORIAL && CTutorial::GetPlayerManager()->GetPlayer(0) != nullptr))
		&& !m_bOpening)
	{
		// 視点移動
		Perspective();
	}
	else if (CApplication::GetModeType() == CApplication::MODE_TITLE
		|| CApplication::GetModeType() == CApplication::MODE_CHAR_SELECT
		|| CApplication::GetModeType() == CApplication::MODE_RESULT)
	{
		Matrix(D3DXVECTOR3(-0.5f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 600.0f, 0.0f));
	}

#ifdef _DEBUG
	// カメラの入力処理
	DebugInput();
#endif // !_DEBUG

	if ((Mode == CApplication::MODE_GAME || Mode == CApplication::MODE_TUTORIAL))
	{
		// 画面の揺れ
		if (m_bTremor)
			Tremor();
		// オープニング
		if (m_bOpening)
			Opening_Move();
	}
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
		&m_worldCameraPosV,
		&m_worldCameraPosR,
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
		50000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

//==============================================
// 行列を使ったカメラ制御
//==============================================
void CCamera::Matrix(D3DXVECTOR3 rot, D3DXVECTOR3 pos)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

	// ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

	// 行列を回転に反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 行列に移動を反映(プレイヤーの位置)
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	D3DXVec3TransformCoord(&m_worldCameraPosV, &m_posV, &m_mtxWorld);
	D3DXVec3TransformCoord(&m_worldCameraPosR, &m_posR, &m_mtxWorld);
}

//==============================================
// カメラの入力処理
//==============================================
void CCamera::DebugInput()
{
	// 入力デバイスの情報
	CInput* pInput = CInput::GetKey();

	// カメラ視点切り替え
	if (pInput->Trigger(DIK_F1))
	{
		m_bPerspective = !m_bPerspective;
	}

	// カメラ視点を切り替えた場合
	if (m_bPerspective == true)
	{
		// カメラの移動処理
		Move();
	}
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
	if (pInput->Press(DIK_Q))
	{
		m_rot.y -= m_fRotSpeed;
	}
	if (pInput->Press(DIK_E))
	{
		m_rot.y += m_fRotSpeed;
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

//==============================================
// カメラの視点
//==============================================
void CCamera::Perspective()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	CPlayer* pPlayer = nullptr;

	// モード毎にプレイヤーを読み込む
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
	}

	if (m_bPerspective == false)
	{	// プレイヤーが使われていたら実行

		//取得処理
		m_PPos = pPlayer->GetPos();
		m_bValue = false;
	}
	else if (m_bPerspective == true && m_bValue == false)
	{	// プレイヤーが使われていたら実行

		//取得処理
		m_PPos = pPlayer->GetPos();
		m_bValue = true;
	}
	// 行列を使ったカメラ制御
	Matrix(m_rot, { m_PPos.x, m_PPos.y, m_PPos.z });
}

//==============================================
// オープニング
//==============================================
void CCamera::Opening_Move()
{
	// 入力デバイスの情報
	CInput* pInput = CInput::GetKey();

	// スキップ
	if (pInput->Trigger(DIK_RETURN) || pInput->Trigger(JOYPAD_START))
	{
		m_posV = { 0.0f, 225.0f, -450.0f };
		m_posR = { 0.0f, 112.5f, 450.0f };

		// オープニング終了
		m_bOpening = false;
	}

	// 行列を使ったカメラ制御
	Matrix(m_rot, { 0.0f,0.0f,0.0f });

	if (!m_bOpening_step[0])
	{// ステップ0
		m_nChange_Count++;

		// 注視点の移動
		if (m_posR.y > 80.0f)
			m_posR.y -= 29.0f;
		else if(m_posR.y <= 80.0f && !m_bTremor)
		{
			// カメラの揺れ
			m_Base_PosV = m_posV;
			m_Base_PosR = m_posR;

			m_fMax_Scale = 15.0f;
			m_bTremor = true;
		}

		if (m_nChange_Count == 300)
		{
			// 次のステップに進める
			m_nChange_Count = 0;
			m_bOpening_step[0] = true;

			m_posV = { 1500.0f,1000.0f,-5000.0f };
			m_posR = { 0.0f,1000.0f,0.0f };

			m_bTremor = false;
		}
	}
	else if (m_bOpening_step[0] && !m_bOpening_step[1])
	{// ステップ1
		m_nChange_Count++;

		float f = D3DX_PI * 2 / 0.05f;

		if (m_nChange_Count < f)
			m_rot.y -= 0.05f;

		// 1フレームの移動量
		float X = 1500 / f;
		float Z = 4000 / f;
		float Y = 500 / f;

		// 視点、注視点の移動
		if (m_posV.x > 0)
			m_posV.x -= X;
		if (m_posV.z < -1000)
			m_posV.z += Z;
		if (m_posV.y < 1500)
		{
			m_posV.y += Y;
			m_posR.y += Y;
		}

		if (m_nChange_Count == 160)
		{
			// 次のステップに進める
			m_nChange_Count = 0;
			m_bOpening_step[1] = true;

			m_posV = { 0.0f,1500.0f,-1000.0f };
			m_posR = { 0.0f,1500.0f,0.0f };
			m_rot.y = 0.0f;
		}
	}
	else if (m_bOpening_step[1] && !m_bOpening_step[2])
	{// ステップ2
		m_nChange_Count++;

		// 1フレームの移動量
		float X = 4000 / 20;
		float Y = 1400 / 20;

		// 視点の移動
		if (m_posV.z > -5000)
			m_posV.z -= X;
		if (m_posV.y > 100)
			m_posV.y -= Y;

		if (m_nChange_Count == 100)
		{
			// 次のステップに進める
			m_nChange_Count = 0;
			m_bOpening_step[2] = true;

			m_posV = { 0.0f, 225.0f, -450.0f };
			m_posR = { 0.0f, 112.5f, 450.0f };

			// オープニング終了
			m_bOpening = false;
		}
	}
}

//==============================================
// 画面の揺れ
//==============================================
void CCamera::Tremor()
{
	// ランダムな数値
	float fRand_TreX = utility::Random<float>(m_fMax_Scale, -m_fMax_Scale);
	float fRand_TreY = utility::Random<float>(m_fMax_Scale, -m_fMax_Scale);

	// 視点の移動
	m_posV.x = m_Base_PosV.x + fRand_TreX;
	m_posV.y = m_Base_PosV.y + fRand_TreY;

	// 注視点の移動
	m_posR.x = m_Base_PosR.x + fRand_TreX;
	m_posR.y = m_Base_PosR.y + fRand_TreY;

	// 揺れを少しずつ小さくする
	if (m_fMax_Scale > 0)
		m_fMax_Scale -= 0.1f;
}