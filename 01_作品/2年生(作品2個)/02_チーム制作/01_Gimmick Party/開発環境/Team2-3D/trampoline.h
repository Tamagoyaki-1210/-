//=============================================================================
//
// trampoline.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"

//=============================================================================
//前方宣言
//=============================================================================
class CCylinderHitbox;

class CTrampoline : public CModel
{
public:
	CTrampoline();										//コンストラクタ
	~CTrampoline();										//デストラクタ
														
	HRESULT Init(void) override;						//初期化処理
	void Uninit(void) override;							//終了処理
	void Update(void) override;							//更新処理
	void Draw(void) override;							//描画処理
														
	static CTrampoline* Create(const D3DXVECTOR3 pos);	//生成処理

private:

	CCylinderHitbox* m_pHitbox;							//ヒットボックス

};






#endif