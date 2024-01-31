//==============================================================================================
//
// ���f���@�@�@madel.h
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
		LPD3DXBUFFER pBuffMat;				// �}�e���A�����ւ̃|�C���^
		LPD3DXMESH pMesh;					// ���b�V�����ւ̃|�C���^
		DWORD nNumMat;						// �}�e���A�����̐�
		std::vector <D3DXCOLOR> MatColor;	// �����}�e���A���F���
		std::vector <LPDIRECT3DTEXTURE9> pTexture;	// ���f���̃e�N�X�`���ւ̃|�C���^�̔z��
	};

	CModel();
	~CModel();

	void ReleaseAll();
	const int ReadObject(const char* name);		// �I�u�W�F�N�g�̓ǂݍ���

	ModelPattern GetModelPattern(const int index) { return m_ModelPattern[index]; }

private:

	std::vector <ModelPattern> m_ModelPattern;		// ���f���\����
	std::vector <std::string> m_vName;				// �e�t�@�C����
};

#endif // !_MODEL_H_
