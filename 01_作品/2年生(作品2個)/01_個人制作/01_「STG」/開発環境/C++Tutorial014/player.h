//=============================================================================
//
// プレイヤー
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object2D.h"

//---------------------------
//前方宣言
//---------------------------
class CLife;

//---------------------------
//クラス
//---------------------------
class CPlayer : public CObject2D
{
public:
	//プレイヤー状態
	enum Player_Situation
	{
		SITU_NONE = 0,		//何もない状態
		SITU_DAMAGE,		//ダメージ状態
		SITU_DEATH,			//無敵状態
		SITU_MAX,
	};

	CPlayer();
	~CPlayer() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Hit(int nDamage);
	void AddLife(int addLife);
	void AddMaxLife(int addMaxLife);
	void AddSpeed(float nSpeed);
	void AddBullet();
	void Input(D3DXVECTOR3 pos);
	void OffScreen(D3DXVECTOR3 pos);
	void Destroy(D3DXVECTOR3 pos);
	void Collision();
	void BlinkColor();
	void SituationType();

	static CPlayer *GetPlayer() { return m_pPlayer; }		//プレイヤー取得
	static CPlayer *Create(D3DXVECTOR3 pos);
	void Load();

	Player_Situation GetSitu() { return m_situ; }
private:
	D3DXVECTOR3 m_posOld;		//前回位置
	D3DXVECTOR3 m_move;			//移動量
	int m_nLife;				//体力
	int m_nMaxLife;				//体力最大値
	int m_nFastMaxLife;			//初期体力最大値
	int m_nAnimX;				//アニメーションX座標
	int m_nAnimY;				//アニメーションY座標
	int m_nBlinkLoop;			//点滅ループ回数
	int m_nBulletCounter;		//弾生成カウンター
	int m_nHoldBullet;			//弾所有数
	int m_nDeathTimer;			//死亡時間
	float m_fWidth;				//長さ横
	float m_fHeight;			//長さ縦
	float m_fSpeed;				//速度
	float m_fBlinkCounter;		//点滅カウンター
	float m_bBlink;				//点滅判定
	Player_Situation m_situ;	//プレイヤー状態
	CLife *m_pLife;				//ライフポインタ
	static CPlayer *m_pPlayer;
};

#endif // !_PLAYER_H_
