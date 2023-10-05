//==============================================================================================
// 
// キャラクター　　　character.cpp
// tutida ryousei
// 
//==============================================================================================
#include"character.h"
#include"model.h"
#include"application.h"
#include"file.h"
#include"game.h"
#include"entry.h"
#include"stage_select.h"
#include"result.h"
#include"input.h"
#include"objectX.h"
#include"gauge.h"
#include"score.h"
#include"scoreitem_stack.h"
#include"receipt.h"
#include"charmanager.h"
#include"ore.h"
#include"oremanager.h"
#include"map.h"
#include"particle_emitter.h"
#include"shop.h"
#include"enemymanager.h"
#include"enemyspawner.h"
#include"itemmanager.h"
#include"result_obj.h"
#include"area.h"
#include"trolley_manager.h"
#include"debugProc.h"
#include"piller.h"
#include"shop_direction.h"
#include"map.h"
#include"gimmick_model.h"

const float CCharacter::Move_Rot_Speed = 0.2f;				// キャラの回転速度
const float CCharacter::Character_Radius = 20.0f;			// キャラの当たり判定のサイズ(半径)
const float CCharacter::Attack_Length = 70.0f;				// キャラの初期攻撃射程
const float CCharacter::Attack_Size = 40.0f;				// キャラの初期攻撃範囲(円形)
const float CCharacter::Attack_LevelUp = 10.0f;				// 攻撃力上昇に伴ったパワーアップ量

const float CCharacter::Attack_Angle = 120.0f;				// キャラの攻撃範囲(扇型)
const float CCharacter::Shop_Blink_Alpha_Speed = 0.02f;		// 質屋のアルファ値の点滅の速度
const float CCharacter::Shop_Blink_Max_Alpha = 0.4f;		// 質屋のアルファ値の点滅の最大値
const float CCharacter::Shop_Col_Radius = 90.0f;			// 質屋の当たり判定の大きさ

int CCharacter::m_nTimeSlip_CoolTime = (m_nBefor_Frame * 2) * CCharManager::Max_Character;
int CCharacter::m_nTimeSlip_CoolTime_Count = 0;
bool CCharacter::Invocation_TimeSlip = false;
int CCharacter::m_nTimeSlip_Charindex = 0;
//==============================================================================================
// コンストラクタ
//==============================================================================================
CCharacter::CCharacter() : CObject(2)
{
	m_fRadius = Character_Radius;
	m_fMoveSpeed = 0.0f;
	m_fRotSpeed = Move_Rot_Speed;
	m_nAttackPower = First_Attack_Power;
	m_fAttackLength = Attack_Length;
	m_fAttackSize = Attack_Size;
	m_nKnockBack_Frame = 0;
	m_nAttack_HitCount = 0;
	m_nSaleCount = 0;
	m_nShopBlinkAlphaCount = 0;
	m_bDraw = true;
	m_nWalkCounter = 0;
	m_bTime_Elapsed = false;
	m_bUseTimeSlip = false;
	m_bZeroOver = false;
	m_bAttack = false;
	m_bKnockBack = false;
	m_bFlashing = false;
	m_bSafe = false;
	m_nNum_StackScoreItem = 0;
	m_Move = { 0.0f, 0.0f, 0.0f };
	m_KnockBack = { 0.0f, 0.0f, 0.0f };
	m_bInTerritory = true;
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CCharacter::~CCharacter()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CCharacter::Init()
{
	m_Motion = MOTION_NEUTRAL;
	m_nCurrentMotion = 0;			// 現在のモーション番号

	// 前回の止まったモーションをfalseにする
	m_MotionSet[m_nCurrentMotion].bStop = false;
	m_nCurrentKey = 0;				// キーを0にする
	m_nCountMotion = 0;				// モーションカウントを0にする

	m_bNullModel = false;

	for (int nCnt = 0; nCnt < m_nBefor_Frame; nCnt++)
	{
		m_nBefore_Pos[nCnt] = m_pos;
	}

	// アイテムをドロップする確率
	m_nProbability = 3;

	// キャラクタータイプの表示
	m_pCharacter_Type = CObject3D::Create({ m_pos.x,m_pos.y + 120.0f,m_pos.z }, { 40.0f,30.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f }, 4, true);
	m_pCharacter_Type->SetTexture(CTexture::TEXTURE_CHARA_TYPE);

	m_bEntry = false;

	for (int nCnt = 0; nCnt <= m_nCharacterIndex; nCnt++)
	{
		// エントリーしたキャラ
		m_bEntry = CEntry::GetEntry(nCnt);
	}

	// プレイヤー番号、CPUの表示
	m_bEntry ?
		m_pCharacter_Type->SetTexPos(0.0f, 1.0f, 0.2f + (0.2f * m_nCharacterIndex), 0.0f + (0.2f * m_nCharacterIndex)) :
		m_pCharacter_Type->SetTexPos(0.0f, 1.0f, 1.0f, 0.8f);

	// パーツの初期位置
	InitPos[m_nNumParts] = {};
	InitRot[m_nNumParts] = {};

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		// キャラのUXを設定
		CharUx();
	}

	// アイテムの取得状態を読み込む
	for (int nCnt = 0; nCnt < CItem::ITEM_MAX; nCnt++)
	{
		Item_Effect(nCnt);
	}

	if (m_pScoreItemStack != nullptr)
	{
		// 最大所持数を取得
		m_pScoreItemStack->SetDefault(m_nGemStack);
	}

	// 王冠の生成
	m_Crown = CObjectX::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 3.0f,3.0f,3.0f }, "Data/model/crown.x");
	m_Crown->SetDrawFlag(false);

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CCharacter::Uninit()
{
	// モデルの終了処理
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
			delete m_pModel[nCnt];
			m_pModel[nCnt] = nullptr;
		}
	}

	// スコアの終了処理
	if (m_pScore != nullptr)
	{
		// スコアをリザルトに送る
		CResult::SetScore(m_pScore->GetScore(), m_nCharacterIndex);
		CPiller::SetScore(m_pScore->GetScore(), m_nCharacterIndex);
		m_pScore->Uninit();
		m_pScore = nullptr;
	}

	// 宝石所持数の終了処理
	if (m_pScoreItemStack != nullptr)
	{
		m_pScoreItemStack->Uninit();
		m_pScoreItemStack = nullptr;
	}

	for (int nCnt = 0; nCnt < Max_GemStack; nCnt++)
	{
		// 宝石所持数の終了処理
		if (m_pOreStock[nCnt] != nullptr)
		{
			m_pOreStock[nCnt]->Uninit();
			delete m_pOreStock[nCnt];
			m_pOreStock[nCnt] = nullptr;
		}
	}

	Release();
}

