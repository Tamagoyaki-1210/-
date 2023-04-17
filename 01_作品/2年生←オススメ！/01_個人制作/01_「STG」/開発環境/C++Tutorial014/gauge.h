//=============================================================================
//
// ゲージ
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//マクロ関数
//---------------------------
#define GAUGE_WIDTH		(250)
#define GAUGE_HEIGHT	(50)

//---------------------------
//クラス
//---------------------------
class CGauge : public CObject2D
{
public:
	CGauge();
	~CGauge() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetGauge(int life, int maxlife, int fastmaxlife);
	static CGauge *Create(D3DXVECTOR3 pos);
private:
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_basepos;	//元の位置
	D3DXVECTOR3 m_move;		//移動量
	float m_fWidth;			//長さ横
	float m_fHeight;		//長さ縦
};

#endif // !_LIFE_H_

