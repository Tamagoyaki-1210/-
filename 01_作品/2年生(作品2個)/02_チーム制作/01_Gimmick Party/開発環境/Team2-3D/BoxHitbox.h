//=============================================================================
//
// BoxHitbox.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef BOX_HITBOX_H
#define BOX_HITBOX_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "hitbox.h"

//=============================================================================
//前方宣言
//=============================================================================
class CLine;

class CBoxHitbox : public CHitbox
{
public:
	CBoxHitbox();										//コンストラクタ
	~CBoxHitbox() override;								//デストラクタ
														
	HRESULT Init(void) override;						//初期化処理
	void    Uninit(void) override;						//終了処理
	void    Update(void) override;						//更新処理

	//==============================================================================================================
	//												静的関数
	//==============================================================================================================


	//生成処理
	static CBoxHitbox* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, CObject* pParent);
	static CBoxHitbox* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, CObject* pParent, const int nScore);
	static CBoxHitbox* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 RelativePos, const D3DXVECTOR3 size, HITBOX_TYPE type, CObject* pParent, const int nScore, INTERACTION_EFFECT effect);

private:

	bool BoxBoxHit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);					//当たり判定

	CLine* m_pLine[12];					//デバッグようの線
};


#endif