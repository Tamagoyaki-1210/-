//==============================================================================================
//
// ゲーム　　　game.cpp
// tutida ryousei
//
//==============================================================================================
#include "game.h"
#include "camera.h"
#include "fade.h"
#include "input.h"
#include "fontString.h"
#include "player_manager.h"
#include "enemy_manager.h"
#include "drop_manager.h"
#include "collision_manager.h"
#include "mob.h"
#include "boss.h"
#include "score.h"
#include "time.h"
#include "halfsphere.h"
#include"meshfield.h"
#include"collision.h"
#include "locus.h"
#include "object2D.h"
#include "confirmation_window.h"
#include "menu.h"
#include "utility.h"
#include "parts.h"
#include "drop_weapon.h"
#include "player_ui.h"
#include"debugProc.h"
#include"pause.h"
#include "parts_file.h"
#include "motion.h"
#include "map.h"
#include "weapon.h"
#include "connect.h"
#include "player_parameter.h"
#include "weapon_parameter.h"
#include "map_object_manager.h"
#include "fog.h"
#include "restrictions.h"
#include "sound.h"

//==============================================================================================
// 静的メンバ変数宣言
//==============================================================================================
CPlayerUi* CGame::m_pPlayer_UI[CPlayerUi::UITYPE_MAX] = {};
CConfirmation_Window* CGame::m_pConfirmationWindow = nullptr;
CPlayerManager* CGame::m_pPlayerManager = nullptr;
CEnemyManager* CGame::m_pEnemyManager = nullptr;
CDropManager* CGame::m_pDropManager = nullptr;
CCollision_Manager* CGame::m_pCollision_Manager = nullptr;
CMeshField *CGame::m_pMeshField = nullptr;
bool CGame::m_bGameEnd = false;
bool CGame::m_bGameWindow = false;
bool CGame::m_bInputFlag = false;
bool CGame::m_bSpawn_Boss = false;
int CGame::m_DeathCount = 0;
CFontString* CGame::m_pFinishRogo = nullptr;
CPause *CGame::m_pPause = nullptr;
CPlayer_Parameter *CGame::m_pPlayer_Parameter = nullptr;
CWeapon_Parameter *CGame::m_pWeapon_Parameter = nullptr;
CMap_Object_Manager *CGame::m_pMap_Object_Manager = nullptr;
CMap *CGame::m_pMap = nullptr;
CTime *CGame::m_pTime = nullptr;

