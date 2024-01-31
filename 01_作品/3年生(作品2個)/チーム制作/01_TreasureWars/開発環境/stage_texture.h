//��������������������������
// StageTexture.h
//��������������������������
#ifndef _STAGE_TEXTURE_H_
#define _STAGE_TEXTURE_H_

#include "application.h"
#include <string>
#include <vector>
#include <unordered_map>

class CStageTexture
{
public:
	CStageTexture();
	~CStageTexture();

	void Release();

	LPDIRECT3DTEXTURE9 Load(std::string fileName);
	bool CheckDuplicateTex(std::string path);

private:
	bool ExistsKey(std::string inKey) { return m_texture.count(inKey) != 0; }	// Map���Ɏw�肳�ꂽKey�����݂��邩�ۂ�

	std::vector<std::string> m_texturePath;		// �e�N�X�`���̃p�X
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_texture;	// �e�N�X�`���̏��
};

#endif