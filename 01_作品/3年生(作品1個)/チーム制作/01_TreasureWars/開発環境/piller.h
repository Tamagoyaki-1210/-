//==============================================================================
//
// piller.h	(柱のオブジェクト.h)
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2重インクルードガード
//==============================================================================
#ifndef _PILLER_H_
#define	_PILLER_H_

//==============================================================================
// インクルード
//==============================================================================
#include "objectX.h"

//==============================================================================
// 前方宣言
//==============================================================================
class CRanking;
class CCharacter;

//==============================================================================
// 柱クラス
//==============================================================================
class CPiller : public CObjectX
{
	static const float MOVING_PILLERSPEED;

public:
	CPiller();						// コンストラクタ
	~CPiller();						// デストラクタ

	HRESULT Init()override;			// 初期化処理
	void Uninit()override;
	void Update()override;			// 更新処理

	static void SetScore(int score, int index) { m_anScore[index] = score; }

	static CPiller *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, int index, float moving, int num);

private:
	CRanking *m_pRanking;				// リザルト構造体のインスタンス
	CCharacter *m_pResult_Char;

	static int m_anScore[4];		// スコア
	static int m_nScore;			// スコア
	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_rot;				// 回転
	D3DXVECTOR3 m_pos;				// 位置

	float m_fMaxMoving;				// 縦の最大値
	int m_no1;
	int m_num;
};

#endif

