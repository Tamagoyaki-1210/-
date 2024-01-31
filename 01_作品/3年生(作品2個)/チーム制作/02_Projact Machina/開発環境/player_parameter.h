//=============================================================================
//
// player_parameter.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PLAYER_PARAMETER_H_
#define _PLAYER_PARAMETER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "d3dx9.h"
#include "player.h"

//---------------------------
// クラス宣言
//---------------------------
class CPlayer_Parameter
{

public:
	static const int PLAYER_PARTS_TYPE_MAX = 17;	// プレイヤーのパーツ種類の最大数
	static const int PARTS_RARITY = 3;				// パーツのレアリティ

	// パラメーター
	struct PARAMETERS
	{
		int nLife;					// 耐久値
		int nStamina;				// スタミナ容量
		int nStan_Tolerance;		// スタン許容値
		int nGravity;				// 重量

									//Skill m_skill;					// スキルタイプ
	};

	CPlayer_Parameter();
	~CPlayer_Parameter();

	HRESULT Init();
	void Uninit();

	void LoadJobFile();		// ジョブの読み込み
	void LoadPartsFile();	// パーツの読み込み

	// プレイヤーパラメーターの取得
	PARAMETERS GetParameterJob(const int job) { return m_Parameter_Job[job]; }

	// 腕パラメーターの取得
	PARAMETERS GetParameterArms(const int type, const int rearity) { return m_Parameter_Arms[type][rearity]; }

	// 脚パラメーターの取得
	PARAMETERS GetParameterLeg(const int type, const int rearity) { return m_Parameter_Leg[type][rearity]; }

private:

	PARAMETERS m_Parameter_Job[CPlayer::JOB_MAX];						// ジョブ数分のパラメーター
	PARAMETERS m_Parameter_Arms[PLAYER_PARTS_TYPE_MAX][PARTS_RARITY];	// 腕のパラメーター
	PARAMETERS m_Parameter_Leg[PLAYER_PARTS_TYPE_MAX][PARTS_RARITY];	// 脚のパラメーター
};

#endif// _PLAYER_PARAMETER_H_