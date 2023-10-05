//=============================================================================
//
// fallingStone.h
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "fallingStone.h"
#include "CylinderHitbox.h"
#include "application.h"
#include "rendering.h"
#include "meshfield.h"
#include "camera.h"


CFallingStone::CFallingStone()
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_shadowPosY = 0.0f;							//影の高さ
	m_fBound = 0.0f;								//影のエリアの幅
	m_fDir = 0.0f;									//落下の方向
	m_frameRot = Vec3Null;							//回転速度
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_type = (CModel::ModelType)0;					//モデルの種類
	m_vModelTexture.clear();						//テクスチャ
	m_pHitbox = nullptr;							//ヒットボックス
}

CFallingStone::~CFallingStone()
{

}

//初期化処理
HRESULT CFallingStone::Init(void)
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_shadowPosY = 0.0f;							//影の高さ
	m_fBound = 0.0f;								//影のエリアの幅
	m_fDir = 1.0f;									//落下の方向
	m_frameRot = Vec3Null;							//回転速度
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_type = (CModel::ModelType)0;					//モデルの種類
	m_vModelTexture.clear();						//テクスチャ
	m_pHitbox = nullptr;							//ヒットボックス

	return S_OK;
}

//終了処理
void CFallingStone::Uninit(void)
{
	//ヒットボックスの破棄
	if (m_pHitbox != nullptr)
	{
		m_pHitbox->Release();
		m_pHitbox = nullptr;
	}
}

//更新処理
void CFallingStone::Update(void)
{
	//位置の更新処理
	if (m_move != Vec3Null)
	{
		m_pos += m_move;
	}

	//重力を加算する
	if (m_move.y >= -7.0f)
	{
		m_move.y += -0.5f;
	}

	//回転の更新
	if (m_frameRot != Vec3Null)
	{
		m_rot += m_frameRot;
	}

	//メッシュフィールドとの当たり判定
	if (CMeshfield::FieldInteraction(this))
	{//当たったら、反射させる
		if (m_move.x < 1.0f)
		{
			m_move.x = 3.0f * m_fDir;
			m_move.y = 7.0f;
			StartRotation(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.01f * m_fDir));
		}
	}

	if (m_pos.y <= -500.0f)
	{//落ちたら、消す
		Release();
	}

	if (m_pHitbox != nullptr)
	{//ヒットボックスがnullではなかったら

		m_pHitbox->Update();		//ヒットボックスの更新処理

		if (m_pHitbox->GetCollisionState())
		{//プレイヤーと当たったら
			m_pHitbox->SetCollisionState(false);								//当っていない状態に戻す

			//反射させて、回転を設定する
			m_move.x = 3.0f * m_fDir;											
			m_move.y = 7.0f;													
			StartRotation(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI * 0.01f * m_fDir));	
		}
	}

	if (GetPos().z < CApplication::GetCamera()->GetPos().z - 100.0f)
	{//見えなくなったら、消す
		Release();
	}
}

