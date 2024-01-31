//==============================================================================================
//
// オブジェクト　　　object.h
// Author : tutida ryousei
//
//==============================================================================================
#ifndef _OBJECT_H_
#define	_OBJECT_H_

#include <d3dx9.h>

class CObject
{
public:
	// 頂点フォーマット
	static const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);	//座標、法線、カラー 、テクスチャ

	// オブジェクトの種類
	enum EObjType
	{
		OBJTYPE_NONE = 0,
		//OBJTYPE_MODE,		// ゲームモード
		OBJTYPE_MAX
	};

	CObject(int nPriority = 3);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void Release();
	void Death(int nPriority);
	float Distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);		// 二つの位置の距離

	bool CollisionCircle(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius);
	bool AttackCollision(const D3DXVECTOR3 pos, const float rot, const float length, const float angle, const D3DXVECTOR3 otherPos, const float otherRadius);
	bool CheckInAngle(const D3DXVECTOR3 pos, const float rot, const float angle, const D3DXVECTOR3 otherPos);

	CObject *GetNext() { return m_pNext; }
	static CObject *GetTop(int Pri) { return m_pTop[Pri]; }
	virtual EObjType GetObjType() { return OBJTYPE_NONE; }		// オブジェクトタイプの取得
	bool GetDeathFlag() { return m_bDeath; }
	bool GetDrawFlag() { return m_bDraw; }
	void SetDrawFlag(bool flag) { m_bDraw = flag; }

private:
	static const int m_nMax_Pri = 6;
	static const float AttackLength_Decreace;	// 攻撃射程の減少

	int m_nPriority;
	CObject *m_pPrev;						// 一つ前のオブジェクトへのポインタ
	CObject *m_pNext;						// 次のオブジェクトへのポインタ
	static CObject *m_pTop[m_nMax_Pri];		// 先頭のオブジェクトのへのポインタ
	static CObject *m_pCurrent[m_nMax_Pri];	// 現在(一番後ろ)のオブジェクトのへのポインタ
	bool m_bDeath;							// 死亡フラグ
	bool m_bDraw;
};

#endif // !_OBJECT_H_
