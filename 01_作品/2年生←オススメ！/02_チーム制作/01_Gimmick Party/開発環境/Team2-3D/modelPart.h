//=============================================================================
//
// modelPart.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef _MODEL_PART_H
#define _MODEL_PART_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "model.h"

class CModelPart
{
public:

	CModelPart();																		//コンストラクタ
	~CModelPart();																		//デストラクタ

	HRESULT Init(void);																	//初期化処理
	void Uninit(void);																	//終了処理
	void Update(void);																	//更新処理
	void Draw(void);																	//描画処理
	void Draw(D3DXMATRIX mtxParent);													//描画処理

	void SetParent(CModelPart* pParent);												//親の設定処理
	CModelPart* GetParent(void);														//親の取得処理

	void SetPos(const D3DXVECTOR3 pos);													//位置の設定処理
	const D3DXVECTOR3 GetPos(void);														//位置の取得処理

	void SetRot(const D3DXVECTOR3 rot);													//向きの設定処理
	const D3DXVECTOR3 GetRot(void);														//向きの取得処理

	const D3DXVECTOR3 GetSize(void);													//サイズの取得処理
	D3DXMATRIX GetMatrix(void);															//マトリックスの取得処理

	void SetShadowHeight(const float fHeight);											//影の高さの設定処理

	void SetModelColor(const int nNumMat, const D3DXCOLOR col);
	void SetModel(CModel::ModelType model);

	static CModelPart* Create(CModel::ModelType type, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);								//生成処理

private:

	LPD3DXMESH				m_pMesh;												//メッシュ情報へのポインタ
	LPD3DXBUFFER			m_pBuffMat;												//マテリアル情報へのポインタ
	DWORD					m_nNumMat;												//マテリアル情報の数
	D3DXVECTOR3				m_pos;													//現在の位置
	D3DXVECTOR3				m_LastPos;												//前回の位置
	D3DXVECTOR3				m_move;													//モデルの移動量
	D3DXVECTOR3				m_rot;													//向き
	D3DXMATRIX				m_mtxWorld;												//ワールドマトリックス
	float					m_shadowPosY;											//影の高さ
	CModel::ModelType		m_type;													//モデルの種類
	std::vector <LPDIRECT3DTEXTURE9> m_vModelTexture;								//モデルのテクスチャへのポインタ
	CModelPart*				m_pParent;												//親へのポインタ
	std::vector <CModel::ModelColor> m_vCol;										//モデルの色
};

#endif