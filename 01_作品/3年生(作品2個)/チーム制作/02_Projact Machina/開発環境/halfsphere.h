//==============================================
//
// ハーフスフィア.h (halfsphere.h)
// Author : Saito Shian
//
//==============================================
#ifndef  _HALFSPHERE_H_				//このマクロ定義がされてなかったら
#define  _HALFSPHERE_H_				//二重インクルード防止のマクロ定義

//==============================================
// インクルード
//==============================================
#include "object.h"
#include <vector>

//==============================================
//マクロ定義
//==============================================
#define HALFSPHEAR_X_BLOCK	(25)	//X方向のブロック数
#define HALFSPHEAR_Z_BLOCK	(25)	//Z方向のブロック数

#define MAX_SPHERE	(2)

#define HALFSPHEAR_VERTEX_NUM	((HALFSPHEAR_X_BLOCK + 1) * (HALFSPHEAR_Z_BLOCK + 1))
#define HALFSPHEAR_INDEXNUM		((HALFSPHEAR_X_BLOCK + 1) * 2 * HALFSPHEAR_Z_BLOCK + (HALFSPHEAR_Z_BLOCK - 1) * 2)
#define HALFSPHEAR_PRIMITIVE_NUM	(HALFSPHEAR_X_BLOCK * (HALFSPHEAR_Z_BLOCK * 2) + (HALFSPHEAR_Z_BLOCK - 1) * 4)

//==============================================
// ハーフスフィアクラス
//==============================================
class CHalfSphere : public CObject
{
public:

	// 3D頂点フォーマット
	const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	//****************
	// 列挙型
	//****************
	enum SPHERE_TEX
	{
		SPHERE_UP = 0,
		SPHERE_DOWN,
		SPHERE_MAX
	};

	//****************
	// publicな関数
	//****************
	CHalfSphere();				// コンストラクタ
	~CHalfSphere() override;							// デストラクタ

	HRESULT Init(void) override;						// 初期化処理
	void Uninit(void) override;							// 終了処理
	void Update(void) override;							// 更新処理
	void Draw(void) override;							// 描画処理

	void SetPos(const D3DXVECTOR3 pos);					// 位置の設定処理
	void SetSize(const D3DXVECTOR3 size);				// サイズの設定
	void SetRot(const D3DXVECTOR3 rot);					// 向きの設定
	void SetTex(const SPHERE_TEX tex);					// 

	const D3DXVECTOR2 GetSize(void);					// サイズの取得処理
	const D3DXVECTOR3 GetPos(void);						// 位置の取得処理
	const D3DXVECTOR3 GetRot(void);

	void AddPos(D3DXVECTOR3 pos) { m_pos += pos; }
	void AddRot(D3DXVECTOR3 rot) { m_rot += rot; }

	static CHalfSphere* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, SPHERE_TEX tex);		// 生成処理

	void SetAnimation(D3DXVECTOR2 animationMove) { m_bAnimation = true; m_AnimationMove = animationMove; }
	void SetRotMove(float fRotMove) { m_bRot = true; m_fRotMove = fRotMove; }

	void BindTexture(LPDIRECT3DTEXTURE9	pTexture);		// テクスチャを代入
	void LoadTexture(const char *aFileName);			// テクスチャの読み込み

private:
	//****************
	// メンバー変数群
	//****************
	struct VERTEX_3D
	{
		D3DXVECTOR3 pos;		//頂点座標
		D3DXVECTOR3 nor;		//法線ベクトル
		D3DCOLOR col;			//頂点カラー
		D3DXVECTOR2 tex;		//テクスチャ座標
	};

	LPDIRECT3DTEXTURE9 m_pTexture;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		// インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;						// 位置
	D3DXVECTOR3 m_size;						// サイズ
	D3DXVECTOR3 m_rot;						// 向き
	D3DXMATRIX m_mtxWorld;					// ワールドマトリックス
	D3DXVECTOR2 m_AnimationMove;
	D3DXVECTOR2 m_Animation;
	bool m_bAnimation;
	bool m_bRot;
	float m_fRotMove;
	float m_fRot;

	SPHERE_TEX m_tex;						// テクスチャ
};

#endif
