//=============================================================================
//
// player.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "player.h"
#include "inputKeyboard.h"
#include "inputPad.h"
#include "camera.h"
#include "application.h"
#include "debugProc.h"
#include "meshfield.h"
#include "modelPart.h"
#include "animator.h"
#include "CylinderHitbox.h"
#include "score.h"
#include "UIString.h"
#include "gamerace.h"
#include "stage.h"
#include "rendering.h"
#include "coin.h"
#include "playerModel.h"
#include "message.h"
#include "BoxHitbox.h"

//=============================================================================
//							静的変数の初期化
//=============================================================================

int CPlayer::m_nRanking;								//ランキングへのポインタ
const float CPlayer::m_MaxWalkingSpeed = 7.0f;			//最大歩くスピード
const float CPlayer::m_AccelerationCoeff = 2.0f;		//加速係数

//プレイヤーの色

D3DXCOLOR CPlayer::m_playerColor[PLAYER_COLOR_MAX]
{

	{ 1.0f, 0.0f, 0.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f }
};

//コンストラクタ
CPlayer::CPlayer() : CObject::CObject(1)
{
	//メンバー変数をクリアする
	m_move = Vec3Null;								//速度の初期化処理
	m_DestRot = Vec3Null;							//目的の角度の初期化処理
	m_pAnimator = nullptr;							//アニメーターへのポインタ
	m_pHitbox = nullptr;							//ヒットボックスへのポインタ
	m_pScore = nullptr;								//スコアへのポインタ
	m_pScoreUI = nullptr;							//スコアUIへのポインタ
	m_rot = Vec3Null;								//向き
	m_bGoal = false;								//ゴールに着いたかどうか
	m_bMove = false;								//動いているかどうか
	m_bWinner = false;								//勝ったどうか
	m_bPos = false;									//位置についたかどうか
	m_bRot = false;									//回転したかどうか
	m_pAnimator = nullptr;							//アニメーターへのポインタ
	m_pHitbox = nullptr;							//ヒットボックスへのポインタ
	m_pScore = nullptr;								//スコアへのポインタ
	m_State = (STATE)0;								//アニメーション状態
	m_pScoreUI = nullptr;							//スコアのUIへのポインタ
	m_bJump = false;								//ジャンプしているかどうか
	m_nInvincibilityCnt = 0;						//無敵状態のカウンター
	m_nFrame = 0;									//フレーム
	m_pAttackHitbox = nullptr;						//攻撃のヒットボックス
	m_pHeadHitbox = nullptr;						//頭のヒットボックス
	m_bAttacking = false;							//攻撃しているかどうか
	m_nCntAttack = 0;								//攻撃カウンター
	m_fFrictionCoeff = 0.0f;						//摩擦係数
	m_nPlayerRanking = 0;							//プレイヤーのランキング
	m_nRanking = 0;									//ランキング
	m_bDebug = false;								//デバッグ判定の初期化

#ifdef _DEBUG
	m_bDebug = true;								//デバッグ中にする
#endif // !DEBUG

	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{//モデルの部分へのポインタ
		m_pModel[nCnt] = nullptr;
	}
}

//デストラクタ
CPlayer::~CPlayer()
{

}

//初期化処理
HRESULT CPlayer::Init(void)
{
	//メンバー変数の初期化処理
	m_move = Vec3Null;								//速度の初期化処理
	m_DestRot = Vec3Null;							//目的の角度の初期化処理
	m_pAnimator = nullptr;							//アニメーターへのポインタ
	m_pHitbox = nullptr;							//ヒットボックスへのポインタ
	m_pScore = nullptr;								//スコアへのポインタ
	m_pScoreUI = nullptr;							//スコアUIへのポインタ
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);		//向き
	m_bGoal = false;								//ゴールに着いたかどうか
	m_bMove = false;								//動いているかどうか
	m_bWinner = false;								//勝ったどうか
	m_bPos = false;									//位置についたかどうか
	m_bRot = false;									//回転したかどうか
	m_pAnimator = nullptr;							//アニメーターへのポインタ
	m_pHitbox = nullptr;							//ヒットボックスへのポインタ
	m_pScore = nullptr;								//スコアへのポインタ
	m_State = STATE_NEUTRAL;						//アニメーション状態
	m_pScoreUI = nullptr;							//スコアのUIへのポインタ
	m_bJump = false;								//ジャンプしているかどうか
	m_nInvincibilityCnt = 0;						//無敵状態のカウンター
	m_nFrame = 0;									//フレーム
	m_pAttackHitbox = nullptr;						//攻撃のヒットボックス
	m_pHeadHitbox = nullptr;						//頭のヒットボックス
	m_bAttacking = false;							//攻撃しているかどうか
	m_nCntAttack = 0;								//攻撃カウンター
	m_fFrictionCoeff = 0.1f;						//摩擦係数
	m_nPlayerRanking = 0;							//プレイヤーのランキング
	m_nRanking = 0;									//ランキング

	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{//モデルの部分へのポインタ
		m_pModel[nCnt] = nullptr;
	}

	return S_OK;
}