//==============================================================================================
// コンストラクタ
//==============================================================================================
CGame::CGame()
{
	
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CGame::~CGame()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CGame::Init()
{
	// カメラのポインタ
	CCamera* pCamera = CApplication::GetCamera();

	if (!pCamera->GetOpening())
	{
		// 視点、注視点の設定
		pCamera->SetPosV({ 0.0f, 225.0f, -450.0f });
		pCamera->SetPosR({ 0.0f, 112.5f, 450.0f });
	}
	
	// 全てのモデルパーツの読み込み
	CApplication::GetPartsFile()->LoadAllFile();

	// 全てのモデルパーツの読み込み
	CApplication::GetMotion()->LoadAllFile();

	// プレイヤーパラメーターの生成
	m_pPlayer_Parameter = new CPlayer_Parameter;
	m_pPlayer_Parameter->Init();

	// 武器パラメーターの生成
	m_pWeapon_Parameter = new CWeapon_Parameter;
	m_pWeapon_Parameter->Init();

	// プレイヤーUIの生成
	//m_pPlayer_UI[CPlayerUi::UITYPE_SUPPORT] = CPlayerUi::Create(D3DXVECTOR3(1200.0f, 50.0f, 0.0f), D3DXVECTOR2(100.0f, 75.0f), CPlayerUi::UITYPE_SUPPORT, CObject::PRIORITY_CENTER);
	//m_pPlayer_UI[CPlayerUi::UITYPE_ATTACK] = CPlayerUi::Create(D3DXVECTOR3(100.0f, 50.0f, 0.0f), D3DXVECTOR2(100.0f, 75.0f), CPlayerUi::UITYPE_ATTACK, CObject::PRIORITY_CENTER);
	m_pPlayer_UI[CPlayerUi::UITYPE_WEAPON] = CPlayerUi::Create(D3DXVECTOR3(1200.0f, 60.0f, 0.0f), D3DXVECTOR2(100.0f, 85.0f), CPlayerUi::UITYPE_WEAPON, CObject::PRIORITY_CENTER);

	m_pPlayerManager = CPlayerManager::Create();	// プレイヤーマネージャーの生成
	m_pEnemyManager = new CEnemyManager;			// 敵キャラマネージャーの生成
	m_pDropManager = new CDropManager;				// 落とし物マネージャーの生成
	m_pCollision_Manager = new CCollision_Manager;	// 当たり判定マネージャーの生成
	m_pMap_Object_Manager = new CMap_Object_Manager;
	m_pMap = new CMap;

	// 全てのモデルパーツの読み込み
	CDrop_Weapon* pWeaponDummer = new CDrop_Weapon;
	pWeaponDummer->LoadAllFile();
	pWeaponDummer->Uninit();

	// プレイヤーの生成(テスト)
	m_pPlayerManager->SetPlayer({ 0.0f, 0.0f, 0.0f }, CPlayerManager::TYPE_PC, 0);

	for (int nCnt = 0; nCnt < 20; nCnt++)
	{
		// モブキャラの生成
		CMob::Create({ utility::Random<float>(5000.0f, -5000.0f), utility::Random<float>(600.0f, 200.0f), utility::Random<float>(5000.0f, -5000.0f) });
	}

	// 武器、パーツのドロップ
	SetDrop_Parts(20, { 0.0f,0.0f,0.0f }, true);

	// タイムの生成
	m_pTime = CTime::Create(D3DXVECTOR3(540.0f,-50.0f,0.0f));

	// ハーフスフィアの生成
	m_pHalfSphere = CHalfSphere::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20000.0f, 20000.0f, 20000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CHalfSphere::SPHERE_UP);
	m_pHalfSphere->LoadTexture("Data/texture/sky000.jpg");

	// メッシュフィールドの生成
	m_pMeshField = CMeshField::Create({ 0.0f, 0.0f, 0.0f }, 10, 10, 4000.0f);

	// ポーズ画面
	m_pPause = CPause::Create();

	// フォグの設定
	CFog::SetFog(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	m_nEndCounter = 0;
	m_bInputFlag = false;

	// マップ生成
	m_pMap->ReadMap("Data/text/map.txt");

	// 移動制限
	CRestrictions::Create({ 0.0f,0.0f,0.0f }, 5000, { 1000.0f,200.0f }, { 1.0f,1.0f,1.0f,1.0f });

	CApplication::GetSound()->Play(CSound::SOUND_LABEL_BGM_GAME);

	// 討伐数をリセット
	m_DeathCount = 0;

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CGame::Uninit()
{
	// プレイヤーパラメーターの破棄
	if (m_pPlayer_Parameter != nullptr)
	{
		m_pPlayer_Parameter->Uninit();
		delete m_pPlayer_Parameter;
		m_pPlayer_Parameter = nullptr;
	}

	// 武器パラメーターの破棄
	if (m_pWeapon_Parameter != nullptr)
	{
		m_pWeapon_Parameter->Uninit();
		delete m_pWeapon_Parameter;
		m_pWeapon_Parameter = nullptr;
	}

	// プレイヤーマネージャーの破棄
	if (m_pPlayerManager != nullptr)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = nullptr;
	}

	// 敵キャラマネージャーの破棄
	if (m_pEnemyManager != nullptr)
	{
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}

	// 落とし物マネージャーの破棄
	if (m_pDropManager != nullptr)
	{
		delete m_pDropManager;
		m_pDropManager = nullptr;
	}

	// コリジョンマネージャーの破棄
	if (m_pCollision_Manager != nullptr)
	{
		// 全ての当たり判定の解放処理
		m_pCollision_Manager->ReleaseAllCollision();
		delete m_pCollision_Manager;
		m_pCollision_Manager = nullptr;
	}

	// タイマーの終了処理
	if (m_pTime != nullptr)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = nullptr;
	}

	// 終了ロゴの終了
	if (m_pFinishRogo != nullptr)
	{
		m_pFinishRogo->Uninit();
		m_pFinishRogo = nullptr;
	}

	// メニューウィンドウの終了処理
	if (m_pConfirmationWindow != nullptr)
	{
		m_pConfirmationWindow->Uninit();
		delete m_pConfirmationWindow;
		m_pConfirmationWindow = nullptr;
	}

	// プレイヤーUIの終了処理
	for (int nCnt = 0; nCnt < CPlayerUi::UITYPE_MAX; nCnt++)
	{
		if (m_pPlayer_UI[nCnt] != nullptr)
		{
			m_pPlayer_UI[nCnt]->Uninit();
			m_pPlayer_UI[nCnt] = nullptr;
		}
	}

	// ポーズの終了処理
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();
		m_pPause = nullptr;
	}

	//マップオブジェクトの破棄
	if (m_pMap_Object_Manager != nullptr)
	{
		delete m_pMap_Object_Manager;
		m_pMap_Object_Manager = nullptr;
	}

	// マップ読み込みの破棄
	if (m_pMap != nullptr)
	{
		delete m_pMap;
		m_pMap = nullptr;
	}

	CApplication::GetSound()->StopAll();

	// フォグの終了処理
	CFog::DestroyFog();
	m_bGameEnd = false;	// ゲーム終了判定を偽にする

	m_bSpawn_Boss = false;
}

