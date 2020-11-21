#pragma once

#include <DirectXMath.h>

class Light
{
public:
	Light();
	Light(const Light& light);
	~Light();

	void SetAmbientColor(DirectX::XMFLOAT4 color);
	void SetDiffuseColor(DirectX::XMFLOAT4 color);
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetLookAt(DirectX::XMFLOAT3 lookAt);

	DirectX::XMFLOAT4 GetAmbientColor();
	DirectX::XMFLOAT4 GetDiffuseColor();
	DirectX::XMFLOAT3 GetPosition();

	void CreateViewMatrix();
	void CreateProjectionMatrix(float screenDepth, float screenNear);

	DirectX::XMFLOAT4X4& GetViewMatrix();
	DirectX::XMFLOAT4X4& GetProjectionMatrix();

private:
	DirectX::XMFLOAT4 _ambientColor;
	DirectX::XMFLOAT4 _diffuseColor;
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _lookAt;
	DirectX::XMFLOAT4X4 _viewMatrix;
	DirectX::XMFLOAT4X4 _projectionMatrix;
};