//==============================================================================================
//
// メッシュフィールドの処理			meshfield.h
// tutida ryousei
//
//==============================================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//**********************************************************************************************
// インクルード
//**********************************************************************************************
#include"renderer.h"
#include"Object3D.h"
#include"texture.h"

class CMeshField : CObject3D
{
public:
	CMeshField(const PRIORITY priority);
	~CMeshField() override;

	HRESULT Init() override;		// 初期化処理
	void Uninit(void) override;		// 終了処理
	void Update(void) override;		// 更新処理
	void Draw(void) override;		// 描画処理

	void Stencil();		// ステンシル
	float MeshCollision(D3DXVECTOR3 pos);	// 当たり判定
	float Ground_Broken(D3DXVECTOR3 pos, float damage, int scope);	// 地面が削れる
	void Normal();			// 法線の設定

	void SetNumBlock(int Xblock, int Zblock) { m_nXBlock = Xblock; m_nZBlock = Zblock; }	// ブロックの数の設定
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }	// 位置の設定
	void SetSize(float size) { m_fSize = size; }		// サイズの設定

	const D3DXVECTOR3 GetPos() { return m_pos; }						// 位置の取得
	bool GetHit() { return m_bHit; }

	static CMeshField* Create(D3DXVECTOR3 pos, int Xblock, int Zblock, float size);	// 生成処理

private:
	int m_nVertexNum;						// 頂点数
	int m_nIndexNum;						// インデックスバッファ
	int m_nPrimitiveNum;					// プリミティブ数
	int m_nXBlock;							// X方向のブロックの数
	int m_nZBlock;							// Z方向のブロックの数
	int m_nHeight;							// 頂点の高さ
	int m_nVecCount;						// 外積がマイナスになった回数
	int m_nPolyCount;						// 頂点バッファのカウント
	int m_nScope;							// 抉れる範囲
	int m_nCntIndex;						// 当たっている頂点番号
	float m_fSize;							// サイズ
	float m_fPosY;							// 当たった位置のY座標
	bool m_bPorigon_Scope;					// ポリゴンの範囲
	bool m_bHit;							// 攻撃が当たった
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_Rot;						// 向き
	D3DXMATRIX m_MtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_NorVec;					// 法線面のベクトル
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファへのポインタ
	CTexture::TEXTURE m_Texture;			// テクスチャ
};

#endif // !_MESHFIELD_H_