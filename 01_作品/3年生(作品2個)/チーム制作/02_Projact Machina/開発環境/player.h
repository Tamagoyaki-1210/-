//=============================================================================
//
// player.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "character.h"

class CBullet;
class CEnergy_Gauge;
class CObject3D;
class CDrop_Weapon;
class CWeapon;
class CEnemy;

//---------------------------
// クラス宣言
//---------------------------
class CPlayer : public CCharacter
{

	static const int FIRST_MAX_LIFE = 100;			// 初期最大体力の値
	static const float PLAYER_COLLISION_RADIUS;		// プレイヤーの当たり判定の大きさ
	static const float PLAYER_JUMP_POWER;			// プレイヤーのジャンプ力
	static const float VIEW_SCOPE_ANGLE;			// 画面に映るぎりぎりの位置
	static const float RETICLE_TRANSPARENCY_SIZE;	// レティクル透明時のサイズ
	static const float RETICLE_SIZE;				// レティクルのサイズ
	static const float BULLET_SPEED_SCALE;			// 弾速の倍率

public:

	// パーツの種類
	enum PARTS
	{
		PARTS_BODY = 0,			// 胴体
		PARTS_ARMS,				// 腕
		PARTS_LEG,				// 脚

		PARTS_MAX,
		PARTS_WEAPON,			// 武器
	};

	// モーションのタイプ
	enum MOTION_TYPE
	{
		MOTION_NEUTRAL = 0,			// ニュートラル
		MOTION_WALK,				// 歩き
		MOTION_JUMP,				// ジャンプ
		MOTION_LANDING,				// 着地
		MOTION_BOOST_RUN,			// ブーストダッシュ

		MOTION_ASSAULT_RIFLE,		// アサルトライフル射撃
		MOTION_SUB_MACHIN_GUN,		// サブマシンガン射撃
		MOTION_HUND_GUN,			// ハンドガン(二丁拳銃)
		MOTION_MACHIN_GUN,			// マシンガン射撃
		MOTION_SHOT_GUN,			// ショットガン射撃
		MOTION_SNIPER_RIFLE,		// スナイパーライフル射撃

		MOTION_FIST_1,				// 拳モーション1
		MOTION_FIST_2,				// 拳モーション2
		MOTION_FIST_3,				// 拳モーション3
		MOTION_FIST_4,				// 拳モーション4
		MOTION_FIST_5,				// 拳モーション5
		//MOTION_FIST_6,			// 拳モーション6

		MOTION_SLASH_1,				// 斬撃モーション1
		MOTION_SLASH_2,				// 斬撃モーション2
		MOTION_SLASH_3,				// 斬撃モーション3
		MOTION_SLASH_4,				// 斬撃モーション4
		MOTION_SLASH_5,				// 斬撃モーション5
		MOTION_SLASH_6,				// 斬撃モーション6

		MOTION_POKE_1,				// 刺突モーション1
		MOTION_POKE_2,				// 刺突モーション2
		MOTION_POKE_3,				// 刺突モーション3
		MOTION_HEAVY_POKE_1,		// 重刺突モーション1
		MOTION_HEAVY_POKE_2,		// 重刺突モーション2
		MOTION_HEAVY_POKE_3,		// 重刺突モーション3

		MOTION_MAX,
	};

	// ジョブの種種類
	enum JOB
	{
		JOB_STANDART = 0,	// スタンダード
		JOB_RUSH ,			// ラッシュ
		JOB_VANGUARD,		// ヴァンガード
		JOB_EAGLEEYE,		// イーグルアイ
		JOB_MARSHALL,		// マーシャル
		JOB_MAX
	};

	CPlayer();
	virtual ~CPlayer() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void ChangeMotion();		// モーションの切り替え
	void Hit(CMove_Object* pHit) override;
	virtual void Destroy() override;			// 破壊処理

