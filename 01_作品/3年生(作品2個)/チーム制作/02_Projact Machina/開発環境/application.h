//==============================================================================================
//
// アプリケーション　　　application.h
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#ifndef _APPLICATION_H_
#define	_APPLICATION_H_

#include "renderer.h"
#include "sound.h"
#include "texture.h"

//前方宣言
class CObject;
class CTexture;
class CModel;
class CMotion;
class CSound;
class CLight;
class CCamera;
class CFade;
class CInput;
class CMenu;
class CMode;
class CParticleManager;
class CMenu_Window;
class CParts_File;
class ConnectManager;
class CChar_Select;

#ifdef _DEBUG
class CDebugProc;
#endif

class CApplication
{
public:

	// モードの種類
	enum MODE
	{
		MODE_NONE = 0,
		MODE_TITLE,			// タイトル
		MODE_CHAR_SELECT,	// キャラクターセレクト
		MODE_STAGE_SELECT,	// ステージセレクト
		MODE_TUTORIAL,		// チュートリアル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_MAX,
	};

	CApplication();
	~CApplication();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	static void SetMode(MODE mode);		// モードの設定
	static void SetGameStart(const bool game_start) { m_bGameStart = game_start; }
	static void SetPauce(const bool pauce) { m_bPauce = pauce; }
	static void SetPlayerJobIndex(const int index, const int parts) { m_nPlayerJobIndex[parts] = index; }
	static void SetPlayerWeaponIndex(const int index) { m_nPlayerWeaponIndex = index; }

	static HWND GetWindow() { return Window; }			// ウィンドウ
	static CRenderer* GetRenderer() { return m_pRenderer; }
	static CTexture* GetTexture() { return m_pTexture; }
	static CModel* GetModel() { return m_pModel; }
	static CMotion* GetMotion() { return m_pMotion; }
	static CSound* GetSound() { return m_pSound; }
	static CLight* GetLight() { return m_pLight; }
	static CCamera* GetCamera() { return m_pCamera; }
	static CMode* GetGameMode() { return m_pGameMode; }
	static CFade* GetFade() { return m_pFade; }
	static CMenu* GetMenu() { return m_pMenu; }
	static CParts_File* GetPartsFile() { return m_pPartsFile; }
	static CParticleManager* GetParticleManager() { return m_pParticleManager; }
	static CChar_Select* GetCharSelect() { return m_pCharSelect; }

	static MODE GetModeType() { return m_modeType; }

	static const bool GetGameStart() { return m_bGameStart; }
	static const bool GetPauce() { return m_bPauce; }
	static const int GetPlayerJobIndex(const int parts) { return m_nPlayerJobIndex[parts]; }
	static const int GetPlayerWeaponIndex() { return m_nPlayerWeaponIndex; }
	static bool GetSkillType() { return m_nSkill; }
	static int GetDestroyTime() { return m_nDestroyTime; }
	static int GetTotalDamage() { return m_nTotalDamage; }

	static ConnectManager* GetClient() { return m_pClient; }
	static void SetClient(ConnectManager* Client) { m_pClient = Client; }
private:
	static HWND Window;				// ウィンドウ

	static CRenderer* m_pRenderer;
	static CTexture* m_pTexture;
	static CModel* m_pModel;
	static CMotion* m_pMotion;
	static CSound* m_pSound;
	static CMode* m_pGameMode;
	static CLight* m_pLight;
	static CCamera* m_pCamera;
	static CFade* m_pFade;
	static CMenu* m_pMenu;
	static CParts_File* m_pPartsFile;
	static CParticleManager* m_pParticleManager;
	static ConnectManager* m_pClient;
	static CChar_Select* m_pCharSelect;
	CInput* m_pInput;

#ifdef _DEBUG
	static CDebugProc* m_pDebugProc;
#endif

	static MODE m_modeType;	// 現在のモード

	bool m_bWireFrame;
	static bool m_bGameStart;
	static bool m_bPauce;
	static int m_nPlayerJobIndex[3];	// プレイヤーのジョブ番号
	static int m_nPlayerWeaponIndex;	// プレイヤーの武器番号

	static bool m_nSkill;
	static int m_nDestroyTime;		// 最短で倒した時間
	static int m_nDamage;			// 耐えたダメージ数
	static int m_nDefeats;			// 倒した敵の数
	static int m_nTotalDamage;		// ダメージの総数
};

#endif // !_APPLICATION_H_
