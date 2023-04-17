//=============================================================================
//
// アイテム
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object2D.h"

//---------------------------
//クラス
//---------------------------
class CItem : public CObject2D
{
public:
	enum Item_Type
	{
		ENERGY_NONE = 0,	//エネルギー無しの状態
		ENERGY_01,			//エネルギー1
		ENERGY_02,			//エネルギー2
		ENERGY_03,			//エネルギー3
		ENERGY_04,			//エネルギー4
		ENERGY_MAX,			//アイテム最大値
	};

	CItem();
	~CItem() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void TypeSet();
	void Destroy();
	void BlinkColor();
	void Collision(D3DXVECTOR3 pos, float fLength);
	//bool CollisionCircle(D3DXVECTOR3 pos, float fLength, D3DXVECTOR3 posSub, float fLengthSub);

	static CItem *Create(D3DXVECTOR3 pos, Item_Type settype);
private:
	D3DXVECTOR3 m_move;		//移動量
	int m_nLife;			//体力
	int m_nBlinkLoop;
	float m_fWidth;			//長さ横
	float m_fHeight;		//長さ縦
	float m_fBlinkCounter;
	bool m_bBlink;
	Item_Type m_type;
};

#endif // !_ITEM_H_
