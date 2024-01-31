//==============================================================================================
//
// パーティクル処理		particle.h
// Author : Tanaka Kota
//
//==============================================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "object3D.h"
#include <string>

class CTrail;

class CParticle : public CObject3D
{
public:
	//--------------------------------------
	// パーティクル情報の列挙
	//--------------------------------------
	enum ERandomBehavior
	{
		RandomBehavior_Default,
		RandomBehavior_Circle,
		RandomBehavior_MAX
	};

	enum ELockVector
	{
		Lock_None,
		Lock_X,
		Lock_Y,
		Lock_Z,
		Lock_XY,
		Lock_XZ,
		Lock_YZ,
		Lock_MAX
	};

	//--------------------------------------
	// ブレンドタイプの列挙
	//--------------------------------------
	enum EBlend
	{
		Blend_Additive,
		Blend_Subtractive,
		Blend_Normal,
		Blend_MAX
	};

	//--------------------------------------
	// パーティクル情報の構造体
	//--------------------------------------
	struct SRotate
	{
		float angle;
		float radius;
		float randomMin;
		float randomMax;
		bool useRotate;
	};

	struct SRandom
	{
		D3DXVECTOR3 randomVelocityMin;
		D3DXVECTOR3 randomVelocityMax;
		D3DXVECTOR2 randomWeight;
		D3DXVECTOR2 randomRotate;
		ERandomBehavior randomBehavior;
		ELockVector randomLockVector;
		D3DXVECTOR2 distortion;
	};

	struct SInfo
	{
		D3DXVECTOR3 pos;						// 位置
		D3DXVECTOR3 destPos;					// 目的の位置
		D3DXVECTOR3 posOffset;					// 位置のオフセット
		D3DXVECTOR3 velocity;					// 移動量
		D3DXVECTOR2 scale;						// 大きさ
		D3DXVECTOR2 scalingValue;				// 拡縮量
		D3DXCOLOR col;							// 色
		D3DXCOLOR destCol;						// 目的の色
		EBlend blendState;
		SRotate rotate;
		SRandom random;
		std::string particleTag;				// パーティクルのタグ
		int fallDelayTime;						// 落ちるまでの遅延
		int popTime;							// 生成間隔
		float moveAttenuation;					// 移動の減衰量
		float colAttenuation;					// 色の減衰量
		float rotateValue;						// 回転量
		float weight;							// 重さ
		float altitude;							// 高度
		float circleDistance;					// 円の範囲
		bool useRandom;							// ランダムを適用するかどうか
		bool useTrail;							// トレイルを使用するかどうか
	};

	CParticle(const PRIORITY priority = PRIORITY_CENTER);
	~CParticle() override;

	static CParticle *Create(SInfo& info, std::string url);	// 生成処理

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;
	void Blending(EBlend state);
	bool isExpired() { return m_DestroyTime <= 0; }

	//セッター
	void SetInfo(SInfo& info) { m_info = info; }
	void SetPath(std::string path) { m_path = path; }

	SInfo GetInfo() { return m_info; }

private:
	void processPath();
	void DefaultBlending();
	void Additive();
	void Subtractive();

	std::string m_path;					// テクスチャのパス
	SInfo m_info;						// パーティクルの情報
	CTexture::TEXTURE m_texture;		// テクスチャ
	CTrail *m_pTrail;					// トレイル
	float m_mul;
	int m_nTime;						// 時間
	int m_DestroyTime;
};

#endif