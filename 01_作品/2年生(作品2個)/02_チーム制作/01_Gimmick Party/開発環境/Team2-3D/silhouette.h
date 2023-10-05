//=============================================================================
//
// silhouette.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _SILHOUETTE_H_
#define _SILHOUETTE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object2D.h"

class CSilhouette : public CObject_2D
{
public:
	CSilhouette();							//コンストラクタ
	~CSilhouette() override;				//デストラクタ
											
	HRESULT Init(void) override;			//初期化処理
	void Uninit(void) override;				//終了処理
	void Draw(void) override;				//描画処理
											
	static CSilhouette* Create(void);		//生成処理

private:

};




#endif
#pragma once
