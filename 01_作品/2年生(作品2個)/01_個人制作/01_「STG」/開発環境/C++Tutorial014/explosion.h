//=============================================================================
//
// 爆発
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//マクロ関数
//---------------------------
#define EXPLOSION_WIDTH		(80)
#define EXPLOSION_HEIGHT	(80)

//---------------------------
//前方宣言
//---------------------------
class CExplosion;

//---------------------------
//クラス
//---------------------------
class CExplosion : public CObject2D
{
public:
	CExplosion();
	~CExplosion() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CExplosion *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
private:
	D3DXVECTOR3 m_move;		//移動量
	int m_nLife;			//体力
	float m_fWidth;			//長さ横
	float m_fHeight;		//長さ縦
};

#endif // !_EXPLOSION_H_