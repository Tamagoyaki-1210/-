//=============================================================================
//
// coin.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _COIN_H
#define _COIN_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"

//=============================================================================
//前方宣言
//=============================================================================
class CBoxHitbox;

class CCoin : public CModel
{
public:

	//コインの種類
	enum COIN_TYPE
	{
		COIN_0 = 0,					//10点のコイン
		COIN_1,						//20点のコイン
		COIN_2,						//30点のコイン
		COIN_3,						//50点のコイン
									
		COIN_DROPPED,				//落としたコイン

		COIN_MAX
	};

	CCoin();									//コンストラクタ
	~CCoin() override;							//デストラクタ

	HRESULT Init(void) override;				//初期化処理
	void Uninit(void) override;					//終了処理
	void Update(void) override;					//更新処理
	void Draw(void) override;					//描画処理

	void SetMove(const D3DXVECTOR3 move);		//移動量の設定処理

	const D3DXVECTOR3 GetMove(void);			//移動量の取得処理


	static CCoin* Create(const D3DXVECTOR3 pos, const COIN_TYPE type);						//生成処理
	static CCoin* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 move, const int nLife, const COIN_TYPE type);		//生成処理

private:

	static const D3DXVECTOR3 m_hitboxSize[COIN_MAX];		//ヒットボックスの大きさ

	D3DXVECTOR3 m_move;				//移動量
	int m_nLife;					//ライフ

	CBoxHitbox* m_pHitbox;			//ヒットボックスへのポインタ

};





#endif