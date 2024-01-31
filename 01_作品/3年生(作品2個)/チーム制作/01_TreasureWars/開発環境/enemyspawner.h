//=============================================================================
//
// enemyspawner.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ENEMYSPAWNER_H_
#define _ENEMYSPAWNER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "objectX.h"

//---------------------------
// クラス宣言
//---------------------------
class CEnemySpawner : public CObjectX
{
	static const int Enemy_Spawn_Timer = 180;		// 敵キャラの生成までの時間
	static const float First_Pos;					// スポナーの初期位置

public:

	// 敵キャラスポナーの種類
	enum EnemySpawn_Type
	{
		EnemySpawn_1 = 0,
		EnemySpawn_2,
		EnemySpawn_3,
		EnemySpawn_MAX,
	};

	CEnemySpawner();
	virtual ~CEnemySpawner() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Appearance();		// 出現までの処理
	void SpawnCount();		// 生成までの時間を計る処理
	void EnemySpawn();		// 敵キャラを出現させる処理
	void Damage();			// 被弾処理

	static CEnemySpawner* Create(const D3DXVECTOR3 pos, const int type);

	const bool GetAppearance() { return m_bAppearance; }
	const float GetRadius() { return m_fRadius; }

private:

	bool m_bAppearance;			// 出現が完了したかどうか
	int m_nSpawn_Counter;		// 生成時間のカウンター
	int m_nMaxSpawn;			// 一度に生成する最大数
	int m_nLife;				// 体力
	float m_fRadius;			// 当たり判定の大きさ

	EnemySpawn_Type m_type;	// 敵キャラスポナーの種類
	
};

#endif// _ENEMYSPAWNER_H_