//==============================================================================================
// 更新処理
//==============================================================================================
void CCharacter::Update()
{
	// ノックバック
	KnockBack();

	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		// 質屋との当たり判定
		ShopCollision();

		// 過去の位置を保存
		Before_Pos();

		if (Invocation_TimeSlip)
		{
			// タイムスリップ
			TimeSlip();
		}

		// タイムスリップを発動したキャラ
		TimeSlip_CoolTime();

		// 陣地内かどうか
		m_bInTerritory = CGame::GetArea()->InCheckTerritory(m_pos, m_nCharacterIndex);

		// 質屋の方向
		Shop_Direction();

		// オブジェクトとの当たり判定
		CollObject();

		// 敵キャラとの当たり判定
		EnemyCollision();
	}

	// プレイヤー番号の位置の更新
	m_pCharacter_Type->SetPos({ m_pos.x,m_pos.y + 120.0f,m_pos.z });

	// 無敵
	Flash();

	// 移動している場合
	if (m_bMove)
	{
		Walk();
	}

	// 攻撃したか
	if (m_bAttack)
	{
		// 攻撃処理
		Attack();
	}

	// 角度の正規化
	CCharacter::NormalizeRot();
}

//==============================================================================================
// 描画処理
//==============================================================================================
void CCharacter::Draw()
{
	if (m_bDraw)
	{
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
		D3DXMATRIX mtxRot, mtxTrans;		//計算用のマトリックス

		//ワールドマトリックス
		D3DXMatrixIdentity(&m_mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
		{
			if (m_pModel[nCnt] != nullptr)
			{
				// モデルの描画処理
				m_pModel[nCnt]->Draw();
			}
		}

		// 現在所持しているスコアアイテムだけを描画する
		for (int nCnt = 0; nCnt < m_nNum_StackScoreItem; nCnt++)
		{
			m_pOreStock[nCnt]->Draw();
		}
	}
}

//==============================================================================================
// モーション値の読み込み
//==============================================================================================
void CCharacter::SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop)
{
	// キーフレームの設定
	m_MotionSet[motion].aKeySet[key].nFrame = frame;

	// 位置と角度の設定
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyPos = pos;
	m_MotionSet[motion].aKeySet[key].aKey[parts].KeyRot = rot;

	// キーの最大数
	m_MotionSet[motion].nMaxKey = maxkey;

	// ループするか
	m_MotionSet[motion].bLoop = loop;
}

//==============================================================================================
// モーションの再生
//==============================================================================================
void CCharacter::Motion()
{
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr && !m_MotionSet[m_nCurrentMotion].bStop)
		{
			// 相対値
			float fRelative = (float)m_nCountMotion / (float)m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame;

			// 差分
			D3DXVECTOR3 fPosDifference;		// 位置用
			D3DXVECTOR3 fRotDifference;		// 回転用

			// 差分
			// 最後のキーじゃない場合
			if (m_nCurrentKey != m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
			{
				// 位置
				fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyPos
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;					// 終了値 - 開始値

				// 角度
				fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey + 1].aKey[nCnt].KeyRot
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;					// 終了値 - 開始値
			}
			// 最後のキーの場合
			else if (m_nCurrentKey == m_MotionSet[m_nCurrentMotion].nMaxKey - 1)
			{
				// 位置
				fPosDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyPos
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos;	// 終了値 - 開始値

				// 角度
				fRotDifference = m_MotionSet[m_nCurrentMotion].aKeySet[0].aKey[nCnt].KeyRot
					- m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot;	// 終了値 - 開始値
			}

			// 現在値
			D3DXVECTOR3 Pos = InitPos[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyPos + (fPosDifference * fRelative);	// 位置
			D3DXVECTOR3 Rot = InitRot[nCnt] + m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].aKey[nCnt].KeyRot + (fRotDifference * fRelative);	// 回転

			m_pModel[nCnt]->SetPos(Pos);	// 位置の設定
			m_pModel[nCnt]->SetRot(Rot);	// 回転の設定

			m_bNullModel = false;
		}
		else if (m_pModel[0] == nullptr)
		{
			// nullのプレイヤー、CPUを無視する
			m_bNullModel = true;
		}
	}

	// モーションカウンター
	m_nCountMotion++;

	// 最後のフレームまで進んだ
	if (m_nCountMotion >= m_MotionSet[m_nCurrentMotion].aKeySet[m_nCurrentKey].nFrame
		&& !m_bNullModel)
	{
		// フレームを0に戻す
		m_nCountMotion = 0;

		// キーを進める
		m_nCurrentKey++;

		if (m_nCurrentKey >= m_MotionSet[m_nCurrentMotion].nMaxKey)
		{
			// ループする
			if (m_MotionSet[m_nCurrentMotion].bLoop)
			{
				// キーを戻してループする
				m_nCurrentKey = 0;
			}
			// ループしない
			else
			{
				m_MotionSet[m_nCurrentMotion].bStop = true;
				m_Motion = MOTION_NEUTRAL;
			}
		}
	}
}

