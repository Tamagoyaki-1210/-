//=============================================================================
//
// ライフ
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _LIFE_H_
#define _LIFE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//マクロ関数
//---------------------------
#define LIFE_WIDTH	(250)
#define LIFE_HEIGHT	(50)

//---------------------------
//前方宣言
//---------------------------
class CGauge;

//---------------------------
//クラス
//---------------------------
class CLife : public CObject2D
{
public:
	CLife();
	~CLife() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetLife(int life, int maxlife, int fastmaxlife);
	static CLife *Create(D3DXVECTOR3 pos);
private:
	int m_nLife;			//体力
	float m_fWidth;			//長さ横
	float m_fHeight;		//長さ縦
	CGauge *m_pGauge;		//ゲージポインタ
};

#endif // !_LIFE_H_

