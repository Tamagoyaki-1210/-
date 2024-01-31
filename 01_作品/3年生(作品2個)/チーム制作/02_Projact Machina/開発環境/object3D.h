//==============================================================================================
//
// 3Dポリゴン　　　Object3D.h
// Author : tutida ryousei
// Author : tanimoto kosuke
//
//==============================================================================================
#ifndef _OBJECT3D_H_
#define	_OBJECT3D_H_

#include <d3dx9.h>
#include"object.h"
#include"texture.h"

//==============================================
// オブジェクト3Dクラス
//==============================================
class CObject3D : public CObject
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

	CObject3D(const PRIORITY priority = PRIORITY_CENTER);
	~CObject3D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	void UV();

	static CObject3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR2 size, const PRIORITY priority = PRIORITY_CENTER, const D3DXCOLOR col = {1.0f,1.0f, 1.0f, 1.0f}, const bool billboard = false);

	void SetTexPos(const float top, const float row, const float right, const float left);			// テクスチャの分割

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }					// 位置の設定
	void AddPos(const D3DXVECTOR3 pos) { m_pos += pos; }				// 位置の加算
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(const D3DXVECTOR2 size) { m_size = size; }
	void SetSubSize(const D3DXVECTOR2 halfsize) { m_Subsize = halfsize; }			// 減少するサイズ
	void SetCol(const D3DXCOLOR col) { m_col = col; }
	void SetAlpha(const float alpha) { m_col.a = alpha; }
	void AddAlpha(const float alpha) { m_col.a += alpha; }
	void Setbillboard(const bool billboard) { m_bBillboard = billboard; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }
	void SetVtxPos(D3DXVECTOR3 pos, int index) { VERTEX_3D *pVtx; m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0); pVtx[index].pos = pos; m_pVtxBuff->Unlock(); }

	const D3DXVECTOR3 GetPos() { return m_pos; }						// 位置の取得
	const D3DXVECTOR3 GetMove() { return m_move; }
	const D3DXVECTOR2 GetScale() { return m_size; }
	const D3DXCOLOR GetCol() { return m_col; }
	const D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }
	const D3DXVECTOR3 GetVtxPos(int index) { VERTEX_3D *pVtx; m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0); return pVtx[index].pos; }
	void VtxBuffUnlock() { m_pVtxBuff->Unlock(); }

private:

	// 頂点フォーマット
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR2 m_size;						// サイズ
	D3DXVECTOR3 m_move;						// 移動量
	D3DXVECTOR3 m_rot;						// 角度
	D3DXVECTOR2 m_Subsize;					// 減少するサイズ
	D3DXCOLOR m_col;						// 色
	CTexture::TEXTURE m_texture;			// テクスチャ
	bool m_bBillboard;						// ビルボードか
};

#endif // !_OBJECT3D_H_