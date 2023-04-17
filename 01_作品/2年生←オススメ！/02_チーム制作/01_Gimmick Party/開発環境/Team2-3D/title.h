//=============================================================================
//
// title.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "mode.h"

//=============================================================================
//前方宣言
//=============================================================================
class CPlayerModel;
class CAnimateUI;

//---------------------------
//クラス宣言
//---------------------------
class CTitle : public CMode
{
public:
	CTitle();											//コンストラクタ
	~CTitle() override;									//デストラクタ
														
	HRESULT Init(void) override;						//初期化処理
	void Uninit(void) override;							//終了処理
	void Update(void) override;							//更新処理
														
	static CTitle* Create(void);						//生成処理
private:												
														
	static const int MAX_TITLE_MODEL = 7;				//モデルの最大数
														
	CPlayerModel* m_pModel[MAX_TITLE_MODEL];			//プレイヤーモデルへのポインタ
	CAnimateUI*   m_pTitle;								//UIへのポインタ

};

#endif // !_TITLE_H_