//終了処理
void CPlayer::Uninit(void)
{
	//モデルパーツの終了処理
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		if (m_pModel[nCnt] != nullptr)
		{
			m_pModel[nCnt]->Uninit();
			delete m_pModel[nCnt];
			m_pModel[nCnt] = nullptr;
		}
	}

	//アニメーターの破棄処理
	if (m_pAnimator != nullptr)
	{
		m_pAnimator->Uninit();
		delete m_pAnimator;
		m_pAnimator = nullptr;
	}
	//ヒットボックスの破棄処理
	if (m_pHitbox != nullptr)
	{
		m_pHitbox->Release();
		m_pHitbox = nullptr;
	}
	//スコアの破棄処理
	if (m_pScore != nullptr)
	{
		m_pScore->Clear();
		m_pScore = nullptr;
	}
	//スコアのUIの破棄処理
	if (m_pScoreUI != nullptr)
	{
		m_pScoreUI->Uninit();
		m_pScoreUI = nullptr;
	}
	//頭のヒットボックスの破棄処理
	if (m_pHeadHitbox != nullptr)
	{
		m_pHeadHitbox->Release();
		m_pHeadHitbox = nullptr;
	}
	//攻撃のヒットボックスの破棄処理
	if (m_pAttackHitbox != nullptr)
	{
		m_pAttackHitbox->Release();
		m_pAttackHitbox = nullptr;
	}
}

