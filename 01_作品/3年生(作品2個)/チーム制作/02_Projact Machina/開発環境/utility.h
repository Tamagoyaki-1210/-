//��������������������������
//Utility.h
//��������������������������
#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "application.h"
#include "camera.h"
#include <vector>
#include <random>
#include <chrono>
#include <functional>

namespace utility
{
	// 0.0f ~ 1.0f�̐��l��0 ~ 255�ɕϊ����鏈��
	template<typename T>
	inline int Convert256(T value)
	{
		if (std::is_same<T, D3DXCOLOR>::value)
		{
			value.r *= 255.0f;
			value.g *= 255.0f;
			value.b *= 255.0f;
			value.a *= 255.0f;
		}
		else
		{
			value *= 255.0f;
		}
		return static_cast<int>(value);
	}

	// ���l�������_���ɂ��鏈��
	template<typename T>
	inline T Random(T max, T min)
	{
		std::random_device seed_gen;
		std::mt19937 gen(seed_gen());
		std::uniform_int_distribution<> distribution(0, 32767);
		int randomValue = distribution(gen);
		return ((randomValue * (1.0f / 32767.0f)) * (max - min)) + min;
	}

	// ���l�������_���ɂ��鏈��(int�����݂̂̃e���v���[�g�̓��ꉻ)
	template <>
	inline int Random(int max, int min)
	{
		std::random_device seed_gen;
		std::mt19937 gen(seed_gen());
		std::uniform_int_distribution<> distribution(min, max - 1);
		return distribution(gen);
	}

	// �v�f�������_���Ɏ擾���鏈��
	template<typename T>
	inline T RandomElement(const std::vector<T>& element)
	{
		std::random_device seed_gen;
		std::mt19937 gen(seed_gen());
		std::uniform_int_distribution<size_t> distribution(0, element.size() - 1);
		return element[distribution(gen)];
	}

	// ���`��Ԃ̏���
	template<typename T>
	inline T Lerp(const T& start, const T& end, float t)
	{
		return start + (end - start) * t;
	}

	// ���l��͈͓��Ɏ��߂鏈��
	template<typename T>
	inline T Clamp(T val, T minVal, T maxVal)
	{
		return max(minVal, min(maxVal, val));
	}

	// �v�f����ӂȂ��̂ɂ��鏈��
	template<typename T>
	inline T Unique(std::vector<T>& container)
	{
		container.erase(std::unique(container.begin(), container.end(), container.end()));
	}

	// �d���Ȃ��v�f��ǉ����鏈��
	template<typename T>
	inline void AddUnique(std::vector<T>& container, const T& val)
	{
		if (std::find(container.begin(), container.end(), val) == container.end())
		{
			container.push_back(value);
		}
	}

	// �x�N�^�[�̑S�Ă̗v�f�𕶎���ɂ��鏈��
	template<typename T>
	inline std::string VectorToString(const std::vector<T>& vector)
	{
		std::string result;
		for (const auto& p : vector)
		{
			result += std::to_string(p) + " ";
		}
		return result;
	}

	// �֐��̎��s���Ԃ��v�����鏈��
	inline long long ExecutionTime(const std::function<void()>& func)
	{
		auto start = std::chrono::high_resolution_clock::now();
		func();
		auto end = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	}

	// �񎟌��O��
	inline float Vec2Cross(D3DXVECTOR3* vec1, D3DXVECTOR3* vec2)
	{
		return vec1->x * vec2->z - vec1->z * vec2->x;
	}

	// �p�x�̐��K��
	inline void AngleNormalize(float& angle)
	{
		if (angle > D3DX_PI)
		{
			angle = angle - D3DX_PI * 2;
		}
		else if (angle < -D3DX_PI)
		{
			angle = angle + D3DX_PI * 2;
		}
	}

	// �_�̐��A���ݎ��ԁA���v���Ԃɉ����Đ��l��ω������鏈��
	inline float CalcPoint(std::vector<float>& pt, const int& time, const int& total)
	{
		float interval = (float)(total / (pt.size() - 1));
		int currentIndex = static_cast<int>(time / interval);
		float t = (time - currentIndex * interval) / interval;

		return utility::Lerp(pt[currentIndex], pt[currentIndex + 1], t);
	}
};

#endif
