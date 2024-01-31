//==============================================================================================
//
// モデル　　　madel.h
// tutida ryousei
//
//==============================================================================================
#ifndef _MODEL_H_
#define	_MODEL_H_

#include "d3dx9.h"
#include <vector>

class CModel
{
public:
	struct ModelPattern
	{
		LPD3DXBUFFER pBuffMat;				// マテリアル情報へのポインタ
		LPD3DXMESH pMesh;					// メッシュ情報へのポインタ
		DWORD nNumMat;						// マテリアル情報の数
		std::vector <D3DXCOLOR> MatColor;	// 初期マテリアル色情報
		std::vector <LPDIRECT3DTEXTURE9> pTexture;	// モデルのテクスチャへのポインタの配列
	};

	CModel();
	~CModel();

	void ReleaseAll();
	const int ReadObject(const char* name);		// オブジェクトの読み込み

	ModelPattern GetModelPattern(const int index) { return m_ModelPattern[index]; }

private:

	std::vector <ModelPattern> m_ModelPattern;		// モデル構造体
	std::vector <std::string> m_vName;				// 各ファイル名
};

#endif // !_MODEL_H_
