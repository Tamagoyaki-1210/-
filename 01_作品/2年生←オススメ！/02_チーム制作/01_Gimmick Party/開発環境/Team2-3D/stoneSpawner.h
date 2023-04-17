//=============================================================================
//
// stoneSpawner.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _STONE_SPAWNER_H_
#define _STONE_SPAWNER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"


class CStoneSpawner : public CObject
{
public:
	CStoneSpawner();									//コンストラクタ
	~CStoneSpawner();									//デストラクタ

	HRESULT Init(void) override;						//初期化処理
	void Uninit(void) override;							//終了処理
	void Update(void) override;							//更新処理
	void Draw(void) override;							//描画処理

	void SetPos(const D3DXVECTOR3 pos) override;		//位置の設定処理
	const D3DXVECTOR3 GetPos(void) override;			//位置の取得処理

	const D3DXVECTOR2 GetSize(void) override;			//サイズの取得処理


	//生成処理
	static CStoneSpawner* Create(const D3DXVECTOR3 pos, const float fShadowHeight, const float fLaterlBounds, const float fLength, const int nMaxTime);

private:

	D3DXVECTOR3 m_pos;				//位置
	float		m_shadowY;			//影の高さ
	float		m_laterlBounds;		//横の限界
	float		m_fAreaLength;		//スポーンエリアの長さ
	int			m_nCntTime;			//フレームカウンター
	int			m_nSpawnTime;		//スポーンタイム
	int			m_nMaxSpawn;		//最大タイム

};





#endif