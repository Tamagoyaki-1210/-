//==============================================================================
//
// skybox.h
// Author : saito shian
//
//==============================================================================

//==============================================================================
// 2重インクルードガード
//==============================================================================
#ifndef _SKYBOX_H_
#define	_SKYBOX_H_

//==============================================================================
// インクルード
//==============================================================================
#include"Object.h"
#include"texture.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_SIZE				(1400.0f)	//サイズの最大値
#define MAX_SIDE_SIZE			(30)		//横の最大値
#define MIN_SIDE_SIZE			(50)		//横の最小値
#define START_SIDE				(16)		//横の最初の値(16角形)
#define MAX_VERTICAL_SIZE		(30)		//縦の最大値
#define MIN_VERTICAL_SIZE		(5)			//縦の最小値
#define START_VERTICAL			(15)		//縦の最初の値
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//座標,法線,カラー	※テクスチャも使う場合はここに追加(構造体にも追加)

//==============================================================================
// 前方宣言
//====================================== ========================================

//==============================================================================
// スカイボックスクラス
//==============================================================================
class CSkyBox : public CObject
{
public:
	CSkyBox(int nPriority);
	~CSkyBox() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;

	static CSkyBox *Create(D3DXVECTOR3 pos,int nStageNum = -1);

	void Setposition(D3DXVECTOR3 pos);												// 位置の設定
	void SetSize(D3DXVECTOR3 size);													// サイズの設定

	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }				// テクスチャの設定
	void SetStageTexture(int nStageNum);											//ステージごとに使用する背景を変える

private:
	// 頂点データ
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;					//頂点座標
		D3DXVECTOR3 nor;					//法線ベクトル
		D3DCOLOR col;						//頂点カラー
		D3DXVECTOR2 tex;					//テクスチャ座標
	};

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				//頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;				//インデックスバッファへのポインタ											
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffCone;			//円錐の頂点バッファへのポインタ
	CTexture::TEXTURE m_texture;					//テクスチャ
	D3DXMATRIX m_mtxWorld;							//ワールドマトリックス
	D3DXVECTOR3 IdxPosNor[3];						//Idxのpos
	D3DXVECTOR3 Calculation3DNor[2];				//3次元外積の計算結果
	D3DXVECTOR3 VecAnswer;							//外積の計算結果
	D3DXVECTOR3	m_pos;								//位置
	D3DXVECTOR3 m_size;								//サイズ
	D3DXVECTOR3	m_move;								//移動
	D3DXVECTOR3 m_rot;								//回転
	D3DXCOLOR m_col;								//色

	int	m_nHorizontal;								// 横
	int	m_nVertical;								// 縦
	int	m_nVtx;										// 頂点数
	int	m_nIdx;										// インデックス数
	int	m_nPolygon;									// ポリゴン数
};

#endif // !_OBJECT_H_
