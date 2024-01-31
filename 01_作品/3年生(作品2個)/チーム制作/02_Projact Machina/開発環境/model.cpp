//==============================================================================================
//
// モデル　　　madel.cpp
// tutida ryousei
//
//==============================================================================================
#include "model.h"
#include "application.h"
#include "renderer.h"

//==============================================================================================
// コンストラクタ
//==============================================================================================
CModel::CModel()
{
}

//==============================================================================================
// デストラクタ
//==============================================================================================
CModel::~CModel()
{
}

//==============================================================================================
// 全てのモデルの終了処理
//==============================================================================================
void CModel::ReleaseAll()
{
	// モデルを全て終了させる
	while (!m_ModelPattern.empty())
	{
		// メッシュの破棄
		if (m_ModelPattern.back().pMesh != nullptr)
		{
			m_ModelPattern.back().pMesh->Release();
		}

		// マテリアルの破棄
		if (m_ModelPattern.back().pBuffMat != nullptr)
		{
			m_ModelPattern.back().pBuffMat->Release();
		}

		for (int nCntMat = 0; nCntMat < (int)m_ModelPattern.back().nNumMat; nCntMat++)
		{
			// テクスチャの破棄
			if (m_ModelPattern.back().pTexture[nCntMat] != nullptr)
			{
				m_ModelPattern.back().pTexture[nCntMat]->Release();
			}
		}	
		// 一番後ろの要素をリストから排除する
		m_ModelPattern.pop_back();
	}

	m_vName.clear();
}

//==============================================================================================
// ファイルの読み込み
//==============================================================================================
const int CModel::ReadObject(const char* name)
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();

	int nIndex = 0;

	// 作成された全てのモデルを読み込む
	for (auto Name : m_vName)
	{
		// 同じファイルを読み込まない処理
		if (!strcmp(Name.data(), name))
		{
			// 使用中のモデルと一致する場合終了
			return nIndex;
		}
		nIndex++;
	}

	// モデル情報の後ろの要素を追加
	m_ModelPattern.emplace_back();

	// 文字列に変換
	char cName[0xff] = {};
	strncpy(cName, name, strlen(name));

	// 文字列をstring型に変換
	std::string dna_seq(cName, sizeof(cName) / sizeof(cName[0]));
	m_vName.push_back(dna_seq);

	//Xファイルの読み込み
	D3DXLoadMeshFromX(m_vName.back().data(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_ModelPattern.back().pBuffMat,
		NULL,
		&m_ModelPattern.back().nNumMat,
		&m_ModelPattern.back().pMesh);

	// 頂点マテリアルが存在する場合
	if (m_ModelPattern.back().pBuffMat != nullptr)
	{
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
				m_ModelPattern.back().pTexture.back() = nullptr;
			}

			// マテリアルの後ろの要素を追加
			m_ModelPattern.back().MatColor.emplace_back();

			// マテリアルの色情報を保存
			m_ModelPattern.back().MatColor.back() = pMat[i].MatD3D.Diffuse;
		}
	}

	return nIndex;
}
