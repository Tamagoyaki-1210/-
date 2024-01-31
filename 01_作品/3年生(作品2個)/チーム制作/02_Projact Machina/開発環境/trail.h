#ifndef _ORBIT_H_
#define _ORBIT_H_

#include "object.h"

class CTrail : public CObject
{
public:
	// 3D頂点フォーマット
	const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;		//頂点座標
		D3DXVECTOR3 nor;		//法線ベクトル
		D3DCOLOR col;			//頂点カラー
		D3DXVECTOR2 tex;		//テクスチャ座標
	};

	CTrail();	//コンストラクタ
	~CTrail();	//デストラクタ

	HRESULT Init() override { return S_OK; }
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetTrail(D3DXMATRIX mtx, D3DXCOLOR col);
	void SetMtxParent(D3DXMATRIX pMtx) { m_pMtxParent = pMtx; };	//親のマトリックスの設定
	void SetPos(D3DXVECTOR3 offset) { m_offsetPos = offset; }	//オフセット座標の設定
	void SetColor(D3DXCOLOR col, D3DXCOLOR destcol);			//色の設定

	D3DXVECTOR3 GetPos() { return m_offsetPos; }	//位置の取得

	static CTrail* Create(D3DXVECTOR3 offset, D3DXMATRIX mtx);

private:
	static const int nMaxVtx = 1024;	//頂点の最大数

	D3DXVECTOR3 m_offsetPos;			//オフセット座標
	D3DXVECTOR3 m_worldPos;				//ワールド座標
	D3DXMATRIX	m_mtxWorld;				//ワールドマトリックス
	D3DXMATRIX m_pMtxParent;			//軌跡の親モデルのマトリックス
	D3DXCOLOR m_beginCol;				//色の開始値
	D3DXCOLOR m_endCol;					//色の終了値
	int m_nNumVtx;						//頂点数

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
};

#endif