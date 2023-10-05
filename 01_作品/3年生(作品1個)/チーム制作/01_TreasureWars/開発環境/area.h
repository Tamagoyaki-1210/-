//=============================================================================
//
// area.h
// Author : koduna hirohito
//
//=============================================================================

#ifndef _AREA_H_
#define _AREA_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "3DPolygon.h"

//---------------------------
// クラス宣言
//---------------------------
class CObject3D;
class CArea : public C3DPolygon
{
	static const int PLAYER_MAX = 4;
	static const int AREA_EXTENSION = 3;
	static const D3DXVECTOR3 AREA_POS;
	static const D3DXVECTOR2 AREA_UV_SIZE_INIT;
	static const D3DXVECTOR2 EACH_PLAYER_DIRECTION[PLAYER_MAX];
	static const float AREA_UV_MOVE;
	static const float AREA_UV_MOVE_OBLIQUE;
	static const float AREA_SIZE;
	static const float AREA_FLOATING_DAMPING_RATE;
	static const int AREA_FLOATING_TIME = 60 * 5;
	static const float ANGLE_EACH_EACH_CHARACTER[PLAYER_MAX];

public:

	CArea();
	~CArea() override;

	HRESULT Init() override;
	//void Uninit() override;
	void Update() override;
	void Draw()  override;

	void SetTopCharaNum(int nNum) { m_nTopCharaNum = nNum; }
	void PlayerRankConfirmation();
	void AreaFluctuation(int nPLNum); //エリア変動
	void SetDestUVPos(D3DXVECTOR2 DestPos) { m_DestUVPos = DestPos; }
	bool InCheckTerritory(D3DXVECTOR3 Pos, int nCharacterNum);

	static CArea *CArea::Create();
private:
	D3DXVECTOR3 m_CrossPos;
	D3DXVECTOR2 m_DestUVPos;
	int m_nAreaExtensionX;
	int m_nAreaExtensionZ;
	int m_nOldAreaExtensionX;
	int m_nOldAreaExtensionZ;
	int m_nAreaFloatingTime;
	int m_nTopCharaNum;
	CObject3D *m_pCObject3D;
};

#endif// _AREA_H_