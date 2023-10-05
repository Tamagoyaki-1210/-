//=============================================================================
//
// 順位
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RANK_H_
#define _RANK_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//マクロ関数
//---------------------------
#define COMMA_WIDTH		(40)
#define COMMA_HEIGHT	(80)

//---------------------------
//クラス
//---------------------------
class CRank : public CObject2D
{
public:
	CRank();
	~CRank() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CRank *Create(D3DXVECTOR3 pos, int nID);
private:
	float m_fWidth;		//長さ横
	float m_fHeight;	//長さ縦
};

#endif // !_RANK_H_