//==============================================================================================
// モーション変更
//==============================================================================================
void CCharacter::ChangeMotion()
{
	// 現在のモーションから変わった場合
	if (m_nCurrentMotion != m_Motion)
	{
		// 現在モーションの終了処理
		switch (m_nCurrentMotion)
		{
		case MOTION_WALK:
			m_bMove = false;
			m_nWalkCounter = 0;
			break;
		case MOTION_MINE:
			m_bAttack = false;
			m_nAttack_HitCount = 0;
			break;
		default:
			break;
		}

		// 目的の角度を現在の角度に変更する
		m_rotDest = m_rot;

		// 前回の止まったモーションをfalseにする
		m_MotionSet[m_nCurrentMotion].bStop = false;

		m_nCurrentMotion = m_Motion;	// モーションを切り替える
		m_nCurrentKey = 0;				// キーを0にする
		m_nCountMotion = 0;				// モーションカウントを0にする
	}
}

//==============================================================================================
// モデルパーツの設定
//==============================================================================================
CModel *CCharacter::SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename)
{
	if (m_pModel[index] == nullptr)
	{
		if (parent >= 0)
		{// 親がいる場合
			m_pModel[index] = CModel::Create(pos, rot, m_pModel[parent], Xfilename);
		}
		else
		{// 親がいない場合
			m_pModel[index] = CModel::Create(pos, rot, nullptr, Xfilename);
		}

		// パーツの初期位置
		InitPos[index] = m_pModel[index]->GetPos();
		InitRot[index] = m_pModel[index]->GetRot();
	}

	// 体モデルの場合
	if (index == 0 || index == 2)
	{
		switch (m_nCharacterIndex)
		{
			// 1P
		case 0:
			m_charaCol = { 1.0f, 0.0f, 0.0f, 0.0f };
			break;
			// 2P
		case 1:
			m_charaCol = { 0.0f, 0.0f, 1.0f, 0.0f };
			break;
			// 3P
		case 2:
			m_charaCol = { 0.0f, 1.0f, 0.0f, 0.0f };
			break;
			// 4P
		case 3:
			m_charaCol = { 1.0f, 1.0f, 0.0f, 0.0f };
			break;
		default:
			break;
		}
		m_pModel[index]->AddColor(m_charaCol);
	}
	// 背負うスコアアイテムの生成処理
	else if(index == 12 && CApplication::GetMode() == CApplication::MODE_GAME)
	{
		Create_ScoreItem();
	}

	return m_pModel[index];
}

//==============================================================================================
// 移動処理
//==============================================================================================
void CCharacter::Move()
{
	// 移動量を位置に加算する
	m_pos += m_Move * m_fMoveSpeed;
}

//==============================================================================================
// キャラ毎の色の初期化
//==============================================================================================
void CCharacter::ResetCharaColor()
{
	for (int index = 0; index < m_nNumParts; index++)
	{
		// 指定したモデルが使用中の場合
		if (m_pModel[index] != nullptr)
		{
			// モデルの色を初期化する
			m_pModel[index]->ResetColor();

			// 胴体モデルの色を加算する
			if (index == 0 || index == 2)
			{
				m_pModel[index]->AddColor(m_charaCol);
			}
		}
	}
}

//==============================================================================================
// 王冠の設定処理
//==============================================================================================
void CCharacter::SetCrown(const bool crown)
{
	// 論理和がtrueの場合描画する
	if (crown == true)
	{
		// 王冠の位置の更新
		m_Crown->SetPos({ m_pos.x,m_pos.y + 150.0f,m_pos.z });

		// 王冠の描画設定
		m_Crown->SetDrawFlag(true);
	}
	else
	{
		// 王冠の描画設定
		m_Crown->SetDrawFlag(false);
	}
}

//==============================================================================================
// 角度の正規化
//==============================================================================================
void CCharacter::NormalizeRot()
{
	// 目的の角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2;
	}

	// 目的の角度にする
	m_rot.y += (m_rotDest.y - m_rot.y) * m_fRotSpeed;

	// 角度の正規化
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//==============================================================================================
// 歩き処理
//==============================================================================================
void CCharacter::Walk()
{
	m_nWalkCounter++;

	// パーティクル
	if (m_bMove && m_nWalkCounter >= 20)
	{
		std::move(CParticleEmitter::Create("Smoke", m_pos));
		m_nWalkCounter = 0;
	}
}

