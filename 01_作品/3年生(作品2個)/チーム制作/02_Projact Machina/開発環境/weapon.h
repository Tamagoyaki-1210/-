//=============================================================================
//
// weapon.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _WEAPON_H_
#define _WEAPON_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "objectX.h"

//---------------------------
// クラス宣言
//---------------------------
class CWeapon : public CObjectX
{
public:

	enum WEAPON_TYPE
	{
		GUN_WEAPON_AR_AR40 = 0,				// AR40
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

		WEAPON_KNUCKLE,						// 素手

		MELEE_WEAPON_FIST_KNUCKLE,			// ナックル
		MELEE_WEAPON_FIST_BUNKER,			// パイルバンカー
		MELEE_WEAPON_FIST_BRAND_MARKERS,	// ブランドマーカー
		MELEE_WEAPON_FIST_STUN_BATON,		// スタンバトン

		MELEE_WEAPON_SLASH_SAMURAI_SWORD,	// 太刀
		MELEE_WEAPON_SLASH_HAMMER,			// ハンマー
		MELEE_WEAPON_SLASH_BEAM_SABER,		// ビームサーベル
		MELEE_WEAPON_SLASH_BASTARD_SWORD,	// バスターソード

		MELEE_WEAPON_POKE_LANCE,			// ランス
		MELEE_WEAPON_POKE_RAPIER,			// レイピア
		MELEE_WEAPON_POKE_PARTISAN,			// パルチザン
		MELEE_WEAPON_POKE_BEAM_JAVELIN,		// ビームジャベリン

		WEAPON_MAX							// 最大値
	};


	CWeapon(const CObject::PRIORITY priority = CObject::PRIORITY_MODEL);
	virtual ~CWeapon() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ChangeWeapon(const int type);

	static CWeapon* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int type, CObjectX* parent = nullptr);

private:
	static const char* m_cWeaponFileName[WEAPON_MAX];		// 武器のファイル名
};

#endif// _WEAPON_H_