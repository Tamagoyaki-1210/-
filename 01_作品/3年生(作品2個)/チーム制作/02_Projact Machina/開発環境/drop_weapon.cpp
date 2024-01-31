//=============================================================================
//
// drop_weapon.p
// Author : tutida ryousei
//
//=============================================================================
#include"drop_weapon.h"
#include"drop_manager.h"
#include"objectX.h"
#include"meshfield.h"
#include"tutorial.h"
#include"game.h"
#include"input.h"
#include"object3D.h"
#include"debugProc.h"
#include"application.h"
#include"player_manager.h"
#include"model.h"


const float CDrop_Weapon::PARTS_COLLISION_RADIUS = 300.0f;	// 落ちてる武器の当たり判定の大きさ
const float CDrop_Weapon::PARTS_FLOTIONG_POS = 100.0f;		// 落ちてる武器の浮遊位置

//=============================================================================
// 定義
//=============================================================================
const char* CDrop_Weapon::s_Weapon_FileName[] =
{
	// 腕
	"Data/model/DropParts/SG_01/SG_01_DropArm.x",
	"Data/model/DropParts/SG_03/SG_03_DropArm.x",
	"Data/model/DropParts/SG_02/SG_02_DropArm.x",
	"Data/model/DropParts/SG_05/SG_05_DropArm.x",
	nullptr,

	// 脚
	"Data/model/DropParts/SG_01/SG_01_DropLeg.x",
	"Data/model/DropParts/SG_03/SG_03_DropLeg.x",
	"Data/model/DropParts/SG_02/SG_02_DropLeg.x",
	"Data/model/DropParts/SG_05/SG_05_DropLeg.x",
	nullptr,

	"Data/model/Weapon/Gun_Weapon/AR40.x",				// AR40
	"Data/model/Weapon/Gun_Weapon/Tec-A3.x",			// Tec-A3
	"Data/model/Weapon/Gun_Weapon/DumpMan.x",			// DumpMan
	"Data/model/Weapon/Gun_Weapon/StarVerseAR.x",		// StarVerseAR
	"Data/model/Weapon/Gun_Weapon/MM400ジンクス.x",		// MM400ジンクス

	"Data/model/Weapon/Gun_Weapon/MPC50.x",				// MPC50
	"Data/model/Weapon/Gun_Weapon/PDW-920.x",			// PDW-920
	"Data/model/Weapon/Gun_Weapon/パールSMG.x",			// パールSMG
	"Data/model/Weapon/Gun_Weapon/SilverComet.x",		// SilverComet

	"Data/model/Weapon/Gun_Weapon/HG37.x",				// HG37
	"Data/model/Weapon/Gun_Weapon/Revolver.x",			// Revolver
	"Data/model/Weapon/Gun_Weapon/アキンボ2.0.x",		// アキンボ2.0

	"Data/model/Weapon/Gun_Weapon/LA2000.x",			// LA2000
	"Data/model/Weapon/Gun_Weapon/メタリカ255.x",		// メタリカ255

	"Data/model/Weapon/Gun_Weapon/12Pump.x",			// 12Pump
	"Data/model/Weapon/Gun_Weapon/WR2154.x",			// WR2154

	"Data/model/Weapon/Gun_Weapon/Winter5000.x",		// Winter5000
	"Data/model/Weapon/Gun_Weapon/アテナ.x",			// アテナ
	nullptr,

	// 武器
	"Data/model/SG_01/DummyParent.x",	// 素手

	"Data/model/Weapon/Melee_Weapon/knuckle.x",			// ナックル
	"Data/model/Weapon/Melee_Weapon/pilebunker.x",		// パイルバンカー
	"Data/model/Weapon/Melee_Weapon/brandmarkers.x",	// ブランドマーカー
	"Data/model/Weapon/Melee_Weapon/stunbaton.x",		// スタンバトン

	"Data/model/Weapon/Melee_Weapon/samuraiSword.x",	// 太刀
	"Data/model/Weapon/Melee_Weapon/hammer.x",			// ハンマー
	"Data/model/Weapon/Melee_Weapon/beamSword.x",		// ビームサーベル
	"Data/model/Weapon/Melee_Weapon/basterSword.x",		// バスターソード

	"Data/model/Weapon/Melee_Weapon/lance.x",			// ランス
	"Data/model/Weapon/Melee_Weapon/rapier.x",			// レイピア
	"Data/model/Weapon/Melee_Weapon/partisan.x",		// パルチザン
	"Data/model/Weapon/Melee_Weapon/beamjavelin.x",		// ビームジャベリン
	nullptr,
};

