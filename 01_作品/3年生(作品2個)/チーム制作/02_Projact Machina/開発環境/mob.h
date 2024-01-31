//=============================================================================
//
// モブキャラ　　　mob.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _MOB_H_
#define _MOB_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "enemy.h"

class CMob_Life_Gauge;
class CBoss;

//---------------------------
// クラス宣言
//---------------------------
class CMob : public CEnemy
{
	static const int FIRST_MAX_LIFE = 2000;		// 初期最大体力の値
	static const float MOB_COLLISION_RADIUS;	// ボスの当たり判定の大きさ
	static const int DRAW_HP_DISTANCE = 3000;	// HPゲージを表示する距離
	static const int DRAW_DISTANCE = 7000;		// 敵を表示する距離
	static const int MOB_BULLET_POWER = 200;	// 敵キャラの弾の攻撃力
	static const int MOB_BULLET_LIFE = 120;		// 敵キャラの弾の寿命
	static const float MOB_BULLET_SPEED;		// 敵キャラの弾の速度
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
		MOTION_MAX,
	};

	CMob();
	virtual ~CMob() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Destroy() override;			// 破壊処理
	void DrawLifeGauge();		// 体力ゲージの表示
	void Move() override;
	void Attack();
	void Avoidance();
	void Clear();

	static CMob* Create(const D3DXVECTOR3 pos);

private:
	float m_fDistance;

	int m_nBullet_Interval;	// 弾を撃つ間隔
	bool m_bAvoidance;		// 回避

	float m_fAvoidance_Speed;		// 回避速度
	int m_fAvoidance_Count;	// 回避するタイミング
	bool m_bfAvo_Step;
	float m_fStep;
	float m_nClear_Counter;	// 透明になる秒数
};

#endif// _MOB_H_