//更新処理
void CPlayer::Update(void)
{
	D3DXVECTOR3 cameraRot = CApplication::GetCamera()->GetRot();					//カメラの向きの取得処理

	//プレイヤーの目的角度の正規化処理
	D3DXVECTOR3 cR = D3DXVECTOR3(-cosf(cameraRot.y), 0.0f, sinf(cameraRot.y));
	float fA = acosf(cR.x);

	if (cR.z < 0.0f)
	{
		fA *= -1.0f;
	}

	//ゲームが始まった後とゴールしてないとき
	if (!m_bGoal && CMessage::GetStart())
	{
		//プレイヤーの操作
		PlayerController(m_nIdxPlayer);
	}

	//位置の更新
	m_pos += m_move;				
	CDebugProc::Print("\n%f %f %f", m_move.x, m_move.y, m_move.z);
	
	m_move.x += (0.0f - m_move.x) * m_fFrictionCoeff;		//移動量のXコンポネントの更新
	m_move.y += (0.0f - m_move.y) * 0.1f;					//移動量のYコンポネントの更新
	m_move.z += (0.0f - m_move.z) * m_fFrictionCoeff;		//移動量のZコンポネントの更新

	//リスポーン処理
	PlayerRespawn();

	//目的の角度の正規化処理
	if (m_DestRot.y - (m_pModel[BODY]->GetRot().y) > D3DX_PI)
	{
		m_DestRot.y -= 2 * D3DX_PI;
	}
	else if (m_DestRot.y - (m_pModel[BODY]->GetRot().y) < -D3DX_PI)
	{
		m_DestRot.y += 2 * D3DX_PI;
	}

	//回転角度の計算
	D3DXVECTOR3 rot = m_pModel[BODY]->GetRot() + ((m_DestRot - m_pModel[BODY]->GetRot()) * 0.1f);		

	//回転角度の設定処理
	m_pModel[BODY]->SetRot(rot);		

	//回転角度の正規化処理
	float fRot = m_pModel[BODY]->GetRot().y;

	if (fRot > D3DX_PI)
	{
		fRot = -D3DX_PI + (fRot - D3DX_PI);
	}
	else if (fRot < -D3DX_PI)
	{
		fRot = D3DX_PI - (D3DX_PI + fRot);
	}

	if (fRot < D3DX_PI * -2.0f)
	{
		fRot += D3DX_PI * 2.0f;
	}
	else if (fRot > D3DX_PI * 2.0f)
	{
		fRot += D3DX_PI * -2.0f;
	}

	//回転の設定処理
	m_pModel[BODY]->SetRot(D3DXVECTOR3(m_pModel[BODY]->GetRot().x, fRot, m_pModel[BODY]->GetRot().z));

	//重量を追加する
	if (m_move.y >= -10.0f)
	{
		m_move.y -= 0.65f;
	}

	float fHeight = 0.0f;

	//メッシュフィールドとの当たり判定
	CMeshfield* pField = CMeshfield::FieldInteraction(this, &fHeight);

	//地面との当たり判定
	if (pField != nullptr)
	{
		m_bJump = false;		//着地している状態にする
		m_bHit = false;			//当たってない状態にする
		//摩擦係数の取得
		m_fFrictionCoeff = pField->GetFriction();

		//影の高さの設定
		for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
		{
			if (m_pModel[nCnt] != nullptr)
			{
				m_pModel[nCnt]->SetShadowHeight(fHeight);		
			}
		}
	}
	else if (m_pos.x < -135.0f || m_pos.x > 125.0f)
	{//メッシュフィールドから離れたら
		//影の高さ見えないように設定する
		for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
		{
			if (m_pModel[nCnt] != nullptr)
			{
				m_pModel[nCnt]->SetShadowHeight(-5000.0f);
			}
		}
	}

	if (m_pAnimator != nullptr)
	{
		//アニメーションの更新
		m_pAnimator->Update();
	}

	//カメラの情報の取得
	CCamera* pCamera = CApplication::GetCamera();

	if (pCamera != nullptr)
	{
		//カメラのPosの取得
		D3DXVECTOR3 wallPos = pCamera->GetPos();
		wallPos.z += 160.0f;

		if (m_pos.z <= wallPos.z)
		{//カメラの前に押し出す
			m_pos.z = wallPos.z;
		}
		else if (m_pos.z >= wallPos.z + 250.0f)
		{//カメラの一定より前に出ない
			m_pos.z = wallPos.z + 250.0f;
		}

		// プレイヤー最大数分のインスタンスを作成
		CPlayer* pPlayer[PLAYER_MAX];

		// 各プレイヤーのポインタを取得し、各インスタンスに代入
		for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
		{
			pPlayer[nCnt] = CStage::GetPlayer(nCnt);
		}

		//ゴールしたら
		if (m_pos.z >= 900.0f && m_bGoal == false)
		{
			m_bGoal = true;
			//順位付け
			m_nRanking++;
			m_nPlayerRanking = m_nRanking;
			//順位によってスコアの加算
			if (m_pScore != nullptr)
			{
				if (m_nPlayerRanking == 1)
				{
					m_pScore->AddScore(100);
				}
				else if (m_nPlayerRanking == 2)
				{
					m_pScore->AddScore(50);
				}
				else if (m_nPlayerRanking == 3)
				{
					m_pScore->AddScore(15);
				}
				else if (m_nPlayerRanking == 4)
				{
					m_pScore->AddScore(5);
				}
			}
		}

		//ゴール後の動き
		GoalMove();

		//無敵時間
		if (m_nInvincibilityCnt > 0)
		{
			m_nInvincibilityCnt--;			//カウンターの更新

			if (m_nInvincibilityCnt <= 0)
			{//0になったら、
				if (m_pHitbox != nullptr)
				{//ヒットボックスがnullではなかったら
					m_pHitbox->SetInvincibility(false);			//無敵状態が終わるように設定する
				}
			}
		}

		if (m_pHitbox != nullptr)
		{
			//スコアの取得
			int nScore = m_pScore->GetScore();

			//当たり判定の更新
			m_pHitbox->SetPos(m_pos);
			m_pHitbox->Update();

			//当たり判定のエフェクトを取得
			CHitbox::INTERACTION_EFFECT effect = m_pHitbox->GetEffect();

			switch (effect)
			{

			case CHitbox::EFFECT_DAMAGE:
			
			{//普通のダメージを受けた場合

				int spawnCoin = (int)((nScore - m_pScore->GetScore()) * 0.1f);			//スポーンコイン数を計算する

				for (int nCnt = 0; nCnt < spawnCoin; nCnt++)
				{//コインを落とす処理
					CCoin::Create(GetPos(), D3DXVECTOR3((float)random(-5, 5), 10.0f, (float)random(-5, 5)), 180, CCoin::COIN_0);
				}

				m_nInvincibilityCnt = 60;												//無敵状態カウンターを設定する
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);			//ダメージサウンドを再生する

				if (m_pHitbox != nullptr)
				{//ヒットボックスのポインタがnullではなかったら
					m_pHitbox->SetEffect(CHitbox::EFFECT_MAX);		//エフェクトを戻す
					m_pHitbox->SetInvincibility(true);				//無敵状態を設定する
				}
			}

			break;

			case CHitbox::EFFECT_LAUNCH:

			{//ノックバック
				m_bHit = true;								//ダメージフラグをtrueにする
				m_pAnimator->SetPresentAnim(4);				//現在のアニメーションをダメージアニメーションにする

				int spawnCoin = (int)((nScore - m_pScore->GetScore()) * 0.1f);		//スポーンコイン数を計算する

				for (int nCnt = 0; nCnt < spawnCoin; nCnt++)
				{//コインを落とす処理
					CCoin::Create(GetPos(), D3DXVECTOR3((float)random(-5, 5), 10.0f, (float)random(-5, 5)), 180, CCoin::COIN_0);
				}

				//ノックバックの処理(新しい速度の設定)
				D3DXVec3Normalize(&m_move, &m_move);
				m_move.x *= -50.0f;
				m_move.y = 10.0f;
				m_move.z *= -50.f;
						
				m_nInvincibilityCnt = 60;												//無敵状態カウンターを設定する
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);			//ダメージサウンドを再生する

				if (m_pHitbox != nullptr)
				{//ヒットボックスのポインタがnullではなかったら
					m_pHitbox->SetEffect(CHitbox::EFFECT_MAX);				//エフェクトを戻す
					m_pHitbox->SetInvincibility(true);						//無敵状態を設定する
				}
			}

			break;

			case CHitbox::EFFECT_PUSH:

			{//押されたエフェクト
				m_bHit = true;								//ダメージフラグをtrueにする
				m_pAnimator->SetPresentAnim(4);				//現在のアニメーションをダメージアニメーションにする

				//ノックバックの処理(新しい速度の設定)
				m_move = m_pHitbox->GetDirection();
				D3DXVec3Normalize(&m_move, &m_move);
				m_move.x *= 5.0f;
				m_move.y = 10.0f;
				m_move.z *= 5.f;

				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE_PUNCH);		//ダメージサウンドを再生する

				if (m_pHitbox != nullptr)
				{//ヒットボックスのポインタがnullではなかったら
					m_pHitbox->SetEffect(CHitbox::EFFECT_MAX);					//エフェクトを戻す
					m_pHitbox->SetDirection(Vec3Null);							//無敵状態を設定する
				}
			}

				break;

			case CHitbox::EFFECT_BOUNCE:

			{//跳ね返るエフェクト
				m_bHit = true;								//ダメージフラグをtrueにする
				m_pAnimator->SetPresentAnim(4);				//現在のアニメーションをダメージアニメーションにする

				//ノックバックの処理(新しい速度の設定)
				D3DXVec3Normalize(&m_move, &m_move);
				m_move.x *= -50.0f;
				m_move.y = 10.0f;
				m_move.z *= -50.f;
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE_PUNCH);			//ダメージサウンドを再生する

				if (m_pHitbox != nullptr)
				{//ヒットボックスのポインタがnullではなかったら
					m_pHitbox->SetEffect(CHitbox::EFFECT_MAX);					//エフェクトを戻す
					m_pHitbox->SetDirection(Vec3Null);							//無敵状態を設定する
				}
			}

			break;

			case CHitbox::EFFECT_JUMP:

			{//ジャンプエフェクト
				m_bHit = true;							//ダメージフラグをtrueにする
				m_bJump = true;							//ジャンプフラグをtrueにする
				m_pAnimator->SetPresentAnim(4);			//現在のアニメーションをダメージアニメーションにする

														//ノックバックの処理(新しい速度の設定)
				D3DXVec3Normalize(&m_move, &m_move);
				m_move.y = 30.0f;
				CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMP);			//ダメージサウンドを再生する

				if (m_pHitbox != nullptr)
				{//ヒットボックスのポインタがnullではなかったら
					m_pHitbox->SetEffect(CHitbox::EFFECT_MAX);					//エフェクトを戻す
					m_pHitbox->SetDirection(Vec3Null);							//無敵状態を設定する
				}
			}

			break;

			default:
				break;
			}
		}
		if (m_nCntAttack > 0)
		{//攻撃中だったら、
			m_nCntAttack--;				//攻撃カウンターを更新する

			if (m_nCntAttack == 15 && m_pAttackHitbox == nullptr)
			{//攻撃アニメションが始まったら、攻撃のヒットボックスがnullだったら、

				//スポーンの位置を計算する
				D3DXVECTOR3 Rot = Vec3Null;

				if (m_pModel[0] != nullptr)
				{
					Rot = m_pModel[0]->GetRot();
				}
				D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, 20.0f, 20.0f);
				D3DXMATRIX mtxOut ,mtxTrans ,mtxRot;
				D3DXMatrixIdentity(&mtxOut);
				D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, 0.0f, 0.0f);
				D3DXMatrixMultiply(&mtxOut, &mtxOut, &mtxRot);
				D3DXVec3TransformCoord(&dir, &dir, &mtxOut);

				//ヒットボックスの生成
				m_pAttackHitbox = CBoxHitbox::Create(dir + m_pos, Vec3Null, D3DXVECTOR3(17.0f, 14.0f, 17.0f), CHitbox::TYPE_OBSTACLE, this, 0, CHitbox::EFFECT_PUSH);
				
				if (m_pAttackHitbox != nullptr)
				{//生成出来たら
					dir.y = 0.0f;
					D3DXVec3Normalize(&dir, &dir);
					m_pAttackHitbox->SetDirection(dir);		//向きの設定
				}
			}

			if (m_nCntAttack <= 0)
			{//カウンターが0以下になったら
				m_pAttackHitbox->Release();			//ヒットボックスを破棄する
				m_pAttackHitbox = nullptr;			//ポインタをnullにする
				m_nCntAttack = 0;					//カウンターを0に戻す
				m_bAttacking = false;				//攻撃フラグをfalseにする
			}
		}
		if (m_pHeadHitbox != nullptr)
		{//頭のヒットボックスがnullではなかったら
			m_pHeadHitbox->SetPos(m_pos);			//位置の更新
			m_pHeadHitbox->Update();				//更新処理
		}
		
		CPlayer *m_pPlayer[PLAYER_MAX] = {};

		for (int nCnt = 0; nCnt < PLAYER_MAX; nCnt++)
		{
			m_pPlayer[nCnt] = CStage::GetPlayer(nCnt);
		}

		//全員が振り向き終わったら
		if (m_pPlayer[0]->m_bRot &&m_pPlayer[1]->m_bRot &&m_pPlayer[2]->m_bRot &&m_pPlayer[3]->m_bRot)
		{
			//カメラを移動する
			pCamera->SetPos(D3DXVECTOR3(pCamera->GetPos().x, -80.0f, 800.0f), pCamera->GetPos());

			m_nFrame++;

			if (m_nFrame >= 60)
			{
				//勝者が前に出る
				MoveWinner();
			}
		}

		if (m_pScoreUI != nullptr && m_pScore != nullptr)
		{//スコアUIの更新処理

			//スコアを取得して、4桁の文字列に変換する
			int nScore = m_pScore->GetScore();
			std::string str = std::to_string(nScore);
			std::string begin = {};

			if (str.size() < 4)
			{
				for (int nCnt = 0; nCnt < 4 - (int)str.size(); nCnt++)
				{
					begin += '0';
				}

				begin += str;
			}
			else
			{
				begin = str;
			}

			const char* pStr = begin.c_str();
			m_pScoreUI->ChangeString(pStr);				//UIの文字列の設定
		}
	}

	//デバッグ用の文字列
	CDebugProc::Print("\nRot: %f\nRot Dest: %f\n\nPos: %f, %f, %f", m_pModel[BODY]->GetRot().y, m_DestRot.y, m_pos.x, m_pos.y, m_pos.z);
	CDebugProc::Print("\nPlayer %d score: %d", m_nIdxPlayer, m_pScore->GetScore());
}

