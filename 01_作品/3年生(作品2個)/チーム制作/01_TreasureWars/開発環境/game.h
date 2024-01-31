//==============================================================================================
//
// ゲーム　　　game.h
// tutida ryousei
//
//==============================================================================================
#ifndef _GAME_H_
#define	_GAME_H_

#include"mode.h"
#include"charmanager.h"

// 前方宣言
class CMap;
class CCharacter;
class CScore;
class CTime;
class CShop;
class CObject2D;
class COreManager;
class CEnemyManager;
class CEnemySpawner;
class CScoreItemManager;
class CItemManager;
class CArea;
class CFever_Time;

class CGame : public CMode
{
public:
	static const int FeverStart_Count = 30;	// フィーバータイムが始まる時間

	CGame();
	~CGame();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void CharaRanking();		// キャラの順位処理
	void EnemySpawn();			// 敵キャラの生成処理
	void SetFeverTime();		// フィーバータイムの設定
	static void EnemySpawnerDeath();	// 敵キャラスポナーを消す処理

	static CGame *Create();

	static CTime *GetTime() { return m_pTime; }							// 時間
	static CShop *GetShop(const int index) { return m_pShop[index]; }	// 質屋
	static COreManager *GetOreManager() { return m_pOreManager; }		// 鉱石
	static CEnemyManager *GetEnemyManager() { return m_pEnemyManager; }	// 敵キャラマネージャー
	static CEnemySpawner *GetEnemySpawner() { return m_pEnemySpawner; }	// 敵キャラスポナー
	static CScoreItemManager *GetScoreItemManager() { return m_pScoreItemManager; }	// スコアアイテム
	static CItemManager *GetItemManager() { return m_pItemManager; }	// アイテム
	static CMap *GetMap() { return m_pMap; }							// マップ
	static CFever_Time *GetFever_Time() { return m_pFever_Time; }		// フィーバータイム
	static CArea *GetArea() { return m_pArea; }							// エリア

private:
	static const int EnemySpawner_Timer = 360;	// 敵キャラスポナーの再生成時間
	static const int GemCreate_Count = 5;	// 宝石を生成する間隔

	int m_nTop_Score;			// 一番高いスコア
	int m_nEnemySpawnerCounter;	// 敵キャラの再生成カウンター
	static CArea *m_pArea;
	static CMap *m_pMap;
	static CTime *m_pTime;
	static CShop *m_pShop[CCharManager::Max_Character];
	static COreManager *m_pOreManager;
	static CEnemyManager *m_pEnemyManager;
	static CEnemySpawner *m_pEnemySpawner;
	static CScoreItemManager *m_pScoreItemManager;
	static CItemManager *m_pItemManager;
	static CFever_Time *m_pFever_Time;

	int m_nStage_Number;
	int m_nTopCameraNum;
};

#endif // !_GAME_H_