//=============================================================================
//
// object2D.h
// Author : Ricci Alex
//
//=============================================================================
#ifndef OBJECT_2D_H
#define OBJECT_2D_H

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"

//=============================================================================
//前方宣言
//=============================================================================
class CBullet;

//=============================================================================
//2Dポリゴンのクラス
//=============================================================================
class CObject_2D : public CObject
{
public:
	CObject_2D();																	//コンストラクタ
	CObject_2D(const int nPriority);												//コンストラクタ
	virtual ~CObject_2D() override;													//デストラクタ
														
	virtual HRESULT Init(void) override;											//初期化処理
	virtual void Uninit(void) override;												//終了処理
	virtual void Update(void) override;												//更新処理
	virtual void Draw(void) override;												//終了処理

public:
	void SetPos(const D3DXVECTOR3 pos) override;									//位置の設定処理
	void SetSize(const D3DXVECTOR2 dim);											//ポリゴンの幅と高さの設定処理
	void SetSize(const float x, const float y);										//ポリゴンの幅と高さの設定処理
	const D3DXVECTOR2 GetSize(void) override;										//ポリゴンのサイズの取得処理
	const D3DXVECTOR3 GetMove(void);												//速度の取得処理
	void SetMove(const D3DXVECTOR3 move);											//速度の設定処理
	const D3DXVECTOR3 GetAcceleration(void);										//加速の取得処理
	void SetAcceleration(const D3DXVECTOR3 acc);									//加速の設定処理
	void SetRotation(const float fFrameRot);										//回転の設定処理
	void AddAngularSpeed(const float fSpeed);										//回転速度の加算処理
	const float GetAngularSpeed(void);												//回転速度の取得処理
	void SetStartingRot(const float fStartRot);										//回転角度の初期値
	const float GetRot(void);														//回転角度の取得処理
	void SetRevolution(const D3DXVECTOR3 Center, 									//公転の設定処理
		const float fAngularSpeed, const float fRadius);							
	void SetRevolutionCenter(const D3DXVECTOR3 Center);								//公転の中心点の設定処理
	void SetPresentRevolutionAngle(const float fAngle);								//現在の公転角度の設定処理
	const D3DXVECTOR3 GetRevolutionCenter(void);									//公転の中心点の取得処理
	void AddRevolutionSpeed(const float fSpeed);									//公転速度の加算処理

	const D3DXVECTOR3 GetPos(void) override;										//位置の取得処理
	void SetTextureParameter(const int MaxPattern, const int LinePattern,
		const int ColumnPattern, const int nAnimFrame);								//テクスチャのパラメータの設定処理
	const int GetPresentAnimPattern(void);											//現在のアニメーションパターンの取得処理
	void SetAnimPattern(const int PatternNum);										//現在のアニメーションパターンの設定処理
	void SetAnimationBase(const int FirstPattern);									//アニメーションの最初のパターンの設定処理
	void MoveTexCoordinates(const D3DXVECTOR2 move);								//テクスチャの移動量の設定処理
	bool GetFlipX(void);															//テクスチャのX座標が反転しているかどうかを取得する
	void FlipX(void);																//テクスチャのX座標を反転する
	void FlipY(void);																//テクスチャのY座標を反転する
	const D3DXCOLOR GetColor(void);													//カーラーの取得処理
	void SetColor(const D3DXCOLOR col);												//カーラーの設定処理
	void SetVtxColor(const int nVtxIdx, const D3DXCOLOR col);						//頂点１つのカーラー設定処理

	void SetTexture(TextType textType);												//テクスチャの種類の設定処理
	LPDIRECT3DVERTEXBUFFER9 GetVtxbuff(void);										//頂点バッファの取得処理

	static void LoadTextures(void);													//テクスチャを全部ロードする処理
	static void DestroyLoadedTextures(void);										//テクスチャを全部破棄する処理
	static LPDIRECT3DTEXTURE9 GetTexturePointer(CObject::TextType type);			//テクスチャへのポインタの取得処理

	static CObject_2D* Create(void);												//CObject_2Dの生成処理

private:

	static char* m_paTextPass[CObject::TEXTURE_TYPE_MAX];

	void UpdateTexture(void);									//テクスチャ座標の更新処理

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							//頂点バッファ

	//================================================================================================================
	//オブジェクトの位置と移動量と関係がある変数
	D3DXVECTOR3 m_posPolygon;									//ポリゴンの位置
	D3DXVECTOR3 m_dir;											//ポリゴンの向き
	D3DXVECTOR3 m_move;											//移動量
	D3DXVECTOR3 m_acc;											//加速
	float m_fRot;												//ポリゴンの回転角度
	float m_fAngle;												//ポリゴンの回転用の角度
	float m_fFrameRot;											//1フレームの回転角度
	D3DXVECTOR3 m_revolutionCenter;								//公転の中心点
	float m_fRevolutionSpeed;									//公転のスピード
	float m_fRevolutionAngle;									//公転の現在角度
	float m_fRevolutionRadius;									//公転の半径
	D3DXVECTOR2 m_size;											//ポリゴンの幅と高さ
	//================================================================================================================
	//================================================================================================================
	//テクスチャとアニメーションと関係がある変数
	int m_nAnimPattern;											//現在のテクスチャパターン
	int m_nCountAnim;											//アニメーションカウンター
	int m_nMaxTexPattern;										//テクスチャのパターン数
	int m_nMaxTexColumn;										//テクスチャの行数
	int m_nTexLine;												//テクスチャの列数
	int m_nFirstPattern;										//アニメーションの最初のパターン
	int m_nAnimFrame;											//アニメーションパターンの変更フレーム数
	D3DXVECTOR2 m_textureTranslation;							//テクスチャの移動量
	D3DXVECTOR2 m_textureAnimSpeed;								//テクスチャの移動速度
	bool m_bFlipX;												//テクスチャのX座標が反転しているかどうか
	bool m_bFlipY;												//テクスチャのY座標が反転しているかどうか
	D3DXCOLOR m_col;											//カーラー
	D3DXCOLOR m_VtxCol[4];										//頂点カーラー
	TextType m_textType;										//テクスチャの種類
	//================================================================================================================
	//================================================================================================================
	//静的変数
	static LPDIRECT3DTEXTURE9 m_pTextureAll[TEXTURE_TYPE_MAX];		//テクスチャへのポインタの配列
	//================================================================================================================

};



#endif