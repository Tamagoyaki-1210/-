//=============================================================================
//
// hitbox.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "hitbox.h"

//=============================================================================
//
//								静的変数
//
//=============================================================================
std::vector <CHitbox*> CHitbox::m_vHitbox;

//コンストラクタ
CHitbox::CHitbox()
{
	//メンバー変数をクリアする
	m_pos = Vec3Null;							//位置
	m_RelativePos = Vec3Null;					//相対位置
	m_LastPos = Vec3Null;						//前回の位置
	m_size = Vec3Null;							//サイズ
	m_rot = Vec3Null;							//向き
	m_shape = (HITBOX_SHAPE)0;					//エフェクトの向き
	m_type = (HITBOX_TYPE)0;					//形
	m_effect = (INTERACTION_EFFECT)0;			//種類
	m_bCollided = false;						//エフェクト
	m_pParent = nullptr;						//何かと当たったかどうか
	m_nPlayerIdx = 0;							//親
	m_nScore = 0;								//プレイヤーインデックス
	m_bInvincible = false;						//スコア
	m_dir = Vec3Null;							//無敵状態であるかどうか

	m_vHitbox.push_back(this);					//このインスタンスを追加する
}

//デストラクタ
CHitbox::~CHitbox()
{

}

//初期化処理
HRESULT CHitbox::Init(void)
{
	//初期化処理
	m_pos = Vec3Null;				//位置
	m_LastPos = Vec3Null;			//相対位置
	m_RelativePos = Vec3Null;		//前回の位置
	m_size = Vec3Null;				//サイズ
	m_rot = Vec3Null;				//向き
	m_shape = (HITBOX_SHAPE)0;		//エフェクトの向き
	m_type = (HITBOX_TYPE)0;		//形
	m_effect = EFFECT_MAX;			//種類
	m_bCollided = false;			//エフェクト
	m_pParent = nullptr;			//何かと当たったかどうか
	m_nPlayerIdx = -1;				//親
	m_nScore = 0;					//プレイヤーインデックス
	m_bInvincible = false;			//スコア
	m_dir = Vec3Null;				//無敵状態であるかどうか

	return S_OK;
}

//終了処理
void CHitbox::Uninit(void)
{

}

//更新処理
void CHitbox::Update(void)
{
	m_LastPos = m_pos;				//前回の位置の更新処理

	if (m_pParent != nullptr)
	{//親があったら、位置を更新する
		D3DXVECTOR3 newPos = m_pParent->GetPos() + m_RelativePos;
		m_pos = newPos;
	}
}

//ヒットボックス1個の破棄処理
void CHitbox::Release(void)
{
	for (int nCnt = 0; nCnt < (int)m_vHitbox.size(); nCnt++)
	{
		if (m_vHitbox.data()[nCnt] == this)
		{
			m_vHitbox.data()[nCnt]->Uninit();
			m_vHitbox.erase(m_vHitbox.begin() + nCnt);
			delete this;
		}
	}
}

//位置の取得処理
D3DXVECTOR3 CHitbox::GetPos(void)
{
	return m_pos;
}

//相対位置の取得処理
D3DXVECTOR3 CHitbox::GetRelativePos(void)
{
	return m_RelativePos;
}

//前回の位置の取得処理
D3DXVECTOR3 CHitbox::GetLastPos(void)
{
	return m_LastPos;
}

//サイズの取得処理
D3DXVECTOR3 CHitbox::GetSize(void)
{
	return m_size;
}

//向きの取得処理
D3DXVECTOR3 CHitbox::GetRot(void)
{
	return m_rot;
}

//形の取得処理
CHitbox::HITBOX_SHAPE CHitbox::GetShape(void)
{
	return m_shape;
}

//種類の取得処理
CHitbox::HITBOX_TYPE CHitbox::GetType(void)
{
	return m_type;
}

//親の取得処理
CObject* CHitbox::GetParent(void)
{
	return m_pParent;
}

//エフェクトの取得処理
CHitbox::INTERACTION_EFFECT CHitbox::GetEffect(void)
{
	return m_effect;
}

//エフェクトの向きの取得処理
const D3DXVECTOR3 CHitbox::GetDirection(void)
{
	return m_dir;
}


//位置の設定処理
void CHitbox::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos + m_RelativePos;
}

//相対位置の設定処理
void CHitbox::SetRelativePos(const D3DXVECTOR3 pos)
{
	m_RelativePos = pos;
}

//前回の位置の設定処理
void CHitbox::SetLastPos(const D3DXVECTOR3 pos)
{
	m_LastPos = pos;
}

//サイズの設定処理
void CHitbox::SetSize(const D3DXVECTOR3 size)
{
	m_size = size;
}

//形の設定処理
void CHitbox::SetShape(const HITBOX_SHAPE shape)
{
	m_shape = shape;
}

//種類の設定処理
void CHitbox::SetType(const HITBOX_TYPE type)
{
	m_type = type;
}

//親の設定処理
void CHitbox::SetParent(CObject* pParent)
{
	m_pParent = pParent;
}

//何かと当たったかどうかの設定処理
void CHitbox::SetCollisionState(bool bCollision)
{
	m_bCollided = bCollision;
}

//プレイヤーインデックスの設定処理
void CHitbox::SetPlayerIdx(const int nPlayerIdx)
{
	m_nPlayerIdx = nPlayerIdx;
}

//スコアの設定処理
void CHitbox::SetScore(const int nScore)
{
	m_nScore = nScore;
}

//エフェクトの設定処理
void CHitbox::SetEffect(INTERACTION_EFFECT effect)
{
	m_effect = effect;
}

//無敵状態の設定処理
void CHitbox::SetInvincibility(const bool bInv)
{
	m_bInvincible = bInv;
}

//エフェクトの向きの設定処理
void CHitbox::SetDirection(const D3DXVECTOR3 dir)
{
	m_dir = dir;
}

//何かと当たったかどうか
bool CHitbox::GetCollisionState(void)
{
	return m_bCollided;
}

//プレイヤーインデックスの取得処理
const int CHitbox::GetPlayerIdx(void)
{
	return m_nPlayerIdx;
}

//スコアの取得処理
const int CHitbox::GetScore(void)
{
	return m_nScore;
}

//無敵状態であるかどうかの取得処理
const bool CHitbox::GetInvincibility(void)
{
	return m_bInvincible;
}







//ヒットボックス全部の破棄処理
void CHitbox::ReleaseAll(void)
{
	CHitbox* pHitbox = nullptr;

	for (int nCnt = 0; nCnt < (int)m_vHitbox.size(); nCnt++)
	{
		pHitbox = m_vHitbox.data()[nCnt];
		delete pHitbox;
		pHitbox = nullptr;
	}

	m_vHitbox.clear();
}

std::vector <CHitbox*>* CHitbox::GetAllHitbox(void)
{
	return &m_vHitbox;
}