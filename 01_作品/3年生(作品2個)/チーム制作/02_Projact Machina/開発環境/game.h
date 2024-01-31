//==============================================================================================
//
// ゲーム　　　game.h
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#ifndef _GAME_H_
#define	_GAME_H_

#include"mode.h"
#include"player_ui.h"
#include <vector>

// 前方宣言
class CPlayerManager;
class CEnemyManager;
class CDropManager;
class CCollision_Manager;
class CTime;
class CHalfSphere;
class CMeshField;
class CCharacter;
class CCollision;
class CFontString;
class CMenuWindow;
class CConfirmation_Window;
class CEnergy_Gauge;
class CPause;
class CPlayer_Parameter;
class CWeapon_Parameter;
class CMap_Object_Manager;
class CFog;
class CMap;
class CBoss;

class CGame : public CMode
{
	static const int MAX_FINISH_ROGO = 60;	// 終了ロゴまでの時間
	static const int MAX_END_TIMER = 240;	// ゲーム終了までの時間

public:
	CGame();
	~CGame() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	void GameEnd();
	void MenuWindow();

	static void SetPlayerUI(const int index, const int type);
	static void SetDrop_Parts(int num, D3DXVECTOR3 pos, bool random = false);
	static void SetGameEnd();
	static void AddDeathCount() { m_DeathCount++; }

	static CGame* Create();

	static const bool GetGameEnd() { return m_bGameEnd; }
	static void SetGameWindow(bool flag) { m_bGameWindow = flag; }
	static const bool GetGameWindow() { return m_bGameWindow; }
	static const bool GetSpawnBoss() { return m_bSpawn_Boss; }
	static int GetDeathCount() { return m_DeathCount; }
	static CPlayerManager* GetPlayerManager() { return m_pPlayerManager; }
	static CEnemyManager* GetEnemyManager() { return m_pEnemyManager; }
	static CDropManager* GetDropManager() { return m_pDropManager; }
	static CCollision_Manager* GetCollision_Manager() { return m_pCollision_Manager; }
	static CMeshField *GetMeshField() { return m_pMeshField; }
	static CConfirmation_Window* GetConfirmationWindow() { return m_pConfirmationWindow; }
	static CPause *GetPause() { return m_pPause; }
	static CPlayer_Parameter *GetPlayerParameter() { return m_pPlayer_Parameter; }
	static CWeapon_Parameter *GetWeaponParameter() { return m_pWeapon_Parameter; }
	static CMap_Object_Manager *GetMap_Object_Manager() { return m_pMap_Object_Manager; }
	static CMap *GetMap() { return m_pMap; }
	static CTime *GetTime() { return m_pTime; }

private:

	static bool m_bGameEnd;							// ゲーム終了判定
	static bool m_bGameWindow;
	static bool m_bInputFlag;
	static int m_DeathCount;
	static bool m_bSpawn_Boss;								// ボスが出現しているかどうか
	int m_nEndCounter;								// ゲーム終了までの時間
	int m_nMobSpawn_Counter;						// モブ出現間隔
	int m_nBossSpawn_Counter;						// ボス出現間隔
	CHalfSphere* m_pHalfSphere;

	static CPlayerUi* m_pPlayer_UI[CPlayerUi::UITYPE_MAX];
	CFog* m_pFog;
	
	static CConfirmation_Window* m_pConfirmationWindow;		// メニューウィンドウ
	static CPlayerManager* m_pPlayerManager;
	static CEnemyManager* m_pEnemyManager;
	static CDropManager* m_pDropManager;
	static CCollision_Manager* m_pCollision_Manager;
	static CMeshField *m_pMeshField;				// メッシュフィールド
	static CEnergy_Gauge* m_pEnergy_Gauge;			// エネルギーゲージ
	static CFontString* m_pFinishRogo;				// 終了ロゴ 
	static CPause *m_pPause;
	static CPlayer_Parameter *m_pPlayer_Parameter;	// プレイヤーパラメーター
	static CWeapon_Parameter *m_pWeapon_Parameter;	// 武器パラメーター
	static CMap_Object_Manager *m_pMap_Object_Manager;
	static CMap *m_pMap;
	static CTime* m_pTime;

};

#endif // !_GAME_H_