//==============================================================================================
//
// �p�[�e�B�N������		particle.h
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
	// �p�[�e�B�N�����̗�
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
	// �u�����h�^�C�v�̗�
	//--------------------------------------
	enum EBlend
	{
		Blend_Additive,
		Blend_Subtractive,
		Blend_Normal,
		Blend_MAX
	};

	//--------------------------------------
	// �p�[�e�B�N�����̍\����
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
		D3DXVECTOR3 pos;						// �ʒu
		D3DXVECTOR3 destPos;					// �ړI�̈ʒu
		D3DXVECTOR3 posOffset;					// �ʒu�̃I�t�Z�b�g
		D3DXVECTOR3 velocity;					// �ړ���
		D3DXVECTOR2 scale;						// �傫��
		D3DXVECTOR2 scalingValue;				// �g�k��
		D3DXCOLOR col;							// �F
		D3DXCOLOR destCol;						// �ړI�̐F
		EBlend blendState;
		SRotate rotate;
		SRandom random;
		std::string particleTag;				// �p�[�e�B�N���̃^�O
		int fallDelayTime;						// ������܂ł̒x��
		int popTime;							// �����Ԋu
		float moveAttenuation;					// �ړ��̌�����
		float colAttenuation;					// �F�̌�����
		float rotateValue;						// ��]��
		float weight;							// �d��
		float altitude;							// ���x
		float circleDistance;					// �~�͈̔�
		bool useRandom;							// �����_����K�p���邩�ǂ���
		bool useTrail;							// �g���C�����g�p���邩�ǂ���
	};

	CParticle(const PRIORITY priority = PRIORITY_CENTER);
	~CParticle() override;

	static CParticle *Create(SInfo& info, std::string url);	// ��������

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw()  override;
	void Blending(EBlend state);
	bool isExpired() { return m_DestroyTime <= 0; }

	//�Z�b�^�[
	void SetInfo(SInfo& info) { m_info = info; }
	void SetPath(std::string path) { m_path = path; }

	SInfo GetInfo() { return m_info; }

private:
	void processPath();
	void DefaultBlending();
	void Additive();
	void Subtractive();

	std::string m_path;					// �e�N�X�`���̃p�X
	SInfo m_info;						// �p�[�e�B�N���̏��
	CTexture::TEXTURE m_texture;		// �e�N�X�`��
	CTrail *m_pTrail;					// �g���C��
	float m_mul;
	int m_nTime;						// ����
	int m_DestroyTime;
};

#endif