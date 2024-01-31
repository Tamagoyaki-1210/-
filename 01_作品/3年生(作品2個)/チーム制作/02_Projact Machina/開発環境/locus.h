//==============================================================================================
//
// 軌跡　　　locus.h
// Author : koduna hirohito
//
//==============================================================================================
#ifndef _LOCUS_H_
#define	_LOCUS_H_

#include <d3dx9.h>
#include"object.h"
#include"texture.h"
#include"move_object.h"

class CLocus : public CObject
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
		bool broken;			//メッシュを削る
	};

	CLocus(const PRIORITY priority = PRIORITY_CENTER);
	~CLocus() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void UV();

	static CLocus* Create(const D3DXVECTOR3 pos, const float fsize, int nAnchorPoints, const PRIORITY priority = PRIORITY_CENTER, const D3DXCOLOR col = { 1.0f,1.0f, 1.0f, 1.0f }, const bool billboard = true);

	void SetTexPos(const float top, const float row, const float right, const float left);			// テクスチャの分割

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(const float fsize) { m_fsize = fsize; }
	void SetCol(const D3DXCOLOR col) { m_col = col; }
	void SetAlpha(const float alpha) { m_col.a = alpha; }
	void AddAlpha(const float alpha) { m_col.a += alpha; }
	void Setbillboard(const bool billboard) { m_bBillboard = billboard; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }
	void SetAnchorPoints(int nanchorpoints);

	const D3DXVECTOR3 GetMove() { return m_move; }
	const D3DXCOLOR GetCol() { return m_col; }
	const D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }

private:

	D3DXVECTOR3 m_pos;
	// 頂点フォーマット
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	float m_fsize;							// サイズ
	D3DXVECTOR3 m_move;						// 移動量
	D3DXVECTOR3 m_rot;						// 角度
	D3DXCOLOR m_col;						// 色
	CTexture::TEXTURE m_texture;			// テクスチャ
	bool m_bBillboard;						// ビルボードか
	int m_nAnchorPoint;						// 軌跡の区分の数
	D3DXVECTOR3 *m_pAnchorPoints;			// 軌跡の区分の基準位置
};

#endif // !_LOCUS_H_