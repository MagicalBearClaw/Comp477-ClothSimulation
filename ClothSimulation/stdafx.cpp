// stdafx.cpp
#include "stdafx.h"

bool isVector3AllZeros(DirectX::XMFLOAT3 vector)
{
	return vector.x == 0 && vector.y == 0 && vector.z == 0;
}

bool isVector3AllZeros(DirectX::XMVECTOR vector)
{
	DirectX::XMFLOAT3 vector3(0, 0, 0);
	DirectX::XMStoreFloat3(&vector3, vector);

	return vector3.x == 0 && vector3.y == 0 && vector3.z == 0;
}