//=============================================================================
//
// 光線エフェクト
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _RAY_H_
#define _RAY_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//前方宣言
//---------------------------
class CTexture;

//---------------------------
//クラス
//---------------------------
class CRay : public CObject2D
{
public:
	enum Ray_Type
	{
		NONE = 0,	//未使用
		BULLET,		//弾
		MAX,		//光線の最大値
	};

	CRay();
	~CRay() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void InitType();
	void UpdateType();
	void Destroy();
	void Rand();
	void Decline();
	static CRay *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, Ray_Type type);
	void LoadRay();
private:
	D3DXVECTOR3 m_move;		//移動量
	int m_nLife;			//体力
	float m_fWidth;			//長さ横
	float m_fHeight;		//長さ縦
	float m_fSpeed;			//移動速度
	float m_fAlphaCounter;	//アルファ値カウンター
	Ray_Type m_type;
	bool m_bBlink;			//点滅判定
};

#endif // !_RAY_H_