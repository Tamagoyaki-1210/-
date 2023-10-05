//==============================================================================================
//
// タイトル　　　title.h
// tutida ryousei
//
//==============================================================================================
#ifndef _TITLE_H_
#define	_TITLE_H_

#include"mode.h"

class CTitleObj;

class CTitle : public CMode
{
	static const int TITLEORE_MAX = 20;	//初期生成時の最大数 
public:
	CTitle();
	~CTitle();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CTitle *Create();

private:
	// 頂点データ
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;				//頂点座標
		D3DXVECTOR3 nor;				//法線ベクトル
		D3DCOLOR col;					//頂点カラー
		D3DXVECTOR2 tex;				//テクスチャ座標
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPD3DXBUFFER m_pBuffMat;			// マテリアル情報へのポインタ
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXMATRIX m_mtxRot;				// 回転マトリックス
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	LPD3DXMESH m_pMesh;					// メッシュ情報へのポインタ
	DWORD m_nNumMat;					// マテリアル情報の数	

	int m_TitleObjModel;				// 鉱石のモデル
	int m_ModelIdx;						// 札束のモデル

	float m_RandX;						// ランダムのx値
	float m_RandY;						// ランダムのy値
};

#endif // !_GAME_H_