//==============================================================================================
// アイテム取得時の効果
//==============================================================================================
void CCharacter::Item_Effect(const int type)
{
	switch ((CItem::Item_Type)type)
	{
		// 移動速度レベル
	case CItem::ITEM_SPEEDUP:
		switch (m_MoveSpeed_Level)
		{
		case 0:
			m_fMoveSpeed = 5;	// 初期値
			break;
		case 1:
			m_fMoveSpeed = 6;
			break;
		case 2:
			m_fMoveSpeed = 7;
			break;
		case 3:
			m_fMoveSpeed = 8;
		default:
			break;
		}
		break;
		// ノックバックのレベル
	case CItem::ITEM_KNOCKBACK:
		// 上限レベル4
		if (m_nKnockBack_Level < 4)
		{
		// 攻撃力Upの効果を反映(射程、範囲、威力)
			m_fAttackLength += Attack_LevelUp;
			m_fAttackSize += Attack_LevelUp * 0.5f;
			m_nAttackPower++;
		}
		switch (m_nKnockBack_Level)
		{
		case 0:
			// 初期値
			m_nKnockBack_Power = 20;
			break;
		case 1:
			m_nKnockBack_Power = 25;
			m_pModel[11]->SetSize({ 1.3f, 1.3f, 1.3f });
			break;
		case 2:
			m_nKnockBack_Power = 30;
			m_pModel[11]->SetSize({ 1.6f, 1.6f, 1.6f });
			break;
		case 3:
			m_nKnockBack_Power = 35;
			m_pModel[11]->SetSize({ 1.f, 1.9f, 1.9f });
		default:
			break;
		}
		break;
		// 一度に持てる鉱石量のレベル
	case CItem::ITEM_STACK:
		switch (m_nGemStack_Level)
		{
		case 0:
			m_nGemStack = 10;	// 初期値
			break;
		case 1:
			m_nGemStack = 14;
			m_pModel[12]->SetSize({ 1.1f, 1.1f, 1.1f });
			break;
		case 2:
			m_nGemStack = 17;
			m_pModel[12]->SetSize({ 1.2f, 1.2f, 1.2f });
			break;
		case 3:
			m_nGemStack = 20;
			m_pModel[12]->SetSize({ 1.3f, 1.3f, 1.3f });
			break;
		default:
			break;
		}

		// スコアアイテムスタックが使用中の場合
		if (m_pScoreItemStack != nullptr)
		{
			// 最大所持数を取得
			m_pScoreItemStack->SetDefault(m_nGemStack);
		}
		break;
	default:
		break;
	}
}

//==============================================================================================
// 攻撃処理
//==============================================================================================
void CCharacter::Attack()
{
	if (!m_bKnockBack)
	{
		// 攻撃を振り下ろすタイミング
		if (m_nAttack_HitCount == Attack_Swing)
		{
			// 採掘モーション_振り下ろすSE
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_SWING);
		}
		// 攻撃の発生時間まで経過すると攻撃処理を行う
		else if (m_nAttack_HitCount == Attack_Hit)
		{
			// 攻撃(円形)設置位置
			const D3DXVECTOR3 attackPos = m_pos - D3DXVECTOR3(sinf(m_rot.y) * m_fAttackLength, 0.0f, cosf(m_rot.y) * m_fAttackLength);

			// 攻撃エフェクト
			CParticleEmitter::Create("Attack", attackPos);

			// 相手キャラクターを攻撃する処理
			for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
			{
				// キャラクターのポインタを取得
				CCharacter *pCharacter = CApplication::GetCharManager()->GetChara(nCnt);

				// ポインタが未使用、且つ 自身ではない場合
				if (pCharacter != nullptr && pCharacter != this)
				{
					// 相手が安全地帯にいない場合 且つ 無敵状態ではない場合
					if (!pCharacter->GetSafe() && !pCharacter->GetFlashing())
					{
						// 位置の取得
						D3DXVECTOR3 atherPos = pCharacter->GetPos();

						// 攻撃が当たっている場合(体の大きさ & ピッケルの射程)
						if (CollisionCircle(attackPos, m_fAttackSize, atherPos, pCharacter->GetRadius())
							|| AttackCollision(m_pos, GetRot().y, CCharacter::m_fAttackLength, CCharacter::Attack_Angle, atherPos, pCharacter->GetRadius()))
						{
							// 相手キャラのノックバックの設定
							pCharacter->KnockBack_Set(atherPos, m_pos, m_nKnockBack_Power, m_nAttackPower);
						}
					}
				}
			}

			// ゲーム中である場合
			if (CApplication::GetMode() == CApplication::MODE_GAME)
			{
				// トロッコの当たり判定の処理
				CTrolley_Manager *pTroManager = CMap::GetTrolleyManager();

				if (pTroManager != nullptr)
				{
					// トロッコが移動中の場合
					if (pTroManager->GetMove() == true)
					{
						CGimmick_Model *pTrolley = pTroManager->GetGimmick_Model();

						// トロッコが使用中の場合
						if (pTrolley != nullptr)
						{
							// 位置の取得
							D3DXVECTOR3 TrolleyPos = pTrolley->GetPos();

							// 攻撃が当たっているか(体の大きさ & ピッケルの射程)
							bool bHit = CollisionCircle(attackPos, m_fAttackSize, TrolleyPos, pTrolley->GetRadius())
								|| AttackCollision(m_pos, m_rot.y, m_fAttackLength, Attack_Angle, TrolleyPos, pTrolley->GetRadius());

							// 攻撃が当たった
							if (bHit)
							{
								// 掘るパーティクル
								std::move(CParticleEmitter::Create("Mine", TrolleyPos));

								// 採掘モーション_採掘SE
								CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_MINE);

								// ランダムにスコアアイテムを(3 + レベル)個ばら撒く処理
								for (int nCnt = 0; nCnt < 3 + pTrolley->GetLevel(); nCnt++) CScoreItem::RandomCreate(TrolleyPos);

								// トロッコのレベルを上げる
								pTrolley->Level(true);
							}
						}
					}
				}

				// 敵キャラの当たり判定
				for (auto pEnemy : CGame::GetEnemyManager()->GetAllEnemy())
				{
					// 位置の取得
					const D3DXVECTOR3 atherPos = pEnemy->GetPos();
					const float atherradius = pEnemy->GetRadius();

					// 攻撃が当たっている場合(体の大きさ & ピッケルの射程)
					if (CollisionCircle(attackPos, m_fAttackSize, atherPos, atherradius)
						|| AttackCollision(m_pos, GetRot().y, CCharacter::m_fAttackLength, CCharacter::Attack_Angle, atherPos, atherradius))
					{
						// 敵キャラを消す
						pEnemy->Death();
					}
				}

				// 敵キャラスポナーのポインタを取得
				CEnemySpawner *pEnemySpawner = CGame::GetEnemySpawner();

				// ポインタが使用中の場合
				if (pEnemySpawner != nullptr)
				{
					// 発生し終えている場合
					if (pEnemySpawner->GetAppearance())
					{
						// 位置の取得
						const D3DXVECTOR3 atherPos = pEnemySpawner->GetPos();
						const float atherradius = pEnemySpawner->GetRadius();

						// 攻撃が当たっている場合(体の大きさ & ピッケルの射程)
						if (CollisionCircle(attackPos, m_fAttackSize, atherPos, atherradius)
							|| AttackCollision(m_pos, GetRot().y, CCharacter::m_fAttackLength, CCharacter::Attack_Angle, atherPos, atherradius))
						{
							// 敵キャラスポナーにダメージを与える
							pEnemySpawner->Damage();
						}
					}
				}
			}

			// 鉱石を採掘する処理
			CMine();
		}

		// 攻撃モーションが終了した場合
		if (m_nAttack_HitCount >= Attack_MaxFrame)
		{
			SetMotion(MOTION_NEUTRAL);
		}
		// 攻撃モーションが終了していない場合カウントする
		else
		{
			// 攻撃が終わるまでのカウント
			m_nAttack_HitCount++;
		}
	}
}

