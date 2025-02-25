//
// StageTexture.h
//
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
	bool ExistsKey(std::string inKey) { return m_texture.count(inKey) != 0; }	// MapàÉwè³ê½Keyª¶Ý·é©Û©

	std::vector<std::string> m_texturePath;		// eNX`ÌpX
	std::unordered_map<std::string, LPDIRECT3DTEXTURE9> m_texture;	// eNX`Ìîñ
};

#endif