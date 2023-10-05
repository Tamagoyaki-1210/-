//=============================================================================
//
// フェード.h
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <d3dx9.h>

//---------------------------
//前方宣言
//---------------------------
class CObject2D;

//---------------------------
//クラス
//---------------------------
class CFade 
{
public:
	// 頂点データ
	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;			//xyz座標
		float rhw;					//rhw
		D3DCOLOR col;				//カーラー
		D3DXVECTOR2 tex;			//テクスチャ座標
	};

	enum FADE
	{
		FADE_NONE = 0,	// 何もしていない状態
		FADE_IN,		// フェードイン(徐々に透明にする状態)
		FADE_OUT,		// フェードアウト(徐々に不透明にする状態)
		FADE_CHANGE,	// フェード切り替え
		FADE_MAX
	};

	CFade();
	~CFade();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetColor(D3DXCOLOR col);		// フェードの色変更処理
	void SetFade();						// フェードの設定処理

	static CFade *Create();

	FADE GetFade() { return m_fade; }	//フェードの取得処理
private:
	float m_fAlpha;						// ポリゴン(フェード)のa値
	FADE m_fade;						// 現在のフェード
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファ

};

#endif // !_PLAYER_H_