//==============================================================================================
// 鉱石を採掘する処理
//==============================================================================================
void CCharacter::CMine()
{
	// 採掘モーション_叩くSE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_HIT);

	// ゲーム中の場合
	if (CApplication::GetMode() == CApplication::MODE_GAME)
	{
		// 鉱石マネージャーのポインタ
		COreManager* pOreManager = CGame::GetOreManager();

		// 鉱石ポインタのリスト
		std::list <COre*> pListOre = pOreManager->GetOreList();

		//イテレーターループ
		for (auto itr = pListOre.begin(); itr != pListOre.end();)
		{
			//イテレーターから鉱石のポインタの代入
			COre* pOre = *itr;

			// 攻撃(円形)設置位置
			const D3DXVECTOR3 attackPos = m_pos - D3DXVECTOR3(sinf(m_rot.y) * m_fAttackLength, 0.0f, cosf(m_rot.y) * m_fAttackLength);

			// 相手側の情報
			const D3DXVECTOR3 atherPos = pOre->GetPos();

			// 攻撃が当たった
			if (CollisionCircle(attackPos, m_fAttackSize, atherPos, COreManager::Ore_Radius)
				|| AttackCollision(m_pos, GetRot().y, CCharacter::m_fAttackLength, CCharacter::Attack_Angle, atherPos, COreManager::Ore_Radius))
			{
				// 採掘モーション_採掘SE
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_MINE_MINE);
				// 自陣の場合
				if (m_bInTerritory == true)
				{
					// 掘るパーティクル
					std::move(CParticleEmitter::Create("Mine", atherPos));
				}
				// 自陣外の場合
				else
				{
					// 掘るパーティクル
					std::move(CParticleEmitter::Create("MineOre", atherPos));
				}

				// 鉱石にダメージを与え、体力が0以下になった場合
				if (pOre->Damage(m_nMinePower) <= 0)
				{
					// 鉱石消失時パーティクル
					CParticleEmitter::Create("Disappear", atherPos);

					// ランダムでアイテムを生成
					int nRand_Item = rand() % m_nProbability;

					if (nRand_Item == 0)
					{
						// 確率でアイテムの生成
						CItem::Create({ atherPos.x, atherPos.y + 10.0f, atherPos.z }, { 2.0f,2.0f,2.0f });
					}

					// 陣地判定で宝石を採掘する回数を変更する処理(自陣:1個, 敵陣:2個)
					for (int nCnt = 0; nCnt < (!m_bInTerritory ? 2 : 1); nCnt++)
					{
						// ランダムにスコアアイテムをばら撒く処理
						CScoreItem::RandomCreate(atherPos);
					}
					// 鉱石を削除する
					pOreManager->OreDeath(pOre);

					//次のイテレーターの代入、現在のイテレーターを破棄
					itr = pListOre.erase(itr);

					// 処理を打ち切りにする
					continue;
				}
			}
			itr++;	// 破壊されない場合イテレータを進める
		}
	}
}

//==============================================================================================
// ノックバックの設定
//==============================================================================================
void CCharacter::KnockBack_Set(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, int knockback_power, int attackpower)
{
	// ノックバックの角度
	float fPlayerAngle = (float)atan2(pos1.x - pos2.x, pos1.z - pos2.z);

	// ノックバックの方向 * ノックバックの初速
	m_KnockBack.x = sinf(fPlayerAngle) * knockback_power;
	m_KnockBack.z = cosf(fPlayerAngle) * knockback_power;

	// ノックバックの移動距離の設定
	SetKnockBackMove(m_KnockBack);

	// 攻撃をくらった方を向く
	SetRotDest({ 0.0f,fPlayerAngle,0.0f });
	SetRot({ 0.0f,fPlayerAngle,0.0f });

	// ノックバックするか
	SetKnockBack(true);

	// 点滅するか
	SetFlashing(true);

	// やられモーションに変更
	SetMotion(MOTION_KNOCKBACK);

	// ジェムをドロップさせる
	Damage(attackpower);
}

