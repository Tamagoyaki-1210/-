//=============================================================================
//
// scoreitem_stack.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _SCOREITEM_STACK_H_
#define _SCOREITEM_STACK_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "ui.h"
#include "scoreitem.h"
#include <vector>

//---------------------------
// クラス宣言
//---------------------------
//#ifdef _DEBUG
class CObject2D;
//#endif

class CReceipt;

//---------------------------
// クラス宣言
//---------------------------
class CScoreItemStack : public CUi
{
	// 宝石の色の種類
	static const D3DXCOLOR Col_Yellow;
	static const D3DXCOLOR Col_Green;
	static const D3DXCOLOR Col_Bule;
	static const D3DXCOLOR Col_Red;
	static const D3DXCOLOR Col_Gray;
	static const D3DXCOLOR Col_White;

	// 宝石の種類
	enum Gem_Type
	{
		GEM_DIAMOND = 0,	// ダイヤモンド
		GEM_RUBY,			// ルビー
		GEM_SAPPHIRE,		// サファイア
		GEM_EMERALD,		// エメラルド
		GEM_TOPAZ,			// トーパズ
		GEM_MAX
	};

	// 化石の種類
	enum Fossil_Type
	{
		FOSSIL_HEAD = 0,		// あたま
		FOSSIL_AMBER,		// コハク
		FOSSIL_BONE,		// ホネ
		FOSSIL_FOOTPRINS,	// あしあと
		FOSSIL_AMMONITE,	// アンモナイト
		FOSSIL_MAX
	};

	// デブリの種類
	enum Debris_Type
	{
		DEBRIS_CPUCHIP = 0,	// CPUチップ
		DEBRIS_PANEL,		// パネル
		DEBRIS_ANTENNA,		// アンテナ
		DEBRIS_ENGINE,		// エンジン
		DEBRIS_OXYGENBONB,	// 酸素ボンベ
		DEBRIS_MAX
	};

public:
	// スコアアイテムの情報
	typedef struct
	{
		int nType;			// スコアアイテムの種類
		std::string name;			// 名前
		D3DXCOLOR col;		// 色
		int value;			// 値段
		int getTime;		// いつ獲得されたか(タイマーの数値)
	}ScoreItem;

	CScoreItemStack();
	~CScoreItemStack() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void LostScoreItem(const int lost, const D3DXVECTOR3 pos);	// スコアアイテムの紛失処理
	int SaleScoreItem();										// スコアアイテムの売却処理
	void SetScoreItem(const int type);							// スコアアイテムの種類の設定
	void SetDefault(const int num);							// プレイヤーから最大所持数を獲得

	int GetScoreItemSize() { return (int)m_listScoreItem.size(); }	// スコアアイテムの所持数

	static CScoreItemStack* Create(const D3DXVECTOR3 pos, const int index);

private:
	static const int Max_Receipt_NameString = 6;	// レシートの名前文字数
	static const float ScoreItem_Stack_Size;		// スコアアイテムの画像のサイズ

	D3DXVECTOR3 m_pos;								// 位置

	CReceipt *m_pReceipt;							// 領収書の情報
	std::vector <ScoreItem> m_listScoreItem;		// スコアアイテムの情報
	CObject2D* m_apScoreItem[20];					// スコアアイテムの画像
};

#endif// _SCOREITEM_STACK_H_