//描画処理
void CPlayer::Draw(void)
{
	if (m_nInvincibilityCnt % 10 <= 5)
	{//無敵状態だったら、点滅させる

		D3DXMATRIX mtxTrans, mtxRot;												//計算用のマトリックス
		D3DXMatrixIdentity(&m_mtxWorld);											//ワールドマトリックスの初期化処理

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		m_pModel[BODY]->Draw(m_mtxWorld);											//最初のモデルの描画処理

		for (int nCnt = 1; nCnt < PARTS_MAX; nCnt++)
		{//他のモデルの描画処理
			if (m_pModel[nCnt] != nullptr)
			{
				m_pModel[nCnt]->Draw();
			}
		}
	}
}

//=============================================================================
//
//								静的関数
//
//=============================================================================

//生成処理
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, int nCntPlayer)
{
	CPlayer* pModel = new CPlayer;		//インスタンスを生成する

	//プレイヤーの初期化処理
	if (FAILED(pModel->Init()))
	{
		return nullptr;
	}

	CModel::ModelType pType = CPlayerModel::GetHeadType(nCntPlayer);			//選択された頭の種類の取得

	pModel->m_pos = pos;																									//位置の設定
	pModel->m_pModel[BODY] = CModelPart::Create(CModel::MODEL_BODY, D3DXVECTOR3(0.0f, 17.0f, 0.0f), Vec3Null);				//体のモデルを生成する

	pModel->m_pModel[HEAD] = CModelPart::Create(pType, D3DXVECTOR3(0.0f, 7.0f, 0.0f), Vec3Null);							//頭のモデルを生成する
	pModel->m_pModel[HEAD]->SetParent(pModel->m_pModel[BODY]);																//頭の親を設定する

	pModel->m_pModel[LEFT_ARM] = CModelPart::Create(CModel::MODEL_LEFT_ARM, D3DXVECTOR3(8.0f, 3.0f, 0.0f), Vec3Null);		//左腕のモデルを生成する
	pModel->m_pModel[LEFT_ARM]->SetParent(pModel->m_pModel[BODY]);															//左腕の親を設定する

	pModel->m_pModel[LEFT_HAND] = CModelPart::Create(CModel::MODEL_LEFT_HAND, D3DXVECTOR3(9.0f, 0.0f, 0.0f), Vec3Null);		//左手のモデルを生成する
	pModel->m_pModel[LEFT_HAND]->SetParent(pModel->m_pModel[LEFT_ARM]);														//左手の親を設定する

	pModel->m_pModel[RIGHT_ARM] = CModelPart::Create(CModel::MODEL_RIGHT_ARM, D3DXVECTOR3(-8.0f, 3.0f, 0.0f), Vec3Null);	//右腕のモデルを生成する
	pModel->m_pModel[RIGHT_ARM]->SetParent(pModel->m_pModel[BODY]);															//右腕の親を設定する

	pModel->m_pModel[RIGHT_HAND] = CModelPart::Create(CModel::MODEL_RIGHT_HAND, D3DXVECTOR3(-9.0f, 0.0f, 0.0f), Vec3Null);	//右手のモデルを生成する
	pModel->m_pModel[RIGHT_HAND]->SetParent(pModel->m_pModel[RIGHT_ARM]);													//右手の親を設定する

	pModel->m_pModel[LEFT_LEG] = CModelPart::Create(CModel::MODEL_LEFT_LEG, D3DXVECTOR3(3.0f, -9.0f, 0.0f), Vec3Null);		//左太腿のモデルを生成する
	pModel->m_pModel[LEFT_LEG]->SetParent(pModel->m_pModel[BODY]);															//左太腿の親を設定する

	pModel->m_pModel[LEFT_FOOT] = CModelPart::Create(CModel::MODEL_LEFT_FOOT, D3DXVECTOR3(0.1f, -5.0f, 0.0f), Vec3Null);	//左足のモデルを生成する
	pModel->m_pModel[LEFT_FOOT]->SetParent(pModel->m_pModel[LEFT_LEG]);														//左足の親を設定する

	pModel->m_pModel[RIGHT_LEG] = CModelPart::Create(CModel::MODEL_RIGHT_LEG, D3DXVECTOR3(-3.0f, -9.0f, 0.0f), Vec3Null);	//右太腿のモデルを生成する
	pModel->m_pModel[RIGHT_LEG]->SetParent(pModel->m_pModel[BODY]);															//右太腿の親を設定する

	pModel->m_pModel[RIGHT_FOOT] = CModelPart::Create(CModel::MODEL_RIGHT_FOOT, D3DXVECTOR3(-0.1f, -5.0f, 0.0f), Vec3Null);//右足のモデルを生成する
	pModel->m_pModel[RIGHT_FOOT]->SetParent(pModel->m_pModel[RIGHT_LEG]);													//右足の親を設定する

	//生成したモデルをアニメーターに代入する
	std::vector <CModelPart*> vParts;
	vParts.clear();
	vParts.push_back(pModel->m_pModel[BODY]);
	vParts.push_back(pModel->m_pModel[HEAD]);
	vParts.push_back(pModel->m_pModel[LEFT_ARM]);
	vParts.push_back(pModel->m_pModel[LEFT_HAND]);
	vParts.push_back(pModel->m_pModel[RIGHT_ARM]);
	vParts.push_back(pModel->m_pModel[RIGHT_HAND]);
	vParts.push_back(pModel->m_pModel[LEFT_LEG]);
	vParts.push_back(pModel->m_pModel[LEFT_FOOT]);
	vParts.push_back(pModel->m_pModel[RIGHT_LEG]);
	vParts.push_back(pModel->m_pModel[RIGHT_FOOT]);

	pModel->m_pAnimator = CAnimator::Create(&vParts, CAnimator::ANIM_TYPE_PLAYER);

	//ヒットボックスの生成
	pModel->m_pHitbox = CCylinderHitbox::Create(pos, Vec3Null, D3DXVECTOR3(10.0f, 35.0f, 10.0f), CHitbox::TYPE_PLAYER, pModel, nCntPlayer);
	pModel->m_pHeadHitbox = CCylinderHitbox::Create(pos, D3DXVECTOR3(0.0f, 35.0f, 0.0f), D3DXVECTOR3(1.0f, 2.0f, 1.0f), CHitbox::TYPE_PLAYER, pModel);

	//プレイヤーのインデックス
	pModel->SetPlayerIdx(nCntPlayer);

	//スコアの生成
	pModel->m_pScore = CScore::Create(nCntPlayer);

	D3DXCOLOR UIcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);				//UI色の設定

	//選択された色の取得と設定
	D3DXCOLOR col = CPlayerModel::GetPlayerColors()[nCntPlayer];		

	pModel->m_pModel[BODY]->SetModelColor(2, col);
	UIcol = col;

	D3DXVECTOR3 fieldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// モードで取得するメッシュフィールドを分ける
	fieldPos = CGameRace::GetStage()->GetField()->GetPos();

	pModel->m_TargetPos = D3DXVECTOR3(-75.0f + (50.0f * (nCntPlayer)), -150.0f, fieldPos.z - 100.0f);

	//UIの生成
	pModel->m_pScoreUI = CUIString::Create(D3DXVECTOR3(100.0f + (320.0f * nCntPlayer), SCREEN_HEIGHT - 80.0f, 0.0f), D3DXVECTOR2(150.0f, 50.0f), UIcol, "0000", 5);
	
	return pModel;					//生成したインスタンスを返す
}