//==============================================================================================
// ノックバック
//==============================================================================================
void CCharacter::KnockBack()
{
	// ノックバックする場合
	if (m_bKnockBack)
	{
		// ノックバック中
		if (m_nKnockBack_Frame <= KnockBack_MaxFrame)
		{
			// やられモーションに変更
			SetMotion(MOTION_KNOCKBACK);

			// 移動量の減衰
			m_KnockBack.x += (0.0f - m_KnockBack.x) * 0.1f;
			m_KnockBack.z += (0.0f - m_KnockBack.z) * 0.1f;

			// ノックバック分の位置を加算
			m_pos += m_KnockBack;

			// 5f毎にエフェクト
			if (m_nKnockBack_Frame % 5 == 0)
			{
				std::move(CParticleEmitter::Create("Smoke", m_pos));
			}
		}
		// ノックバック終了
		else
		{
			m_nKnockBack_Frame = 0;
			m_bKnockBack = false;		// ノックバックを止める
			m_Motion = MOTION_NEUTRAL;

			// 攻撃モーションの解除
			m_bAttack = false;
			m_nAttack_HitCount = 0;
		}
		m_nKnockBack_Frame++;
	}
}

//==============================================================================================
// 無敵処理
//==============================================================================================
void CCharacter::Flash()
{
	// 点滅するか
	if (m_bFlashing)
	{
		if (m_nFlashCount < Flashing_MaxFrame)
		{
			// 秒間(点滅の減速)で点滅を変える
			m_bDraw = m_nFlashCount++ % (Flashing_SlowDown * 2) > Flashing_SlowDown;
		}
		else
		{
			m_nFlashCount = 0;
			m_bFlashing = false;		// 点滅を止める
			m_bDraw = true;				// 描画する
		}
	}
}

//==============================================================================================
// 被弾処理
//==============================================================================================
void CCharacter::Damage(const int power)
{
	// ダメージSE
	CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);

	// 宝石スタックが使用中の場合
	if (m_pScoreItemStack != nullptr)
	{
		m_pScoreItemStack->LostScoreItem(power, m_pos);

		// 持っているジェムを減らす
		if (m_nNum_StackScoreItem - power > 0)
		{
			m_nNum_StackScoreItem -= power;
		}
		else
		{
			m_nNum_StackScoreItem = 0;
		}
	}
}

//==============================================================================================
// スコアアイテムをかごに入れる
//==============================================================================================
void CCharacter::SetStackScoreItem()
{
	// スコアアイテムを使用中の場合
	if (m_pOreStock[m_nNum_StackScoreItem] != nullptr)
	{
		D3DXVECTOR3 Rand_OrePos, Rand_OreRot;

		// スコアアイテムのランダムな位置と角度の設定
		Rand_OrePos.x = (float)(rand() % (10 + 10 + 1) - 10) * 0.75f;	// X軸(10～-10)
		Rand_OrePos.z = (float)(rand() % (10 + 10 + 1) - 10) * 0.75f;	// Z軸(-10～10)

		Rand_OreRot = { (float)(rand() % 3) ,(float)(rand() % 3) ,(float)(rand() % 3) };	// 角度

		// スコアアイテムの位置を設定
		m_pOreStock[m_nNum_StackScoreItem]->SetPos({ Rand_OrePos.x, -10.0f + m_nNum_StackScoreItem, Rand_OrePos.z });
		m_pOreStock[m_nNum_StackScoreItem]->SetRot(Rand_OreRot);
	}

	// 現在数を増やす
	m_nNum_StackScoreItem++;
}

//==============================================================================================
// オブジェクトとの当たり判定
//==============================================================================================
void CCharacter::CollObject()
{
	CMap *pMap = CGame::GetMap();

	if (pMap != nullptr)
	{
		m_pos += pMap->CollisionMapObject(m_pos, m_fRadius);
	}
}

//==============================================================================================
// 過去の位置を保存
//==============================================================================================
void CCharacter::Before_Pos()
{
	if (!m_bPlayTimeSlip && CApplication::GetGameStart())
	{
		// 数秒前の位置の格納
		m_nBefore_Pos[m_nTimeSlip_Count] = m_pos;
		m_nBefore_Rot[m_nTimeSlip_Count] = m_rot;

		if (m_nTimeSlip_Count < m_nBefor_Frame - 1)
		{
			m_nTimeSlip_Count++;
		}
		else
		{
			m_nTimeSlip_Count = 0;
		}

		// 現在の時間
		m_nTimeSlip_Frame = m_nTimeSlip_Count;

		// クールタイムの設定
		m_nTimeSlip_CoolTime_Count++;

		if (m_nTimeSlip_CoolTime_Count >= m_nTimeSlip_CoolTime)
			m_bTime_Elapsed = true;
	}
}

