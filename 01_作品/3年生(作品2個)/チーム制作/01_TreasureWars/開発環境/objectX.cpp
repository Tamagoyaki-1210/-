//==============================================================================================
//
// オブジェクトX　　　objectX.cpp
// tutida ryousei
//
//==============================================================================================
#include"ObjectX.h"
#include"application.h"
#include"game.h"
#include"input.h"
#include"fade.h"
#include"light.h"
#include"camera.h"
#include"charmanager.h"
#include"character.h"
#include"score.h"
#include"shop.h"
#include"charmanager.h"

std::vector <CObjectX::ModelPattern> CObjectX::m_ModelPattern = {};

//==============================================================================================
// コンストラクタ
//==============================================================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_Size = { 1.0f, 1.0f, 1.0f };
	m_vtxMinModel = { 0.0f,0.0f,0.0f };
	m_vtxMaxModel = { 0.0f,0.0f,0.0f };
	m_vecDir = { 0.0f,0.0f,0.0f };
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CObjectX::~CObjectX()
{
}

//==============================================================================================
// 初期化処理
//==============================================================================================
HRESULT CObjectX::Init()
{
	// マテリアル色を初期化する
	for (int i = 0; i < (int)m_ModelPattern[m_nIndex].nNumMat; i++)
	{
		m_col.emplace_back();
		m_col.back() = m_ModelPattern[m_nIndex].MatColor[i];
	}

	D3DXMatrixIdentity(&m_mtxRot);
	
	int nNumVtx;		//頂点数
	DWORD sizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	//頂点数の取得
	nNumVtx = m_ModelPattern[m_nIndex].pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_ModelPattern[m_nIndex].pMesh->GetFVF());

	//頂点バッファのロック
	m_ModelPattern[m_nIndex].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//最小値の取得
		if (vtx.x < m_vtxMinModel.x)
		{
			m_vtxMinModel.x = vtx.x;
		}
		else if (vtx.y < m_vtxMinModel.y)
		{
			m_vtxMinModel.y = vtx.y;
		}
		else if (vtx.z < m_vtxMinModel.z)
		{
			m_vtxMinModel.z = vtx.z;
		}

		//最大値の取得
		if (vtx.x > m_vtxMaxModel.x)
		{
			m_vtxMaxModel.x = vtx.x;
		}
		else if (vtx.y > m_vtxMaxModel.y)
		{
			m_vtxMaxModel.y = vtx.y;
		}
		else if (vtx.z > m_vtxMaxModel.z)
		{
			m_vtxMaxModel.z = vtx.z;
		}

		//頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += sizeFVF;
	}

	//頂点バッファのアンロック
	m_ModelPattern[m_nIndex].pMesh->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================================
// 終了処理
//==============================================================================================
void CObjectX::Uninit()
{
	Release();
}

//==============================================================================================
// 画面遷移時にオブジェクトを破棄
//==============================================================================================
void CObjectX::UninitObject()
{
	while(!m_ModelPattern.empty())
	{
		//メッシュの破棄
		if (m_ModelPattern.back().pMesh != nullptr)
		{
			m_ModelPattern.back().pMesh->Release();
			m_ModelPattern.back().pMesh = nullptr;
		}

		//マテリアルの破棄
		if (m_ModelPattern.back().pBuffMat != nullptr)
		{
			m_ModelPattern.back().pBuffMat->Release();
			m_ModelPattern.back().pBuffMat = nullptr;
		}

		for (int nCntMat = 0; nCntMat < (int)m_ModelPattern.back().nNumMat; nCntMat++)
		{
			//テクスチャの破棄
			if (m_ModelPattern.back().pTexture[nCntMat] != nullptr)
			{
				m_ModelPattern.back().pTexture[nCntMat]->Release();
				m_ModelPattern.back().pTexture[nCntMat] = nullptr;
			}
		}
		// 一番後ろの要素をリストから排除する
		m_ModelPattern.pop_back();
	}
}

//==============================================================================================
//更新処理
//==============================================================================================
void CObjectX::Update()
{

}

//==============================================================================================
// 描画処理
//==============================================================================================
void CObjectX::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScaling;		//計算用のマトリックス
	D3DMATERIAL9 matDef;		//現在のマテリアルの保存用
	D3DXMATERIAL *pMat = nullptr;			//マテリアルデータへのポインタ

	//テクスチャの設定を戻す
	pDevice->SetTexture(0, NULL);

	//ワールドマトリックス
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxScaling, m_Size.x, m_Size.y, m_Size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScaling);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&m_mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 平面投影
	//DrawShadow();

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_ModelPattern[m_nIndex].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_ModelPattern[m_nIndex].nNumMat; nCntMat++)
	{
		// マテリアルの色変更
		pMat[nCntMat].MatD3D.Diffuse = pMat[nCntMat].MatD3D.Specular = pMat[nCntMat].MatD3D.Emissive = m_col[nCntMat];

		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_ModelPattern[m_nIndex].pTexture[nCntMat]);

		//モデルパーツの描画
		m_ModelPattern[m_nIndex].pMesh->DrawSubset(nCntMat);
	}

	//保持していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