//==============================================================================================
//更新処理
//==============================================================================================
void CGame::Update()
{
	// メニューウィンドウ処理
	MenuWindow();

	// ゲーム終了判定が真の場合
	if (m_bGameEnd == true)
	{
		GameEnd();	// ゲーム終了処理
	}
	else
	{
		// タイマーの更新
		if (m_pTime != nullptr)
			m_pTime->Update();

		// デバッグ専用コマンド
#ifdef _DEBUG
		CInput* pInput = CInput::GetKey();
		if (pInput->Trigger(DIK_F5))
		{
			SetGameEnd();
			CFade::SetFade(CApplication::MODE_RESULT, 0.05f);
		}
		// 左Shiftキーを押したままの場合
		if (pInput->Press(DIK_LSHIFT))
		{
			int nKey = -1;
			if (pInput->Trigger(DIK_1))
			{
				nKey = 0;
			}
			if (pInput->Trigger(DIK_2))
			{
				nKey = 1;
			}
			if (pInput->Trigger(DIK_3))
			{
				nKey = 2;
			}
			if (pInput->Trigger(DIK_4))
			{
				nKey = 3;
			}
		}
		// LShiftキー無しの場合
		else
		{
			// パーツ変更処理
			if (pInput->Press(DIK_C))
			{
				int nKey = -1;
				if (pInput->Trigger(DIK_1))
				{
					nKey = 0;
				}
				if (pInput->Trigger(DIK_2))
				{
					nKey = 1;
				}
				if (pInput->Trigger(DIK_3))
				{
					nKey = 2;
				}
				if (pInput->Trigger(DIK_4))
				{
					nKey = 3;
				}
				if (pInput->Trigger(DIK_5))
				{
					nKey = 4;
				}
				if (pInput->Trigger(DIK_6))
				{
					nKey = 5;
				}

				if (nKey >= 0)
				{
					CPlayer* pPlayer = m_pPlayerManager->GetPlayer(0);
					pPlayer->GetParts(CPlayer::PARTS_BODY)->SetParts(CParts_File::PARTS_PLAYER_BODY_1 + nKey);
				}
			}

			// スコアの加算
			if (pInput->Trigger(DIK_0))
			{
				for (auto pEnemy : m_pEnemyManager->GetAllEnemy())
				{
					// 50ダメージ
					pEnemy->Damage(50);
				}
			}

			if (pInput->Trigger(DIK_U))
			{
				CLocus::Create(D3DXVECTOR3(0.0f,200.0f,0.0f),1000.0f,10);
			}
		}
#endif

		if (m_bGameEnd == false)
		{
			// ボスが出るまではチュートリアル
			if (m_bSpawn_Boss == false)
			{
				// 計10体倒すと出現
				if (m_DeathCount > 10)
				{
					if(m_nBossSpawn_Counter == 0)
					{ 
						// BGMの停止
						CApplication::GetSound()->StopAllBGM();

						// 赤いフォグを発生させる
						CFog::SetFog({ 1.0f, 0.0f, 0.0f, 1.0f }, 0.000075f);
					}

					// 180f後にボス出現
					if (m_nBossSpawn_Counter == 180)
					{
						CSound* pSound = CApplication::GetSound();

						// ボス出現
						m_bSpawn_Boss = true;

						// ボスキャラの生成
						CBoss::Create({ 0.0f, 5000.0f, 0.0f });

						// ボス登場SE
						pSound->Play(CSound::SOUND_LABEL_SE_BOSSENTRY);

						// ボスBGM
						pSound->Play(CSound::SOUND_LABEL_BGM_BOSS);

						m_nBossSpawn_Counter = 0;
					}
					else
					{
						m_nBossSpawn_Counter++;
					}
					
				}
			}
			// ボスが出てからの処理
			else
			{
				// 十匹以下になったら一体復活
				if (m_pEnemyManager->GetAllEnemy().size() < 10)
				{
					if (m_nMobSpawn_Counter >= 180)
					{
						CMob::Create({ utility::Random<float>(5000.0f, -5000.0f), utility::Random<float>(600.0f, 200.0f), utility::Random<float>(5000.0f, -5000.0f) });
						m_nMobSpawn_Counter = 0;
					}
					else
					{
						m_nMobSpawn_Counter++;
					}
				}
			}
		}

		//オンラインの送信
		/*if (CApplication::GetClient()->GetIsConnect())
		{
			CModelData::SSendEnemy sendData;
			sendData.m_pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
			sendData.m_rot = D3DXVECTOR3(0.1f, 0.0f, 0.1f);
			for (int j = 0; j < 5; j++)
			{
				sendData.m_haveAbnormal.abnormalData[j] = 0;
				sendData.m_haveItem.itemData[j] = 0;
			}
			sendData.m_motion = 0;
			sendData.m_log = 2;
			sendData.m_pushBomComands = 0;

			CApplication::GetClient()->SendPlayerData(sendData);
		}*/
	}
}

