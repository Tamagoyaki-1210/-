//==============================================
//
// number.h (ナンバー.h)
// Author : Saito Shian
//
//==============================================
#ifndef  _NUMBER_H_				//このマクロ定義がされてなかったら
#define  _NUMBER_H_				//二重インクルード防止のマクロ定義

//==============================================
// インクルード
//==============================================
#include "main.h"
#include "object2D.h"

//==============================================
// ナンバークラス
//==============================================
class CNumber : public CObject2D
{
public:

	//****************
	// publicな関数
	//****************
	CNumber(const PRIORITY priority);		// ナンバーのコンストラクタ
	~CNumber() override;					// ナンバーのデストラクタ
	 
	HRESULT Init() override;				// 初期化処理
	void Uninit() override;					// 終了処理
	void Update() override;					// 更新処理
	void Draw() override;					// 描画処理

	static CNumber* Create(const D3DXVECTOR3 &pos, float xsize, float ysize, const PRIORITY priority);	// 生成

private:
	//****************
	// privateな変数
	//****************
};
#endif
