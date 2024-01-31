//=============================================================================
//
// ボスキャラ　　　boss.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "enemy.h"

//---------------------------
// クラス宣言
//---------------------------
class CBoss : public CEnemy
{
	static const int FIRST_MAX_LIFE = 100000;	// 初期最大体力の値
	static const float BOSS_COLLISION_RADIUS;	// ボスの当たり判定の大きさ
public:

	// パーツの種類
	enum PARTS
	{
		PARTS_BODY = 0,			// 胴体

		PARTS_MAX
	};

	// モーションのタイプ
	enum MOTION_TYPE
	{
		MOTION_NEUTRAL = 0,	// ニュートラル
		MOTION_WALK,		// 歩き
		MOTION_ATTACK1,		// 攻撃1
		MOTION_ATTACK2,		// 攻撃2
		MOTION_ATTACK3,		// 攻撃3
		MOTION_ENTRANCE,	// 登場
		MOTION_MAX,
	};

	CBoss();
	virtual ~CBoss() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Destroy() override;			// 破壊処理
	void ChangeMotion();
	void Landing(const D3DXVECTOR3 pos) override;	// 着地処理
	void Move() override;
	void Slash_Attack();
	void Bullet_Attack();

	static CBoss* Create(const D3DXVECTOR3 pos);

private:
	bool m_bOpening;
	int m_nAttack_Cooltime;
	int m_nBullet_Cooltime;
};

#endif// _BOSS_H_