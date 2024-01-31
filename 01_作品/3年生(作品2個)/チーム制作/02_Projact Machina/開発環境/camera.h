//==============================================
//
// カメラの処理[camera.h]
// Author：tutida ryousei
// Author：tanimoto kosuke
//
//==============================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include"renderer.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void Matrix(D3DXVECTOR3 rot, D3DXVECTOR3 pos);
	void DebugInput();			// 入力処理
	void Move();
	void Perspective();			// 視点処理
	void Opening_Move();		// オープニング
	void Tremor();				// カメラの揺れ

	bool GetPerspective() { return m_bPerspective; }
	void SetPerspective(bool flag) { m_bPerspective = flag; }
	D3DXVECTOR3 GetPlayerPos() { return m_PPos; }
	void SetPlayerPos(D3DXVECTOR3 pos) { m_PPos = pos; }
	void SetOpening(bool opening) { m_bOpening = opening; }

	void SetPosV(const D3DXVECTOR3 posV) { m_posV = posV; }// 視点
	void SetPosR(const D3DXVECTOR3 posR) { m_posR = posR; }// 注視点
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void AddPosV(const D3DXVECTOR3 posV) { m_posV += posV; }
	void AddPosR(const D3DXVECTOR3 posR) { m_posR += posR; }

	const D3DXVECTOR3 GetPosV() { return m_posV; }
	const D3DXVECTOR3 GetPosR() { return m_posR; }
	const D3DXVECTOR3 GetRot() { return m_rot; }
	const CCamera* GetCamera() { return m_pCamera; }
	const D3DXVECTOR3 GetWorldPosV() { return m_worldCameraPosV; }
	const D3DXVECTOR3 GetWorldPosR() { return m_worldCameraPosR; }
	const bool GetOpening() { return m_bOpening; }

private:
	D3DVIEWPORT9	m_viewport;			// ビューポート
	D3DXVECTOR3     m_pos;				// 位置
	D3DXVECTOR3		m_posV;				// 視点
	D3DXVECTOR3		m_InitposV;			// 視点の初期値
	D3DXVECTOR3     m_posDest;			// 目的の視点
	D3DXVECTOR3		m_posVDest;			// 目的の視点
	D3DXVECTOR3		m_posR;				// 注視点
	D3DXVECTOR3		m_InitposR;			// 注視点の初期値
	D3DXVECTOR3		m_posRDest;			// 目的の注視点
	D3DXVECTOR3		m_CPos;				// キャラクターの位置
	D3DXVECTOR3		m_PPos;				// プレイヤーの位置
	D3DXVECTOR3		m_RPos;				// ロボットの位置
	D3DXVECTOR3		m_vecU;				// 上方向ベクトル
	D3DXMATRIX		m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX		m_mtxView;			// ビューマトリックス
	D3DXVECTOR3		m_rot;				// 向き
	D3DXVECTOR3		m_rotDest;			// 目的の向き
	D3DXVECTOR3		m_Saverot;			// 角度の保存

	int				m_nShiftGoalCount;	// ゴールを映す時間
	int				m_nGoalCount;		// ゴールを映すまでの時間
	float			m_fDistance;		// 視点から注視点までの距離
	float			m_fRotSpeed;		// 回転速度
	bool			m_bCollision;		// オブジェクトに当たっているか
	bool			m_bPerspective;		// 視点のフラグ
	bool			m_bValue;			// 値を通すフラグ

	bool			m_bOpening;			// オープニング
	bool			m_bOpening_step[3];	// カメラ移動の順番

	bool			m_bTremor;			// 揺れているか
	int				m_nChange_Count;	// 移動カウント
	float			m_fMax_Scale;		// 揺れの大きさ
	D3DXVECTOR3		m_Base_PosV;		// 揺れるときのベースの視点
	D3DXVECTOR3		m_Base_PosR;		// 揺れるときのベースの注視点

	D3DXMATRIX		m_mtxWorld;			// ワールドマトリックス
	D3DXVECTOR3		m_worldCameraPosV;	// 視点のワールド座標
	D3DXVECTOR3		m_worldCameraPosR;	// 注視点のワールド座標

	CCamera*		m_pCamera;		//カメラの情報
};

#endif // !_CAMERA_H_