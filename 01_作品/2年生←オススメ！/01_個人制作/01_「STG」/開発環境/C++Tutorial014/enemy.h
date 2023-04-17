//=============================================================================
//
// 敵キャラ
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object2D.h"

//---------------------------
//クラス
//---------------------------
class CEnemy : public CObject2D
{
public:
	enum Enemy_Type
	{
		TYPE_01 = 0,	//敵キャラ1
		TYPE_02,		//敵キャラ2
		TYPE_03,		//敵キャラ3
		TYPE_04,		//敵キャラ4
		TYPE_MAX,
	};

	//敵キャラAI
	enum Enemy_AI
	{
		AI_NONE = 0,	//何もない状態
		AI_01,			//AIその1
		AI_02,			//AIその2
		AI_03,			//AIその3
		AI_MAX,
	};

	//敵キャラ状態
	enum Enemy_Situation
	{
		SITU_NONE = 0,		//何もない状態
		SITU_DAMAGE,		//ダメージ状態
		SITU_NONEANIM,		//アニメーションしない状態
		SITU_MAX,
	};

	CEnemy();
	~CEnemy() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TypeAI(D3DXVECTOR3 pos);
	void Hit(int nDamage);
	void SetType();
	void OffScreen(D3DXVECTOR3 pos);
	void Destroy(D3DXVECTOR3 pos);
	void BlinkColor();
	void SituationType();

	static CEnemy *Create(D3DXVECTOR3 pos, Enemy_Type type);
	void Load();

	void SetSitu(Enemy_Situation situ) { m_situ = situ; }
	Enemy_Type GetType() { return m_type; }
private:
	int m_nLife;			//体力
	int m_nAnimX;			//アニメーションX座標
	int m_nAnimY;			//アニメーションY座標
	int m_nBulletTimer;		//弾発射までの間隔
	int m_nBulletCounter;	//弾発射カウンター
	int m_nBlinkLoop;		//点滅ループ回数
	D3DXVECTOR3 m_move;		//移動量
	float m_fWidth;			//長さ横
	float m_fHeight;		//長さ縦
	float m_fSpeed;			//移動速度
	float m_fBlinkCounter;	//点滅カウンター
	float m_bBlink;			//点滅判定
	Enemy_Type m_type;		//敵キャラ種別
	Enemy_AI m_AI;			//敵キャラAI
	Enemy_Situation m_situ;	//敵キャラ状態
};

#endif // !_ENEMY_H_
