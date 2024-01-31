//==============================================================================================
//
// アプリケーション　　　application.h
// tutida ryousei
//
//==============================================================================================
#ifndef _APPLICATION_H_
#define	_APPLICATION_H_

#include"renderer.h"
#include "sound.h"

//前方宣言
class CParticleManager;
class CStageManager;
class CTitle;
class CGame;
class CResult;
class CObject;
class CTexture;
class CSound;
class CLight;
class CCamera;
class CMode;
class CFade;
class CInput;
class CCharManager;
class CMenu;

#ifdef _DEBUG
class CDebugProc;
#endif

class CApplication
{
public:
	enum MODE
	{
		MODE_NONE = 0,
		MODE_TITLE,
		MODE_ENTRY,
		MODE_STAGE_SELECT,
		MODE_GAME,
		MODE_RESULT,
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

	static CRenderer *GetRenderer() { return m_pRenderer; }
	static MODE GetMode() { return m_mode; }
	static CTexture *GetTexture() { return m_pTexture; }
	static CSound *GetSound() { return m_pSound; }
	static CLight *GetLight() { return m_pLight; }
	static CCamera *GetCamera() { return m_pCamera; }
	static CMode *GetGameMode() { return m_pGameMode; }
	static CFade *GetFade() { return m_pFade; }
	static CParticleManager* GetParticleManager() { return m_pParticleManager; }
	static CStageManager* GetStageManager() { return m_pStageManager; }
	static CCharManager *GetCharManager() { return m_pCharManager; }	// キャラマネージャー
	static CMenu *GetMenu() { return m_pMenu; }

	static const bool GetGameStart() { return m_bGameStart; }
	static const bool GetPauce() { return m_bPauce; }

private:
	static CRenderer *m_pRenderer;
	static MODE m_mode;
	static CTexture *m_pTexture;
	static CSound *m_pSound;
	static CMode *m_pGameMode;
	static CLight *m_pLight;
	static CCamera *m_pCamera;
	static CFade *m_pFade;
	static CParticleManager* m_pParticleManager;
	static CStageManager* m_pStageManager;
	static CCharManager *m_pCharManager;
	static CMenu *m_pMenu;
	CInput *m_pInput;

#ifdef _DEBUG
	static CDebugProc *m_pDebugProc;
#endif
	bool m_bWireFrame;
	static bool m_bGameStart;
	static bool m_bPauce;
};

#endif // !_APPLICATION_H_