//==============================================================================================
// ゲーム終了処理
//==============================================================================================
void CGame::GameEnd()
{
	// ゲームが終了するまでカウント
	if (m_nEndCounter < MAX_END_TIMER)
	{
		// 終了ロゴを生成する
		if ((m_nEndCounter == MAX_FINISH_ROGO) && (m_pFinishRogo == nullptr))
		{
			m_pFinishRogo = CFontString::Create({ SCREEN_WIDTH / 2 - 360, SCREEN_HEIGHT / 2, 0.0f }, { 100.0f, 100.0f }, "げきはァ!");
			m_pFinishRogo->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });

			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_ENTER);
		}
		m_nEndCounter++;
	}
	// ゲームを終了させる
	else
	{
		// 終了ロゴの終了
		if (m_pFinishRogo != nullptr)
		{
			m_pFinishRogo->Uninit();
			m_pFinishRogo = nullptr;
		}

		// リザルトへ飛ぶ
		CFade::SetFade(CApplication::MODE_RESULT, 0.05f);
	}
}

//==============================================================================================
// メニューウィンドウ処理
//==============================================================================================
void CGame::MenuWindow()
{
	CInput* pInput = CInput::GetKey();
	//// 現在のモード
	//CApplication::MODE Mode = CApplication::GetModeType();

	//if (Mode == CApplication::MODE_GAME)
	//{
	//	if (this != nullptr && m_bInputFlag == false)
	//	{
	//		pInput->SetKeyLock(100);
	//		m_bInputFlag = true;
	//	}
	///*	else if (this != nullptr && m_bInputFlag == true)
	//	{
	//		pInput->UnlockKey(100);
	//	}*/
	//}
	if (m_pConfirmationWindow == nullptr)
	{
		if (pInput->Trigger(DIK_M))
		{
			m_pConfirmationWindow = CConfirmation_Window::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 400.0f, 0.0f),500.0f, 320.0f, D3DXCOLOR(0.5f, 0.5f, 1.0f, 1.0f));
			return;
		}
	}
	else
	{
		m_pConfirmationWindow->Update();
	}
	// ウィンドウが閉じた場合 
	if (m_pConfirmationWindow != nullptr && m_pConfirmationWindow->GetUninit() == true)
	{
		delete m_pConfirmationWindow;
		m_pConfirmationWindow = nullptr;
	}
}

