//=============================================================================
//
// pteranodon.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PTERANODON_H_
#define _PTERANODON_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "enemy.h"

//---------------------------
// クラス宣言
//---------------------------
class CPteranodon : public CEnemy
{
public:
	// モデルの配置情報
	enum Ptera_Model
	{
		PTERA_MODEL_BODY = 0,	// [0]体
		PTERA_MODEL_HEAD,		// [1]頭
		PTERA_MODEL_ARM_LEFT,	// [2]左腕
		PTERA_MODEL_ARM_RIGHT,	// [3]右腕
		PTERA_MODEL_LEG,		// [4]足
		PTERA_MODEL_MAX,
	};							
	// モーションのタイプ
	enum Ptera_Motion_type
	{
		PTERA_MOTION_NEUTRAL = 0,	// ニュートラル
		PTERA_MOTION_WALK,			// 歩き
		//PTERA_MOTION_ATTACK,		// 攻撃
		PTERA_MOTION_MAX,
	};

	// 徘徊情報
	enum Ptera_Roaming_Type
	{
		ROAMING_NONE = 0,	// 徘徊していない
		ROAMING_BOTTOM,		// 下方向
		ROAMING_LEFT,		// 左方向
		ROAMING_UP,			// 上方向
		ROAMING_RIGHT,		// 右方向
		ROAMING_MAX,
	};

	CPteranodon();
	~CPteranodon() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void ChangeMotion() override;		// モーションの切り替え

	void AI();			// AI処理
	void Search();		// 探索処理
	void Roaming();		// 徘徊処理
	void CollEnemy();

	static CPteranodon* Create(const D3DXVECTOR3 pos, const float alpha = 1.0f);
private:
	static const int Ghost_KnockBack_Power = 10;	// ゴーストのノックバックさせる力

	static const float Ptera_Roaming_Speed;			// ゴーストの徘徊速度
	static const float Ptera_Search_Speed;			// ゴーストの探索速度
	static const float Ptera_Roaming_Clear;			// ゴーストの徘徊透明度
	static const float Ptera_Radius_Collision;		// 当たり判定のサイズ(半径)
	static const float Ptera_Radius_Search;			// 検索のサイズ(半径)
	static const float Ptera_Roaming_Length;		// 徘徊する距離
	static const float Ptera_Clear_Speed;			// 透明になる速度

	D3DXVECTOR3 m_anchorPos;						// 位置の軸
	Ptera_Roaming_Type m_Roaming;					// 徘徊情報
	bool m_bClockwise;								// 時計回りかどうか
	float m_fAlpha;									// アルファ値
};

#endif// _PTERANODON_H_