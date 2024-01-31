#include "application.h"
#include "object3d.h"
#include "particle_manager.h"

#include <algorithm>
#include <cctype>

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
//パーティクルマネージャのコンストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
CParticleManager::CParticleManager()
{
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// パーティクルマネージャのデストラクタ
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
CParticleManager::~CParticleManager()
{
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// パーティクルマネージャの初期化
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
HRESULT CParticleManager::Init()
{
	return S_OK;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// パーティクルマネージャの解放
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticleManager::ReleaseAll()
{
	m_data.clear();
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テキストファイルのデータ読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticleManager::LoadText(std::string path)
{
	std::string name = "Data/text/" + path;
	std::ifstream file(name);

	std::map<std::string, std::vector<SBundleData>, std::less<>> bundleData = {};
	std::vector<std::string> tag(32, "");
	std::string line;
	int index = 0;
	int tagIndex = 0;
	int dataIndex = 0;

	while (std::getline(file, line))
	{	// 一行ずつ読み込む
		// 空白を削除
		line.erase(std::remove_if(line.begin(), line.end(), [](char c) { return std::isspace(c); }), line.end());

		std::stringstream ss(line);

		if (ReadString(ss, "Tag", tag[tagIndex]))
		{
			dataIndex = tagIndex;
			tagIndex++;
			continue;
		}

		if (ReadInt(ss, "Index", index))														// インデックス
		{
			bundleData[tag[dataIndex]].resize(index + 1);
			continue;
		}

		bundleData[tag[dataIndex]][index].emitterData.emitterIndex = index;
		int behaviorIndex;
		int setRandomPosIndex;
		int lockPosIndex;
		int blendIndex;

		ReadBool(ss, "UseRandom", bundleData[tag[dataIndex]][index].info.useRandom);							// ランダムを使用するかどうか
		ReadVector3(ss, "RandomVelocityMin", bundleData[tag[dataIndex]][index].info.random.randomVelocityMin);	// 移動量のランダムの最低値
		ReadVector3(ss, "RandomVelocityMax", bundleData[tag[dataIndex]][index].info.random.randomVelocityMax);	// 移動量のランダムの最大値
		ReadFloat(ss, "RandomRotateMin", bundleData[tag[dataIndex]][index].info.random.randomRotate.y);			// 回転のランダムの最低値
		ReadFloat(ss, "RandomRotateMax", bundleData[tag[dataIndex]][index].info.random.randomRotate.x);			// 回転のランダムの最大値
		ReadFloat(ss, "RandomWeightMin", bundleData[tag[dataIndex]][index].info.random.randomWeight.y);			// 重さのランダムの最低値
		ReadFloat(ss, "RandomWeightMax", bundleData[tag[dataIndex]][index].info.random.randomWeight.x);			// 重さのランダムの最大値
		ReadFloat(ss, "DistortionMin", bundleData[tag[dataIndex]][index].info.random.distortion.y);
		ReadFloat(ss, "DistortionMax", bundleData[tag[dataIndex]][index].info.random.distortion.x);
		ReadInt(ss, "RandomCreate", setRandomPosIndex);															// ランダムな位置設定の列挙
		ReadInt(ss, "LockPosition", lockPosIndex);																// 位置のロック設定
		ReadFloat(ss, "CreateRange", bundleData[tag[dataIndex]][index].info.circleDistance);					// 生成位置の範囲
		ReadFloat(ss, "Altitude", bundleData[tag[dataIndex]][index].info.altitude);								// 生成位置の高度（位置のオフセットYでもよい）
		ReadVector3(ss, "PosOffset", bundleData[tag[dataIndex]][index].info.posOffset);							// 位置のオフセット
		ReadVector3(ss, "Velocity", bundleData[tag[dataIndex]][index].info.velocity);							// 移動量
		ReadFloat(ss, "VelocityAttenuation", bundleData[tag[dataIndex]][index].info.moveAttenuation);			// 移動の減衰量
		ReadInt(ss, "Behavior", behaviorIndex);																	// 挙動の列挙
		ReadVector3(ss, "Scale", bundleData[tag[dataIndex]][index].info.scale);									// 大きさ
		ReadColor(ss, "Color", bundleData[tag[dataIndex]][index].info.col);										// 色
		ReadColor(ss, "DestColor", bundleData[tag[dataIndex]][index].info.destCol);								// 目的の色
		ReadFloat(ss, "ColorAttenuation", bundleData[tag[dataIndex]][index].info.colAttenuation);				// 色の減衰量
		ReadInt(ss, "BlendType", blendIndex);
		ReadVector3(ss, "Scaling", bundleData[tag[dataIndex]][index].info.scalingValue);						// 拡縮量
		ReadFloat(ss, "Weight", bundleData[tag[dataIndex]][index].info.weight);									// 重さ
		ReadFloat(ss, "Radius", bundleData[tag[dataIndex]][index].emitterData.radius);							// 円周
		ReadFloat(ss, "RotateAngle", bundleData[tag[dataIndex]][index].info.rotate.angle);						// 螺旋状の回転量
		ReadFloat(ss, "RotateRadius", bundleData[tag[dataIndex]][index].info.rotate.radius);					// 螺旋状の半径
		ReadFloat(ss, "RandomRotateMaxBH", bundleData[tag[dataIndex]][index].info.rotate.randomMax);				// 回転量のランダムの最大値
		ReadFloat(ss, "RandomRotateMinBH", bundleData[tag[dataIndex]][index].info.rotate.randomMin);				// 回転量のランダムの最低値
		ReadInt(ss, "PopTime", bundleData[tag[dataIndex]][index].info.popTime);									// 生存時間
		ReadInt(ss, "Quantity", bundleData[tag[dataIndex]][index].emitterData.popParticleNum);					// 生成量
		ReadString(ss, "TextureTag", bundleData[tag[dataIndex]][index].emitterData.texTag);						// テクスチャのタグ
		ReadBool(ss, "Trail", bundleData[tag[dataIndex]][index].info.useTrail);									// トレイルを使用するかどうか

		bundleData[tag[dataIndex]][index].emitterData.behavior = (CParticleEmitter::EBehavior)behaviorIndex;
		bundleData[tag[dataIndex]][index].info.blendState = (CParticle::EBlend)blendIndex;
		bundleData[tag[dataIndex]][index].info.random.randomBehavior = (CParticle::ERandomBehavior)setRandomPosIndex;
		bundleData[tag[dataIndex]][index].info.random.randomLockVector = (CParticle::ELockVector)lockPosIndex;
	}

	int cnt = 0;
	std::string old_tag;
	for (size_t i = 0; i < bundleData.size(); i++)
	{
		if (tag[i] == "")
		{
			break;
		}

		if (tag[i] != old_tag)
		{
			cnt = 0;
		}

		old_tag = tag[i];
		m_data[tag[i]] = std::move(bundleData[tag[i]]);
		cnt++;
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テキストファイルのベクターの読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticleManager::ReadVector3(std::stringstream& ss, std::string name, D3DXVECTOR3& vec)
{
	std::vector<float> num;
	std::string str;
	std::string result;
	float value;

	// ：が見つかるまでの文字を抽出
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ssの状態をクリアし、読み取り位置を先頭に戻す
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);

		// 先頭から":"まで読み飛ばす
		std::getline(ss, str, ':');

		while (std::getline(ss, str, ','))
		{
			value = std::stof(str);
			num.push_back(value);
		}
	}

	if (num.size() == 3)
	{
		vec.x = num[0];
		vec.y = num[1];
		vec.z = num[2];
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テキストファイルの色の読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticleManager::ReadColor(std::stringstream& ss, std::string name, D3DXCOLOR& color)
{
	std::vector<float> num;
	std::string str;
	std::string result;
	float value;

	// ：が見つかるまでの文字を抽出
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ssの状態をクリアし、読み取り位置を先頭に戻す
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);

		// 先頭から":"まで読み飛ばす
		std::getline(ss, str, ':');

		while (std::getline(ss, str, ','))
		{
			value = std::stof(str);
			num.push_back(value);
		}
	}

	if (num.size() == 4)
	{
		color.r = num[0];
		color.g = num[1];
		color.b = num[2];
		color.a = num[3];
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テキストファイルの文字の読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool CParticleManager::ReadString(std::stringstream& ss, std::string name, std::string& tag)
{
	std::vector<float> num;
	std::string str;
	std::string result;

	// ：が見つかるまでの文字を抽出
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ssの状態をクリアし、読み取り位置を先頭に戻す
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);
		std::string str_tag;

		// 先頭から":"まで読み飛ばす
		std::getline(ss, str, ':');

		if (ss >> str_tag)
		{
			tag = str_tag;
		}
	}
	else
	{
		return false;
	}

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テキストファイルの整数の読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool CParticleManager::ReadInt(std::stringstream& ss, std::string name, int& value)
{
	std::string str;
	std::string result;

	// ：が見つかるまでの文字を抽出
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ssの状態をクリアし、読み取り位置を先頭に戻す
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);

		// 先頭から":"まで読み飛ばす
		std::getline(ss, str, ':');

		while (std::getline(ss, str, ','))
		{
			value = std::stoi(str);
		}
	}
	else
	{
		return false;
	}

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テキストファイルの浮動小数の読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticleManager::ReadFloat(std::stringstream& ss, std::string name, float& value)
{
	std::string str;
	std::string result;

	// ：が見つかるまでの文字を抽出
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ssの状態をクリアし、読み取り位置を先頭に戻す
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		std::string str_random;

		ssInitialize(ss);

		// 先頭から":"まで読み飛ばす
		std::getline(ss, str, ':');

		while (std::getline(ss, str, ','))
		{
			value = std::stof(str);
		}
	}
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// テキストファイルの真偽の読み込み
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
bool CParticleManager::ReadBool(std::stringstream& ss, std::string name, bool& value)
{
	std::string str;
	std::string result;

	// ：が見つかるまでの文字を抽出
	size_t pos = ss.str().find(":");
	if (pos != std::string::npos)
	{
		result = ss.str().substr(0, pos);
	}

	// ssの状態をクリアし、読み取り位置を先頭に戻す
	ssInitialize(ss);

	if (ss >> str && result == name)
	{
		ssInitialize(ss);
		std::string str_boolean;

		// 先頭から":"まで読み飛ばす
		std::getline(ss, str, ':');

		if (ss >> str_boolean)
		{
			value = (str_boolean == "true");
		}
	}
	else
	{
		return false;
	}

	return true;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// データを設定する処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
void CParticleManager::SetParticleData(std::string& str, std::vector<SBundleData>& info)
{
	m_data[str] = info;
}

//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
// 抽出したデータを返す処理
//＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
std::vector<CParticleManager::SBundleData> CParticleManager::GetParticleData(std::string str)
{
	if (m_data.count(str) == 0)
	{	//モデルデータが入ってなかった場合
		assert(false);
	}

	return m_data[str];
}