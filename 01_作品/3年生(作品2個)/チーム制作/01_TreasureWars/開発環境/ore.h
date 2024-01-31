//==============================================================================
//
// ore.h	(鉱石.h)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2重インクルードガード
//==============================================================================
#ifndef _ORE_H_
#define	_ORE_H_

//==============================================================================
// インクルード
//==============================================================================
#include "main.h"
#include "objectX.h"

//==============================================================================
// マクロ定義
//==============================================================================

//==============================================================================
// 前方宣言
//==============================================================================

//==============================================================================
// マップクラス
//==============================================================================
class COre : public CObjectX
{
	static const float MOVING_DISTANCE;
	static const int UNINIT_TIME;
	static const char* STAGE_MODEL_01[];
	static const char* STAGE_MODEL_02[];
	static const char* STAGE_MODEL_03[];

public:
	COre();						// コンストラクタ
	~COre();					// デストラクタ

	HRESULT Init() override;	// 初期化処理
	HRESULT Init(int nStageNum);	// 初期化処理
	void Uninit() override;		// 終了処理
	void Update() override;		// 更新処理

	int Damage(int damage);

	static COre *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size,int nStageNum);

	const bool GetOreDeath() { return m_bOreDeath; }
	const bool GetAppearance() { return m_bAppearance; }

private:
	static const int Normal_Ore_Life = 20;	// 通常鉱石の耐久力
	static const int Ore_Blink_Speed;		// 鉱石の点滅する速度
	static const float Ore_Blink_Timer;		// 鉱石が点滅し始める時間

	D3DXVECTOR3 m_move;			// 移動量
	int m_nLife;				// 体力
	int m_UninitTime;			// 消すまでの時間
	int m_nOreModelIndex;		// 鉱石モデル番号
	int m_nBigOreModelIndex;	// 鉱石(大)モデル番号
	bool m_bOnce;
	bool m_bAppearance;			// 出現が完了したかどうか
	bool m_bOreDeath;			// 自身が終了したか
};

#endif

