//==============================================================================
//
// Map.h
// Author : Saito Shian
//
//==============================================================================

//==============================================================================
// 2重インクルードガード
//==============================================================================
#ifndef _MAP_H_
#define	_MAP_H_

//==============================================================================
// インクルード
//==============================================================================
#include "main.h"
#include"trolley_manager.h"
#include <list>

//==============================================================================
// 前方宣言
//==============================================================================
class CObjectX;
class C3DPolygon;

//==============================================================================
// マップクラス
//==============================================================================
class CMap
{
	static const int LINE_MAX_READING_LENGTH = 256;
	static const char* MAP_OBSTACLE_OBJECT_FILENAME[];		// ファイルパス
	static const char* MAP_BACKGROUND_OBJECT_FILENAME[];	// ファイルパス
	static const float Map_Collision_Decreace;				// マップオブジェクトの当たり判定を小さくする値
	
public:
	CMap();						// コンストラクタ
	~CMap();					// デストラクタ

	HRESULT Init(int nMopNum);				// 初期化処理
	void Uninit();				// 終了処理
	void Update();				// 更新処理
	void Draw();				// 描画処理

	D3DXVECTOR3 CollisionMapObject(const D3DXVECTOR3 pos, const float radius);

	void LoadMap(bool bCollision, const char* s_FileName);			// マップ読み込み

	void Trolley_Route();		// トロッコのルート設定

	static CTrolley_Manager *GetTrolleyManager() { return m_pTrolley_Manager; }	// トロッコマネージャー

private:
	C3DPolygon *m_pFloor;			//　床
	std::list<CObjectX*> m_MapObjectList;		// オブジェクトXのインスタンス
	int m_nMapSizeMax;					//　マップの最大幅

	int m_nMapMax;
	//int *m_pMapTypeData;
	int m_nMapTypeMax;
	CObjectX *m_pMapMadelData;
	int m_nMapNum;

	static CTrolley_Manager *m_pTrolley_Manager;
};

#endif