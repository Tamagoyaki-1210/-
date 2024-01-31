//==============================================================================================
//
// 動くオブジェクト　　　move_object.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _MOVE_OBJECT_H_
#define	_MOVE_OBJECT_H_

#include"object.h"
#include"collision.h"

class CObjectX;

class CMove_Object : public CObject
{
public:
	static const int DRAW_DROP_DISTANCE = 7000;		// 武器、パーツを表示する距離

	// 移動オブジェクトのタグ
	enum TAG
	{
		TAG_NONE = 0,
		TAG_CHARACTER,		// キャラクタータグ
		TAG_BULLET,			// 弾タグ
		TAG_ATTACK,			// 攻撃タグ
		TAG_EXPLOSION,		// 爆発タグ
		TAG_MAP_OBJECT,		// マップオブジェクト
		TAG_MAX,
	};

	enum COLLISION_TYPE
	{
		COLLISION_NONE = 0,
		COLLISION_TYPE_SHERER,
		COLLISION_TYPE_BLOCK,
		COLLISION_TYPE_BOSS,
		COLLISION_MAX,
	};

	CMove_Object(const PRIORITY priority);
	~CMove_Object();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	virtual void Hit(CMove_Object* pHit) = 0;
	void DelayCollision();							// 当たり判定を遅延して設定する処理
	void CollisionDestroy();											// 当たり判定ポインタを消す処理

	void AddPos(const D3DXVECTOR3 pos) { m_pos += pos; }				// 位置の加算

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }					// 位置の設定
	void SetPosOld(const D3DXVECTOR3 posold) { m_posOld = posold; }					// 位置の設定
	void SetCenterPos(const D3DXVECTOR3 pos) { m_CenterPos = pos; }		// 初期位置の設定
	void SetRadius(const float radius) { m_fRadius = radius; }			// 半径の設定
	void SetTag(TAG tag) { m_tag = tag; }								// タグの設定
	void SetPlayerSide(const bool side) { m_bPlayerSide = side; }		// プレイヤー側かどうかの設定
	void SetCollisionNoneHit(const bool noneHit) { m_bCollision_NoneHit = noneHit; }	// 当たり判定のヒット処理を読み込まない判定の設定
	void SetCollision();						// 当たり判定の生成
	void SetDelayCollision() { m_bCollision_Delay = true; }				// 当たり判定を遅延して生成
	void SetCollision_Type(COLLISION_TYPE type) { m_Collision_Type = type; }
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }
	void SetGround(const bool ground) { m_bGround = ground; }
	void SetLandObj(bool land) { m_bLand_Obj = land; }
	void SetObjY(const float Y) { m_fObjY = Y; }
	void SetObjXZ(const bool xz) { m_bInXZ = xz; }
	//void SetOnObj(CMove_Object *onobj, int index) { m_OnObject[index] = onobj; }
	void SetIndex(D3DXVECTOR3 index, int cnt) { m_Index[cnt] = index; }
	void AddOnObjCnt(int cnt) { m_nOnObjCnt += cnt; }
	void SetOnObjCnt(int cnt) { m_nOnObjCnt = cnt; }
	void SetObjectX(CObjectX *objectX) { m_pObjectX = objectX; }

	const D3DXVECTOR3 GetPos() { return m_pos; }						// 位置の取得
	const D3DXVECTOR3 GetPosOld() { return m_posOld; }
	const D3DXVECTOR3 GetCenterPos() { return m_pos + m_CenterPos; }	// 中心位置の取得
	virtual const int GetPower() { return NULL; }						// 威力の取得
	const float GetRadius() { return m_fRadius; }						// 半径の取得
	TAG GetTag() { return m_tag; }										// タグの取得
	const bool GetPlayerSide() { return m_bPlayerSide; }				// プレイヤー側かどうかの取得
	const bool GetCollisionNoneHit() { return m_bCollision_NoneHit; }
	CCollision* GetCollision() { return m_pCollision; }					// 当たり判定の情報
	const COLLISION_TYPE GetCollision_Type() { return m_Collision_Type; }
	const D3DXVECTOR3 GetSize() { return m_Size; }
	const bool GetGround() { return m_bGround; }
	bool GetLandObj() { return m_bLand_Obj; }
	const float GetObjY() { return m_fObjY; }
	const bool GetObjXZ() { return m_bInXZ; }
	//CMove_Object *GetOnObj(int index) { return m_OnObject[index]; }
	D3DXVECTOR3 GetIndex(int cnt) { return m_Index[cnt]; }
	int GetOnObjCnt() { return m_nOnObjCnt; }
	CObjectX *GetObjectX() { return m_pObjectX; }

private:
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_posOld;		// 前回の位置
	D3DXVECTOR3 m_CenterPos;	// 中心位置
	float m_fRadius;			// 半径
	TAG m_tag;					// タグ
	CCollision* m_pCollision;	// 当たり判定の情報
	bool m_bPlayerSide;			// プレイヤー側かどうか
	bool m_bCollision_Delay;	// 当たり判定の遅延設置判定
	bool m_bCollision_NoneHit;	// 当たり判定のヒット処理を読み込まない判定
	COLLISION_TYPE m_Collision_Type;		// 当たり判定のタイプ
	D3DXVECTOR3 m_Size;			// サイズ
	bool m_bGround;				// 接地判定
	bool m_bLand_Obj;			// オブジェクトに着地
	bool m_bInXZ;				// XZが重なってるか
	float m_fObjY;				// オブジェクトのY座標
	D3DXVECTOR3 m_Index[4];		// オブジェクトの4頂点
	//CMove_Object *m_OnObject[128];
	int m_nOnObjCnt;
	CObjectX *m_pObjectX;
};

#endif // !_MOVE_OBJECT_H_