//プレイヤーの色の取得処理
D3DXCOLOR* CPlayer::GetPlayerColors(void)
{
	return m_playerColor;
}

//プレイヤーのキー処理
void CPlayer::PlayerController(int nCntPlayer)
{
	D3DXVECTOR3 cameraRot = CApplication::GetCamera()->GetRot();					//カメラの向きの取得処理
	D3DXVECTOR3 cR = D3DXVECTOR3(-cosf(cameraRot.y), 0.0f, sinf(cameraRot.y));
	float fA = acosf(cR.x);

	if (!m_bDebug && m_nIdxPlayer == 0)
	{// デバッグ中ではない場合 & プレイヤー番号が0の場合
		if (!m_bAttacking)
		{
			//移動量と目的の角度の計算
			if (CInputKeyboard::GetKeyboardPress(DIK_W))
			{//Wキーが押された場合
				if (CInputKeyboard::GetKeyboardPress(DIK_A))
				{//Aキーも押された場合
					if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
					{
						m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
					}
					if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
					{
						m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
					}

					m_DestRot.y = -D3DX_PI * 0.75f + fA;
				}
				else if (CInputKeyboard::GetKeyboardPress(DIK_D))
				{//Dキーも押された場合
					if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
					{
						m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
					}
					if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
					{
						m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
					}

					m_DestRot.y = -D3DX_PI * 0.25f + fA;
				}
				else
				{//Wキーだけが押された場合
					if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
					{
						m_move.x += m_AccelerationCoeff * cosf(cameraRot.y) * (m_fFrictionCoeff);
					}
					if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
					{
						m_move.z += m_AccelerationCoeff * sinf(cameraRot.y) * (m_fFrictionCoeff);
					}

					m_DestRot.y = -D3DX_PI * 0.5f + fA;
				}
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_S))
			{//Sキーが押された場合
				if (CInputKeyboard::GetKeyboardPress(DIK_A))
				{//Aキーも押された場合
					if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
					{
						m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
					}
					if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
					{
						m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
					}

					m_DestRot.y = D3DX_PI * 0.75f + fA;
				}
				else if (CInputKeyboard::GetKeyboardPress(DIK_D))
				{//Dキーも押された場合
					if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
					{
						m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
					}
					if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
					{
						m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
					}

					m_DestRot.y = D3DX_PI * 0.25f + fA;
				}
				else
				{//Sキーだけが押された場合
					if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
					{
						m_move.x += m_AccelerationCoeff * cosf(D3DX_PI + cameraRot.y) * (m_fFrictionCoeff);
					}
					if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
					{
						m_move.z += m_AccelerationCoeff * sinf(D3DX_PI + cameraRot.y) * (m_fFrictionCoeff);
					}

					m_DestRot.y = D3DX_PI * 0.5f + fA;
				}
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_D))
			{//Dキーだけ押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = fA;
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_A))
			{//Aキーだけ押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
				}
				m_DestRot.y = D3DX_PI + fA;
			}
		}

		//SPACEキーが押された場合
		if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE) && !m_bJump && !m_bAttacking && m_move.y < 0.0f)
		{//ジャンプ
			m_move.y = 18.0f;
			m_bJump = true;
			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMP);
			m_pAnimator->SetPresentAnim(2);
		}

		//移動キーが押されている時
		if (CInputKeyboard::GetKeyboardPress(DIK_W) || CInputKeyboard::GetKeyboardPress(DIK_S) || CInputKeyboard::GetKeyboardPress(DIK_A) || CInputKeyboard::GetKeyboardPress(DIK_D))
		{
			if (!m_bJump && !m_bHit && !m_bAttacking)
			{
				m_pAnimator->SetLoopingAnim(1);
			}
		}

		//攻撃キーが押されている時
		if (CInputKeyboard::GetKeyboardTrigger(DIK_V) && !m_bJump && !m_bHit && !m_bAttacking)
		{
			m_pAnimator->SetPresentAnim(3);

			m_bAttacking = true;
			m_nCntAttack = 19;

			CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_PUNCH);
		}
	}

