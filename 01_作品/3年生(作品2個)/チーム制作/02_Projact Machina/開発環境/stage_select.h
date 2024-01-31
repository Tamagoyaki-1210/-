//==============================================================================================
//
// ステージセレクト　　　stage_select.h
// tutida ryousei
//
//==============================================================================================
#ifndef _STAGE_SERECT_H_
#define	_STAGE_SERECT_H_

#include"mode.h"

class CObject2D;

class CStage_Select : public CMode
{
public:
	CStage_Select();
	~CStage_Select();

	enum STAGE
	{
		STAGE_1 = 0,
		STAGE_2,
		STAGE_3,
		STAGE_TUTORIAL,
		STAGE_MAX,
		STAGE_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	static CStage_Select* Create();
	void Select();			// ステージ選択
	void Stage_Start();		// ステージ開始処理
	void Stage_Slide();		// ステージをスライドさせる
	void Select_Arrow();	// 選択の矢印
	void Min_Stage();		// ステージ(小)の設定
	void Arrow_Flash();		// 矢印の点滅

	static int GetStage_Number() { return m_nStage_Number; }

private:
	static const int m_nScreen_Width = 1280;
	static const int m_nScreen_Height = 720;

	CObject2D *m_pStage[2][STAGE_MAX];		// 選択するステージ
	CObject2D *m_pMin_Stage[2][STAGE_MAX];	// 選択するステージ(小)
	CObject2D *m_pFloor;					// 背景
	CObject2D *m_pSelect_Arrow[2];			// 矢印
	D3DXVECTOR3 m_StagePos[STAGE_MAX];		// ステージの位置
	D3DXVECTOR3 m_StagePosDest[STAGE_MAX];	// ステージの目的の位置
	D3DXVECTOR3 m_Select_Arrow[2];			// 矢印
	static int m_nStage_Number;				// ステージの番号
	int m_nStartCount;						// 始まるまでのカウント
	bool m_bSelect;							// ステージを決めたか
	float m_fAlpha;							// アルファ値
	bool m_Flash;							// 点滅してるか
	float m_fRed;
	bool m_bMin;
};

#endif // !_STAGE_SERECT_H_
