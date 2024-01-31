//==============================================================================================
// 
// キャラクター　　　character.h
// tutida ryousei
// 
//==============================================================================================
#ifndef _CHARACTER_H_
#define	_CHARACTER_H_

#include"object.h"
#include"model.h"

class CGauge;
class CScore;
class CScoreItemStack;
class CObject3D;
class CObjectX;
class CShop_Direction;

class CCharacter : public CObject
{
public:
	static const int Max_GemStack = 20;				// キャラの宝石所持数の最大数
	static const float Attack_Angle;				// キャラの攻撃範囲(扇型)

	CCharacter();
	~CCharacter();

	// モーションのタイプ
	enum Motion_Type
	{
		MOTION_NEUTRAL = 0,		// ニュートラル
		MOTION_WALK,			// 歩き
		MOTION_MINE,			// 掘る
		MOTION_KNOCKBACK,		// ノックバック
		MOTION_MAX,
		MOTION_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	virtual void Move();	// 移動
	void SetMotionData(int maxkey, int key, int parts, int motion,
	int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop);				// モーション値の読み込み
	void Motion();							// モーションの設定
	void ChangeMotion();		// モーションの切り替え
	CModel *SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename);	// モデルパーツの設定
	void ResetCharaColor();								// キャラ毎の色の初期化
	void SetCrown(const bool crown);	// 王冠の表示設定
	void NormalizeRot();	// 角度の正規化
	void Walk();			// 歩き
	void Item_Effect(const int type);		// アイテム取得時の効果
	void Attack();			// 攻撃処理
	void CMine();			// 採掘
	void KnockBack_Set(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, int knockback_power, int attackpower);	// ノックバックの設定
	void KnockBack();		// ノックバック
	void Flash();			// 無敵処理
	void Damage(const int power);	// 被弾処理
	void SetStackScoreItem();	// スコアアイテムを持つ
	void CollObject();		// オブジェクトとの当たり判定
	void Before_Pos();		// 過去の位置を保存
	void TimeSlip();		// タイムスリップ
	void TimeSlip_CoolTime();
	void ShopCollision();	// 質屋との当たり判定
	void EnemyCollision();	// 敵キャラとの当たり判定
	void Shop_Direction();	// 質屋の方向
	void CharUx();			// キャラのUXを設定する
	void Create_ScoreItem();	// 背負うスコアアイテムの生成処理

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }	// 角度
	void SetRotDest(D3DXVECTOR3 rotdest) { m_rotDest = rotdest; }	// 目的の角度
	void SetSizeMin(D3DXVECTOR3 min) { m_SizeMin = min; }				// サイズの最小値
	void SetSizeMax(D3DXVECTOR3 max) { m_SizeMax = max; }				// サイズの最大値
	void SetMotion(Motion_Type motion) { m_Motion = motion; }			// モーション
	void SetMoveSpeed_Level(int speedlevel) { m_MoveSpeed_Level = speedlevel; }			// 移動速度のレベル
	void SetArmor_Level(int armorlevel) { m_nArmor_Level = armorlevel; }
	void SetGemStack_Level(int stacklevel) { m_nGemStack_Level = stacklevel; }			// 一度に持てる鉱石の量のレベル
	void SetNext(CCharacter *next) { m_pNextChar = next; }
	void SetMove(bool move) { m_bMove = move; }
	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	void SetKnockBack(bool knodkback) { m_bKnockBack = knodkback; }
	void SetKnockBackMove(D3DXVECTOR3 knockbackmove) { m_KnockBack = knockbackmove; }
	void SetKnockBack_Level(int knockbacklevel) { m_nKnockBack_Level = knockbacklevel; }
	void SetAttack(bool attack) { m_bAttack = attack; }
	void SetFlashing(bool flashing) { m_bFlashing = flashing; }
	void SetDraw(bool draw) { m_bDraw = draw; }
	void SetMoveSpeed(float speed) { m_fMoveSpeed = speed; }
	void SetMinePower(int power) { m_nMinePower = power; }
	void SetIndex(const int index) { m_nCharacterIndex = index; }
	void SetTimeSlip(bool timeslip) { m_bUseTimeSlip = timeslip; }
	void SetTimeSlip_SaveTime(int save) { m_nTimeSlip_Count = save; }
	void SetPlayTimeSlip(bool playtimeslip) { m_bPlayTimeSlip = playtimeslip; }
	void SetTime_Elapsed(bool timeelapsed) { m_bTime_Elapsed = timeelapsed; }
	void SetZeroOver(bool zeroover) { m_bZeroOver = zeroover; }
	void SetInvocation(bool invocation) { Invocation_TimeSlip = invocation; }

	D3DXVECTOR3 GetPos() { return m_pos; }			// 位置
	D3DXVECTOR3 GetRot() { return m_rot; }			// 角度
	D3DXVECTOR3 GetRotDest() { return m_rotDest; }	// 目的の角度
	D3DXVECTOR3 GetSizeMin() { return m_SizeMin; }						// サイズの最小値
	D3DXVECTOR3 GetSizeMax() { return m_SizeMax; }						// サイズの最大値
	Motion_Type GetMotion_Type() { return m_Motion; }					// モーション
	const float GetRadius() { return m_fRadius; }						// サイズ(半径)
	const float GetAttackLength() { return m_fAttackLength; }			// 攻撃射程
	const float GetAttackSize() { return m_fAttackSize; }				// 攻撃範囲(円形)
	float GetMoveSpeed() { return m_fMoveSpeed; }						// 移動速度
	int GetMoveSpeed_Level() { return m_MoveSpeed_Level; }				// 移動速度のレベル
	int GetArmor_Level() { return m_nArmor_Level; }						// アーマーのレベル
	int GetNumGemStack() { return m_nGemStack; }						// 一度に持てる鉱石の量
	int GetGemStack_Level() { return m_nGemStack_Level; }				// 一度に持てる鉱石の量のレベル
	int GetNamStack_ScoreItem() { return m_nNum_StackScoreItem; }		// 背負ってるスコアアイテムの数
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }						// ワールドマトリックス

	CCharacter *GetNext() { return m_pNextChar; }						// 次のキャラ
	bool GetKnockBack() { return m_bKnockBack; }						// ノックバック
	int GetKnockBack_Level() { return m_nKnockBack_Level;}				// ノックバックの初速
	bool GetFlashing() { return m_bFlashing; }							// 無敵
	bool GetMove() { return m_bMove; }									// 移動したか
	bool GetAttack() { return m_bAttack; }								// 攻撃したか
	int GetMinePower() { return m_nMinePower; }							// 採掘攻撃力
	int GetNum_StackScoreItem() { return m_nNum_StackScoreItem; }				// 持っているスコアアイテムの数
	const int GetIndex() { return m_nCharacterIndex; }					// キャラクターの番号
	bool GetPlayTimeSlip() { return m_bPlayTimeSlip; }					// タイムスリップ中か
	bool GetInvocation() { return Invocation_TimeSlip; }
	const bool GetSafe() { return m_bSafe; }							// 安全地帯の判定
	CScore *GetScore() { return m_pScore; }								// スコアの情報
	CScoreItemStack *GetGemStack() { return m_pScoreItemStack; }		// スコアアイテムスタックの情報
	bool GetInTerritory() { return m_bInTerritory; }					// 陣地内かどうかのGet
	void SetInTerritory(bool bbInTerritory) { m_bInTerritory = bbInTerritory; }	// 陣地内かどうかのSet
	bool GetEntry() { return m_bEntry; }