#ifdef _DEBUG
	if (!m_bAttacking)
	{
		//移動量と目的の角度の計算
		if (CInputKeyboard::GetKeyboardPress(DIK_W))
		{//Wキーが押された場合
			if (CInputKeyboard::GetKeyboardPress(DIK_A))
			{//Aキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = -D3DX_PI * 0.75f + fA;
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_D))
			{//Dキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = -D3DX_PI * 0.25f + fA;
			}
			else
			{//Wキーだけが押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = -D3DX_PI * 0.5f + fA;
			}
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_S))
		{//Sキーが押された場合
			if (CInputKeyboard::GetKeyboardPress(DIK_A))
			{//Aキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = D3DX_PI * 0.75f + fA;
			}
			else if (CInputKeyboard::GetKeyboardPress(DIK_D))
			{//Dキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = D3DX_PI * 0.25f + fA;
			}
			else
			{//Sキーだけが押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = D3DX_PI * 0.5f + fA;
			}
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_D))
		{//Dキーだけ押された場合
			if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
			{
				m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}
			if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
			{
				m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}

			m_DestRot.y = fA;
		}
		else if (CInputKeyboard::GetKeyboardPress(DIK_A))
		{//Aキーだけ押された場合
			if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
			{
				m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}
			if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
			{
				m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}
			m_DestRot.y = D3DX_PI + fA;
		}
	}

	//SPACEキーが押された場合
	if (CInputKeyboard::GetKeyboardTrigger(DIK_SPACE) && !m_bJump && !m_bAttacking && m_move.y < 0.0f)
	{//ジャンプ
		m_move.y = 18.0f;
 		m_bJump = true;
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMP);
		m_pAnimator->SetPresentAnim(2);
	}

	//移動キーが押されている時
	if (CInputKeyboard::GetKeyboardPress(DIK_W) || CInputKeyboard::GetKeyboardPress(DIK_S) || CInputKeyboard::GetKeyboardPress(DIK_A) || CInputKeyboard::GetKeyboardPress(DIK_D))
	{
		if (!m_bJump && !m_bHit && !m_bAttacking)
		{
			m_pAnimator->SetLoopingAnim(1);
		}
	}

	//攻撃キーが押されている時
	if (CInputKeyboard::GetKeyboardTrigger(DIK_V) &&  !m_bJump && !m_bHit && !m_bAttacking)
	{
		m_pAnimator->SetPresentAnim(3);

		m_bAttacking = true;
		m_nCntAttack = 19;

		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_PUNCH);
	}
