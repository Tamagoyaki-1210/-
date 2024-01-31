//=============================================================================
//
// pc.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _PC_H_
#define _PC_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "player.h"

class CEnergy_Gauge;

//---------------------------
// クラス宣言
//---------------------------
class CPC : public CPlayer
{
public:

	CPC();
	virtual ~CPC() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Input();		// 入力処理
	void Destroy() override;			// 破壊処理
	void Perspective();	// 視点移動

	static CPC* Create(const D3DXVECTOR3 pos, const int index);

private:
	D3DXVECTOR3 m_rotDest;					//目的の向き
	int m_nWalk_Counter;					// 歩き時間
};

#endif// _PC_H_