	void PlayerAttack();				// プレイヤーの攻撃処理
	void MeleeWeaponAttack();			// 近接武器の攻撃処理
	void GunWeaponAttack();				// 銃武器の攻撃処理
	void BulletAttack(const int weapon);				// 弾攻撃処理
	void AttackInit();					// 攻撃終了処理
	void JumpStart();					// プレイヤーのジャンプ処理
	void JumpBoost();					// プレイヤーのジャンプブースト処理
	void Landing(const D3DXVECTOR3 pos) override;	// 着地処理
	void Target();						// ターゲット
	bool Target_Scope(D3DXVECTOR3 nearpos);				// ターゲットを狙う範囲
	void Reticle(D3DXVECTOR3 target);	// レティクル
	void DropGet(CDrop_Weapon* pDrop);		// 落とし物を入手する処理

	void CollisionDropWeapon();			// 落ちてる武器の当たり判定

	void SettingParameter();			// パラメーターの設定処理

	void SetCharaIndex(const int index) { m_nCharaIndex = index; }
	void SetEnergyGauge(CEnergy_Gauge *pEnergy) { m_pEnergy_Gauge = pEnergy; }
	void SetDropGet(const bool drop_get) { m_bDrop_Get = drop_get; };
	void SetJobIndex(const int parts, const int index) { m_Parts_Job[parts] = (JOB)index; }
	void SetPlayerParts(const PARTS parts, const int weapon, const int rarity);
	void SetPlayerWeapon(const int weapon, const int rarity);
	void SetPlayerAttack(const bool attack) { m_bPlayer_Attack = attack; }
	void SetAdditionalAttack(const bool attack) { m_bAdditional_Attack = attack; }


	const bool GetDropContact() { return m_bDrop_Contact; }
	const int GetCharaIndex() { return m_nCharaIndex; }
	CEnergy_Gauge* GetEnergy_Gauge() { return m_pEnergy_Gauge; }
	const bool GetPlayerAttack() { return m_bPlayer_Attack; }
	const bool GetStandbyAttack() { return m_bStandby_Attack; }
	const int GetWeaponType() { return m_nWeapon_type; }
	const JOB GetJobIndex(const int index) { return m_Parts_Job[index]; }

private:
	int m_nCharaIndex;			// 自身の番号
	float m_fTarget_Scope;		// ターゲットを狙う範囲
	bool m_bTarget;				// ターゲットがいるか
	bool m_bDrop_Contact;		// 落とし物との接触判定
	bool m_bDrop_Get;			// 落とし物を入手する判定
	bool m_bChange_Parameter;	// パラメーターの変動判定

	CEnergy_Gauge* m_pEnergy_Gauge;		// エネルギーゲージ

	D3DXVECTOR3 m_NearMob_Pos;
	CObject3D *m_pReticle;				// レティクル
	D3DXVECTOR3 m_Reticle_Pos;
	D3DXVECTOR2 m_Reticle_Size;
	D3DXVECTOR2 m_Reticle_Tran_Size;
	CEnemy *m_pEnemy;
	float m_fReticle_Alpha;
	float m_fHypotenuse;
	bool m_bReticle_Draw;
	bool m_bReticle_Reset;
	int m_nEnemy_Count;
	float m_fEnemy_Speed;
	float m_fAngle;
	int m_nStan_Tolerance;				// スタン許容値
	int m_nGravity;						// 重量
	bool m_bPlayer_Attack;				// プレイヤー判定の攻撃判定
	bool m_bStandby_Attack;				// 追加攻撃の待機判定
	bool m_bAdditional_Attack;			// 追加攻撃の判定
	int m_nAdditional_Attack;			// 追加攻撃の回数
	float m_fAttackRate_Counter;		// 攻撃間隔の経過時間
	float m_fAttackRate_Max_Counter;	// 攻撃間隔の最大時間

	JOB m_Parts_Job[PARTS_MAX];			// プレイヤーのパーツ毎のジョブ種類
	int m_nArms_Rarity;					// 腕のレアリティ
	int m_nLeg_Rarity;					// 脚のレアリティ

	int m_nWeapon_type;					// 武器の種類
	int m_nWeapon_Rarity;				// 武器のレアリティ
	CWeapon* m_pRightWeapon;			// 右手武器
	CWeapon* m_pLeftWeapon;				// 左手武器

};

#endif// _PLAYER_H_