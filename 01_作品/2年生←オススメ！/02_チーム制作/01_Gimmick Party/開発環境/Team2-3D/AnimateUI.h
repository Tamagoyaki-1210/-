//=============================================================================
//
// AnimateUI.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _ANIMATE_UI_H_
#define _ANIMATE_UI_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object2D.h"

class CAnimateUI : public CObject_2D
{
public:

	struct UIAnimation
	{
		D3DXVECTOR2 deltaSize;							//サイズの減数	
		int			nChangeTime;						//アニメーション半時間
	};

	CAnimateUI();																//コンストラクタ
	~CAnimateUI() override;														//デストラクタ
																				
	HRESULT Init(void) override;												//初期化処理
	void Uninit(void) override;													//終了処理
	void Update(void) override;													//更新処理

	void AnimateColor(const bool bAnim);										//カーラーアニメーションの設定処理
																				
	static CAnimateUI* Create(CObject::TextType texture, const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const D3DXCOLOR col, UIAnimation animInfo);		//生成処理


private:

	static const D3DXCOLOR targetCol[6];				//カーラーアニメーションのターゲット
														
	UIAnimation m_AnimInfo;								//アニメーション情報
	int			m_nCntAnim;								//アニメーションカウンター
	int			m_ntargetCol;							//目的のカーラー番号
	D3DXCOLOR	m_deltaCol;								//カーラーの減数
	bool		m_bAnimateColor;						//カーラーアニメーションをするかどうか

};







#endif