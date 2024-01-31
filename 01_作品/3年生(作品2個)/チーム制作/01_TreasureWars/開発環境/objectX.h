//==============================================================================================
//
//モデルの処理[model.h]
//Author：tutida ryousei
//
//==============================================================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include"Object.h"
#include <vector>

class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = 3);
	~CObjectX();

	virtual HRESULT Init() override;
	void Uninit() override;
	virtual void Update() override;
	void Draw() override;

	static void UninitObject();
	void DrawShadow();		// 影
	static int ReadObject(const char* name);		// オブジェクトの読み込み
	static CObjectX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* Xfilename);

	void SetIndex(const int index);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void AddPos(D3DXVECTOR3 pos) { m_pos += pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void AddRot(D3DXVECTOR3 rot) { m_rot += rot; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }

	// マテリアルの色情報を初期マテリアル色に戻す処理
	void ResetColor() { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] = m_ModelPattern[m_nIndex].MatColor[nCntMat]; } }
	void SetColor(const D3DXCOLOR col) { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] = col; } }
	void AddColor(const D3DXCOLOR col) { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat] += col; } }
	void SetAlpha(const float alpha) { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat].a = alpha; } }
	void AddAlpha(const float alpha) { for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++) { m_col[nCntMat].a += alpha; } }

	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	D3DXVECTOR3 GetSize() { return m_Size; }
	D3DXVECTOR3 GetMax() { return m_vtxMaxModel; }
	D3DXVECTOR3 GetMin() { return m_vtxMinModel; }
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }

private:

	struct ModelPattern
	{
		LPD3DXMESH pMesh;					// メッシュ情報へのポインタ
		LPD3DXBUFFER pBuffMat;				// マテリアル情報へのポインタ
		DWORD nNumMat;						// マテリアル情報の数
		std::string fileName;				// ファイル名
		std::vector <D3DXCOLOR> MatColor;	// 初期マテリアル色情報
		std::vector <LPDIRECT3DTEXTURE9> pTexture;		// テクスチャへのポインタ
	};
	static std::vector<ModelPattern> m_ModelPattern;

	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_posOld;			// 前回の位置
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_Initrot;			// 最初の向き
	D3DXVECTOR3 m_Size;				// サイズ
	D3DXVECTOR3 m_vtxMinModel;		// 頂点の最小値
	D3DXVECTOR3 m_vtxMaxModel;		// 頂点の最大値
	D3DXVECTOR3 m_vecDir;			// プレイヤーの方向ベクトル
	D3DXMATRIX m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX m_mtxRot;			// 回転マトリックス
	std::vector <D3DXCOLOR> m_col;	// モデルの色
	const char *m_FileName;			// ファイルの名前
	int m_nIndex;					// オブジェクトの番号
	static bool m_bRelease;
};

#endif // !_OBJECTX_H_