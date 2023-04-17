//=============================================================================
//
// light.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _LIGHT_H_
#define _LIGHT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

class CLight
{
public:
	CLight();								//コンストラクタ
	virtual ~CLight();						//デストラクタ
										
	virtual HRESULT Init(void);				//初期化処理
	virtual void Uninit(void);				//終了処理
	virtual void Update(void);				//更新処理

	void Release(void);						//破棄処理

	const int GetLightIdx(void);			//インデックスの取得処理

	static void ReleaseAll(void);			//全部の破棄処理

private:

	static const int MaxLight = 5;			//ライトの最大数

	int m_nIdx;								//インデックス

	static CLight* m_pLight[MaxLight];		//ライトへのポインタ
};

#endif // !_LIGHT_H_