//==============================================================================================
// タイムスリップ
//==============================================================================================
void CCharacter::TimeSlip()
{
	if (m_bTime_Elapsed && !m_bUseTimeSlip)
	{
		m_bPlayTimeSlip = true;

		// 時間を戻す
		m_nTimeSlip_Frame -= m_nTimeSlip_Speed;

		// 0以下になったら一番上に戻す
		if (m_nTimeSlip_Frame < 0 && !m_bZeroOver)
		{
			// 0を超えて戻った分
			int sub = m_nTimeSlip_Frame;

			m_nTimeSlip_Frame = m_nBefor_Frame - 1;
			m_nTimeSlip_Frame += sub;

			m_bZeroOver = true;
		}

		// 最後まで戻った
		if (m_nTimeSlip_Frame <= m_nTimeSlip_Count && m_bZeroOver)
		{
			m_nTimeSlip_Frame = m_nTimeSlip_Count;

			// キャラクターの取得
			for (int nCnt = 0; nCnt < CCharManager::Max_Character; nCnt++)
			{
				// キャラクターのポインタを取得
				CCharacter *pCharacter = CApplication::GetCharManager()->GetChara(nCnt);

				if (pCharacter != nullptr)
				{
					m_nTimeSlip_CoolTime_Count = 0;			// クールタイムのリセット
					pCharacter->SetTimeSlip_SaveTime(0);	// 進んだフレームの初期化
					pCharacter->SetPlayTimeSlip(false);		// タイムスリップ中は位置の格納をしない
					pCharacter->SetTime_Elapsed(false);		// 300フレームたったか
					pCharacter->SetZeroOver(false);			// 0をすぎたか
					pCharacter->SetTimeSlip(false);			// タイムスリップを受ける状態にする
				}
			}

			if (m_nTimeSlip_Charindex != 3)
			{
				if (m_nCharacterIndex == 3)
					Invocation_TimeSlip = false;
			}
			else
			{
				if (m_nCharacterIndex == 2)
					Invocation_TimeSlip = false;
			}
		}

		// 位置と角度の更新
		m_pos = m_nBefore_Pos[m_nTimeSlip_Frame];
		m_rot = m_nBefore_Rot[m_nTimeSlip_Frame];
	}
	else if (!m_bTime_Elapsed)
	{
		Invocation_TimeSlip = false;
		m_bUseTimeSlip = false;
	}
}

//==============================================================================================
// タイムスリップを発動したキャラ
//==============================================================================================
void CCharacter::TimeSlip_CoolTime()
{
	if (m_bUseTimeSlip)
	{
		// タイムスリップを発動したキャラの番号
		m_nTimeSlip_Charindex = m_nCharacterIndex;
	}
}

//==============================================================================================
// スコアアイテムの売却
//==============================================================================================
void CCharacter::ShopCollision()
{
	// 質屋の情報
	CShop *pShop = CGame::GetShop(m_nCharacterIndex);

	// 質屋が使用されている場合
	if (pShop != nullptr)
	{
		// 位置の取得
		D3DXVECTOR3 Shop_Pos = pShop->GetPos();

		// 質屋に触れてる
		if (CollisionCircle(Shop_Pos, Shop_Col_Radius, m_pos, m_fRadius))
		{
			// 宝石を所持している場合に売却中UXを表示する
			pShop->SetSaleUx(m_nNum_StackScoreItem > 0 ? true : false);

			// 点滅の時間(点滅の最大値 / 点滅の速度)
			int nBlinkMax = (int)(Shop_Blink_Max_Alpha / Shop_Blink_Alpha_Speed);

			// 質屋に減算したいアルファ値(売る間隔 % (点滅の時間の2倍) < 点滅の最大値) 結果→色を増減する
			pShop->AddAlpha(m_nShopBlinkAlphaCount++ % (nBlinkMax * 2) < nBlinkMax ? -Shop_Blink_Alpha_Speed : Shop_Blink_Alpha_Speed);

			// 点滅の最大値に達するとリセット
			if (m_nShopBlinkAlphaCount >= nBlinkMax * 2)
			{
				m_nShopBlinkAlphaCount = 0;
			}

			// 売る間隔
			m_nSaleCount++;

			// 売却開始のタイミングに達した場合
			if (m_nSaleCount >= Sale_Start_Timer)
			{
				// 宝石を売る処理(返り値 : 宝石の値段)
				int GemScore = GetGemStack()->SaleScoreItem();

				// スコアの加算
				GetScore()->AddScore(GemScore);

				// スコアが加算された場合のみパーティクル表示
				if (GemScore > 0)
				{
					// 売却SE
					CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_SALE);

					// 売るパーティクル
					std::move(CParticleEmitter::Create("Buy", Shop_Pos));

					// 持っているジェムを減らす
					m_nNum_StackScoreItem--;

					// カウントのリセット
					m_nSaleCount = Sale_Start_Timer - Sale_Next_Timer;
				}
			}

			// 安全地帯の判定
			if (m_bSafe == false)
			{
				m_bSafe = true;
			}
		}
		// 前回売却を行ってた場合
		else if (m_bSafe == true)
		{
			// 売却中UXの表示
			pShop->SetSaleUx(false);

			// カウントのリセット
			m_nSaleCount = 0;
			m_nShopBlinkAlphaCount = 0;

			// 安全地帯の判定
			m_bSafe = false;

			// 色をリセットする
			pShop->ResetColor();
		}
	}
}

//==============================================================================================
// 敵キャラとの当たり判定
//==============================================================================================
void CCharacter::EnemyCollision()
{
	// 全ての敵キャラの情報
	for (auto pEnemy : CGame::GetEnemyManager()->GetAllEnemy())
	{
		D3DXVECTOR3 atherPos = pEnemy->GetPos();

		// キャラクターと敵キャラが接触した場合
		if (CollisionCircle(m_pos, m_fRadius, atherPos, pEnemy->GetRadius()))
		{
			// 無敵中で無ければ
			if (!m_bFlashing)
			{
				// ノックバックの角度
				float fPlayerAngle = (float)atan2(m_pos.x - atherPos.x, m_pos.z - atherPos.z);
				// ノックバック
				KnockBack_Set(m_pos, atherPos, pEnemy->GetKnockBack(), 3);

				// 幽霊消失時パーティクル
				CParticleEmitter::Create("EraseGhost", pEnemy->GetPos());

				// 敵キャラを消す処理
				pEnemy->Death();
			}
		}
	}
}

