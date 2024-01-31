//=============================================================================
//
// enemy.h
// Author : Tanimoto Kosuke
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "object.h"
#include "model.h"
#include <vector>

//---------------------------
// クラス宣言
//---------------------------
class CEnemy : public CObject
{
public:

	CEnemy();
	virtual ~CEnemy() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Move();						// 移動処理
	void Motion();						// モーションの設定
	virtual void ChangeMotion();		// モーションの切り替え
	const float NormalizeRot();			// 角度の正規化
	void Death();						// 自身の終了処理

	void LoadFileEnemy(const char *Xfilename);

	CModel* SetModel(int index, int parent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *Xfilename);								// モデルパーツの設定
	void SetMotionData(int maxkey, int key, int parts, int motion, int frame, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool loop);	// モーション値の読み込み

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void AddMove(const D3DXVECTOR3 move) { m_move += move; }
	void SetRotDest(const D3DXVECTOR3 rot) { m_rotDest = rot; }
	void AddRotDest(const D3DXVECTOR3 rot) { m_rotDest += rot; }
	void SetKnockBack(const int power) { m_nKnockBack_Power = power; }
	void SetMotion(const int motion) { m_nMotion = motion; }
	void SetCurrentMotion(const int motion) { m_nCurrentMotion = motion; }
	void SetRadius(const float radius) { m_fColRadius = radius; }
	void SetSpeed(const float speed) { m_fSpeed = speed; }
	
	const D3DXVECTOR3 GetPos() { return m_pos; }
	const D3DXVECTOR3 GetRot() { return m_rot; }
	const int GetKnockBack() { return m_nKnockBack_Power; }
	const int GetMotion() { return m_nMotion; }
	const int GetCurrentMotion() { return m_nCurrentMotion; }
	const float GetRadius() { return m_fColRadius; }				// サイズ(半径)
	std::vector<CModel*> GetModel() { return m_pModel; }

private:
	static const float Move_Inertia;	// 敵キャラの移動慣性
	static const float Enemy_Rot_Speed;	// 敵キャラの回転する速度

	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_posOld;				// 過去の位置
	D3DXVECTOR3 m_move;					// 移動量
	D3DXVECTOR3 m_rot;					// 現在の角度
	D3DXVECTOR3 m_rotDest;				// 目的の角度
	std::vector<D3DXVECTOR3> m_InitPos;	// パーツの初期位置
	std::vector<D3DXVECTOR3> m_InitRot;	// パーツの初期角度
	D3DXCOLOR m_col;					// モデルの色

	float m_fColRadius;					// 当たり判定の大きさ
	float m_fSpeed;						// 移動速度
	float m_fRotSpeed;					// 回転のスピード

	int m_nKnockBack_Power;				// ノックバックさせる力
	int m_nMotion;						// モーション
	int m_nCurrentMotion;				// 現在のモーション番号
	int m_nCountMotion;					// モーションカウンター
	int m_nCurrentKey;					// 現在のキー番号
	bool m_bNullModel;					// モデルがnull
	std::vector<CModel*> m_pModel;		// モデルの情報

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
		std::vector<KEY> aKey;
	};

	// モーションセット
	struct MOTION_SET
	{
		int nMaxKey;			// キーの総数
		bool bLoop;				// ループするか
		bool bStop;				// モーションが止まったか
		std::vector<KEY_SET> aKeySet;
	};
	std::vector<MOTION_SET> m_MotionSet;
};

#endif// _ENEMY_H_