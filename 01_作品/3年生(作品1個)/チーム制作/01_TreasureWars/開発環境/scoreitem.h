//=============================================================================
//
// scoreitem.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _SCOREITEM_H_
#define _SCOREITEM_H_

#include "objectX.h"
#include <vector>

class CShadow;

//---------------------------
// クラス宣言
//---------------------------
class CScoreItem : public CObjectX
{
public:
	static const char* STAGE_SCOREITEM_01[];
	static const char* STAGE_SCOREITEM_02[];
	static const char* STAGE_SCOREITEM_03[];

	static const int MAX_SCOREITEM_MODEL = 5;	// スコアアイテムのモデル最大数
	static const float ScoreItem_Radius;		// スコアアイテムの当たり判定のサイズ

	CScoreItem();
	~CScoreItem() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CScoreItem* RandomCreate(const D3DXVECTOR3 pos);
	static CScoreItem* Create(const D3DXVECTOR3 pos, const int type);

	void Destroy();
	void Move();
	void CharColl();
	void WallColl();
	void Blink();
	void Bounce();
	void Floating();

	void SetShadow(const D3DXVECTOR3 pos);		// 影の生成処理

	const int GetNum() { return m_nNumMaxScoreItem; }
	const bool GetGround() { return m_bGround; }
	const int GetType() { return m_nType; }

private:
	static const int Floating_Timer = 60;			// 浮遊時間
	static const int Blink_Timer = 180;				// 点滅までの時間
	static const int Max_Destroy_Timer = 360;		// 消えるまでの時間
	static const int ScoreItem_Blink_interval = 10;	// 宝石の点滅間隔
	static const int m_nNumMaxScoreItem = 5;		// スコアアイテムの最大個数

	static const float ScoreItem_Gravity;			// スコアアイテムの重力
	static const float ScoreItem_Inertia;			// スコアアイテムの慣性
	static const float ScoreItem_Launch;			// スコアアイテムが縦に打ち上がる力
	static const float ScoreItem_Blink_Speed;		// スコアアイテムの点滅速度

	int m_nType;									// スコアアイテムの種類
	int m_nFloatingCounter;							// 浮遊時間
	int m_nDestroyCounter;							// 消えるまでの時間
	bool m_bGround;									// 着地判定

	D3DXVECTOR3 m_move;								// 移動量
	CShadow* m_pShadow;								// 簡単な影のポインタ
};

#endif// _SCOREITEM_H_