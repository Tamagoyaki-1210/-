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

	// 描画順の種類
	enum PRIORITY
	{
		PRIORITY_NONE = 0,		// 描画順_一番奥
		PRIORITY_BACK_GROUND,	// 描画順_背景
		PRIORITY_BACK,			// 描画順_後ろ側
		PRIORITY_MODEL,			// 描画順_モデル
		PRIORITY_CENTER,		// 描画順_中央
		PRIORITY_FRONT,			// 描画順_前側
		PRIORITY_SCREEN,		// 描画順_画面手前
		PRIORITY_MAX,			// 描画順の最大数
	};

	// 列挙型でのみ描画順の指定が可能
	explicit CObject(const PRIORITY priority = PRIORITY_CENTER);
	virtual ~CObject();

	virtual HRESULT Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void Release() { m_bDeath = true; }	// 死亡フラグを建てる
	void Death(const int nPriority);

	bool Sphere_Collision(const D3DXVECTOR3 pos, const float radius, const D3DXVECTOR3 otherPos, const float otherRadius);	// 円の当たり判定

	CObject* GetNext() { return m_pNext; }
	static CObject* GetTop(const int Pri) { return m_pTop[Pri]; }
	bool GetDeathFlag() { return m_bDeath; }
	bool GetDrawFlag() { return m_bDraw; }
	void SetDrawFlag(bool flag) { m_bDraw = flag; }

private:

	int m_nPriority;
	CObject* m_pPrev;							// 一つ前のオブジェクトへのポインタ
	CObject* m_pNext;							// 次のオブジェクトへのポインタ
	static CObject* m_pTop[PRIORITY_MAX];		// 先頭のオブジェクトのへのポインタ
	static CObject* m_pCurrent[PRIORITY_MAX];	// 現在(一番後ろ)のオブジェクトのへのポインタ
	bool m_bDeath;								// 死亡フラグ
	bool m_bDraw;
};

#endif // !_OBJECT_H_
