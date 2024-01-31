//==============================================================================================
//
// タイトル　　　title.h
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#ifndef _TITLE_H_
#define	_TITLE_H_

#include"mode.h"

class CHalfSphere;
class CFontString;
class CMenuWindow;
class CObject2D;
class CObjectX;

class CTitle : public CMode
{
public:
	static const int TITLE_LOGO_MAX = 11;
	static const int TITLE_PROJECT_LOGO_MAX = 3;
	static const int TITLE_LOGO_GEAR_MAX = 7;
	static const int TITLE_GEAR_MAX = 3;
	static const int GEAR_AFTERIMAGE = 3;
	static const int GEAR_AFTERIMAGE_MAX = 60;
	static const int GEAR_AFTERIMAGE_UPDATE = 1;
	static const D3DXVECTOR3 TITLE_LOGO_POS;
	static const D3DXVECTOR3 TITLE_LOGO_GEAR_POS;
	static const float TITLE_LOGO_GEAR_SIZE;

	static const float TITLE_PROJECT_LOGO_GEAR_INIT_ROT;
	static const float TITLE_PROJ_LOGO_GEAR_INIT_ROT;
	static const float TITLE_ECT_LOGO_GEAR_INIT_ROT;

	static const D3DXVECTOR3 TITLE_3D_MODEL_POS;
	static const float GEAR_SPEED;

	CTitle();
	~CTitle();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	static void SetWindow(bool flag) { m_bWindow = flag; }
	static void SetUninitWindow(bool flag) { m_bWindowUninit = flag; }
	static bool GetUninitWindow() { return m_bWindowUninit; }
	static CMenuWindow* GetMenuWindow() { return m_pMenuWindow; }
	static void UninitMenuWindow();
	static CTitle* Create();

private:
	static CMenuWindow* m_pMenuWindow;
	CHalfSphere* m_pHalf;
	CFontString* m_pFomntString[2];

	static bool m_bWindow;		// ウィンドウ使用状態
	static bool m_bWindowUninit;
	bool m_bKeyFlag;
	CObject2D *m_pTitle[TITLE_LOGO_MAX];
	CObject2D *m_pGearLogo[TITLE_LOGO_GEAR_MAX];
	D3DXVECTOR3 m_ProjectLogoPos[TITLE_PROJECT_LOGO_MAX];
	D3DXVECTOR2 m_ProjectLogoRot[TITLE_PROJECT_LOGO_MAX];
	D3DXVECTOR2 m_ProjLogoRot;
	D3DXVECTOR2 m_EctLogoRot;
	CObjectX *m_pGear[TITLE_GEAR_MAX];
	D3DXVECTOR3 m_GearRotAfterimage[TITLE_GEAR_MAX][GEAR_AFTERIMAGE_MAX];
	CObjectX *m_pGearAfterimage[TITLE_GEAR_MAX][GEAR_AFTERIMAGE];
	int m_nGearIndex[TITLE_GEAR_MAX];
	int m_nGearAfterimageCnt;
	CObjectX *m_pEarth;
	CObjectX *m_pFlare;
};

#endif // !_TITLE_H_
