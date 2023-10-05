//=============================================================================
//
// fallingStone.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _FALLING_STONE_H_
#define _FALLING_STONE_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"
#include <vector>

//=============================================================================
//前方宣言
//=============================================================================
class CCylinderHitbox;

class CFallingStone : public CObject
{
public:
	CFallingStone();												//コンストラクタ
	~CFallingStone() override;										//デストラクタ

	virtual HRESULT Init(void) override;							//初期化処理
	virtual void Uninit(void) override;								//終了処理
	virtual void Update(void) override;								//更新処理
	virtual void Draw(void) override;								//描画処理

	void SetPos(const D3DXVECTOR3 pos) override;					//位置の設定処理
	const D3DXVECTOR3 GetPos(void) override;						//位置の取得処理

	const D3DXVECTOR2 GetSize(void) override;						//サイズの取得処理

	void StartRotation(const D3DXVECTOR3 frameRot);					//回転開始の設定処理
	void StopRotating(void);										//回転停止処理
	void SetDirection(const float fLateralDirection);				//向きの設定処理


	//生成処理
	static CFallingStone* Create(CModel::ModelType type, D3DXVECTOR3 pos, const D3DXVECTOR3 spawnPos, const float ShadowHeight, const float fBound, const float fDir);			//生成処理

private:

	LPD3DXMESH				m_pMesh;												//メッシュ情報へのポインタ
	LPD3DXBUFFER			m_pBuffMat;												//マテリアル情報へのポインタ
	DWORD					m_nNumMat;												//マテリアル情報の数
	D3DXVECTOR3				m_pos;													//現在の位置
	D3DXVECTOR3				m_LastPos;												//前回の位置
	D3DXVECTOR3				m_move;													//モデルの移動量
	D3DXVECTOR3				m_rot;													//向き
	D3DXVECTOR3				m_spawnPos;												//スポーンのオブジェクトの位置
	D3DXMATRIX				m_mtxWorld;												//ワールドマトリックス
	float					m_shadowPosY;											//影の高さ
	float					m_fBound;												//影のエリアの幅
	float					m_fDir;													//落下の方向
	D3DXVECTOR3				m_frameRot;												//回転速度
	CModel::ModelType		m_type;													//モデルの種類
	std::vector <LPDIRECT3DTEXTURE9> m_vModelTexture;								//モデルのテクスチャへのポインタ
	CCylinderHitbox*		m_pHitbox;

};



#endif