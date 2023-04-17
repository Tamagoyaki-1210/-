//=============================================================================
//
// bouncePole.h
// Author : KAJITA HIROMU
//
//=============================================================================
#ifndef _BOUNCEPOLE_H
#define _BOUNCEPOLE_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"

//=============================================================================
//前方宣言
//=============================================================================
class CCylinderHitbox;

class CBouncePole : public CModel
{
public:

	CBouncePole();											//コンストラクタ
	~CBouncePole() override;									//デストラクタ

	HRESULT Init(void) override;							//初期化処理
	void Uninit(void) override;								//終了処理
	void Update(void) override;								//更新処理
	void Draw(void) override;								//描画処理

	static CBouncePole* Create(const D3DXVECTOR3 pos);		//生成処理

private:

	CCylinderHitbox* m_pHitbox;
	D3DXVECTOR3 m_move;
	float m_Height;
};


#endif
#pragma once