//==============================================================================================
// 平面投影
//==============================================================================================
void CObjectX::DrawShadow()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	D3DMATERIAL9 matDef;		// 現在のマテリアル保存用
	D3DXMATERIAL *pMat = nullptr;			// マテリアルデータへのポインタ
	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DXPLANE planeField;		// 平面
	D3DXVECTOR3 pos, normal;

	// アンビエントを無効にする
	CApplication::GetRenderer()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0);

	// シャドウマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// ライトの取得
	CLight *pLight = CApplication::GetLight();

	// ライトの方向
	D3DXVECTOR3 vecDir = pLight->GetLight().Direction;

	D3DXVECTOR4 vecLight = D3DXVECTOR4(-vecDir.x, -vecDir.y, -vecDir.z, 0.0f);

	// 位置の設定
	pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 法線の設定
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 平面の作成
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);

	// ライトと平面から影行列を作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// シャドウマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 現在のマテリアル保持
	pDevice->GetMaterial(&matDef);

	if (m_ModelPattern[m_nIndex].pBuffMat != nullptr)
	{
		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_ModelPattern[m_nIndex].pBuffMat->GetBufferPointer();
	}

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int nCnt = 0; nCnt < (int)m_ModelPattern[m_nIndex].nNumMat; nCnt++)
	{
		D3DMATERIAL9 matD3D;

		if (pMat != nullptr)
		{
			// マテリアル情報の設定
			matD3D = pMat[nCnt].MatD3D;
		}

		// 色の設定
		matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアルの設定
		pDevice->SetMaterial(&matD3D);

		if (m_ModelPattern[m_nIndex].pMesh != nullptr)
		{
			// モデルパーツの描画
			m_ModelPattern[m_nIndex].pMesh->DrawSubset(nCnt);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	// アンビエントを戻す
	CApplication::GetRenderer()->GetDevice()->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
}

//==============================================================================================
// ファイルの読み込み
//==============================================================================================
int CObjectX::ReadObject(const char* name)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	int nIndex = 0;

	// 作成された全てのモデルを読み込む
	for (auto model : m_ModelPattern)
	{
		if (model.fileName == name)
		{
			// 使用中のモデルと一致する場合終了
			return nIndex;
		}
		nIndex++;
	}

	// モデル情報の後ろの要素を追加
	m_ModelPattern.emplace_back();

	m_ModelPattern.back().fileName = name;

	//Xファイルの読み込み
	D3DXLoadMeshFromX(m_ModelPattern.back().fileName.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_ModelPattern.back().pBuffMat,
		NULL,
		&m_ModelPattern.back().nNumMat,
		&m_ModelPattern.back().pMesh);

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_ModelPattern.back().pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)m_ModelPattern.back().nNumMat; i++)
	{
		// テクスチャの後ろの要素を追加
		m_ModelPattern.back().pTexture.emplace_back();

		if (pMat[i].pTextureFilename != nullptr)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&m_ModelPattern.back().pTexture.back());
		}
		else
		{
			m_ModelPattern.back().pTexture[i] = nullptr;
		}
		// マテリアルの後ろの要素を追加
		m_ModelPattern.back().MatColor.emplace_back();

		// マテリアルの色情報を保存
		m_ModelPattern.back().MatColor.back() = pMat[i].MatD3D.Diffuse;
	}

	return nIndex;
}

//==============================================================================================
// モデル番号の設定
//==============================================================================================
void CObjectX::SetIndex(const int index)
{
	m_nIndex = index;

	// モデルの色が設定中の場合
	if (!m_col.empty())
	{
		// 現在のマテリアルの色を消す
		m_col.clear();

		// マテリアル色を初期化する
		for (int i = 0; i < (int)m_ModelPattern[m_nIndex].nNumMat; i++)
		{
			m_col.emplace_back();
			m_col.back() = m_ModelPattern[m_nIndex].MatColor[i];
		}
	}
}

//==============================================================================================
// 生成処理
//==============================================================================================
CObjectX * CObjectX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, const char* Xfilename)
{
	CObjectX *pObjectX = new CObjectX(2);

	if (pObjectX != nullptr)
	{
		pObjectX->SetPos(pos);
		pObjectX->SetRot(rot);
		pObjectX->SetSize(size);
		pObjectX->SetIndex(ReadObject(Xfilename));
		pObjectX->Init();
	}

	return pObjectX;
}