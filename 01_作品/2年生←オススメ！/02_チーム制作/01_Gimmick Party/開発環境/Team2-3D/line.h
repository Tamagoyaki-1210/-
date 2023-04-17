//=============================================================================
//
// line.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef LINE_H
#define LINE_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"

class CLine : public CObject
{
public:
	CLine();											//コンストラクタ
	~CLine();											//デストラクタ

	HRESULT Init(void) override;						//初期化処理
	void Uninit(void) override;							//終了処理
	void Update(void) override;							//更新処理
	void Draw(void) override;							//描画処理

	void SetPos(const D3DXVECTOR3 pos) override;		//位置の設定処理

	const D3DXVECTOR2 GetSize(void) override;			//サイズの取得処理
	const D3DXVECTOR3 GetPos(void) override;			//位置の取得処理

	//生成処理
	static CLine* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 start, const D3DXVECTOR3 end, D3DXCOLOR col);
	static CLine* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 start, const D3DXVECTOR3 end);

private:

	void SetLine(void);

	D3DXVECTOR3 m_pos;						//位置
	D3DXVECTOR3 m_rot;						//回転
	D3DXCOLOR   m_col;						//カラー
	D3DXVECTOR3 m_start;					//始点
	D3DXVECTOR3 m_finish;					//終点
	D3DXMATRIX  m_mtxWorld;					//ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ

};

#endif