//=============================================================================
//
// 3DPolygon.h
// Author : koduna hirohito
//
//=============================================================================
#ifndef _3DPOLGON_H_			//このマクロ定義がされなかったら
#define _3DPOLGON_H_			//2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"
#include"texture.h"


//*****************************************************************************
// クラス定義
//*****************************************************************************
class C3DPolygon : public CObject
{
private:
	static const float FLASHING_AMOUNT;

public:

	// 頂点情報[3D]の構造体を定義
	typedef struct
	{
		D3DXVECTOR3 pos;//頂点座標
		D3DXVECTOR3 nor;//法線ベクトル
		D3DCOLOR col;//頂点カラー
		D3DXVECTOR2 tex;		//テクスチャ座標
	}VERTEX_3D;

	C3DPolygon();//コンストラクタ
	virtual ~C3DPolygon() override;//デストラクタ

	virtual HRESULT Init() override;//初期化
	virtual void Uninit() override;//終了処理
	virtual void Update() override;//更新処理
	virtual void Draw() override;//描画処理

	virtual bool IsUnused() { return true; }//終了条件処理
	virtual void SetUp(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move);//セットアップ

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	D3DXVECTOR3 GetPos() { return m_Pos; }
	void AddPos(D3DXVECTOR3 pos) { m_Pos += pos; }
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }
	void AddRot(D3DXVECTOR3 addrot) { m_Rot += addrot; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	void SetUV(D3DXVECTOR2 uv) { m_UV = uv; }
	D3DXVECTOR2 GetUV() { return m_UV; }
	void AddUVPos(D3DXVECTOR2 uvmove) { m_UV += uvmove; }
	void SetUVMove(D3DXVECTOR2 uvmove) { m_UVMove = uvmove; }
	void SetUVSize(D3DXVECTOR2 uvsize) { m_UVSize = uvsize; }
	void SetColor(D3DXCOLOR color) { m_Color = color; }
	void AddColor(D3DXCOLOR color) { m_Color += color; }
	D3DXCOLOR GetColor() { return m_Color; }
	D3DXVECTOR3 GetRot() { return m_Rot; }
	void SetFlashing(int nflashing) { m_nFlashingCnt = nflashing; }

	virtual void SetDiagonalLine(float fwidth, float fheight);//表示するポリゴンの縦横幅設定
	void AddDiagonalLine(float faddwidth, float faddheight);//表示するポリゴンの縦横幅Add

	virtual void SetPolygon();//更新処理以外でのポリゴンのセット
	void SetTextIndex(CTexture::TEXTURE Index) { m_TextIndex = Index; }//このポリゴンが使用するテクスチャ番号のセット
	CTexture::TEXTURE GetTextIndex() { return m_TextIndex; }//このポリゴンが使用するテクスチャ番号の取得

	static C3DPolygon *C3DPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 size, D3DXCOLOR col);
private:

	D3DXMATRIX m_MtxWorld;				//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;//頂点バッファへのポインタ
	float m_fWidth;//横幅
	float m_fHeight;//縦幅
	D3DXVECTOR2 m_UV;//UVの座標
	D3DXVECTOR2 m_UVSize;//UVのサイズ
	D3DXCOLOR m_Color;//色
	CTexture::TEXTURE m_TextIndex;//テクスチャ番号
	D3DXVECTOR3 m_Pos;//位置
	D3DXVECTOR3 m_Rot;//向き
	D3DXVECTOR3 m_Move;//移動量
	D3DXVECTOR2 m_UVMove;//UVの移動量
	int m_nFlashingCnt;//点滅カウント
	bool m_bFadeInOut;//フェード状態(true = in,false = out)


};

#endif