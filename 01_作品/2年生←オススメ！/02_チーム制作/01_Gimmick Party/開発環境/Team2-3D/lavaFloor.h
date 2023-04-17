//=============================================================================
//
// model.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _LAVA_FLOOR_H
#define _LAVA_FLOOR_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"

//=============================================================================
//前方宣言
//=============================================================================
class CBoxHitbox;

class CLavaFloor : public CModel

{
public:
	CLavaFloor();										//コンストラクタ
	~CLavaFloor();										//デストラクタ

	HRESULT Init(void) override;						//初期化処理
	void Uninit(void) override;							//終了処理
	void Update(void) override;							//更新処理
	void Draw(void) override;							//描画処理
														
	static CLavaFloor* Create(const D3DXVECTOR3 pos);	//生成処理

private:

	CBoxHitbox* m_pHitbox;								//ヒットボックス
};





#endif