#endif // !DEBUG
	if (!m_bAttacking)
	{
		//移動量と目的の角度の計算
		if (CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).y < -0.3f)
		{//Wキーが押された場合
			if ( CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).x < -0.3f)
			{//Aキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = -D3DX_PI * 0.75f + fA;
			}
			else if ( CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).x > 0.3f)
			{//Dキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.25f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = -D3DX_PI * 0.25f + fA;
			}
			else
			{//Wキーだけが押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = -D3DX_PI * 0.5f + fA;
			}
		}
		else if (CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).y > 0.3f)
		{//Sキーが押された場合
			if (CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).x < -0.3f)
			{//Aキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = D3DX_PI * 0.75f + fA;
			}
			else if (CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).x > 0.5f)
			{//Dキーも押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.75f + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = D3DX_PI * 0.25f + fA;
			}
			else
			{//Sキーだけが押された場合
				if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
				{
					m_move.x += m_AccelerationCoeff * cosf(D3DX_PI + cameraRot.y) * (m_fFrictionCoeff);
				}
				if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
				{
					m_move.z += m_AccelerationCoeff * sinf(D3DX_PI + cameraRot.y) * (m_fFrictionCoeff);
				}

				m_DestRot.y = D3DX_PI * 0.5f + fA;
			}
		}
		else if (CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).x > 0.3f)
		{//Dキーだけ押された場合
			if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
			{
				m_move.x += m_AccelerationCoeff * cosf(-D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}
			if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
			{
				m_move.z += m_AccelerationCoeff * sinf(-D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}

			m_DestRot.y = fA;
		}
		else if (CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).x < -0.3f)
		{//Aキーだけ押された場合
			if (m_move.x <= m_MaxWalkingSpeed && m_move.x >= -m_MaxWalkingSpeed)
			{
				m_move.x += m_AccelerationCoeff * cosf(D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}
			if (m_move.z <= m_MaxWalkingSpeed && m_move.z >= -m_MaxWalkingSpeed)
			{
				m_move.z += m_AccelerationCoeff * sinf(D3DX_PI * 0.5f + cameraRot.y) * (m_fFrictionCoeff);
			}
			m_DestRot.y = D3DX_PI + fA;
		}
	}

	//パッドのAボタンが押された時
	if (CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_A, nCntPlayer) && !m_bJump && !m_bAttacking && m_move.y < 0.0f)
	{//ジャンプ
		m_move.y = 18.0f;
		m_bJump = true;
		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_JUMP);
		m_pAnimator->SetPresentAnim(2);
	}

	//パッドのスティックが入力されている時
	if (CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).x  < -0.3f || CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).x  > 0.3f
		|| CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).y  < -0.3f || CInputPad::GetJoypadStick(CInputPad::JOYKEY_LEFT_STICK, nCntPlayer).y  > 0.3f)
	{
		if (!m_bJump && !m_bHit && !m_bAttacking)
		{
			m_pAnimator->SetLoopingAnim(1);
		}
	}

	//パッドのBボタンが押されている時
	if (CInputPad::GetJoypadTrigger(CInputPad::JOYKEY_B, nCntPlayer) && !m_bJump && !m_bHit && !m_bAttacking)
	{
		m_pAnimator->SetPresentAnim(3);

		m_bAttacking = true;
		m_nCntAttack = 19;

		CApplication::GetSound()->Play(CSound::SOUND_LABEL_SE_PUNCH);
	}
}

