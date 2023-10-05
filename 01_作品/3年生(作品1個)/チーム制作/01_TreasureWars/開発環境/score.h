//==============================================================================
//
// score.h
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2重インクルードガード
//==============================================================================
#ifndef _SCORE_H_
#define	_SCORE_H_

//==============================================================================
// インクルード
//==============================================================================
#include "main.h"
#include "object.h"
#include "ui.h"

//==============================================================================
// 前方宣言
//==============================================================================
class CNumber;

//==============================================================================
// マクロ定義
//==============================================================================
#define SCORE_SIZE_X		 (20.0f)	//スコアのxサイズ
#define SCORE_SIZE_Y		 (35.0f)	//スコアのyサイズ
#define SCORE_DIGIT			 (8)		//スコアの桁数

//==============================================================================
// 数字を表示するクラス
//==============================================================================
class CScore : public CUi
{
public:
	CScore();					// コンストラクタ
	~CScore() override;					// デストラクタ

	HRESULT Init() override;				// 初期化処理
	void Uninit() override;				// 終了処理
	void Update() override;				// 更新処理
	void Draw() override;				// 描画処理

	void Roll();				// 数字の回転処理
	void DigitDraw();			// 桁の描画を設定する処理
	void NumberTex();			// 番号の描画

	void SetScore(int nScore);						//スコアの設定処理
	void AddScore(int nValue);						//スコアの加算処理
	void SubScore(int nSubtract);					//スコアの減算処理
	
	void SetPos(D3DXVECTOR3 pos) { ScorePos = pos; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetWidth(float width) { m_fWidth = width; }
	void SetRollSpeed(const int speed) { m_nRollSpeed = speed; }

	void SetFiver();	// フィーバー用の設定

	int GetScore(void) { return m_nScore; }			//スコアの取得処理

	static CScore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float width);			//生成

private:
	static const int Score_Roll_Speed = 20;			// スコアの回転速度

	D3DXVECTOR3 ScorePos;							// スコアの位置
	D3DXVECTOR3 m_Size;								// サイズ
	float m_fWidth;									// 数字同士の幅

	CNumber *m_apNumber[SCORE_DIGIT];				// スコアの数字(桁数)
	int m_nScore;									// 現在のスコア
	int m_nScoreOld;								// 前回用スコア
	int m_nRollSpeed;								// 回転速度
};

#endif
