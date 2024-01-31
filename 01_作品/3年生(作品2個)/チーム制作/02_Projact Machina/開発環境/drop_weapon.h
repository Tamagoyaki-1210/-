//=============================================================================
//
// drop_weapon.h
// Author : tutida ryousei
//			tanimoto kosuke
//
//=============================================================================
#ifndef _DORP_WAPON_H_
#define _DORP_WAPON_H_

#include "move_object.h"
#include "player.h"

class CObject3D;

class CDrop_Weapon : public CMove_Object
{
	static const float PARTS_FLOTIONG_POS;			// 落ちてる武器の浮遊位置
public:
	
	static const float PARTS_COLLISION_RADIUS;		// 当たり判定の大きさ

	CDrop_Weapon(const PRIORITY priority = PRIORITY_MODEL);
	~CDrop_Weapon();

	// パーツの種類
	enum Drop_Parts_FileName
	{
		// [1]腕
		ARMS_SG01 = 0,			// SG01の腕
		ARMS_SG02,				// SG02の腕
		ARMS_SG03,				// SG03の腕
		ARMS_SG04,				// SG04の腕
		ARMS_MAX,				// 腕の最大数

		// [2]脚
		LEG_SG01,				// SG01の脚
		LEG_SG02,				// SG02の脚
		LEG_SG03,				// SG03の脚
		LEG_SG04,				// SG04の脚
		LEG_MAX,				// 脚の最大数

		GUN_WEAPON_AR_AR40,					// AR40
		GUN_WEAPON_AR_TEC_A3,				// Tec-A3
		GUN_WEAPON_AR_DUMPMAN,				// DumpMan
		GUN_WEAPON_AR_STARVERSE_AR,			// StarVerseAR
		GUN_WEAPON_AR_MM400JINX,			// MM400ジンクス

		GUN_WEAPON_SMG_MPC50,				// MPC50
		GUN_WEAPON_SMG_PDW_920,				// PDW-920
		GUN_WEAPON_SMG_PEARL_SMG,			// パールSMG
		GUN_WEAPON_SMG_SILVERCOMET,			// SilverComet

		GUN_WEAPON_HG_HG37,					// HG37
		GUN_WEAPON_HG_REVOLVER,				// Revolver
		GUN_WEAPON_HG_AKIMBO20,				// アキンボ2.0

		GUN_WEAPON_MG_LA2000,				// LA2000
		GUN_WEAPON_MG_METALLICA255,			// メタリカ255

		GUN_WEAPON_SG_12PUMP,				// 12Pump
		GUN_WEAPON_SG_WR2154,				// WR2154

		GUN_WEAPON_SR_WINTER5000,			// Winter5000
		GUN_WEAPON_SR_ATHENA,				// アテナ

		GUN_WEAPON_MAX,						// 銃武器の最大数

		// 武器
		MELEE_WEAPON_NONE,					// 素手

		MELEE_WEAPON_FIST_KNUCKLE,			// ナックル
		MELEE_WEAPON_FIST_BUNKER,			// パイルバンカー
		MELEE_WEAPON_FIST_BRAND_MARKERS,	// ブランドマーカー
		MELEE_WEAPON_FIST_STUN_BATON,		// スタンバトン

		MELEE_WEAPON_SLASH_SAMURAI_SWORD,	// 太刀
		MELEE_WEAPON_SLASH_HAMMER,			// ハンマー
		MELEE_WEAPON_SLASH_BEAM_SABER,		// ビームサーベル
		MELEE_WEAPON_SLASH_BASTARD_SWORD,	// バスターソード

		MELEE_WEAPON_STABBING_LANCE,		// ランス
		MELEE_WEAPON_STABBING_RAPIER,		// レイピア
		MELEE_WEAPON_STABBING_PARTISAN,		// パルチザン
		MELEE_WEAPON_STABBING_BEAM_JAVELIN,	// ビームジャベリン

		MELEE_WEAPON_MAX,					// 近接武器の最大数

		DROP_PARTS_MAX,
		DROP_PARTS_NONE
	};

	static const char* s_Weapon_FileName[];	// ファイルパス

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void Hit(CMove_Object* pHit) override;

	void ItemPointerMove();// アイテムポインターの移動処理
	void Pick_Up_Weapon();		// 武器を拾う
	void Parts_Type();			// パーツの部位の設定
	void FieldCollision();		// 床との当たり判定

	void SetPick_Up(const bool pick_Up) { m_bPick_Up = pick_Up; }

	const int GetRarity() { return m_nRarity; }
	const D3DXVECTOR3 GetCentePos() { return m_CenterPos; }
	const int GetWeaponType() { return m_nWeapon_Type; }
	CPlayer::PARTS GetPartsType() { return m_Parts; }

	static CDrop_Weapon *Create(D3DXVECTOR3 pos, int weapon, int rarity);


	static void LoadAllFile();			// 全てのドロップパーツの読み込み

private:

	D3DXVECTOR3 m_CenterPos;	// 中心位置
	int m_nWeapon_Type;			// 武器のタイプ
	int m_nRarity;				// レアリティ
	CObject3D *m_pPointer;		// アイテムポインターの表示
	CObject3D *m_pPick_Up;		// 拾う時の表示
	float m_fMove;				// 落下速度
	bool m_bPick_Up;			// 拾える範囲にいるか
	CPlayer::PARTS m_Parts;		// パーツ情報
};

#endif // !_DORP_WAPON_H_
