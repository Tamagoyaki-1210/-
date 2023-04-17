//=============================================================================
//
// 弾
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _BULLET_H_
#define _BILLET_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//マクロ関数
//---------------------------
#define MAX_ENEMY_HIT			(100)	//敵キャラ同時ヒット最大数
#define MAX_OBSTACLE_HIT		(40)	//障害物同時ヒット最大数

//---------------------------
//前方宣言
//---------------------------
class CTexture;
class CEnemy;
class CObstacle;
class CRay;

//---------------------------
//クラス
//---------------------------
class CBullet: public CObject2D
{
public:
	enum Bullet_Player
	{
		PLAYER_NONE = 0,//プレイヤー弾不使用
		PLAYER_NORMAL,	//プレイヤー通常弾
		PLAYER_LASER,	//プレイヤーレーザー
		PLAYER_BOMM,	//プレイヤーボム
		PLAYER_MAX,		//プレイヤー弾最大値
	};

	enum Bullet_Enemy
	{
		ENEMY_NONE = 0,	//敵キャラ弾不使用
		ENEMY_NORMAL,	//敵キャラ通常弾
		ENEMY_LASER,	//敵キャラレーザー
		ENEMY_SPREAD,	//敵キャラ拡散弾
		ENEMY_MAX,		//敵キャラ弾最大値
	};

	enum Bullet_Collision
	{
		COLLISION_CIRCLE = 0,	//敵キャラ弾不使用
		COLLISION_SQUARE,		//敵キャラ通常弾
		COLLISION_MAX,			//敵キャラ弾最大値
	};

	CBullet();
	~CBullet() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TypeAI(D3DXVECTOR3 pos);
	void OffScreen(D3DXVECTOR3 pos);
	void Destroy();
	void Collision(D3DXVECTOR3 pos, float fLength);

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Bullet_Player playerBullet);
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Bullet_Enemy enemyBullet);
	void LoadPlayerBullet();
	void LoadEnemyBullet();
private:
	D3DXVECTOR3 m_poslast;		//前回位置
	D3DXVECTOR3 m_move;			//移動量
	int m_nLife;				//体力
	int m_nPower;				//威力
	int m_nPenetrat;			//貫通回数
	float m_fMaxWidth;			//最大長さ横
	float m_fWidth;				//長さ横
	float m_fHeight;			//長さ縦
	float m_fSpeed;				//移動速度
	bool m_bHitDestroy;			//弾貫通判定
	Bullet_Player m_playerBullet;
	Bullet_Enemy m_enemyBullet;
	Bullet_Collision m_collisionBullet;
	CEnemy *m_pEnemy[MAX_ENEMY_HIT];
	CObstacle *m_pObstacle[MAX_OBSTACLE_HIT];
};

#endif // !_BILLET_H_
