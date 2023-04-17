//=============================================================================
//
// オブジェクト2D
// Author : tanimoto kosuke
//
//=============================================================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "object.h"

//オブジェクト2Dクラス
class CObject2D : public CObject
{
public:
	CObject2D(int priority = 0);
	~CObject2D() override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	
	void SetUV();
	void SetAnim(int nAnimX, int nAnimY, int nControl);
	void SetTex(int tex, int div);
	void SetColor(D3DXCOLOR col0, D3DXCOLOR col1, D3DXCOLOR col2, D3DXCOLOR col3);
	void SetColor(D3DXCOLOR col);

	void SetObject2D(Object_Type type, float fWidth, float fHeight);
	static CObject2D *Create(D3DXVECTOR3 pos);

	virtual void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	virtual void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	virtual void SetWidth(float fWidth) { m_fWidth = fWidth; }
	virtual void SetHeight(float fHeight) { m_fHeight = fHeight; }
	virtual void SetLength(float fLength) { m_fLength = fLength; }
	virtual void SetAngle(float fAngle) { m_fAngle = fAngle; }

	const D3DXVECTOR3& GetPos() const { return m_pos; }
	const D3DXVECTOR3& GetRot() const { return m_rot; }
	const float GetWidth() const { return m_fWidth; }
	const float GetHeight() const { return m_fHeight; }
	const float GetLength() const { return m_fLength; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//Bufferオブジェクト
	D3DXVECTOR3 m_pos;					//位置
	D3DXVECTOR3 m_rot;					//角度
	float m_fWidth;						//長さ横
	float m_fHeight;					//長さ縦
	float m_fLength;					//長さ(スクウェアルート)
	float m_fAngle;						//角度(アークタンジェント)
	int m_nAnimCounterX;				//アニメーションカウンターX座標
	int m_nAnimCounterY;				//アニメーションカウンターY座標
	int m_nAnimControl;					//アニメーション制御
};

#endif // !_OBJECT2D_H_