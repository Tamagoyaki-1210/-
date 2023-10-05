//=============================================================================
//
// オブジェクト
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"

//---------------------------
//マクロ関数(オブジェクト)
//---------------------------
#define MAX_OBJECT_PRIORITY			(7)		//描画順リスト

#define OBJECT_PRIORITY_BACK		(1)		//奥
#define OBJECT_PRIORITY_CENTER		(2)		//中央

#define EFFECT_PRIORITY_BACK		(3)		//エフェクト奥
#define EFFECT_PRIORITY_CENTER		(4)		//エフェクト中央

#define UI_PRIORITY_UI				(5)		//UI
#define UI_PRIORITY_FADE			(6)		//フェード

//オブジェクトクラス
class CObject
{
public:
	enum Object_Type
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,	//プレイヤー
		TYPE_ENEMY,		//敵キャラ
		TYPE_OBSTACLE,	//障害物
		TYPE_ITEM,		//アイテム
		TYPE_BG,		//背景
		TYPE_EFFECT,	//エフェクト
		TYPE_UI,		//UI
		TYPE_MAX,		//最大値
	};

	CObject(int priority);
	virtual ~CObject();

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetType(Object_Type type);
	void Release();

	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void SetDeath() { m_bDeath = true; }
	bool GetDeath() { return m_bDeath; }
	CObject* GetNext() { return m_pNext; }
	static CObject* GetTop(int priority) { return m_pTop[priority]; }

	const CObject::Object_Type& GetType() const;
	virtual const D3DXVECTOR3& GetPos() const = 0;
	virtual const float GetWidth() const = 0;
	virtual const float GetHeight() const = 0;
	virtual const float GetLength() const = 0;
private:
	bool m_bDeath;					//死亡フラグ
	Object_Type m_type;				//オブジェクトの種別
	static CObject *m_pTop[MAX_OBJECT_PRIORITY];			//先頭のオブジェクトへのポインタ
	static CObject *m_pCurrent[MAX_OBJECT_PRIORITY];		//最後の(現在)オブジェクトへのポインタ
	CObject *m_pPrev;				//前回のオブジェクトへのポインタ
	CObject *m_pNext;				//次回のオブジェクトへのポインタ
};

#endif // !_OBJECT_H_