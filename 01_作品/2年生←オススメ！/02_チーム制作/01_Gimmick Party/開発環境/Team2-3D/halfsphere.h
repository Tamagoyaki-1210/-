//-------------------------------------
//
//ハーフスフィアヘッダー[halfsphere.h]
//Author:梶田大夢
//
//-------------------------------------
#ifndef  _HALFSPHERE_H_				//このマクロ定義がされてなかったら
#define  _HALFSPHERE_H_				//二重インクルード防止のマクロ定義

#include "object.h"
#include <vector>

//---------------------------
//マクロ定義
//---------------------------
#define HALFSPHEAR_X_BLOCK	(15)							//X方向のブロック数
#define HALFSPHEAR_Z_BLOCK	(15)							//Z方向のブロック数

#define MAX_SPHERE	(2)

#define HALFSPHEAR_VERTEX_NUM	((HALFSPHEAR_X_BLOCK + 1) * (HALFSPHEAR_Z_BLOCK + 1))
#define HALFSPHEAR_INDEXNUM		((HALFSPHEAR_X_BLOCK + 1) * 2 * HALFSPHEAR_Z_BLOCK + (HALFSPHEAR_Z_BLOCK - 1) * 2)
#define HALFSPHEAR_PRIMITIVE_NUM	(HALFSPHEAR_X_BLOCK * (HALFSPHEAR_Z_BLOCK * 2) + (HALFSPHEAR_Z_BLOCK - 1) * 4)

//---------------------------
//クラス宣言
//---------------------------
class CHalfSphere : public CObject
{
public:
	enum SPHERE_TEX
	{
		SPHERE_UP = 0,
		SPHERE_DOWN,
		SPHERE_MAX
	};

	CHalfSphere();
	CHalfSphere(const int nPriority);					//コンストラクタ
	~CHalfSphere() override;

	HRESULT Init(void) override;	//初期化処理
	void Uninit(void) override;	//終了処理
	void Update(void) override;	//更新処理
	void Draw(void) override;	//描画処理

	void SetPos(const D3DXVECTOR3 pos) override;		//位置の設定処理
	void SetSize(const D3DXVECTOR3 size);				//サイズの設定
	void SetRot(const D3DXVECTOR3 rot);
	void SetTex(const SPHERE_TEX tex);

	const D3DXVECTOR2 GetSize(void) override;			//サイズの取得処理
	const D3DXVECTOR3 GetPos(void) override;			//位置の取得処理
	const D3DXVECTOR3 GetRot(void);

	static CHalfSphere* Create(D3DXVECTOR3 pos,D3DXVECTOR3 size,D3DXVECTOR3 rot, SPHERE_TEX tex);		//生成処理

	void BindTexture(LPDIRECT3DTEXTURE9	pTexture);			//テクスチャを代入
	void LoadTexture(const char *aFileName);				//テクスチャの読み込み

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//インデックスバッファへのポインタ
	D3DXVECTOR3 m_pos;						//位置
	D3DXVECTOR3 m_size;						//サイズ
	D3DXVECTOR3 m_rot;						//向き
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

	SPHERE_TEX m_tex;
};

#endif
