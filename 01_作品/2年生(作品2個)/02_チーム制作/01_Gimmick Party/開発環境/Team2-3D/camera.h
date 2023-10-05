//=============================================================================
//
// camera.h
// Author : Ricci Alex
//
//=============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//class CBoxHitbox;

class CCamera
{
public:
	CCamera();											//コンストラクタ
	~CCamera();											//デストラクタ
														
	HRESULT Init(void);									//初期化処理
	void Uninit(void);									//終了処理
	void Update(void);									//更新処理
	void Set(void);										//設定処理

	const D3DXVECTOR3 GetPos(void);						//位置（視点）の取得処理
	const D3DXVECTOR3 GetRot(void);						//向きの取得処理
	const D3DXVECTOR3 GetDirection(void);				//視点から注視点までのベクトルを返す処理
	const D3DXVECTOR3 CCamera::GetFocalPoint(void);		//注視点の取得処理
														
	void SetFocalPoint(const D3DXVECTOR3 pos);			//注視点の設定処理
	void SetPos(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR);	//視点、注視点の設定

	static CCamera* Create(D3DXVECTOR3 pos, D3DXVECTOR3 focalPoint);		//生成処理

private:
	static const float CAMERA_SPEED;

	D3DXVECTOR3 m_posV;						//視点
	D3DXVECTOR3 m_posR;						//注視点
	D3DXVECTOR3 m_DestPosV;					//目的の視点
	D3DXVECTOR3 m_DestPosR;					//目的の注視点
	D3DXVECTOR3 m_vecU;						//上方向ベクトル
	D3DXMATRIX  m_mtxProjection;			//プロジェクションマトリックス
	D3DXMATRIX  m_mtxView;					//ビューマトリックス
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_rotDest;					//目的の向き
	float		m_fLenght;					//XZ面の視点と注視点の間の距離
	int			m_nDelay;					//

	//CBoxHitbox *m_pHitBox;
};

#endif // !_CAMERA_H_