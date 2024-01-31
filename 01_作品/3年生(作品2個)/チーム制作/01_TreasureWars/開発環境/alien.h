//=============================================================================
//
// alien.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ALIEN_H_
#define _ALIEN_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "enemy.h"

//---------------------------
// クラス宣言
//---------------------------
class CAlien : public CEnemy
{
public:

	// モデルの配置情報
	enum Alien_Model
	{
		ALIEN_MODEL_UFO = 0,	// [0]UFO
		ALIEN_MODEL_ALIEN,		// [1]エイリアン
		ALIEN_MODEL_MAX,
	};

	// モーションのタイプ
	enum Alien_Motion_type
	{
		ALIEN_MOTION_NEUTRAL = 0,	// ニュートラル
		ALIEN_MOTION_WALK,			// 歩き
		ALIEN_MOTION_MAX,
	};

	// 徘徊情報
	enum ALIEN_Roaming_Type
	{
		ROAMING_NONE = 0,	// 徘徊していない
		ROAMING_BOTTOM,		// 下方向
		ROAMING_LEFT,		// 左方向
		ROAMING_UP,			// 上方向
		ROAMING_RIGHT,		// 右方向
		ROAMING_MAX,
	};

	CAlien();
	~CAlien() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ChangeMotion() override;		// モーションの切り替え

	void AI();			// AI処理
	void Search();		// 探索処理
	void Roaming();		// 徘徊処理
	void CollEnemy();

	static CAlien* Create(const D3DXVECTOR3 pos, const float alpha = 1.0f);
private:
	static const int Alien_KnockBack_Power = 10;	// エイリアンのノックバックさせる力

	static const float Alien_Roaming_Speed;			// エイリアンの徘徊速度
	static const float Alien_Search_Speed;			// エイリアンの探索速度
	static const float Alien_Roaming_Clear;			// エイリアンの徘徊透明度
	static const float Alien_Radius_Collision;		// 当たり判定のサイズ(半径)
	static const float Alien_Radius_Search;			// 検索のサイズ(半径)
	static const float Alien_Roaming_Length;		// 徘徊する距離
	static const float Alien_Clear_Speed;			// 透明になる速度

	D3DXVECTOR3 m_anchorPos;						// 位置の軸
	ALIEN_Roaming_Type m_Roaming;					// 徘徊情報
	bool m_bClockwise;								// 時計回りかどうか
	float m_fAlpha;									// アルファ値
};

#endif// _GHOST_H_