//==============================================================================================
// 質屋の方向
//==============================================================================================
void CCharacter::Shop_Direction()
{
	if (m_Shop_Direction == nullptr)
		// 質屋の方向
		m_Shop_Direction = CShop_Direction::Create({ m_pos.x,m_pos.y, m_pos.z }, { 0.0f,0.0f,0.0f }, { 100.0f,0.0f,100.0f });

	if (m_Shop_Direction != nullptr)
	{
		if (m_bEntry)
		{
			switch (m_nCharacterIndex)
			{
			case 0:
				m_Shop_Direction->SetCol({ 1.0f,0.0f,0.0f,1.0f });
				break;
			case 1:
				m_Shop_Direction->SetCol({ 0.0f,0.0f,1.0f,1.0f });
				break;
			case 2:
				m_Shop_Direction->SetCol({ 0.0f,1.0f,0.0f,1.0f });
				break;
			case 3:
				m_Shop_Direction->SetCol({ 1.0f,1.0f,0.0f,1.0f });
				break;
			default:
				break;
			}

			m_Shop_Direction->SetDaller(true);
		}
		else
		{
			m_Shop_Direction->SetCol({ 1.0f,1.0f,1.0f,1.0f });
			m_Shop_Direction->SetDaller(true);
		}

		if(m_bSafe)
		{
			m_Shop_Direction->SetCol({ 1.0f,1.0f,1.0f,0.0f });
			m_Shop_Direction->SetDaller(false);
		}

		// 質屋の情報
		CShop *pShop = CGame::GetShop(m_nCharacterIndex);
		D3DXVECTOR3 Shop_Pos = { 0.0f,0.0f,0.0f };

		// 質屋が使用されている場合
		if (pShop != nullptr)
		{
			// 位置の取得
			Shop_Pos = pShop->GetPos();
		}

		// 質屋の角度
		float fShop_Angle = (float)atan2(Shop_Pos.x - m_pos.x, Shop_Pos.z - m_pos.z);

		// 質屋までのベクトル
		D3DXVECTOR3 Vec = Shop_Pos - m_pos;

		// ベクトルの正規化
		D3DXVec3Normalize(&Vec, &Vec);

		m_Shop_Direction->SetShopVec(Vec);

		// 矢印の位置
		m_Shop_Direction->SetPos_Direction(m_pos);
		m_Shop_Direction->SetRot_Direction({ fShop_Angle,0.0f,0.0f });
	}
}

//==============================================================================================
// キャラのUXを設定する
//==============================================================================================
void CCharacter::CharUx()
{
	switch (m_nCharacterIndex)
	{
	case 0:
		m_pScore = CScore::Create({ 0.0f, 40.0f, 0.0f }, { 20.0f, 30.0f, 0.0f }, 25.0f);
		m_pScore->SetCol(1.0f, 0.0f, 0.0f, 1.0f);
		m_pScoreItemStack = CScoreItemStack::Create({ 30.0f, 90.0f, 0.0f }, m_nCharacterIndex);
		break;
	case 1:
		m_pScore = CScore::Create({ 1080.0f, 40.0f, 0.0f }, { 20.0f, 30.0f, 0.0f }, 25.0f);
		m_pScore->SetCol(0.0f, 0.0f, 1.0f, 1.0f);
		m_pScoreItemStack = CScoreItemStack::Create({ 1120.0f, 90.0f, 0.0f }, m_nCharacterIndex);
		break;
	case 2:
		m_pScore = CScore::Create({ 0.0f, 320.0f, 0.0f }, { 20.0f, 30.0f, 0.0f }, 25.0f);
		m_pScore->SetCol(0.0f, 1.0f, 0.0f, 1.0f);
		m_pScoreItemStack = CScoreItemStack::Create({ 30.0f, 370.0f, 0.0f }, m_nCharacterIndex);
		break;
	case 3:
		m_pScore = CScore::Create({ 1080.0f, 320.0f, 0.0f }, { 20.0f, 30.0f, 0.0f }, 25.0f);
		m_pScore->SetCol(1.0f, 1.0f, 0.0f, 1.0f);
		m_pScoreItemStack = CScoreItemStack::Create({ 1120.0f, 370.0f, 0.0f }, m_nCharacterIndex);
		break;
	default:
		break;
	}
}

//==============================================================================================
// 背負うスコアアイテムの生成処理
//==============================================================================================
void CCharacter::Create_ScoreItem()
{
	char *Gem_Name = "\0";

	D3DXVECTOR3 Size = { 1.0f,1.0f,1.0f };

	// ステージごとの鉱石
	switch (CStage_Select::GetStage_Number())
	{
	case 0:
		Gem_Name = "Data/model/BasketDuringObject/Stage01/Geminthebasket.x";
		Size = { 1.3f,1.3f,1.3f };
		break;
	case 1:
		Gem_Name = "Data/model/BasketDuringObject/Stage02/Fossil.x";
		Size = { 2.0f,2.0f,2.0f };
		break;
	case 2:
		Gem_Name = "Data/model/BasketDuringObject/Stage03/Debris.x";
		Size = { 3.0f,3.0f,3.0f, };
		break;
	default:
		break;
	}

	for (int nCnt = 0; nCnt < Max_GemStack; nCnt++)
	{
		m_pOreStock[nCnt] = CModel::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, m_pModel[12], Gem_Name);
		m_pOreStock[nCnt]->SetSize(Size);
		m_pOreStock[nCnt]->SetShadow(false);	// 影を描画しない
	}
}