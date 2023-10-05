//=============================================================================
//
// icePillar.cpp
// Author : Ricci Alex
//
//=============================================================================

//=============================================================================
//インクルードファイル
//=============================================================================
#include "icePillar.h"
#include "CylinderHitbox.h"
#include "application.h"
#include "rendering.h"
#include "meshfield.h"
#include "camera.h"


//コンストラクタ
CIcePillar::CIcePillar()
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_shadowPosY = 0.0f;							//影の高さ
	D3DXMatrixIdentity(&m_mtxWorld);				//ワールドマトリックス
	m_nLife = 0;									//ライフ
	m_type = (CModel::ModelType)0;					//種類
	m_vModelTexture.clear();						//テクスチャ
	m_pHitbox = nullptr;							//ヒットボックス(上)
	m_pDamageHitbox = nullptr;						//ヒットボックス(下)
}

//デストラクタ
CIcePillar::~CIcePillar()
{

}

//初期化処理
HRESULT CIcePillar::Init(void)
{
	m_pMesh = nullptr;								//メッシュ情報へのポインタ
	m_pBuffMat = nullptr;							//マテリアル情報へのポインタ
	m_nNumMat = 0;									//マテリアル情報の数
	m_pos = Vec3Null;								//現在の位置
	m_LastPos = Vec3Null;							//前回の位置
	m_move = Vec3Null;								//モデルの移動量
	m_rot = Vec3Null;								//向き
	m_shadowPosY = 0.0f;							//影の高さ
	m_nLife = 0;									//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);				//ライフ
	m_type = (CModel::ModelType)0;					//種類
	m_vModelTexture.clear();						//テクスチャ
	m_pHitbox = nullptr;							//ヒットボックス(上)
	m_pDamageHitbox = nullptr;						//ヒットボックス(下)

	return S_OK;
}

//終了処理
void CIcePillar::Uninit(void)
{
	//ヒットボックスの破棄処理
	if (m_pHitbox != nullptr)
	{
		m_pHitbox->Release();
		m_pHitbox = nullptr;
	}
	if (m_pDamageHitbox != nullptr)
	{
		m_pDamageHitbox->Release();
		m_pDamageHitbox = nullptr;
	}
}

//更新処理
void CIcePillar::Update(void)
{
	//位置の更新
	if (m_move != Vec3Null)
	{
		m_pos += m_move;
	}
	//重力を加算する
	if (m_move.y >= -7.0f)
	{
		m_move.y += -0.5f;
	}

	//メッシュフィールドとの当たり判定
	if (CMeshfield::FieldInteraction(this))
	{//当たったら
		m_nLife++;				//ライフを更新する

		if (m_pDamageHitbox != nullptr)
		{//要らなくなったヒットボックスを破棄する
			m_pDamageHitbox->Release();
			m_pDamageHitbox = nullptr;
		}

		if (m_nLife >= 300)
		{//300フレーム後破棄する
			Release();
		}
	}

	if (m_pHitbox != nullptr)
	{//上のヒットボックスがnullではなかったら、位置を更新する
		m_pHitbox->SetPos(m_pos);
	}
	if (m_pDamageHitbox != nullptr)
	{//下のヒットボックスがnullではなかったら、位置を更新する
		m_pDamageHitbox->SetPos(m_pos);
	}

	if (GetPos().z < CApplication::GetCamera()->GetPos().z - 150.0f)
	{//見えなくなったら、消す
		Release();
	}
}

//描画処理
void CIcePillar::Draw(void)
{
	if (GetPos().z < CApplication::GetCamera()->GetPos().z + 650.0f)
	{//遠すぎたら、描画しない
		if (m_nLife < 100 || m_nLife % 30 < 15)
		{//メッシュフィールドと当たったら、点滅させる

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

			pos = D3DXVECTOR3(0.0f, m_shadowPosY, 0.0f);

			Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);					//面の高さ
			
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
}

//位置の設定処理
void CIcePillar::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//位置の取得処理
const D3DXVECTOR3 CIcePillar::GetPos(void)
{
	return m_pos;
}

//サイズの取得処理
const D3DXVECTOR2 CIcePillar::GetSize(void)
{
	return Vec2Null;
}



//=============================================================================
//
//			　　　　			静的関数
//
//=============================================================================



//生成処理
CIcePillar* CIcePillar::Create(CModel::ModelType type, D3DXVECTOR3 pos, const float ShadowHeight)
{
	CIcePillar* pPillar = new CIcePillar;				//インスタンスを生成する

	if (FAILED(pPillar->Init()))
	{//初期化処理
		return nullptr;
	}

	pPillar->m_pos = pos;								//位置の設定
	pPillar->m_rot = Vec3Null;							//向きの設定
	pPillar->m_LastPos = pos;							//前回の位置の設定
	pPillar->m_type = type;								//種類の設定
	pPillar->m_shadowPosY = ShadowHeight;				//影の高さの設定

	//モデル情報の取得
	CModel::GetModel(type, &pPillar->m_pMesh, &pPillar->m_pBuffMat, &pPillar->m_nNumMat);
	CModel::GetTextures(pPillar->m_vModelTexture, type);

	//ヒットボックスの生成
	pPillar->m_pHitbox = CCylinderHitbox::Create(pos, (D3DXVECTOR3(0.0f, -47.5f, 0.0f)), D3DXVECTOR3(25.0f, 80.0f, 25.0f), CHitbox::TYPE_NEUTRAL, pPillar);
	pPillar->m_pDamageHitbox = CCylinderHitbox::Create(pos, (D3DXVECTOR3(0.0f, -67.5f, 0.0f)), D3DXVECTOR3(25.0f, 20.0f, 25.0f), CHitbox::TYPE_OBSTACLE, -30, pPillar, CHitbox::EFFECT_LAUNCH);

	return pPillar;										//生成したインスタンスを返す
}