//==============================================================================================
//
// CPU　　　cpu.h
// tutida ryousei
//
//==============================================================================================
#ifndef _CPU_H_
#define _CPU_H_

#include"character.h"

class CCpu : public CCharacter
{
public:
	CCpu();
	~CCpu() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CCpu *Create(D3DXVECTOR3 pos, int index);

	void Move() override;											// 移動処理
	void Action();													// アクション
	void Ai();														// AI処理
	void Search(const int search);									// 探索処理
	void CharaSearch(const D3DXVECTOR3 pos, const float radius);	// キャラの探索処理
	void OreSearch(const D3DXVECTOR3 pos, const float radius);		// 鉱石の探索処理
	void ScoreItemSearch(const D3DXVECTOR3 pos);					// スコアアイテムの探索処理
	void ItemSearch(const D3DXVECTOR3 pos);							// アイテムの探索処理
	void EnemySearch(const D3DXVECTOR3 pos, const float radius);	// 敵キャラの探索処理
	void ShopSearch(const D3DXVECTOR3 pos, const float radius);		// 質屋の探索処理

private:
	static const int First_Mine_Power = 10;			// 採掘攻撃力の初期値

	static const float Radius_Search_Ore;			// 鉱石の検索のサイズ(半径)
	static const float Radius_Search_ScoreItem;		// スコアアイテムの検索のサイズ(半径)
	static const float Radius_Search_Chara;			// キャラの検索のサイズ(半径)
	static const float Radius_Search_Enemy;			// 敵キャラの検索のサイズ(半径)

	// 思考ルーチンの種類
	enum Trial_Routine
	{
		TRIAL_ROUTINE_NEUTRAL = 0,	// 色々考える思考
		TRIAL_ROUTINE_ATTACK,		// 相手を攻撃する思考
		TRIAL_ROUTINE_SALE,			// 宝石を売却する思考
		TRIAL_ROUTINE_MAX,
		TRIAL_ROUTINE_NONE
	};

	// 検索の種類
	enum Search
	{
		SEARCH_NONE = 0x00,			// 検索しない
		SEARCH_CHARA = 0x01,		// キャラの探索
		SEARCH_ORE = 0x02,			// 鉱石の探索
		SEARCH_SCOREITEM = 0x04,	// スコアアイテムの探索
		SEARCH_ITEM = 0x08,			// アイテムの探索
		SEARCH_ENEMY = 0x10,		// 敵キャラの探索
		SEARCH_SHOP = 0x20,			// 質屋の探索
		SEARCH_MAX = 0xff,
	};

	int d;						// タイムスリップのデバッグ変数(名前を変えて欲しい)
	bool m_bSetAttack;			// 攻撃をするかどうかの判定
	D3DXVECTOR3 m_targetPos;	// 目的位置

	Trial_Routine m_trialRoutine;	// 現在の思考ルーチン
};

#endif // !_CPU_H_