//描画処理
void CFallingStone::Draw(void)
{
	if (GetPos().z < CApplication::GetCamera()->GetPos().z + 650.0f)
	{//遠すぎたら、描画しない
		LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();				//デバイスの取得
		D3DXMATRIX mtxRot, mtxTrans, mtxShadow;							//計算用マトリックス
		D3DMATERIAL9 matDef;											//現在のマテリアル保存用
		D3DXMATERIAL *pMat;												//マテリアルデータへのポインタ
		D3DXVECTOR4 vecLight;											//ライトの向き
		D3DXVECTOR3 pos, Normal;										//投影用の位置と法線
		D3DXPLANE planeField;											//面

		//ライトの向きを設定する
		D3DXVECTOR3 dir = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		D3DXVec3Normalize(&dir, &dir);

		vecLight = D3DXVECTOR4(-dir.x, -dir.y, -dir.z, 0.0f);

		pos = D3DXVECTOR3(0.0f, m_shadowPosY, 0.0f);				//面の高さ

		if (m_pos.y < m_shadowPosY - 10.0f || (m_pos.x > m_spawnPos.x + m_fBound && m_fDir > 0.0f) || (m_pos.x < m_spawnPos.x - m_fBound && m_fDir < 0.0f))
		{//落ちたら影の高さを見えないように設定する
			pos.y = -5000.0f;
		}

		Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);					//面の法線

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);
		D3DXMatrixIdentity(&mtxShadow);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//面の生成
		D3DXPlaneFromPointNormal(&planeField, &pos, &Normal);
		D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

		//影の描画用のマトリックス
		D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

		//現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタの取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			D3DXCOLOR col = pMat[nCntMat].MatD3D.Diffuse;			//マテリアルの色を保存する

			//マテリアルの色を真っ黒にする
			pMat[nCntMat].MatD3D.Diffuse.r = 0.0f;
			pMat[nCntMat].MatD3D.Diffuse.g = 0.0f;
			pMat[nCntMat].MatD3D.Diffuse.b = 0.0f;
			pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, m_vModelTexture.data()[nCntMat]);

			//モデルパーツの描画
			m_pMesh->DrawSubset(nCntMat);

			//マテリアルの色を元に戻す
			pMat[nCntMat].MatD3D.Diffuse.r = col.r;
			pMat[nCntMat].MatD3D.Diffuse.g = col.g;
			pMat[nCntMat].MatD3D.Diffuse.b = col.b;
			pMat[nCntMat].MatD3D.Diffuse.a = col.a;
		}


		//ステンシルバッファを有効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		//ステンシルバッファと比較する参照値設定
		pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

		//ステンシルバッファの値に対してのマスク設定
		pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

		//ステンシルテストの比較方法の設定
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

		//ステンシルテストの結果に対しての反映設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

		pMat = nullptr;

		//保持しいたマテリアルを戻す
		pDevice->SetMaterial(&matDef);


		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタの取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, m_vModelTexture.data()[nCntMat]);

			//モデルパーツの描画
			m_pMesh->DrawSubset(nCntMat);
		}

		//保持しいたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		//ステンシルバッファを無効にする
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		pMat = nullptr;

		//保持しいたマテリアルを戻す
		pDevice->SetMaterial(&matDef);


		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタの取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, m_vModelTexture.data()[nCntMat]);

			//モデルパーツの描画
			m_pMesh->DrawSubset(nCntMat);
		}

		//保持しいたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//位置の設定処理
void CFallingStone::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//位置の取得処理
const D3DXVECTOR3 CFallingStone::GetPos(void)
{
	return m_pos;
}

//サイズの取得処理
const D3DXVECTOR2 CFallingStone::GetSize(void)
{
	return Vec2Null;
}


//回転開始の設定処理
void CFallingStone::StartRotation(const D3DXVECTOR3 frameRot)
{
	m_frameRot = frameRot;
}

//回転の停止処理
void CFallingStone::StopRotating(void)
{
	m_frameRot = Vec3Null;
}

//向きの設定処理
void CFallingStone::SetDirection(const float fLateralDirection)
{
	if (fLateralDirection >= 0)
	{
		m_fDir = 1.0f;
	}
	else
	{
		m_fDir = -1.0f;
	}
}






//生成処理
CFallingStone* CFallingStone::Create(CModel::ModelType type, D3DXVECTOR3 pos, const D3DXVECTOR3 spawnPos, const float ShadowHeight, const float fBound, const float fDir)
{
	CFallingStone* pModel = new CFallingStone;			//インスタンスを生成する

	if (FAILED(pModel->Init()))
	{//初期化処理
		return nullptr;
	}

	pModel->m_pos = pos;						//位置の設定
	pModel->m_rot = Vec3Null;					//回転の設定
	pModel->m_LastPos = pos;					//前回の位置の設定
	pModel->m_type = type;						//種類の設定
	pModel->m_shadowPosY = ShadowHeight;		//影の高さの設定
	pModel->m_fBound = fBound;					//メッシュフィールドの幅の設定
	pModel->m_fDir = fDir;						//向きの設定
	pModel->m_spawnPos = spawnPos;				//スポーンの位置の設定

	//モデル情報の取得処理
	CModel::GetModel(type, &pModel->m_pMesh, &pModel->m_pBuffMat, &pModel->m_nNumMat);
	CModel::GetTextures(pModel->m_vModelTexture, type);

	//ヒットボックスの生成処理
	pModel->m_pHitbox = CCylinderHitbox::Create(pos, Vec3Null, D3DXVECTOR3(20.0f, 40.0f, 20.0f), CHitbox::TYPE_OBSTACLE, -30, pModel, CHitbox::EFFECT_LAUNCH);

	return pModel;				//インスタンスを返す
}