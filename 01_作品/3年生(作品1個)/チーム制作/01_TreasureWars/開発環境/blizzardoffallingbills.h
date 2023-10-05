//==============================================================================================
//
// 札吹雪　　　blizzardoffallingbills.h
// Author :　koduna hirohito
//
//==============================================================================================
#ifndef _BLIZZARD_OF_FALLING_BILLS_H_
#define	_BLIZZARD_OF_FALLING_BILLS_H_

#include"object2D.h"


class CBlizzardOfFallingBills : public CObject2D
{
	static const int EFFECT_PATTERN_MAX = 4;		//札吹雪のパターン数
	static const float WIDTH_PER_PATTERN_ONE;		//パターン一個当たりの幅
	static const float BASIC_ROT_MOVE;				//Rotの基本移動量
	static const float BASIC_POS_MOVE;				//Posの基本移動量
	static const float RANGE_WITH_PARENT;			//仮想親との距離
	static const float RIGHT_LEFT_WIDTH;			//右左の幅

public:
	CBlizzardOfFallingBills(int nPriority);
	~CBlizzardOfFallingBills() override;

	// 定義無しの描画の初期設定 = 3
	static CBlizzardOfFallingBills *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority = 3);

	HRESULT Init() override;
	void Update() override;

	void SetParentPos(D3DXVECTOR3 pos) { m_ParentPos = pos; }
private:

	float m_fParentRot;		//仮想親のRot
	D3DXVECTOR3 m_ParentPos;		//仮想親のPos
	float m_fParentRotMove;	//仮想親のRotの移動量
	D3DXVECTOR3 m_ParentPosMove;	//仮想親のPosの移動量
	float m_fRangeWithParent;		//仮想親との距離
	bool m_bRightLeft;				//右左
};

#endif // !_BLIZZARD_OF_FALLING_BILLS_H_
