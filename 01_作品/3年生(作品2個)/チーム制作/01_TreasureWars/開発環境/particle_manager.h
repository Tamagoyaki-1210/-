#ifndef _PARTICLE_MANAGER_H_
#define _PARTICLE_MANAGER_H_

#include "particle.h"
#include "particle_emitter.h"

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

class CParticle;
class CParticleEmitter;

class CParticleManager
{
public:
	struct SBundleData
	{
		CParticle::SInfo info;
		CParticleEmitter::SEmitterInfo emitterData;
	};

	CParticleManager();
	~CParticleManager();

	HRESULT Init();
	void ReleaseAll();
	void LoadText(std::string path);

	void SetParticleData(std::string& str, std::vector<SBundleData>& info);

	std::vector<SBundleData> GetParticleData(std::string str);
	std::map<std::string, std::vector<SBundleData>, std::less<>> GetData() { return m_data; }

private:
	// テキストファイルのデータ読み込み関係
	void ReadVector3(std::stringstream& ss, std::string name, D3DXVECTOR3& vec);
	void ReadColor(std::stringstream& ss, std::string name, D3DXCOLOR& color);
	bool ReadString(std::stringstream& ss, std::string name, std::string& tag);
	bool ReadInt(std::stringstream& ss, std::string name, int& value);
	void ReadFloat(std::stringstream& ss, std::string name, float& value);
	bool ReadBool(std::stringstream& ss, std::string name, bool& value);

	// ssの状態をクリアし、読み取り位置を先頭に戻す処理
	void ssInitialize(std::stringstream& ss) { ss.clear(), ss.seekg(0); }

	std::map<std::string, std::vector<SBundleData>, std::less<>> m_data;
};

#endif