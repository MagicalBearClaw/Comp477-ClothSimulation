#pragma once

#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(DirectX::XMFLOAT3 postion);
	void SetRotation(DirectX::XMFLOAT3 rotation);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Update();
	DirectX::XMFLOAT4X4& GetViewMatrix();

	void UpdateBaseViewMatrix();
	DirectX::XMFLOAT4X4& GetBaseViewMatrix();

private:
	DirectX::XMFLOAT3 _position;
	DirectX::XMFLOAT3 _rotation;
	DirectX::XMFLOAT4X4 _viewMatrix;
	DirectX::XMFLOAT4X4 _baseViewMatrix;
};