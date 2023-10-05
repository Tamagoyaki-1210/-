//=============================================================================
//
// スコア
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "Object2D.h"

//---------------------------
//前方宣言
//---------------------------
class CNumber;

//---------------------------
//マクロ関数
//---------------------------
#define SCORE_WIDTH		(240)
#define SCORE_HEIGHT	(80)

//---------------------------
//クラス
//---------------------------
class CScore : public CObject2D
{
public:
	CScore();
	~CScore() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetScore();
	void AddScore(int nValue);
	static CScore *GetScore() { return m_pScore; }
	static CScore *Create(D3DXVECTOR3 pos);
	void Save(void);
private:
	int m_nScore;		//スコア
	float m_fWidth;		//長さ横
	float m_fHeight;	//長さ縦
	static CScore *m_pScore;
	CNumber *m_apNumber[MAX_DIGIT];
};

#endif // !_SCORE_H_
