//==============================================================================================
//
// モデル　　　madel.h
// tutida ryousei
//
//==============================================================================================
#ifndef _MODEL_H_
#define	_MODEL_H_

#include"renderer.h"
#include <vector>

class CModel
{
public:
	CModel();
	~CModel();

	HRESULT Init();
	void Uninit();
	void Update();
	D3DXMATRIX Draw();

	void DrawShadow(const LPDIRECT3DDEVICE9 device);
	void Stencil();
	static void ModelReleaseAll();
	int ReadObject(const char* name);		// オブジェクトの読み込み

	static CModel* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel *parent, char *Xfilename);

	void SetIndex(const int index) { m_nIndex = index; }
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetShadow(const bool shadow) { m_bShadow = shadow; }
	void SetParent(CModel *pModel) { m_pParent = pModel; }

	// マテリアルの色情報を初期マテリアル色に戻す処理
	void ResetColor() { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] = m_Model[m_nIndex].MatColor[nCntMat]; } }
	void SetColor(const D3DXCOLOR col) { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] = col; } }
	void AddColor(const D3DXCOLOR col) { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] += col; } }
	void SetAlpha(const float alpha) { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat].a = alpha; } }
	void AddAlpha(const float alpha) { for (int nCntMat = 0; nCntMat < (int)m_Model[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat].a += alpha; } }

	D3DXVECTOR3 GetPos() { return m_Pos; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }
	CModel *GetParent() { return m_pParent; }
	D3DXVECTOR3 GetMaxSize() { return m_vtxMaxModel; }
	D3DXVECTOR3 GetMinSize() { return m_vtxMinModel; }

private:

	struct Model
	{
		LPD3DXBUFFER pBuffMat;				// マテリアル情報へのポインタ
		LPD3DXMESH pMesh;					// メッシュ情報へのポインタ
		DWORD nNumMat;						// マテリアル情報の数
		const char* fileName;				// ファイル名
		std::vector <D3DXCOLOR> MatColor;	// 初期マテリアル色情報
		std::vector <LPDIRECT3DTEXTURE9> pTexture;	// モデルのテクスチャへのポインタの配列
	};
	static std::vector <Model> m_Model;		// モデル構造体

	D3DXVECTOR3 m_Pos;			// 位置
	D3DXVECTOR3 m_Rot;			// 現在の角度
	D3DXVECTOR3 m_Size;			// サイズ
	D3DXVECTOR3 m_vtxMinModel;	// 頂点の最小値
	D3DXVECTOR3 m_vtxMaxModel;	// 頂点の最大値
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	std::vector <D3DXCOLOR> m_col;		// モデルの色
	CModel *m_pParent;			// 親モデルへのポインタ
	const char *m_XFileName;	// Xファイルの名前
	int m_nIndex;				// Xファイルの番号
	bool m_bShadow;				// 影の描画処理
};

#endif // !_MODEL_H_
