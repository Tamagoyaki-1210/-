//==============================================================================================
//
// �A�v���P�[�V�����@�@�@application.h
// tutida ryousei
// tanimoto kosuke
//
//==============================================================================================
#ifndef _APPLICATION_H_
#define	_APPLICATION_H_

#include "renderer.h"
#include "sound.h"
#include "texture.h"

//�O���錾
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

	// ���[�h�̎��
	enum MODE
	{
		MODE_NONE = 0,
		MODE_TITLE,			// �^�C�g��
		MODE_CHAR_SELECT,	// �L�����N�^�[�Z���N�g
		MODE_STAGE_SELECT,	// �X�e�[�W�Z���N�g
		MODE_TUTORIAL,		// �`���[�g���A��
		MODE_GAME,			// �Q�[��
		MODE_RESULT,		// ���U���g
		MODE_MAX,
	};

	CApplication();
	~CApplication();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	void Draw();

	static void SetMode(MODE mode);		// ���[�h�̐ݒ�
	static void SetGameStart(const bool game_start) { m_bGameStart = game_start; }
	static void SetPauce(const bool pauce) { m_bPauce = pauce; }
	static void SetPlayerJobIndex(const int index, const int parts) { m_nPlayerJobIndex[parts] = index; }
	static void SetPlayerWeaponIndex(const int index) { m_nPlayerWeaponIndex = index; }

	static HWND GetWindow() { return Window; }			// �E�B���h�E
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
	static HWND Window;				// �E�B���h�E

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

	static MODE m_modeType;	// ���݂̃��[�h

	bool m_bWireFrame;
	static bool m_bGameStart;
	static bool m_bPauce;
	static int m_nPlayerJobIndex[3];	// �v���C���[�̃W���u�ԍ�
	static int m_nPlayerWeaponIndex;	// �v���C���[�̕���ԍ�

	static bool m_nSkill;
	static int m_nDestroyTime;		// �ŒZ�œ|��������
	static int m_nDamage;			// �ς����_���[�W��
	static int m_nDefeats;			// �|�����G�̐�
	static int m_nTotalDamage;		// �_���[�W�̑���
};

#endif // !_APPLICATION_H_
