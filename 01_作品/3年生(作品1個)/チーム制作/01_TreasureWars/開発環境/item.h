//==============================================================================================
//
// アイテム　　　	item.h
// tutida ryousei
//
//==============================================================================================
#ifndef _ITEM_H_
#define	_ITEM_H_

#include"objectX.h"
#include<vector>

class CShadow;
class CCharacter;

class CItem : public CObjectX
{
public:
	CItem();
	~CItem();

	enum Item_Type
	{
		ITEM_SPEEDUP = 0,	// スピードUP
		ITEM_KNOCKBACK,		// ノックバック強化
		ITEM_STACK,			// 一度にもてる鉱石を増やす
		ITEM_MAX,
		ITEM_NONE,
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	void CollItem();	// キャラクターとの当たり判定
	void Type(CCharacter* pChara);
	void Flash_Item();	// アイテムの点滅
	void Life();

	void SetItemPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetItemSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetItemCol(D3DXCOLOR col) { m_Col = col; }

	float GetRadius() { return m_fRadius; }

private:
	D3DXVECTOR3 m_Pos;	// 位置
	D3DXVECTOR3 m_Rot;	// 角度
	D3DXVECTOR3 m_Size;	// サイズ
	D3DXCOLOR m_Col;	// 色
	float m_fRadius;	// 当たり判定の半径
	float m_fAlpha;		// アルファ値
	bool m_bAlpha;		// 消えてるか
	int m_nLife;		// 寿命
	int m_emitTime;
	CShadow *m_pShadow;
	Item_Type m_nRand_Type;
};

#endif // !_ITEM_H_