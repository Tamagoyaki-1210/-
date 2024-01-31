//==============================================================================================
//
// オブジェクト　　　object.h
// Author : tutida ryousei
//			tanimoto kosuke
//
//==============================================================================================
#ifndef _OBJECT2D_H_
#define	_OBJECT2D_H_

#include <d3dx9.h>
#include"Object.h"
#include"texture.h"

class CObject2D : public CObject
{
	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 頂点フォーマット

public:
	CObject2D(const PRIORITY priority);
	~CObject2D() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw()  override;

	void Stencil();
	void UV();																		// UVの処理

	// 定義無しの描画の初期設定 = 3
	static CObject2D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const PRIORITY priority = PRIORITY_CENTER);

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }								// 位置の設定
	void AddPos(const D3DXVECTOR3 add) { m_pos += add; }							// 位置の加算設定
	void SetMove(const D3DXVECTOR3 move);											// 移動量の設定
	void SetSize(const D3DXVECTOR2 size) { m_size = size; }							// サイズの設定
	void SetSubSize(const D3DXVECTOR2 halfsize) { m_Subsize = halfsize; }			// 減少するサイズ
	void SetRot(const D3DXVECTOR2 rot) { m_rot = rot; }							// 角度の設定
	void AddRot(const D3DXVECTOR2 add) { m_rot += add; }							// 角度の設定
	void SetTexPos(float top, float row, float right, float left);					// テクスチャ座標の設定
	void AnimTexture(int nPattern, int nPatternMax);
	void SetAnimControl(const int nAnimColumn, const int nAnimLine);
	void SetAnimControl(const int nAnimCounter);
	void SetAnimDiv(const int nDivX, const int nDivY) { m_nAnimColumnDiv = nDivX; m_nAnimLineDiv = nDivY; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }				// テクスチャの設定
	void SetCol(D3DXCOLOR col) { m_col = col; }										// 色の設定
	void SetAlpha(const float alpha) { m_col.a = alpha; }							// アルファ値の設定
	void AddAlpha(const float alpha) { m_col.a += alpha; }							// アルファ値の加算

	D3DXVECTOR3 GetPos() { return m_pos; }						// 位置の取得
	const D3DXVECTOR2 GetSize() { return m_size; }				// サイズの取得
	const D3DXVECTOR2 GetRot() { return m_rot; }				// 角度の取得
	const D3DXCOLOR GetColor() { return m_col; }				// 色の取得

private:
	struct VERTEX_2D		// 頂点データ
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	int m_nAnimColumn;						// 画像アニメーションの列数
	int m_nAnimColumnDiv;					// 画像アニメーションの列区分
	int m_nAnimLineDiv;						// 画像アニメーションの行区分
	D3DXVECTOR3 m_pos;						// ポリゴンの位置
	D3DXVECTOR3 m_move;						// 移動量
	D3DXVECTOR2 m_size;						// サイズ
	D3DXVECTOR2 m_Subsize;					// 減少するサイズ
	D3DXVECTOR2 m_rot;						// ポリゴンの角度
	D3DXCOLOR m_col;						// 色
	CTexture::TEXTURE m_texture;			// テクスチャ
};

#endif // !_OBJECT_H_
