//=============================================================================
//
// ghost.cpp
// Author : Tanimoto Kosuke
//
//=============================================================================
#include "ghost.h"
#include "application.h"
#include "game.h"
#include "charmanager.h"
#include "enemymanager.h"

const float CGhost::Ghost_Roaming_Speed = 0.4f;			// ゴーストの徘徊速度
const float CGhost::Ghost_Search_Speed = 0.5f;			// ゴーストの探索速度
const float CGhost::Ghost_Roaming_Clear = 0.5f;			// ゴーストの徘徊透明度
const float CGhost::Ghost_Radius_Collision = 45.0f;		// 当たり判定のサイズ(半径)
const float CGhost::Ghost_Radius_Search = 250.0f;		// 検索のサイズ(半径)
const float CGhost::Ghost_Roaming_Length = 450.0f;		// 徘徊する距離
const float CGhost::Ghost_Clear_Speed = 0.05f;			// 透明になる速度

//=====================================
// デフォルトコンストラクタ
//=====================================
CGhost::CGhost()
{
	SetKnockBack(Ghost_KnockBack_Power);
	SetRadius(Ghost_Radius_Collision);
	SetSpeed(Ghost_Roaming_Speed);
	m_Roaming = ROAMING_NONE;
}

//=====================================
// デストラクタ
//=====================================
CGhost::~CGhost()
{

}

//============================================================================
// 初期化処理
//============================================================================
HRESULT CGhost::Init()
{
	CEnemy::Init();

	// 初期向きをランダムにする
	SetRotDest({ 0.0f, ((rand() % 628) - 314) / 100.0f, 0.0f });

	// 半分の確率で回転方向を選ぶ
	m_bClockwise = rand() % 2 == 0;

	return S_OK;
}

//============================================================================
// 終了処理
//============================================================================
void CGhost::Uninit()
{
	CEnemy::Uninit();
}

//============================================================================
// 更新処理
//============================================================================
void CGhost::Update()
{
	// モーション番号の設定
	ChangeMotion();

	// モーション
	Motion();

	// ゲームが開始されている場合
	if (CApplication::GetGameStart())
	{
		AI();
		NormalizeRot();
		CEnemy::Update();
	}
}

//============================================================================
// 描画処理
//============================================================================
void CGhost::Draw()
{
	CEnemy::Draw();
}

//============================================================================
// モーション変更
//============================================================================
void CGhost::ChangeMotion()
{
	// 現在のモーション
	const int nCuttentMotion = GetCurrentMotion();
	const int nMotion = GetMotion();

	// 現在のモーションから変わった場合
	if (nCuttentMotion != nMotion)
	{
		// 現在モーションの終了処理
		switch (nCuttentMotion)
		{
		case GHOST_MOTION_NEUTRAL:
			break;
		case GHOST_MOTION_WALK:
			m_Roaming = ROAMING_NONE;
			AddRotDest({ 0.0f, D3DX_PI, 0.0f });
			break;
		default:
			break;
		}

		// 現在モーションの開始処理
		switch (nMotion)
		{
		case GHOST_MOTION_NEUTRAL:
			SetSpeed(Ghost_Roaming_Speed);
			// 半分の確率で回転方向を選ぶ
			m_bClockwise = rand() % 2 == 0;
			break;
		case GHOST_MOTION_WALK:
			SetSpeed(Ghost_Search_Speed);
			break;
		default:
			break;
		}

		CEnemy::ChangeMotion();
	}
}

//============================================================================
// AI処理
//============================================================================
void CGhost::AI()
{
	Search();
}

//============================================================================
// 探索処理
//============================================================================
void CGhost::Search()
{
	CCharacter* pNearChara = nullptr;	// 距離が一番近いキャラクターのポインタ
	float fNearDistance = 0.0f;

	// 一番距離が近いキャラクターを探す
	for (int index = 0; index < CCharManager::Max_Character; index++)
	{
		// キャラクターのポインタを取得
		CCharacter* pChara = CApplication::GetCharManager()->GetChara(index);

		// 使用中の安全地帯にいないキャラを読み込む
		if (pChara != nullptr)
		{
			// 位置同士の距離の計算用変数
			D3DXVECTOR2 distance = { pChara->GetPos().x - GetPos().x, pChara->GetPos().z - GetPos().z };
			float fDistance = sqrtf((distance.x * distance.x) + (distance.y * distance.y));

			// 距離が小さい場合、又は最初の処理の場合確保する
			if (fNearDistance > fDistance || fNearDistance == 0)
			{
				fNearDistance = fDistance;
				pNearChara = pChara;
			}
		}
	}

	// キャラクターが一人も使用されていない場合
	if (pNearChara == nullptr)
	{
		SetMotion(GHOST_MOTION_NEUTRAL);
		Roaming();
		return;
	}

	// 索敵範囲に入った場合で
	if (CollisionCircle(GetPos(), Ghost_Radius_Search, pNearChara->GetPos(), pNearChara->GetRadius()))
	{
		// ターゲットが安全地帯にいる場合終了する
		if (pNearChara->GetSafe() == true)
		{
			Death();
			return;
		}
		// 目的位置と自身の位置の差分
		D3DXVECTOR2 distance = { pNearChara->GetPos().x - GetPos().x, pNearChara->GetPos().z - GetPos().z };

		// 向かう方向へ向く
		float fAngle = (float)atan2(-distance.x, -distance.y);

		// キャラクターの方を向く
		SetRotDest({ 0.0f, fAngle, 0.0f });

		SetMotion(GHOST_MOTION_WALK);
		m_Roaming = ROAMING_NONE;

		float valNormalize = 0.0f;

		D3DXVECTOR2 move = { 0.0f, 0.0f };
		D3DXVec2Normalize(&move, &distance);
		AddMove({ move.x, 0.0f, move.y });

		// モデルのアルファ値の設定
		if (m_fAlpha != 1.0f)
		{
			// 不透明にする
			m_fAlpha < 1.0f ? m_fAlpha += Ghost_Clear_Speed : m_fAlpha = 1.0f;

			// 自身の全てのモデルにアルファ値を設定する
			for (auto pModel : GetModel()) pModel->SetAlpha(m_fAlpha);
		}
		// 当たり判定を読み込む
		CollEnemy();
	}
	// 検索範囲に誰もいない場合
	else
	{
		// 徘徊させる
		SetMotion(GHOST_MOTION_NEUTRAL);
		Roaming();
	}
}

