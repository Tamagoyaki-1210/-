//=============================================================================
//
// hitbox.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef HITBOX_H
#define HITBOX_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object3D.h"
#include <vector>

//=============================================================================
//インクルードファイル
//=============================================================================
class CObject;

class CHitbox
{
public:

	enum HITBOX_SHAPE
	{
		SHAPE_SPHERE = 0,
		SHAPE_BOX,
		SHAPE_CYLINDER,

		SHAPE_MAX
	};

	enum HITBOX_TYPE
	{
		TYPE_PLAYER = 0,
		TYPE_OBSTACLE,
		TYPE_NEUTRAL,
		TYPE_VANISHING,

		TYPE_MAX
	};

	enum INTERACTION_EFFECT
	{
		EFFECT_DAMAGE = 0,
		EFFECT_LAUNCH,
		EFFECT_PUSH,
		EFFECT_BOUNCE,
		EFFECT_JUMP,

		EFFECT_MAX
	};

	CHitbox();												//コンストラクタ
	virtual ~CHitbox();										//デストラクタ

	virtual HRESULT Init(void);								//初期化処理
	virtual void    Uninit(void);							//終了処理
	virtual void    Update(void);							//更新処理
															
	void Release(void);										//破棄処理
															
	D3DXVECTOR3  GetPos(void);								//位置の取得処理
	D3DXVECTOR3  GetRelativePos(void);						//相対位置の取得処理
	D3DXVECTOR3  GetLastPos(void);							//前回の位置の取得処理
	D3DXVECTOR3  GetSize(void);								//サイズの取得処理
	D3DXVECTOR3  GetRot(void);								//向きの取得処理
	HITBOX_SHAPE GetShape(void);							//形の取得処理
	HITBOX_TYPE  GetType(void);								//種類の取得処理
	CObject*	 GetParent(void);							//親の取得処理
	INTERACTION_EFFECT GetEffect(void);						//エフェクトの取得処理
	const D3DXVECTOR3  GetDirection(void);					//エフェクトの向きの取得処理
															
	void SetPos(const D3DXVECTOR3 pos);						//位置の設定処理
	void SetRelativePos(const D3DXVECTOR3 pos);				//相対位置の設定処理
	void SetLastPos(const D3DXVECTOR3 pos);					//前回の位置の設定処理
	void SetSize(const D3DXVECTOR3 size);					//サイズの設定処理
	void SetShape(const HITBOX_SHAPE shape);				//形の設定処理
	void SetType(const HITBOX_TYPE type);					//種類の設定処理
	void SetParent(CObject* pParent);						//親の設定処理
	void SetCollisionState(bool bCollision);				//何かと当たったかどうかの設定処理
	void SetPlayerIdx(const int nPlayerIdx);				//プレイヤーインデックスの設定処理
	void SetScore(const int nScore);						//スコアの設定処理
	void SetEffect(INTERACTION_EFFECT effect);				//エフェクトの設定処理
	void SetInvincibility(const bool bInv);					//無敵状態の設定処理
	void SetDirection(const D3DXVECTOR3 dir);				//エフェクトの向きの設定処理
															
	bool GetCollisionState(void);							//何かと当たったかどうかの取得処理
	const int GetPlayerIdx(void);							//プレイヤーインデックスの取得処理
	const int GetScore(void);								//スコアの取得処理
	const bool GetInvincibility(void);						//無敵状態の取得処理
															
	static void ReleaseAll(void);							//全部のヒットボックスの破棄処理
	static std::vector <CHitbox*>* GetAllHitbox(void);		//全部のヒットボックスの取得処理

private:

	D3DXVECTOR3  m_pos;										//位置
	D3DXVECTOR3  m_RelativePos;								//相対位置
	D3DXVECTOR3  m_LastPos;									//前回の位置
	D3DXVECTOR3  m_size;									//サイズ
	D3DXVECTOR3  m_rot;										//向き
	D3DXVECTOR3  m_dir;										//エフェクトの向き
	HITBOX_SHAPE m_shape;									//形
	HITBOX_TYPE  m_type;									//種類
	INTERACTION_EFFECT m_effect;							//エフェクト
	bool		 m_bCollided;								//何かと当たったかどうか
	CObject*	 m_pParent;									//親
	int			 m_nPlayerIdx;								//プレイヤーインデックス
	int			 m_nScore;									//スコア
	bool		 m_bInvincible;								//無敵状態であるかどうか
															
	static std::vector <CHitbox*> m_vHitbox;				//全部のヒットボックス
};



#endif
#pragma once