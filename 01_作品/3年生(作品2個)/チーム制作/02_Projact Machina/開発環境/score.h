//==============================================
//
// score.h (スコア.h)
// Author : Saito Shian
//
//==============================================
#ifndef  _SCORE_H_				//このマクロ定義がされてなかったら
#define  _SCORE_H_				//二重インクルード防止のマクロ定義

//==============================================
// インクルード
//==============================================
#include "main.h"
#include "object.h"

//==============================================
// 前方宣言
//==============================================
class CNumber;

//==============================================
// スコアクラス
//==============================================
class CScore : public CObject
{
public:
	static const int MAX_DIGIT = 10;		// 最大桁数

	//****************
	// publicな関数
	//****************
	CScore();								// スコアのコンストラクタ
	~CScore() override;						// スコアのデストラクタ

	HRESULT Init() override;				// 初期化処理
	void Uninit() override;					// 終了処理
	void Update() override;					// 更新処理
	void Draw() override;					// 描画処理

	void SetScore(__int64 nScore);				//スコアの設定処理
	void AddScore(__int64 nValue);				//スコアの加算処理

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	static CScore* Create(D3DXVECTOR3 pos);				// 生成

private:
	//****************
	// privateな変数
	//****************
	CNumber* m_apNumber[MAX_DIGIT];			//スコアの数字(桁数)
	D3DXVECTOR3 m_pos;						// 位置
	__int64 m_aPosTexU[MAX_DIGIT];				//各桁のスコアを格納
	__int64 m_nScore;							//スコア
	
};
#endif