//============================================================================
// 徘徊処理
//============================================================================
void CGhost::Roaming()
{
	// 自身の位置を取得
	const D3DXVECTOR3 pos = GetPos();

	// 上に向かう
	if (m_anchorPos.x - Ghost_Roaming_Length > pos.x && (m_Roaming == (m_bClockwise ? ROAMING_LEFT : ROAMING_RIGHT) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? D3DX_PI : 0.0f, 0.0f });
		m_Roaming = ROAMING_UP;
	}
	// 右に向かう
	if (m_anchorPos.z + Ghost_Roaming_Length < pos.z && (m_Roaming == (m_bClockwise ? ROAMING_UP : ROAMING_BOTTOM) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? -D3DX_PI / 2 : D3DX_PI / 2, 0.0f });
		m_Roaming = ROAMING_RIGHT;
	}
	// 下に向かう
	if (m_anchorPos.x + Ghost_Roaming_Length < pos.x && (m_Roaming == (m_bClockwise ? ROAMING_RIGHT : ROAMING_LEFT) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? 0.0f : D3DX_PI, 0.0f });
		m_Roaming = ROAMING_BOTTOM;
	}
	// 左に向かう
	if (m_anchorPos.z - Ghost_Roaming_Length > pos.z && (m_Roaming == (m_bClockwise ? ROAMING_BOTTOM : ROAMING_UP) || m_Roaming == ROAMING_NONE))
	{
		SetRotDest({ 0.0f, m_bClockwise ? D3DX_PI / 2 : -D3DX_PI / 2, 0.0f });
		m_Roaming = ROAMING_LEFT;
	}

	// 角度の方向に真っ直ぐ移動
	AddMove({ -sinf(GetRot().y), 0.0f, -cosf(GetRot().y) });

	// モデルのアルファ値の設定
	if (m_fAlpha != Ghost_Roaming_Clear)
	{
		// 透明にする
		m_fAlpha > Ghost_Roaming_Clear ? m_fAlpha -= Ghost_Clear_Speed : m_fAlpha = Ghost_Roaming_Clear;

		// 自身の全てのモデルにアルファ値を設定する
		for (auto pModel : GetModel()) pModel->SetAlpha(m_fAlpha);
	}
}

//============================================================================
// 当たり判定の処理
//============================================================================
void CGhost::CollEnemy()
{
	// 自身の位置を取得
	const D3DXVECTOR3 pos = GetPos();

	// 全ての敵キャラのポインタを読み込む
	for (auto pEnemy : CGame::GetEnemyManager()->GetAllEnemy())
	{
		//ポインタの解放
		if (pEnemy != nullptr && pEnemy != this)
		{
			const D3DXVECTOR3 atherPos = pEnemy->GetPos();

			//衝突検知
			if (CollisionCircle(pos, GetRadius(), atherPos, pEnemy->GetRadius()))
			{
				//ベクトルの算出
				D3DXVECTOR3 vector = pos - atherPos;
				D3DXVec3Normalize(&vector, &vector);

				//長さの調整(ゴースト + 敵キャラ)
				AddMove(vector);

				// 1匹だけを読み込んで終了
				break;
			}
		}
	}
}

//============================================================================
// 生成処理
//============================================================================
CGhost* CGhost::Create(const D3DXVECTOR3 pos, const float alpha)
{
	CGhost* pGhost = new CGhost;

	pGhost->LoadFileEnemy("Data/model/Motion_Enemy/Motion_Ghost/motion_ghost.txt");

	if (FAILED(pGhost->Init()))
	{
		return nullptr;
	}

	pGhost->SetPos(pos);
	pGhost->m_anchorPos = pos;
	pGhost->m_fAlpha = alpha;

	// モデルのa値設定
	for (auto pModel : pGhost->GetModel())
	{
		pModel->SetAlpha(alpha);

		// ゴーストの影は描画しない
		pModel->SetShadow(false);
	}

	return pGhost;
}