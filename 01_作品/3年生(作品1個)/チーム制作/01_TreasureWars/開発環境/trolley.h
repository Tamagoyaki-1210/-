//==============================================================================================
//
// トロッコ　　　trolley.h
// tutida ryousei
//
//==============================================================================================
#ifndef _TROLLEY_H__
#define	_TROLLEY_H_

#include"trolley_manager.h"

class CObjectX;
class CModel;

class CTrolley : public CObject
{
public:
	CTrolley();
	~CTrolley();

	// モーションのタイプ
	enum Trolley_Motion_Type
	{
		TROLLEY_MOTION_MOVE = 0,			// 移動
		MOTION_MAX,
		MOTION_NONE
	};

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetTrolleyInfomation();			// トロッコの初期情報を設定
	void SetStart(const int index);			// 初期位置の設定
	void SetMotionData(int maxkey, int key, int parts, int motion,
		int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop);				// モーション値の読み込み
	void Motion();				// モーションの設定
	void SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename);
	void Trolley_Collision();	// 当たり判定
	void Turn();				// 巡回地点を回る
	void Level(const bool up);		// レベルの処理
	void NormalizeRot();		// 角度の正規化
	void LoadFileGimmick(const char *Xfilename);

	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }
	void SetTrolley_Index(int index) { m_nIndex = index; }

	D3DXVECTOR3 GetPos() { return m_Pos; }
	int GetLevel() { return m_nLevel; }
	float GetRadius() { return m_fRadius; }

private:
	static const int m_nNum_GimmickParts = 13;		// パーツ数
	static const int m_nNum_GimmickKey = 5;			// キーの最大数
	static const int Level_Down_Timer = 300;		// レベルダウンまでの時間

	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXVECTOR3 m_TurnPos[CTrolley_Manager::Max_TurnPos];		// 巡回地点
	D3DXVECTOR3 m_Start_Pos[CTrolley_Manager::Max_StartPos];	// 初期位置
	D3DXVECTOR3 InitPos[m_nNum_GimmickParts];					// パーツの初期位置
	D3DXVECTOR3 InitRot[m_nNum_GimmickParts];					// パーツの初期角度
	D3DXVECTOR3 m_Pos;			// 位置
	D3DXVECTOR3 m_Rot;			// 角度
	D3DXVECTOR3 m_Rot_Dest;		// 目的の角度
	D3DXVECTOR3 m_Next_Vec;		// 次の目的地までのベクトル
	int m_nLevel;				// トロッコのレベル
	int m_nLevelDownCounter;	// レベルダウンまでの時間
	int m_nTurnNum;				// 巡回地点の数
	int m_nStartNum;			// スタート地点の数
	float m_fSpeed[CTrolley_Manager::MaxLevel];	// 移動速度
	int m_nNextTurn_Index;		// 次の目的地の番号
	int m_nNextTurn_IndexInit;	// 最初の目的地の番号
	int m_nTurn;				// どっち回転か
	int m_nIndex;				// 番号
	float m_fCp;				// 外積
	float m_fRadius;			// 半径
	bool m_bReturn;				// 初期位置に戻る
	bool m_bOneTurn;			// 一周したか
	bool m_bBack;				// 後退してるか

	CModel *m_pModel[m_nNum_GimmickParts];	// モデルの情報

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
		KEY aKey[m_nNum_GimmickParts];
	};

	// モーションセット
	struct MOTION_SET
	{
		int nMaxKey;			// キーの総数
		bool bLoop;				// ループするか
		bool bStop;				// モーションが止まったか
		KEY_SET aKeySet[m_nNum_GimmickKey];
	};
	MOTION_SET m_MotionSet;
	int m_nCurrentMotion;		// 現在のモーション番号
	int m_nCountMotion;							// モーションカウンター
	int m_nCurrentKey;							// 現在のキー番号
	Trolley_Motion_Type m_Motion;				// モーション
};

#endif // !_TROLLEY_MANAGER_H_
