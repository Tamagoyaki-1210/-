//==============================================================================================
//
// プレイヤー　　　player.cpp
// tutida ryousei
//
//==============================================================================================
#include"player.h"
#include"application.h"
#include"input.h"
#include"camera.h"
#include"model.h"
#include"game.h"
#include"entry.h"
#include"countdown.h"
#include"time.h"
#include"gauge.h"
#include"charmanager.h"
#include"debugProc.h"
#include"score.h"
#include "particle_emitter.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CPlayer::CPlayer()
{
	m_SizeMax = { -1000.0f,-1000.0f,-1000.0f };
	m_SizeMin = { 1000.0f, 1000.0f, 1000.0f };

	// 攻撃力の初期値を設定
	SetMinePower(First_Mine_Power);
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CPlayer::~CPlayer()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CPlayer::Init()
{
	CCharacter::Init();

	// エントリーモードの時のみ未使用にする
	m_bEntry = CApplication::GetMode() != CApplication::MODE_ENTRY;

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CPlayer::Uninit()
{
	CCharacter::Uninit();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CPlayer::Update()
{
	// モーション番号の設定
	ChangeMotion();

	// モーション
	Motion();

	// ゲームが開始されている場合 又は エントリー画面の場合 且つ チュートリアル中では無い場合
	if ((CApplication::GetGameStart()
		|| (CApplication::GetMode() == CApplication::MODE_ENTRY && !CEntry::GetTutorial()))
		|| CApplication::GetMode() == CApplication::MODE_RESULT)
	{
		// ノックバック中か
		bool bKnockBack = CCharacter::GetKnockBack();

		// 入場
		bool bIn = CEntry::GetIn(GetIndex());

		// 目的の角度の取得
		m_RotDest = CCharacter::GetRotDest();

		if (!bKnockBack && ((bIn || CApplication::GetGameStart()))
			|| CApplication::GetMode() == CApplication::MODE_RESULT)
		{
			// アクション
			Action();

			// 移動処理
			Move();
		}

		// 目的の角度の設定
		CCharacter::SetRotDest(m_RotDest);

		CCharacter::Update();
	}
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CPlayer::Draw()
{
	CCharacter::Draw();
}

//==============================================================================================
// 生成処理
//==============================================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, int index)
{
	CPlayer *pPlayer = nullptr;

	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->CCharacter::SetPos(pos);
		pPlayer->SetIndex(index);
		pPlayer->Init();
	}

	return pPlayer;
}

//==============================================================================================
// 移動処理
//==============================================================================================
void CPlayer::Move()
{
	//入力デバイスの情報
	CInput* pInput = CInput::GetKey();

	D3DXVECTOR3 CRot = { 0.0f,0.0f,0.0f };

	// 移動量
	D3DXVECTOR3 move = { 0.0f, 0.0f, 0.0f };

	// カメラの角度
	CRot =  CApplication::GetCamera()->GetRot();

	const int nIndex = GetIndex();	// 自身の番号

	if (!GetAttack())
	{
		// プレイヤーの移動
		if ((pInput->Press(DIK_W) && nIndex == 0) || pInput->Press(JOYPAD_UP, nIndex))
		{
			if ((pInput->Press(DIK_A) && nIndex == 0) || pInput->Press(JOYPAD_LEFT, nIndex))
			{// 左前
				m_RotDest.y = CRot.y + D3DX_PI * 3 / 4;
				move.x = -sinf(CRot.y + D3DX_PI * 3 / 4);
				move.z = -cosf(CRot.y + D3DX_PI * 3 / 4);
			}
			else if ((pInput->Press(DIK_D) && nIndex == 0) || pInput->Press(JOYPAD_RIGHT, nIndex))
			{// 右前
				m_RotDest.y = CRot.y - D3DX_PI * 3 / 4;
				move.x = sinf(CRot.y + D3DX_PI / 4);
				move.z = cosf(CRot.y + D3DX_PI / 4);
			}
			else
			{// 前
				m_RotDest.y = CRot.y + D3DX_PI;
				move.x = sinf(CRot.y);
				move.z = cosf(CRot.y);
			}
		}
		else if ((pInput->Press(DIK_S) && nIndex == 0) || pInput->Press(JOYPAD_DOWN, nIndex))
		{
			if ((pInput->Press(DIK_A) && nIndex == 0) || pInput->Press(JOYPAD_LEFT, nIndex))
			{// 左下
				m_RotDest.y = CRot.y + D3DX_PI / 4;
				move.x = -sinf(CRot.y + D3DX_PI / 4);
				move.z = -cosf(CRot.y + D3DX_PI / 4);
			}
			else if ((pInput->Press(DIK_D) && nIndex == 0) || pInput->Press(JOYPAD_RIGHT, nIndex))
			{// 右下
				m_RotDest.y = CRot.y - D3DX_PI / 4;
				move.x = sinf(CRot.y + D3DX_PI * 3 / 4);
				move.z = cosf(CRot.y + D3DX_PI * 3 / 4);
			}
			else
			{// 下
				m_RotDest.y = CRot.y;
				move.x = -sinf(CRot.y);
				move.z = -cosf(CRot.y);
			}
		}
		else if ((pInput->Press(DIK_A) && nIndex == 0) || pInput->Press(JOYPAD_LEFT, nIndex))
		{// 左
			m_RotDest.y = CRot.y + D3DX_PI / 2;
			move.x = -sinf(CRot.y + D3DX_PI / 2);
			move.z = -cosf(CRot.y + D3DX_PI / 2);
		}
		else if ((pInput->Press(DIK_D) && nIndex == 0) || pInput->Press(JOYPAD_RIGHT, nIndex))
		{// 右
			m_RotDest.y = CRot.y - D3DX_PI / 2;
			move.x = sinf(CRot.y + D3DX_PI / 2);
			move.z = cosf(CRot.y + D3DX_PI / 2);
		}

		if ((pInput->Press(DIK_W) && nIndex == 0)
			|| (pInput->Press(DIK_A) && nIndex == 0)
			|| (pInput->Press(DIK_S) && nIndex == 0)
			|| (pInput->Press(DIK_D) && nIndex == 0)
			|| pInput->Press(JOYPAD_RIGHT, nIndex)
			|| pInput->Press(JOYPAD_LEFT, nIndex)
			|| pInput->Press(JOYPAD_UP, nIndex)
			|| pInput->Press(JOYPAD_DOWN, nIndex))
		{
			// 歩き
			SetMotion(MOTION_WALK);
			SetMove(true);
		}
		else
		{
			// 歩きを終了させる
			SetMotion(MOTION_NEUTRAL);
		}
		// 移動量を更新
		CCharacter::SetMove(move);

		// 移動処理
		CCharacter::Move();
	}
}

//==============================================================================================
// アクション
//==============================================================================================
void CPlayer::Action()
{
	//入力デバイスの情報
	CInput* pInput = CInput::GetKey();

	// プレイヤーの番号
	const int nIndex = GetIndex();

	// 攻撃する処理
	if (((pInput->Trigger(DIK_SPACE) && nIndex == 0)
		|| (pInput->Trigger(JOYPAD_B, nIndex)) || pInput->Trigger(JOYPAD_A, nIndex))
		&& GetMotion_Type() != MOTION_MINE
		&& m_bEntry)
	{
		// 攻撃モーション
		SetMotion(MOTION_MINE);
		
		// 攻撃処理
		CCharacter::SetAttack(true);
	}

	// エントリーモードのエントリー処理が終わったら使用する
	if (m_bEntry == false)
	{
		m_bEntry = true;
	}
}