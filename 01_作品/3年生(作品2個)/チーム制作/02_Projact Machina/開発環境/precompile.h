//==================================================
// pre.h
// Author: hamada
//==================================================
#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

// 全てで使うヘッダのインクルード

//オンライン
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
#define	 DIRECTINPUT_VERSION (0x0800)	// ビルド時の警告対策用マクロ(この位置から動かさない)
#include <dinput.h>

#include <iostream>
#include <fstream>
#include <sstream> // 文字列ストリームを使用するために追加
#include <array>
#include <future>
#include <algorithm>
#include <tuple>
#include <stack>
#include <map>
#include <unordered_map>

//*****************************************************************************
// ライブラリーリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")			// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include "d3dx9.h"							// 描画処理に必要



#endif // _PRECOMPILE_H_