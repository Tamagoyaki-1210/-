//==============================================================================================
//
// エントリー　　　entry.h
// tutida ryousei
//
//==============================================================================================
#ifndef _ENTRY_H_
#define	_ENTRY_H_

#include"mode.h"
#include "charmanager.h"

class CCharacter;
class CFile;
class CObject2D;
class CFontString;

class CEntry : public CMode
{
public:
	CEntry();
	~CEntry();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CEntry *Create();

	void TutorialCol();			// チュートリアルの当たり判定
	void Tutorial();			// チュートリアルの処理
	void TutorialStart();		// チュートリアルの開始処理
	void TutorialEnd();			// チュートリアルの終了処理
	void TutorialSelect();		// チュートリアルの選択処理
	void TVDisplayStart();		// TV画面の表示の開始処理
	void TVDisplayEnd();		// TV画面の表示の終了処理
	void PlayerIn(int index);	// プレイヤーの入場
	void Act_Limit();			// 行動制限

	void SetEntryUX(const bool use);	// エントリー画面の全てのUXの設定処理

	static bool GetEntry(int index) { return m_bEntry[index]; }
	static bool GetIn(int index) { return m_bIn[index]; }
	static bool GetTutorial() { return m_bTutorial; }

private:
	static const D3DXVECTOR3 TVSizeMax;							// TV画面の最大サイズ

	static const float TVDisplaySpeed;							// TV画面の表示速度
	static const float TVStartAnchorAttenuation;				// TV開始アンカー減衰
	static const float TVEndAnchorAttenuation;					// TV終了アンカー減衰
	static const int MaxTutorialPage = 3;						// チュートリアル3ページ
	static const int TVDisplayEndTimer = 20;					// TV画面の表示の終了時間

	static bool m_bEntry[CCharManager::Max_Character];			// エントリーしたか
	static bool m_bIn[CCharManager::Max_Character];				// 入場したか
	D3DXVECTOR3 m_CharPos[CCharManager::Max_Character];			// キャラクターの位置
	D3DXVECTOR3 m_CharPosDest[CCharManager::Max_Character];		// キャラクターの目的の位置
	CCharacter* m_pChara[CCharManager::Max_Character];			// キャラクターの情報

	static bool m_bTutorial;									// チュートリアル判定
	bool m_bTutorialStart;										// チュートリアルの開始判定
	bool m_bTutorialEnd;										// テレビ画面の表示の終了判定
	bool m_bTVDisplayStart;										// テレビ画面の表示の開始判定
	bool m_bTVDisplayEnd;										// テレビ画面の表示の終了判定
	CFontString* m_apTutorialFont[2];							// チュートリアル文
	CFontString* m_pGameStartFont;								// ゲームスタート文
	CObject2D* m_apPlayerNumber[CCharManager::Max_Character];	// プレイヤー番号
	CObject2D* m_apABotton[CCharManager::Max_Character];		// プレイヤー人数分のAボタン
	CFontString* m_apOKString[CCharManager::Max_Character];		// プレイヤー人数分のOK
	CObject2D* m_pABotton;										// Aボタン
	CObject2D* m_pTutorial;										// チュートリアル画面
	CObject2D* m_apArrow[2];									// 矢印2つ

	int m_nSellect;												// 現在の選択画面
	int m_nDisplayEndCounter;									// テレビ画面の表示の終了時間
	int m_nTutorialInitiative;									// チュートリアルの主導権
};

#endif // !_ENTRY_H_
