//=============================================================================
//
// player.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "object.h"

//=============================================================================
//前方宣言
//=============================================================================
class CModelPart;
class CAnimator;
class CCylinderHitbox;
class CScore;
class CUIString;
class CBoxHitbox;

class CPlayer : public CObject
{
public:

	enum Parts
	{
		BODY = 0,								//体
		HEAD,									//頭
		LEFT_ARM,								//左腕
		LEFT_HAND,								//左手
		RIGHT_ARM,								//右腕
		RIGHT_HAND,								//右手
		LEFT_LEG,								//左足
		LEFT_FOOT,								//左太腿
		RIGHT_LEG,								//右足
		RIGHT_FOOT,								//右太腿
		PARTS_MAX
	};

	enum STATE
	{
		STATE_NEUTRAL = 0,						//ニュートラル
		STATE_RUNNING,							//走る
		STATE_JUMP,								//ジャンプ
		STATE_PUNCH,							//パンチ
		STATE_DAMAGE,							//ダメージ
		STATE_MAX								
	};

	enum PlayerColor
	{
		PLAYER_RED = 0,							//赤
		PLAYER_GREEN,							//緑
		PLAYER_BLUE,							//青
		PLAYER_YELLOW,							//黄
		PLAYER_MAGENTA,							//マゼンタ
		PLAYER_CYAN,							//シアン
		PLAYER_BLACK,							//黒
		PLAYER_WHITE,							//白

		PLAYER_COLOR_MAX
	};

	CPlayer();															//コンストラクタ
	~CPlayer() override;												//デストラクタ

	HRESULT Init(void) override;										//初期化処理
	void Uninit(void) override;											//終了処理
	void Update(void) override;											//更新処理
	void Draw(void) override;											//描画処理

	void SetPos(const D3DXVECTOR3 pos) override { m_pos = pos; }		//位置の設定処理
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }					//位置の設定処理

	const D3DXVECTOR2 GetSize(void) override { return Vec2Null; }		//サイズの取得処理
	const D3DXVECTOR3 GetPos(void) override { return m_pos; }			//位置の取得処理

	D3DXVECTOR3 GetDestRot(void) { return m_DestRot; }					//目的の角度の取得処理

	static CPlayer* Create(const D3DXVECTOR3 pos,int nCntPlayer);		//生成処理
	static D3DXCOLOR* GetPlayerColors(void);							//プレイヤーの色の取得処理

	void PlayerController(int nCntPlayer);								//プレイヤーのキー処理
	void SetPlayerIdx(int nCntPlayer);									//プレイヤー番号の設定
	void SetFriction(const float fFriction);

	void MoveWinner();													//勝者の移動処理
	void SetWinner(bool bWinner);										//勝者の設定処理
	void WinnerAnim();													//勝者のアニメーション
	void LoserAnim();													//敗者のアニメーション
	void PlayerRespawn();												//プレイヤー復活処理

	bool GetGoal() { return m_bGoal; }									//ゴールの状態取得
	bool GetRotCmp();													//ゴール後の回転情報の取得

private:
	void GoalMove();													//ゴール後の動き

	static D3DXCOLOR m_playerColor[PLAYER_COLOR_MAX];					//プレイヤーの色
	static const float m_MaxWalkingSpeed;								//最大の歩くスピード
	static const float m_AccelerationCoeff;								//加速係数
	static int m_nRanking;												//順位

	D3DXVECTOR3 m_pos;													//位置
	D3DXVECTOR3 m_move;													//速度
	D3DXVECTOR3 m_rot;													//向き
	D3DXVECTOR3 m_DestRot;												//目的の角度
	D3DXMATRIX  m_mtxWorld;												//ワールドマトリックス
	int			m_nInvincibilityCnt;									//無敵状態のカウンター
	int			m_nCntAttack;											//攻撃カウンター
	int			m_nPlayerRanking;										//プレイヤーの順位
	int			m_nIdxPlayer;											//プレイヤー番号
	int			m_nFrame;												//フレーム数
	float		m_fFrictionCoeff;										//摩擦係数
	float		m_fAngle;														
	bool		m_bJump;												//ジャンプしているかどうか
	bool		m_bGoal;												//ゴールしたかどうか
	bool		m_bMove;												//動き切ったかどうか
	bool		m_bWinner;												//勝利したかどうか
	bool		m_bPos;													//位置についたかどうか
	bool		m_bRot;													//回転したかどうか
	bool		m_bHit;													//当たったかどうか
	bool		m_bPunch;												//パンチしたかどうか
	bool		m_bAttacking;											//アタックしたかどうか
	bool		m_bDebug;												//デバッグ判定

	D3DXVECTOR3 m_TargetPos;											//目標の位置
	D3DXVECTOR3 GoalPos;												//ゴール判定の位置
	STATE m_State;														//プレイヤーの状態

	CModelPart* m_pModel[PARTS_MAX];									//モデルへのポインタ
	CAnimator* m_pAnimator;												//アニメーターへのポインタ
	CCylinderHitbox* m_pHitbox;											//ヒットボックス
	CCylinderHitbox* m_pHeadHitbox;										//頭のヒットボックス
	CBoxHitbox* m_pAttackHitbox;										//攻撃ヒットボックス
	CScore* m_pScore;													//スコアのポインタ
	CUIString* m_pScoreUI;												//スコアUIのポインタ
};

#endif