//==============================================================================================
// プレイヤーUIテクスチャの設定
//==============================================================================================
void CGame::SetPlayerUI(const int index, const int type)
{
	// テクスチャ番号
	int nTexNumber = 0;

	if (index == CPlayerUi::UITYPE_SUPPORT)
	{
		// サポートスキルの最低値 + 自身の番号を設定
		nTexNumber = CTexture::TEXTURE_SKILL_RUSH + type;
	}
	else
	{
		// 近接武器の最低値より大きい場合
		if (type >= CWeapon::MELEE_WEAPON_POKE_LANCE)
		{
			// 武器の最低値を初期値に設定
			nTexNumber = CTexture::TEXTURE_WEAPON_POKE;
		}
		// 近接武器の最低値より大きい場合
		else if (type >= CWeapon::MELEE_WEAPON_SLASH_SAMURAI_SWORD)
		{
			// 武器の最低値を初期値に設定
			nTexNumber = CTexture::TEXTURE_WEAPON_SLASH;
		}
		// 素手の最低値より大きい場合
		else if(type >= CWeapon::WEAPON_KNUCKLE)
		{
			// 武器の最低値を初期値に設定
			nTexNumber = CTexture::TEXTURE_WEAPON_KNUCKLE;
		}
		// SRの最低値より大きい場合
		else if (type >= CWeapon::GUN_WEAPON_SR_WINTER5000)
		{
			// 武器の最低値を初期値に設定
			nTexNumber = CTexture::TEXTURE_WEAPON_SR;
		}
		// SGの最低値より大きい場合
		else if (type >= CWeapon::GUN_WEAPON_SG_12PUMP)
		{
			// 武器の最低値を初期値に設定
			nTexNumber = CTexture::TEXTURE_WEAPON_SG;
		}
		// MGの最低値より大きい場合
		else if(type >= CWeapon::GUN_WEAPON_MG_LA2000)
		{
			// 武器の最低値を初期値に設定
			nTexNumber = CTexture::TEXTURE_WEAPON_MG;
		}
		// HGの最低値より大きい場合
		else if (type >= CWeapon::GUN_WEAPON_HG_HG37)
		{
			// 武器の最低値を初期値に設定
			nTexNumber = CTexture::TEXTURE_WEAPON_HG;
		}
		// SMGの最低値より大きい場合
		else if (type >= CWeapon::GUN_WEAPON_SMG_MPC50)
		{
			// 武器の最低値を初期値に設定
			nTexNumber = CTexture::TEXTURE_WEAPON_SMG;
		}
		// ARの最低値より大きい場合
		else if (type >= CWeapon::GUN_WEAPON_AR_AR40)
		{
			// 武器の最低値を初期値に設定
			nTexNumber = CTexture::TEXTURE_WEAPON_AR;
		}

		if (index == CPlayerUi::UITYPE_ATTACK)
		{
			// サポートスキルの最低値 + 自身の番号を設定
			nTexNumber += (CTexture::TEXTURE_ATTACK_SKILL_KNUCKLE - CTexture::TEXTURE_WEAPON_KNUCKLE);
		}
	}

	// スキル画像の設定
	m_pPlayer_UI[index]->GetSkillUI()->SetTexture((CTexture::TEXTURE)nTexNumber);
}

//==============================================================================================
// 武器、パーツのドロップ
//==============================================================================================
void CGame::SetDrop_Parts(int num, D3DXVECTOR3 pos, bool random)
{
	for (int nCnt = 0; nCnt < num; nCnt++)
	{
		D3DXVECTOR3 Pos = pos;
		if (random)
			// ランダムな位置
			Pos = { utility::Random<float>(5000.0f, -5000.0f), utility::Random<float>(600.0f, 200.0f), utility::Random<float>(5000.0f, -5000.0f) };

		// タイプの設定
		int nRandType = 0;

		// タイプ
		nRandType = utility::Random<int>(CDrop_Weapon::DROP_PARTS_MAX, 0);

		// 最大数 または 素手が読み込まれた場合やり直す処理
		while (CDrop_Weapon::ARMS_MAX == nRandType || CDrop_Weapon::LEG_MAX == nRandType
			|| CDrop_Weapon::MELEE_WEAPON_NONE == nRandType || CDrop_Weapon::MELEE_WEAPON_MAX == nRandType
			|| CDrop_Weapon::GUN_WEAPON_MAX == nRandType)
		{
			// タイプ
			nRandType = utility::Random<int>(CDrop_Weapon::DROP_PARTS_MAX, 0);
		}

		int nRandRarity = utility::Random<int>(3, 0);

		// 生成
		CDrop_Weapon::Create(Pos, nRandType, nRandRarity);
	}
}

//==============================================================================================
// ゲーム終了処理
//==============================================================================================
void CGame::SetGameEnd()
{
	// ゲーム終了判定を真にする
	m_bGameEnd = true;
}

//==============================================================================================
// 生成処理
//==============================================================================================
CGame* CGame::Create()
{
	CGame* pGame = new CGame;

	if (FAILED(pGame->Init()))
	{
		return nullptr;
	}

	return pGame;
}