//=============================================================================
//
// character.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "move_object.h"
#include "parts.h"
#include <vector>
#include <map>

class CGauge_Manager;

//---------------------------
// クラス宣言
//---------------------------
class CCharacter : public CMove_Object
{
	static const float CHARACTER_FIRST_MOVE_SPEED;		// キャラクターの初期移動速度

	static const float CHARACTER_ROT_SPEED;				// キャラクターの回転する速度
	static const float CHARACTER_MOVE_INERTIE;			// キャラクターの移動慣性
	static const float CHARACTER_GRAVITY;				// キャラクターの重力
	static const int CHARACTER_INVINCIBLE_TIMER = 20;	// キャラクター無敵時間
	 const int CHARACTER_INVINCIBLE_SPEED = 6;			// キャラクターの透明速度

public:

	CCharacter(const CObject::PRIORITY priority = CObject::PRIORITY_MODEL);
	virtual ~CCharacter() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Move();
	void Damage(const int value);
	void Recovery(const int value);
	virtual void Destroy();							// 自身を破壊する処理
	virtual void Landing(const D3DXVECTOR3 pos);	// 着地処理
	void FieldCollision();							// 床の当たり判定
	void Invincible();								// 透明状態
	void Rotation();								// 回転方向へ振り向かせる処理
	void NormalizeRot();							// 角度の正規化
	virtual void Hit(CMove_Object* pHit) override = 0;

	/*void SetGround(const bool ground) { m_bGround = ground; }*/
	void SetLife(const int life);
	void SetMaxLife(const int maxlife);
	void SetSpeed(const float speed) { m_fSpeed = speed; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void AddMove(const D3DXVECTOR3 move) { m_move += move; }
	void SetRotDest(const D3DXVECTOR3 rot) { m_rotDest = rot; }
	void AddRotDest(const D3DXVECTOR3 rot) { m_rotDest += rot; }
	void SetBulletRot(const D3DXVECTOR3 bulletrot) { m_BulletRot = bulletrot; }
	void SetBoost(const bool boost) { m_bBoost = boost; }
	void SetJump_Boost(const bool jumpboost) { m_bJump_Boost = jumpboost; }

	void SetParts(const int charaParts, const int partsIndex, CMotion::MOTION motion);				// パーツの設定処理

	void SetJump_PressCount(const int jumpcount) { m_nJump_PressCount = jumpcount; }
	void AddJump_PressCount(const int jumpcount) { m_nJump_PressCount += jumpcount; }
	void SetAvoidance(const bool avoidance) { m_bAvoidance = avoidance; }
	void SetAvoidanceCount(const int avoidancecount) { m_nAvoidance_Count = avoidancecount; }
	void SetHitExplosion(const bool hit) { m_bHit_Explosion = hit; }
	void SetGaugeManager(CGauge_Manager *gauge) { m_pGaugeManager = gauge; }

	/*const bool GetGround() { return m_bGround; }*/
	const int GetLife() { return m_nLife; }
	const int GetMaxLife() { return m_nMaxLife; }
	const float GetSpeed() { return m_fSpeed; }
	const D3DXVECTOR3 GetMove() { return m_move; }
	const D3DXVECTOR3 GetRot() { return m_rot; }
	const D3DXVECTOR3 GetRotDest() { return m_rotDest; }
	const D3DXVECTOR3 GetBulletRot() { return m_BulletRot; }
	const D3DXMATRIX GetWorldMtx() { return m_mtxWorld; }
	CParts* GetParts(const int index) { return m_Parts[index]; }
	std::map<int, CParts*> GetAllParts() { return m_Parts; }
	const bool GetBoost() { return m_bBoost; }
	const bool GetJump_Boost() { return m_bJump_Boost; }
	const int GetJump_PressCount() { return m_nJump_PressCount; }
	const bool GetAvoidance() { return m_bAvoidance; }
	const bool GetHitExplosion() { return m_bHit_Explosion; }
	CGauge_Manager *GetGaugeManager() { return m_pGaugeManager; }
	int GetDamage() { return m_fDamageReceived; }

private:

	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_posOld;				// 過去の位置
	D3DXVECTOR3 m_move;					// 移動量
	D3DXVECTOR3 m_rot;					// 現在の角度
	D3DXVECTOR3 m_rotDest;				// 目的の角度
	D3DXVECTOR3 m_BulletRot;			// 弾を撃つ角度

	D3DXCOLOR m_col;					// モデルの色

	//bool m_bGround;						// 接地判定

	int m_nLife;						// 体力
	int m_nMaxLife;						// 最大体力

	float m_fSpeed;						// 移動速度
	float m_fRotSpeed;					// 回転のスピード
	int m_fDamageReceived;				// 受けたダメージの総数

	bool m_bBoost;						// ブーストしているか
	bool m_bJump_Boost;					// ジャンプブーストしているか
	int m_nJump_PressCount;				// ジャンプの長押ししてる時間
	bool m_bAvoidance;					// 回避しているか
	int m_nAvoidance_Count;				// 回避の硬直時間

	bool m_bHit_Explosion;				// 爆発に当たったか

	int m_nInvincible_Counter;			// 無敵時間

	std::string m_name;					// 自身の名前
	std::map<int, CParts*> m_Parts;		// パーツ情報のポインタ

	CGauge_Manager *m_pGaugeManager;	// ゲージマネージャー
};

#endif// _CHARACTER_H_