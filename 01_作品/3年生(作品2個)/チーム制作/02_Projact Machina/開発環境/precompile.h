//==================================================
// pre.h
// Author: hamada
//==================================================
#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

// �S�ĂŎg���w�b�_�̃C���N���[�h

//�I�����C��
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#include <Windows.h>

#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <functional>
#include <time.h>
#include <vector>
#include <stdarg.h>
#include <assert.h>

#include <thread>

#include <d3dx9.h>
#include <Xinput.h>
#define	 DIRECTINPUT_VERSION (0x0800)	// �r���h���̌x���΍��p�}�N��(���̈ʒu���瓮�����Ȃ�)
#include <dinput.h>

#include <iostream>
#include <fstream>
#include <sstream> // ������X�g���[�����g�p���邽�߂ɒǉ�
#include <array>
#include <future>
#include <algorithm>
#include <tuple>
#include <stack>
#include <map>
#include <unordered_map>

//*****************************************************************************
// ���C�u�����[�����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")			// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]�̊g�����C�u����

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include <Windows.h>
#include "d3dx9.h"							// �`�揈���ɕK�v



#endif // _PRECOMPILE_H_