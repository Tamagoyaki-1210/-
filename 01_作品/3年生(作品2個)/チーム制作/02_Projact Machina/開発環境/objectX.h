//==============================================================================================
//
// オブジェクトX　　　objectX.h
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#ifndef _OBJECTX_H_
#define	_OBJECTX_H_

#include"object.h"
#include "model.h"
#include <vector>

class CObjectX : public CObject
{
public:
	CObjectX(const PRIORITY priority);
	virtual ~CObjectX() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void DrawShadow(const LPDIRECT3DDEVICE9 device, CModel::ModelPattern model);
	void Stencil();

	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const char* Xfilename, const PRIORITY priority = PRIORITY_MODEL);
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CObjectX* parent, const int index, const PRIORITY priority = PRIORITY_MODEL);

	void SetShadow(const bool shadow) { m_bShadow = shadow; }
	void SetParts(const bool parts) { m_bParts = parts; }
	void SetIndex(const int index) { m_nIndex = index; }
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void AddPos(const D3DXVECTOR3 pos) { m_pos += pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void AddRot(const D3DXVECTOR3 rot) { m_rot += rot; }
	void SetSize(const D3DXVECTOR3 size) { m_size = size; }
	void SetParent(CObjectX* pModel) { m_pParent = pModel; }
	void SetModel(const int index);
	void SetWireFrame(bool bwireFrame) { m_bWireFrame = bwireFrame; }

	// マテリアルの色情報を初期マテリアル色に戻す処理
	void SetColor(const D3DXCOLOR col) { m_col = col; }
	void AddColor(const D3DXCOLOR col) { m_col += col; }
	void SetAlpha(const float alpha) { m_col.a = alpha; }
	void AddAlpha(const float alpha) { m_col.a += alpha; }

	const D3DXVECTOR3 GetPos() { return m_pos; }
	const D3DXVECTOR3 GetMove() { return m_move; }
	const D3DXVECTOR3 GetRot() { return m_rot; }
	const D3DXMATRIX GetMatrix() { return m_mtxWorld; }
	CObjectX* GetParent() { return m_pParent; }
	const D3DXVECTOR3 GetMaxSize() { return m_vtxMaxModel; }
	const D3DXVECTOR3 GetMinSize() { return m_vtxMinModel; }

private:

	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 現在の角度
	D3DXVECTOR3 m_move;			// 移動量
	D3DXVECTOR3 m_size;			// サイズ
	D3DXVECTOR3 m_vtxMinModel;	// 頂点の最小値
	D3DXVECTOR3 m_vtxMaxModel;	// 頂点の最大値
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXCOLOR m_col;				// モデルの色
	std::vector <D3DXCOLOR> m_Original_col;		// モデルの元の色
	CObjectX* m_pParent;			// 親モデルへのポインタ
	int m_nIndex;				// Xファイルの番号
	int m_nNumMat;				// 現在のモデルのマテリアル数
	bool m_bShadow;				// 影の描画処理
	bool m_bParts;				// パーツ用のモデルであるかどうか
	bool m_bWireFrame;			// ワイヤーフレーム
};

#endif // !_MODEL_H_
