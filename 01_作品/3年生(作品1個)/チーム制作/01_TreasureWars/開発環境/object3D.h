//==============================================================================================
//
// 3Dポリゴン　　　Object3D.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _OBJECT3D_H_
#define	_OBJECT3D_H_

#include <d3dx9.h>
#include"object.h"
#include"texture.h"

class CInputkeyboard;
class CTexture;


class CObject3D : public CObject
{
public:
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;		//頂点座標
		D3DXVECTOR3 nor;		//法線ベクトル
		D3DCOLOR col;			//頂点カラー
		D3DXVECTOR2 tex;		//テクスチャ座標
	};

	CObject3D(int nPriority = 3);
	~CObject3D() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col = {1.0f,1.0f, 1.0f, 1.0f}, int priority = 3, bool billboard = false);

	void SetTexPos(float top, float row, float right, float left);			// テクスチャの分割

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; m_SizeX = size.x; }
	void SetSizeX(float sizeX) { m_SizeX = sizeX; }
	void SetCol(D3DXCOLOR col) { m_Col = col; }
	void SetAlpha(float alpha) { m_Col.a = alpha; }
	void AddAlpha(float alpha) { m_Col.a += alpha; }
	void Setbillboard(bool billboard) { m_bBillboard = billboard; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }

	D3DXVECTOR3 GetPos() { return m_Pos; }
	D3DXVECTOR3 GetScale() { return m_Size; }
	D3DXCOLOR GetCol() { return m_Col; }
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }

protected:
	const float SCREEN_WIDTH = 1280;		// スクリーンの幅
	const float SCREEN_HEIGHT = 720;		// スクリーンの高さ

private:
	// 頂点フォーマット
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR3 m_Size;						// サイズ
	float m_SizeX;							// 長さ変える用
	float m_SizeZ;							// 奥行
	D3DXVECTOR3 m_Pos;						// ポリゴンの位置
	D3DXVECTOR3 m_Rot;						// 角度
	D3DXCOLOR m_Col;						// 色
	CTexture::TEXTURE m_texture;			// テクスチャ
	bool m_bBillboard;						// ビルボードか
	EObjType m_ObjType;						// オブジェクトのタイプ
};

#endif // !_OBJECT3D_H_