//=============================================================================
//
// enemy.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "character.h"

//---------------------------
// クラス宣言
//---------------------------
class CEnemy : public CCharacter
{

public:
	// 敵キャラの種類
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_MOB = 0,	// モブキャラ
		ENEMY_TYPE_BOSS		// ボスキャラ
	};

	CEnemy();
	virtual ~CEnemy() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	
	virtual void Move();
	void Hit(CMove_Object* pHit) override;

	virtual void Destroy() override;			// 破壊処理

	void SetEnemyType(ENEMY_TYPE type) { m_type = type; }	// 敵キャラの種類を設定
	void SetTracking(bool tracking) { m_bTracking = tracking; }

	ENEMY_TYPE GetEnemyType() { return m_type; }			// 敵キャラの種類を取得
	bool GetTracking() { return m_bTracking; }				// 敵が追跡状態か
	float GetDistance() { return m_fDistance; }				// 敵とプレイヤーの距離
	float GetAngle() { return m_fAngle; }					// プレイヤーまでの角度

private:
	ENEMY_TYPE m_type;		// 敵キャラの種類
	bool m_bTracking;		// 追跡状態
	float m_fDistance;		// プレイヤーまでの距離
	float m_fAngle;			// プレイヤーまでの角度
};

#endif// _ENEMY_H_