//=============================================================================
// コンストラクタ
//=============================================================================
CDrop_Weapon::CDrop_Weapon(const PRIORITY priority) : CMove_Object(priority)
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	// 生成時に自身のポインタを敵キャラマネージャーに設定
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CTutorial::GetDropManager()->SetDrop(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CGame::GetDropManager()->SetDrop(this);
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CDrop_Weapon::~CDrop_Weapon()
{
	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	// マネージャーが使用中の場合、解放時に自身のポインタを配列から除外する
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		CDropManager* pManager = CTutorial::GetDropManager();
		if (pManager != nullptr) pManager->DestroyDrop(this);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		CDropManager* pManager = CGame::GetDropManager();
		if (pManager != nullptr) pManager->DestroyDrop(this);
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CDrop_Weapon::Init()
{
	SetDelayCollision();

	SetRadius(PARTS_COLLISION_RADIUS);

	m_CenterPos = { 0.0f,PARTS_FLOTIONG_POS,0.0f };

	CObjectX* pObjectX = GetObjectX();

	// 角度の設定
	pObjectX->SetRot({ 0.4f,0.0f,0.0f });

	// 落ちてる武器の生成
	//pObjectX->SetSize({ 3.0f,3.0f,3.0f });

	// パーツの部位の設定
	Parts_Type();

   	m_bPick_Up = false;
	m_fMove = 0.0f;

	D3DXVECTOR3 ModelPos = GetPos();

	// ポインターUIの生成
	m_pPointer = CObject3D::Create(GetPos(), { 200.0f,200.0f }, PRIORITY_CENTER, { 1.0f,1.0f,1.0f,1.0f }, true);
	m_pPointer->SetTexture(CTexture::TEXTURE_ITEM_POINTER);

	// ピックアップUIの生成
	m_pPick_Up = CObject3D::Create({ ModelPos.x, ModelPos.y + 120.0f, ModelPos.z }, { 70.0f,70.0f }, PRIORITY_CENTER, { 1.0f,1.0f,1.0f,1.0f }, true);
	m_pPick_Up->SetTexture(CTexture::TEXTURE_KEY_E);
	m_pPick_Up->SetDrawFlag(false);

	const float nAddCol = -0.05f * m_nRarity;

	pObjectX->AddColor({ nAddCol, nAddCol,nAddCol, 0.0f});


	CMove_Object::Init();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CDrop_Weapon::Uninit()
{
	if (m_pPointer != nullptr)
	{
		m_pPointer->Uninit();
		m_pPointer = nullptr;
	}

	if (m_pPick_Up != nullptr)
	{
		m_pPick_Up->Uninit();
		m_pPick_Up = nullptr;
	}

	if (GetObjectX() != nullptr)
		GetObjectX()->Uninit();

	//if (GetLandObj())
	//{
	//	for (int nCnt = 0; nCnt < GetOnObj(0)->GetOnObjCnt(); nCnt++)
	//	{
	//		GetOnObj(0)->SetOnObj(nullptr, nCnt);
	//	}
	//	GetOnObj(0)->SetOnObjCnt(0);
	//}

	CMove_Object::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CDrop_Weapon::Update()
{
	CMove_Object::Update();

	// 床との当たり判定
	FieldCollision();

	// 角度の加算
	GetObjectX()->AddRot({ 0.0f,0.03f,0.0f });

	// 現在のモード
	CApplication::MODE Mode = CApplication::GetModeType();

	CPlayer* pPlayer = nullptr;

	// モード毎にプレイヤーを読み込む
	if (Mode == CApplication::MODE_TUTORIAL)
	{
		pPlayer = CTutorial::GetPlayerManager()->GetPlayer(0);
	}
	else if (Mode == CApplication::MODE_GAME)
	{
		pPlayer = CGame::GetPlayerManager()->GetPlayer(0);
	}

	D3DXVECTOR3 Player_Pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 Mob_Pos = { 0.0f,0.0f,0.0f };

	if (pPlayer != nullptr)
	{
		// 位置の取得
		Player_Pos = pPlayer->GetPos();
		Mob_Pos = GetPos();
	}

	// プレイヤーから落ちてるパーツの距離
	D3DXVECTOR3 Vec = Player_Pos - Mob_Pos;

	// 距離の算出
	float fDistance = sqrtf(Vec.x * Vec.x + Vec.z * Vec.z);

	// 距離7000以下の場合に表示する
	const bool bDisplay = fDistance <= DRAW_DROP_DISTANCE;

	GetObjectX()->SetDrawFlag(bDisplay);
	m_pPointer->SetDrawFlag(bDisplay);
	ItemPointerMove();
	// 武器を拾う
	Pick_Up_Weapon();

	// ピックアップ状態をリセットする
	m_bPick_Up = false;
}

//=============================================================================
// 描画処理
//=============================================================================
void CDrop_Weapon::Draw()
{
	//CMove_Object::Draw();
}

//=============================================================================
// アイテムポインターの移動処理
//=============================================================================
void CDrop_Weapon::ItemPointerMove()
{
	m_pPointer->SetPos(GetPos());
}

//=============================================================================
// 武器を拾う
//=============================================================================
void CDrop_Weapon::Pick_Up_Weapon()
{
	// ピックアップUIが存在する場合
	if(m_pPick_Up != nullptr)
	{
		// 描画判定
		m_pPick_Up->SetDrawFlag(m_bPick_Up);

		// ピックアップされている場合
		if (m_bPick_Up)
		{
			D3DXVECTOR3 ModelPos = GetPos();

			// 色の設定
			m_pPick_Up->SetCol({ 1.0f,1.0f,1.0f,1.0f });

			m_pPick_Up->SetPos({ ModelPos.x, ModelPos.y + 120.0f, ModelPos.z });
		}
		else
		{
			// 色の設定
			m_pPick_Up->SetCol({ 1.0f,1.0f,1.0f,0.0f });
		}
	}
}

//=============================================================================
// パーツの部位の設定
//=============================================================================
void CDrop_Weapon::Parts_Type()
{
	if (ARMS_MAX > m_nWeapon_Type)
	m_Parts = CPlayer::PARTS_ARMS;
	else if(LEG_MAX > m_nWeapon_Type)
	m_Parts = CPlayer::PARTS_LEG;
	else if (MELEE_WEAPON_MAX > m_nWeapon_Type)
	m_Parts = CPlayer::PARTS_WEAPON;
}

//=============================================================================
// 床との当たり判定
//=============================================================================
void CDrop_Weapon::FieldCollision()
{
	CMeshField *pMesh = CGame::GetMeshField();

	if (pMesh != nullptr)
	{
		const D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 Pos = {};
		SetPosOld(pos);

		// メッシュフィールドとの当たり判定
		float MeshY = pMesh->MeshCollision(pos);

		// メッシュフィールドに乗ってる
		if (pMesh->GetHit())
		{
			m_fMove = 0.0f;
			// 自身の位置
			Pos = { pos.x, MeshY, pos.z };
		}
		else if (GetLandObj())
			Pos = { pos.x, GetObjY(), pos.z };
		else
		{
			// モデルの位置
			D3DXVECTOR3 Weapon_Pos = pos;
			// 落下速度
			m_fMove += 0.5f;
			// Y座標の移動
			Weapon_Pos.y -= m_fMove;

			// 当たり判定の位置
			Pos = { Weapon_Pos.x, Weapon_Pos.y, Weapon_Pos.z };
		}

		SetPos(Pos);
		GetObjectX()->SetPos(Pos);
	}
}

void CDrop_Weapon::Hit(CMove_Object* /*pHit*/)
{
}

//=============================================================================
// 生成処理
//=============================================================================
CDrop_Weapon *CDrop_Weapon::Create(D3DXVECTOR3 pos, int weapon, int rarity)
{
	CDrop_Weapon *pDrop_Weapon = nullptr;

	pDrop_Weapon = new CDrop_Weapon(PRIORITY_MODEL);

	if (pDrop_Weapon != nullptr)
	{
		pDrop_Weapon->SetPos(pos);
		pDrop_Weapon->SetObjectX(CObjectX::Create(pos, { 0.0f,0.0f,0.0f }, nullptr, CApplication::GetModel()->ReadObject(s_Weapon_FileName[pDrop_Weapon->m_nWeapon_Type = weapon])));
		pDrop_Weapon->m_nRarity = rarity;
		pDrop_Weapon->Init();
	}

	return pDrop_Weapon;
}

//==============================================================================================
// 全てのドロップパーツの読み込み
//==============================================================================================
void CDrop_Weapon::LoadAllFile()
{
	for (int nCnt = 0; nCnt < DROP_PARTS_MAX; nCnt++)
	{
		// 名前が存在する場合
		if (s_Weapon_FileName[nCnt] != nullptr)
		{
			CApplication::GetModel()->ReadObject(s_Weapon_FileName[nCnt]);
		}
	}
}