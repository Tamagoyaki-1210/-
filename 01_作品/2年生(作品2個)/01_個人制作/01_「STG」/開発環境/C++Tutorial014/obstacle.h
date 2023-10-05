//=============================================================================
//
// 障害物
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object2D.h"

//---------------------------
//クラス
//---------------------------
class CObstacle : public CObject2D
{
public:
	enum Obstacle_Type
	{
		TYPE_01 = 0,	//障害物1
		TYPE_02,		//障害物2
		TYPE_MAX,
	};

	enum Obstacle_Flip
	{
		FLIP_CIRCLE = 0,	//円形
		FLIP_SQUARE,		//四角形
		FLIP_MAX,
	};

	//障害物状態
	enum Obstacle_Situation
	{
		SITU_NONE = 0,		//何もない状態
		SITU_DAMAGE,		//ダメージ状態
		SITU_MAX,
	};

	CObstacle();
	~CObstacle() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TypeAI();
	void Hit(int nDamage);
	void SetType();
	void OffScreen(D3DXVECTOR3 pos);
	void Destroy(D3DXVECTOR3 pos);
	void BlinkColor();
	void SituationType();

	D3DXVECTOR3 FlipCircle(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fLength);
	D3DXVECTOR3 FlipSquare(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, float fWidth, float fHeight);
	static CObstacle *Create(D3DXVECTOR3 pos, Obstacle_Type type);
	void Load();

	void SetSitu(Obstacle_Situation situ) { m_situ = situ; }

	Obstacle_Flip GetFlip() { return m_flip; }
private:
	int m_nLife;			//体力
	int m_animX;			//アニメーションX座標
	int m_animY;			//アニメーションY座標
	int m_nBlinkLoop;		//点滅ループ回数
	D3DXVECTOR3 m_posOld;	//前回位置
	D3DXVECTOR3 m_move;		//移動量
	float m_fWidth;			//長さ横
	float m_fHeight;		//長さ縦
	float m_fBlinkCounter;	//点滅カウンター
	float m_bBlink;			//点滅判定
	Obstacle_Type m_type;	//障害物の種別
	Obstacle_Flip m_flip;	//弾き判定の種別
	Obstacle_Situation m_situ;	//敵キャラ状態
};

#endif // !_OBSTACLE_H_
