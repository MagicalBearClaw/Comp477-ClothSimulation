#pragma once
#include <Windows.h>
#include <memory>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <optional>
#include <d3d11.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_image.h>
#include <fstream>
#include <wrl.h>
#include <d3dcompiler.h>

bool isVector3AllZeros(DirectX::XMFLOAT3 vector);
bool isVector3AllZeros(DirectX::XMVECTOR vector);