private:
	static const int m_nNumParts = 14;		// パーツ数
	static const int m_nNumKey = 5;			// キーの最大数
	static const int m_nNumMotion = MOTION_MAX;		// モーション数

	static const int First_Attack_Power = 3;		// プレイヤーへの初期攻撃力
	static const int KnockBack_MaxFrame = 60;		// ノックバックする時間
	static const int Flashing_MaxFrame = 120;		// 無敵時間
	static const int Flashing_SlowDown = 4;			// 点滅の減速
	static const int Attack_MaxFrame = 55;			// 攻撃の全体フレーム
	static const int Attack_Swing = 10;				// 攻撃の振り下ろすタイミング
	static const int Attack_Hit = 15;				// 攻撃の発生
	static const int Sale_Start_Timer = 60;			// 売却開始タイミング
	static const int Sale_Next_Timer = 10;			// 次の売却タイミング
	static const int m_nBefor_Frame = 300;			// タイムスリップで戻る時間
	static const int m_nTimeSlip_Speed = 3;			// タイムスリップの速度

	static const float Attack_Length;				// キャラの初期攻撃射程
	static const float Attack_Size;					// キャラの初期攻撃範囲(円形)
	static const float Attack_LevelUp;				// 攻撃力上昇に伴ったパワーアップ
	static const float Move_Rot_Speed;				// 移動の回転速度
	static const float Character_Radius;			// キャラの当たり判定のサイズ(半径)
	static const float Shop_Blink_Alpha_Speed;		// 質屋のアルファ値の点滅の速度
	static const float Shop_Blink_Max_Alpha;		// 質屋のアルファ値の点滅の最大値
	static const float Shop_Col_Radius;				// 質屋の当たり判定の大きさ

	D3DXMATRIX m_mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_posOld;				// 過去の位置
	D3DXVECTOR3 m_rot;				// 現在の角度
	D3DXVECTOR3 m_rotDest;			// 目的の角度
	D3DXVECTOR3 InitPos[m_nNumParts];			// パーツの初期位置
	D3DXVECTOR3 InitRot[m_nNumParts];			// パーツの初期角度
	D3DXVECTOR3 m_SizeMin;						// 頂点の最小値
	D3DXVECTOR3 m_SizeMax;						// 頂点の最大値モーション
	D3DXVECTOR3 m_nBefore_Pos[m_nBefor_Frame];	// 数秒前の位置
	D3DXVECTOR3 m_nBefore_Rot[m_nBefor_Frame];	// 数秒前の角度
	int m_nCurrentKey;							// 現在のキー番号
	int m_nCurrentMotion;						// 現在のモーション番号
	int m_nCountMotion;							// モーションカウンター
	int m_nTimeSlip_Count;						// タイムスリップのカウント
	static int m_nTimeSlip_CoolTime;			// タイムスリップのクールタイム
	static int m_nTimeSlip_CoolTime_Count;		// タイムスリップのクールタイムのカウント
	int m_nTimeSlip_Frame;
	int m_nGemStack;							// 一度に持てる鉱石の量
	int m_nSaleCount;							// ジェムを売る間隔
	int m_nShopBlinkAlphaCount;					// 質屋のアルファ値の点滅の間隔
	float m_fRadius;							// キャラの当たり判定の大きさの半径
	float m_fAttackLength;						// キャラの攻撃射程
	float m_fAttackSize;						// キャラの攻撃範囲(円形)
	float m_fMoveSpeed;							// 移動速度
	float m_fRotSpeed;							// 回転のスピード
	int m_nNum_StackScoreItem;					// 今持っているスコアアイテムの数
	float m_fCollisionRadius;					// キャラの当たり判定の大きさ
	float m_fSpeed;								// 移動速度
	char *m_XFileName[m_nNumParts];				// Xファイルネーム
	bool m_bNullModel;							// モデルがnull
	bool m_bMove;								// 動いているか
	bool m_bUseTimeSlip;						// タイムスリップするか
	bool m_bPlayTimeSlip;						// タイムスリップ中
	bool m_bTime_Elapsed;						// 一定時間経過
	bool m_bZeroOver;							// 0を超えた
	static int m_nTimeSlip_Charindex;			// タイムスリップを発動したキャラの番号
	static bool Invocation_TimeSlip;			// 誰かがタイムスリップを発動したか
	Motion_Type m_Motion;						// モーション
	CModel *m_pModel[m_nNumParts];				// モデルの情報
	CCharacter *m_pNextChar;					// 次のキャラクターの情報
	CObject3D *m_pCharacter_Type;				// キャラクターのタイプの表示

	bool m_bAttack;									// 攻撃したか
	bool m_bKnockBack;								// ノックバックしたか
	bool m_bFlashing;								// 点滅してるか
	bool m_bDraw;									// 描画するか
	bool m_bSafe;									// 安全地帯の中にいるかどうか
	int m_nKnockBack_Power;							// ノックバックさせる力
	int m_nWalkCounter;								// 歩きカウンター
	int m_nFlashCount;								// 点滅の間隔
	int m_nKnockBack_Frame;							// ノックバックする時間のカウント
	int m_nAttack_HitCount;							// 攻撃が終わるまでの時間
	int m_nMinePower;								// 採掘攻撃力
	int m_nAttackPower;								// プレイヤーへの攻撃力
	D3DXVECTOR3 m_Move;								// 移動量の距離
	D3DXVECTOR3 m_KnockBack;						// ノックバックの距離
	D3DXCOLOR m_charaCol;							// キャラ毎の色
	int m_nCharacterIndex;							// キャラクターの番号
	int m_nProbability;								// アイテムをドロップする確率
	CObjectX *m_Crown;								// 王冠
	CScore *m_pScore;								// スコアの情報
	CScoreItemStack *m_pScoreItemStack;				// スコアアイテムスタックの情報
	CModel *m_pOreStock[Max_GemStack];
	CShop_Direction *m_Shop_Direction;
	bool m_bEntry;

	int m_MoveSpeed_Level;			// 移動速度のレベル
	int m_nKnockBack_Level;			// ノックバックのレベル
	int m_nArmor_Level;				// アーマーレベル
	int m_nGemStack_Level;			// 一度に持てる鉱石量のレベル
	bool m_bInTerritory;			// 陣地内かどうか

	// キー
	struct KEY
	{
		D3DXVECTOR3 KeyPos;
		D3DXVECTOR3 KeyRot;
	};

	// キーセット
	struct KEY_SET
	{
		int nFrame;
		KEY aKey[m_nNumParts];
	};

	// モーションセット
	struct MOTION_SET
	{
		int nMaxKey;			// キーの総数
		bool bLoop;				// ループするか
		bool bStop;				// モーションが止まったか
		KEY_SET aKeySet[m_nNumKey];
	};
	MOTION_SET m_MotionSet[m_nNumMotion];
};

#endif // !_CHARACTER_H_