//プレイヤー番号設定処理
void CPlayer::SetPlayerIdx(int nCntPlayer)
{
	m_nIdxPlayer = nCntPlayer;
}

void CPlayer::SetFriction(const float fFriction)
{
	m_fFrictionCoeff = fFriction;
}

//振り向き取得処理
bool CPlayer::GetRotCmp()
{
	return m_bRot;
}

//=====================================
// 勝者の移動処理
//=====================================
void CPlayer::MoveWinner()
{
	if (m_bWinner && GetRotCmp())
	{
		if (!m_bPos)
		{
			GoalPos = m_pos - D3DXVECTOR3(0.0f, 0.0f, 50.0f);
			m_bPos = true;
		}

		m_fAngle = sqrtf((float)(pow(GoalPos.x - m_pos.x, 2) + pow(GoalPos.z - m_pos.z, 2)));
		m_move.x = (GoalPos.x - m_pos.x) / (m_fAngle / 1.0f);
		m_move.z = (GoalPos.z - m_pos.z) / (m_fAngle / 1.0f);

		if (m_pos.z <= GoalPos.z)
		{
			m_move = Vec3Null;
			m_bMove = true;
		}

		WinnerAnim();
	}
	else if(GetRotCmp())
	{
		LoserAnim();
	}
}

//勝者設定処理
void CPlayer::SetWinner(bool bWinner)
{
	m_bWinner = bWinner;
}

//勝者アニメーション処理
void CPlayer::WinnerAnim()
{
	m_pAnimator->SetPresentAnim(5);
}

//敗者アニメーション処理
void CPlayer::LoserAnim()
{
	m_pAnimator->SetPresentAnim(6);
}

//復活処理
void CPlayer::PlayerRespawn()
{
	if (m_pos.y <= -500.0f)
	{
		D3DXVECTOR3 posCamera = CApplication::GetCamera()->GetPos();
		m_pos = D3DXVECTOR3(posCamera.x, posCamera.y + 100.0f, posCamera.z + 100.0f);
		m_fFrictionCoeff = 0.1f;

		if (m_pHitbox != nullptr)
		{
			m_pHitbox->SetInvincibility(true);
		}

		if (m_pScore != nullptr)
		{
			m_pScore->AddScore(-30);
		}

		m_nInvincibilityCnt = 180;
	}
}

//ゴール後の動き処理
void CPlayer::GoalMove()
{
	D3DXVECTOR3 cameraRot = CApplication::GetCamera()->GetRot();
	D3DXVECTOR3 cR = D3DXVECTOR3(-cosf(cameraRot.y), 0.0f, sinf(cameraRot.y));
	float fA = acosf(cR.x);

	if (!m_bGoal)
	{
		return;
	}

	// 対象までの角度の算出
	m_fAngle = sqrtf((float)(pow(m_TargetPos.x - m_pos.x, 2) + pow(m_TargetPos.z - m_pos.z, 2)));
	m_move.x = (m_TargetPos.x - m_pos.x) / (m_fAngle / 1.0f);
	m_move.z = (m_TargetPos.z - m_pos.z) / (m_fAngle / 1.0f);

	if (m_pos.z >= m_TargetPos.z)
	{
		m_move = Vec3Null;
		m_DestRot.y = D3DX_PI * 0.5f + fA;
	}
	
	if(m_rot.y <= m_DestRot.y * 2.0f)
	{
		m_bRot = true;
	}
}
