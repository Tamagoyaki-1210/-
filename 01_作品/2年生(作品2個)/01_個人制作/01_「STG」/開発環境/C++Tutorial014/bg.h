//=============================================================================
//
// 背景
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object2D.h"

//---------------------------
//マクロ関数
//---------------------------
#define BG_WIDTH	(SCREEN_WIDTH)
#define BG_HEIGHT	(SCREEN_HEIGHT)

//---------------------------
//クラス
//---------------------------
class CBG : public CObject2D
{
public:
	enum BG_Type
	{
		TYPE_BG01 = 0,		//背景
		TYPE_Tutorial01,	//チュートリアル01
		TYPE_Tutorial02,	//チュートリアル02
		TYPE_Tutorial03,	//チュートリアル03
		TYPE_Clear,			//クリア背景
		TYPE_MAX,			//最大値
	};

	CBG();
	~CBG() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void UpdateType();
	static CBG *Create(D3DXVECTOR3 pos, BG_Type bgType);
private:
	int m_nDestroyCounter;
	D3DXVECTOR3 m_move;	//移動量
	float m_fWidth;		//長さ横
	float m_fHeight;	//長さ縦
	BG_Type m_bgType;	//背景種類
};

#endif // !_PLAYER_H_
