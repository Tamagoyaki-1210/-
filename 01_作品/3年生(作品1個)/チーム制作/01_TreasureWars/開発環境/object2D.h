//==============================================================================================
//
// オブジェクト　　　object.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _OBJECT2D_H_
#define	_OBJECT2D_H_

#include <d3dx9.h>
#include"Object.h"
#include"texture.h"

class CInputkeyboard;
class CTexture;

class CObject2D : public CObject
{
public:
	CObject2D(int nPriority);
	~CObject2D() override;

	virtual HRESULT Init() override;
	void Uninit() override;
	virtual void Update() override;
	void Draw()  override;

	void Stencil();
	void UV();																		// UVの処理

	// 定義無しの描画の初期設定 = 3
	static CObject2D *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const int nPriority = 3);

	void Setposition(D3DXVECTOR3 pos);												// 位置の設定
	void Addposition(D3DXVECTOR3 add) { m_Pos += add; }								// 位置の加算設定
	D3DXVECTOR3 Getposition() { return m_Pos; }										// 位置の設定
	void SetMove(D3DXVECTOR3 move);													// 移動量の設定
	void SetSize(float width, float height);										// サイズの設定
	void SetGaugeSize(float width, float width2);									// ゲージの幅
	void Setrot(float rot);															// 角度の設定
	void Addrot(float fAdd) { m_fRot += fAdd; }										// 角度の設定
	float GetRot() { return m_fRot; }
	void SetTexPos(float top, float row, float right, float left);					// テクスチャ座標の設定
	void AnimTexture(int nPattern, int nPatternMax);
	void SetAnimControl(const int nAnimColumn, const int nAnimLine);
	void SetAnimControl(const int nAnimCounter);
	void SetAnimDiv(const int nDivX, const int nDivY) { m_nAnimColumnDiv = nDivX; m_nAnimLineDiv = nDivY; }
	void SetTexture(CTexture::TEXTURE texture);										// テクスチャの設定
	bool SetDisappear(float size, float alpha, float sizespeed, float alphaspeed);	// 消える設定
	void SetCol(D3DXCOLOR col) { m_Col = col; }										// 色の設定
	void SetAlpha(const float alpha) { m_Col.a = alpha; }							// アルファ値の設定
	void AddAlpha(const float alpha) { m_Col.a += alpha; }							// アルファ値の加算

	const D3DXVECTOR3 GetSize() { return{ m_fWidth, m_fHeight, 0.0f }; }
	const D3DXCOLOR GetColor() { return m_Col; }

protected:
	const float SCREEN_WIDTH = 1280;		// スクリーンの幅
	const float SCREEN_HEIGHT = 720;		// スクリーンの高さ

private:
	struct VERTEX_2D		// 頂点データ
	{
		D3DXVECTOR3 pos;
		float rhw;
		D3DCOLOR col;
		D3DXVECTOR2 tex;
	};

	const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 頂点フォーマット
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファ
	float m_fWidth, m_fWidth2;				// 幅
	float m_fHeight, m_fHeight2;			// 高さ
	float m_fInitWidth, m_fInitWidth2;		// 幅(初期値)
	float m_fInitHeight, m_fInitHeight2;	// 高さ(初期値)
	float m_fAngle;							// 対角線の角度を算出する
	float m_fRot;							// ポリゴンの角度
	int m_nAnimColumn;						// 画像アニメーションの列数
	int m_nAnimColumnDiv;					// 画像アニメーションの列区分
	int m_nAnimLineDiv;						// 画像アニメーションの行区分
	D3DXVECTOR3 m_Pos;						// ポリゴンの位置
	D3DXVECTOR3 m_move;						// 移動量
	D3DXCOLOR m_Col;						// 色
	CTexture::TEXTURE m_texture;			// テクスチャ
	EObjType m_ObjType;						//オブジェクトのタイプ
};

#endif // !_OBJECT_H_
